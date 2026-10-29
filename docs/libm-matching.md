# EE math matching

These functions use ee-gcc 2.96 with the per-unit `sdk/libm -G0` assembler
setting described in [SDK matching](sdk-matching.md#assembler-options-for-float-constants).
The compiler remains ee-gcc 2.96; C options and assembler options are
recorded separately.

## Library call and data contracts

The C profile also uses `-G 0 -fno-builtin`. Native `atanf` calls `fabsf` at
0x002D0FE8; `__ieee754_atan2f` calls it at0x002CFCA4 before calling `atanf`.
Default builtin folding replaces that provider with `abs.s`. Disabling builtins
preserves the real call boundary without wrappers, function-pointer shims or
false declarations. Historical newlib's
[shared configuration macro](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/acinclude.m4)
adds `-fno-builtin` to `NEWLIB_CFLAGS`, and its
[math build](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/libm/math/Makefile.am)
uses those flags. This corroborates the option, not an exact bundled release.

The compiler's `-G 0` is separate from the assembler option: scalar readonly
values at0x0054B404 and0x0054B414 use native absolute address loads rather than
small-data GP-relative loads. These options preserve the existing exact floor
and scaler functions. They do not by themselves qualify the arctangent drafts.

## floorf

All216 instruction bytes at0x002D11E8 match. The input and result are scalar
floats in `$f12` and `$f0`. Separate input/output word-view blocks preserve the
[historical fdlibm conversion boundaries](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/libm/common/fdlibm.h).
The [upstream floor implementation](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/libm/math/sf_floor.c)
corroborates the integral early return, common fractional-bit clear and the
`huge + x > 0` gates. Native bytes govern the PS2 differences; these links do
not identify an exact bundled newlib release.

The mask variable is unsigned, while its shifted constant is signed, matching
the native arithmetic shift. Keep the actual bit tests: negative zero and
negative subnormals pass through; negative normal values below one become-1.
Inputs with unbiased exponent at least23 pass through, including raw exponent255
encodings. No generic IEEE NaN/infinity handling is added.

## scalbnf

All200 instruction bytes at0x002D12C0 match. Its float input/result use the
float register bank independently of the signed32-bit exponent in `$4`.
The explicit unsigned sum followed by a signed cast reproduces native32-bit
wrapping, including the extreme exponent path checked against50000.

Zero and subnormal encodings return unchanged. Adjusted exponents1..255 replace
only the exponent bits. Larger results use the signed `1.0e30f` product; results
at or below zero select that product when the original exponent exceeds50000,
otherwise the signed `1.0e-30f` product. Preserve the native `copysignf` call and
multiplication rather than substituting host IEEE overflow/underflow results.

Extract the original exponent before the zero/subnormal gate and add the
requested exponent afterward. Separate direct return expressions on the two
underflow branches keep their constants in those branches; a shared scale local
lets the compiler hoist the large constant and emits192 rather than200 bytes.
The [historical scaler](https://github.com/mirror/newlib-cygwin/blob/newlib-1_9_0/newlib/libm/common/sf_scalbn.c)
corroborates those separate returns, while the PS2 denormal/exponent limits are
established by native control flow.
