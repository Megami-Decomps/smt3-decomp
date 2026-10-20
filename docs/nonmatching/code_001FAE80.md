# Non-matching drafts: code_001FAE80

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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

