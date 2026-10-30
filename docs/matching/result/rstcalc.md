# Matching notes: rstcalc

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

The guarded typed lookup emits 108 bytes against 116 native bytes, with all
27 emitted words differing. It scans 24 level-event records after a six-byte
header. Each four-byte record contains a level byte, kind byte and u16 value;
the lookup compares the level as a low byte, returns the first matching
record and optionally writes the eight-bit match count.

The consumer `rstCalcEventInfo` now uses that record view directly. Its signed
count/index, halfword value store and signed event-kind return are recovered;
the lookup remains assembly-backed.

Native keeps `id * 0x66 + 4 * i` as a byte offset, the bare table base in `$10`
and a separate `$12 = $10 + 6`. Its level load is `lbu 6($6 + $10)` and its
returned entry address is `$6 + $12`. The typed pointer walk folds these into
one induction pointer. The source-level representation preserving both native
address forms is still unresolved; the size gap also remains.

Typed nested-row indexing emitted 128 bytes. A direct byte-offset form emitted
116 bytes but matched only two of 29 words, so correct size alone does not close
this function. Earlier integer-address and explicit two-base forms either
rematerialised the table inside the loop or added another induction variable.

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

## cmbInitStockUnit

The earlier 172-byte guarded entry-pointer form has been reduced to 160
emitted and native bytes; 37 of 40 words match. Three differing words
at `+0x20`, `+0x24` and `+0x2C` exchange the temporaries in the multiply-by-102
address expansion. All loads, conditions, stores, loop ordering and calls
match. Normal builds retain assembly.

The helper masks the devil ID to 16 bits, clears the unit's skills, then
scans all 24 level-up entries in order. A kind-one, level-zero entry appends
its 16-bit value and increments the unit's 16-bit skill count. The established
102-byte row and four-byte entry types express the complete access path.

A parity-qualified `cc1probe` run on this guarded body used `--alloc --sched
--no-dumps --cflag -DNON_MATCHING --func cmbInitStockUnit`; hooked and unhooked
assembly matched (`verify: OK`). In local allocation, UID33 combines pseudo91
into92; UID37 combines92 into94; UID39 combines94 into95. The resulting
quantity has nine references over lifetime10..26 and receives `$2`, while
the separate pseudo93 receives `$3`. Thus the three-times-ID intermediate
shares the final offset quantity. Native instead holds the three-times-ID
value in `$3` and the shifted term/final offset in `$2`. This is coalescing,
not instruction scheduling or an incorrect row size.

An ordinary signed byte-offset form and a shared table/entry index control
are inert. The direct typed-row source remains preferred. A further change
needs a genuine source lifetime or access-path premise that explains the
different temporary combination; manually expanding the constant multiply
is not justified by this observation.

## rstSetCurrentDevil (176 bytes, matched)

The signed stock scan is now exact at 44/44 words. Existing signed and unsigned
union views of the index byte express retail's `lb`/`lbu` observations without
volatile casts. A real `DDS3_GBWK` snapshot before the index-byte store supplies
the stock access; the later global observation supplies the unit pointer.
The labeled retry and shared exhaustion exit preserve native control flow,
call order, and the conditional publication of `EventStart->unit`.

An initialized success value of one has a real return-choice consumer after
`rstSetLevelUpCount` and `rstChkParamLimitAll`. A localized empty `+r` constraint
keeps that value live across the scan and calls without fixing a register.
The allocator retains it in saved `$18`; the unconstrained ordinary form sinks
one after the final call and produces 168 bytes rather than retail's 176.
This is a target-specific compiler aid, not original-source evidence or a claim
about an unmeasured compiler pass. The constraint is not a memory barrier;
the complete target output establishes the required accesses and ordering.

The inherited contract still requires valid stock and unit indices whenever
those accesses occur. The signed scan is not a new lower-bound check, and no
bounds or guaranteed retry termination under arbitrary callbacks are invented.
The byte increment is promoted before narrowing back to the stored byte.
Return zero leaves the published unit unchanged; a qualifying unit selects two
unless the parameter-limit check returns zero, selecting the retained one.

Independent complete-owner checking gives 83 match, 0 differ, including the
caller and both helpers, with owned-data checks enabled. Final-source checks
also pass the retail checksum, all 149 guarded owners / 601 drafts, seven IRX
owners, 26 tooling test files / 427 cases, shell mocks and documentation checks.
Source commit `ee3811254c1dfa0a09931f37f9ec61c5478ec292` passed exact-source
hosted CI [40977985806](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40977985806),
including build and deploy.

## rstChkSkillPowerUp2 (result/rstcalc, 0xA8)

42 of 42 words match at 168 bytes in the unsigned random interval check. The
unit ID selects one call to the existing `u32 dds3GetRandA(void *)` provider with `NULL`, which
uses the mutable global RNG state. The result is reduced modulo 254. The first
arm tests the low interval below 112 and then unsigned `roll - 112 < 142`; the
other tests below 128 and the remaining interval below 254. The shared fallback
preserves retail's return-two path, although that path is unreachable for the
actual modulo-reduced roll. No extra bounds or optimizer assumptions are added.

The ordinary shared-fallback form reaches 39/42 words at the correct size;
three words differ only in the first arm's interval temporary register. A single
localized empty operand constraint retains both real initialized values at the
boundary after RNG/modulo: the unsigned delta uses native `$5`, while the roll
uses a generic `+r` operand. The subsequent low and interval tests consume these
values, and neither constrained lifetime crosses a call. Constraining the delta
alone coalesces the roll into `$5` and changes the test ordering, so it is not the
accepted reconstruction. The paired form emits the native roll in `$4`, delta
in `$5`, and complete branch/delay-slot/epilogue sequence.

This is a target-specific compiler aid, not evidence of original source spelling
or a claim about an unmeasured internal compiler pass. The diagnostic plain
and stock-dump complete-owner assembly pair has qualified parity; this
establishes output parity for that diagnostic source, not a debugger-hook trace
of the final source. No false provider declaration,
fabricated consumer, instruction-emitting asm or new pointer bounds are used.
The existing valid `EventStart` and unit-pointer contracts remain unchanged.

Independent final-source owner checking gives 84 match, 0 differ with normal
owned-data checks, including the previously published `rstSetCurrentDevil`.
Final-source gates pass the retail checksum, all 149 guarded owners / 599 drafts,
seven IRX owners, 26 tooling test files / 427 cases, shell mocks and documentation
checks. Source commit `cf18b20a8a57745e600ca3c1467549745b9cd8f8` passed exact-source
hosted CI [40982354001](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40982354001),
including build and deploy.
