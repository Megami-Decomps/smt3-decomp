# Normal field follow camera

`field/fldCamera::calcCamNormal` at `0x0012D3D8` is 1,364 bytes (341
instruction words). The reconstruction uses the player-follow geometry and
collision flow from Megami-Decomps/dds-decomp / redthing1,
`field/fldFileResolver::fldUpdateCameraFollowPlayer`. The current donor snapshot
is `3419648e1c2ef3c9acbc55f149dd20c03a163d6c`; its
`docs/matching/player-follow-camera.md` credits Opus5's earlier draft/analysis
and KiB's semantic, vector, allocation and qualification work. The SMT3 native
body, not the donor's distinct 1,608-byte algorithm, determines this port.

## Native behavior and real storage

The native 0xC0-byte frame contains seven 16-byte vector footprints: near point
at +0x00, reference at +0x10, far point at +0x20, reference/far copies at
+0x30/+0x40, scaled scratch at +0x50 and collision output at +0x60. Near, reference, far and scaled scratch
are ordinary existing `sdfVector4_t` locals. The two collision input copies
and output are real `f32[4]` arrays with explicit 16-byte alignment, matching
the provider's scalar-array contract without a struct-to-array cursor. The emitted addresses satisfy the
16-byte alignment required by the existing `SDF_VU0_COPYVECTOR` primitive.
There is no new vector type, macro, alias, volatile qualification or frame pad.
Only near/far W is explicitly set to one, as in retail; the implementation
does not invent initializations for the other vectors' unspecified W lanes.

The scaled scratch has no later read or escaped address. Nevertheless, retail
contains its real calculation and storage: `0x0012D724–0x0012D764` computes
`near[i] - far[i] * fldCamParamTable2`, with XYZ stores at `0x0012D758`,
`0x0012D75C` and `0x0012D764` to +0x50/+0x54/+0x58. This is surviving dead
vector work, not an untouched-frame-gap or tail-call exception. The live
componentwise projection at `0x0012D80C–0x0012D82C` corroborates the same
base-minus-scaled-vector shape. The ordinary source naturally retains these
stores with the existing hardware-copy primitives; no artificial consumer or
store-forcing primitive was introduced. The source describes observed native
storage, not a claim that the scratch has a useful later result.

The collision call at `0x0012D768` instead receives far copy +0x40, reference
copy +0x30 and output +0x60. Its actual provider writes XYZ through the third
argument (`0x0013BF90/94/98`), without accessing the scaled scratch. The
current-public matched provider in `field/fldHit.c` (baseline `0f799620`)
independently documents the
three-coordinate input/output contract. Its canonical local declaration is
`s32(const f32[3], const f32[3], f32[3])`. The new call and both pre-existing
guarded reset calls use actual aligned float-array input copies and output,
with no pointer casts over struct members. This corrects their older vector
pointer declaration/storage contract without a new alias type.

SMT3-specific behavior retained here includes the 80-unit near offset, the
0x40 encounter/input gate, near smoothing divisors 3/4 or 4/8, the shared
far-angle computation, support-height interpolation, 1,000-unit distance
approach and final divisors 3/4/3 or 8/8/8. Strict comparisons preserve equality
and unordered no-store paths. SMT3 has neither the DDS camera-tilt distance
divisor nor the DDS base-length/45-unit collision fallback; neither is ported.
The donor's special cross-phase `cameraScalar` reuse is also unnecessary here.

## Providers and post-publication reloads

The existing control records supply the unit handle at +0 and follow-camera
basic at +0x78. Camera handles are loaded again at each native call phase.
`dds3CameraObjectBasicGetCameraTM` is a misleading recovered name: its matched
provider copies the supplied near vector into the camera target position.
`dds3SetPosPoseSet` publishes the far position. The pose getters retain their
real VU return behavior, and the final basic callback comes from +0x10 then
+0x08 before `fldCamSetLeng` updates the distance.

`R360_LR` really takes and returns binary32 values; retail discards its return.
`fldEncountTestFlag` takes a `u32` mask and returns the tested boolean. The input
read is signed byte 0x23. Existing field/table owners and the native four-byte
collision/input flags are used without new shared declarations or layouts.

The first source port had the exact size but 56 differing register words. It
incorrectly kept pre-publication `fx/fy/fz` scalar values across the first far
publication and vector copies. Native `0x0012D724/728/72C` reloads all three
far components. The corrected source explicitly reloads those values while
forming the scaled scratch; those same values also feed the real final
smoothing path when collision is disabled. This is a real memory/value-flow
correction, not a fake scratch consumer. The DDS donor has the corresponding
post-publication XYZ reload phase before its additional base-length work.

## Qualification

The complete owner passes **18 matches, 0 differences**; all three existing
unrelated drafts compile. The full image passes retail SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. The two existing reset calls receive only the canonical float-array cleanup;
their algorithms, constant-pool problem and nonmatching status are unchanged.

Independent ordinary and `-da` compilations agree after normalizing only the
randomized first `.file` line: normalized assembly SHA-256
`845bed92eef66453f5ce3c71db951442a3c1f995861e8be7ee20b6775e516cf3`.
The ordinary RTL preserves the three +0x50/+0x54/+0x58 stores through final
scheduling and delay-slot filling. This is output-parity-qualified ordinary
RTL evidence, not a hooked GDB/cc1probe observation. The unmodified project
compiler and assembler are used; no option or symbol-name changes are needed.
