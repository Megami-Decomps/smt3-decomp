# Non-matching drafts: code_001FAE80

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## ragDrawList2, ragDrawList3 (code_001FAE80, 0x150 each)

10 of 84 words differ, at the exact 336-byte size, for each of the pair. They
differ only in the scroll and cursor offsets, the row stride, the list offset,
the caption word, the kind constant and the header colour, so one analysis
covers both.

Every instruction matches. The ten differing words are the `ragDrawWork`
pointer landing in the other caller-saved register in each of the two blocks
that load it: retail holds it in `$3` for the first block and `$2` for the
second, this holds it in `$2` and then `$3`. Everything the pointer feeds moves
with it, which is the whole difference; the `lh`/`lbu` of the scroll and cursor,
the row pointer, the `lb` of the selected item in the first `fclDrawPanel`
delay slot, and the three `lw`s of the second panel's position are otherwise
identical.

Ruled out: the work pointer as a local against `(u8 *)ragDrawWork` written out
at every use; declaring the local before and after the scroll read; a separate
local for the cursor byte; and `s16` against `s32` for the scroll. All five give
the same ten words, so nothing in the source reaches the choice.

## ragBuildList2, ragBuildList3 (code_001FAE80, 0x174 each)

22 of 93 words differ, at the exact 372-byte size, for each of the pair. They
differ only in the destination offset (`+0x3B` against `+0x44`), the weight
table and which `ragGetListItem` they call, so one analysis covers both.

The function is settled apart from one instruction. It builds a list of four
fixed items plus `rand % 4` random ones, sorts it with `qsort` and returns the
count. The shape that reproduces retail's frame, its `break 0, 7` trap, both
`bgez` downcounting loops, the `s8` write cursor with its `sll`/`sra`
re-truncation, the `slt` guarded third loop and the `(s8)` return is

    s32 roll = dds3GetRandA(0);
    u32 n = 4;
    s8 pick[n];
    s32 extra = (u32)roll % sizeof(pick);

The variable-length array is what puts the trap there: ee-gcc expands a
division by a VLA's size as a divide-by-zero check against the saved size
plus the division, then folds the division itself. `count` has to be a `u8`
set in two steps, `count = n;` before the random loop and `count += extra;`
after, which is what keeps retail's `addiu $20, $0, 0x4` in the loop preheader
and the `andi $20, $2, 0xFF` after it; written as one `count = n + extra;`
ee-gcc proves the result fits a byte, drops both the `andi` and the `sll`/`sra`
of the return, and the function comes out seven words short.

What is left is the modulo. Retail emits `andi $19, $2, 0x3` with no divide,
this emits `divu`/`mfhi`. The fold happens only when the VLA's size variable is
dead after the array declaration; here `n` is also read by `count = n;`, which
keeps it live in a callee-saved register, so the divisor is a register and the
divide stays. The two cost the same number of words, so the size still matches,
and the rest of the difference is the register renaming that follows.

Ruled out: `% n` and `% (u32)n` instead of `% sizeof(pick)`, both of which
divide for real whatever else changes; every combination of `n`, `4` and
`sizeof(pick)` for the loop bounds, the `shuffleIndices` count and `count`'s
initial value, nine of which plateau at 22 and three of which fold `count` away
and lose seven words; a second `u32 m = 4;` for `count`, which ee-gcc merges
with `n`; declaring the array's size variable separately from `n` so the fold
does happen, which then loses the parameter copy and both loop-alignment `nop`s
and measures 86 of 91; and `&work[0x3B]`, `(u8 *)((u32)work + 0x3B)` and a
`u8 *list = work;` local, none of which reproduce retail's `daddu $16, $5, $0`
copy of the second parameter.

## func_00206308, func_002065C8 (code_001FAE80, 0x2C0 each)

35 of 176 words differ, at the exact 704-byte size, for each of the pair. They
are the same copy in opposite directions: `func_00206308` moves the fields of
one work struct down into a smaller one, `func_002065C8` moves them back, with
every source and destination displacement exchanged. One analysis covers both.

Every instruction matches. The whole function is twelve copies between the two
pointers, and both big ones are 576-byte block moves that ee-gcc expands with a
runtime alignment test, `or` of the two pointers against 7, then an unaligned
`ldl`/`ldr` loop and an aligned `ld`/`sd` loop. That shape, the twelve copies,
their order, the two 36-byte and one 32-byte straight-line `ldl`/`ldr` blocks,
the loop bounds and the branch delay slots all reproduce from

    *(s32 *)(dst + 0x00) = *(s32 *)(src + 0x10);
    *(u16 *)(dst + 0x04) = *(u16 *)(src + 0x14);
    *(cmbCopy36_t *)(dst + 0x06) = *(cmbCopy36_t *)(src + 0x16);
    ...

with `cmbCopy36_t`, `cmbCopy32_t` and `cmbCopy576_t` as all-`s16` structs, whose
alignment of 2 is what selects the unaligned path.

What is left is one register pair and the scheduling it drags with it. Retail
reuses the incoming `$4` for the first loaded word and for the second chunk of
the 36-byte block, and keeps `$3` for the alignment test; this uses `$3` for the
loaded word and `$4` for the test. Retail also emits the test's `andi`
immediately after its `or`, nine words in, where this emits it after the
36-byte block, 37 words in. The second group of copies allocates and schedules
identically in both.

Ruled out: statement order, searched over the twelve copies including every
swap of the two scalar pairs and moving the block copies ahead of them, which
plateaus at 35 or loses 100 words; `s32` against `u32` for the 4-byte copies;
`u8 *`, `s8 *` and `void *` parameters; explicit `u8 *` locals for the big
copy's two pointers; and the first six bytes as a single block move, which
ee-gcc compiles with `lh` rather than retail's `lhu` whether the alignment
comes from the type or from an `aligned(4)` field, so it cannot be one.

## func_002429C0 (code_001FAE80, 0x54)

19 of 21 words match, at the exact 84-byte size. Sums `attr[].capacity` over the
leader's inherited-skill list.

The two differing words are one constant split inside loop strength
reduction. Retail starts the induction variable at `0xA80` and leaves `0xE`
in the load displacement; every shape tried starts it at `0x30` and puts
`0xA5E` in the displacement. Same sum, same instruction count, same
registers.

