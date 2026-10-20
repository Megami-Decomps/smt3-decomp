# sdk/libcdvd

## sceCdSyncS

112 bytes, retail's size. The structure is right: the `mode != 0` arm tail-calls
`sceSifCheckStatRpc`, the debug print is guarded on `D_0052A010 > 0`, and the
wait loop polls and sleeps 60 ticks. Only the zero return is materialised a
scheduling slot away from where retail puts it, which is the same
delay-slot-filling difference as `sceMcSync`.

## The scmd template, for the next pass

`sceCdMmode`, `sceCdGetDiskType` and their siblings share one shape, and
writing it out gets within one instruction without any searching:

    if (!_sceCd_scmd_prechk(<cmd>)) return 0;
    <optional: fill D_0052B640 and sceSifWriteBackDCache(&D_0052B640, 4)>
    if (sceSifCallRpc(D_0052BA48, <fno>, 0, <send>, <ssize>, &D_0052B200, 4, 0, 0) < 0) {
        SignalSema(D_0052A02C);
        return 0;
    }
    ret = UNCACHED(D_0052B200);
    SignalSema(D_0052A02C);
    return ret;

Two details are load-bearing. The reply word is read through the uncached
mirror, `*(volatile s32 *)((u32)&D_0052B200 | 0x20000000)`, because the IOP
wrote it behind the data cache; and `SignalSema` is written out in both the
error and the success arm, never hoisted, exactly as in libmc.

What stops these matching is the same scheduling difference as `sceMcSync` and
`sceCdSyncS`: the draft emits a `nop` in the load delay after reading the
reply and then returns, where retail sinks the return move into the
`SignalSema` call's delay slot. `sceCdGetDiskType` is 156 bytes against
retail's 152, `sceCdMmode` 192 against 200, `sceCdSync` 156 against 160. All
three are one instruction of scheduling, not structure.
