# Matching notes: fclShare

## fclDrawSpriteAngleCore (fclShare, 0x4A8)

209 of 298 words match, 89 of 298 differ, at the exact 1192-byte size. Builds the
four-vertex rotated sprite packet: sets the texture filter, allocates the prim
buffer, turns the rectangle into a centre plus half extents, calls
`fclCalcRotPos` once per corner, writes the four colour pairs, then the UV,
screen and depth quadwords, picks a `fclDrawPanelSprite2` code from `mode` and
links the draw tag into `sdfGrOtTable[ot]`.

The first 70 words are exact, and so is the whole tail from the `mode` switch
on. Three facts are load-bearing and should not be re-derived:

- `sdf2DGetPrimPacketSize(3, 4)` and `sdf2DMakePrimPacketHeader(packet, 0x15C,
  3, 0x531, 4)`, the four-vertex counterpart of the five-vertex numbers in
  `fclDrawSpriteCore`.
- The four corners live in an addressable array of two signed `s32` members
  at `sp+0x10`, matching the actual `fclCalcRotPos(s32 *, s32 *, ...)` output
  contract. The UVs remain `u32 uv[4]` at `sp+0x0`. Convert each signed corner
  to `u32` when packing its low/high lane into a `u64`; this gives the native
  `lwu` reloads without inventing unsigned output storage or pointer casts.
- In the `color != NULL` arm, the two assignments have to be written low pair
  first, `prim[1]` before `prim[0]` and so on for all four vertices. Written the
  other way round ee-gcc folds both of `prim[0]`'s shifts into `lbu 0x2` and
  `lbu 0x3` byte loads, the function comes out two words short, and the
  difference is 136 words instead of 86.

Three differences are left. The angle reload `mov.s $f12, $f20` sits in the
`jal` delay slot of the second, third and fourth `fclCalcRotPos` call in retail
and one instruction earlier here, with `daddu $9, $19, $0` taking the slot
instead, which accounts for 21 words. One byte of the non-NULL colour comes out
as `lbu 0x1($30)` here where retail reads `lbu 0x0($30)`: whichever byte
extraction ee-gcc meets first gets folded into a byte load, and in retail that
is `c & 0xFF` rather than `(c >> 8) & 0xFF`. And the twelve quadword stores are
scheduled as six `lwu`s hoisted to the top followed by a chain of `or`s, where
retail interleaves load, `or` and `sd` per vertex.

Ruled out: four orders for the UV assignments, of which `uv[0]`, `uv[2]`,
`uv[1]`, `uv[3]` is the best and the only one that reaches 136 before the
colour fix; swapping the `|` operands in either colour expression, which adds
two words; the low byte written as `*(u8 *)color` or through a `u8 *cb` local,
both of which run eight words long; `c = color[0]` against
`c = *(u32 *)color`; `flags & 1` against `(flags & 1) != 0`; and three orders
for the quadword block, grouping the UVs in pairs, all four UVs first, and
depth before UV, all of which give the same 86 words.

### Guarded coordinate-lane correction

The corner fields remain signed because the rotation provider writes signed
coordinates. Directly ORing signed X with `(u64)Y << 32` first converts X to
`u64`: for negative X its high 32 bits become all ones, overwriting the packed
Y lane. Native instead zero-extends both 32-bit words before combining them.
The guarded draft now explicitly uses `(u32)x | ((u64)(u32)y << 32)` at all four
point stores. For every 32-bit X/Y pattern, this produces low 32=X and high 32=Y;
no nonnegative-coordinate domain is assumed. The high-lane cast is also
explicit, preserving the native unsigned load width. The real signed provider,
array layout, all reads/stores and call interfaces are unchanged.

A focused control confirms all eight point reloads change from `lw` to `lwu`.
The positional score remains 209/298 at 1192 bytes because independent scheduling
residuals remain. One ordinary shared vertex-pair temporary gives 210/298 but does
not resolve them and is not retained. This is a guarded-draft correctness fix,
not a promotion or matching credit. The broader existing geometry arithmetic
and input contracts are not newly qualified by this local lane proof.

## fclDrawSpriteCore (fclShare, 728 bytes)

All 182 native words match. Four genuine unsigned halfword UV fields are
read through their typed member addresses after packet setup. The widened
values feed the real UV array, without a redundant mask on the selected
endpoint. Coordinates and dimensions use unsigned intermediates for the
native wrapping 32-bit arithmetic. The ordinary narrow flip flag drives
the two coordinate swaps.

