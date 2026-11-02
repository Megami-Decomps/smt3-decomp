# Remainder runtime findings

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

## __moddi3 (sdk/libgcc_longlong, 0x690)

165 of 420 native words match. The draft emits1644 versus1680 bytes,
with246 differing overlapping words and36 uncovered bytes. Production keeps
assembly; this reconstruction adds no exact matching credit.

The signed wrapper forms both unsigned operand magnitudes, calls the same
inlined divide/modulo primitive, and restores the numerator's sign on the
remainder. Each negation first preserves the negated high word, replaces the
low word, then subtracts its nonzero borrow from the high word. Unsigned word
arithmetic handles the minimum signed64 magnitude without signed overflow;
its remainder for a divisor of minus one is zero. Denominator sign does not
change the remainder sign. A zero divisor retains the native conditional
`break 0,7` path rather than returning a fabricated result.

The signed object references its own256-byte bit-length table, `D_0054BE78`.
The shared inline helper accepts that table separately from the unsigned
object's `D_0054C078`; their first256 bytes are equal. All assembly imports,
including the unsigned table's trailing alignment bytes, remain intact.
The existing unsigned quotient and remainder drafts emit byte-identical
instruction bodies after introducing the table parameter.

An initial low-word-first negation reread the high word after modifying the
union and emitted1652 bytes with only one matching word. Staging the real
high-word arithmetic before that write emits1644 bytes with165 matches.
A qualified actual-owner CSE/expand observation passes hooked/unhooked and
plain-output parity: CSE reuses the original high-word extraction in UID32's
negation, before the low-word replacement at UIDs43/44; UID51 subtracts the
new low word's nonzero borrow. This recovers native word-negation operations,
while frame size, scalar allocation and the normalized core remain residuals.
A temporary host oracle with undefined-behavior checks passes400,240 boundary
and deterministic random cases, including minimum signed inputs and both
divisor signs. Native sign/borrow and divmod paths were independently reviewed.

## __errno_8 (sdk/libgcc_longlong_tail, 0xC)

3 of 3 words match at 12 bytes in ordinary C. The accessor returns the address
of the actual first `_errno` member of the existing `struct _reent`, through
its global `_impure_ptr`. The incomplete tagged declaration agrees with the
canonical libc definition, whose first member is `s32 _errno`; converting
that struct pointer to `s32 *` obtains the member address. It loads the global
pointer, not the errno value. The existing `.data` declaration and native
symbol address agree. No fabricated prefix type, inline assembly, special
compiler flag or instruction replay is used.

The accessor ends at file offset 0x1E7E3C (VRAM 0x2E6E3C). The next twenty
native bytes are zero: eight bytes end native `.text` at VRAM 0x2E6E44, and
twelve precede `.vutext` at VRAM 0x2E6E50. A separate padding subsegment covers
file offsets [0x1E7E3C, 0x1E7E50). Full production regeneration removes the
old trailing assembly zeros and emits that interval exactly once; a simple
16-byte alignment would not describe all twenty bytes. This is native image
layout evidence, not an original errno archive-object boundary claim.

Independent semantic and native-layout review accepts the reconstruction.
Canonical owner checking gives 1 match, 0 differ. The regenerated full
production image passes the retail SHA-1 and direct byte comparison, followed
by a no-op Ninja build. No generic linker behavior change is required. This
adds one function and twelve bytes to SDK coverage, outside the primary
game/engine denominator; zero padding adds no function or instruction credit.

Final local tooling passes all 27 test files (436 cases), the inline-assembly
gate, and all seven IRX checks. A fresh two-worker guard run passes all
145 owners / 506 drafts with identical before/after inputs. Reconciled primary and all-code reports also pass. Source commit
`b096bc42ccb44d0f518d8a8ee95043aa271a2b1f` passed exact-source hosted
[CI run 41376898338](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41376898338),
attempt 1, including build and Pages deployment. Raw logs verify the exact
checkout and deployed commit; the run completed successfully on 2026-11-02
at 02:19:51 UTC.