Ruled out: four struct bases for the skill block, `(cmbUnitSkills_t *)
cmbGlobalWork->party` with the array at `+0x3A`, a block struct at
`cmbGlobalWork + 0xA80` with the array at `+0xE`, the same reached as
`&party[0].unit.namecode[4]`, and the count read through one base with the
array read through the other; four declared sizes for the skill array, which
change nothing; `sum +=` against `sum = sum +`; and an explicit
`if (count == 0) return 0;` guard, which makes gcc hoist the bound and
produce a downcounter instead of re-reading it.

## rstDrawStatusRow (code_001FAE80, 0x5C)

21 of 23 words match, at the exact 92-byte size. A pure call wrapper around
`cmpDrawStatusComEx`.

The two differing words are the `sra` halves of the two `s8` arguments that
travel on the stack, emitted in the opposite order: retail sign-extends the
second parameter and then the third, this emits the third and then the
second. Both `sll` halves, both stack stores, the eight register arguments
and the frame are identical, so nothing but the schedule is left.

Ruled out: `s8` parameters against `s32` parameters with `(s8)` casts at the
call; `(s32)` casts on the two stack arguments; `s32` locals copied from the
parameters and passed instead; an `s8` local for the first parameter; a local
for the colour constant; a `rstDrawWork_t *` local for the global; and
renaming the parameters, which changes nothing because the `sll` order
already matches.

## campDhGetEntryRatio (code_001FAE80, 0x60)

11 of 24 words match, at the exact 96-byte size. Decrements a delay, then either
rolls a new ratio or advances the current one.

Retail copies the work pointer out of `$4` into `$3` as its first
instruction and then uses `$3` and `$5` throughout; this keeps it in `$4` and
uses `$3` for the decremented value, so every differing word is that one
rename. The structure, the `blez`/`bnez` arms, the float add and the shared
epilogue already match, and `entry = NULL;` after the call is what stops
ee-gcc sibcalling `campDhRollRatio` and gets the size right. Nothing in the
source reaches the pointer's register: it is not live across the call, so
gcc has no reason to copy it and every shape tried declines to.

Ruled out: `s32` and `void` return types; a `campDhEntry_t *` local for the
parameter, declared with and without a separate assignment; assigning NULL
to the local against assigning it to the parameter; an `if`/`else if`/`else`
chain instead of early returns; and an `s32 next` local holding each loaded
value instead of `--` on the field.

## cmpFindKugatatiMsgEntry (code_001FAE80, 0x60)

12 of 24 words differ, at the exact 96-byte size. Scans a devil's 24 level-up
records for kind 5 and returns that record's value, or -1.

Every instruction matches; the twelve differing words are two register pairs
exchanged. Retail loads the id into `$5` and reuses the parameter register
`$4` for the loop index; this loads the id into `$2` and puts the index in
`$5`, which shifts the multiply chain's temporaries with it. Nothing in the
source reaches that choice: the parameter is dead after the single `lhu`
either way.

The record walk itself is settled. The induction pointer is based one record
early and the fields are read at `entry[1]`, which is what puts 0x6 and 0x7
in the load displacements against a pointer stepping by 4.

Ruled out: a `u16` and a `u32` local for the id, declared before and after
the entry pointer; `unit->id * 0x66` against `0x66 * unit->id`; biasing the
base by 4 and reading `entry->kind` with no `[1]`; `entry[i + 1]` indexing
instead of a walking pointer; and a typed `datLevelUpTable` array with an
`asm` alias, which costs one more word.

## cmp_241A10 (code_001FAE80, 0x74)

19 of 28 words match, 112 against retail's 116 bytes. Recounts how many of a
cursor's `unk_20` entries have bit 1 set in the `unk_2C` flag array, stores
the count in `unk_21`, and tail-calls `cmpUpdateCursorScroll(cursor, 1)`.

The loop is settled: it is a post-decrement `do`/`while` behind a `count > 0`
guard, `i = count - 1; do { ... } while (i-- > 0);`, which is what gives
retail's `daddu $2, $5, $0` copy before each test and the `blez`/`bgtz` pair
on the pre-decrement value. A plain `for (i = count - 1; i >= 0; i--)` tests
`bltz` on the decremented index instead and loses six words; `i = count;
while (i-- > 0)` lets gcc prove the `lbu`-loaded count non-negative and
rewrite the test as `i != 0`, which loses the guard entirely.

What is left is the missing word: retail loads the flag pointer into `$3`,
copies it to `$6` for the loop and reuses `$3` as the in-loop temp, where
this loads straight into `$6`. The branch that follows differs with it,
retail's `beq` with the `lbu` of the count in its delay slot against this
`beql` with the epilogue restore.

Ruled out: testing `cursor->unk_2C` instead of the `flags` local so the load
is CSEd into a copy; reading the count before and after the null guard;
moving the `unk_21 = 0` store above the guard; and a `count = i; i--;` while
loop. All of those either plateau at the same nine words or lose more.

## cmpUpdateList (code_001FAE80, 0x6C)

26 of 27 words match, exact size. The one differing word is the first branch:
retail's `beql $3, $0` against this `beq $3, $0`, with the same
`ld $31, 0x0($29)` in the delay slot and the same displacement. Both
duplicate the epilogue restore into the slot; only the annul bit differs, so
gcc decided the restore was harmless on the fall-through where retail's
compile annulled it.

Ruled out: the early-`return` chain, the nested `if` form, an `||` of the two
tests, and an `&&` of their inverses. All four compile to the identical
27 words, so the control-flow shape does not reach the delay-slot filler's
choice here. Same shape as the branch-likely cases listed under
[Matching levers](matching-levers.md).

## cmbChkDevilEvo (code_001FAE80, 0x74)

25 of 29 words match, at the exact size. Walks the same 24 level-up records as
`rstFindEvoEntryType5`, and on the first record of kind 5 returns that
record's devil's `datDevilTable` entry byte at +5.

The four differing words are the same `* 0x66` register exchange recorded for
`rstFindEvoEntryType5` below, plus the one scheduling slot it drags with it:
retail emits the first `sll` before the hoisted `addiu $7, $2,
%lo(datDevilTable)` and this emits it after.

