# Non-matching drafts: rstcalc

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## cmpFindKugatatiMsgEntry (rstcalc, 0x60)

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

## cmbChkDevilEvo (rstcalc, 0x74)

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

## datFindKugatatiEntry (rstcalc, 0x74)

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

## cmbGetHeartsParamByIndex (rstcalc, 0x58)

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

## cmbCountEvoReady (rstcalc, 0x84)

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

## cmbGetLevelUpCount (rstcalc, 0x84)

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

## rstFindEvoEntryType5, rstFindEvoEntryType7 (rstcalc, 0x6C each)

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
