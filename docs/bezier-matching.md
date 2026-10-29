# Bezier ribbon group update

`effPCPBezierBillDraw` at `0x0017B1D8` reconstructs 5,988 retail bytes. The
complete `effect/effPCPBezier` owner has 13 matching functions, including its
constructors, three-vector history writer and end-cap drawing consumer.

## Provenance and title-specific behavior

The implementation is adapted from redthing1 and the
[Megami-Decomps DDS decompilation](https://github.com/Megami-Decomps/dds-decomp),
`src/dds1/effect/effPCPThunder.c:effThunderUpdateCurveGroup` (donor snapshot
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`), with its audited
[Thunder Bezier notes](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/thunder-bezier-matching.md).
The instruction streams agree after relocated operands, three unannotated RNG
low-address immediates, and one actual title difference: SMT3 passes `0` to
`effBTLSkillGetTargetFormation`, whereas the DDS selector takes no argument.
SMT3's recovered symbols, parameter fields and call interfaces are retained.

The existing parameter/group/slot/history layouts remain respectively
`0x50`/`0x64`/`0x80`/`0x30` bytes. The curve begins at slot offset `0x1C`; its
index, parameter and increment are at slot offsets `0x70`/`0x74`/`0x78`.
The event handle follows at `0x7C`. No shared type or provider was changed.

The native constructor's timing and subdivision conditions remain prerequisites.
The draw path retains five control-point modes, delayed respawn, two joined
cubic segments, the history ring, alpha fade and endpoint-event update. It adds
no guards, dead stores, artificial dependencies or unused fourth-lane fills.

## VU compiler boundary

The new `_EXTENDED` variants are fixed-register forms of the existing
register-only operations. Their opcodes and implicit SDF VU register/ACC/Q
convention are unchanged. An empty extended-asm operand list represents
`ASM_OPERANDS`, rather than basic asm's `ASM_INPUT`. This is the same source
boundary documented and measured in the DDS donor, and already used by SMT3's
`SDF_VU0_SUBVECTOR_EXTENDED` and `SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED`.
No scalar register is pinned and no artificial operand or clobber is added.
Existing basic macros and all their consumers remain unchanged.

The four cap blends pass their actual float constants through the existing
`SDF_VU0_INTERPOLATE` unconstrained GPR operand, as the donor does. There is no
separate forced float-to-integer scratch operation. Stack vectors and the
placement record explicitly require 16-byte alignment; the three-vector ribbon
is one real 48-byte object and the allocated end cap contains eight quadwords.

## Scoped SDK-store/readback audit

`SDF_VU0_STOREVECTOR_NOMEM` is the existing SDK input-address-only store, not a
portable or generally sound memory barrier. Do not replace normal stores with
it globally. The donor notes document both historical Sony precedent and unsafe
conditional/loop counterexamples. This use is qualified only for this complete
native-matching pipeline:

- The conditional start-vector producer supplies the XYZ reads in both arms.
  The camera direction and normalized direction are written before use.
- Each normalized or rotated offset is read back from its real stack vector.
  The native scalar reloads survive; no stale pre-rotation values are reused.
- Each ribbon's three quadwords are written before `dds3BezierTrackPolySetVC`.
  That existing owner consumes exactly three vectors with its real ring-buffer
  wrap behavior. Copying the previous point occurs at the same call boundary.
- Tangent and side are stored before the final history call. Subsequent cap
  construction reads their actual values after that call and real vector copies.
  It writes all eight cap vectors before the drawing consumer.
- Quaternion storage writes `place.payload[4..7]` before the actual
  `effEveBEDCutSetParams` consumer; the placement remains a complete `0x30` record.
- `effPCPBezierEval` writes XYZ and homogeneous W. Other scratch W lanes retain
  the native inherited values: normalization and cross products reduce XYZ,
  and the final `effPkt3DPrimMakePoly` uses `MMI_STORE_VEC3_VALUE`, discarding W.
  No uninitialized fourth lane feeds geometric XYZ or a scalar control value.

The complete-unit comparison checks all scalar readbacks, call boundaries and
VU words against SMT3 retail, not just a standalone function's prefix. Changing
these expressions, branches, loops or consumers requires a new memory-access
audit and exact complete-owner plus retail-checksum qualification. The compiler
explanation above is donor provenance, not a claim of a new hooked SMT3 trace.