Settled on the way: the `+5` has to come off a pointer local,
`info = &datDevilTable[id]; info->unk_05`. Written as
`datDevilTable[id].unk_05` gcc hoists `datDevilTable + 5` out of the loop and
loads at `0(reg)`, where retail hoists the bare symbol and loads at `5(reg)`.
That was one of the five differing words.

Ruled out for the register exchange: `(u8 *)datLevelUpTable + id * 0x66 + 8`
against `&datLevelUpTable[id * 0x66 + 8]`, a `u16` local for the id, and
declaring the `datDevilInfo_t *` at function scope instead of inside the
`if`. All three give the same four words.

## datFindKugatatiEntry (code_001FAE80, 0x74)

28 of 28 words differ, at the exact 112-byte size. Scans a devil's 24 level-up
records for ones whose byte at +6 equals the unit's level, returns a pointer
to the first match and, when the second argument is not NULL, writes the match
count through it.

Everything in the body is settled: the 0x66 shift chain, the `u8` count with
the `xor`-against-1 `movz` that stores the first match only, the
`for (i = 23; i >= 0; i--)` downcounter with the decrement in the `bne` delay
slot, and the `bnel` guarding the count store. The difference is which
registers hold the table: retail hoists `%lo(datLevelUpTable)` into `$10`,
derives `$12 = $10 + 6`, and keeps `0x66 * id + 4 * i` as a byte offset added
to both, so the load is `lbu 6($6 + $10)` and the stored pointer is
`$6 + $12`. This folds the row offset into one base pointer and reads at a
displacement.

Ruled out: the `cmpKugatatiEntry_t *` walk with `entry[1].value`, which is the
draft and the only form at the exact size; `((u8 *)ofs + (u32)datLevelUpTable)[6]`
with a `u32 ofs` byte offset, which rematerialises the `lui` inside the loop
and runs two words long; the same with a `u8 *table` local, which merges the
two bases into one induction pointer and comes out one word short; and the two
bases as explicit locals, both as `u8 *`/`s8 *` and as `u32`, which adds a
third induction variable and runs two words long.

## cmpClearCursorSlots (code_001FAE80, 0x54)

18 of 21 words differ, 84 against retail's 76 bytes. Clears both cursor slots of 256
rows at `cursor + 0x3C`: `state` and `param` to 0 and `value` to 0x100.

Retail does not strength-reduce the inner loop. It keeps the row pointer in
`$6`, recomputes `$7 = $6 + 4` at the top of each outer iteration, and in the
inner loop forms `i * 8` once and adds it to both bases, so `i` survives as a
counter with `slti $5, 0x2`. Every shape tried reduces the inner loop into a
pointer walk with a downcounter, and folds the `+ 4` into the `sw`
displacement so only one base is left.

Ruled out: a `cmpCursorSlot_t (*)[2]` 2D array with `slot[j][i].field`, which
is the draft and the closest; a `u8 *` row pointer with `row[i * 8]` and
`*(s32 *)(row + i * 8 + 4)`; the same with a `u32 ofs = i * 8` local, which
turns both pointers into induction variables and loses two more words; a
per-row `s32 *v = (s32 *)(p + 4)` with the value written as
`*(s32 *)((u8 *)v + i * 8)`, both from a `u8 *` row and from `&slot[j][0].value`;
and permuting the three stores, which only moves which word lands in the
`bnez` delay slot.

## mselToggleFlag (code_001FAE80, 0x6C)

13 of 27 words differ, at the exact 108-byte size. Flips a `u8` toggle, then steps
the low byte of a packed `s32` by +0x10 or -0x10 depending on the third
argument and clamps it to 0..0x80.

The structure is settled: the `bnez` on the narrowed `s8` argument with the
toggle store in its delay slot, the duplicated `lw` in both arms, the
duplicated `addiu -0x100` mask and the final `and`/`or`/`sw`. Retail carries
two plain register copies, `daddu $4, $2, $0` in each arm (the packed value
the tail re-reads) and `daddu $3, $2, $0` at the top of the clamp (the clamped
value). Writing the arms as `level = *value; v = level; level = (v & 0xFF) +
0x10;` reproduces the first pair, which is what took this from 20 of 25 words
and two words short to the exact size. The clamp copy needs the same
treatment and the two will not coexist: adding a third local for the clamped
value produces `daddu $3, $2, $0` and loses both arm copies instead, in all
six declaration orders of the three locals.

Ruled out: `*value` written at every use; an `s32 v` local assigned once above
the `if`, which hoists the load out of both arms; a separate `raw` temp in the
arms rather than reusing `level`; `if (dir != 0)` rather than
`if (dir == 0)`, which inverts the first branch; the clamp as
`if (level <= 0) ... else if (level > 0x80)`, which emits `bgtz` where retail
has `blez`; and `~0xFF` against `-0x100` against `0xFFFFFF00` for the mask,
all of which give the same `addiu $2, $0, -0x100`.

## cmpSetCursorSlot (code_001FAE80, 0x3C)

8 of 15 words differ, at the exact 60-byte size. Writes a cursor slot's `param`,
`state` and `value` unless its `state` is already 3. Four callers in the same
unit pass four arguments, so the signature is settled.

Retail computes the row address into `$8` (`addu $8, $7, $4`) and copies it
back into the parameter register (`daddu $7, $8, $0`), then reads and writes
the two bytes through `$7` and stores the zero through `$8`. Every shape tried
computes the row once into the parameter's own register, so the copy and one
of the two bases are missing and the rest of the body shifts with them. The
draft reaches the right size by giving the `value` store its own `s32 *`
local, which is what adds the second address instruction, but gcc still
addresses the bytes off the same register.

Ruled out: `slot += index` on the parameter; a single `cmpCursorSlot_t *`
local; two locals assigned the same address (coalesced); the two addresses
written with opposite cast order, `(u8 *)(index * 8) + (u32)slot` against
`&slot[index]`, which only swaps the `addu` operands; a `u32 ofs = index * 8`
byte offset shared by both; `slot[index].field` written out at all four use
sites; and moving the `value` store ahead of the two byte stores, which is the
draft and the closest form.

