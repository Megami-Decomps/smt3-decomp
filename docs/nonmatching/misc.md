# Non-matching drafts: misc

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## func_001D4EB0, func_001D4C08 (code_001D1470, 0x58 and 0x94)

21 of 22 and 36 of 37 words match, both at the exact retail size.

One word each, and the same word in both: retail takes the `beqz` after a
`pbGetMoveChar` call as `beql` and puts the next instruction in the annulled
delay slot, where ee-gcc emits the plain `beqz` and runs that instruction
unconditionally. In `func_001D4EB0` the annulled instruction is the
`lui 0x5` for the trailing `panel->flags = 0x50000`; in `func_001D4C08` it is
the `ld $16` epilogue restore, which is the branch-likely case
`docs/matching-levers.md` already lists as ruled out. Both are semantically
identical to retail, since the annulled instruction is dead when the branch
is taken.

Ruled out for `func_001D4EB0`: the `panel->flags` store written into both
arms of an `if`/`else` so gcc merges the tail, which duplicates the `lui` and
the epilogue and costs 12 words; and the store placed before the `if`.

Two other functions in this unit, `exitPB` and `func_001D4E70`, have the same
`pbGetMoveChar` null-test shape and match exactly, so it is the annulment and
not the shape. Anyone picking these up should start from how ee-gcc chooses
`beql`, not from the source.

## effMagaCylinderDraw, effMagaSpiralDraw and effMagaSmokeDraw (effect/effMagatuhi, 0x2B4, 0x2C4 and 0x2DC)

132 of 172, 136 of 176 and 140 of 182 words match, each one word short of retail
(688 against 692, 704 against 708, 728 against 732 bytes). All three are the same
reconstruction with a different particle advance, and all three sit on one
mechanism, so fixing one fixes the three.

Retail keeps `work->managedResource` in `$4` from the prologue load right through
to the `effMagaCylinderDrawTrack` call. The pointer is therefore still in `$4`
on the `count == 0` path, which jumps straight to the call, so retail reloads it
in the loop-exit block and leaves the `jal`'s delay slot a `nop`. ee-gcc here
loads it into `$2` instead, the pointer dies at `valueWork =
...->valueWork`, and the delay-slot filler takes the reload for both paths. That
is the whole missing word, and the rest of the diff follows from `$4` being free
in the draft: the three spilled values (count at `0x24`, respawn at `0x20`, the
tint colour at `0x28`, the same slots as retail) use `$4` as their temp where
retail uses `$2` and `$5`, the `SDF_VU0_COPYVECTOR` destination lands in `$4`
rather than `$5`, and the saved registers rotate, with `valueWork` taking `$30`
where retail gives it `$22` and `fadeIn`/`fadeOut` moving down to `$23`/`$22`
from retail's `$30`/`$23`.

Everything else is exact, including the fade chain (`if (age < fadeIn)`, then
`else if (life - age <= fadeOut)`, then `1.0f`), the `(u32)` float conversion
with its `c.le.s`/`cvt.w.s` pair, the `age` cached across
`effMaga*PartInit`, and the `-(dds3GetRandA(dds3EffectBase) % spread)` respawn.
The two nops ee-as pads between `cvt.s.w` and `div.s` are reproduced.

The sibling `effMagaSmokeConvDraw` in the same unit has the identical tail and
**matches**, with `lw $4, 0x18C($18)` in the `jal`'s delay slot. So retail's own
compile goes both ways on this; the three Draw functions are the side where
gcse kept the load available across the empty path, and nothing in the source
says which.

Ruled out, all leaving the count at exactly 40/40/42: an
`effMagatuhiOwner_t *owner` local for the pointer (one assignment, or reassigned
after the loop, or assigned and then `work->managedResource` written inline at
the call); every ordering of the nine prologue loads (nine sampled, including
`valueWork` first and last); fifteen declaration orders of the locals, which
have no effect whatsoever, as in the `fldAutoMapDrawIconOfsRF` row in
[matching levers](matching-levers.md); `while` in place of the `for`; a
`remaining` local for `life - age`; an `s32` `count` with the loop bound cast;
the `s32 dummy; s32 *p = &dummy;` sibling-call blocker. An explicit
`if (count != 0)` wrapper round the loop is far worse (153 of 174), and
`if (respawn && age >= life)` in place of `if (age >= life && respawn)` is 46.

