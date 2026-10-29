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

## __kernel_cosf

All336 instruction bytes match. The shared unsigned word-view macros preserve
both the magnitude extraction and the reconstructed `qx` float. Tiny inputs
return one only after the native integer-conversion gate. The correction split
uses the inclusive0x3E999999 cutoff; `qx` is0.28125 above0x3F480000 and otherwise
comes from subtracting0x01000000 from the magnitude word.

The nested Horner expression retains all six native binary32 coefficient words.
Hexadecimal float literals avoid the pinned compiler's one-bit truncation of
five earlier decimal spellings. The polynomial already contains one factor of
`z`; the later `z * r` is a second multiplication, followed by the native tail
correction. The compound expression keeps the native Horner accumulator where
successive assignments leave fourteen register-operand differences.

The actual-owner `cc1probe.sh --func __kernel_cosf --alloc --sched --no-dumps`
run passes hooked/unhooked assembly parity. Local allocation combines the
Horner intermediates at UIDs65 through88 into one quantity (reg113), selects
`$f0` for it and keeps the final scaled result distinct. No data objects,
compiler settings, extra state or register constraints are added.

## __kernel_sinf

All260 instruction bytes at0x002D0BA0 match, with no data section or target
relocation. The inputs are two floats in `$f12`/`$f13` and an independent integer
tail selector in `$4`. The tiny-input word gate and integer conversion are
preserved before the squared argument, cubic factor, Horner polynomial and
optional tail correction. All six coefficients keep their native binary32 words.

The source uses the same scoped `GET_FLOAT_WORD` conversion as the matched
floor function, followed by a separate signed magnitude mask. Keeping one signed
union alive throughout the function emits the native260 bytes but differs in38
words. The original unsigned word-view macro boundary closes all65 words under
the qualified library profile. No artificial storage, register constraint or
added operation is used.

## cosf and sinf

The cosine and sine wrappers match all212 and232 instruction bytes. Their
existing quadrant dispatches were already exact. Replacing the first scoped
word-view block with the shared `GET_FLOAT_WORD` macro, followed by the same
signed magnitude mask, closes the fifteen entry scheduling/temporary words
in each wrapper under the qualified library profile. Their union remains a
real binary32 representation transfer; there is no invented access or storage.

The magnitude cutoff is0x3F490FD8. The direct kernels receive a positive-zero
tail, with selector0 for sine. All larger words use the two-component range
reducer, including exponent255 encodings; no generic infinity/NaN gate is added.
The low two bits of the signed quadrant result select the original kernel/sign
permutation, and reduced sine calls pass selector1.

## __ieee754_atan2f

All480 instruction bytes at0x002CFB88 match. Inputs y/x use `$f12`/`$f13`, with
the scalar result in `$f0`. Only exact raw x=+1 takes the direct `atanf(y)` tail
handoff. Both magnitude words have explicit zero/subnormal gates: small y uses
the four quadrant cases, and small x returns signed half-pi before division.
Do not assume division by zero reproduces that explicit path.

Other paths test signed exponent difference against61 and-60, then call
`fabsf(y/x)` and `atanf`. The final quadrant logic preserves native sign-bit
flipping and pi/low-part correction order. The immediate words are40490FDA,
3FC90FDA and34222168; validate them with the configured toolchain rather than
host float rounding. No generic infinity/NaN branch is added.

Use the common word-conversion macros for both inputs and the result sign
flip. Group each input's `GET_FLOAT_WORD` with its immediate magnitude mask,
x first, then y. Performing both GETs before both masks leaves fourteen
instruction differences at the same480-byte size; the native grouping closes
all120 words. The earlier plain scoped unions left21 words different.

## tanf

All120 instruction bytes at0x002CF6B8 match. The shared unsigned word-view
macro followed by a signed magnitude mask reproduces the wrapper under the
qualified library profile. Magnitudes through0x3F490FDA call the tangent
kernel with a positive-zero tail and integer selector1. Larger words pass
through the two-float range reducer, then call the kernel with selector
`1 - ((quadrant & 1) << 1)`, giving1 or-1. Float inputs use `$f12`/`$f13`
independently of the integer selector in `$4`. No separate infinity/NaN gate
or alternate reciprocal operation is introduced.

## fabsf and copysignf

The sign helpers match all40 and60 native instruction bytes under the
qualified library profile. They use the existing scoped unsigned
`GET_FLOAT_WORD` / `SET_FLOAT_WORD` conversion boundaries: `fabsf` clears bit31;
`copysignf` combines the first input's low31 bits with the second input's bit31.
`fabsf` maps negative zero to positive zero; `copysignf` follows the second
input even for zero. Both retain the other representation bits and NaN
payloads, with no floating-point arithmetic or classification shortcut.

