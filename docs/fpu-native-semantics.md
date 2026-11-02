# FPU native semantics

`include/fpu.h` wraps four MIPS COP1 instructions in GNU inline asm:
`sqrt.s` (`fsqrtf`), `rsqrt.s` (`frsqrtf`), `abs.s` (`ffabsf`), and
`cvt.w.s`+`mfc1` (`fcvtws`). A host compiler that cannot assemble EE
instructions needs a C replacement for each. Like
[return-width mismatches](return-width-mismatches.md), the risk here is
invisible on retail hardware (the asm runs as written, unconditionally) and
only matters to a native, non-EE build: the replacement is wrong only if its
C does not reproduce what the real instruction does.

The EE's single-precision FPU is not IEEE 754. It has no denormals (a
denormal bit pattern, exponent field 0, reads as a signed zero) and no
reserved Inf/NaN encoding: exponent field 255 is an ordinary exponent, read
through the plain `(-1)^sign * (1+mantissa/2^23) * 2^(exp-127)` formula same
as any other exponent. A plain `sqrtf`, `1.0f / sqrtf`, or a plain C cast is
wrong on exactly these inputs.

This was first written from PCSX2's EE FPU interpreter, `pcsx2/FPU.cpp`
(`fpuDouble`, `checkOverflow`, `checkUnderflow`, `SQRT_S`, `RSQRT_S`,
`ABS_S`, `CVT_W`), which was the only reference available at the time.
Real-hardware results from the `ps2autotests` corpus
(`tests/cpu/ee_fpu/sqrt.expected`, `tests/cpu/ee_fpu/edge.expected`,
`tests/cpu/ee_fpu/arithmetic.expected`, `tests/cpu/ee_fpu/convert.expected`)
contradict PCSX2's model on `sqrt.s` and `rsqrt.s`; `abs.s` and `cvt.w.s`
already matched. Per `uee`'s `CONTRIBUTING.md`, a real-hardware autotest
outranks a vector derived from an emulator's source, so the sections below
describe the measured hardware behaviour, not PCSX2's, and say where the
two differ.

## The guard

The replacements live behind `NATIVE_FPU`, a compile-time macro in the same
style as `SKIP_ASM`/`M2CTX`/`PERMUTER`: the retail build never defines it, so
it keeps taking the original inline asm unchanged. Rebuilding with `ninja`
and checking `build/smt3/SLUS_209.11` against `config/smt3/checksum.sha1`
confirms the guard does not perturb retail codegen at all; this is done
before every commit that touches this header, not after.

## Per-site semantics

- **`sqrt.s`**: a zero or denormal operand always returns *positive* zero,
  sign dropped. PCSX2's model keeps the operand's sign here (a same-signed
  zero); hardware does not, confirmed by `sqrt.expected`'s
  `sqrt 80000000: 00000000` row and corroborated in `edge.expected`. A
  negative operand returns the square root of its magnitude rather than a
  NaN (there is no NaN representation); the EE's Invalid flag that this sets
  is not modeled, since nothing in this codebase reads it back through this
  helper. An exponent-255 operand is read at its literal extended value (see
  `EeFpuLiteralValue`), not clamped to `Fmax` first as PCSX2 does; its square
  root always lands back in the encodable range, so no result-side clamp is
  needed. Source of the measured behaviour: `edge.expected`'s `sqrt.s:`
  section, 20 rows, 0 unexplained mismatches.