## lstPageUp, lstPageDown (code_001FAE80, 0x74 each)

28 of 29 words each match, at the exact size. Each steps the list cursor up or down
by a page, stopping early when the cursor's `unk_04` says it cannot move, and
returns how many steps it took.

The one differing word is the early-exit branch inside the loop: retail emits
`bnez $2` and this emits `bnel $2`, with the same `daddu $2, $17, $0` in the
delay slot and the same displacement. Both keep a second copy of that
`daddu` at the loop's normal exit, so only the annul bit differs. gcc here
annulled the delay slot; retail's compile decided the write to `$2` was
harmless on the fall-through, where the next instructions set up a `jal`.

Ruled out: `break` against a `return i` inside the loop against a `goto` to
the trailing return; a `while` with the increment in the body; an explicit
`do`/`while` behind a zero-trip guard; folding the test into the `for`
condition as `i < count && cursor->unk_04 == 0`; `cursor->unk_04` written
without the `!= 0`; and declaring `lstMoveCursor` `s32` rather than `void`,
which costs two more words. `break`, `goto` and the `&&` form all give the
identical 29 words.

These two and `cmpUpdateList` in the same unit are the same class: one word
out on a branch-likely that no control-flow shape reaches.

## rstFindEvoEntryType5, rstFindEvoEntryType7 (code_001FAE80, 0x6C each)

24 of 27 words each match, at the exact size. Both scan a devil's 24 level-up
records for a record of kind 5 (or 7) whose level field is below (or at or
below) the unit's level, and return that record's index as an `s8`, or -1.

The three differing words are the `* 0x66` row multiply: retail puts `3 * id`
in `$3` and `48 * id` in `$2`, this puts them the other way round. The
accumulate (`addu $2, $2, $3`) and the final `sll 1` are identical either
way, so it is which register the first temporary of the synthesised multiply
lands in, nothing in the source.

Ruled out: `datLevelUpTable + id * 0x66` against `&datLevelUpTable[...]`,
a separate `s32` offset local, `0x66 * id`, a `u16` and a `u32` local for the
id, declaring the level before and after the entry pointer, assignments split
out of the declarations, a `u8 *` walk reading `p[6]`/`p[7]` instead of a
typed `entry[1]`, and nested `if`s instead of the `&&`. All nine compile to
the same three-word swap. This is the same register exchange recorded above
for `cmpFindKugatatiMsgEntry`, which walks the same table.

## Chk_ENHOJOMAX, Chk_FRHOJOMAX (newbattle/nbMakePacket, 0xDC each)

54 of 55 words each match, at the exact 220-byte size.

Both are the established `Chk_*` shape of this unit: a `one = 1;` local above
the loop, `a->data->party[i]` subscripted at every use so the base reloads
after the call, and a `continue` per flag test. The one differing word is the
third flag test: retail emits a plain `bnez $2, .L00286134` and duplicates
`slti $2, $18, 4` into all four predecessors of the loop-continue block, while
ee-gcc makes that one branch `bnel` and annuls the `slti` instead. The other
two flag tests come out plain in both.

Ruled out: `!= 0` written explicitly on the `0x20` test, fusing it with the
`ChkAiHojoMax` call into one `||`, and nesting the last two tests instead of
`continue`. All three give the identical one-word diff, as do the matching
siblings' shapes (`Chk_ENCNT` and `Chk_FRCNT`, which have no call in the loop,
match exactly).

## Flow8_7, Flow8_15 (newbattle/nbMakePacket, 0x7C each)

18 of 29 words each differ, two words short of retail's 124.

