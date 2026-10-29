# EE math matching

These functions use ee-gcc 2.96 with the per-unit `sdk/libm -G0` assembler
setting described in [SDK matching](sdk-matching.md#assembler-options-for-float-constants).
The compiler choice and optimization flags stay the same.

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
