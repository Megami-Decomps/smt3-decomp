# Matching notes: effect/effMagatuhi

## effDebugDrawRectLine (effect/effMagatuhi, 0x154)

85 of 85 words match (340 bytes), with the complete owner at 80 match,
0 differ. Separate `x1`/`y1` coordinate locals recover the native saved-register
allocation. The final scheduling difference is resolved by localized register
bindings for the eight genuine arguments to the first `dds3PrimLine` call.

The initialized values are color0, depth0, x1, y1, depth1, color1, x0 and y0,
in that source order. Each binding names its actual argument register and is
consumed by the first call only; none is live across another call. Computing
the scalar copies and constants before the drawCross branch introduces no
memory access or side effect. The coordinates, branch, call order and second
line are unchanged. This is a compiler-specific allocation aid, not a claim
about the original source spelling.

Fifty initialization-order controls and nine binding-removal controls isolate
the resolution. Removing any one binding or making all eight ordinary locals
regresses; all-ordinary native-order initialization returns to 72/85. This
establishes local necessity among those controls, not global minimality.


## effDebugMoveCamera (effect/effMagatuhi, 0x628)

394 of 394 words match at 1,576 bytes with the native 0x40-byte frame.
The routine samples all four stick channels from the actual
`sdfPadAnalogSticks[8]` array, using byte 2 as the interior base and
offsets +1, 0, -1, and -2. The two right-stick values drive translation
and rotation; the left pair is loaded after the right pair's optional
rotation. The native branch delay can speculatively load left Y before
that call; the taken rotation path reloads the byte. Button 10 wins over button 8 for derived-up translation.
The last left channel stays cached across later calls. The inclusive
dead zone covers input bytes 64 through 192, excluding both endpoints
from motion. Arithmetic keeps the native binary32 multiplication order.

An inline one-byte reader lets EE GCC retain the relative load without
making the pad object opaque or volatile. The real base allocates
naturally to saved register $17. Two localized empty operands keep
the actual first byte and its widened dead-zone sum in native order.
The narrowing to u8
still happens before the comparison. These are target-specific compiler
aids, not evidence of historical source spelling; they do not synchronize
the pad buffer. The real pad provider initializes eight bytes and copies
four bytes per pad.

The native scale scratch writes XYZ and leaves W as the raw stack lane.
This is the existing address-only VU0 hand-off: the quadword load and
.xyzw operations still use W. No initialization or claim of W
unobservability is added. The rotation axis is stored as a complete,
native-aligned vector; the unchanged rotation provider consumes the
quadword by address and uses the normalized XYZ components.

The earlier guarded operand on the pointer is not needed. The byte and
sum operands have actual downstream consumers and neither pins a register.
All other candidate holds and the temporary base binding were removed.
Whole-owner checking verifies 81 functions, with all 80 prior peers and
owned data unchanged.

## effDebugDrawMenu (effect/effMagatuhi, 0x764)

473 of 473 words match at 1892 bytes in ordinary C. Separate color cases
9/10/11/12 represent actual attributes 2/4/5/6 and byte offsets 0/1/2/3;
only case 9 draws the packed-color rectangle first. Each snapshots/restores
attribute at the native point, reloads the label after SetAttr, then reloads
the value pointer/byte after the first print. Item type is not rechecked
across callbacks. Separate numeric cases 4/5 retain genuine native entries.
Direct format calls for 13/14 recognize only 0x44/0x48/0x42/6; unknown values
skip formatting and print the existing text, without an invented default.

Type 15's local u32 view of the actual s32 value recognizes exactly 0/1/2/3.
Negative representations cannot become those small values, so no domain
restriction or false field/provider type is introduced. The signed control
is 1908 bytes; changing only this local selector yields native equality-only
dispatch and 1892 bytes. Paired expansion shows signed versus unsigned range
splits, not a claimed hidden compiler decision. All 24 outer cases/defaults
and native distinct body placements remain. No constraint, binding, volatile,
extra consumer or new type recovery is needed.

The update callback and parameter dereference remain before the null-menu
test; original invalid-input behavior is not repaired. Callback-sensitive
menu/count/selection and item observations remain fresh. Control and final
complete-owner plain/dump builds independently pass normalized assembly,
allocated-section contracts, relocations and function-inventory parity.
Bounded formatting/color decision models cover selected signed32 domains and
all byte-type routes, not full callback execution, arbitrary memory or all
32-bit inputs. Exact calls/accesses provide separate native evidence.

Independent owner checking gives 79 match, 0 differ, preserving all 78 peers
and owned literals/tables. Combined retail checksum, 427 tests across 26 tooling
files, 147 guarded owners / 547 drafts, seven IRX owners and report gates passed
for source `b362bd871f355c758be545f97a43237e7f324ead`. Exact-source CI
[41078870717](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41078870717)
completed successfully, with every actual step passing and none skipped.
