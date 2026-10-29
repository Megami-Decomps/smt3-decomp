# Unsigned remainder runtime findings

## __umoddi3 (sdk/libgcc_longlong, 0x568)

130 of 346 words match at the native1384-byte size. The draft emits1408
bytes, with24 excess bytes. Production retains assembly; no exact
matching credit is claimed.

The wrapper returns the remainder stored by an inlined divide/modulo helper,
whose quotient is discarded. This boundary is corroborated by the historical
[GCC libgcc2 implementation](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95/gcc/libgcc2.c).
The object map independently byte-identifies the EE `_umoddi3.o` implementation;
it does not identify an exact upstream source revision.

The one-word-divisor paths normalize the divisor, estimate and correct two
base-65536 digits, then unnormalize the remainder. A larger divisor first has
the unchanged-numerator and one-subtraction cases. Its normalized path corrects
the quotient estimate's product by subtracting the divisor, subtracts that
product from the numerator with borrow, then shifts the residual back. The
optional remainder-output checks are preserved. The target compiler emits the
native conditional `break 0,7` before the zero-divisor operation.

Independent native reviews cover both divisor-width paths. A temporary host
oracle checked306 boundary pairs and650,000 deterministic randomized nonzero
cases against unsigned quotient and remainder operations, with both null and
non-null remainder outputs, including exercised quotient-product corrections. All256 leading-zero table entries were verified;
the following eight alignment bytes remain in their existing assembly import.

An isolated original-object context produces the same result, so no translation
unit split is justified by that control. Word extraction, scalar allocation and
branch layout remain unqualified; the existing quotient draft is unchanged.
