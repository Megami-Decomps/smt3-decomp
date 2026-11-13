# Matching notes: newbattle/nbMisc

## nbDrawRotatedModelX (newbattle/nbMisc, 404 bytes)

All 101 of 101 words match at the native 404-byte extent. The existing
SDF_VU0_SCALEVECTOR_BITS primitive takes the actual f32 scale through its `r`
operand at both real consumers. This lets EE GCC move its binary32
representation into the native saved GPR without a numeric conversion,
fixed binding, empty constraint, new macro or fabricated temporary. Radius
retains the original FPR helper. Native pi is binary32 0x40490FDA; 3.1415926f
reproduces that value in both EE GCC and the host compiler. The old
3.1415925f gives 0x40490FD9; the conventional longer spelling is native on EE
but rounds differently on the host.

The real four-float scratch remains a volatile sdfVector4_t initialized to
four ones. Volatile documents its sole bare-address VU0 reader; removing it
is byte-neutral and is not claimed necessary for allocation. The native
64-byte frame places the 16-byte object at sp+0. Its existing host alignment
hook remains intact. The two model submissions consume 66 and 64 vertices
from the real 66-row table and matching colors. Even counts preserve the
pairwise provider's i/i+1 accesses. All matrices, vector loads, callbacks,
scalar snapshots, model flags, packet returns and tag links remain native.

Each scale-helper reversal independently grows the target to 412 bytes,
with 20/101 native-window words matching. Restoring old pi keeps 404 bytes
and 100/101 value-aware words but changes the literal; actual linked-code
words alone cannot certify its data. The source has no new compiler aid.

## nbDrawRotatedModel (newbattle/nbMisc, 508 bytes)

All 127 of 127 words match at the native 508-byte extent. The six real
scale/radius/scale consumers in the two transform phases use the existing
GPR hardware primitive with initialized f32 values. Native radius and scale
are preserved as binary32 bits in saved GPR20/GPR16 across calls. Only their
low 32 bits feed vf2.x; no numerical integer conversion is introduced.
Each individually reverted FPR use grows this target to 516 bytes; the
all-FPR baseline emits 532 bytes with 10/127 native-prefix words matching.
X remains unchanged by these normal-model controls.

The scratch is a real volatile f32 array whose four initializers determine
its exact 16-byte extent. It is not a flat typed view of struct members.
Both existing VU loads consume only its bare address; no callback receives
that address. The array remains live through both loads and cannot alias
the caller's three input vectors. QWORD_ALIGNED is the existing host hook,
empty on EE. Actual EE output places it at sp+0 in the native 80-byte frame,
with saved registers starting at sp+16. Removing array volatility is exact.

Rotation is loaded before each ToMatrix callback, with center and position
loaded after it. The second phase reloads all three vectors after first-phase
render callbacks. Scalar parameters remain snapshots even when input vectors
alias. All vector/matrix operations, call order, model/count/flag arguments,
packet returns and tag links are unchanged. The real model tables supply
48 and 18 rows, with matching color extents and safe even-count pair accesses.
The routine's branch-free VU work is truthfully annotated; scalar calls and
addressing remain C, not handwritten scalar instruction replay.

## Combined rotated-model literal ownership

The two individually reviewed struct-based candidates cannot simply be
combined. EE GCC merges their identical aggregate literal initializers.
Both focused target checkers still pass, but the complete owner reports
31 match, 2 differ; production rodata shrinks from 96 to 80 bytes and the
full image is 16 bytes short. Actual linked text differs at 0x258A40,
0x258E44 and 0x25914C, including peer data references. Focused matches alone
are therefore insufficient.

The one-declaration normal scratch-array spelling preserves the two actual
native templates at 0x544130 and 0x544140. Qualified initial RTL refers to
separate LC0/LC1 items, while the two-struct reversal refers to shared LC0.
Each item holds the same four real float values. This records observed
pinned-compiler literal separation with truthful storage types, not an
original-source reconstruction claim or a fabricated nominal-type trick.
Independent review approves this combined-owner change specifically.

