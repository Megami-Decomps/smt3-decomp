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
