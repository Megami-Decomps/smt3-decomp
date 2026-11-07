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
    if (sceSifCallRpc(&D_0052BA48, <fno>, 0, <send>, <ssize>, &D_0052B200, 4, 0, 0) < 0) {
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

## _sceCd_scmd_prechk (sdk/libcdvd, 368 bytes)

All 92 of 92 words match in the configured SDK 2.9 compiler. The corrected
RPC-client view is 0x28 bytes, ending at its server pointer at +0x24. The
canonical provider checks that same 40-byte size; native data places the
separate stream-active global at +0x28. The older 0x40 byte-array declaration
did not establish a 64-byte backing object. No unsupported tail remains.
The six-word initialization region retains its actual signed word at +12,
and existing direct users still access word zero. Declarations allocate no
storage; existing calls continue passing the same native client address.

Two initialized semaphore operands preserve the real argument load/call
boundaries. Each is immediately consumed by PollSema or SignalSema and then
dead; neither spans a call or acts as a general memory barrier. The zero mode
argument is bound to a0 only after sceCdSyncS returns, consumed immediately by
sceSifInitRpc on success, and dead on the busy path before SignalSema. No live
fixed-register value crosses a call. No extra mode operand, fabricated
consumer or instruction-emitting helper remains. These are target compiler
reconstruction aids, not evidence of historical spelling.

The post-PollSema semaphore reload, positive-debug failure logging, command
publication, thread-status call, sync failure cleanup, signed initialization
state, service ID 0x80000593 and bind retries retain their native accesses and
call order. A server pointer is read only after a nonnegative bind result.
Both bounded delays decrement 0x100000 through -1 without overflow; the four
NOPs are emitted by the configured compiler, not spelled as padding in source.
Success clears the initialization word and deliberately retains the semaphore.

Fresh corrected controls reproduce the independently reviewed results:
- Baseline and declaration-only owners: 27 match, 0 differ.
- Final: 368 bytes, 92/92 words matching; owner 28 match, 0 differ.
- Removing only the PollSema operand: 360 bytes, 14/90 overlap words matching.
- Removing only the SignalSema operand: 360 bytes, 38/90 matching.
- Removing only the mode binding: 368 bytes, 84/92 matching.
- Removing all three aids: 360 bytes, 15/90 matching.
Each 360-byte control leaves eight native bytes uncovered. Qualified RTL,
scheduler and delay-branch outputs support the narrow argument-boundary
explanation: semaphore ablations permit their loads in call delay slots;
mode ablation moves the busy-path load into the post-sync branch delay slot.

All seven complete sources have fresh production, SKIP_ASM and retained-C
plain/dump, implicit-declaration-strict and full-Werror builds: 84 clean
compilations and 21 whole-output sets. Complete assembly and entire ELF agree
after replacing exactly one equal-length generated source filename. No code,
relocation, data, symbol or debug payload is removed. Corrected owner layout
assertions pass under the actual SDK compiler. The outputs also reproduce
the independent review's artifacts.

All 35 production / 27 SKIP / 28 retained peers preserve instruction bytes
and paired resolved targets. Allocated data and semantic symbol contracts
remain equal after the explicit target-string mapping: production removes
only its two target-owned labels without changing bytes; SKIP modes insert
exactly the target's 64-byte string block at rodata offset 32. All single
ablations preserve peers and nontext data. Retained sceCdStRead remains a
known mismatch and receives no new credit.

Raw relocation serialization is not identical. Production changes the order
in _sceCd_ncmd_prechk; both SKIP modes change order in sceCdInitEeCB,
sceCdMmode, sceCdGetDiskType, sceCdStatus and sceCdPowerOff. Their per-offset
resolved targets and code bytes agree. Independently linking baseline,
declarations and final gives exact unmasked 7,016-byte text and 796-byte
rodata against retail, including the 368-byte target. This is resolved-contract
preservation and exact final bytes, not ordered-record identity.

Full current-tree retail/generated-source checks, 505 no-skip tooling tests,
818 layout claims, frozen EE/IRX draft sweeps, inline policy and report-scope
checks pass. Production provenance verifies one compiler-owned target and
no target fallback input. The three upstream field/event/vector commits
through a5cfb79d are preserved and their affected owners are rechecked.
This promotes one verified SDK C function of 368 bytes; game coverage remains
unchanged. The current production reconciler only processes game units. The
SDK report therefore retains raw objdiff's 99.78261% for this target and leaves
its matched-function/byte totals unchanged despite exact linked production
bytes. No SDK dashboard increase is claimed, and report tooling is unchanged.

## _sceCd_ncmd_prechk (sdk/libcdvd, 368 bytes)

All 92 of 92 words match for the N-command precheck under the configured SDK
2.9 compiler. This is a bounded transfer of the independently qualified
S-command sibling's three localized reconstruction aids, with N's actual
semaphore, command, client, signed initialization word, service 0x80000595
and sceCdSync provider. The complete owner increases from 28 to 29 exact C
functions with zero differences. Historical source spelling is not established.

Each empty operand constraint carries an initialized, freshly loaded semaphore
to its actual PollSema or SignalSema consumer and is then dead. It is not a
general memory barrier. The zero RPC-mode argument is bound to a0 only after
sceCdSync returns, immediately consumed by sceSifInitRpc on success, and dead
before the busy path loads its actual semaphore argument. No fixed-register
value remains live across a clobbering call. These are target-specific compiler
reconstruction aids; no padding helper, fabricated consumer or storage is added.

The existing RPC-client type models the real 0x28-byte provider object, with
server at +0x24. N's native label spans 0x30 bytes, not the previously declared
0x40-byte array. No speculative tail is accessed. Existing call sites pass
its address explicitly without changing that address or ABI. The signed
initialization alias names +8 in the verified 12-byte D_0052A030 region,
initially -1, without indexing beyond a scalar object or allocating storage.

Failed acquisition reloads the semaphore after PollSema, logs only for positive
debug level and returns zero without signaling. Success publishes the command
before ReferThreadStatus and calls the actual N sync provider. Unlike the S
provider, sceCdSync(1) short-circuits on the busy word and returns Boolean 0/1.
Any nonzero result signals one freshly reloaded semaphore and returns zero.
Otherwise RPC initialization runs; negative initialization binds until a
nonnegative result and nonnull server, then clears only the initialization
word. Negative bind status ignores the server even if nonnull. Success returns
one while deliberately retaining the semaphore for the caller transaction.
Both native countdowns decrement 0x100000 through -1, exactly 0x100001 times,
without signed overflow. The four NOPs are compiler-emitted, not source padding.

Fresh full-owner controls reproduce the reviewed causal results:
- Baseline and declaration-only: 28 matches, zero differences.
- Complete candidate: 368 bytes, 92/92 words; 29 matches, zero differences.
- Remove PollSema operand: 360 bytes, 14/90 covered words.
- Remove SignalSema operand: 360 bytes, 38/90 covered words.
- Remove mode binding: 368 bytes, 84/92 words.
- Remove all three aids: 360 bytes, 15/90 covered words.
Each 360-byte control leaves eight retail target bytes uncovered. Ordinary and
no-Poll controls also retain their checker diagnostic about 0x174 rodata bytes
with no instruction reference. They are diagnostic controls, not accepted
matches. RTL, scheduler and delay-branch outputs preserve the narrow argument
boundary explanation: removing a semaphore operand permits its load in the
call delay; removing the mode binding moves the busy-path load into the
post-sync branch delay.

All seven complete sources are rebuilt in production, SKIP_ASM and retained-C
contexts, using plain, -da and full-Werror plus implicit-declaration-strict
options: 63 clean compilations, 21 entire assembly/ELF parity sets. Only exactly
one equal-length generated or canonical source filename is normalized. No code,
data, symbol, relocation or debug payload is discarded. Compiler-pass archives
are lossless and roundtrip hash-verified. Baseline, declaration-only and final
production owners independently link to all 7,016 retail text bytes and 796
rodata bytes without masking.

All 35 production, 28 SKIP and 29 retained peers preserve instruction bytes
and resolved relocation contracts. HI/LO pairs are resolved in original ELF
serialization order before comparison. Production raw peer relocation order
also agrees. SKIP and retained raw order differs for sceCdSync, cdvd_exit and
sceCdNcmdDiskReady; resolved per-offset contracts agree. This is explicitly
not raw ordered identity. Allocated non-target data/symbol contracts agree
after mapping the expected 64-byte target string block inserted at rodata zero
in nonproduction modes. Production data matches retail exactly. The retained
sceCdStRead mismatch is unchanged and receives no credit.

Independent review approves the exact frozen source. Fresh source-extracted
fixtures pass all 1,080 cases with the semaphore changed to 41 inside sync,
including optimized and ASan/UBSan host executables. This checks call order,
arguments, semaphore reloads, publication, signed initialization, debug and
negative/null/success retry paths. It is not EE/IOP, concurrent-interrupt,
ABI/layout, timing or hardware execution evidence. Host optimization may remove
the empty delays; the actual native bounds/instructions are separately exact.

Current-tree qualification passes all 35 owner checks, 505 no-skip tooling
tests, 818 layout assertions, 141 guarded EE owners, seven IRX modules,
inline policy, serialized generated-source positive/negative controls and
full retail SHA-1/byte comparison. Final Ninja is a no-op. Production provenance
proves a unique compiler-owned 368-byte target without target fallback input.
The published libkernl TLB/cache trio and all prior claims/notes are preserved.
This adds one verified SDK C function and 368 instruction bytes; game source
coverage remains unchanged. Raw report-category changes are measured separately
and are not inferred from that source credit. Exact-commit CI follows publication.

The fresh raw SDK report gives this target 99.78261%, so matched totals receive
no new credit: SDK remains 71,540 bytes / 409 functions (delta zero / zero).
Its fuzzy category score does rise from 60.379163% to 60.672895%; the libcdvd
fuzzy score rises from 52.690094% to 57.97811%, while that owner's raw matched
totals remain 3,132 bytes / 26 functions. Only the target's function report row
changes; every peer row and other report unit is unchanged. As for the S-command
sibling, the current production reconciler only processes game units. The
unique compiler-owned linked target is nevertheless all 368 retail bytes,
and the complete native owner and full image are exact. This separates verified
SDK source credit from raw report matched totals; reporting tools are unchanged.