The final entire owner, linked at native addresses, reproduces 7,240 text
bytes, 96 rodata bytes and four lit4 bytes exactly. Generated imports no
longer duplicate either C-owned vector. Every production peer is exact.
SKIP_ASM adds exactly the two 16-byte templates and native pi. Retained C
needs a more precise comparison: the old guarded drafts also merged their
templates and used old pi. Final retained rodata grows from 44 to 60 bytes,
and unchanged nbSetTargetSkill's seven-entry jump table moves from +16 to +32.
Its 480-byte instruction sequence is unchanged apart from the HI16/LO16
address pair at function offsets 84/92, which follows that table by +16. All seven destinations,
table extent and 16-byte section alignment remain unchanged. Raw retained
section equality is not claimed; the native initializer and pi correction
and the preserved jump-table contract are proved explicitly.

Independent combined review checks 36 complete ordinary/dump assembly and
entire-ELF pairs across three contexts, strict declarations, all eight
single-use scalar reversals, ordinary-array volatility removal and the
failing struct reversal. Qualified stock initial RTL supports the literal
observation; there is no hooked optimizer-cause claim. Header-backed actual-body
fixtures pass 1,010,800 normal and 110,800 X cases per optimized and ASan/UBSan
mode. The normal adapter reads object-representation bytes through volatile
unsigned-char lvalues, not by casting the array to a vector struct. It checks
all 27 vector aliases, special scalar bits, callback mutations, both scratch
loads, phase reloads, native counts and returned packet links. These tests
validate bits/order/lifetimes, not VU arithmetic or gameplay execution.

The integrated four-function battle batch adds exactly 2,044 game bytes and
leaves SDK credit unchanged. Its 69 complete three-context compiler pairs
and explicit peer/data audits are carried to a later landing base only after
source, dependency and artifact identities are verified. Current combined
owner, whole-image, generated-source, tooling/layout, retained EE/IRX and
report/provenance gates are rerun. No compiler options or common headers change.

## nbDrawKoukaSprite (newbattle/nbMisc, 0x108)

Exact: 66 of 66 words, 264 bytes, ordinary C. The decimal extraction and zero
fixup are unchanged. The draw loop keeps the real digit count through its entry
test, decrements before indexing, and draws from the most significant digit to
the least. Alignment zero subtracts `n * 96`, alignment -1 subtracts `n * 192`,
and every other signed alignment leaves the origin unchanged.

Forward exits keep the center-aligned block before the right-aligned block.
Under `align != 0`, positive alignment goes directly to drawing; the negative
case exits when `align != -1` and otherwise jumps to the right-aligned block.
No one-pass block, empty case, artificial bound, unused value, constraint,
register binding, ABI change or new runtime computation remains.

The paired ordinary control changes only the final comparison to
`if (align == -1) goto right_aligned; goto draw_digits;`. It retains the same
behavior but differs in 11/66 words at the native 264-byte size. In the stock
compiler's initial `.00.rtl`, both forms have comparison instruction 92 and
forward jumps 96/101. The accepted form emits `eq` at 92, jump 96 to draw label
131, and jump 101 to right label 119. The control emits `ne` at 92 and reverses
those jump destinations. After `.02.jump`, the accepted comparison directly
targets right label 119, followed by the draw jump; the control directly targets
draw label 131. This establishes the required early block shape. It does not
claim the compiler's hidden internal decision rule or original source spelling.

Qualification used the pinned stock ee-gcc 2.96 through `tools/cc.sh`, QEMU and
the configured owner, with strict implicit-declaration diagnostics. Canonical,
accepted and control sources each have whole-owner plain versus `-da` equality
in production, `SKIP_ASM`, and `NON_MATCHING+SKIP_ASM` modes: normalized assembly,
all allocated sections, non-file symbols and relocation contracts. The accepted
owner checks 32/0; the canonical owner checks 31/0; the control checks 31/1 with
only this function differing. The two recently matched rotated-model functions
remain exact, as do every other peer and all owned data. Input hashes include
source, headers, fallback assembly, configuration, tools, compiler/assembler,
QEMU and its actual loader/runtime, with unchanged before/after closure.

Independent semantic review extracted the actual final body into an oracle and
passed 501,575 cases in both optimized and ASan+UBSan builds. Coverage includes
zero, decimal boundaries, maximum `u32`, and signed alignment extremes. Portable
signed-coordinate tests keep the initial subtraction and every x increment in
range; no claim is made about defined ISO C behavior outside that range.