## cmp_23A918 (code_001FAE80, 0x40)

4 of 16 words differ, at the exact 64-byte size. Calls `func_00239C08(0, arg)` and
`cmpAllocRefGifTag(arg)`, then tail-calls `fclDrawPanelSprite(0x5C000, arg)`.

Everything up to and including the `lui $4` of the sprite id matches. The four
differing words are a rotation of the tail: retail emits the `ori $4, $4,
0xC000` immediately after the `lui`, then the `daddu $5, $16, $0` argument
copy and the two `ld` restores, while this emits the copy and both restores
first and the `ori` last. Same instructions, same count, same registers.

Ruled out: the id in an `s32` local; an unsigned literal; dropping the
`return` so the tail call is a statement. All three leave the `ori` in the
same place, so what is left is ee-gcc's second scheduling pass.

## trmGetJumpTerminalNo (code_001FAE80, 0x44)

8 of 17 words differ, at the exact 68-byte size. Returns a terminal number from
`fldAllCnt` when the work's `unk_21F1` is set, and otherwise indexes the work's
own `unk_21F4` table by `unk_11D6 + unk_11D8`.

Both arms are structurally right, the `beql` with the other arm's `lh` in its
delay slot included. What differs is one hard register: retail keeps the whole
`unk_21F1 != 0` arm in `$2` (`lhu`, `sll`, then `lh $2, fldAllCnt+2($2)`), and
because the load's destination is also its index register `ee-as` has to
expand the macro through `$at`. This puts the index in `$3`, so the assembler
uses `$2` itself as the temp and the `lui`/`addu`/`lh` triple differs in all
three words. The sum in the other arm accumulates into `$3` rather than `$2`
for the same reason.

The two halves cannot be had at once. Writing the sum `unk_11D8 + unk_11D6`
puts retail's `lh unk_11D6` in the delay slot but makes the `addu`'s first
operand `unk_11D8`; writing it `unk_11D6 + unk_11D8` gets the operand order
right and puts `lbu unk_11D8` in the delay slot instead. gcc emits the two
loads in the reverse of the source order here and takes the `addu`'s first
operand from the source order, so no spelling satisfies both.

Ruled out: both sum orders; the two values in named locals in either
assignment order, with `+=` and with one expression; `fldAllCnt` as
`s16 [][2]`, as a flat `s16 []` indexed `[i * 2 + 1]`, as a 4-byte struct
array, and as a cast pointer, all four of which make cc1 emit the same
`lh $2, fldAllCnt+2($3)`; a `trmUpdateWork_t *` local for the global; the
guard inverted so the `fldAllCnt` arm is the fall-through; and an `(s32)` cast
on either field.

## cmpItemHasFlag (code_001FAE80, 0x54)

13 of 17 words differ, 68 against retail's 84 bytes. Tests bit 0 of
`datItemTable[item].flags` when the second argument is zero and bit 1 of
`flags2` otherwise.

The index arithmetic and the entry stride are settled: `item * 8` is computed
once above the branch and reused in both arms, which is what the shared
`sll $2, $4, 3` and the `daddu $4, $2, $0` copy give.

What is left is that ee-gcc folds both single-bit tests and retail's compile
does not. Retail's first arm is `andi 1`, a redundant `andi 0xFF`, then a
`beqz` selecting 0 or 1 through a shared exit; every source shape tried folds
that to `andi $2, $2, 1` as the return value. Retail's second arm is
`andi 2` plus `sltiu $2, $2, 1`; this emits `srl $2, $2, 1` plus `xori
$2, $2, 1`, the bit-extract form of the same test. The redundant `andi 0xFF`
says cc1 did not know the masked value was already 0 or 1, which no C spelling
of the mask reaches here.

Ruled out: a `u8` local for the masked value, declared and assigned in one
statement and in two; an explicit `(u8)` cast on the mask result; a one-case
`switch` over the mask, which worked for `cmpItemIsStockFull`'s call result but
not for a mask; `== 1` instead of `!= 0`; an `s32` local for the whole field
read before the mask; the arms swapped so the `which != 0` arm is the
fall-through; and writing the second arm as an explicit `if`/`return` pair,
which costs another word.

## cmp_241A88 (code_001FAE80, 0x68)

3 of 26 words differ, at the exact 104-byte size. Sets bit 1 of a cursor's `unk_2C`
flag byte for one entry, bumps `unk_21` and calls `cmpUpdateCursorScroll`,
returning whether the bit had to change.

The body is settled, the `value | 2` computed above the bit test included,
which is what puts the `ori` before the branch and lets the `sb` reuse it.
What differs is two annul bits and the slot they let gcc fill: retail emits
`beqz` on the range test with the epilogue's `ld $31` in its delay slot and
`bnez` on the bit test with a `nop`; this emits `beql` and `bnel`, the second
with the `ld` in its slot. The reload is of an unmodified save, so filling it
unconditionally is harmless and retail's compile proved that; this one annulled
both branches instead.

Ruled out: the three guards nested inside each other; the null guard as an
early `return` with the rest nested; `goto` to a trailing `return 0`; and
`index < count` against `index >= count`. All three restructurings move the
pointer out of `$6` and lose thirteen more words, so the early-return chain is
the right shape.

## cmbGetHeartsParamByIndex (code_001FAE80, 0x58)

8 of 22 words differ, at the exact 88-byte size. Reads one of two byte tables inside
`datRaceEntry_t`, at `+6` and `+0xC` of a 66-byte row.

The two tables have to be flat `extern s8 D_00339E06[]` / `D_00339E0C[]`
indexed by `race * 66 + index`: that is what folds the `+6` and `+0xC` into the
symbol's `%hi`/`%lo` pair the way retail does. Reached through
`D_00339E00[race].field[index]` with the row struct split instead, cc1 keeps
the constant in the load displacement and the function is 12 words out.

What is left is one register. Retail puts each arm's `index + row` sum in `$2`,
the register the sign-extended index already occupies, so `ee-as` has to expand
`lb $2, D_00339E0C($2)` through `$at`; this puts the sum in `$3` and the
assembler then uses `$2` itself as the macro's temp, which changes all three
words of each arm's load.

