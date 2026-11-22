# GCC integer division runtime

## __divdi3 (sdk/libgcc_signed_divmod, 0x694)

421 of 421 words match at 1,684 bytes. The signed wrapper runs the
shared normalized word algorithm on the operands' magnitudes, then applies
the quotient sign. The native 8-byte-aligned signed pair starts at
0x002E5320, after the unchanged debug dispatch stub and its four alignment
bytes. There is one separate four-byte gap between the two signed bodies.

The original GCC negation spells a signed 32-bit unary minus on each limb.
That has signed-overflow undefined behavior when a limb is INT32_MIN,
even for valid nonzero divisors. The reconstruction uses unsigned
negation of both limbs, then subtracts a real 0/1 borrow from the upper
word. Staging the upper magnitude and borrow separately before subtraction
matters: the direct unsigned expression made GCC branch in initial RTL
expansion. Independent plain/dump output confirmed that distinction; the
defined three-step version restores exact bytes without any new compiler
constraint. EE signed conversions only preserve the final bit representation.

## __moddi3 (sdk/libgcc_signed_divmod, 0x690)

420 of 420 words match at 1,680 bytes. Remainder sign depends only on the
dividend; the helper writes the result into a real unsigned local of the
declared output type. The native minimum signed dividend divided by minus
one yields the same wrapped quotient bits and zero remainder. The native
zero-divisor trap and both signed bit-length providers stay unchanged.

Independent native decoding tested 27,801 boundary and seeded vectors per
wrapper, including 606 zero-divisor traps per wrapper. A separate host
UBSan run of the defined arithmetic verified 832,350 nonzero signed
operand pairs against a widened arithmetic oracle. These checks do not
claim portable C behavior for the intentional target trap.

## __udivdi3 (sdk/libgcc_unsigned_divmod, 0x57C)

351 of 351 words match at 1,404 bytes. The unsigned quotient wrapper and
unsigned remainder wrapper use the normalized divmod implementation adapted
from GNU GCC's October 3, 2000 snapshot:
[libgcc2.c](https://github.com/gcc-mirror/gcc/blob/380f6ad3dde6db991434068c4f3776c6762fa564/gcc/libgcc2.c)
and [longlong.h](https://github.com/gcc-mirror/gcc/blob/380f6ad3dde6db991434068c4f3776c6762fa564/gcc/longlong.h).
The upstream notices are retained in the source and shared private header; the accompanying GPL
text is already checked in at [GCC-frame-COPYING](../../licenses/GCC-frame-COPYING).

The core uses unsigned words for normalization, estimates and corrections.
It uses the historical two-output MIPS multiply only to obtain the real
high and low words of the quotient-estimate product; both feed the correction
comparison and subtraction. The approved `PS2_MULTU_WORDS` macro expresses
that arithmetic, with no added memory effect, register binding or fake
dependency. Substituting a normal widening C product and splitting it into
two words emits 1,432 and 1,408 bytes for the two wrappers instead of the
native 1,404 and 1,384. The intentional zero-divisor expression uses the
pinned EE compiler's division-trap expansion, which emits native BREAK 7
before the hardware division; it is not a portable C return value.

## __umoddi3 (sdk/libgcc_unsigned_divmod, 0x568)

346 of 346 words match at 1,384 bytes. The quotient is discarded; the
remainder is written by the same normalized helper to the wrapper's local.
All six table relocations per wrapper name their real independent 256-byte
bit-length provider in `sdk/libgcc_longlong`. That existing owner retains the four original tables and the native
debug dispatch stub. None of those definitions changed bytes. The new C unit
starts at 0x002E6048, where the native 8-byte-aligned unsigned quotient begins;
a native four-byte zero separates the two bodies. It is not counted as code.

Independent native execution checked 27,801 edge and seeded vectors for
each routine, including 606 zero-divisor traps apiece. Sharing the normalized
GNU C core with the signed pair preserves both unsigned production and
SKIP_ASM objects, their section contents and all ordered relocations.

## Earlier remainder draft (historical)

### Earlier unsigned draft

130 of 346 words match at the native1384-byte size. The draft emits1408
bytes, with24 excess bytes. At that stage, production retained assembly; the old draft earned no exact
matching credit.

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

## Earlier signed remainder draft (historical)

165 of 420 native words match. The draft emits1644 versus1680 bytes,
with246 differing overlapping words and36 uncovered bytes. At that stage, production kept
assembly; the earlier draft earned no exact matching credit.

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

### Guarded signed quotient reconstruction

The guarded signed-quotient wrapper now reconstructs the native 1,684-byte
operation using the existing normalized word-division helper with a null
remainder output. Each negative operand is converted to its unsigned magnitude
with staged high-word negation and low-word borrow. The numerator sets the sign
flag and a negative denominator complements it; the resulting quotient is
negated only when the signs differ. All word negations use unsigned arithmetic,
including the native wrapped result for the minimum signed value divided by -1.

This object uses its own existing 256-byte `libgccDivdiBitLengths` table at
`0x0054BD78`; all bytes agree with retail and their bit-length meanings. The
native zero-divisor path is preserved by the pinned compiler's conditional
`break 7` before division. Host tests exclude that trap path, rather than claiming
that division by zero is defined portable C.

The draft emits 1,704 bytes with 112/421 native instruction words matching and
remains assembly-backed. No matching credit is claimed. Existing signed
remainder, unsigned quotient and unsigned remainder draft instruction bodies
remain byte-identical. Complete-owner normal, SKIP_ASM and retained assembly and
object output agree with diagnostic-dump builds after normalizing only the
randomized source filename. Extracted optimized and ASan/UBSan fixtures pass
849,612 cases against a signed 128-bit quotient oracle; an independent reviewer
passes another 9,410,234 boundary and biased randomized cases and verifies the
native table and sign/borrow paths. These checks do not claim EE timing or trap
execution on the host.