- **`rsqrt.s`**: computes `numerator / sqrt(|x|)` at full precision; it is
  not a fast approximate reciprocal like x86's `rsqrtss`, so a native
  division and `sqrt` reproduce it rather than approximate it. A zero or
  denormal `x` (the sqrt operand, the divide-by-zero case) produces the bit
  pattern `0x7FFFFFFF`/`0xFFFFFFFF` on hardware, signed by the **numerator's**
  sign bit, not `x`'s, and regardless of the numerator's own magnitude.
  PCSX2's model signs this by `x` and clamps to `+/-Fmax` instead; both of
  those are wrong, confirmed by `sqrt.expected`'s 41-row `rsqrt:` table
  (e.g. `rsqrt 80000000/-0.00, 00000000/+0.00: ffffffff/-NaN`, signed by the
  numerator's `80000000`, not by the zero denominator) and `edge.expected`'s
  20x20 `rsqrt.s:` matrix. An overflowing quotient (one that would need
  exponent field 255 or past it) lands on that same bit pattern on hardware,
  not on `Fmax`: hardware's divide-by-zero sentinel and its arithmetic
  overflow clamp are bit-identical. An underflowing quotient flushes to a
  signed zero. `tests/cpu/vu0_macro/divsqrt.expected`'s `vrsqrt:` section
  corroborates the same sentinel-signed-by-numerator behaviour on VU0's
  divide/rsqrt unit, a separate but related EE-era FPU.
- **`abs.s`**: a bare sign-bit clear, not an arithmetic absolute value.
  Unlike every other EE FPU op it never flushes a denormal or clamps a
  maximum-exponent pattern. The replacement masks the bit directly rather
  than calling `fabsf`, which could differ under a host FPU mode that
  flushes denormal operands. Confirmed against `edge.expected` (20 rows) and
  `arithmetic.expected` (19 rows), 0 mismatches; PCSX2's model already
  matched here, so nothing changed.
- **`cvt.w.s`+`mfc1`**: always truncates toward zero, ignoring FCR31's
  rounding mode field entirely (unlike plain MIPS `CVT.W.S`); a plain C
  float-to-int cast already truncates toward zero. A magnitude at or above
  2^31, including a bit pattern with exponent field 255, saturates to
  `INT32_MAX`/`INT32_MIN` by sign instead of wrapping. `mfc1` just moves
  that 32-bit result into the integer register, which the plain `int`
  return already represents. Confirmed against `edge.expected` (20 rows) and
  `convert.expected` (32 rows, including the non-saturating truncation case
  `ceffffff(-2147483520.00) -> 80000080`), 0 mismatches; PCSX2's model
  already matched here too.

## The sentinel is not a safe native return value

Hardware's divide-by-zero and overflow result for `rsqrt.s` is the bit
pattern `0x7FFFFFFF`/`0xFFFFFFFF`. That is a measured fact about the PS2,
stated as such above, and it is not what this header's native
implementation returns.

The reason is that this header runs on the host, where IEEE 754 defines
exponent field 255 with a nonzero mantissa as a quiet NaN. `0x7FFFFFFF` is
an ordinary large finite number (about 6.8e38) on the EE, which has no NaN
encoding, but stored into a host C `float` and used in any further
arithmetic, it behaves as an actual NaN: `0.0f * that_value` is NaN on the
host, where the PS2 produces a finite result (often zero). Returning the raw
hardware bits from a native `float`-returning function would be a real
defect, not a faithful port of one: it would turn a finite PS2 computation
into a NaN avalanche on the host in exactly the situation (an rsqrt of a
zero-length vector, then multiplied against that vector) the PS2's own
finite encoding exists to avoid.

So `EeFpuEncodeExtended` and `frsqrtf`'s explicit zero-denominator branch
clamp to `+/-FLT_MAX` (`0x7F7FFFFF`/`0xFF7FFFFF`, exponent field 254) instead
of hardware's `0x7FFFFFFF`/`0xFFFFFFFF` (exponent field 255). This is a
deliberate host-implementation substitution, not a claim about what
hardware returns, and it costs correctness in two specific, named ways:

1. **Direct bit observation.** Code that reads the raw bits of a `sqrt.s`
   or `rsqrt.s` result (instead of only ever using it in further float
   arithmetic) sees `FLT_MAX`'s bits here, not hardware's
   `0x7FFFFFFF`/`0xFFFFFFFF`. See "Call-site audit" below for whether
   anything in this codebase does that.
2. **Multiplication past unit magnitude.** Hardware's sentinel is itself a
   finite value, so multiplying it by anything above magnitude 1 still
   clamps to a large-but-finite EE result. `FLT_MAX` is already the largest
   finite `float` the host can represent, so the same multiply overflows to
   host `+/-Infinity`, where real PS2 hardware would have saturated to a
   finite value instead.

Both risks are confined to the already-degenerate inputs that produce the
sentinel in the first place (a zero/denormal `rsqrt.s` denominator, or a
genuinely overflowing quotient); ordinary in-range results are unaffected,
since they never reach exponent field 254 or 255 at all.

## Call-site audit

Checked whether anything under `src/smt3/` reads a `fsqrtf`/`frsqrtf`
result's raw bits directly, rather than purely through further float
arithmetic, since that is exactly where the FLT_MAX-vs-sentinel divergence
above would become visible. Every call site in the decompilation (quaternion
construction in `sdf/sdfQuat.c`'s `LookAt`, `pipe/mnMyLink.c`'s
matrix-to-quaternion conversion, camera distance/radius math in
`pipe/pipeCamera.c`, `newbattle/nbCameraCommand.c`,
`newbattle/nbCameraSkill.c`, `field/fldCamera.c`, `field/fldSe.c`,
`field/fldHit.c`'s vector normalize, `field/fldPlayer.c`,
`newbattle/nbKoukaProcess.c`, `newbattle/nbPanelProcess.c`,
`effect/effPCPBoss.c`, and the plain-cast truncations in
`effect/effMath.c`'s `effPCPBlurScreenOffset`, `fclPanel.c`, `fclShare.c`,
and `code_0022C578.c`) either uses the result only in further float
arithmetic (add, multiply, divide, compare) or converts it to an integer
with an ordinary `(s32)` value cast, never a bit reinterpretation (no
`union`, no `(u32*)`/`(int*)` cast, no `memcpy` applied to a sqrt/rsqrt
result anywhere on these paths). So the first named risk above (direct bit
observation) is not currently live in this codebase; it is recorded here as
a known risk for future code, not a present defect.

## The normal-range residual

Both host `sqrtf` and the native divide give results within 1 ULP of
hardware on ordinary, non-saturating inputs, never more. Measured directly:
against `edge.expected`'s matrices, `sqrt.s` has 1 mismatch out of 20 rows
(`sqrt(pi)`, host and hardware a single bit apart), `rsqrt.s` has 96
mismatches out of 400 (a 20x20 matrix), and plain `div.s`, checked for
comparison since it is a single rounding instead of rsqrt's chained
sqrt-then-divide, has 10 out of 400. Every mismatch in all three, with no
exception, is exactly a 1-ULP difference in the raw bit pattern; none is
larger. This confirms the EE's sqrt/rsqrt/div hardware units are not
correctly-rounded IEEE operations, and a host-libm-based native
implementation cannot be made to match them exactly without reimplementing
the EE's own undocumented approximation algorithm bit for bit. `rsqrt.s`'s
higher mismatch rate (24% of cases vs. div's 2.5%) is consistent with
chaining two roundings (a sqrt, then a divide) instead of div's one.

This is accepted as a known limitation of the native build, not something
to chase further: it is measured, it is bounded at 1 ULP, and it only
affects ordinary in-range magnitudes, never the zero/denormal/overflow
cases above, which are exact.

One case makes the residual look larger than it is:
`rsqrt.s(nrmin=0x00800000, 1+1ulp=0x3F800001)` is hardware `0x00800000`
(nrmin, essentially unchanged) but the mathematically exact quotient is a
hair *below* nrmin, which this header's rounding correctly flushes to
`0x00000000`. The EE has no denormals, so nrmin and zero are adjacent
representable values with nothing between them; hardware's own 1-ULP
residual lands on the other side of that boundary than the exact answer
does, turning an ordinary 1-ULP difference into what looks like a full
jump in raw bits. `tools/test_fpu_native.py`'s `within_one_ulp` treats that
specific zero/nrmin jump as the same accepted 1-ULP case.

## Differential test

`tools/test_fpu_native.py` compiles `include/fpu.h` with `NATIVE_FPU`
defined through the host's own C compiler and checks every result against
literal values taken from the `ps2autotests` `.expected` files (not against
a from-scratch model of PCSX2's or any other emulator's behaviour, since
hardware is the authority here). It lives under `tools/` rather than next
to the header because that is where this repo's other `test_*.py` checks
already live, and it runs as part of the same `for test in tools/test_*.py`
loop `docs/CONTRIBUTING.md` already documents for CI.

Exact, hardware-guaranteed cases (zero/denormal operands, `abs.s`,
`cvt.w.s`, the `rsqrt.s` zero-denominator case) are checked for bit-exact
equality against the `.expected` value, except where this header
deliberately returns `FLT_MAX` instead of hardware's sentinel bits (noted
inline in the test, pointing back to this document). Ordinary in-range
`sqrt.s`/`rsqrt.s` results are checked against the `.expected` value with a
1-ULP tolerance, per the measured residual above; the tolerance is not a
hedge, it is the measured maximum.
