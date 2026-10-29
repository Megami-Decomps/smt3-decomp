# Math runtime matching residuals

## __ieee754_atan2f

The reconstruction emits480 native-sized bytes:99 of120 instruction words
match,21 differ, and no bytes are uncovered or excess. Differences concern
initial register/scheduling choices, magnitude/sign calculations, two branch
delay slots and the return restore sequence. Production retains assembly;
there is no exact-match credit. All three existing math C functions stay exact.

The ABI is scalar y/x in `$f12`/`$f13` and a scalar result in `$f0`. Only the
exact +1 encoding of x takes the direct `atanf(y)` path. Both magnitude words
have explicit zero/subnormal gates: small y uses the four quadrant cases;
small x returns signed half-pi before any division. The original draft omitted
that x gate; do not replace it with an assumption about division by zero.

The general path tests signed exponent difference against61 and-60, then
computes `y/x`, calls `fabsf` and calls `atanf`. The library's documented
`-fno-builtin` option preserves those actual providers. Final quadrants return
the angle, flip its sign bit, or use the native pi/low-part correction order.
The immediate words are40490FDA,3FC90FDA and34222168. Validate literals with the
configured compiler/assembler, whose truncation differs from host rounding.
No generic infinity/NaN branch is added to the PS2 paths.

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