A control with persistent unsigned union views emits36/40 bytes for `fabsf`
(two of nine emitted words match) and56/60 for `copysignf` (one of fourteen
emitted words matches). The shared macros close both bodies on the first
compile in the actual owner,
including the native stack transfers and return-register move. No compiler
options, artificial state or register constraints were added. The complete
math owner has9 matches and no differences, including readonly data.


## __kernel_tanf

All644 instruction bytes match. The tiny-input path preserves the integer
conversion and the distinct zero/selector returns, including `1/fabsf(x)`.
The large-input path folds both input components around the native two-part
pi/4 constant. Thirteen assembly-owned coefficients feed the interleaved
polynomial; the tail input participates even when no angle fold occurs. The
selector and original sign determine the folded result. No extra exponent255
classification is introduced.

The compensated reciprocal clears the low twelve bits of both the tangent
and its negative reciprocal. Initialize the tangent working value with `z = w`
before reading and replacing its float word. Reading `w` directly in that
macro instead emits648 bytes with an extra float/stack transfer. The ordinary
read-modify-write sequence preserves the actual value used in compensation
and closes all161 words; the reciprocal itself retains its direct word read.
A conditional tiny-input return also preserves the native dispatch.

The actual-owner `cc1probe.sh --func __kernel_tanf --expand --cse --alloc
--no-dumps --maxlines 3500` run passes hooked/unhooked assembly parity. In its
CSE trace, UID297 assigns the tangent to the working float, UID304 reads that
float's word, UID325 masks it and UID329 writes the truncated float back.
These are consumed arithmetic values, with no artificial copies, fixed
registers or compiler-option changes.
## __ieee754_rem_pio2f

All944 instruction bytes match. The scalar input uses `$f12`; the pointer in
`$4` receives two float remainders. Small values return the input/zero pair;
the next range uses a signed single-quarter-turn split. Medium values use the
native magnitude/table check and up to three cancellation corrections, including
the exponent-loss gates at8 and25. Sign restoration negates both remainders
and the returned integer.

The large path normalizes the word, extracts three bounded float chunks into
`tx[3]`, trims trailing zero chunks and calls the real six-argument reduction
kernel. Its arguments are chunk pointer, output pointer, exponent, chunk count,
precision2 and the two-over-pi table pointer. The leading chunk is nonzero, so
the native trim loop cannot exhaust the array. No IEEE infinity/NaN shortcut
is added to this bitwise PS2 path.

The readonly tables have separate native bounds:198 integer digits at
0x0054AF60 through0x0054B274, followed by32 comparison words at0x0054B278
through0x0054B2F4. The final digit0x6B belongs to the first array. Curated
sizes0x318 and0x80 preserve both objects, including that digit and all
following scalar data. The former split at0x0054B274 was a symbol fragment,
not a source-array boundary.

The native biased address at0x0054B274 plus `n*4`, guarded by `n < 32`, is
`D_0054B278[n-1]` for the actual comparison array. Typed declarations now
use the198/32 extents and this natural index. The two arrays remain
assembly-owned; this boundary correction changes no instructions, values
or matching credit.

## Math forwarding wrappers

The ordinary C bodies at0x002CF730 (`__ieee754_acosf`) and0x002CF748
(`func_002CF748`) each match all20 instruction bytes. They forward one float
to the acos core and two floats to the atan2 implementation, respectively.
Each preserves the actual float argument/result banks and native16-byte
frame/tail-jump sequence. The intervening alignment NOPs receive no credit.
See the [native entry boundaries](sdk-matching.md#inverse-cosine-entry-boundaries).

## Inverse-cosine arithmetic body

`func_002CF760` matches all1060 instruction bytes. The two preceding
forwarding entries also have exact C bodies; see
[their boundaries](sdk-matching.md#inverse-cosine-entry-boundaries).

The signed input word and its masked magnitude preserve exact-unit,
out-of-domain, tiny-input and half-range branches. Native pi and half-pi
returns use their actual binary32 words. The small-input expression keeps
the low correction in `halfPi - (x - (low - x*r))`. Each rational expression
has six numerator and four denominator coefficients with native bit patterns.

Negative inputs at or below minus one-half compute the polynomial before the square
root call and return `pi - 2*(s + (r*s - low))`. Positive inputs at or above
one-half truncate twelve low bits from a copy of the square root, preserve
the original result for the compensation denominator and final product, and
return twice the compensated sum. Both calls use the actual float square-root
provider. No host IEEE special cases are substituted.

Use the shared `GET_FLOAT_WORD` and `SET_FLOAT_WORD` macros for representation
transfers. A pointer-pun initial extraction emits1068 bytes with broad entry
and scheduling differences; the existing scoped unsigned union boundary
restores every word under the qualified2.96 library profile. An isolated
older-compiler diagnostic also remains nonmatching; no compiler flags or
unit boundaries were changed to obtain the result.
