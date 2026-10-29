# Orbiting breath node matching

`effPCPBreathDraw` is 592 retail bytes at `0x0018FCA8`. The reconstruction
adapts `effPcpUpdateOrbitingAimNode` from Megami-Decomps/dds-decomp (redthing1),
`src/dds1/effect/effPCPMisc.c`, using the donor working snapshot based on
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. The algorithms and VU primitives
correspond, but the battle-selector calls are title-specific.

SMT3 first obtains the invocation formation, then computes the target party's
model-group center, and finally obtains the invocation formation's sphere
center. The x/z difference determines the first-frame heading. It does not use
DDS's actor-selector masking or bounds provider. After the lifetime guard, an
optional effect node orbits the anchor, rotates, fades out and draws; the work's
frame advances even when the optional node is absent.

## Native contracts retained

- The constructor allocates the existing `0x78`-byte work record. Its matrix,
  `0x24`-byte parameter record, color, frame, angle, angular speed and node
  pointer keep their established offsets and types.
- The four-quad matrix and three VU vector buffers are explicitly 16-byte
  aligned. Together with the real x/z direction scratch, they reproduce the
  native `0xC0` frame and native scratch offsets `0x00`, `0x40`, `0x50`,
  `0x60` and `0x70` without padding objects.
- The first center and sphere stores use existing memory-clobbering VU stores
  before the C x/z reads. The subsequent direction, scale and anchor pipeline
  uses the unchanged canonical DDS/SMT3 macros. No new asm block, operand,
  clobber, volatile qualifier or fixed C register is introduced.
- The three-component matrix operation ignores the input direction's W lane.
  The radius vector's W lane and resulting position W lane are not initialized
  by the native routine, and the reconstruction does not invent a value for
  them. All four-float buffers are complete and addressable.
- The final existing `SDF_VU0_STOREVECTOR_NOMEM` writes the computed position,
  which is immediately handed by address to `dds3EffectSetPosition`. There is
  no intervening C scalar readback or reuse of cached position elements. This
  narrowly audited pipeline is not a claim that the NOMEM primitive is a
  general memory barrier. The following matrix output uses the ordinary
  memory-clobbering store before the external matrix setter.
- Fade time is signed while frame/lifetime comparisons remain unsigned where
  native uses `sltu`. The native color interpolation preserves RGB and fades
  alpha from the existing color toward its transparent counterpart.

## Qualification

The initial translated donor candidate and final source both reproduce the
complete owner: 385 functions match, with production-context and data checks.
A plain compile and ordinary `-da` compile of the candidate emit identical
assembly after the randomized first `.file` line. The measured target retains
all native vector stores and the `0xC0` stack frame. This is unhooked output
parity, not a new hooked compiler trace.

```sh
python tools/check_unit.py src/smt3/effect/effPCPMisc.c -v
python tools/check_nonmatching.py src/smt3/effect/effPCPMisc.c
ninja -j4
```

The untouched compiler is the project-patched ee-gcc 2.96. No recovered symbol,
provider ABI, compiler flag or unrelated fallback is changed.

The complete retail image checksum also passes:
`5facfbf7916836613449c922e26a4ec412207b32` (SHA-1).
