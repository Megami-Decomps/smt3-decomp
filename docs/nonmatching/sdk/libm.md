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

## cosf and sinf

Both wrappers emit native size: cosine212 bytes with38 of53 words matching
and15 differing; sine232 bytes with43 of58 words matching and15 differing.
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
