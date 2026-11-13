# Matching notes: fclPanel

## fclDrawPanel (fclPanel, 0x54)

21 of 21 words match at the native 84-byte size. Two localized empty constraints
resolve independent scheduling groups without changing the truthful
`void fclDrawPanelSprite(s32, s32)` contract. The entry constraint applies to the
initialized panel pointer before its real callback load and argument use. The
second applies to the initialized final sprite ID immediately before its real
call. No memory access, arithmetic, callback, register binding or artificial
consumer is added. These are target-specific reconstruction aids, not evidence
of original source spelling.

Paired complete-owner controls at the same 84-byte size give six differing
words with neither constraint, four with only the panel constraint, two with
only the final-ID constraint, and zero with both. The panel constraint restores
the opening ID's `ori` before the saved `$17` store. The final-ID constraint
keeps its `ori` adjacent to `lui` before argument setup and restores. The actual
callback is loaded after the first sprite call, from panel `+0x38`, and receives
both the panel and ordering-table ID. No deeper compiler-hook mechanism is
claimed from these paired byte comparisons.

Whole-owner production/SKIP checking gives 18 match, 0 differ, preserving all
17 existing peers, including Type0Grad. The retained-draft target is also exact;
all 19 retained peers remain unchanged. The owner has no nonempty allocated
data payload. Existing truthful forward declarations for `cmpDrawBoxF`,
`fclDrawPanelSprite2` and `memset` are now unconditional; all three modes pass
strict implicit-declaration diagnostics. Complete plain/dump assembly and all
ELF section headers and contents agree in production, SKIP and retained modes,
normalizing only the equal-length generated source path. Independent native
and semantic review includes callback replacement by the first sprite call,
signed ordering-table extremes, and sanitizer checks.

## fclDrawPanelType0Grad (facility/fclPanel, 0x100)

64 of 64 words match at 256 bytes. A current-snapshot scan of 147 guarded owners /
543 drafts identified this singleton alongside ConfigCursor. Removing only the
guard, fallback and obsolete residual comment makes the existing ordinary body
exact; no helper, prototype or body change is needed. The historical six-word
second-color residual below is stale for this measured context. No historical
compiler cause or original-source spelling is claimed.

Two actual `u32 coords[2][3]` subarrays supply independently bounded X/Y triples.
The triangle provider consumes all six coordinates and all three initialized
colors, with real per-vertex palette/alpha roles. The second negative X is an
unsigned word for native modulo32 GS-offset addition, not a fabricated pointer
or overflow assumption. Panel Z is freshly loaded and converted to the unsigned
depth argument after each preceding callback. Sprite 0x48, both triangles and
sprite 0x44 keep native ordering and truthful provider types. No constraints,
volatile, dead scratch, fake extent or scalar phase-reuse rationale is needed.

Independent whole-owner checking gives 16 match, 0 differ, preserving all
15 peers; this owner emits no allocated data payload. Other guarded functions
remain unchanged. Combined retail checksum, 427 tests across 26 tooling files,
147 guarded owners / 541 drafts, seven IRX owners and report gates passed for
source `efc12ecfd119cb16699f1afdd2508de80df5e137`. Exact-source CI
[41096589522](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41096589522)
attempt 1 completed successfully, with every actual step passing and none skipped.

### Historical residual and controls (superseded measurement)

6 of 64 words differ at the exact 256-byte size. Two gradient triangles sit
between sprite passes 0x48 and 0x44. Each triangle reloads panel Z after the
preceding callback; the third vertex keeps the first color's low-byte alpha.

The paired `coords[2][3]` array preserves contiguous X/Y triples. Colors precede
vertex assignments, and coordinates are assigned by vertex. Only the second
color's `ori` differs in scheduling: retail places it at +0x98, while the draft
places it at +0xAC after five argument/constant moves. All stored values agree.

Separate X/Y arrays change the stack layout. Chained tail-color assignments
swap two stores; a byte cast reloads the alpha and adds eight bytes. The word
mask preserves the native size and the six-word scheduling residual.

## fclDrawPanelType0 (facility/fclPanel, 0x394)

### Current measured blockers (2026-11-06)

