# Matching notes: pb/pbGame

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

## pbInitGame (pb/pbGame, 0x290)

152 of 164 words match, 12 differ, at the exact 656-byte size. The draft
restores the saved 20-by-20 map, validates it, initializes the exit and
character panels, refreshes motion, calls the selected character callback,
clears the record, and optionally starts the stage script.

The work prefix contains four registration bytes at +4, four pairs of signed
coordinates at +8, exit coordinates at +0x18/+0x1A, and the map at +0x1C.
Character retirement uses the original slot index. Creation uses a separate
compact output cursor, advanced only for registered characters. The constructor
receives parent, color index and motion zero; its USA provider ignores the last
two arguments. Record clearing follows both callback paths. Script startup
passes zero as its third argument. Work and character-slot loads after callbacks
remain observable reloads.

A qualified `cc1probe --alloc --no-dumps` run has identical hooked/unhooked
output. The loop index has 13 references over a live range of 92, priority 4239;
the output cursor has 10 references over 68, priority 4411. The cursor therefore
gets saved register 17 and the index 18, opposite retail. The map-copy dispatch,
other registers and branch layout match. See [compiler traces](../../cc1probe.md).

An indexed compact output adds four bytes. Early cursor initialization hoists
its address and grows the function. A shared loop-index/script-label scalar
fixes the loop registers but changes the final label register; it remains five
words short of exact. Explicit postincrement in the final draw call, a do-loop,
and carrying map-validation status in the index do not close the gap. The
preferred draft keeps the separate index, cursor and script-label values.

A current parity-qualified allocation observation also identifies the fixed
character-array base as pseudo 129 (three references, 64 live instructions,
three crossed calls), assigned GPR19. It is distinct from the advancing cursor
pseudo 87/GPR17 and ordinal pseudo 86/GPR18; changing the source to name that
fixed array base explicitly preserves the same 152 matching words. Reusing a
signed index for the ordinal and script label gives 158/164 matching words:
the loop bound becomes signed and the final label still occupies the wrong
register. These controls do not close the residual. The facility flag-clear
declaration now agrees with its actual signed-byte provider; both callers
here pass literal zero, so their emitted instructions stay unchanged.
