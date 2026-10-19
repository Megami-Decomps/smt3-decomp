# Non-matching drafts: code_001FAE80

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## func_00213190 (code_001FAE80, 0x50)

8 of 18 words, 72 against retail's 80 bytes. A `qsort` comparator over the
`u16` inherited-skill list: zeros sort last, otherwise ascending.

Retail copies the first argument into `$3` and reloads `*a` in the tail where
the draft reuses the halfword already loaded for the zero test, so the
pointer copy and the reload are the two missing words and every later
register is shifted by one. The two compares, the `movz` and the whole middle
block are already identical. GCSE sees the second load as available, and no
source shape tried stops it; the retail source must have made the two reads
different RTL, or the function was compiled with the pointer live for another
reason.

Ruled out: a `const u16 *` local at two scopes and declared both before and
after the `bv` local; `bv` as `u16`, `u32` and `s32`; the dereference written
as `*pa`, `pa[0]` and `*(const u16 *)a`; `u16 *` parameters instead of
`const void *` (the `qsort` call site fixes the signature); and nesting the
tail inside an `else` so the two compares sit in one block.

## func_002429C0 (code_001FAE80, 0x54)

19 of 21 words, at the exact 84-byte size. Sums `attr[].capacity` over the
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

21 of 23 words, at the exact 92-byte size. A pure call wrapper around
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

11 of 24 words, at the exact 96-byte size. Decrements a delay, then either
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

12 of 24 words, at the exact 96-byte size. Scans a devil's 24 level-up
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

19 of 28 words, 112 against retail's 116 bytes. Recounts how many of a
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

26 of 27 words, exact size. The one differing word is the first branch:
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

25 of 29 words, at the exact size. Walks the same 24 level-up records as
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

28 of 28 words, at the exact 112-byte size. Scans a devil's 24 level-up
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

18 of 21 words, 84 against retail's 76 bytes. Clears both cursor slots of 256
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

13 of 27 words, at the exact 108-byte size. Flips a `u8` toggle, then steps
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

8 of 15 words, at the exact 60-byte size. Writes a cursor slot's `param`,
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

4 of 16 words, at the exact 64-byte size. Calls `func_00239C08(0, arg)` and
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

8 of 17 words, at the exact 68-byte size. Returns a terminal number from
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

## cmpSkillOrder (code_001FAE80, 0x50)

12 of 20 words, at the exact 80-byte size. A `qsort` comparator over the `u16`
skill list: zeros sort last, otherwise ascending.

Retail copies the first argument into `$3` as its first instruction and reads
`*a` through the copy both at the top and again in the tail compare. Every
shape tried loads straight through `$4`, so the copy is missing and the rest of
the registers shift with it. This is the same missing pointer copy recorded for
`func_00213190` above, which walks the same list; both comparators are one
word short on it.

The rest is settled: `*b` belongs in the `bnez`'s delay slot, the tail
re-reads `*a` rather than reusing the halfword from the zero test, and the
final three-way result is `if (b < a) return 1; if (a < b) return -1;
return 0;`, which gives retail's `sltu` pair and the `movz` against `$0`.

Ruled out: `u16 sa` and `u16 sb` locals for both values; a `u16 *` local for
`a` with `*pa` written at every use; the same with a second `u16 sa2` local for
the reload; two `u16 *` locals, which is the draft and the closest; the first
test written `*pa != 0` so the arms swap; and `u16 *` parameters in place of
`const void *`, which the compile rejects against the `qsort` signature.

## cmpItemHasFlag (code_001FAE80, 0x54)

13 of 17 words, 68 against retail's 84 bytes. Tests bit 0 of
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

3 of 26 words, at the exact 104-byte size. Sets bit 1 of a cursor's `unk_2C`
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

8 of 22 words, at the exact 88-byte size. Reads one of two byte tables inside
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

8 of 21 words, at the exact 84-byte size. Draws the panel's background sprite,
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

9 of 25 words, at the exact 100-byte size. Looks up a type-0x11 scene basic by
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