### nbSetNormalSkill0 (retained nonmatching draft, native 360 bytes)

The retained C now models the native selection as a switch with initialized
selected-entry pointers and a success flag. Command zero selects normal skill
zero and calls `nbHatudoCheckSkill(work, 0x130)` before setting its target type.
Command one selects only indices 0xA4 through 0xA7. Other commands and indices
perform no writes or print call. The success flag guards the real attribute,
count and animation updates; it is not a dummy matching dependency.

With the draft enabled in isolation, the emitted extent improves from 300 to
360 bytes and 48 native words match out of 90. This is not an exact match and adds
no matching credit. Residual differences include table-address reuse and
allocation, range-test lowering, branch placement and store scheduling. The
production assembly fallback remains unchanged.

The ordinary draft preserves the provider call and subsequent work-flag read,
uses the existing truthful table types, and adds no compiler constraints.
Independent scalar-model fixtures cover all 65,536 indices for commands -1,
0, 1 and 2, both relevant flag states, positive/zero/negative provider results,
and byte-valued devil attributes/counts: 524,288 cases at O0, O2, and
ASan+UBSan, comparing all normal/visual/attribute table bytes and call counts.
These are host-model tests, not EE execution or arbitrary callback-side-effect
coverage. Leak detection was disabled because the sandbox's ptrace environment
prevents LeakSanitizer startup; address and undefined-behavior checks ran.
All 33 previously matched owner functions remain exact. Plain and stock-dump
compiler runs produce identical full raw assembly and objects in production
and SKIP_ASM contexts for both enabled drafts. The guarded final edit is also
raw assembly/object identical to the baseline in those two contexts.

An independent native-control-flow review also checks all signed command
values and post-callback mutation of command, index, ID and flags in 393,216
cases each at O0, O2 and ASan/UBSan. These retain the preselected entry while
observing the native post-call flag and ID reads. Actual provider source was
separately checked; the fixtures use mocked callbacks, not game execution.

### nbSetTargetSkill (retained nonmatching draft, native 560 bytes)

The guarded reconstruction now retains the native command-block order and
explicit target-rule bit construction. Its isolated enabled body improves
from 480 bytes / 25 matching words to the native 560 bytes / 126 of 140
matching words. It is not an exact match; the assembly fallback and matching
credit are unchanged. Remaining differences concern flag-phase allocation and
scheduling and the command-six branch's delay slot.

Two localized empty operands preserve native operations on actual initialized
values. A value of one supplies the area selector and rule bit zero, preventing
constant folding of the rule construction to an AND mask. The selected value
two in command six supplies the final rule output and retains the native branch
instead of a conditional move. Neither crosses a call or introduces memory
accesses, register bindings, dummy consumers or a historical-source claim.

The signed command dispatch, provider calls, post-item-call table reads,
signed player-count threshold, and nullable output stores in type/rule/area
order remain unchanged. Independent source/native review covers callback
mutation and aliasing. Extracted-body host tests pass 6,453,696 cases each at
O2 and ASan/UBSan, including every signed-halfword command, all rule and area
bytes, signed mode/count boundaries, callback mutations, and all 64 mappings
of the three output pointers to null or shared output locations. These are
host semantic checks, not native hardware execution or concurrency tests.

The full guarded owner retains 33 exact functions and unchanged owned data;
the independently enabled target is the only differing function. Strict
NON_MATCHING compilation succeeds. Pinned plain and diagnostic compiler runs
have byte-identical raw assembly and complete object output in production,
SKIP_ASM and NON_MATCHING+SKIP_ASM contexts. An independent instruction-level
bit oracle also passes 50,331,648 UBSan cases.

A bounded ordinary forward-edge/shared-join alternative without the command-six
operand regresses to 548 bytes. Qualified initial RTL retains the desired
branch and common area assignment, but the early jump pass replaces them with
conditional selection before scheduling. Production and SKIP_ASM plain/dump
assembly and whole objects agree. Merely spelling a forward join does not
resolve that residual; no such alternative was retained.
