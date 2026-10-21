# Non-matching drafts: fclShare

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## fclDrawSpriteAngleCore (fclShare, 0x4A8)

212 of 298 words match, 86 of 298 differ, at the exact 1192-byte size. Builds the
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
- The four corners live in an addressable array of a two-`u32` struct at
  `sp+0x10`, and the UVs in a `u32 uv[4]` at `sp+0x0`, which is what gives the
  `sw`/`lwu` round trips through the frame. The corner members have to be `u32`,
  not `s32`: retail reloads them with `lwu`, and `s32` members give `lw` with the
  same instruction count. The `fclCalcRotPos` calls then need `(s32 *)` casts.
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

## fclDrawSpriteCore (fclShare, 0x2D8)

69 of 180 words match positionally, 111 of 180 words differ, 720 against retail's
728 bytes. Builds the five-vertex 2D sprite packet: sets the texture filter, allocates
the prim buffer, writes the colour quadword, the two UV pairs and the two screen
corners, picks a `fclDrawPanelSprite2` code from `mode`, then links a draw tag into
`sdfGrOtTable[ot]`.

Everything about the body is settled. The 12-word prologue, the whole filter
`if`/`else` including the three dead `addiu $5, $0, 1` that only appear once
`sdf2DGetPrimPacketSize` is given a second argument, both colour arms (the
`color != NULL` arm's fourth byte really does come out as `lbu` from a single
`lw` plus `c & 0xFF`), every `dsll32`/`dsrl32`/`or` pair of the six quadword
stores, the three-way `switch` with its two call sites, and the whole tail from
`sdfGrAllocPacketBuff(0x20)` to the second `fclDrawPanelSprite2(0x44, ot)` are
word-for-word.

The one unresolved difference is a register-allocation swap in the prologue:
retail puts `x` in `$19` and `flags` in `$20`, this puts `flags` in `$19` and
`x` in `$20`, and every later allocation shifts with it. The first differing
word is the prologue copy at +0x20, and the 111 differing words are that one
swap plus the load and store order it drags behind it.

Two facts that are load-bearing and should not be re-derived: the four
`u0/v0/u1/v1` values live in an addressable `u32 uv[4]`, not four scalars,
which is what produces retail's four `sw` into `sp+0x0..0xC` and the four `lwu`
reloads; and `sdf2DGetPrimPacketSize` takes two arguments, `(5, 1)`, which is
worth 8 bytes and three instructions.

Ruled out: all six orderings of the UV, screen-corner and colour blocks; three
declaration orders for the locals (array first, scalars first, pointers last);
`u32` against `s32` for `x0`/`y0`/`x1`/`y1` with the cast moved to the store;
the corner swap written as a three-statement temp swap against an `if`/`else`
that assigns both arms (the `if`/`else` is 139 words, far worse); `x + 0x6FF8`
spelled twice so `x` has two references, which fixes the size to 182 words and
reaches 97 matching but computes the constant twice and so is not the original;
a `u8 f = flags` local both before and after the UV block; `flags & 1` with and
without `!= 0`; and the `sdfGrOtTable[ot]` call written through a hoisted
`sdfGrOt_t *` local, which the three `cmp` packet builders in this unit need but
which changes nothing here.
