# Event-unit motion driver matching

`evtUnitManagerPrimaryFunc` is 1,560 retail bytes at `0x001E4538`. Its C adapts
`evtUpdateUnitMotion` from Megami-Decomps/dds-decomp (redthing1),
`src/dds1/event/evtUnitManager.c`, using the donor working snapshot based on
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. The donor's 1,640-byte driver has
an additional vertical-displacement vector and update for motion submode 1;
SMT3's native submodes 0 and 1 both call `evtUnitMoveType0` without that update.
The absent vector is not retained as an unused frame object.

All existing SMT3 layouts and names remain intact: the basic object's `workdata`
contains the event-unit pointer at offset eight, `evtUnit_t` supplies its motion
state and slot arrays, and `mdlObj_t` supplies its current motion and first
controller. The five-argument loop/single-motion providers and two-argument
end-status provider agree with their actual definitions in `model/mdlManager.c`.
No shared type, provider signature, compiler flag or unit boundary changes.

## Restart paths and native retained load

As in the current DDS source, idle restart distinguishes a requested motion ID
from the captured current ID. The unequal arm passes `unit->waitMotion`; the
equal arm passes `currentMotion`. These are meaningful source arguments that
become equal under the guard, not identical copied arms or an unused load added
to control code generation. The compiler later merges the paths but retains the
current-index load. Native SMT3 has exactly that trace at `0x001E464C`:
`lh $2, 0x12($20)`, immediately overwritten by the controller-pointer load.

The DDS evidence is documented under “Unit-motion manager: restore local body
knowledge and contiguous data” in its `docs/idioms.md`, including the analogous
DDS1, DDS2 and December 2002 Nocturne loads. The SMT3 source comment names the
trace, and an ordinary real-compiler RTL audit independently confirms it here:

- Initial RTL expands the current-index read into UID 235 and its sign extension
  into UID 239, which feeds the distinct-arm comparison UID 247.
- `25.sched2` still contains UID 239 and conditional jump UID 247.
- `27.jump2` removes that conditional jump while retaining UID 239.
- `29.dbr` and final assembly retain the native otherwise-dead load.

Plain and `-da` compilation emit identical assembly after the randomized first
`.file` line. This is output-parity-qualified unhooked RTL evidence, not a new
hooked trace. The untouched project compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.

The real `evtUnitMoveType0` and `evtUnitMoveTypePathTrace` definitions remain in
this owner. The current DDS audit explains why this local provider knowledge
matters to the final direction-call delay slot; no replacement wrapper or
`nothrow` annotation is used. Wobble constants are ordinary float literals,
whose emitted pool bytes match SMT3.

## Qualification

The initial native-adapted donor candidate and final production source both
match the complete owner: 45 functions, with production-context and data checks.
The remaining `UnitAnime` fallback stays assembly. Targeted draft compilation
and the full `ninja` build pass. Retail image SHA-1:
`5facfbf7916836613449c922e26a4ec412207b32`.

```sh
python tools/check_unit.py src/smt3/event/evtUnitManager.c -v
python tools/check_nonmatching.py src/smt3/event/evtUnitManager.c
ninja -j4
```
