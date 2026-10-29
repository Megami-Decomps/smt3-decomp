# Math runtime matching residuals

## __ieee754_sqrtf

The guarded reconstruction emits the native 204 bytes; 17 of 51 instruction
words match. The remaining 34 words include the working-mantissa/bit register
allocation, initial mask scheduling, the sign-branch delay slot and final
packing order. The normal build retains assembly; this adds no exact SDK or
game-code credit.

Native first returns the input unchanged when its magnitude word is at most
`0x007FFFFF`, including both zero signs and all subnormals. Other negative
words take `(x - x) / (x - x)`. Positive exponent-bearing words enter the
integer algorithm even when their exponent field is 255; there is no separate
infinity or NaN propagation path. Preserve these operations and branches.

The loop starts with the hidden mantissa bit, doubles for an odd unbiased
exponent, then doubles once more. It tests the signed remainder against
`partial + bit`, subtracts accepted trials, advances the partial sum and root,
and halves `bit` from `0x01000000` to zero. A nonzero final remainder adds
`root & 1`; packing combines the root, exponent and `0x3F000000`. The source
uses binary32 unions for actual representation transfers and an unsigned
exponent shift for the final bit assembly.

A parity-qualified `cc1probe` allocation/scheduler observation on this body
(`--func __ieee754_sqrtf --cflag -DNON_MATCHING --alloc --sched --no-dumps`)
confirms that the mantissa pseudo has
21 references over 32 live instructions (priority 26250), ahead of the bit
pseudo's 13 references over 22 (17727). It receives GPR4 while the bit receives
GPR5; native uses GPR5 and GPR4 respectively. A new ordinary lifetime or
control-flow premise is needed. No register constraints or extra dependencies
were added.

Controls already ruled out: sharing the input/output union gives 10 matching
words at the same size; exchanging the independent accepted-trial updates and
staging the final exponent addition retains 17 matching words; assigning the
packed word through the working-mantissa local is inert; extracting the
exponent before the sign test gives 10 matching words. The preferred source
keeps distinct representation objects and the direct algorithm.

## cosf (sdk/libm, 0xD4)

15 of 53 words differ; 38 match. All 212 native bytes are emitted.

## sinf (sdk/libm, 0xE8)

15 of 58 words differ; 43 match. All 232 native bytes are emitted.

The same initial scheduling and mask/comparison temporary choices differ in
both, first at+8. Cosine dispatch from+48 onward and sine dispatch from+48
onward match exactly. Normal builds retain assembly; no exact credit.

Magnitude is the binary32 word with its sign bit masked. Inputs at or below
0x3F490FD8 call the corresponding kernel with positive-zero tail; the sine
flag is zero. All larger magnitudes go directly to the two-component reduction
provider, with no added infinity/NaN check. The quadrant is the signed reducer
result masked with3. Cosine uses +cos,-sin,-cos,+sin; sine uses
+sin,+cos,-sin,-cos. Reduced sine calls pass flag1. Native provider bodies
establish two floating arguments in f12/f13 and integer arguments in a0.

A scoped binary32 union preserves the actual stack representation transfer.
A direct pointer cast instead emits MFC1, shortens each body by8 bytes and
changes the prologue broadly; it was discarded. Splitting the magnitude mask
assignment or scoping the union is compiler-inert. No forced storage, artificial
reference or unsupported compiler setting was added to constrain scheduling.

The entry residual also includes RA preservation: native saves RA before the
float-word store, while the current source fills the threshold branch delay
slot with that save. The remaining dispatch bytes already match.

The existing [object audit](../../sdk-matching.md) records a relocation-masked
instruction match for sine in the 2.96 package's `libm.a/sf_sin.o`. Cosine is
a name-only association to `sf_cos.o`, and its 216-byte audit span includes
four bytes after the 212-byte function. These records do not establish the
producer's compiler invocation. A separate 2.9 control changes the float-word
transfer to `mfc1` and reduces the frame from 0x30 to 0x20; it does not close
this residual or justify a compiler change.

The named zero argument used by the [newlib sine](https://raw.githubusercontent.com/mirror/newlib-cygwin/master/newlib/libm/math/sf_sin.c)
and [cosine](https://raw.githubusercontent.com/mirror/newlib-cygwin/master/newlib/libm/math/sf_cos.c)
wrappers is compiler-inert here, as is a lexical union followed by an explicit
signed-word normalization. Their [fdlibm header](https://raw.githubusercontent.com/mirror/newlib-cygwin/master/newlib/libm/common/fdlibm.h)
supports a largest-exponent-is-normal classification that removes the
nonfinite shortcut, a source configuration consistent with native's paths.
No compiler setting was changed. Further work needs evidence for the entry
scheduling or the producer invocation.
