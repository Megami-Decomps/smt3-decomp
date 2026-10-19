# Non-matching drafts: pb/pbGame

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## gameKey1 (pb/pbGame, 0x2A8)

150 of 168 words differ, 672 bytes against retail's 680. The whole body shape is
there: the two `bgezl` pad tests, the map-cursor save and restore pair of
`SDF_VU0_COPYVECTOR`s, the `(degree_mode ^ 2) | 1` toggle, the four-way key
chain, the `movn` flag chain and the `else if` pair of `pbRoundCamera` calls.
Almost every differing word is the same two-word shift repeated.

The blocker is a data split, not a source shape. The cursor position is two
`s16` globals that retail addresses off `$gp` one at a time: it passes
`addiu $5, $28, %gp_rel(D_00551EC4)` and
`addiu $6, $28, %gp_rel(D_00551EC4 + 2)` to `pbGetMyCharcterPos2`, and later
reads each with its own `lh %gp_rel(...)($28)`. splat has only the one symbol
`D_00551EC4` covering both halves, so any C spelling of the second half goes
through the first one's address: gcc CSEs the taken address and emits
`addiu $6, $5, 2` for the argument and `addiu $6, $28, 0` plus `lh 0($6)` for
every later read, which is where the two words go. Splitting `D_00551EC4`
into two symbols is what this needs.

Ruled out: the pair as `s16 D_00551EC4[2]` with `&D_00551EC4[0]` and
`&D_00551EC4[1]`; as a two-member struct with `&D_00551EC4.x` and
`&D_00551EC4.y`; and the second address as `(s16 *)((u32)&D_00551EC4 + 2)`
and `(s16 *)((u8 *)&D_00551EC4 + 2)`. All four emit the identical code.

Confirmed on the way: `moveViewCamera` holds a `pbPanel_t *` (the draft sets
its `flags` to 0x50000), `pbGameKeyState` is the `u8` at 0x0054F5F5, the flag
word is built as `flags = y < 0x13;` then `|= 2`, `|= 4`, `|= 8` in that
order, and `ret` is reset to -1 inside the cursor block.

## deleteMoveBlock (pb/pbGame, 0x144)

46 of 77 words differ, 308 bytes against retail's 324. The first 31 words, both
record arms and the `pbStartMoveBlock` call are exact.

What is left is the `index == 4` arm. Retail reads `work->block` once, keeps a
second copy of it in another register, zeroes the counter before loading
`num`, and reloads `block->blk` on every iteration while holding
`D_0054F5C4 + 0xC` in a register across the loop. ee-gcc hoists `block->blk`
out of the loop instead, which is where the four missing words go, and sinks
the counter's zero into the guard's delay slot.

Ruled out: `block` as a local assigned before and after the two leading
calls, and `work->block->` at every use; the counter zeroed before the
`block` load, after it, and as a `for` init; `block->blk[i]` against
`(block->blk + i)`; and the map word through a cached `pbMapArea_t *`
(`D_0054F5C4 + 0xC`, retail's form) against the plain
`D_0054F5C4->area.map.blk[y][x]` expression (`+ 0x1C`). None of them makes
gcc reload the entry array.

Confirmed on the way: the `index != 4` arm has to be written first, since
retail branches to the `== 4` arm; `moveB` holds a `pbPanel_t *`, not an
`s32`; and `pbDeleteGroupEntry` takes the group id from `block->grpID` at
`0x4`.

## pbGetBlockAt (pb/pbGame, 0x120)

60 of 68 words differ, 272 bytes against retail's 288. Only the three `sll`/`slti`
words at the top match.

The four neighbour tests are each `if (out of range || (cell & 0x7FF) != grp)
ret |= bit;` over `D_0054F5C4->area.map.blk[y][x]` with the neighbour offset
in the load displacement, which is the right shape: retail recomputes the
cell address and reloads `D_0054F5C4` four times, as the draft does. Two
things differ. Retail if-converts the first test to `xor` plus
`movn $8, $5, $3` with the 0 in the guard's delay slot, where ee-gcc emits a
second branch; and retail allocates `x` to `$9` and `y` to `$7` where the
draft gets `$7` and `$5`, so every later word is shifted by the register
numbering.

Ruled out for the first test: the `||` chain with `ret` zeroed at the top;
`if (x + 1 < 20) ret = (... != grp) ? 4 : 0; else ret = 4;` and the same with
the arms swapped; and `ret` as `s32` rather than `u8`. The ternary does not
reach the `movn`, and none of the four changes the register numbering.

## roundBlock (pb/pbGame, 0x8C)

12 of 34 words differ, 136 bytes against retail's 140.

The body and every register match. What differs is block placement at the
end: retail gives each clamp arm its own `swc1 $f0, 0xB8(block)` followed by
`b` to the shared `sh step` block, and places the `addiu $2, $0, -1` guard
exit between the clamp arms and that block. ee-gcc cross-jumps the two
identical stores into one block and puts the guard exit last, which is one
word shorter.

Ruled out: the clamp pair as `if`/`else if`, as two separate `if`s with their
own `work->step` store and `return 0`, as a nested `if` inside the `else`, and
through a `goto` to a shared tail; `block->rot` through a `pbBlockObj_t *`
local, through `work->block->rot` at every use, and with the sum held in an
`f32` local and stored explicitly before the tests; the guard as
`if (step >= 4) return -1;` ahead of the body (17 of 32) against the body
nested in `if (step < 4)` with a trailing `return -1` (12 of 34); and
`step` as `u16` rather than `u32`, which adds an `andi 0xffff`.

Confirmed on the way: `pbBlockObj_t` carries the rotation as an `f32` at
`0xB8`, the `.lit4` entries the function reads are `0.39269909f`,
`6.28318501f` and `-6.28318501f` in that order, and `work->step` has to be
read into a `u32` local for retail's `sltiu`.