In a fresh current-owner measurement, 174 of 229 words match at 916 bytes
for the unchanged ordinary body; the 172-word matching score below is
historical. Binding only the real animated-height value to native `$22`, after
the first background consumer, puts Y in `$fp`, but only 168 of 229
words match at the same size. The earlier curve instead occupies `$22`
rather than native `$23`, and blended color uses `$23` rather than native
`$18`. The Y/height swap alone is therefore insufficient; the competing
curve/color lifetimes and remaining opening arithmetic and color store
scheduling still need an explanation. A whole-height declaration binding has
the same residual. These are rejected diagnostic controls, not landed C.

The ordinary and localized-height controls have complete plain/dump assembly
and ELF parity after normalizing the single equal-length generated source
path, in production, SKIP_ASM and retained-draft modes with stable runtime
inputs. The localized control preserves all peers and owned data. Strict
production/SKIP diagnostics remain the same three pre-existing implicit
function declarations and associated prototype warnings; retained-draft strict
compilation passes. No new allocator-hook mechanism or original source
spelling is claimed.

### Historical ordinary-source reconstruction

172 of 229 words match at the native 916-byte size. The animation and drawing
passes are reconstructed; the assembly fallback remains active and no exact
matching credit is claimed.

The height curve first multiplies `(256 - Timer)` by the height-derived width,
truncates the signed integer division by 256, converts to float, takes a square
root, squares that result, and converts back before subtracting from the width.
The configured EE compiler emits the native `sqrt.s` and `cvt.w.s` for the
`fsqrtf` helper and C cast. The earlier guarded source in commit `48956298`
used `fcvtws`; that helper preserves the conversion opcode but its two volatile
operations leave 82 differing words with these lifetimes. The ordinary cast
emits the same native conversion here. Omitting the square root changes the
algorithm.
The initial background height is `H - 8`; the animated height adds 0x22 only
after that first background callback. The first blended gradient has fixed
height 0x22 and the second has the animated height.

One four-word color array is reused throughout. The first blend reads a fresh
`256 - Timer`; the second reads a fresh `Timer` directly. The final lower
panels reset their palette rather than retaining the blended values. Z is
reloaded for each drawing callback. Y is captured before the upper sprite
callback: the first two small bars use the captured Y and Y + 0x20, and both
blended gradients use Y + 0x40. The lower sequence reloads H and Y after the
preceding callback and updates its own coordinate through the remaining draws.

The live curve scalar follows the native integer, square-root conversion and
complement phases. The consumed constant temporaries likewise follow the
native normalization/opacity endpoints and denominator/RGB-base phases. These
are ordinary arithmetic inputs, not unused copies or register constraints.
A literal multiply by 102 expands into shifts and additions; the real opacity
endpoint local retains the native multiply. Named palette values and the
consumed phases restore the ordinary divide/multiply sequence.

Plain and instrumented compiler output parity was checked without RTL dumps
using the actual owner and the pinned EE 2.96 compiler. The allocator gives the combined Y
quantity 22 references across 18 calls (priority 6518) and height four
references across 12 calls (priority 792). Y takes `$22`, leaving height in
`$fp`; native uses the opposite assignments. The remaining 57 words concern
those quantities, the opening arithmetic operands/schedule, and a few color
stores. This is a measured allocation question, not a missing buffer or draw.

Splitting upper and lower Y values regressed to 908 bytes. Feeding the width
from the mutable height before its decrement remains 916 bytes but differs in
59 words. Changing percentage, alpha-byte, packed-color, product-order or
cached-delta spellings was inert; chained color assignments worsened the
result. Revisit the value ranges or compiler decision before repeating those
source forms. The complete normal owner remains exact, including its data.

### Type-one panel: captured depth and consumed width phases

The guarded reconstruction matches 100 of 109 words at the native 436-byte
size. It keeps the entry width across the initial sprite callback, subtracts
the border before the first box, and subtracts 19 before passing that width
to the second box. The entry depth is captured before the callback and shifted
by four for subsequent sprites; the box callbacks still read fresh depth.

Localized constraints on these real values preserve the second-box width
phase and native callee-saved depth and border-offset registers. These are
reconstruction aids, not evidence of original source spelling. Every value
has an actual consumer, and both bound registers are callee-saved across the
calls they span. Unsigned-word coordinate arithmetic preserves native wrap.
The nine residual words concern scheduling around the second box and texture
address materialization. The assembly fallback remains active; no exact
matching credit is claimed.
