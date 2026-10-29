# Two local vectors: declaration order fixes the stack offsets


Local aggregates are laid out in declaration order from offset 0, each rounded
to 16 bytes, so which of two `f32 v[4]` locals lands at `0($29)` decides
whether a VU0 macro taking the address as a register operand gets a bare
`$29` or retail's `addiu $2, $29, 0x10`. One wrong order shifts the whole body
by a word: `dds3Particle_SparkBuffInit` sat at 55 matching words and 2008
bytes until `position[4]` was declared before `direction[4]`, and went to 335
matching words at the exact size on that line alone. Read the offsets in the
retail `lqc2`/`sqc2` address setups before writing the declarations.

## A live float as the scalar VU transfer input

`effPCPScreenCrackModelDraw` loads the view vector into `vf11`, then negates
the projection distance and transfers its binary32 representation through a
GPR to `vf2.x`. Use the existing `SDF_VU0_SCALEVECTOR_B_BITS(-distance)` form:
its unconstrained `r` input accepts the live float representation, and cc1
emits `mfc1` before `qmtc2`. No numeric integer conversion or fixed GPR is
needed. This float-input idiom also occurs in light interpolation.

The qualified allocation/scheduling trace verifies ordinary/probed parity.
UID 164 initially consumes SF pseudo 124 through `r`; after allocation its
input is `$2`. The separate-output scale macro instead creates an unused,
early-clobber GPR quantity with zero priority; its lifetime overlaps the next
eye-address quantity, so local allocation gives the scratch `$3`. That form
leaves two differing words at the same 588-byte size. The live-input form is
exact and adds no dead uses or artificial arithmetic.

Keep the angle difference before the atan2 call, and load `vf11` before
negating the distance. These are the native scalar and VU operation boundaries.

## The store macro's memory clobber decides whether a frame address is reused

Three uses of `&v[0]` in a row, each as a register operand of a VU0 macro,
either collapse into one callee-saved register or stay as three separate
`addiu $2, $29, off`. `SDF_VU0_STOREVECTOR` carries a `"memory"` clobber and
`SDF_VU0_STOREVECTOR_NOMEM` does not, and that is the whole difference: with
the clobber cc1 keeps the address in one register, without it each use
recomputes it. In `nbCameraState_NegoHojo_Init` the clobbering form left one
extra callee-saved value live, which renamed every register in the function;
swapping the two sphere-centre stores to the `_NOMEM` form took it from 33 to
250 of 250 words with no other change.

Two smaller levers from the same function. Local arrays, not scalars, are what
put a value in memory and give it 16-byte alignment: `nbFormation_t *p[2]` at
`0($29)` and `f32 len[2]` at `0x10($29)` leave the 8-byte gaps retail has,
which separate scalars never produce. And `u32 i` is what keeps a `for (i = 0;
i < 4; i++)` counting up: with `s32 i` gcc reverses it into a `bgez`
countdown, while the unsigned compare retail emits as `sltiu $2, $17, 0x4`
blocks the reversal.

### Preserve both sphere results in the top-view camera setter

`nbCameraShot_InvokeTargetPartyTop2_Set` matches all 720 bytes with separate
radius and height locals and two aligned stack vectors. `effBTLModelGroupGetSphere`
returns the radius in `$f0` and leaves the center in `vf10`: double the radius
and preserve that center immediately with `SDF_VU0_STOREVECTOR`. The later look
callback uses the saved center; the second vector receives the invoke group's
position through `effBTLGetGroupPos`.

The native formation scan walks all 32 records and raises the zero-initialized
maximum only on ordered strict height comparisons. Radius above 1400 or height
above 500 selects the existing fallback; the random fallback is gated by the
invoke formation's `0x100` bit. Keep both rotate checks in the final target walk:
there is a state/identity test between those two real provider calls. The
complete normal camera-skill owner checks 31 match / 0 differ.

The sibling `nbCameraShot_InvokeTargetPartyTop_Set` also matches, at 716 bytes.
Its own radius and scanned-height locals preserve their separate lifetimes.
The earlier draft compared formation indices in the final walk; retail compares
the formation pointers themselves. Its extra invoke-height fallback is
`height <= 150`, and its random fallback is unconditional after the height
scan. It uses one final rotate check per target, where Top2 uses two. The
combined normal owner checks 32 match / 0 differ.
