# Fireball head and trail matching

`effPCPAgiFireballDraw` is 2,508 retail bytes at `0x00190D38`. It adapts
`effPcpBlockSetUpdate` from Megami-Decomps/dds-decomp (redthing1),
`src/dds1/effect/effPCPMisc.c`, working snapshot based on
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. The DDS donor is 1,964 bytes;
SMT3's additional mode-one and mode-two aiming branches are reconstructed
from this title's native instructions, not assumed from DDS.

The donor's documented PCP attribution is retained: Deep Quasar, KiB,
Silver Helix, Amber, Brass Osprey, Sable Forge and Evening Notes. See the
[DDS recovery qualification](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/seven-function-recovery-qualification.md)
and its immutable PCP handoff link.

## Native algorithm and owners

The existing `0x10C` work owner and `0x50` parameter prefix are unchanged.
The constructor and clone allocate the three trail lists from count times
`TrackCount`, and the updater retains the native signed guards, unsigned
loop bounds, callback-visible reloads and entry frame/count snapshots.
`effPCPAgiFireballCopySub` can replace the live count, so the pre-call count
snapshot is meaningful rather than a redundant read.

The head is the model payload returned by `dds3EffectPartGetHandle`.
After positioning, rotation, color and `mdlLink`, mark one of that model
supplies the current position. The old position is retained for the fifth
attachment and the three randomized trail channels. The remaining node
families retain their native start, tail and fade conditions.

SMT3 obtains its invocation formation through `effBTLSkillGetInvokeFormation`
and transforms the model sphere center through `effBTLModelGetSphereCenter`.
These are pointer and VU-result providers, not DDS actor selectors. The
additional modes are:

- Mode one grounds the invocation center, places the head there, and aims
  from the grounded parameter position when battle skill context is active.
- Mode two keeps the head at the parameter position. Its aim point is the
  grounded invocation center plus a normalized direction toward the grounded
  parameter position, scaled by 300. The head scale is the resulting distance
  to the grounded parameter position divided by 400.

Inactive battle context loads the retained work matrix. Modes zero, one and
two are established by the constructors; no invented fallback matrix is added
for other values. The recovered `effPCPAgiFireballParam` symbol contains the
actual four-float up vector `(0, 1, 0, 0)` at `0x0031FA00`.

## VU and callback contracts

The four-quad matrix and five complete four-float vectors are explicitly
16-byte aligned. Their native offsets are `0x00`, `0x40`, `0x50`, `0x60`,
`0x70` and `0x80` in the `0x140` frame. All are live data buffers, not frame
padding. The position buffer is a genuine position workspace used first for
projected aiming and later for trail accumulation; the step buffer supplies
both real scaling stages.

Only existing `ps2asm.h` primitives are used. No new macro, operand, clobber,
volatile qualifier, alias type or fixed C register is introduced. The matrix
copy uses the existing load followed by the memory-clobbering store. The
copy of old/current position and the returned model mark use their existing
memory-aware operations.

The NOMEM muzzle stores feed external position/look-at consumers or subsequent
VU loads. The only C access before a consumer is the explicit Y-lane overwrite;
there is no C scalar readback or reuse of cached muzzle components. The later
NOMEM trail-position store likewise feeds a setter and the next VU iteration,
not a cached scalar read. This is a narrowly audited native hand-off, not a
claim that the NOMEM macro is a memory barrier. Native unspecified W lanes
remain unspecified; the reconstruction does not invent initialization or claim
that every effect callback ignores W.

`dds3EffectPartSetPosition`, `SetScale`, `SetRotateMatrix`, `SetColor`, `Draw`
and `Copy` dispatch through the actual effect-kind table. Work reads remain
at their native callback boundaries. `dds3GetRandFloatA` takes the existing
RNG owner and returns the low 24 random bits scaled into `[0, 1)`; native
random-call order and all arithmetic are preserved.

## Source ordering and verification

The literal native adaptation first reproduced 625 of 627 words. Its only
remaining differences were the order of two independent SP2 initialization
stores. Initializing the 300-unit scale vector before the projected position
is ordinary source ordering: both values are fully live in the following
geometry calculation. With that order, ordinary real-compiler RTL records
step-Z store UID 538 at cycle 8 and position-Z store UID 548 at cycle 9,
matching native `0x00191024` and `0x00191028`. The earlier source placed the
position-Z store first. No artificial data dependency is introduced.

Both source versions have identical plain and `-da` assembly after the
randomized first `.file` line. This is qualified ordinary RTL/output parity,
not a new hooked compiler trace. The final source reproduces all 627 words,
and the complete owner reports 386 match / 0 differ, including data and
production-context checks. All unrelated assembly fallbacks remain intact.

A fresh public-main owner at `dbbed7f69a8001cc55fedcb59944e707bf846610`
with this same body also passes 387 match / 0 differ, preserving the additional
concurrent match. That existing match receives no credit here.

```sh
python tools/check_unit.py src/smt3/effect/effPCPMisc.c -v
python tools/check_nonmatching.py src/smt3/effect/effPCPMisc.c
ninja -j4
```

The isolated production owner has zero guarded drafts, and the complete retail
image passes SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`.
The untouched project-patched ee-gcc 2.96 and normal build flags were used.
