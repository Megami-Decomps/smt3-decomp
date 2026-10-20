# sdk/libcdvd

## sceCdSyncS

112 bytes, retail's size. The structure is right: the `mode != 0` arm tail-calls
`sceSifCheckStatRpc`, the debug print is guarded on `D_0052A010 > 0`, and the
wait loop polls and sleeps 60 ticks. Only the zero return is materialised a
scheduling slot away from where retail puts it, which is the same
delay-slot-filling difference as `sceMcSync`.
