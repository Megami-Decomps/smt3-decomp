# Matching notes: fclPanel

## fclDrawPanel (fclPanel, 0x54)

8 of 21 words differ, at the exact 84-byte size. Draws the panel's background sprite,
calls the panel's own `draw` hook with the panel and the id, and tail-calls the
foreground sprite.

Four of the eight words are the same `ori` rotation recorded for `cmp_23A918`
above: retail emits each sprite id's `ori` immediately after its `lui`, and
ee-gcc's second scheduling pass moves it past the `sd`/`ld` of a saved register.

The other four are the function-pointer load and its `jalr`: retail uses `$2`,
immediately after a `jal fclDrawPanelSprite` whose result is ignored, and this
uses `$3` because the ignored `s32` result still reserves `$2`. The fix by the
levers page is a `void` declaration of `fclDrawPanelSprite`, but it is defined
in this unit as `s32` with `return cmpDrawSprite(...)` and matches that way, and
three other callers ignore its result as well, so turning it `void` would have
to be measured across all of them at once.

Also settled: the `draw` field at `+0x38` takes two parameters, the panel and
the id, which is why `fclPanel_t` declares it that way; `panel->draw(panel)`
does not compile to retail's `$5` setup.

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
