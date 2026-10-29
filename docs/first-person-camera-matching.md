# First-person field camera

`field/fldCamera::calcCamSyukan` at `0x0012F018` is 1,228 bytes (307
instruction words). The exact Normal camera provides the sibling source
shape; the native first-person body determines all differences. Shared DDS
family provenance is recorded in [Normal field follow camera](normal-camera-matching.md)
(Megami-Decomps/dds-decomp / redthing1, with Opus5/KiB donor analysis). No
separate byte-identical DDS counterpart is claimed for this mode.

## Native mode-specific behavior

After obtaining the existing follow and unit controls, the function reads
`fldGlobalBase.wordF8`. If set, XYZ smoothing divisors are all one; otherwise
they are 2, 4 and 2. It clears that real +0xF8 field before the pose getter.
The same divisors remain live for both near and far smoothing; they are not
reused for a different role or artificially kept alive.

Near X/Z are 30 units behind the player along the pitch direction; near Y is
player Y minus 140. Reference XYZ uses the same lowered Y but unshifted X/Z.
Far X/Z are 100 units forward and far Y is also player Y minus 140. The near
sine/cosine angle evaluations are separate, while the far pair shares its
computed radian angle, exactly as in retail. Near and far W are one.

When collision is enabled, support height is `fldCamDirLock / 5.0f`, not the
unscaled Normal value. The offset interpolates from `fldCamDist` toward that
support height with factor 0.0625, then stores the new support height. Disabled
collision resets both actual height state values to zero. The far position is
published before distance approach and collision processing.

The 1,000-unit approach to `D_0054ECA0`, XYZ ray query, length/ratio limit and
projection agree with Normal's native shape. Strict comparisons retain the
native equality and unordered no-store paths. Post-publication far XYZ reloads
at `0x0012F304/308/30C` also feed final smoothing when collision is disabled.
After final far publication, the persistent near/far vectors are copied and
the camera's actual basic callback is reloaded and called. This mode then
stores **zero** to `fldCamLength`; it does not call `fldCamSetLeng`.

## Actual vector storage and provider contract

The 0xE0-byte native frame has the same seven 16-byte vector footprints as
Normal: near +0x00, reference +0x10, far +0x20, reference/far copies +0x30/+0x40,
scaled scratch +0x50 and collision output +0x60. Near/reference/far/scaled are
the existing vector record; the input copies and collision output are actual
aligned `f32[4]` arrays, matching `fldHitChkCamera`'s canonical scalar-array
parameters. No record-to-array cursor or new alias type is used. Existing
COPYVECTOR primitives perform the real aligned quadword copies, and no
unspecified reference/collision W initialization is invented.

Retail retains the scaled XYZ calculation at `0x0012F304–0x0012F344`, with
stores +0x50/+0x54/+0x58 at `0x0012F338/33C/344`. This scratch has no later
read. It is surviving native dead vector work, corroborated by Normal's
identical stage and the live base-minus-scaled-vector projection at
`0x0012F3EC–0x0012F40C`, not unused padding. Ordinary source retains the real
stores without a fake consumer, new volatile qualification or special store
primitive. The collision call instead receives output +0x60.

## Qualification

The first native-adapted sibling compile is exact. The complete camera owner,
including Normal and DirLock, passes **20 matches, 0 differences**. All three
unrelated drafts compile and the full retail image passes SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. No shared header, provider, compiler,
option or symbol-name changes are part of this additive patch.

Ordinary and `-da` builds agree after normalizing only the randomized first
`.file` line. Complete normalized assembly SHA-256:
`caa73109666087620cc899b1f9fde6b771006dc71e3f4288daec08300458d2fd`.
All 22 target-only RTL passes are retained. This is ordinary output-parity
qualification, not a hooked GDB/cc1probe trace or a scratch-allocation claim.
