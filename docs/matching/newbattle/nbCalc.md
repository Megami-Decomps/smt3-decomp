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

## nbCheckBackAttack (newbattle/nbCalc, 0x178)

94 of 94 words match at the native 376-byte size. The complete owner has
58 exact C functions with no code or owned-data difference.

The encounter table's signed byte is observed once and promoted to an `s32`
comparison value. A single localized binding keeps that value in `$4` for
the `-1` and `1` early-exit tests. It is dead before `nbGetLvAvg2`; neither
a fixed-register value nor an artificial dependency crosses a call. This
is a target-specific compiler reconstruction aid, not original source evidence.
The ordinary signed `s32` local produces 372 bytes and 30 of 93 matching words:
it leaves the incoming data argument available in `$4`, removing the native
argument restoration and changing the subsequent allocation and schedule.
Ordinary `register s32` and `register s8` controls give the same 372-byte,
30-of-93 result, so the fixed binding is not replaced by storage-class spelling.

The initial rate of 12 is assigned after acquiring the player work pointer,
before its level observation and parameter calls. Initializing it at entry
with the choice binding retained gives 376 bytes but only 55 of 94 matching
words. Both changes therefore have independent ablation evidence. Ordinary
level-to-score reuse and explicit score accumulation phases were weaker and
are not retained.

The two level/parameter scores remain diagnostic arguments, not inputs to
the encounter rate. Provider call order, signed division by two, the player
level snapshot before parameter callbacks, the random roll, the special-skill
query and the optional table divisor are preserved. The final result remains
`roll < rate`. No provider type, data layout or extra memory observation changes.

Plain and `-da` full-owner builds have identical allocated and relocation
sections (ten sections) and normalized assembly. The production compiler flags
are unchanged.