Ruled out: `index + race * 66` instead of `race * 66 + index`, which fixes the
`addu` operand order but moves the `sll`/`sra` pair and costs four more words;
an `s32 ofs` local inside each arm's own block, which gives the identical eight
words; and an `s32 row = race * 66;` local above the branch, in both summation
orders, which splits the row multiply across the test and loses six more.

## fclDrawPanel (code_001FAE80, 0x54)

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

## cmbFindCameraPoint (code_001FAE80, 0x64)

9 of 25 words differ, at the exact 100-byte size. Looks up a type-0x11 scene basic by
name, and on a hit copies the point resource's quadword to the caller's vector
and returns 1.

The quadword copy is `SDF_VU0_COPYVECTOR(out, basic)`, and the result variable
has to be the search result itself so that the failure path returns the `$2`
the call already left at zero.

Retail copies that result into `$3` before the null test and passes the copy to
`dds3PointBasicGetResource`, so the pointer and the return value are two live
pseudos from the first call onwards. Written as `void *basic = (void *)found;`
gcc coalesces the copy away, because the two hold the same value until `found`
is set to 1; moving `found = 1;` above the second call does keep both live and
reaches the exact size, but then gcc parks the pointer in `$16` and hoists the
constant, which is the draft's nine words.

Ruled out: a separate local for the resource pointer instead of reusing
`basic`; the pointer declared first with `found` cast from it; both as separate
declarations with the assignments split out; and the test written on `found`
rather than on the pointer. All four coalesce the copy and run one word short.

## movieViewerStep (code_001FAE80, 0x4C)

6 of 14 words match, 56 emitted bytes against retail's 80. Walks the movie
viewer's chip list forward by the list's own count and returns the node it
lands on, or NULL if the chain runs out first.

The entry sequence and the single shared exit are settled: the head load into
`$3`, the count `lh` in the `beqz` delay slot, `blez` on the count, the
peeled first `chip = *chip`, and `jr $31` / `daddu $2, $3, $0` all match.
What is left is the loop's closing branch. Retail ends the loop with
`bgtzl $2` and the next `lw $3, 0($3)` in its annulled delay slot, which is
also why retail's loop carries five `nop`s and one trailing `nop` that the
draft does not: the pad is the R5900 short-loop fix the assembler applies to
a loop closed by a branch-likely, so getting the `bgtzl` back should bring
the padding with it. Every spelling that writes the preheader advance
explicitly has it folded away again: gcc threads
`n--; if (n > 0) { chip = *chip; goto loop; }` into an unconditional `b` back
to the entry `blez`, which reuses the guard as the loop test and leaves one
`lw` instead of two.

Ruled out: a test-at-top `while (n > 0)` with the null test as a `break` (8
of 13 words match); the same with `--n`; a `for` with the count read in the
init (2 of 13); `chip != NULL && n > 0` as the guard and a `do`/`while` on
`--n > 0` (6 of 15); `while (1)` with two `goto`s out (6 of 15); the null
test as a `while (chip != NULL)` condition with the advance in the `for`
increment (6 of 15); an early `return NULL` per exit instead of the shared
one, which runs past the next function; and both `goto` forms of the
rotated loop, with the bottom test written `if (n > 0)` and `if (--n <= 0)`,
which are the draft at 6 of 14.

## cmpClearStatusFlags (code_001FAE80, 0x30)

1 of 10 words match, 40 emitted bytes against retail's 48. Clears every bit
but bit 1 of the camp status panel's flag byte and returns the result, or 0
when the panel is not allocated.

The instruction sequence is otherwise retail's: `lw` of `campStatusPanel`,
`beqz` to a `jr $31` / `daddu $2, $0, $0` arm written last, then
`lbu`/`andi 2`/`sb`. The two missing words are a zero-extension of the
returned byte that retail spells `sll $2, $3, 24` then `srl $2, $2, 24`, with
the `sll` scheduled ahead of the `sb`. No source shape reaches that pair:
every form that keeps the value in a register lets gcc see it is already
`& 2` and drops the extension entirely, and the one form that does emit an
extension gives `andi $2, $2, 0xFF`, which is this port's `zero_extendqisi2`
for a register operand. The register map follows from it, since retail needs
`$2` for the extension and puts the pointer in `$4` and the byte in `$3`,
where the draft uses `$3` and `$2`.

Ruled out: `return panel->flags &= 2;` and
`return panel->flags = panel->flags & 2;`; the `&=` as its own statement with
the value re-read on the next line, through a local pointer (`andi 0xFF`) and
through the global (which reloads `campStatusPanel` and re-reads the byte, the
right 12 words but the wrong two); a `u8`, `char` and `volatile u8` local for
the masked value; `u8`, `u16`, `u32` and `s32` return types; `(u8)(s8)` and an
explicit `(x << 24) >> 24` at the return; declaring the struct field `s8`; and
`u8 flags : 8` and `u32 flags : 8` bitfields reached through a cast.

## cmbDrawFullScreenPanel (code_001FAE80, 0x68)

20 of 26 words match, at the exact 104-byte size. Calls `cmpDisableRGB`, draws
sprite 0x34000, fills the screen with `cmpDrawBoxF`, allocates a ref GIF tag
and tail-calls the same sprite draw again.

Every instruction is retail's, in retail's registers. The six differing words
are the delay slots of the two `fclDrawPanelSprite` calls and the epilogue
they shift. Retail schedules `lui $4` then `ori $4, $4, 0x4000` and fills the
`jal` delay slot with `daddu $5, $16, $0`; this schedules `lui`, the flags
copy, and then fills the delay slot with the `ori`. Same instructions, same
count, so what is left is ee-gcc's second scheduling pass, as in `cmp_23A918`
above.

Ruled out: an `extern s32 fclDrawPanelSprite(s32 id, s32 arg);` prototype above
the function, which the unit otherwise leaves implicit; an `s32 id = 0x34000;`
local for the sprite id, which the `cmp_23A918` entry also rules out; and
passing `flags` to `cmpDisableRGB`, which the unit's other caller does not.

## fclTerminalLoadTextures (code_001FAE80, 0x70)

16 of 27 words match, 108 emitted bytes against retail's 112. Loads the
terminal's one texture from the name table at `D_00330808` and prints
`D_0053D5E8`.

