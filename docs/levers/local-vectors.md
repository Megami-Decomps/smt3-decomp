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
