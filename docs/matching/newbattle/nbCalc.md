# Matching notes: newbattle/nbCalc

## nbGetVirtualAisyo (newbattle/nbCalc, 0xAC)

43 of 43 words match, at the exact 172-byte native size. Matched in
`f55fd495` with ordinary C and no compiler constraints.

A `switch` groups attributes `-1`, `12`, `13` and `14` into a `break` to
the final zero return. The default calls
`nbGetAisyo(nskill, dformindex, skillattr)` and returns the first present
flag in priority order `0x20000`, `0x40000`, `0x10000`; otherwise it also
returns zero. The source formation argument is unused in retail.

The earlier note was filed under `nbMakePacket`, but the owner is `nbCalc`.
Its 39-of-43 switch residual is historical, not the current compiler result.
In the current owner/provider context, the switch with default-arm early
returns reproduces the native dispatch, its delay slots and the prioritized
flag tests exactly. The prior range-chain draft emitted 140 bytes; assigning
a shared result in the switch emitted 172 bytes but differed in 22 words.
No new scheduling or allocation constraint is needed.

The complete owner and owned-data check passes with 57 match, 0 differ;
all eight remaining owner drafts compile. Regression tests and the EE/IRX
draft sweeps pass. The source-owned production span is independently verified,
and the full image retains retail SHA-1
`5facfbf7916836613449c922e26a4ec412207b32` with a no-op repeat build.
