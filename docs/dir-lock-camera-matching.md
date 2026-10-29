# Direction-locked field camera

`field/fldCamera::calcCamDirLock` at `0x0012D930` is 684 bytes (171
instruction words). This is a native SMT3 reconstruction using the same
existing field, camera, pose, vector and callback contracts as the newly
matched `calcCamNormal`. The latter's DDS donor provenance is recorded in
[Normal field follow camera](normal-camera-matching.md); no separate DDS
byte-identical counterpart is claimed for this direction-locked mode.

The actual 0x80-byte frame has near, reference and far vector footprints at
+0x00, +0x10 and +0x20. All three use the existing `sdfVector4_t`; the emitted
copy addresses are 16-byte aligned. Retail sets near/far W to one but does not
initialize reference W, and the reconstruction preserves that distinction.

The reference XYZ setup is retained although this mode performs no collision
query: native `0x0012DA30/34/3C` writes +0x10/+0x18/+0x14. This is surviving
dead vector setup, not invented untouched padding. Its exact copied live
shape appears in the same owner's `calcCamNormal`: global X/Z and global Y
plus the selected camera row's eye height, followed by the player's pose-matrix
getter. Normal subsequently uses that reference in collision geometry. The
ordinary DirLock source retains the stores without a new volatile qualifier,
artificial read, fake call argument or store-forcing primitive.

## Geometry and providers

Near-point construction takes the player global X/Z and eye-height-adjusted Y,
then smooths each toward the previous near point with factor 0.5. The first
scratch port flattened construction into each interpolation expression and
had 21 differing words confined to this prefix. The ordinary two-phase shape,
already established by Normal, closes the prefix without reordering unrelated
operations or changing any numerical evaluation.

The camera basic is loaded from the existing follow control at +0x78; the
unit handle comes from +0. The matched near-vector setter's recovered name is
`dds3CameraObjectBasicGetCameraTM`, but the provider really copies the supplied
position to camera work. After the real pose-matrix getter, this mode copies
`D_0054EC88` into `fldCamPitch` and computes the shared radian angle with
binary32 multiplication by 3.14 and division by 180.

Far X/Z use sine/cosine and the row distance; far Y uses the row's target
height. With collision enabled, the support-height offset interpolates from
`fldCamDist` toward `fldCamDirLock` by 0.0625 and updates the old height. With
collision disabled, both real state values reset to zero. This mode does not
perform Normal's ray query or distance limiting. It publishes the far point,
reloads its actual memory values for factor-0.5 XYZ smoothing, publishes again,
then copies the real near/far vectors to the persistent buffers. Finally it
reloads the camera's actual +0x10/+0x08 basic callback and calls
`fldCamSetLeng`. No callback ABI, field layout or shared header changes.

## Qualification

Applied after the independently qualified Normal source, the complete camera
owner passes **19 matches, 0 differences**. All three unrelated drafts compile;
the full image passes SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`.
This additive change leaves reset-camera sources and their constant-pool
issue untouched; the preceding Normal patch canonicalizes their collision
call buffer representation.

Ordinary and `-da` builds agree after normalizing only their randomized first
`.file` line. Complete normalized assembly SHA-256:
`fe7ba297d47a825eed2c6ed82016f8d21761e9919555a053a612f71204704c5f`.
The 22 target RTL dumps preserve the reference XYZ stores through final
scheduling/delay-slot filling. This is ordinary output-parity-qualified RTL,
not a hooked GDB/cc1probe trace. No type fiction, arbitrary source-order grid,
register pinning, special scratch reuse, compiler flags or compiler patch.
