# NPC light component staging

`fldNpcLightSet` at `0x0014AE28` is exact ordinary C: 652 bytes, 163 words.
The existing native reconstruction is retained, with its two packed RGB values
staged through real `red`, `green` and `blue` locals as in DDS1's
`fldInitializeHuntSparkUnit`, `src/dds1/game/code_001411F0.c`, at
Megami-Decomps/dds-decomp commit
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. The donor's unrelated trailing
SDATA declaration migrations are not used. Existing reconstruction credit is
retained; this change closes its 38-word allocation residual.

## Actual data and provider contracts

The palette producer reads or copies 0x100 bytes. Each native eight-byte row
contains light RGB at 0..2, ambient RGB at 3..5, face-division nibbles at 6,
and direction mode at 7. The six component loads at `0x0014AFF4..0x0014B01C`
are real byte reads, not added uses. Each component is 0..255, so its signed
32-bit shift by 8 or 16 is defined; the final OR with `0x80000000` produces
the actual `u32` packed color. Once the first color is assembled, its component
locals are no longer live and can carry the second color's RGB. Both packed
colors themselves survive and are reused at the two real transition calls.

SMT3 retains its existing mode 0..3/default handling. DDS's 64-row clamp and
extended mode>=20 direction calculation are not imported. No palette type,
array bound, native callback argument, or data ownership changes.

Two inherited local declarations now agree with their matched canonical
providers in `basic/dds3ObjectBase.c`: `dds3ObjectBaseGetModelHandle` returns
`void *` (ordinary C assignment to the actual model pointer), and
`dds3ObjectBaseOnFlag` takes an `s32` mask. No provider implementation or
other owner's declaration changes.

The direction vector still initializes all four lanes at the native aligned
`sp+0` slot in a 0x50-byte frame. `evtSetUnitLightDir` consumes vf10 and changes
it through its base-light-direction lookup, so the second vector reload and
normalization are real work. The original radians expression remains intact:
ordinary compiler scheduling places multiplication before the flag/face calls
and division afterwards, as in native. Those matched callees do not change
the FP environment. A donor-style explicit multiply/divide split was tested,
proved unnecessary, and removed.

## Qualified compiler evidence

The pinned unmodified 2.96 compiler was run through `tools/cc.sh` normally
and with stock `-da` RTL dumps, using the canonical unit path. The complete
assembly is identical after normalizing only randomized `.file` scratch
prefixes, for both the matching source and the old-expression comparison.
No GDB is installed in this worker environment: this is qualified stock-RTL
evidence, not a new hooked allocator-decision trace.

In `00.rtl`, UIDs 346/349/352 load RGB into SI pseudos 93/94/95. UIDs
368/371/374 reuse those same component pseudos for the ambient row after
the first packed color has consumed them. There are no dead stores or dummy
consumers. `20.greg` reports the native homes: unit84->19, object85->16,
model86->17, row87->18, with component93->6,94->5,95->16. The old combined
expressions instead yield unit84->18, object85->17, model86->19, row87->16.
Thus the real component phases alter the global interference/allocation
context; no register is pinned or selected by source.

A bounded necessity check separates the two donor differences: explicit
radians splitting alone retains 38 differing words; RGB staging alone is
exact. Only the latter remains. No permutation grid or optimization flags
are part of the implementation.

## Qualification

The whole `field/fld_Npc` owner checks **17 match, 0 differ**, including its
existing model/texture/data peers. `ninja` reproduces retail SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. The implementation is one new
652-byte match; no existing donor or owner-peer work is credited again.