Everything through the store matches: the three saved registers, the counter
in `$16`, the texture slot pointer walking `$17` by 4, the table base in
`$18`, the indexed `sll`/`addu`/`lw` of the name and the `sw` of the handle.
The one missing word is the counter's own normalization. Retail writes
`addiu $3, $16, 1`, `sll $3, $3, 24`, `sra $16, $3, 24`, so the counter is a
signed byte; a `u8 i` gives `andi $16, $3, 0xFF`, one word short.

The two halves pull against each other, which is the finding worth keeping: a
**signed** byte counter is a biv ee-gcc's strength reduction recognises, so it
also reduces `D_00330808[i]` into a second walking pointer and the whole body
goes to 8 of 27 words match with the registers rotated. A `u8` counter is set
by an `and`, which is not a biv form, so the table stays indexed exactly as
retail has it. Every signed spelling tried reduces the table and every
unsigned one gets the `andi`.

Ruled out, all 19 of 27 words differing with the table reduced: `s8` and plain
`char` counters with the bound written `i < 1U`, `i < 1`, `i == 0` and
`(u8)i < 1U`; the name read as `*(char **)((u8 *)D_00330808 + i * 4)`, as
`D_00330808[(u32)i]` and through a `char **name = D_00330808;` local; the slot
advance written `tex++` as its own statement, in the `for` increment, and as
`*tex++ =`; and an `extern s32 fclLoadTexture(char *name);` prototype above the
function. Also ruled out: both sides indexed (`tex[i] = ...D_00330808[i]`),
which spends a fourth saved register on the shared `i * 4`; and
`D_00330808[(u8)i]` with a signed counter, which is 10 of 28 words match and
one word long.

## rstBuildDestroyList (code_001FAE80, 0x64)

1 of 24 words matches positionally, 23 of 24 with the register names ignored;
96 emitted bytes against retail's 100. Compacts the destroy unit's inherited
skill list, moving every non-zero entry to the front and storing the new count
back over `skillcnt`.

Every instruction is retail's, in retail's order: the `lhu` of `skillcnt` in
the preheader with its `beqz` skip, the source pointer walking from
`unit + 0x3A`, the destination pointer biased to `unit + 0x38` and advanced by
2 *before* both stores, the `sh $0` then `sh value` pair, the `sll 24`/`sra 24`
on the `s8` count, the reload of `skillcnt` inside the taken arm, and the
`slt`/`bnez` at the bottom. The destination's bias is what
`unit->skill[count - 1] = skill;` after `count++;` gives; the natural
`unit->skill[count] = skill; count++;` bases it at `unit + 0x3A` instead and is
no closer.

The one missing word is a `daddu $3, $2, $0` copy of the loaded skill. Retail
loads the halfword into `$2`, copies it into `$3` (where the loop bound is dead
at that point) and reuses `$2` for `count + 1`; the draft computes `count + 1`
in the `beqz` delay slot, which keeps `$2` busy, loads the skill into `$3`
directly and needs no copy. That one value shifts every register in the
function up by one, which is why the positional count looks so much worse than
the body is.

Ruled out: `skill` as a block-scope and a function-scope local, and as `u16`
and `s32`; `count` as `s8` and `s32` (the `s32` form is 22 words, two short);
`count++` written before the stores, between them and after them;
`unit->skill[count]` with a post-increment; a `u16 *dst = &unit->skillcnt;`
local advanced in the arm; and a `while` loop with `i++` hoisted above the test,
which runs one word long.

## cmbDrawResultFooter (code_001FAE80, 0x74)

25 of 29 words match, at the exact 116-byte size. Draws the combine result
footer: two panel sprites around a `cmpDrawBoxGDefault` whose four corner
colours come from a stack array.

