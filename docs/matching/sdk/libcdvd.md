# sdk/libcdvd

## What cracked the scmd family

Four rules, each measured here, took `sceCdSyncS`, `sceCdMmode`,
`sceCdGetDiskType` and `sceCdStatus` from one instruction out to exact:

- **The uncached reply read is not `volatile`.** 2.9 never moves a volatile
  access into a delay slot, and retail reads the reply word *in the delay slot
  of the `SignalSema` call*, so the original used a plain pointer. This was the
  whole of the "one instruction long, nop in the delay slot" difference that
  also shows up in `sceMcSync`.
- **`D_0052BA48` and `D_0052B1D0` are the client-data structures, not pointers
  to them.** Retail passes their address with `addiu`; declaring them `void *`
  loads a word instead, which costs an instruction and a register.
- **The fall-through arm is the one written first.** `sceCdSyncS` branches away
  on `mode != 0`, so the `mode == 0` body comes first in the source and the
  early return is the tail call.
- **One saved register holds a repeated address argument.** `sceCdMmode` passes
  `&D_0052B640` to both `sceSifWriteBackDCache` and `sceSifCallRpc`; a local
  `s32 *` keeps it in `$s2` across both, where naming the global twice
  recomputes it. The store itself still goes through the global, not the local.

## The scmd template, for the next pass

`sceCdMmode`, `sceCdGetDiskType` and their siblings share one shape, and
writing it out gets within one instruction without any searching:

    if (!_sceCd_scmd_prechk(<cmd>)) return 0;
    <optional: fill D_0052B640 and sceSifWriteBackDCache(&D_0052B640, 4)>
    if (sceSifCallRpc(D_0052BA48, <fno>, 0, <send>, <ssize>, &D_0052B200, 4, 0, 0) < 0) {
        SignalSema(D_0052A02C);
        return 0;
    }
    ret = UNCACHED(D_0052B200)[0];
    SignalSema(D_0052A02C);
    return ret;

Two details are load-bearing. The reply word is read through the uncached
mirror, `*(s32 *)((u32)D_0052B200 | 0x20000000)`, because the IOP
wrote it behind the data cache; and `SignalSema` is written out in both the
error and the success arm, never hoisted, exactly as in libmc.

`sceCdPowerOff` and `sceCdReadClock` now match 180 and 248 bytes. Both
read the status at the uncached reply base and copy payload from byte offset 4:
one word for power-off, eight clock bytes for clock-read. The shared
`s32 D_0052B200[4]` declaration preserves the native single-base addressing.
`sceCdSync` is 156 against 160: unlike `sceCdSyncS` its branch polarity is the
other way round, and inverting it makes things worse rather than better.

## Completion callback contracts and remaining drafts

The completion callback takes one signed reason and returns void; registration
returns the previous callback pointer. This agrees with the
[PS2SDK public interface](https://github.com/ps2dev/ps2sdk/blob/master/common/include/libcdvd-common.h).
The recovered setter and callback slot use that function-pointer type instead
of an integer placeholder, preserving the setter's 92 bytes and all 22
matching functions in the owner.

The completion-producer draft emits 140 of 160 bytes. Native reloads status
after storing it, twice before any call. A module-wide volatile declaration
only reaches 148 bytes and makes the retained teardown draft grow from 148 to
152 bytes, so it is not an established access contract. The thread consumer
emits 184 of 188 bytes: its final callback uses the reason-test register for
the argument, while native reloads the argument in the call delay slot and
keeps the callback in another register. Naming a typed callback snapshot does
not change that result. Changing its return type to steer those registers
would contradict the interface. Both bodies remain assembly-backed.

## sceCdStRead

392 bytes, 95 of 98 words matching. Only the stream-active flag check at
+0x58 differs: the candidate uses r3 for the address and r2 for its loaded
value; retail uses r2 and r3 respectively, including the following branch.
The remaining instructions and complete frame match. The three diagnostic
literals follow native text; production assembly retains their ownership.
The guarded whole-owner diagnostic additionally reports three shared-rodata
ownership warnings for the target's assembly fallback.

The initial diagnostic precedes sticky-error initialization. Initializing
that error at declaration moves the prologue and emits only 74 matching
words; moving initialization after the inactive guard swaps two saved
registers and leaves 88 matching words. Initializing it between diagnostic
and guard gives the preferred 95-word result. An explicit active-flag local
is inert and was removed. A positive outer guard changes the control layout
and was discarded; no further scheduling search was performed.

The inactive path returns zero without writing the status output. Mode zero
performs one read and splits its reply into sector count and error. Nonzero
mode accumulates sectors, remembers the most recent nonzero error, and
continues after a partial read even when that read reports an error. A zero
error with zero progress delays eight ticks; a nonzero error with zero
progress terminates. Each subsequent request uses remaining sectors and the
buffer offset in 2048-byte sectors. The final diagnostic precedes publishing
the sticky error. The configured SDK 2.9 register choice at the active check
remains unresolved; no dummy dependency, qualifier or primitive was added.

## cdvd_exit (sdk/libcdvd, 148 bytes)

All 37 of 37 words match in ordinary C. If the callback thread ID is nonzero,
the routine publishes signed stop reason -1 before signaling its semaphore.
It deletes the ncmd, companion and scmd semaphores in native order, disables
interrupts, and removes command handler 0x80000012. Interrupts are re-enabled
only when DIntr returned nonzero. The function returns void; its caller resets
the handles separately. No wait, thread deletion, handle reset, new guard or
failure handling is introduced.

A one-pass block around the actual reason store retains the native store-before-
semaphore-load schedule. Qualified SDK 2.9 whole-owner traces show that its
loop-end note preserves the scheduling dependency from that store to the
following semaphore address setup. The direct spelling has the same 148-byte
size but differs in five words. This is a target compiler reconstruction aid,
not evidence of the historical source spelling. No compiler constraint,
volatile storage, altered ABI, compiler flag or artificial consumer is needed.

The added semaphore declaration retains the external signed word and data
section. The command-removal declaration matches the actual void(s32) provider.
The callback reason, signal, three deletes, interrupt calls and handler target
retain their native accesses and order. Baseline, declaration-only, final and
minimal direct-control sources have strict plain/dump complete assembly and
ELF parity in production, SKIP_ASM and retained-C contexts. Declaration-only
changes are output-neutral. The complete 0x31c-byte production rodata remains
identical to retail, with no ownership change.

Independent semantic, ABI and compiler-evidence review passes. Both complete-
owner checkers report 27 exact C functions. All 35 production, 26 SKIP_ASM and
27 retained-C peers preserve their instructions and resolved relocation targets;
mode-specific data remains unchanged. The new C-only target naturally shifts
following function offsets, without changing peer behavior. Full retail,
generated-source, regression, guarded-source and report gates pass. Production
provenance verifies a unique compiler-owned 148-byte linked span, adding one
SDK function and 148 bytes while leaving game coverage unchanged.