The two-case `switch` over `nownegophase`, both arms, the `0xA1`/`0xA3`
message constants and the `return 2`/`return 1` are all right: the dispatch is
retail's `beqz 0` then `beq 1` with no bound test, and the arms' bodies are
word for word identical. The single difference is that ee-gcc makes the second
dispatch test a `beql` and annuls `case 1`'s `lw $2, 0x184($16)` into its slot,
where retail keeps a plain `beq` with the default arm's `daddu $2, $0, $0` in
the slot and gives the default its own `b` to an exit placed after the shared
`ld $16`. Every later word is that one-instruction shift.

`Flow8_12` next door matches with the identical source shape, because its
`case 1` body starts with a `jal dds3GetRandIntA` that cannot be annulled, so
the shape is confirmed and only the annul bit is wrong. This is the same class
as the `Flow13_8` entry below.

Ruled out: a `default: return 0;` arm, a `ret` local with `break` in each arm,
a `goto zero` for the shared exit, the two cases in the other order, and an
extra empty `case 2`, `case 3` or `case 4`. The empty case reaches the exact
124-byte size and 11 differing words, but it does so by building a three-node
tree with an `slt 2` bound test that retail does not have, so it is further
from the retail source rather than nearer.

## nbAddStageCamera, nbAddStageCamera2 (newbattle/nbStageProcess, 0xC4 each)

2 of 49 words each differ, at the exact 196-byte size.

**There is no config blocker here**, against an earlier reading of these two.
The 16-byte `f32[4]` template each one copies off the stack lives at
`D_00543628` and `D_00543648`, which splat assigned to
`newbattle/nbResultProcess`'s rodata; a block-scope initializer would emit a
second copy and shift the image. Declaring the symbol with its real type and
assigning it, `extern sdfVector4_t D_00543628;` then `pos = D_00543628;`,
gives retail's `ldl`/`ldr` plus `sdl`/`sdr` copy from `%hi`/`%lo` of the symbol
and leaves both the rodata boundary and `nbResultProcess`'s `INCLUDE_RODATA`
lines alone. Everything else in both bodies is exact.

What is left is the `jal dds3CameraObjectBasicSetFovy` delay slot. Retail loads
the fovy out of `%gp_rel(D_0054E4C4)` first and fills the slot with
`daddu $4, $16, $0`, which is the argument order gcc produces when the second
argument is expanded before the first. The draft expands the pointer first and
the `lwc1` lands in the slot instead: the same two instructions, swapped.

Ruled out: an `f32` local holding the fovy, assigned either just before the
preceding call or at the top of the body (both run over size), and dropping the
`extern f32` declaration so the load goes through an implicit `int`.

## fcl_103420, fclEncycAllocEntry and fcl_103560 (kernel/dds3AdminiProcess, 0x94, 0xA8, 0xAC)

The three slot-registration wrappers, each at the exact retail size: 30 of 37,
31 of 42 and 32 of 43 words match. All three are the same function with a
different `kind` constant (3, 1, 2), and all three differ from retail the same
way: two address temps for the row are exchanged, which swaps the order of the
`destroy` and `userValue` stores. In `fclEncycAllocEntry` and `fcl_103560` the
three incoming function-pointer arguments are also rotated between `$16`,
`$17` and `$18`: retail takes them in parameter order, the drafts one place
around.

Store order is the lever that got them here and does not finish them.
Ascending field offset, `kind`, `task`, `update`, `destroy`, `setState`,
`enabled`, `userValue`, is what the drafts use; the obvious order (`kind` then
the argument store, then the zeroes) is 23 of 37 and 29 of 42 words.
`fclEncycAllocEntry` was swept exhaustively over all 5040 orderings of its
seven stores and caps at 11; `fcl_103420` was swept over 5040 and caps at 7.

Ruled out: splitting the declaration from the `fclEncycFindFreeSlot` call, a
`ret` local with `if (slot >= 0)` in place of the early `return 0`, a dead
self-store to raise a reference count, and parking
`fclEncycFindFreeSlot`'s draft so the callee is only an `extern`.