The instruction stream, the store order into the array (`corner[2]`,
`corner[0]`, `corner[3]`, `corner[1]`, the last in the call's delay slot) and
every other register match. The four differing words are one register name:
retail builds `color & ~0xFF` in `$2` (`addiu $2, $0, -0x100` then
`and $2, $16, $2`) and stores it from there, while this builds it in `$3`. The
`$2`/`$3` row in the levers page blames a non-void callee whose result is
ignored, but it does not apply here: making `fclDrawPanelSprite2` `void`, which
keeps its own body's `j cmpDrawSprite2` and leaves it byte-exact, changes
nothing in this function.

Ruled out: the mask written `color & 0xFFFFFF00`; an `s32 dark` local for it,
which hoists the `and` above the first call and runs one word long;
`corner[2] = corner[0]; corner[3] = corner[1];` in place of re-reading `color`;
and the `void` callee above.

## ragCheckHeartsChoice (code_001FAE80, 0x80)

22 of 29 words match, 116 emitted bytes against retail's 128. Returns a hearts
choice: a table entry for index 1, the rag work's `+0x54` halfword for 2 and 3,
and 0 otherwise.

The body is settled. `ragGetHeartsFlag`'s result has to land in `$4`, the
`index == 1` test has to be written as `if (index != 1) { ... }` with the table
arm after it so the `beq` reaches the table arm as retail's forward branch
does, and the two range tests have to be **nested** `if (index > 0) { if (index
< 4) ... } return 0;` rather than two sibling early returns: as siblings
ee-gcc folds them into an `addiu -1`/`sltiu 3` range test and loses retail's
`blez` and `slti 4` pair, which is 8 of 30 words match.

The seven remaining words are the exit layout. Retail restores `$16` twice,
once in the delay slot of the `b` that leaves the table arm and once at the
shared `L830`, and lets `jump2` merge only from the `ld $31` on; this draft
gets one `ld $16` and three words fewer, so every branch displacement in the
function is off by three.

Ruled out: `index == 1` with a `goto` to a trailing table arm (8 of 30 match);
a `u16 ret` local assigned in each arm with one trailing `return` (8 of 30);
the same with explicit `goto done;` out of each arm (15 of 29); and
`(flag * 7) * 2` in place of `flag * 14` at the subscript.

## cmpGetStockInfoIndex (code_001FAE80, 0x80)

26 of 32 words match, at the exact size, with zero instruction differences.
Reads the two counts in `cmpStockWork_t::info[index]` and uses their `s8` sum
to subscript one of the two `s8` tables inside `stock[index]`, picked by the
third argument.

Every remaining word is a register name. Retail accumulates
`slot + index * 0x56` in place in the register that already holds `slot`
($2) and this allocates a fresh one ($3), which also commutes the two
`addu` operands and renames the base register in the two `lb`s. The shapes,
the branch sense, the `0x103C` and `0x56` multiply chains and the
`sll 24`/`sra 24` on the sum are all identical.

Ruled out: `slot` as `s32` with the `(s8)` cast written out; the row offset in
its own `s32` local, declared both before and after `slot`; the subscript as
`stock[index].unk_05[slot]` and as a `s8 *row` local, both of which fold the
base pointer into the row multiply and come out 12 bytes short; and dropping
the `base` and `info` locals. Writing the `info` offset as its own `s32 ofs`
local, which `cmpStockHasRoom` above it already does, is what fixed the two
adjacent words at +0x38.

## cmpDisablePadBitStop (code_001FAE80, 0x80)

17 of 32 words match, at the exact size. Negates the cursor's `s8` at +0x38
when it is not negative, otherwise divides the timer at +0x30 by
`D_0054E474`, clamps it at zero and clears the byte when the timer reached
zero; then masks the pad bits when `unk_04` is 2.

One delay-slot decision accounts for all fifteen. Retail closes the first
timer test with `bltzl` and the shared `sw $0, 0x30` annulled in its slot,
retargeted past the store; this emits a plain `bltz` to the store itself and
fills the slot with the `lwc1` of the divisor from the fall-through. Every
instruction after that is the same, one word earlier. The two hazard `nop`s
move with the `lwc1`.

Ruled out: the two tests written as `if (v < 0) ... else ...` with a store in
each arm, which runs long; reloading `work->cursor.unk_30` for the second test
instead of keeping `next`; `> -1` and `<= -1` for the two comparisons; a
`f32` local for the converted value, which does not reorder the divisor load;
and a `s32 *` local for the timer, which materialises the pointer and is 11
words out instead.

## cmpReCalcStockCursorPos (code_001FAE80, 0x84)

27 of 33 words match, at the exact size. Re-counts one or both stock cursors
from `stock[i].unk_02 + stock[i].unk_03[0]`.

The six differing words are the two loads and the `addu` in each half: retail
puts `unk_02` in $3 and `unk_03[0]` in $4 and adds `$4 = $3 + $4`, this
puts them the other way round. Same instructions, same count, same order.

Settled on the way: the `u8` narrowing has to be written `(... ) & 0xFF` with
an `s32` prototype parameter. Written `(u8)(...)` gcc pushes the mask back
into the first load and emits `lbu` where retail has `lb`; declaring
`cmpChangeCursorListNums` with a `u8` first parameter drops the `andi`
altogether and runs two words short.

Ruled out: both source orders of the sum, and `(u8)((s32)a + (s32)b)`.

## cmpSetIniStockRecoverEff (code_001FAE80, 0x84)

20 of 32 words match, at the exact size. Picks the stock cursor slot for the
recover effect from `cmpFindFreeStockSlot`, offset by 0x1078 or 0x3C
depending on whether the stock has room.

The twelve differing words are one if-conversion: gcc turns the choice of
offset into `movz` where retail keeps the `beqz` with the `addiu $7, $7,
0x1078` in its delay slot and re-derives the base in the fall-through arm.

Settled on the way: the pointer must be the reassigned parameter
(`work += 0xAC`), not a separate `base` local. With a separate local the
parameter and the base are live at once, work lands in $17 instead of $16 and
the whole prologue shifts.

Ruled out for the `movz`: `if/else` assigning the pointer in both arms, which
emits two branches and runs long; the common `base + slot * 16` in its own
local with `+=` in each arm; the condition reversed; and two separate
`cmpSetCursorSlot` calls in the two arms, which are not cross-jumped and run
36 bytes long.

## cmpUpdateCursorSlots (code_001FAE80, 0x84)

1 of 31 words matches, 124 bytes against retail's 132. Ages the two cursor
slots of each of 256 rows: adds `param` to `value` while it is under 0x100,
clears `state` once it is not, and clamps `value` at 0x100.

The body is right but the inner loop is not. Retail keeps the inner index
live, recomputing `$8 << 3` each iteration and testing `slti $9, $8, 2`
ahead of the body, so `check_dbra_loop` cannot reverse it. Every shape tried
reduces the inner subscript to a pointer, which frees the index and lets gcc
run the inner loop down from 1 with `bgez`. Retail also carries a loop
alignment `nop` ahead of the outer loop that none of these emit.

Ruled out: `cmpCursorSlot_t (*)[2]` with `slot[j][i]`, which is the form
`cmpClearCursorSlots` above it uses, and a `cmpCursorSlot_t *` advanced by 2
per outer iteration with `slot[i]` inside.

## ragGetListItem2 (code_001FAE80, 0xCC)

29 of 51 words match, at the exact size. Rolls `dds3GetRandA` modulo the
count byte at +6 of a 35-byte row of `D_0032FC98`, then walks the row's seven
weights and returns the matching byte out of the seven-entry table the second
argument selects.

The loop, the `divu` with its divide-by-zero `break`, the `mfhi`, the
`weight++` hoisted into the `bnez` delay slot and the two `nop`s are all
identical. What differs is one register more in flight: retail holds the row
offset in $17 and the count in $16, carries `pick` in $6 and the counter in
$5, and re-derives the item pointer as `(7 * kind + table) + row` then `+ 7`
after the divide; every shape tried allocates the pair the other way round,
runs `pick` and the counter one register lower, and folds the `+ 7` in before
the divide.

Settled on the way: the division has to be unsigned (`(u32)dds3GetRandA(0) %
limit` with `u32 limit`), otherwise it is `div` where retail has `divu`; and
the row has to be an `s32` byte offset into the array rather than a `u8 *`
row pointer, which otherwise merges the table base into one pointer and runs
20 bytes short.

Ruled out: `row` as a variable and as `index * 35` repeated inline, both of
which give the same 22 words; `weight` declared before `pick`; `pick`
assigned in a statement after the declarations, which adds a register; `item`
declared before `weight`; and `item = weight + kind * 7 + 7`.

## ragGetListItem3 (code_001FAE80, 0xCC)

29 of 51 words match, at the exact size. Byte-identical to
`ragGetListItem2` above except for the table, `D_0032FCE0`, and it differs
in the same 22 words for the same reason. Fix one and the other follows.

## fclDrawSpriteAngleCore (code_001FAE80, 0x4A8)

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

## cmbCountEvoReady (code_001FAE80, 0x84)

9 of 34 words match, 25 of 34 differ, 136 against retail's 132 bytes. Counts
how many of `count` consecutive `datUnitWork_t` entries are alive, not already
evolved, and have an entry in the evolution table.

The body is settled: the single `lw` of `flag` masked twice, the `beql`/`bnel`
pair that carries the `count--` into both delay slots, the `(u8)(ready + 1)`
with its `andi`, the `unit++` by `0xEC` in the loop-back delay slot and the
`(s8)` return. Written with three nested `if`s rather than one `&&` chain,
which is worth three words.

What is left is one register. Retail narrows the count parameter in place,
`andi $5, $5, 0xFF`, and then copies it into `$17`; this narrows straight into
`$17`, which is one word shorter and leaves a spare callee-saved register that
shifts the rest of the allocation. Nothing in the source reaches that copy: a
separate `u8 n = count;` local, a separate `datUnitWork_t *p = unit;` local and
a `for (; count != 0; count--, unit++)` head all coalesce it away exactly as
the plain `while` does. See the row on un-coalesced copies in
[When to stop](../levers/when-to-stop.md).

Ruled out: `ready++` against `ready = ready + 1` against an `s32 ready` with an
explicit `(u8)` cast, all three identical; the `&&` chain against nested `if`s,
where nested is three words better and is the draft; and the two loop forms
above.

## cmbGetLevelUpCount (code_001FAE80, 0x84)

22 of 32 words match, 10 of 32 differ, 128 against retail's 132 bytes. Copies a
devil into a scratch `datUnitWork_t`, adds the experience, and counts how many
levels that buys by stepping `datAdvanceDevilLevel` until the experience no
longer reaches the next threshold or the level hits 0xFF.

Settled: the `b` into the loop test, the `sltu` of the stored experience against
the call's return, the 0x110 frame for the 0xEC scratch struct, and the level
step written so that the `sh` of `level + 1` lands unconditionally in the
`bnez` delay slot, which is what the store-before-test order gives:

    level = work.level;
    work.level = level + 1;
    if (level >= 0xFF) {
        break;
    }
    count++;

What is left is the copy. Retail emits `daddu $3, $2, $0` so the 0xFF compare
reads the copy while the increment overwrites the original; every shape tried
compares and increments out of the one register and comes out a word short.

Ruled out: `u16`, `u32` and `s32` for the level local, all identical; the test
before the store, which is one word shorter again; `work.level++`; a positive
`if (work.level < 0xFF) { ... } else break;`, which reaches the right size but
13 of 33 words; and `level = work.level + 1;` with the old value re-read for the
compare, also the right size at 13 of 33.

## fclDrawSpriteCore (code_001FAE80, 0x2D8)

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

## campDhDrawIcon (code_001FAE80, 0x70)

26 of 28 words match, at the exact 112-byte size, with zero instruction
differences. Draws the box sprite, and when the hearts icon is loaded the
panel and the hearts icon, then tail-calls `campDhDrawBox`.

The two differing words are the second `campDhDrawWork` load and the `lb` that
feeds off it: retail reloads the pointer into `$2` and reads `0x31($2)`, this
reloads it into `$3` and reads `0x31($3)`. Both remaining words are a register
name.

The call site is what pins two signatures. `campDhDrawHeartsIcon` is passed
five arguments here, so its fifth parameter exists and is unused in its body;
and the work pointer is read at `+0x1C` with `lw`, where `campStatusPanel_t`
has `u16 slot[10]`, so the devil-hearts work is a different record and gets its
own `campDhWork_t`.

Ruled out: an `s8` local for the icon id; a `campDhWork_t *` local for the
second read; and an early return for the not-ready case, which costs 24 bytes.
The first two give the same two words.

## fclTerminalInitMenu (code_001FAE80, 0x7C)

27 of 31 words match, at the exact 124-byte size, with zero instruction
differences. Picks the terminal menu's text table, with the second group of
four entries when the current event is 0x296, hands it to the cursor window,
and sets the command bit and the work's enable byte.

The four differing words are the two registers of the `fclTerminalCommandCount`
read-modify-write: retail holds the `$28`-relative address in `$2` and the byte
in `$3`, this holds the address in `$3` and the byte in `$2`. All four remaining
words are a register name.

Ruled out: writing the work's `+0x25` byte before the command bit, which costs
four more words; the command bit spelled `x = x | 2` instead of `x |= 2`; a `u8
*` work parameter with `work[0x25]`; an `s8 *` local for the work byte, which
adds 12 bytes; and testing `!= 0x296` with the arms exchanged, which reaches
only 24 matching.

## cmbActionBGM (code_001FAE80, 0x68)

22 of 25 words match and every instruction up to the last call is identical,
but the draft is one word short of the 104-byte size. Stops or starts the
combine BGM and prints which it did.

The three remaining words are the final `printf`. Retail calls it with `jal`
and then runs the epilogue, where this turns it into the tail call
`ld $31` / `j printf` / `addiu $29` and so saves a word. The first branch's
`printf` is a tail call in retail too and reproduces exactly, so the shape is
right for one of the two and wrong for the other.

The parameter is `s32`, not `s8`: retail tests the incoming register raw with
`bnez` and sign-extends separately for `fclCombineBGM`, which needs a prototype
with an `s8` parameter ahead of this function. That half is settled.

Ruled out: `void` with `printf(...); return;` in the first branch; `s32` with
`return printf(...)` in the first branch and a plain `printf(...)` at the end;
`return printf(...)` in both; an `if`/`else` instead of an early return; and a
`switch (mode)` with `case 0` and `default`. All five tail-call the last
`printf`, so nothing in the statement shape reaches the choice.
