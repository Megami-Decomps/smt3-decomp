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
denormal operand or result flushes to a same-signed zero) and no true
infinities or NaNs (a maximum-exponent bit pattern reads as, and an
overflowing result saturates to, the largest finite float, `Fmax` =
`0x7F7FFFFF`/`0xFF7FFFFF`). A plain `sqrtf`, `1.0f / sqrtf`, or a plain C
cast is wrong on exactly these inputs. PCSX2's EE FPU interpreter,
`pcsx2/FPU.cpp`, is the usual software reference for these corners; the
functions used here are `fpuDouble` (sanitizing an operand), `checkOverflow`
and `checkUnderflow` (clamping a result), `SQRT_S`, `RSQRT_S`, `ABS_S`, and
`CVT_W`.

## The guard

The replacements live behind `NATIVE_FPU`, a compile-time macro in the same
style as `SKIP_ASM`/`M2CTX`/`PERMUTER`: the retail build never defines it, so
it keeps taking the original inline asm unchanged. Rebuilding with `ninja`
and checking `build/smt3/SLUS_209.11` against `config/smt3/checksum.sha1`
confirms the guard does not perturb retail codegen at all; this was done
before committing, not after.

## Per-site semantics

- **`sqrt.s`**: a zero or denormal operand returns a same-signed zero. A
  negative operand returns the square root of its magnitude rather than a
  NaN (there is no NaN representation); the EE's Invalid flag that this sets
  is not modeled, since nothing in this codebase reads it back through this
  helper. No result-side clamp is needed: the square root of any value in
  `[FLT_MIN, Fmax]` stays in that range. Source: `SQRT_S`.
- **`rsqrt.s`**: computes `numerator / sqrt(|x|)` at full precision; it is
  not a fast approximate reciprocal like x86's `rsqrtss`, so a native
  division and `sqrtf` reproduce it rather than approximate it. A zero or
  denormal `x` saturates to `+/-Fmax` signed by `x`'s own sign bit,
  regardless of the numerator, before any division happens. The quotient is
  then clamped the same way any divide result is. Source: `RSQRT_S`.
- **`abs.s`**: a bare sign-bit clear, not an arithmetic absolute value.
  Unlike every other EE FPU op it never flushes a denormal or clamps a
  maximum-exponent pattern. The replacement masks the bit directly rather
  than calling `fabsf`, which could differ under a host FPU mode that
  flushes denormal operands. Source: `ABS_S`.
- **`cvt.w.s`+`mfc1`**: always truncates toward zero, ignoring FCR31's
  rounding mode field entirely (unlike plain MIPS `CVT.W.S`); a plain C
  float-to-int cast already truncates toward zero. A magnitude at or above
  2^31, including a bit pattern that reads as +Inf/NaN, saturates to
  `INT32_MAX`/`INT32_MIN` by sign instead of wrapping. `mfc1` just moves
  that 32-bit result into the integer register, which the plain `int`
  return already represents. Source: `CVT_W`.

All four were established with high confidence directly from PCSX2's
source; none are left as a guess. The one residual approximation is using
the host's IEEE `sqrtf`/division (after sanitizing to a normal float) to
compute the normal-range result, rather than reimplementing PCSX2's
double-precision intermediate math bit for bit. Double has enough extra
mantissa bits over float that a single rounding step to float32 should match
a double-rounding one in practice; this is a standard double-rounding safety
argument, not a proof, so treat the normal-range (non-saturating) results as
the one place a future, more hostile test vector could still find a gap.

## Differential test

`tools/test_fpu_native.py` compiles `include/fpu.h` with `NATIVE_FPU`
defined through the host's own C compiler and compares every result, bit for
bit, against a second, from-scratch Python implementation of the same
PCSX2-documented behaviour. It lives under `tools/` rather than next to the
header because that is where this repo's other `test_*.py` checks already
live, and it runs as part of the same `for test in tools/test_*.py` loop
`docs/CONTRIBUTING.md` already documents for CI.

The test covers positive and negative zero, the smallest and largest
denormals, ordinary normal values, `+/-FLT_MAX`, the four bit patterns that
look like `+/-Inf`/NaN, the `cvt.w.s` saturation threshold at 2^31, and for
`rsqrt.s`, zero/denormal/negative denominators and both an overflowing and
an underflowing quotient. All four sites matched the Python model bit for
bit on every vector; `test_no_compiler_warnings` also holds the native body
to a warning-free host compile.