The optional override uses both the packed color word and the native
separate low-byte access. A null override loads the four per-sprite
channels instead. The signed depth field still converts through u32, so
the two native 64-bit depth stores have zero high halves even for negative
sprite depths. Packet and draw-tag allocations, alpha panel sprites, and
texture registration preserve native callbacks and layout.

Earlier drafts were guarded while the UV loads, primitive-pointer copy,
and width/height setup differed. The complete matched owner still uses
the genuine addressable UV array and the two-argument packet-size
provider. This C shape is not a claim about original source spelling.

## fclDrawPhaseValue (facility/fclShare, 380 bytes)

95 of 95 words match in ordinary C. The function brackets two corner gradients
with panel sprites 0x48 and 0x44. Real coordinate backing is u32 points[2][3],
with three X values followed by three Y values. The separate three-color array
contains the original color and its low byte twice. Every coordinate is set
before its draw, while the same colors remain live across both draw calls.
The actual cmpDrawTriG provider consumes exactly three elements from each row
and color array without retaining their pointers. Matched fclDrawPanelType0Grad
corroborates this genuine two-row layout; no fabricated bounds or padding
ownership is introduced.

The panel keeps signed 32-bit X/Y/Z and unsigned halfword width/height. Explicit
u32 coordinate conversions preserve native modulo-32 add/subtract behavior for
all signed coordinate patterns, including wrap at the endpoints. Real halfword
storage bounds the dimension arithmetic: height*80 is at most 5,242,800 and
width*16 at most 1,048,560. These signed products and positive quotients cannot
overflow. A real signed height snapshot after the first triangle preserves
native signed division by five. It does not move a read across that callback
or invent negative heights.

Geometry is read after the opening sprite callback and reloaded after the first
triangle. Coordinates, height and depth therefore observe callback changes;
color and ordering-table arguments remain original inputs. Call order, actual
field widths, stack lifetimes and provider signatures are unchanged. No binding,
empty operand, volatile qualifier, compiler option or dummy use is added.

The target-active old draft emits 376 bytes. Genuine row backing establishes the
native 12-byte row offset; direct geometry expressions reach 94/95 at 380 bytes.
Capturing the actual signed height fixes the sole DIVU/DIV difference. Explicit
unsigned coordinate arithmetic leaves exact output unchanged while removing
signed-overflow exposure. These comparisons establish the source/output
relation, not original spelling or an unobserved compiler-internal decision.

Independent actual-body fixtures pass 363,516 cases each in optimized and
ASan/UBSan builds, covering every u16 height, signed-word coordinate/depth
extremes, colors, arguments and callback mutation. The fixture checks real rows,
full draw arguments, call order and canaries. It models geometry and callback
behavior, not PS2 rendering. Actual providers still require a valid ordering
table and live runtime allocation; arbitrary argument bits in fixtures do not
claim otherwise.

Complete owner checks report 21 match, 0 differ. All 23 production, 20 SKIP_ASM
and 23 retained peers, owned data and resolved call contracts remain unchanged,
including prior sprite coordinate/depth repairs. Five controls across three
modes have 15 strict whole-assembly and raw-ELF plain/dump parity pairs, with
canonical-wrapper equality apart from the source filename. Resolved compiler,
assembler, QEMU and loader/runtime inputs are included in the corrected
334-file stable closure. Current-base integration additionally verifies unchanged
provider bodies and compilation inputs, fresh final three-mode parity, and
complete artifact identity before full repository gates.

## fclDrawSpriteAngleCore (facility/fclShare, 0x4A8)

All 298 words match at the original 1,192-byte extent (0x00214B30).
The low-word wrapped coordinates and sign-aware half extents feed four
separate rotation calls. The renderer reads the four texture halfwords
before selecting its flip paths, and the packet encodes the resulting
signed coordinates as separate 32-bit lanes. Each vertex receives the
same arithmetic-shifted depth, zero-extended when packed. The override
colour uses the native word and byte reads; the default path reads the
sprite colour. The original three blend cases and ordered registration
calls follow the packet writes.

Only the actual angle/call arguments and texture and packet values use
local EE constraints. In particular, arranging the raw first Y value
before its 64-bit shift gives the right-U load the native order. Each
paired input is initialized and used in the packet or blend test, and
the packed depth slot is real. Reused physical registers contain only
one live logical value at a time; none of the packet constraints span a
call. The neighboring retained phase-header draft is unchanged.
