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

`sceCdPowerOff` and `sceCdReadClock` read a second word at `D_0052B200 + 4`.
Retail derives it from one base (`addiu v0, s0, 4`, then the uncached `or` on
each), which an `s32 D_0052B200[4]` reproduces, but the draft then comes out
172 bytes against retail's 184 and the two loads schedule differently.
`sceCdSync` is 156 against 160: unlike `sceCdSyncS` its branch polarity is the
other way round, and inverting it makes things worse rather than better.
