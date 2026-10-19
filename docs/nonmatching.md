# Non-matching drafts

Reconstructions that are close but not exact, kept in the source behind
`#ifdef NON_MATCHING` so the next person starts from the draft instead of
from scratch. The build never defines `NON_MATCHING`, takes the assembly
fallback, and stays byte-identical; the function still counts as unmatched.

The source carries one line saying how close the draft is and what differs.
This page carries the rest: what the remaining difference actually is, and
which source shapes have already been ruled out. Read the entry before
picking a draft up, or you will repeat the search.

A draft is not evidence of anything. It is not compiled by the build, so
`python3 tools/check_nonmatching.py` compiles every draft and checks that each
has an entry here.

**Write the measurement as "N of M words match", never "N of M words".** The
bare form is ambiguous and has already misled: an entry reading "10 of 146"
was taken for a near miss when it meant ten words matching out of 146, a
draft 135 words out. Say `match` or `differ` explicitly, and give the emitted
size against retail's when they differ.

## fldSkyDrawFilter (field/fldDraw, 0xAD8)

682 of 694 words, at the exact 2776-byte size. Transliterated from
dds-decomp's `fldDrawSkyGradient`, the same function at the same size, which
is the one dds pairing that has held exactly.

The remaining twelve words are three reload spill slots rotated. Retail
numbers them `0x118` `fldSkyDataTable`, `0x11C` `row*8`, `0x120` `row*32`;
the draft gives `row*8`, `row*32`, `fldSkyDataTable`. Every instruction is
otherwise identical, so it is purely which pseudo-register gets which slot.
`-fno-gcse` moves 653 of the 682 words and `-fno-strength-reduce` changes
nothing, which puts the mechanism in GCSE and reload pseudo numbering rather
than anything the source can state. Someone attacking this needs to change
what order the pseudos are created in, not what the source says.

Ruled out: a `fldSkyData_t *` local at four scopes, which gcc folds back to
the global with byte-identical output; the flattened colour array; pointer
and `(p + i)->` subscript forms; `<<` for `*`; a `mode` local for the switch;
an if/else chain for the switch; `u32` loop indices; a row-base `s32 *` with
`cur[column+9]` and `cur[column+10]`; alpha extracted before rgb; hoisted `x`
and `y`; and nine declaration-order permutations.

Confirmed on the way: `fldSkyDataTable`'s row stride is
`{s32 mode; s32 colors[8][9];}`, 292 bytes; `rsetOtOfs` takes `0x39` here
where DDS1 passes `0x44`; the sway offset global is `fldRunDanteGunFlag[14]`,
not `[16]`; and the clamp list is three alphas with `a3` unclamped.

## fldStatusSet (field/fldDraw, 0xF8)

36 of 62 words, at the exact 248-byte size.

Retail carries two induction variables where the draft has one: an integer
offset added to `DDS3_GBWK` serving `hp` and the first `badstatus` read, and
a pointer biased at the `badstatus` field serving the flag at `-0xE`. The
body never uses the loop index.

Ruled out: the typed `akuma[i]` form throughout; a `fldAnahoriAkuma_t *`
local at three scopes; all eight cast orders of `(u8 *)base + ofs` against
`(u8 *)ofs + (u32)base`, which gcc folds together here; a `u16 *` walking the
`badstatus` field with the flag at `p[-0xE]`; and an explicit second pointer
induction.

This draft proved `fldAnahoriAkuma_t.badstatus` at `+0x12`: retail tests that
`u16` against 0x40, 0x100, 0x20 and 0x8000, and `datCheckDying` is called on
`&akuma[0].flag`, which makes the record `datUnitWork_t` shifted by four and
puts `badstatus` at `0x0E + 4`.

## nbCameraBoss_BSKAGU_EP_IT_Set (code_001B0858, 0x144)

19 of 79 words before the first difference. The four-case switch and all four
camera store lists are exact; only the selector that feeds the switch differs.

Retail computes the selector in two arms that both end with the same
`movz $4, $3, $2`, so the arms cross-jump onto one shared copy of it: each arm
sets `$3` to the value for an even draw (3 or 2) and `$4` to the value for an
odd one (0 or 1), then falls into the shared `movz`. ee-gcc here emits a
`movn` inside each arm instead, with the result register preset to the even
value and the odd value moved in. Because the moved operand differs between
the arms (`$0` against `$17`), the two `movn`s are not identical and do not
cross-jump, so the arm bodies stay separate and the allocation rotates from
there on. The needed form is an `if_then_else` whose RTL condition is `EQ`,
which gcc's folding turns back into `NE` for every source spelling tried.

Ruled out: `(x & 1) == 0 ? 3 : 0`, `!(x & 1) ? 3 : 0`, `((x & 1) ^ 1) ? 3 : 0`,
an explicit `if`/`else` per arm, a `u32` temp holding the masked draw, and
initialising the selector to 0 before the `if`.

## nbCameraBoss_BSSAMA_Command_Set (code_001B0858, 0x1CC)

25 of 115 words before the first difference, 8 bytes short of the retail size.
The instruction list is otherwise the same work in the same order.

Retail forms the address of the local `center` vector twice, `addiu $2, $29,
0x20` for `SDF_VU0_STOREVECTOR` and `addiu $3, $29, 0x20` again for the
`SDF_VU0_LOADVECTOR` after `effBTLModelGetHeight`. ee-gcc here CSEs the two
into one pseudo that is live across that call, so it lands in `$16` and the
two `addiu`s collapse to one. That costs a callee-saved register and rotates
the float registers in the `pBase->h * pBase->scl * 0.35f` multiply just
above. Both `addiu`s are `$29`-relative, so the equivalence class survives
the call in cse.c and nothing in the source appears to break it.

Ruled out: a local for the height instead of negating the call result in
place, both multiply operand orders, `+=` against an explicit
`target.y = target.y + ...`, reordered local declarations, and the
`_MEMOPS` VU0 variants (which address the frame directly and drop the
`addiu` entirely).

## nbCameraBoss_Sukady_DAIJIS_Set (code_001B0858, 0x134)

4 of 77 words, at the exact 308-byte size and with the same instruction list.

In case 0 the camera's `direction.x` comes from a `lui`/`%lo` global rather
than a `.lit4` literal. Retail schedules that `lwc1` fifth among the six value
loads, after the four `.lit4` loads for `direction.y`, `direction.z`,
`target.x` and `target.y`; this emits it second, right after `direction.y`, so
four loads rotate. The `lui` is already in the `beqz` delay slot in both, and
the store order and every other case are exact, which leaves only where the
scheduler places that one load.

Ruled out: declaring the symbol `extern f32 D_0054F3A0[]`, `extern f32
D_0054F3A0[4]` and `extern sdfVector4_t D_0054F3A0`, read as `[0]`, `*p` and
`.x` respectively. Moving the `direction.x` assignment later in case 0, so the
load has fewer predecessors to be scheduled against, does not delay it either:
placed fifth or last among the six it rotates `$f4`-`$f7` to `$f0`-`$f3` and
goes to 17 differing words, so the draft's source order is the one that gives
retail's fp registers and only the scheduler's placement of that one load is
left.

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

## nbAddOrder (newbattle/nbStageProcess, 0x9C)

17 of 37 words, 148 bytes against the native 156.

Retail keeps the start index and the loop counter in two registers with a
copy between them: `$4` holds `orderindex + 1` and is used only to form the
`&data->order[start]` address, and `$5` is the counter, set from `$4` by a
`daddu`. The draft coalesces both into `$4`, so the loop body is one
instruction shorter and `ee-as` then pads it with one nop fewer, which is
where the 8-byte size difference comes from.

Ruled out: an explicit `s8 *p` walking the array alongside the counter; a
separate `start` local with `data->order[start + (i - start)]`; a `while (i <
12)` head test; a `u32` counter; and the plain `for (i = ...; i < 12; i++)`
form, which gcc rotates and which costs a duplicated test.

## nbSetNextOrder (newbattle/nbStageProcess, 0xAC)

10 of 44 words, 176 bytes against the native 172.

Retail recomputes `data + 8 + (s8)orderindex` with an `addu` on every
iteration and reads the order byte at `0x40` of it. The draft strength-reduces
that to an induction pointer set up in the loop preheader, which adds the
preheader block and the extra 4 bytes. Retail also holds the counter in `$4`
across the whole function, loading the initial `orderindex` into `$2` and
copying it, where the draft loads straight into `$4`.

Ruled out: `data->orderindex` used directly in the loop rather than an `s8`
local; an `s32` local, which drops the `sll 24`/`sra 24` pair retail has; and
a `do`/`while` on `data->order[data->orderindex] < 0`.

## nbGetRandamStock (newbattle/nbStageProcess, 0xB4)

4 of 44 words, 176 bytes against the native 180.

The same mechanism as `fldStatusSet` above, over the same records. Retail
keeps one induction variable holding the byte offset into `dds3GlobalWork_t`,
starting at `0xA50` and stepping `0xEC`, plus two base registers, `DDS3_GBWK`
for the `badstatus` and `hp` reads and `DDS3_GBWK + 4` for the flag, and adds
a base per use. Every source shape tried folds the base into the induction
variable instead, giving two induction pointers and one `addu` fewer.

Ruled out: all accesses inline through `DDS3_GBWK->akuma[i]`; all accesses
through a `fldAnahoriAkuma_t *` local; the flag inline with the other two
through a local, which is the split retail's two bases suggest; and a
`dds3GlobalWork_t *` local for the whole loop.

## nbSetBadstatMotion (newbattle/nbStageProcess, 0xA8)

6 of 42 words, at the exact 168-byte size.

Retail holds `data` in `$4` and `data->party` in `$6`, computing the party
address as `addu $5, $6, $5` with the two `nbGetUnitWorkPointer` arguments in
their final registers before the four constant stores are emitted. The draft
loads `data->party` into `$4` first, so the `daddu $4, $2, $0` that moves
`data` into place lands one instruction later and the three following stores
shift with it. The instruction multiset is identical; only the order and two
register numbers differ.

Ruled out: the party argument inlined at the call; a `struct nbParty_t *`
local assigned just before the call rather than at its declaration; and all
24 orders of the four constant stores in each of those three shapes.
`nbSetBadstatMotion2`, which is the same shape without the float argument and
the `work` local, matches with the argument hoisted into a local above the
stores, so the difference is register pressure from `$17` holding `work`.

## nbInitActionProcess (newbattle/nbStageProcess, 0xA0)

6 of 40 words, at the exact 160-byte size.

Every instruction matches; the six differing words are the two temporaries
of the `(data->unkE4 + 1) % 4` sequence, which retail puts in `$9` and `$10`
and the draft puts in `$5` and `$6`. Nothing in the source says which, so
this needs whatever makes gcc pass over `$5` through `$8` at that point, not
a different expression.

Ruled out: `data->unkE4++; data->unkE4 %= 4;`, an `s32` local holding the new
index, `& 3` in place of `% 4` (which drops the `movn` retail has, 31 words),
and computing the modulo after the `InitActionProcessData` call (24 words).

## pbSetPanelModel (code_001D1470, 0x60)

2 of 23 words, 92 bytes against retail's 96.

Retail puts the `index` parameter in `$16` and the `0xC` work pointer in
`$17`; the draft swaps them, so the work pointer loads in the
`jal pbGetPanelField` delay slot instead of the `beqz` one and the final
`sw` names the other register. Nothing else about the body differs. The
two pseudos are created in parameter order, so whatever decides this is
local-alloc priority rather than anything the source states.

Ruled out: the `work` local at function scope and at block scope inside the
`if`; storing through `panel->data.model->modelIndex` with no local at all;
and moving that store ahead of the `pbSetModelRotate` call.

Confirmed on the way: `pbSetModelRotate` really takes `(mdlObj_t *, f32, f32,
f32)` and `func_001D2D00` takes `(pbPanel_t *, s32)`, even though both bodies
ignore everything past the first argument. Widening the two definitions is
what lets any caller in this unit be written at all.

## func_001D4EB0, func_001D4C08 (code_001D1470, 0x58 and 0x94)

21 of 22 and 36 of 37 words, both at the exact retail size.

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

## moveBlock (code_001D1470, 0x8C)

30 of 31 words, 124 bytes against retail's 140.

Retail is three instructions longer in two ways the draft has not reproduced.
It holds the `D_00327370` delta row in three copies of the same pointer
(`daddu $6, $5` then `daddu $2, $6`) and reads `delta[0]`, `delta[1]` and
`delta[2]` through a different register each; and it materialises the `-1`
return in the `jr` delay slot of its own epilogue at the end of the function,
while the draft hoists `addiu $2, $0, -1` into the entry block. It also loads
all three of the object's `0xC4`/`0xC8`/`0xCC` floats before the first add.

Ruled out: `f32 *delta` plus `delta[i]`; `*delta++` three times; three
separate `D_00327370[index * 3 + n]` subscripts; three `f32` locals holding
the object's coordinates; `u16` against `s32` for the `step + 1` temp, where
`u16` adds an `andi 0xffff` retail does not have; and loading `work->step`
once into a local against re-reading it for the store.

Confirmed on the way: `pbRoundBlock_t` is `{s16 index; u16 step; u8 [4];
pbBlockObj_t *block;}` and the block object's position floats are at `0xC4`,
`0xC8` and `0xCC`, which `deleteRoundBlock` and `pbInitMapBaseMat` both agree
with.

## pbInitGameCamera (code_001D1470, 0x94)

26 of 38 words, 172 bytes against retail's 148.

The three-way dispatch on `D_0054F5C4->charIndex` tests the middle case
first in both, as `balance_case_nodes` requires, but the two then diverge:
retail follows the `== 1` test with a plain `beqz` for case 0 and a `beq`
against 2 for case 2, while gcc emits `slti $4, 2` and a second branch for
the low subtree, which costs the extra words and leaves each arm its own
epilogue. Retail also takes the `== 1` test as `beql` with the
`lui %hi(aViewEye)` annulled in the delay slot, the same branch-likely
pattern as the two drafts above.

Ruled out: `switch` with the case bodies in source order 0, 1, 2 and in
order 1, 0, 2; `return` per arm against `break`; an `else if` chain in
retail's test order, which drops to 140 bytes but loses the dispatch
entirely; and an `s32` local for the index. The index field is `s32` at
`0x33C`, which the two matching readers of it in this unit agree with, so a
narrower type is not the answer.

## pbStartErrorDraw (code_001D1470, 0xBC)

2 of 47 words, at the exact 188-byte size.

Retail ends the function `sw` data, `sw` flags, then
`jal dds3SetProcessData` with the `lw 0x0(panel)` argument load in the
delay slot. The draft emits that load first and leaves one of the two
stores in the delay slot instead. Every other word, the two allocations,
the conditional `dds3LinkProcess` and the three work-field stores included,
is identical. This is the delay-slot scheduling case
`docs/matching-levers.md` lists as a place to stop.

Ruled out: `panel->data.error` before `panel->flags` and after it (both
orders put the load first, at 3 and 2 differing words); the work allocation
folded into the data store, `panel->data.error = work = sdfChip...`, which
hoists all five stores and costs 6 words; the allocation moved above
`dds3StartProcess`, which costs 45; and a dead `panel->process =
panel->process` after the call.

The five sibling starters in this unit, `pbStartDummy`, `pbStartClear`,
`pbStartBackGround`, `pbWaitLoadMapData`, `pbStartDrawMapCursor`,
`pbStartMenu`, `pbStartRecord` and `pbStartHelp`, all match exactly, and in
every one of them the statement before `dds3SetProcessData` is a `memset`
call rather than a store, which is what leaves the argument load as the only
instruction free to fill the slot. That is the difference to attack.

Confirmed on the way: a 4-byte `memset` has to be written through a
`void *` local. With a typed `pbPanelValue_t *` destination ee-gcc expands
the builtin to a single `sw $0` where retail calls `memset`; the 8-byte and
0x1E0-byte calls in the siblings are not expanded either way.

## pbInitMapBaseDraw (code_001D1470, 0xC4)

3 of 49 words, at the exact 196-byte size.

The same delay-slot scheduling as `pbStartErrorDraw` above, mirrored.
Retail loads the `dds3SetProcessData` process argument, moves the panel
into `$5`, then calls with the `sw arg, 0x4(work)` store in the delay slot;
the draft emits the store ahead of the call and leaves the argument load in
the slot. Both orders are legal before the call, so this is which of the two
gcc's delay-slot filler picks, not what the source states.

Ruled out: the store after `dds3SetProcessData` in source order, which
costs 5 words instead of 3; a typed `pbMatWork_t *` local instead of the
`void *` one, which drops the `lw 0xC(panel)` reload retail has; and the
`pbInitMapBaseMat` call moved ahead of `dds3SetProcessData`.

Worth knowing before picking this up: `pbStartBlockDataBase` had the
matching problem one step further along and did come out, by holding
`sdfMatNew`'s result in a local instead of reading `panel->data.blockDB->mat`
back for the `sdfMatSetTex` argument. No equivalent lever exists here, since
the value being stored is already a parameter.

## slCopyNonZero (code_00144798, 0x40)

14 of 16 words, at the exact 64-byte size.

The function copies byte pairs, skipping a zero lead byte, and null
terminates. Retail stores the trail byte, decrements the counter, then
advances `src`; the draft advances `src` first. The two words are the `sb` and
the `addiu $5, $5, 1` swapped, with every other instruction identical, so it
is instruction scheduling with nothing in the source to steer.

Ruled out: six statement orders for the three tail statements; the plain-if
and if/else forms of the lead-byte test (the if/else is what gets the first
fourteen words, the plain if loses seven); `*dst++ = *src++`; subscript and
temp-variable forms of the trail store; `src++` before the store with
`src[-1]`; and five loop forms (`for` up, `for` down to 0 and to != 0,
`do`/`while`, `while` with `count--`, and a `u32` index).

Confirmed on the way: the lead-byte test is a signed `lb` and both copies are
unsigned `lbu`, which comes from `u8 *` parameters with `*(s8 *)src` at the
test, not from an `s8 *` parameter.

## fldCommand_FLD_NPC_ON (code_00144798, 0x12C)

74 of 75 words, at the exact 300-byte size. `fldCommand_FLD_NPC_OFF` next to
it is the same function with `1` for the flag value and its own format string,
and is the same one word away.

The trailing `world == fldGlobalBase.area && stage == fldGlobalBase.floor + 1`
test ends on a `bnel` in retail and a plain `bne` here, with the same
`addiu $2, $0, 1` in the delay slot. Retail keeps a second copy of that
`addiu` on the fallthrough path, which the draft also emits, so the lengths
agree and only the branch-likely bit differs. That is the one ee-gcc 2.96
reorg decision the levers page already records as resisting every source
shape.

Ruled out: nested `if`s; an `||` early return; two separate `!=` early
returns; a dead store after the `fldNpcHojiRefresh` call; a cached
`floor + 1` local; `stage - 1 == fldGlobalBase.floor`; reversing both
comparison operands; and a `return 1` inside the `if` as well as after it.

## fldCommand_FLD_NPC_OFF (code_00144798, 0x12C)

74 of 75 words, at the exact 300-byte size. Identical to
`fldCommand_FLD_NPC_ON` above in both the draft and the remaining difference;
see that entry.

## fldCommand_DIR_HIT_CNTR (code_00144798, 0xC0)

32 of 42 words, at the exact 168-byte size.

Retail calls `scrSetIntReturnValue` from three places: `1` for the
`fldGb[0] < 0xA` early exit at the top of the function, `0` for the shared
failure block, and `1` again for the success block after it. The draft emits
the same 42 words but gcc cross-jumps the two `scrSetIntReturnValue(1)` sites
into one and relocates the merged block to the end, so the early exit becomes
a forward `b` and every block after the first branch shifts. Nothing in the
draft is semantically different; the question is what stops gcc 2.96 merging
two identical call blocks that differ only in whether they end with a `b` to
the shared epilogue or fall into it.

The two newest levers were both tried here. The early-return shape per block
is what gets the 32 words, and it is also what matched
`fldCommand_ACT_WIN_DSP` and `fldCommand_ACT_WIN_DSP2`, the two neighbours
with the same cross-jumped tail; for those two the dispatch additionally
needed a `switch` with `case 1` written before `case -1`, the source-order
rule. Neither helps here because the merge gcc performs is one retail did
not.

Ruled out: the early-return-per-block shape; nested `if`s with one
`scrSetIntReturnValue(0)` at the end; an if/else chain with a single trailing
`return 1`, which costs six extra words; wrapping the body in
`if (fldGb[0] >= 0xA)` so the `1` call has a single site, which drops to 40
words; a dead store after the first call; and storing the `fldEveHit_ChkDir`
result in a local before the test.

## fldSnowInit (code_00144798, 0x1B4)

28 of 110 words, and two words longer than retail's 436 bytes, so the draft
overruns the next function. Semantically complete: every floor test, every
constant and the `-(gFldSnowW * 256.0f * 0.5f)` wrap-around are read off the
retail code and the `.lit4` entries are written as literals, so only the
shape is open.

The difference is block ordering in the `area == 0x1F` arm. Retail places the
`gFldSnowYD = 10000.0f` else-arm far away, immediately before the shared
`sdfMemAllocDirect` tail at the end of the function, and lets the
`gFldSnowYD = 10.0f` arm fall through; the draft places the else-arm right
after the branch. Everything from the first store onwards is then shifted by
two words, and the four earlier branch displacements differ by the same
amount. `fldGlobalBase.floor` also lands in `$3` in retail and `$5` here.

Ruled out: hoisting the `area == 0x18` arm's `gFldSnowY` into an `f32` local
assigned in both arms, which is what retail does with it and buys one word;
`life = 0xF0` before the `floor == 5` test against assigning it in both arms;
and inverting the `floor == 0` test, which only moves the fallthrough to the
wrong arm.

Confirmed on the way: `gFldSnowLife` is an `s32`, not the `u16` the unit
declared, since retail stores it with `sw`; and `D_0054C578`, `D_0054C57C`
and `D_0054C580` are `.lit4` entries holding -6900.0, 10000.0 and 10000.0,
two separate pool slots for the same value.

## fldAmModelLoadFile (code_00144798, 0x174)

24 of 91 words, two words short of retail's 372 bytes, both of them `ee-as`
alignment `nop`s after a branch delay slot.

The flow is right: every instruction appears, in the same order, with the
same four format strings and the same thresholds. What differs is which saved
register holds what, and it cascades through the whole body. Retail puts the
`dir` buffer address in `$17`, a `&fldGlobalBase` pointer in `$16` and the
`%hi(fldGlobalBase)` half in `$18`, re-forming the address with
`addiu $2, $18, %lo(fldGlobalBase)` at each later use; the draft puts `dir` in
`$18` and the `%hi` half in `$17`. That is register allocation, which the
levers page lists as not worth a second attempt, so the next attempt needs to
change the order the pseudos are created in rather than the source wording.

Ruled out: a `fldAreaWork_t *work = &fldGlobalBase;` local declared first and
declared after the two buffers, both of which reproduce retail's `$16` but
not its `$17`/`$18`; and going through the global for every access, which
loses the `$16` pointer as well.

Confirmed on the way: `fldMakeFilePath` takes a third argument, `1` here; the
four `.amb` suffixes are chosen by floor+1 thresholds 4, 0xB, 0x13 and 0x1A
in area 0x22, with area 0x19 taking the `b` suffix from floor+1 >= 0x11; and
the `>= 0x1A` case falls into the same plain-suffix `sprintf` as every other
area, which needs a `goto` since it is the `else` of an outer `if`.

## getPartsType (code_0029B780, 0x5C)

15 of 24 words, four bytes long.

Retail keeps the result in `$2` and the `getPartsInfo` pointer in `$3`, so the
NULL arm returns the pointer value itself with no extra instruction. Every
source shape tried coalesces the two into one pseudo and pays a `daddu $2`
move in the tail, which shifts the whole post-call allocation by one register.
The arithmetic is settled: `(s8)((id >> 6) << 1)` gives retail's
`sra 6`/`sll 25`/`sra 24`, and the `movn` default is `base + 1` with
`base + 2` conditional on `inWay + goWay != 7`.

Ruled out: three declaration orders of the pointer and the result, which copy
propagation reduces to one pseudo with byte-identical output; the pointer
overwritten in place as the HD reconstruction has it; an uninitialised result
on the NULL path (20 of 24); the shift hoisted above the call (23 of 24); and
both operand orders of `inWay + goWay`.

## pipeMakeNodeLink (code_0029B780, 0x80)

7 of 32 words, at the exact 128-byte size, with every instruction the same.

Three registers are permuted. Retail puts the biased `&pInfo->nextId` in `$4`,
the loop bound hoisted out of `pInfo->maxNum` in `$6`, and the id read through
that pointer in `$3`; the draft swaps the first two and puts the id in `$4`,
which also swaps the `sh` and the `addiu` that follow it. The pointer pseudo
is created inside the `if` arm here, so it is numbered after the bound.
Someone attacking this needs the pointer pseudo created before the bound
without also moving its `addiu` out of the loop preheader.

Ruled out: the pointer declared at the top of the function or second after the
node (11 words and over-long respectively); the bound read into a local
instead of left for gcc to hoist out of `pInfo->maxNum` (16); no pointer local
at all, which reads `nextId` twice, once `lw` and once `lhu` (11); and all 24
orders of the four stores in the arm.

The unit's recurring shapes, for whoever picks these up. `code_0029B780` is
eleven original translation units of pipe-minigame and config code, and three
levers cover most of it: a dead store to a local after the last call, since
almost every `void` function here ends in a call ee-gcc would otherwise
sibcall; a `u32` switch index, which changes which case gcc tests first; and
`for (i = n; i != -1; i--)` for the table scans, where `i >= 0` gives `bgezl`
against retail's `bnel` against a materialised `-1`.

## pipeGetObjData (code_0029B780, 0x120)

9 of 71 words, at the exact 284-byte size, with every instruction present.

The first two arms are exact. In the third arm (`maxSize == 6`) nine preheader
instructions are permuted: retail materialises the loop counter 4, copies
`objMyData`'s result, loads `offsetCount`, then the two float constants 16.0
and 48.0, then `0xFF`, and computes `offsetCount + 4` last; the draft puts
both float constants first and the counter and `0xFF` after. The instructions
are the same, so this is which pseudo-register gets created first in the
preheader, not anything the source states.

Ruled out: three declaration and assignment orders of the `objMyData` pointer
and the offset local inside the arm, all of which compile identically.

Also measured here and worth knowing for the neighbouring effect functions:
`PIPE_EFF_t` has a 0x2C stride with `mhFile` at 0, `ptr` at 4, `d3p` at 0xC,
`pos2[2]` at 0x1C, `offset` at 0x24 and `count` at 0x28, and the unit's effect
loops are u8 downcounters from `maxSize - 2` to `0xFF`. `pipeEffDraw2Doffset`
was attempted and discarded at 63 of 64 words: it needs 0x30 of locals where a
`f32[4]` plus a 0x10 struct gives 0x20, so one local there is still missing.

## Flow6_36 (newbattle/nbMakePacket, 0xA8)

24 of 41 words, one word short of retail's 168.

ee-gcc allocates the `nextkouho` temp to `$2`, which forces it to materialise
the `return 0` once at the tail. Retail keeps that temp in `$3` and emits
`daddu $2, $0, $0` only on the else path, reusing the zero that
`evtBitCheck`'s tested return already left in `$2` on the taken path. Every
differing word is that register number or a branch offset shifted by the
missing instruction.

Ruled out: a ternary for the select; `s32` and `u32` for the temp; a store in
each arm instead of one at the tail; `!evtBitCheck(8)` and `!n->negoreqtype`
instead of `== 0`; and `n->cnt6_36 > 2` for `>= 3`. All five give the same
allocation.

## Flow2_13 (newbattle/nbMakePacket, 0xC8)

44 of 50 words, at the exact 200-byte size.

The inner dispatch is a `switch` over `qtype` with cases 1-3 written before
case 0, which is what gives retail's `beql`, `bltz`, `slti` tree instead of a
folded `sltiu` range test. What is left is cross-jumping: ee-gcc merges the
two `movn` tails of the `qindex` arms into one shared instruction, where
retail duplicates the `movn` into the branch delay slot of the first arm's
`b`. The two delay-slot fills around the `mes = 0xA` arm are also swapped.

Ruled out: nested `if`s in place of both ternaries; the `qindex` test
inverted so the other arm falls through; and a pre-set `mes` overwritten in
the second arm. The `== 0 ? low : high` form of the ternary is required, as
`!= 0 ? high : low` gives `movz` with the constants swapped.

## Flow2_2 (newbattle/nbMakePacket, 0xCC)

22 of 48 words, one word short of retail's 204.

The structure, case order and constants are right. Two allocation
differences remain: ee-gcc keeps `n->ework` in `$2` and the id in `$5`, where
retail loads the id over the pointer in `$3`; and it emits a plain `bne` out
of the `id` range test, where retail uses `bnel` with `dds3GetRandIntA`'s
first argument annulled into the delay slot.

The `(u16)` arithmetic is confirmed right: retail's `ori 0xFFDC`, `addu`,
`andi 0xFFFF`, `sltiu 4` is what the natural `id >= 0x24 && id < 0x28` range
test on the `u16` field produces, and the later `andi $3, 0xFFFF` on the id
itself comes from the same narrowing.

## nbGetVirtualAisyo (newbattle/nbMakePacket, 0xAC)

39 of 43 words, at the exact 172-byte size.

The `switch` over `-1` and `0xC..0xE` gives retail's dispatch tree exactly:
`beq -1`, `slti -1`, `slti 0xF`, `slti 0xC`. The two remaining words are
branch delay slots: retail leaves a `nop` after the first two tests, and
ee-gcc fills both with the following `slti`, which shifts the two branch
offsets. The instruction order is otherwise identical.

`nbBadCurseApplyMove` in the same unit matches with the same switch shape
over `0` and `1..2`, and there retail's nops come out, so the shape itself is
not the problem.

Ruled out: direct `return` per arm instead of `break` to a shared tail; a
separate `== -1` test in front of a `switch` over `0xC..0xE` only; the
`skillattr == -1 || (u32)(skillattr - 0xC) < 3` range chain, which folds to
one `sltiu`; and an explicit `goto` dispatch, which compiles to the same
words as the switch.

## Flow1_11 (newbattle/nbMakePacket, 0xC8)

19 of 48 words, at the exact 192-byte size.

Same bodies, same case order, same constants. ee-gcc fills the branch delay
slot in front of the last `nbDispNegoMessage` block where retail leaves a
`nop`, so every word from there on is one slot early. Retail has three such
nops after unconditional branches in this function and the draft reproduces
the first two.

The `stagemajor` range test needs an `s32` local: used directly, the `u16`
field gives `ori 0xFF10`, `addu`, `andi 0xFFFF`, `sltiu 7` instead of
retail's `addiu -0xF0`, `sltiu 7`.

## nbFreeFormResources (newbattle/nbMakePacket, 0xD8)

51 of 54 words, at the exact 216-byte size.

The three remaining words are one rotation inside the loop. Retail emits
`lw w->file[i]`, then `w->buf[i] = NULL`, then the `buf` pointer bump; the
draft emits the store and the bump before the load. Every other word,
including both induction pointers and the four tail stores, is identical.

The four tail stores have to be written `motfile`, `unk_02`, `id`, `refcnt`
to come out in retail's `refcnt`, `motfile`, `unk_02`, `id` order; that was
found by walking all 24 permutations, and only that one reaches three words.

Ruled out for the loop: `w->buf[i] = NULL` moved into both arms of the
`w->file[i]` test; both stores moved after the `file` test; and a
`w->buf[i] = NULL` duplicated into each arm of the `buf` test.

Confirmed on the way: `nbStatWork_t` carries `void *file[26]` at `+0x08`,
`void *buf[26]` at `+0x70`, `motfile` at `+0xD8` and `motbuf` at `+0xDC`,
which fills the former `pad[0xD8]` exactly and keeps the 224-byte stride that
`&data->statwork[index]` needs.

## fldAutoMapCloseBlock2 (field/fldAutoMap, 0xA8)

40 of 42 words, at the exact 168-byte size.

Retail computes `floor - 1` into `$7` in the instruction before the `div` and
`flagNumber - 1` into `$6` in the `beql` delay slot of the divide-by-zero
check. The draft emits the same two `addiu`s with the same registers, in the
other order: `flagNumber - 1` before the `div` and `floor - 1` in the delay
slot. Every other word, both arms included, is identical.

ee-gcc sinks whichever of the two locals is written later in the source into
the delay slot, so retail's order asks for `floor - 1` first. Declaring it
first does produce that order, but then the index lands in `$5` instead of
`$7` and the whole `>= 30` arm reallocates, which costs 18 words. All 6
declaration orders and all 36 declaration-order/assignment-order splits were
measured: the three with `bit` declared ahead of `index` reach 2 words and
nothing reaches 0.

Also ruled out: `floor - 1` or `flagNumber - 1` written inline at the use
sites instead of as locals (23 and 29 words), both inline (35), and the
`u32 ofs` local in the `0xFC58` arm as well as the `0x17F10` one (which costs
a word of size).

`fldAutoMapOpenBlock2` is the same function with `|=` for `&= ~`, and it does
match, from `room`, `index`, `bit` locals in that order. The extra `nor` in
the close path is what moves the allocation.

## fldAutoMapDrawIconOfsR (field/fldAutoMap, 0x244)

10 of 146 words, at the exact 584-byte size.

The function builds one textured quad for an icon. What it does is fully
recovered; what is left is register allocation.

`fldAutoMapIconOfsTable` is `s16 [][8]`: `[0]` and `[1]` are the texture
origin, `[2]` and `[3]` the size, `[4]` and `[5]` the anchor offset. The u
coordinates and the x coordinates scale by 16, the v and y coordinates by 8.
`sdfPrimLinkFt4` takes 21 arguments, the last 13 in the outgoing frame at
`0x0` through `0x60` in 8-byte EABI slots:

    sdfPrimLinkFt4(tag, color, mode,
                   x0, y0, u0, v0,  x1, y1, u1, v1,
                   x2, y2, u2, v2,  x3, y3, u3, v3,  mask, next)

`mode` selects the quad's winding: the default lays the four corners out
top-left, top-right, bottom-left, bottom-right; `mode == 8` flips the rows;
`mode == 4` transposes, taking the anchor from `[4]`/`[5]` the other way
round and the extent from `[3]`/`[2]`. The tail is two separate
`fldDrawGetOt` calls, the first supplying the argument and the second the
`regFunc` to call through, so the source reads
`((...)fldDrawGetOt()->regFunc)(ot, tag)` with `ot` from an earlier call.

What is needed to get the three `lh` loads and their shifts right: `s32`
locals for `t[0]`..`t[3]`. Read as `s16` at each use, ee-gcc loads `lhu` for
the `* 16` use and then sign-extends with `sll 16`, `sra 13` for the `* 8`
one, which adds two instructions per arm. `s16 *t = fldAutoMapIconOfsTable
[index];` as an initializer at the declaration is what gives retail's four
saved registers and the 0xB0 frame; assigning it after the three leading
calls spends one fewer and lands at 564 bytes.

The remaining difference: retail copies `x` into `$f21` and `y` into `$f20`,
the draft the other way round, so every `mul.s` and the two `cvt.w.s`/`mfc1`
pairs in each of the three arms name the other register. Both compute `y`
first, in the same statement order, and swapping the statement order does not
move the assignment. Retail also keeps `index * 16` in `$16` and rebuilds the
table address into a temp after the calls, while the draft keeps the whole
pointer in `$16`; recomputing `fldAutoMapIconOfsTable[index]` inside the
default arm does reproduce that, but costs size elsewhere.

Measured and rejected: the row addressed as `fldAutoMapIconOfsTable[index][k]`
at every use (ee-gcc builds a chain of pointer copies, 620 bytes), `x0` before
`y0` in each arm, and the four combinations of those with the pointer either
initialized or assigned.

## fldAutoMapSeqStart (field/fldAutoMap, 0x30C)

148 of 195 words, at the exact 780-byte size. Everything up to `+0x1A4` is
identical; the rest is the closing block that fills three vectors.

Recovered along the way, and used by the matching `fldInitPacLoader`:
`fldAreaWork_t.pacRes[10]` at `+0xD0` is the pac loader's handle and mapped
pointer per entry, and this function reads the mapped pointers `pacRes[7]`,
`[9]`, `[1]`, `[3]`, `[5]` straight into `sdfTexSet`. It also reads
`amFloor` at `+0x64`.

`D_00532230` is the `{0, 0, -1, 1}` up vector, assigned to a local
`sdfVector4_t` (which gives the `ldl`/`ldr`/`sdl`/`sdr` run), and the three
locals go out through `SDF_VU0_COPYVECTOR` to `D_002F5E00`, `fldAutoMapRect`
and `D_002F5E10`. `D_0054C4C4` and `D_0054C4C8` are this unit's `.lit4`
entries at file offsets `0x44D4C4` and `0x44D4C8`, holding `-24000.0f` and
`20000.0f`; the other four constants in the block (200.0, 255.0, 1000.0,
2048.0) have zero low halves and come out as `lui`/`mtc1`, so they add
nothing to the pool.

Two things were needed to reach the exact size. `fldAreaWork_t`'s light block
has to be addressed through a local pointer to `&dds3LightParam2[0x20]`, which
is what folds the `0x20` into the `%hi`/`%lo` pair instead of leaving it in
the store displacements. And the three `0.0f` stores have to go through one
`f32` local: written as literals ee-gcc emits three `sw $0`, where retail
materialises the zero once with `mtc1 $0` and stores it with `swc1`, which is
one word more. Assigning `target.y` and then copying it into the other two
works equally well (49 words out instead of 50 before the rest was ordered).

What is left: retail holds the two world coordinates in `$f3` and `$f0` and
the zero in `$f1`, the draft in `$f2`, `$f1` and `$f0`, and retail does the
up-vector copy before the float arithmetic rather than after. All 720
orderings of the six statement groups in that block were measured (the best
six tie at 47 words), as were all 24 orderings of the four `D_00551CAC`
through `D_00551CB8` stores. Nothing moves the register choice.

A forward prototype for `fldAllIconDel` is required: the unit defines it below
this function, and the implicit declaration makes ee-gcc treat it as returning
`int`.

## effPCPBossBaelCopy (effect/effPCPBoss, 128 bytes)

21 of 32 words, at the exact 128-byte size.

Pure scheduling. Retail splits the `lui`/`ori` pair that builds `0x80808080`
around the eight-instruction `ldl`/`ldr`/`sdl`/`sdr` block that copies the
16-byte parameter vector: `lui` right after the allocation, the copy, then
`ori` and the colour store. The draft keeps `lui` and `ori` adjacent and emits
them before the copy, which rotates the eight copy words by two and also swaps
which instruction fills the first `dds3EffectPartCopy` delay slot: retail takes
`sw $0, 0x20` (the frame reset), the draft takes `lw $4, 0x28`.

The sibling `effPCPBossBaelCreate` matches with the same statement order, so
the trigger is the one-argument `dds3EffectPartCopy` rather than the stores.

Ruled out: all six orderings of the vector copy, the colour store and the frame
store, each with the copy written both as `memcpy` and as a struct assignment
(twelve compiles, every one 11 or 12 words off); the colour in a `u32` local
initialised at its declaration; the two `dds3EffectPartCopy` results through
named locals; `sizeof` instead of a literal allocation size. Caching
`src->modelResource` and `src->auxResource` in locals first is far worse, 31 of
32.

## effPCPBossKAGUThunderboltCopy (effect/effPCPBoss, 176 bytes)

21 of 44 words, at the exact 176-byte size.

Register allocation. Retail keeps the source work in `$17` and the new work in
`$16`; the draft has them the other way round, which touches every instruction
that names either. The three sibling copies in the same unit
(`effPCPBossDengekiCopy`, `effPCPBossKAGUAttackCopy`,
`effPCPBossKAGU2AttackCopy`) match with the identical statement shape and the
opposite assignment, so the trigger is the extra `effPCPCrackCopy` call rather
than the body. The leftover two words are a rotation of the `src->bed` load
against the `1.0f` materialisation.

Ruled out: the `effPCPCrackCopy` result stored directly against held in a
named local; reading `src->bed` before the crack copy instead of after;
storing `work->crack` before `work->bed` instead of after.

## effPCP_BED_Create (effect/effPCPBoss, 248 bytes)

21 of 62 words, at the exact 248-byte size.

One source fact is missing. Retail materialises `0x80808080` twice: once as
`lui`/`ori` before the parameter copy for `work->color`, and again inside the
`bedParams` arm for `place.color`, with the second `ori` filling the guard's
delay slot. Every C shape tried lets ee-gcc common the two constants into one
register that stays live across the guard, which costs a register and rotates
the whole arm. The structure around it is right: the 12-byte parameter copy as
one struct assignment (one `ldl`/`ldr` pair plus an `lw`/`sw`), the two VU0
identity macros, and the placement block written `payload[0..10]` then `color`,
which is the shape that matched for the five sibling copy routines.

Ruled out for the duplicated constant: the `work->color` store in all four
positions around the parameter copy and the two VU0 macros; `place.color`
written before the payload instead of after; an early `return work` in place of
the `if`; the literal as `0x80808080u`, `-2139062144` and `0x80808080 | 0`;
`place` and `bed` declared in the guarded block instead of the function body
(every combination). All land on 41 or more differing words except the four
listed positions, which land on 41, 41, 42 and 51.

Worth knowing before retrying the bigger `Create` routines in this unit: they
set a work colour and a placement colour the same way, so whatever defeats the
common subexpression here should carry several of them.

## effPCPCrackCopy (effect/effPCPBoss, 292 bytes)

63 of 73 words, at the exact 292-byte size.

Scheduling of the `0x80808080` materialisation. `effPCPCrackCreate` next to it
matches with the identical body, so the 0x8C-byte head copy as one struct
assignment and the store order are right. In the copy, ee-gcc builds the colour
into `$2` before the tail of the block copy, so the tail's two temporaries land
in `$5` and `$6`; retail uses `$2` and `$5` and builds the colour after the tail
stores. The other six words follow from that: the `src->part` load and the
colour store swap sides of the `dds3EffectPartCopy` delay slot, the same way
`effPCPBossBaelCopy` does, and both are one-argument calls.

Ruled out: all six orderings of the head copy, the colour store and the
`unk9C` store (four of them are far worse, two land on 10); `unk9C` as an `f32`
written `0.0f`, which reaches 11.

## effPCPDANTE_Thunder_ProcessDraw (effect/effPCPBoss, 172 bytes)

28 of 43 words, one word short of retail's 44.

Retail computes `$sp + 0x40` twice: once into `$2` for the `sqc2` that stores
the mark position, and again into `$5` in the `effPCPDANTE_Thunder_SetPosition`
delay slot. Every C shape tried lets ee-gcc common the two into one register,
which it then allocates straight to `$5`, so the draft is one instruction
shorter and everything after the `sqc2` shifts. The matrix half is already
exact, including `SDF_VU0_STOREMATRIX_MEMOPS` for the four frame-addressed
`sqc2`s against `SDF_VU0_STOREVECTOR` for the register-addressed one.

Ruled out for the duplicated address: the position as a bare `u128`, as a
one-element array, and as the fifth element of the matrix array (so the two
locals are one `u128 buf[5]`); a `u128 *` local assigned just before the macro
and `&position` at the call.

The same obstacle blocks `effPCPBossKAGU2MUHENKDraw`, which is otherwise
straightforward: retail computes the mark index `i + 0x14` separately for its
`effBTLModelGetMarkPosition2` and `sdfModelGetMarkPosMatrix` calls, and ee-gcc
commons the two into one saved register, leaving the draft a word short. Its
structure is known: the shared invoke-model search that
`effPCPBossThunderboltDraw` now carries (with a `u32` index and ids 0x125 and
0x108), then a seven-iteration loop that stores the mark position, calls
`effDebugPointDraw`, loads `(0, -1, 0)` through `SDF_VU0_APPLYMATRIX` and draws
a 200-unit line with `effDebugLineDraw`.

## effPCPDANTE_ShowTime_Draw (effect/effPCPBoss, 300 bytes)

71 of 75 words, at the exact 300-byte size.

One instruction's schedule slot. Retail places the `mov.s $f12, $f20` that
passes 0.45f to the first `nbPanelFlashInit` directly after the two `lui`s for
its colour arguments; the draft emits it three slots later, right before the
`jal`, which rotates four words. Everything else is exact, including the three
`.lit4` constants as literals: `D_0054C9D0`, `D_0054C9D4` and `D_0054C9D8` sit
in this unit's pool at file offsets 0x44D9D0, 0x44D9D4 and 0x44D9D8 and hold
0.45f, 0.35f and 0.55f, with 0.45f shared by the first two calls, so ee-gcc
keeps it in `$f20` and the pool gets one entry for it.

Ruled out: 0.45f through a named `f32` local, declared before `frame`, after it,
and at the top of the guarded block. The first two are far worse (67 words),
because the load then happens outside the guard; only the block-scoped one stays
at four.

## nbEventProcess1 (newbattle/nbMakePacket, 0x7C)

23 of 31 words, at the exact 124-byte size and with the same instruction
multiset.

Retail schedules `addiu $2, $2, %lo(nbEventProcess3)` into the load-delay
slot after `lw $4, 0x88($17)`, four words after its `lui`. ee-gcc leaves the
`lui` in the same place but emits the `addiu` last, where the return value is
used, so the eight words from there on sit one slot late. Nothing else
differs.

Ruled out: a `void *` local holding the return value, which costs a saved
register and adds four words; an early `return nbEventProcess3;` for the
`rid <= 0` case, which duplicates the `lui`/`addiu` pair; and swapping the
two `rid` stores, which only moves the stores.

Confirmed on the way: `nbEventEntry_t` +0x00 is an `s32` resource id, not a
pointer. Retail stores it into `data->eventrid` after a signed `blez`, and
`nbStartEventPacks` still matches with the field as an `s32` and
`evtLoadEventPack` taking one. A local for the field is needed because the
store to `data->eventrid` otherwise aliases the table and forces a reload.

## evtEventViewerProcess3 (event/evtEventViewer, 0x16C)

88 of 91 words, at the exact 364-byte size.

Three consecutive words, the second `viewerinfo->flags & 1` test after
`FrameSelect`: retail loads the field into `$2`, masks it there and branches
on it, letting the `return 0` in the branch's delay slot overwrite the same
register. The draft puts the temp in `$3` because it treats `$2` as taken by
the return value. Every other word, including both jump targets, the three
`evtViewerStopFrame` sites and the shared flags load under the pad `0x2C`
test, is identical.

The whole function body had to go through `viewerinfo->` directly: a local
`evtViewer_t *viewer` reassigned after each call costs a saved register and
misses by 58 words.

The frame advance at the end reads `if (frameCurrent >= frameLast)` with the
`evtViewerStopFrame` arm first; written as `frameCurrent < frameLast` with
the increment first, gcc places the arms the other way round and the whole
tail shifts.

Ruled out for the three words: `& 1` with and without `!= 0`, `== 1`, an
early `return 0` per arm in the frame advance, a `ret` variable for the
single exit, and restructuring the two flags tests as one `!= 0` call guard
plus one `== 0` tail guard. `FrameSelect` and `evtViewerStopFrame` are
defined in this unit, so the `s32`-return lever that blocks the `$2`
reservation is not available for them.

## nbCameraShot_PullOne_Init (newbattle/nbCameraSkill, 0xE8)

56 of 58 words, at the exact 232-byte size.

One store is scheduled differently. Retail emits `div.s`, then
`swc1 pCamera->distance`, then the two `sw $0` that zero `angles.x` and
`angles.z`, then the `lqc2` of `pForm->rot`. The draft emits the two `sw $0`
first: cc1's second scheduling pass fills the `div.s` latency with them and
sinks the `swc1` behind, which retail's cc1 run did not do.

All six orders of the three statements give either `sw, sw, swc1` or, once
the `SDF_VU0_LOADVECTOR` moves above the two zero stores,
`swc1, lqc2, sw, sw` with the function one word shorter. Also ruled out:
zeroing `angles.y` as well (dead store, one word longer),
`angles.x = angles.z = 0.0f` (one word longer), a `volatile sdfVector4_t`
(longer), `SDF_VU0_LOADVECTOR_BARRIER` in place of the plain load (same
sinking), and writing the zeros through a `sdfVector4_t *` local so the two
stores alias `pCamera->distance` (unchanged).

The rest is exact, including `pForm->r > pForm->h` for the load order of the
compare, the `0.69813163f` and `0.78539807f` pool literals, and the
`-1.0f`/`0.0f` direction stores in `x, z, y, w` source order.

## nbCameraState_SkillStart_Init (newbattle/nbCameraSkill, 0x78)

29 of 30 words, at the exact 120-byte size.

The one differing word is the `addu` operand order:
retail `addu $16, $16, $2` (base, index), the draft `addu $16, $2, $16`. Both
write the same register and keep the `lbu 0x2` displacement, so only the
commutative operand order inside the address differs.

Retail also keeps `datSkillTable + 0x4388` in the relocation addend, which
only a local pointer base reproduces: `datSkillTable.visual[id].hatudo`
written directly folds the whole `0x4388 + 2` into the `lbu` displacement and
leaves the relocation at addend 0.

Ruled out for the operand order, all giving the identical `addu $16, $2, $16`:
`pVisual += effBTLSkillGetID()`, `pVisual = pVisual + id`,
`pVisual += (s32)id`, `pVisual -= -(s32)id`, a `u8 *` base with
`+= id * 4` and `+= id << 2`, a `u32` base with `+= id * 4`, a separate
`u32 ofs = id * 4` added to either base type, both casts
`(u8 *)pVisual + ofs` and `(u32)pVisual + ofs`, and the reversed
`id * 4 + (u32)pVisual`. Subscripting (`pVisual[id].hatudo`) instead of
assigning the pointer costs two more words by moving the result into `$3`.

## nbCameraState_SkillAttack_Calc (newbattle/nbCameraSkill, 0x170)

85 of 91 words, at the exact 364-byte size.

Two delay slots that retail fills from in front of the branch and ee-as
leaves empty here, because cc1 placed the instruction after the branch
instead.

The first is `nbCameraSkillAttackStep`: retail has a plain
`bnez` with `addiu $2, $2, -1` in its slot, so the decrement was computed
before the test; the draft's decrement sits in the `else` arm, so ee-as makes
the branch a `bnel`. Computing `step - 1` into a second local before the test
does produce the plain `bnez`, but the subtraction then uses `$3` instead of
retail's `$2`, which costs two words instead of one, so the plain form is
kept. `--nbCameraSkillAttackStep == 0` as the `else if` condition changes
nothing.

The second is the `nbCameraModelMotionCr < 450.0f` test inside the
`step == 0` arm: retail fills the `bc1t` slot with `daddu $4, $18`, the
`pCamera` argument of the fall-through call, so cc1 had hoisted that argument
above the branch; the draft emits it inside the arm and the slot stays a
`nop`, rotating the five words of that block by one. Inverting the test to
`>= 450.0f` so the arms swap gives eight differing words instead of six.

Everything else is exact, including both `effBTLSkillGetInvokeParty` and
`effBTLSkillGetTargetParty` being called before the `stat & 0x200` test
rather than short-circuited, `effBTLModelGroupPopPosition(targetParty)`
reusing the `$4` the target-party call left, and the `AttackTuibi_Set` arm
written last so the two guards branch forward to it.

## slSaveData_writeFile (mc/slMain, 0x238)

106 of 142 words, at the exact 568-byte size.

The 36 remaining words are one register renaming carried through the whole
record-copy block. The instruction sequence is identical; retail holds the
byte offset `52 * slot` in `$5` and `%lo(D_00569740)` in `$2`, and the draft
holds the offset in `$2` and the base in `$7`, so every address temp after it
shifts by one number. Nothing in the block differs otherwise.

What the shape needed, and is worth keeping:

- The save header lives at offset 0 of the save work, so the stores go through
  `((slFileHead_t *)DDS3_GBWK)` written out per statement; that reloads
  `DDS3_GBWK` for every field, which retail does too.
- Two types for the same 52 bytes. The header in the save work is 4-aligned
  (its 20-byte block at `+0x10` copies as `ldl`/`ldr` plus a plain `lw` for
  the trailing word), while the on-card record array is byte-aligned (the same
  block copies as `ldl`/`ldr` plus `lwl`/`lwr`). One struct for both gives the
  wrong half either way.
- The record side is a `u8 (*)[52]` view. Five of the thirteen field stores
  (`+0x1`, `+0x2`, `+0x8`, `+0x10`, `+0x24`) go through the subscript and the
  other eight through a cached `u8 *rec`: that mix is what produces retail's
  two `addu` operand orders and its three copy temps. All thirteen through the
  pointer is six words short, all thirteen through the subscript is twelve
  words over.
- Source order ends `playTime`, `endcnt`, `newfile`, `mode`, `name`,
  `mapOpen`; putting `mode` back in field order after `level` costs nine
  words.

Ruled out: the declaration and assignment order of the two pointer locals;
`rec` as `u8 *` and as `u8 (*)[52]`; a separate `base` local so the symbol
address is created first; `slot` as `u32`; writing the `+0x8` store
base-first and the `+0x1`/`+0x2`/`+0x24` stores index-first with explicit
casts; and five orderings of the last six stores.

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

## lstPageUp, lstPageDown (code_001FAE80, 0x74 each)

28 of 29 words each, at the exact size. Each steps the list cursor up or down
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

## rstFindEvoEntryType5, rstFindEvoEntryType7 (code_001FAE80, 0x6C each)

24 of 27 words each, at the exact size. Both scan a devil's 24 level-up
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

## nbCameraBoss_Cut1_Run (newbattle/nbCameraBoss, 0x1DC)

33 of 117 words, two words short of retail's 119.

The dispatch is a `switch` on `effBTLModelGetID` of the invoke formation whose
arms each run one `nbCameraBoss_*_Cut1_Run`. Retail does that as `jal callee`,
then `b` to one shared epilogue with `ld $16` in the delay slot, with the result
left in `$2` and never copied; the default arms put `daddu $2, $0, $0` in a
compare's delay slot.

Two facts got the draft from 59 words to 33:

The arms must only call, not assign. `return callee(pCamera);` tail-calls every
arm (30 words over) and `ret = callee(pCamera); break;` with one trailing
`return ret;` allocates `ret` to `$3`, costing a `daddu` per arm. Dropping both
and letting the function fall off the end of the `switch` leaves the callee's
value in `$2`, which is what retail returns.

The last arm's failing guard must fall into `default`, not run its own
`return 0`. With `case 0x158: ... else { return 0; } break; default: return 0;`
the `skill` local lands in `$4` and every compare against it shifts; writing the
arm as two `if (...) { call; break; }` tests followed directly by the `default:`
label puts `skill` in `$3` as retail has it. A nested `switch (skill)` for that
arm also frees `$3` but runs three words over.

What is left is two branch delay slots. On the tests whose arms call
`BSARI_MASEHA_Run` and `Lusi_Cut1_Set`, retail leaves a `nop` and uses `bnel`
where the draft fills the slot with the following `addiu $2, $0, <const>` and
uses `bne`. Those two callees and `Pish_MUHENK_Run` are the only arms still
`INCLUDE_ASM`, and this is the branch-annul signature of the "a caller can need
its callee's definition" row in [matching-levers.md](matching-levers.md): the
remaining two words should close on their own once those three are C. Permuting
the caller does not reach it. Ruled out at 33 words: forward prototypes for the
three assembly callees and removing the two that exist; `return callee(...)`
with the sibling call blocked by an address-taken dummy local (identical
output); early-`break` instead of `else if` in the `0x123` and `0x102` arms;
`switch (skill)` for those two arms (six words worse).

The case bodies are laid out in retail as `{0x103, 0x124}`, `0x123`, `0x102`,
`0x125`, `0x108`, `0x158`, and that order is what the draft uses. The `0x123`
and `0x102` arms share their `BSARI_MASEHA_Run` and `BSARI_SIMANO_Run` calls by
cross-jumping, which the draft reproduces.

## nbCameraBoss_Lusi_Cut1_Set (newbattle/nbCameraBoss, 0x1F4)

21 of 125 words, at the exact 500-byte size.

Two independent differences remain, and nothing in the rest of the function
differs:

The blend factor's bits are allocated to `$3` where retail uses `$4`, and the
switch value to `$2` where retail uses `$3`, so the whole allocation is shifted
down by one. That also shows up as `qmtc2.ni $3` instead of `$4` in all four
`SDF_VU0_INTERPOLATE` sites. Retail also duplicates the `mfc1` into both arms
of the clamp (`mfc1 $4, $f1` before the `bc1f`, `mfc1 $4, $f2` on the
fallthrough) where the draft's `if (rate > 1.0f) rate = 1.0f;` gives one
`mov.s` and one `mfc1`. Writing the clamp over the bits instead
(`PS2_FLOAT_BITS(bits, rate); if (rate > 1.0f) PS2_FLOAT_BITS(bits, 1.0f);`, or
the same as an `if`/`else`) does produce the two `mfc1`s but reorders the
divide and the 1.0 materialisation ahead of them, which is worse: 106 to 114
words differing instead of 21.

In the first of case 0's two lerps ee-gcc rotates two fp registers, emitting
two `lwc1` and two `swc1` at a time where retail loads all six constants and
then stores all six. Case 1's identical block comes out with retail's six and
six, and so do the same blocks in `nbCameraDANTE_DCOMBO_Set` and
`nbCameraDANTE_DCOMBO_KAGU_Set`, so this is pressure local to that one arm.

Ruled out: reading `nbCameraBoss_BSLUSI_Type` before the divide, between the
clamp and the `PS2_FLOAT_BITS`, and after it; declaring `type` before `bits`;
and a named `one` local for the 1.0 comparand.

## fldCamUpdateChase (field/fldDraw, 0x250)

120 of 148 words, at the exact 592-byte size. The first 119 words are exact;
everything from `+0x16C` is one word rotated because retail schedules the
`lw $4, 0x78($20)` that fetches `control->camera` for
`dds3CameraObjectBasicGetCameraTM` to the top of the smoothing block, while
ee-gcc puts it last. The rotation then drags a handful of `$f2`/`$f3`
allocations with it.

Confirmed on the way: `D_0054C380` and `D_0054C384` are `.lit4` entries
holding `0.7f` and `3.14f`; the stick is read as the plain subscripts
`dds3PadStickState[0][0][0]` and `[0][0][1]`, not through a `u8 *` local,
which costs a word by making gcc form the gp-relative address in a register;
and both vectors are zeroed with `memset` and then given a `1.0f` in `[3]`.

Ruled out: a `u8 *` stick local in four spellings (plain, `&...[0][0][0]`, a
cast of the array, `volatile`); an incomplete array declaration; all 36
orderings of the two three-component smoothing blocks (the best is the target
block written y, x, z, which is the draft); `node = control->camera;` as its
own statement before each of the three uses; and a separate `dds3Basic_t *`
local for the camera.

## encDrawProc (field/fldDraw, 0x1D8)

116 of 118 words, at the exact 472-byte size. The two differing words are both
`bne` where retail has `bnel`, at the same displacement and with the same
instruction in the slot: `addiu $2, $0, 1` before the `gencMode` test, and
`addiu $3, $3, 1` before the `encDrawState` body. ee-gcc takes the insn from
the branch's taken thread in both compiles and leaves a second copy at the
join, so only the annul bit differs.

Confirmed on the way: the box-draw body is written out in full in both arms of
`if (encDrawState >= 0x29) ... else if (encDrawState >= 0x15) ...`, and ee-gcc
cross-jumps them at the `jal rsetOtOfs`, which is where retail's duplicated
`addiu $4, 0x47` and the `nop` in the call's delay slot come from. Writing it
as `if (state < 0x29) { if (state >= 0x15) ... } else { ... }` inverts the
first branch and costs 56 words. `D_0054C2A4` is a `.lit4` entry holding
`0.001f`.

Ruled out: `gencMode != 0` with an empty `then`; `s32` return types on
`dds3EffectPartialMultiBlurSetParam` and `...Draw` to reserve `$2`; a `result`
local in place of the three `return 0`s; and an `s32` return type on
`encFreeBlur`, which the definition below refuses.

## slDrawWindow (mc/slMain, 0x4EC)

87 of 313 words, two words short of the 1260-byte size.

Everything up to the window-height arithmetic is exact, and the arithmetic
itself is reconstructed: the counter is bumped and stored, the first box's
`y` and `h` come from `0xE9 - cnt * 0xB5 / 4` and `cnt * 0x16B / 4` clamped to
`0x34` and `0x16B` from four on, and the four later box pairs divide by 5 and
by 100 exactly as retail does. The draft's argument lists and the `slti`,
`sltiu` tail are right.

The two missing words are one register copy and the `nop` that follows from
it. Retail increments into `$2`, stores and compares that, then copies it to
`$4` for the two multiplies; the draft increments straight into `$4` and needs
no copy. Because the block is then one instruction shorter, the scheduler
hoists the second `li` above the first multiply and puts the second multiply
on `mult1`, where retail leaves both on pipe 0, and `.L00152408` loses the
8-byte alignment `nop` in front of it. Every division and both `movn` biases
are identical. After that point the draft and retail differ only in the order
of the twelve stack-argument stores of each `aDrawBoxG` call, which is the
scheduler working from the shifted block.

Ruled out for the copy: `cnt` as a local, `gSlWindowCnt + 1` stored then
re-read, `gSlWindowCnt++`, `++gSlWindowCnt` as the test expression, a second
local assigned from the first, the increment split into two statements,
products hoisted into named locals, `0xB5 * cnt` operand order, decimal
literals, `h` before `y`, the subtraction split off into its own statement,
all 120 declaration orders of the five locals, and separate locals per block
(that one reaches 88).

## slDrawInfo (mc/slMain, 0x2F4)

183 of 189 words, at the exact 756-byte size, and **exact when slDrawWindow is
compiled as C in the same unit**. That is the finding worth acting on: the
draft here is complete and correct, and banking it only needs slDrawWindow
above it to match.

The six remaining words with slDrawWindow still behind `INCLUDE_ASM` are six
`bnel` where retail has `bne`, on the `gslInfoCode` tests for 1 through 6. The
delay slot holds the next comparison constant in both, so only the annul bit
differs; the tests for 0xB, 0xC and 0xD match either way. With slDrawWindow
defined above it in the unit, ee-gcc drops the annul on all six.

Ruled out while slDrawWindow was still assembly: `gslInfoCode == N &&
slDrawWindow() != 0` fused into one test, the call's result in a local,
`== 0` with an empty arm and an `else`, `if (slDrawWindow())` without the
comparison, `u32 gslInfoCode`, an `s32` return type with `return 0` for the
early exit, and the whole body nested in `if (gslInfoCode != 0)` instead of an
early `return`.

## slDrawError (mc/slMain, 0x4AC)

293 of 299 words, at the exact 1196-byte size, and **exact when slDrawWindow
is compiled as C in the same unit** - measured, not assumed. Same situation as
slDrawInfo above and the same six-`bnel` difference, on the `gslErrorCode`
tests for 1, 2 and 7, which are the three arms that branch on `slDDS3To31Flg`
inside. Nothing else in the draft differs.

slDrawWindow is therefore the one thing standing between this unit and three
matched functions: itself, slDrawInfo and slDrawError, 801 words of retail
code between them.

## effBTLModelLook2 (effect/effBattleMisc, 0x110)

2 of 68 words, at the exact 272-byte size, and the two words are each other's:
retail emits `abs.s $f1, $f2` then `mov.s $f13, $f0`, the draft the same pair
in the other order. Both are independent single-cycle moves off the two
`lwc1`s that precede them and that the draft already matches, so there is no
dependence to steer: `$f1` is `fabsf(direction.x)` for the compare and `$f13`
is `sdfAtan2`'s second argument copied out of `$f0` before `abs.s` clobbers
it.

Everything else in the function matches, including the `sdfSin`/`sdfCos`
divide into one register, the three-way clamp and the `$f20`/`$f21`/`$f22`
saved-register layout. Two source facts were needed to get there and are worth
keeping:

- The clamped value and the half-width are one variable, not two. Retail's
  `mul.s $f12, $f20, $f0` writes the register that later carries
  `sdfAtan2`'s first argument, so `x = tangent * direction.z;` followed by
  `x = direction.x;` / `x = fabsf(x);` / `x = -fabsf(x);` in the three arms is
  what matches. A separate `width` local coalesces into the tangent's register
  instead and costs seven words.
- `fabsf` declared as a plain `extern f32 fabsf(f32);` compiles to `abs.s`;
  `EE_ABS_F32` does not match here (29 to 31 words differing, and the function
  runs long).

Ruled out: `direction.z * tangent` instead of `tangent * direction.z` (four
words); a named `tangent` local, a named `absx` local, and `absx` computed
before the multiply; `fabsf(x) > fabsf(direction.x)` for the compare;
`!(fabsf(x) <= fabsf(direction.x))`; a `z` local for `direction.z`; and
declaring `x` before or after the other locals.

## effBTLPartPtrModelGroupGetCenterAndHeight (effect/effBattleMisc, 0x248)

4 of 146 words, at the exact 584-byte size. One instruction is wrong and the
three stores after it are scheduled one slot early as a consequence: retail
rematerialises the zero it stores into the accumulator vector
(`mtc1 $0, $f0`), the draft copies it out of the saved register that already
holds `front` (`mov.s $f0, $f21`). The copy has the same cost but a
dependency, so ee-gcc's scheduler fills the next three slots with the two
`addiu`s and the first `swc1` in the other order.

cc1's copy propagation is what does it: `front = 0.0f;` leaves 0.0 available
in a pseudo that lives across the `nbGetMainProcessData` call, and the later
`zero = 0.0f;` becomes a copy of it. Nothing tried stops that. Ruled out:
spelling the second zero `0.0`, `0`, `-0.0f` (two words, worse), a chained
`sum.x = sum.y = sum.z = 0.0f`, a literal at each of the three stores (which
reverts to the shared register and costs 114 words), `sum.* = front`, and all
120 orderings of the four initialiser groups (`count`/`back`, `front`/`total`,
the `nbGetMainProcessData` call, the `top` select, the `sum` zeros) — six of
them tie at this same 4-word result and the rest are worse.

The rest of the function matches, and four source facts were needed:

- The tail is `if (count != 0) { ... } else { ... }`, not an early return for
  the empty case. Retail branches forward to the empty arm, so the populated
  arm has to be written first; the early-return form differs in 85 words.
- The `sum` zeros come from a named `f32 zero` local that the empty arm also
  stores through. Giving the empty arm its own local costs 114 words.
- `sum.x`, `sum.y`, `sum.z` in that source order; `z, y, x` costs one more
  word.
- `group & 0x100` is a `u32` local, read twice (once for the `top` select,
  once inside the loop), which is what keeps it in a saved register.

## gameKey1 (pb/pbGame, 0x2A8)

150 of 168 words, 672 bytes against retail's 680. The whole body shape is
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

46 of 77 words, 308 bytes against retail's 324. The first 31 words, both
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

60 of 68 words, 272 bytes against retail's 288. Only the three `sll`/`slti`
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

12 of 34 words, 136 bytes against retail's 140.

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

## DispComm1PanelComm (newbattle/nbStageProcess, 0xE0)

53 of 56 words, at the exact 224-byte size.

The guard is `if (no <= 0x8003) { if (no >= 0x8000) return; }` over an `s32`
local holding the `u16` parameter, which is what gives retail's two separate
signed `slt`s rather than the subtract plus one `sltiu` that an `&&` or `||`
range chain folds to, and what stops `shorten_compare` turning the compares
unsigned.

The three remaining words are the lower bound's encoding. Retail materialises
0x8000 and tests `slt no, reg` with a `beqz`, the inverted-branch form of
`LT`. ee-gcc 2.96's `fold` rewrites every spelling of `< 0x8000` and
`>= 0x8000` into `> 0x7FFF` before the MIPS backend sees it, so the compare
comes out as `li 0x7FFF`, `slt reg, no`, `bnez`: same test, three words off.

Ruled out: both nesting orders; `||` of the complement and `&&` of the range,
which fold; a `goto` into the `if` body so the draw block is the fallthrough;
an `else return`; `!(no < 0x8000)`; `no <= 0x7FFF`; `no > 0x7FFF` with an
early return; an explicit `s32` local holding the comparison result, which
adds an `xori`; `(s32)` casts on either operand; and two distinct `s32` locals
for the two tests, which does defeat the range fold but leaves the same
encoding.

## Flow13_8 (newbattle/nbMakePacket, 0xB8)

42 of 46 words, at the exact 184-byte size.

Four cases over 0 to 3, with `case 3` a bare `return 1`, give retail's
dispatch tree and both `nbDispNegoMessage` blocks. What the source cannot
have both ways is the case label order: written 0, 1, 2, 3 the bodies land in
retail's order but ee-gcc turns the first dispatch test into a `beql` that
annuls `case 1`'s first load, and every later word shifts; written 0, 2, 1, 3
the dispatch is retail's plain `beq` with the following `slti` in the slot but
the two message blocks swap places, which is the four words kept here, two
branch offsets and the two message constants.

Ruled out: all 24 case label orders; every mix of `break` and `return 0` in
the arms; a `u32` and an `s32` phase local; `case 3` lifted out in front of
the switch as an `if`; and reaching one message block through a `goto`, both
from a label inside the switch (which loses the plain `beq`) and from one
after it.

## Flow9_20 (newbattle/nbMakePacket, 0xAC)

35 of 43 words, at the exact 172-byte size.

The `switch` over 0, 1 and 2 gives retail's dispatch tree exactly, `beq 1`,
`slti 2`, `beqz`, `beq 2`, and the `movn` in the `case 2` arm needs the phase
read into an `s32` local so the ternary reuses that register. What is left is
one delay slot: retail fills the `beqz` that enters `case 0` with the
`ld $31` of the fall-through return path and keeps a plain `beq`, while
ee-gcc annuls `case 0`'s first load into it as a `beql`, which shifts the
four words of that arm and leaves a trailing `nop`.

Ruled out: all six source orders of the three arms (only ascending reaches
this close, the rest run over size); every mix of `break` and `return 0` in
the arms, all eight of which are worse; `default: return 0;` and a `goto` to
a trailing label; a `u32` phase, which reshapes the tree; and the ternary
written against the field or the literal 2 instead of the local.

## datGetPlayerParam (newdata/datCalc, 0x50)

7 of 20 words differ, exact size. The structure is right and the remaining
difference is which register holds the hearts bonus. Retail puts it in `$2`:
the `beqz` that skips `cmbGetHeartsParam` carries `daddu $2, $0, $0` in its
delay slot and falls into a single `subu $2, $16, $2`, so the zero and the
call result share the return register and the `$gp`-relative `DDS3_GBWK` load
has to avoid `$2` and lands in `$3`. Every shape tried either puts the bonus
in `$6` with a copy after the call (12 of 20) or duplicates the `subu` and the
epilogue into both arms (88 bytes against retail's 80).

Ruled out: `bonus = 0` before the `if`; `if`/`else` in both polarities; a
ternary at the return and in a local; a `goto` into a shared tail from either
arm; duplicating `return param - bonus` in both arms to invite cross-jumping;
`hearts` as `u8`, as `s8` and as an `s32` with the cast at the read; and all
36 combinations of declaration order and initialisation order of the three
locals.

## datAddUniqueDevil (newdata/datCalc, 0x6C)

12 of 26 words differ and the draft is one word short. Retail keeps
`index * 0xEC + DDS3_GBWK` in one register, biases a second to `+ 0xA54` for
the flag load at displacement 0, and then copies the first with
`daddu $3, $4, $0` so `$4` can take the flag, storing `uniqueid` at
displacement 0xA76 off the copy. ee-gcc coalesces that copy away in every
spelling tried, and also schedules the `sh` above the `lw` where retail has
the load first.

Ruled out: `unit->uniqueid` off a single pointer (which puts the store at
displacement 0x22 and loses the second base); a `u8 *base` local with the
store written as `*(u16 *)(base + 0xA76)`; the same through a `u16 *` local;
the store through `DDS3_GBWK->akuma[index]`, which forms the right address but
does not add the copy; both orders of the flag read and the uniqueid store;
and a `volatile` read of the flag to pin the order.

## datDeadNakama (newdata/datCalc, 0xC4)

28 of 48 words differ and the draft is one word short. The loop body, the
`fldAnahoriAkuma_t` view at `DDS3_GBWK + ofs + 0xA50` and the `u32` offset
local that keeps `ofs + 0xA50` in a saved register are all right. Two things
are not. Retail copies the record address with `daddu $5, $3, $0` in the
`flag & 1` branch's delay slot so `$3` can be reused, and it forms the second
read of the same flag as `(DDS3_GBWK + 4) + ofs` with displacement 0 where
the draft forms `(DDS3_GBWK + ofs) + 4`. The two are the same instruction
count, so this is an association ee-gcc canonicalises.

Ruled out for the association: explicit parentheses around
`((u8 *)DDS3_GBWK + 4)`; the same through a `datUnitWork_t *` cast; the
index-first cast `(u8 *)aofs + ((u32)DDS3_GBWK + 4)`; an address derived from
`&DDS3_GBWK->akuma[0].flag`; and `s32` instead of `u32` for the offset local.
Ruled out for the copy: a named `fldAnahoriAkuma_t *` local for the record,
with the second access written through it, through the global, and through the
`+ 4` form.

## datPartyDamagePipe (newdata/datCalc, 0x114)

54 of 68 words differ, one word short, and every instruction retail emits is
present: the guard, the `fldAnahoriAkuma_t` view at `0xA50 + i * 0xEC`, the
`div`/`div1` pair on the two pipes, the `mult`/`mult1` pair, and all three
cmov clamps. The whole allocation is rotated by one. Retail puts the two rate
parameters in `$21` and `$20` and hoists the loop-invariant constants in the
order 100, 10, 1; the draft puts the parameters in `$22` and `$21` and hoists
10, 100, 1.

Ruled out: both declaration orders of the offset and index locals crossed with
three ways of initialising the offset (a statement above the loop, and either
order inside the `for` init), crossed with both orders of the two terms of the
damage sum. All twelve produce byte-identical output, so ee-gcc canonicalises
the sum and the constant-hoisting order is not reachable from the expression.

## datInitPlayer (newdata/datCalc, 0x12C)

7 of 75 words differ, exact size. The `memset`, the 0xEC struct assignment
from `D_003493C0` with its runtime 8-byte-alignment dispatch, the item clear
loop and the tail call to `evtBitOn` are all right. What is left is a
one-register rotation over the two scalar stores: retail puts the constant 1
in `$4` and the loop index in `$5`, skipping `$2`, while the draft uses `$2`
and `$4`. Retail also emits `sb 0x1914` before `sw 0x1924` and the draft emits
them the other way round, and no source order changes that.

Ruled out: both orders of the two stores, written through the `u8 *` local as
a subscript and as a cast; both declaration orders of the pointer and index
locals; and four `memset` declarations (the `s32` return meant to reserve
`$2`, `void *`, unprototyped, and none at all, leaving gcc's builtin). All
sixteen combinations produce byte-identical output.

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

## effPCPBossMETATOBeamDraw (effect/effPCPBoss, 0x25C)

7 of 151 words, at the exact 604-byte size.

Where the two interpolation factors finish being built. Retail materialises both
`0x3EA8F5C2` and `0x3F28F5C2` with `lui`/`ori` pairs before the
`effColorBlend` argument setup, so the `and $4, $5, $4` lands in the call's
delay slot; the draft's scheduler sinks `lui $20` and both `ori`s past the
`jal`, which rotates seven words. Everything else is exact.

Two facts carried this one most of the way, both reusable:

- The lerp factors have to be `u32` locals holding the binary32 bits, assigned
  just before the `effColorBlend` call. Written as literals inside
  `SDF_VU0_INTERPOLATE` in the loop body, gcc hoists the materialisation into
  the loop preheader and the whole saved-register assignment rotates by one
  (`work` takes `$18` where retail uses `$19`): that costs 30 words rather than
  7. `PS2_FLOAT_BITS` cannot be the source shape here, since retail has no
  `mtc1`/`mfc1` pair for either factor.
- `found` has to be declared after every other local. It is spilled in this
  function, and with `found` declared next to `model` it takes the lower of the
  two spill slots (`0x10`) where retail gives that slot to `limit`.

Ruled out for the seven words: both assignment orders of the two factors; the
assignments before the `frame < limit` guard, before the fade if-chain, between
the blend and the loop, and split either side of the call (the three early
positions are far worse, 88 to 96 words, because the factors then occupy
registers the fade chain needs); and `found` declared first rather than last.

## fldNpcLightSet (field/fld_Npc, 0x28C)

125 of 163 words, at the exact 652-byte size. Everything the source can say
is said: the light row is `D_003129D0 + (fldArea[0] + 1) * 8` with the
direction mode in byte 7 and the face-division nibbles in byte 6, the
`if (m == 0) ... else if (m == 3) ... else` chain with the last arm repeating
the first (gcc cross-jumps the two into one block, which is what gives
retail's `beqz` and its `bne $3, 3` both targeting `.L0014AF28`), each arm
carrying its own `3.14f` and `0.6f` so the four pool entries at `0x54C4F8`
through `0x54C514` come out one per arm, `dir = dir * 180.0f / 3.14f;` split
from `dir = dir - 70.0f;` so the division writes `dir`'s own register, and
`rad = dir * 3.14f / 180.0f;` placed after `sdfModelSetFaceDivParm` so `dir`
and the light power stay live across the three calls in `$f21` and `$f22`.

The remaining 38 words are one hard-register permutation of the four
callee-saved integers. Retail allocates `object` `$16`, the model handle
`$17`, the light row `$18`, the unit `$19`; the draft gets row `$16`, object
`$17`, unit `$18`, handle `$19`. Every instruction is otherwise the same, the
float registers included, and the two colour words land in `$17` and `$16` in
both. gcc 2.96 sorts allocnos by `floor_log2(n_refs) * freq / live_length`,
so this needs a live range changed, not a statement reworded.

Ruled out: a `mode` local for `row[7]` (60 words); a `div` local for
`row[6]` (39); the row as a pointer to an eight-`u8` struct, as a `u32`-cast
read, and as a plain `u8 *` (all 38); computing the row before
`dds3ObjectBaseGetModelHandle`, after `EulerAngleAxisY` and before the
`SDF_VU0_LOADVECTOR` (53, 47, 48); the ambient word computed before the light
word (42); the blue component or'd before the green one (38);
`dds3ObjectBaseGetModelHandle(unit->effObj)` with `object` read afterwards
(39); a `switch` over the four modes, which runs past the next function; and
seven declaration-order permutations of the nine locals, all 38.

## fldDrawEATexQuad (field/fld_Npc, 0x340)

143 of 208 words, at the exact 832-byte size. The structure is settled: ten
`s32` parameters plus one `f32`, the ninth and tenth integers read from
`0x120` and `0x128` because this EABI pads each outgoing stack argument to
eight bytes (which is also why `sdfPrimLinkFt4`'s thirteen stack arguments
occupy `0x0` through `0x60`); four `sdfVector4_t` corners built in the XZ
plane from `-(ax * 16)`, `-(ay * 16)`, `+w * 16` and `+h * 16`; a
`SetRotateMatrixY((angle + 180.0f) * 3.14f / 180.0f)` with the `3.14f` out of
this unit's `.lit4` pool; `SDF_VU0_LOADVECTOR` /
`SDF_VU0_APPLYMATRIX_XYZ` / `SDF_VU0_STOREVECTOR_NOMEM` per corner, the
`_NOMEM` form being what rematerialises the corner address for the store
instead of sharing one register with the load; and the GS bias folded in as
`pos[i].x + xf + 28672.0f` and `pos[i].z * 0.5f + yf + 30976.0f` written back
into the corner.

Two things remain. The shift and negate block has to sit after the three
packet calls, not before them, or the parameters are not live across a call,
cc1 coalesces their entry copies into the shifts and the function comes out
four words short; with the block moved the entry copies appear and the shifts
fill the three delay slots exactly as retail does. What is left after that is
a swap of two callee-saved pairs, `u` and `v` taking `$21` and `$20` where
retail gives them `$23` and `$22` and the stack-read `uw` and `vh` taking
`$22` and `$23` where retail gives them `$20` and `$21`, and the store
schedule for the four corners: retail emits the zero stores in the order
`v0.y`, `v3.y`, `v1.y`, `v2.y` and pairs each coordinate store with the other
corner that shares its value, where the draft emits them in corner order.

Ruled out: the shift block before the calls (185 of 204 words and sixteen
bytes short); in-place updates of the parameters instead of six locals (which
runs past the next function when the block is after the calls, and 198 of 202
words when before); `ev` computed before `eu`; and `uw * 16 + su` instead of
`su + uw * 16`.

## fldSibukiStart (field/fld_Npc, 0x98)

28 of 38 words, at the exact 152-byte size. `fldBlurEffectId` has to be an
incomplete array so the three handle reads go through one `lui`/`addiu` pair
in `$16` rather than `$gp`; with a plain `extern void *` the function is
136 bytes and 31 words out.

The ten remaining words are two scheduling decisions, both of the same kind:
the draft schedules the address and the load earlier than retail does.
Retail sets up `memset`'s three arguments before saving `$16` and starting
the `%hi`/`%lo` pair; the draft saves `$16` and emits the `lui` first. After
`memset`, retail stores `x`, then `w`, then `y`, loads the handle and puts
the `z` store in the call's delay slot; the draft loads the handle right
after the `mtc1`, stores `x`, `y`, `z` and puts the `w` store in the delay
slot. Instruction counts are identical in both blocks.

Ruled out: `position.w` assigned first, last, and between the coordinates;
`extern void *volatile fldBlurEffectId[]`; `extern void *fldBlurEffectId[2]`
(which goes back to `$gp` and 31 words); and a `void **p = fldBlurEffectId`
local feeding the three calls.

## fldRunTamaFree (code_00116AD0, 0xBC)

27 of 47 words, at the exact 188-byte size. The loop structure, the table
stride and the two-arm mode test are all retail's; what is left is one
register choice repeated three times. Retail loads each handle straight into
the argument register (`lw $4, 0x10($16)` then `beql $4, $0`), so the test and
the call share one register and the copy disappears; ee-gcc here puts the load
in `$2` and adds `move $4, $2`, which also costs the branch-likely and moves
the loop-alignment `nop` ahead of the inner loop head.

The mechanism is visible from the other side: `pipeFreeEffects` in
`pipe/pipeEffect` has the identical `if (p->h != NULL) { f(p->h); p->h = NULL; }`
shape and does get `lw $4`, because `$2` there holds the `u8` counter's
`i + 255` across the if. So the fix is whatever makes `$2` unavailable across
the guarded block, not a different spelling of the guard.

Ruled out: a reused `void *handle` local for all three arms and a fresh local
per arm (cse propagates both away); `while` in place of the inner `for`, with
`entry++` at the end of the body; the counter as `s32`, `u8` and an index
`tbl[j]` instead of a pointer; `s32` and unprototyped declarations for the
three callees, and an `s32` parameter in place of `void *`; casting the field
at the call site; `while` in place of each `if`; and declaring `res` before
`entry`.

## fldRunEffSet (code_00116AD0, 0x12C)

67 of 73 words, 292 bytes against retail's 300. The loop, the two float range
tests, the arm split and the five-store tail are retail's: the tail stores
land in retail's order (`0x14`, `0x24`, `0x18`, `0x1C`, `0x20`) once the
source writes `x`, `y`, `z`, `turn` after `life`, the left rotation of the
emitted list.

Two things remain. The same `lw $4` against `lw $2` plus `move $4, $2` as
`fldRunTamaFree`, twice, so crack that one first. And the prologue order:
retail forms `&fldRun_Dante` from a `lui` issued third and an `addiu` after
the `D_002FBEF0[id]` load, where the draft's `fldRunDante_t *dante =
&fldRun_Dante;` initialiser puts both in front of it.

Ruled out: the float tests written as `>=` (gives `c.le.s` in the other
encoding); the tail stores in the three other rotations; and `fldRun_Dante.`
at every use instead of the `dante` local, which loses the saved register
entirely.

## fldRunEffTask (code_00116AD0, 0x220)

115 of 136 words, 544 bytes against retail's 544 at the same size. The two
arms, the `dds3EffectGetState` delete path, the `(90.0f - turn) * 3.14f /
180.0f` rotation and `SDF_VU0_STOREMATRIX` into a 0x40 frame slot are all in
place, and the handle loads already reach `$4` here because each one is alone
in its block.

What is left is which saved register each of three values gets. Retail
allocates `i` to `$19`, the hoisted constant 1 to `$20` and the
`D_002FBEF0` walk to `$21`; the draft gives 1 to `$19`, the walk to `$20` and
`i` to `$21`, and every later word shifts. The `3.14f` literal is the unit's
own `.lit4` entry `D_0054C204` and must stay a literal at this position, so
any reshaping has to keep one float constant used here and in this order.

Ruled out: nothing yet beyond the declaration order of the five locals.

## fldRunTamaSe (code_00116AD0, 0x3C8)

165 of 230 words, 920 bytes against retail's 968. The skeleton is established
and is what the draft is for: three near-identical blocks, one per tama
handle, each reading `dds3ObjectBaseGetModelHandle(g)->motion->frame` (the
`mdlObj_t` motion pointer at `+0x1C`, then `sdfMotionCtrlr_t.frame` at
`+0x1C`) and scanning a ten-entry `f32` frame table for the window
`tbl[i] - 0.5f < now && now < tbl[i] + 0.5f`, then
`p2sdPlaySE(0x6F0006, fldRunDanteDistToTarget(pos) * vol[i] / 128 / 2, 0x3F)`
and `break`.

The four tables are the unit's own rodata: `D_00531158`, `D_00531180` and
`D_005311A8` are `f32[10]` frame windows and `D_005311D0` is `s16[10]`
volumes (0x7F 0x7F 0x5F 0x5F 0x3F 0x3F 0x1F 0x1F 0x0F 0x0F, 20 bytes). They
are copied to the frame at `0x10`, `0x40`, `0x70` and `0xA0`, which the draft
already reproduces, including the `ldl`/`ldr` plus `lwl`/`lwr` tail on the
20-byte one.

What is left is bigger than a spelling: retail is twelve words longer and the
copies come out in the order `D_00531158`, `D_00531180`, `D_005311D0`,
`D_005311A8`, which is neither declaration nor statement order.

Ruled out: all 24 orders of the four copy statements, which move the count
only between 165 and 170 words, so the copy order is not the lever. Someone
picking this up should start from the per-block code rather than the
prologue.

## resetCamTgtPlayer (field/fldDraw, 0x1F8)

90 of 126 words, at the exact 504-byte size, and every instruction in the
function is the one retail emits. Two things differ and nothing else.

The first is a three-way rotation of the fp registers the eye block uses for
`fldGlobalBase.y`, `D_002FC9C0[1]` and `D_002FC9C0[2]`: retail numbers them
`$f3`, `$f4`, `$f5` and the draft gives `$f4`, `$f5`, `$f3`. The second is
where the scheduler puts `lw $4, 0x78($19)`, the `control->camera` argument of
each of the three calls: retail fills the `div.s` hazard slot with it and
leaves the `swc1` to `camAngle` for the `jal` delay slot, while the draft puts
the `lw` in the delay slot and the `sub.s` for `eye.f[2]` in the hazard slot.
The whole tail from the second `dds3SetPosPoseSet` to the epilogue, the
`sqrt.s`, the `GetSphericalPosMove` argument setup and the `fldCamPitch`
store, is byte-identical.

`-fno-gcse` and `-fno-expensive-optimizations` leave the difference exactly
where it is; `-fno-schedule-insns` and `-fno-schedule-insns2` make it much
worse. So this is register allocation plus the list scheduler's tie-break, not
anything the source states.

Confirmed on the way: `D_0054C2E8` is a `.lit4` entry holding `3.14f`;
`fldCamParamTable[fldGlobalBase.cameraParamIndex][1]` is the height offset
added to `fldGlobalBase.y`; the eye and target vectors are both written as the
degenerate lerp `a + (b - a)` with no factor, which is what retail's bare
`sub.s`/`add.s` pair per component is; and the two angle fields written here
are `fldGlobalBase.camAngle` at `0x124` and `camAngle2` at `0x128`, both taking
`EulerAngleAxisY() * 180.0f / 3.14f`.

Ruled out: six orderings of the three eye components; the `camAngle` stores
before, after and in either order relative to the eye block; seven spellings
of the angle expression (inline, three statements, a named `180.0f` local
assigned before the leading calls, a named `3.14f` local, and the
`180.0f / 3.14f` constant folded together, which costs the whole function); a
`f32 *` local for `D_002FC9C0`; a `* 1.0f` factor on each lerp; a local for
the parameter-table height and for `y + height`; `node = control->camera;` as
its own statement before each of the four uses; and `dx`/`dy`/`dz` locals for
the three deltas, which costs another 19 words.

## nbMainProcess1 (newbattle/nbMainProcess, 0x420)

113 of 263 words, at the exact 1052-byte size.

The 16-arm jump table over `data->phase` (`s8` at +0x69, `sltiu 0x10`, every
index 0 through 15 a real case) reproduces exactly, and so do cases 0, 1 and
2. The whole remaining difference is one cross-jumping choice in `case 3`.
Retail has four `jal nbSetNextPhase` sites in the `case 3` / `case 9` /
`case 10` region: the `0xB` arm of `case 3` keeps its own call with
`addiu $4, 0xB` in the call's own delay slot, while the `5` arm of `case 3`
and the `0xA` arm of `case 9`'s inner `endtype` switch share one call with
their `addiu $4` in the predecessor's delay slot, and both fall into
`case 10`'s body, which is `next = 1; break;` on its own. ee-gcc either
shares none of them (the `if`/`else` with one trailing `next = 1`, which is
the kept draft: correct size, `case 3`'s else arm four words too long and
everything after it shifted) or shares all three (any spelling that
duplicates `next = 1` into both arms, which comes out four words short).

Ruled out: `next = 1` duplicated per arm, placed before the call, placed
after it, and with a `break` in either arm; the two arms as a `switch` over
`endtype` with `case 1`/`case 8` and a `default`; and the inverted test with
an early `break` in the `5` arm. All four of those produce the same
259-word over-merged shape. The case order in the draft is the body label
order of the retail jump table (0, 1, 2, 3, 5, 4, 6, 7, 8, 9, 10, 15, 11,
12, 13, 14), which is what gets the first 113 words; ascending order is
worse. `limit` in `case 9` has to be assigned in both arms of an explicit
`if`/`else` around `nbCheckActionStack()`, or it lives across the call and
takes a third saved register. `data->unk118` and `data->unkC8` are `u8[4]`
in `nb.h`, so they have to be read through `*(s32 *)`, not compared
directly.

## nbSetWaitMotion (newbattle/nbStageProcess, 0xD8)

8 of 54 words, at the exact 216-byte size, with the same instruction multiset.

Retail reads the visual's motion speed and multiplies it by `ritu` before it
loads `form->hunit` for the first call, so the `jal evtSetWaitMotion` delay
slot carries the motion-number argument. The draft schedules the `lwc1` into
that delay slot and leaves the `mul.s` after the call, which rotates the four
words either side of it.

Ruled out: the multiply split into `speed = *(f32 *)...; speed *= ritu;`; a
`datUnitVisual_t *visual` local; reusing `ritu` as the destination (which
loses the saved fp register entirely); and moving `form->waitmot = mot;` above
the two calls.

## CheckSenseiNego (newbattle/nbStageProcess, 0x130)

57 of 74 words, 296 bytes against the native 300.

The guards are right: the stage-major range test folds to retail's
`addiu -0xF0`/`sltiu 7`, the four `data->stat` bit tests come out in retail's
order, and the age-of-moon test is `== 8` returning 0, not `!= 8`.

The missing word is the empty delay slot after `jal evtGetAgeOfMoon`: retail
leaves a `nop` there and materialises the 8 afterwards, while the draft fills
the slot with `li $3, 8`. Retail then tests with `bnel` and annuls the
`work = a->work` load into the branch's own slot. With one word fewer the
draft's `a` lands in `$16` and `rate` in `$17`, where retail has them the
other way round, which is what most of the 57 words are.

Ruled out: `goto` to one trailing `return 0` (69 words, worse); both
polarities of the `0x2000000` test; `rate` preset to 0x64 before the test; and
an intermediate local for the `21 / enemypcnt` quotient.

## CheckAction_STKSEL (newbattle/nbStageProcess, 0xE8)

9 of 58 words, at the exact 232-byte size.

A `switch (StkSelId)` written `default:`, `case -1:`, `case -2:` gives
retail's two forward `beq`s and the body order (the normal arm first, then the
-1 arm, then the -2 arm); the ascending order runs over size.

What is left is two `$2`/`$3` swaps and one pair of adjacent constant inits.
Retail materialises the loop's `~8` mask before the `0xF` counter and the
draft does the reverse, and retail holds `StkSelId` in `$2` with the shifted 1
in `$3` where the draft has them swapped.

Ruled out: `do`/`while` and a pre-loop `i` init for the stat-clearing loop;
`form->stat = form->stat & ~8` written out; a named local for
`StkSelId + 0x10`; and a `(u32)1` shift base.

## CheckAction_ESCAPE (newbattle/nbStageProcess, 0xE4)

9 of 57 words, at the exact 228-byte size.

A `switch ((u16)((u16)a->seqtime / 15))` over cases 1 and 2 with a `goto` to
one trailing `return 0` gives retail's divide trap, dispatch and both arms.

The remaining words are which register holds the compared constants: retail
uses `$5` for the 1 and the 3, the draft uses `$4`. That also moves
`daddu $4, $16, $0` out of the `jal SetTimeList` delay slot, where retail puts
it, and shifts the default arm's branch by one word.

Ruled out: early `return 0` per arm instead of the `goto`; no `default` arm;
an `s32`, `u16` and `u32` local for the switch value; and swapping the two
case bodies' source order.

## DispCommCursor (newbattle/nbStageProcess, 0x184)

29 of 97 words, at the exact 388-byte size and with the same instruction
sequence.

Two register copies retail keeps are what the structure hinges on, and both
are reproduced: the alpha `if` has to come first in the source, which makes
`ox` a global pseudo and emits `daddu $20, $4, $0` in the entry block, and
`ox` has to be reused for the left edge of both quads (`ox = xb + 0x7000;`
then `ox = x1;` between the calls), which is where the second
`daddu $20, $21, $0` comes from.

What is left is a three-way register rotation: retail has `ox` in `$20`, `x1`
in `$21` and `oy` in `$22` with `y0` sharing `oy`'s register, while the draft
has `oy` in `$17` with `y1` sharing it, `ox` in `$21` and `x1` in `$22`. Both
spend nine saved registers.

Ruled out: every spelling that makes `y0` reuse `oy` (`yb = oy << 3;
oy = yb + 0x7900; yb += 0x7998;` and three variants), all of which make gcse
hoist the whole y chain into the entry block ahead of
`dds3FontReqAllocDrawTag` and cost 27 more words; both source orders of the
two y values; a separate local for the depth `-1`; a named local for the
second quad's right edge; an `x` local copied from `ox`; computing `ox` after
the `if`; and all declaration orders of the locals, which change nothing.

## Chk_ENHOJOMAX, Chk_FRHOJOMAX (newbattle/nbMakePacket, 0xDC each)

54 of 55 words each, at the exact 220-byte size.

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

## nbGetAiEntry (newbattle/nbMakePacket, 0xE0)

43 of 54 words, two words short of retail's 216.

The table address, both `nbAiChkJoken` calls, the two `>> 16` tests and the
`nbSetHojoCounter` call are identical. Retail places the cross-jumped
`return 0` block *after* the `nbSetHojoCounter` arm, so its tail reads
`b .L0028686C` with `addiu $2, $0, 1` annulled, then `daddu $2, $0, $0`, then
the epilogue. ee-gcc puts the `1` immediately before the epilogue and reaches
the zero from earlier, which costs the `b` and the zero block and shifts
three branch offsets.

Ruled out: a `goto fail;` with a trailing `fail: return 0;` label (runs over
size), `return 1` duplicated into the `nbSetHojoCounter` arm, and the two
`>> 16` tests as nested `if`s instead of `||`.

Confirmed on the way: `datDevilAI_t`'s 164-byte row holds thirteen 12-byte AI
entries at `+0x04`, each `{u32 joken[2]; s32 no;}`. `D_003989EC`, which
`nbRunAi` reads with a `[41]` stride, is the `no` of the same rows.

## nbClearActionFlags (newbattle/nbMakePacket, 0xE0)

18 of 56 words, at the exact 224-byte size and with the same instruction
multiset.

The structure is settled: a `mask = ~0x800;` local above the first loop so the
constant is materialised before the counter, `for (i = 3; i >= 0; i--)` over
four formations clearing `stat`, then a second loop that remembers the last
index whose `stat & 1` was set and, at the first formation without it, copies
that formation's `orgpos` and `orgrot` with four `SDF_VU0_COPYVECTOR`s,
adds `form[src].r + 100.0f + 50.0f` to `orgpos.x`, sets `stat |= 0x800` and
returns. Both float constants are `lui`/`mtc1` pairs, not `.lit4` entries.

What is left is which call-clobbered register each of the four locals gets:
retail has `mask` in `$6`, the counter in `$5` and the pointer in `$3`, and
ee-gcc permutes all three, which rewrites every word that names them. There
are no calls in the function, so this is local-alloc ordering; four
declaration orders leave it unchanged. This is the ruled-out
register-permutation class, kept only so the next reader does not redo the
decode.

## nbE901Process_1 (newbattle/nbMakePacket, 0xCC)

50 of 51 words, at the exact 204-byte size.

The dispatch, both vibration arms and the `D_00527420[e->step](e)` call are
identical. The one differing word is the second equality test: retail emits
`beql $4, $2, .L002919A0` with the `0xDE` arm's `daddu $4, $0, $0` annulled
into the slot, and ee-gcc emits a plain `beq` with the same instruction
unconditional. `$4` is dead on the fall-through path either way, so both are
correct; only the annul bit differs.

Ruled out: the two `case` labels in the other order and as an `if`/`else if`
chain (both reshape the dispatch and are much worse), an explicit
`default: break;`, a `u32` switch index, and an early `return 0` per arm with
the `nbUpdateEventProcessData` tail duplicated. The last three reach this same
one word.

## nbRunAi (newbattle/nbMakePacket, 0xBC)

46 of 47 words, at the exact 188-byte size.

The one differing word is the loop-exit branch: retail emits a plain
`beqz $2, .L00287D40` with `lui $4, %hi(D_00545840)` in its delay slot, and
ee-gcc emits `beql` with the same instruction annulled. Every other word,
including the `$18` induction pointer, the `id * 164` shift chain and the
`j nbRunAiSub` sibcall, is identical.

Ruled out: a `while` loop, a `continue` in place of the `if` body, a `u32`
index, a `goto` out of the loop, a `do`/`while (1)` with two `break`s, the
format string in a local, and `i = 3` instead of `break`. Duplicating the
`printf`/`nbRunAiSub` tail into the found and not-found paths runs over size.
Varying `nbGetAiEntry`'s declaration (absent, K&R, `u32`) changes nothing,
unlike the `nbMaeCheckDone` case below.

## nbMaeCheckDone (newbattle/nbMakePacket, 0xC8)

Byte-exact on its own. It is kept behind the fallback because compiling it
turns `nbE905_End`, the only caller, from retail's `bnel $2, $0` into a plain
`bne`: its `return 0` stops being annulled into the branch delay slot the
moment the callee is a C definition in the unit rather than an `INCLUDE_ASM`
declaration. That is the mirror image of the `slDrawWindow` finding in
[matching levers](matching-levers.md): what the compiler can see of the rest
of the unit changes the delay-slot decisions it makes for one function, and
here seeing more makes it worse. The two cannot both match, and
`nbE905_End` was already banked.

Ruled out for keeping both: a K&R `s32 nbMaeCheckDone()` definition, which is
what the implicit declaration gave the caller, and four shapes of
`nbE905_End` itself (an `&&` chain, two early returns, a `ret` local, and a
`return 0` inside the outer guard), none of which recover the `bnel`.

Confirmed on the way: a six-entry jump table does come out of six `case`
labels here, so `case_values_threshold` is not 10 for this shape. The switch
needs an explicit empty `case 0:` alongside 1 to 5; without it ee-gcc builds
a five-node compare tree instead of the table. `nbMaeFlag`, `nbMaeState`,
`nbMaeCounter` and `D_00551540` are all `s32` sdata.

## nbCameraBoss_Cut2_Run (newbattle/nbCameraBoss, 0x230)

92 of 138 words, two words short of retail's 140.

The same jump-table dispatcher as `nbCameraBoss_Cut1_Init`, on `id - 0x101`
over 0x58 entries, and written the same way: each arm is one or two
`if (skill == <n>) { callee(pCamera); break; }` tests followed by a `return 0`
or `return 1`, the arms only call, the function falls off the end of the
`switch`, and the `0x158` arm's failing guard falls into `default`. Every
difference is a displacement shift from one block.

The `0x11F` arm is the one block that is wrong. Retail reaches the shared exit
from both of its `return 1` paths with `addiu $2, $0, 1` in a branch delay slot
(`beqz $4, exit` for `skill == 0`, then `bnel $4, $2, exit` annulling a
`daddu $2, $0, $0` for `skill != 0xA5`, then `b exit` with the 1 again).
`if (skill == 0) return 1; if (skill == 0xA5) return 1; return 0;` and
`if (skill == 0) return 1; if (skill != 0xA5) return 0; return 1;` both let
ee-gcc if-convert the pair into `xori`/`sltiu`, four words short.
`if (skill != 0 && skill != 0xA5) { return 0; } return 1;` and a two-case
`switch (skill)` returning 1 both keep the branches but merge the first
`return 1` into a local block that the `beqz` jumps to, leaving its delay slot
empty and the `0xA5` constant in `$3`: two words short. The missing fact is
whatever makes that first `return 1` a bare jump to the shared exit.

The jump table differs as a consequence; the case set itself is confirmed by
the default entries, which all resolve to the one default block.

## nbCameraShot_All_Set2 (newbattle/nbCameraSkill, 0x248)

16 of 125 words, 500 bytes against retail's 584.

The structure is mapped and believed correct; what the draft cannot reproduce
is one instruction, and losing it costs the 84 bytes that feed it. After the
formation loop retail emits a bare `c.lt.s $f20, $f23` (the enemy-height
clamp) with no branch after it and no later reader of the condition: the
compare is dead, and `$f20` (the accumulated width) is overwritten with
`0.5f` five instructions later. `nbCameraShot_All_Set3` is the same function
over a caller-supplied target vector and has the identical dead compare, so it
is not a one-off.

Written the obvious way, `if (total < maxEnemyHeight) { total = maxEnemyHeight; }`,
ee-gcc sees `total`'s only consumer is that compare, deletes the assignment,
the compare and then the whole accumulation: the `r * scl` sum in the loop,
the `nbGetEnemyInterval()` multiply and the unsigned-to-float conversion all
go, which is exactly the 84 missing bytes. Retail kept the compare, so in its
compile the branch must have survived to a pass after the last dead-code
elimination.

Ruled out: an empty `if` body (drops even more), `total = (total <
maxEnemyHeight) ? maxEnemyHeight : total`, `!(total >= maxEnemyHeight)`,
`maxEnemyHeight > total`, and assigning to a third dead local in the arm. The
one shape that keeps the accumulation is `else { maxEnemyHeight = total; }`,
which reaches 68 of 146 words at 588 bytes (four over) and changes the
meaning, so it is not the draft.

What the draft does have right, and is worth keeping: the four zeroed `f32`
accumulators as `maxPlayerHeight = 0.0f;` plus three copies from it, which
gives retail's one `mtc1 $0` and three `mov.s`; `width = r * scl; width +=
width; total += width;` for the in-place double; `(u32)(enemies - 1)` for the
`bltz`/`srl`/`andi`/`or` unsigned-to-float conversion; the single local reused
for the accumulator, then `0.5f`, then the `SDF_VU0_GETLENGTH` output, which
is why all three land in `$f20`; and `maxEnemyHeight` reused for the
`sdfAtan(0.5235987f)` result so the quotient divisor stays in `$f21`.

## nbCameraState_SkillAttack_Init (newbattle/nbCameraSkill, 0x224)

117 of 137 words, at the exact 548-byte size.

Everything up to the width test matches, including the frame: `sdfVector4_t
look` at `0x0`, an eight-word `u32 ptr[8]` whose members 2 to 5 take the two
`dds3GetRandA` remainders and two zeros at `0x18` through `0x24`, and
`nbCameraSkillInfo_t work` at `0x30`. Reading the first remainder back out of
the aggregate, `D_003246D0[ptr[2]]`, is what gives retail's `sw`/`lw` pair
around the second `dds3GetRandA` and the `addu $1, $1, $2` table rows.

The 20 remaining words are the two arms of `if (width < 1500.0f)`. Retail
emits `bc1f` to the `printf` arm with an empty delay slot and falls through to
`bCameraShot_AttackInvokeTargetPartyNear_Set`; both arms then set `$2 = 0xB`
and cross-jump to one shared `sw $2, nbCameraSkillAttackStep`. The draft gets
the arms the other way round, with `daddu $4, $17` hoisted into a `bc1tl`
delay slot, and the two `nbCameraSkillAttackStep = 0xB;` stores are not
cross-jumped because the constant lands in `$2` in one arm and `$3` in the
other. Retail also keeps `D_005349A0` in `$16` across the `sdfFloatToULong`
call, where the draft materialises it after.

Ruled out for the arm order: inverting the test so the `printf` arm is the
`if` body, hoisting `nbCameraSkillAttackStep = 0xB;` below the `if`/`else`
(47 words out), and a `char *msg = D_005349A0;` local for the format string.
This is the same delay-slot-placement class as the
`nbCameraState_SkillAttack_Calc` draft below.

## fldAutoMapDrawIconOfsRF (field/fldAutoMap, 0x33C)

187 of 207 words, at the exact 828-byte size.

The rotated-icon sibling of `fldAutoMapDrawIconOfsR`: it builds the same
textured quad, but in world space, rotating the four corners through the VU0
matrix before projecting. Fully recovered apart from register allocation.

The shape, which carried over to the matching `fldAutoMapDrawBgOfsRF`:

- Four `sdfVector4_t` locals get `.x`, `.y = 0.0f`, `.z` in declaration order
  `v0`, `v3`, `v1`, `v2`, which is what matches retail's store schedule. The
  natural `v0`..`v3` order is 8 words worse here and 8 worse in the sibling.
- Each is rotated with `SDF_VU0_LOADVECTOR(&v)`, `SDF_VU0_APPLYMATRIX_XYZ()`,
  `SDF_VU0_STOREVECTOR_NOMEM(&v)`. The `_NOMEM` store is required: the plain
  form's `"memory"` clobber lets gcse share the `addiu rd, $29, N` between the
  load and the store, where retail rematerialises it for each.
- The screen coordinates are then written back into the same fields,
  `v.x = v.x + x * 16.0f;` and `v.z = v.z * 0.5f + y * 16.0f;`, x before z for
  every vector *except* `v1`, where z comes first. That one inversion is worth
  14 words; every other combination of the four is worse, and inverting all
  four costs 23.
- `color` needs the `+ 0x80000000` as its own statement,
  `color = (s32)(...) << 24; color += 0x80000000;`, or the shift goes to a temp
  and the add writes the variable, which is 2 words off.
- `0.2f` and `3.14f` are this unit's first two `.lit4` entries, `D_0054C498`
  and `D_0054C49C`.

What is left: the four texture-coordinate locals hold the other register of
each pair. Retail has `u1` in `$16`, `v0` in `$17`, `v1` in `$18` and `u0` in
`$19`; the draft swaps `$16` with `$17` and `$18` with `$19`, which moves ten
`lh`/`sll`/`addu`/`sw` words and the two `daddu` argument copies. Nothing
reached it: all 24 declaration orders and all 24 assignment orders of the four
locals (no effect at all, gcse normalises them), separate `tw`/`th` locals for
the widths, `u1 += u0` for `u1 = u0 + u1`, the sums computed before the float
coordinates, `u1 = t[0] * 16 + t[2] * 16` with the bases written at the call
sites (135 words), the bases as locals with the sums at the call sites (192),
`u32` locals (171), the shifts as their own statements (38), and
`fldAutoMapIconOfsTable[index][k]` at every use (160).

The sibling `fldAutoMapDrawBgOfsRF` had the same permutation in its eight
constant texture coordinates and *was* fixed by declaration order, because
there they are initializers materialised in the prologue: declaring them
`au, av, du, dv, bu, bv, cu, cv` (the `d` pair moved ahead of `b` and `c`) took
it from 10 differing words to exact. The same lever does not exist here, since
these four are computed from the table mid-function.

## fldAutoMapProc (field/fldAutoMap, 0x48)

15 of 18 words, at the exact 72-byte size.

Three words, and all three are the branch-annul shape the levers page describes
from the other side: retail has `beqz` and `bne` with a `nop` in the delay
slot, the draft `beqzl` and `bnel` with the epilogue's `ld $31, 0x0($29)`
annulled into it. Both differing tests guard the arm that calls
`fldAutoMapCalc`, which is still `INCLUDE_ASM`, so this is the `mc/slMain`
mechanism (a caller needing its callee's definition, not just a prototype):
decompile `fldAutoMapCalc` and try this again before touching the source.

The body is settled. `fldGb[0]` is the field id, and the two bounds have to be
separate `if` statements with early returns, which is what gets both `slti`s
computed before the first branch; `&&` folds them and costs 13 words.
`fldAutoMapCalc` must stay without a forward prototype, since the implicit
`int` declaration is what reserves `$2` across the call and stops the sibcall.

## fldDrawAreaBlockFont (field/fldAutoMap, 0x388)

57 of 226 words, 908 bytes against retail's 904.

The area-name panel: it steps the name's fade counter by the panel mode, then
draws the place name (or the area name) and its backing panel. Fully recovered
as behaviour; what is left is one saved register and the two fade arms.

What the function reads, since none of it was named before:

- `fldGlobalBase + 0x90` is the panel mode, 0 to 7, which drives the jump
  table. The HD reconstruction calls it `AllPanelMode`. The draft spells it
  `*(s32 *)((u8 *)&fldGlobalBase + 0x90)` rather than naming the field, since
  nothing compiled uses it yet.
- `D_0054EEA4` is the fade counter, 0.0 to 1.0 (`gFldAreaNameCnt` in the HD
  source), and `fldAreaFontWork` is its direction, -1, 0 or 1
  (`fldDrawAreaBlockFont_pm_mode` there).
- Mode 0 and 1 snap the counter to 0.0 and 1.0; 2 and 3 step it by 0.1 toward
  0.0 and 1.0, clamping and zeroing the direction at the end; 4 through 7 step
  it in whichever direction the counter already holds, but only while it is
  strictly between 0.01 and 0.99.
- The panel position is an interpolation whose endpoints are all zero except
  one: `x = (s32)(v * 0.0f + (1.0f - v) * 250.0f)` and
  `y = (s32)(v * 0.0f + (1.0f - v) * 0.0f)`, which is what shares the one
  `mul.s` by zero between the two. The HD source has the same expression with
  a 3.75 and a 2.25 resolution scale added.
- `fldGlobalBase.nameD` selects the place name, `D_0030D714` with
  `fldAutoMapFindPlace` and colour `0xF0DC6480`; `-1` there falls back to
  `nameU`, `D_0030AB54` with `fldAutoMapFindArea` and `0xA09DC380`. Both
  tables are `char [][28]`, already declared, and both are skipped when the
  matching `D_00567F50`/`D_00567EA0` length is below 2, read with an `(s16)`
  cast at the use site for retail's `lh`.
- The six `.lit4` entries from `D_0054C4D8` to `D_0054C4EC` are this
  function's, in the order 0.1, 0.1, 0.99, 0.01, 0.1, 0.1.

The `nameD` arm has to be written first (`if (nameD != -1)`), or gcc hoists
the `nameU` load above the test and emits the two arms in the other order,
which is 15 words worse. Case 3's clamp has to be written inverted,
`if (!(v > 1.0f)) { direction = 1; } else { v = 1.0f; direction = 0; }`, so
that the constant arm is the branch target; case 2 reads better the plain way
and measures the same.

What is left:

- The frame saves one callee-saved register too many, which is the whole
  2-word size difference. Retail keeps the font request handle and the string
  width in one register (`$17`): the handle is copied into the destroy call's
  argument and the register is immediately rewritten with `width + 8` in that
  call's delay slot. Two locals cannot reach that, and merging them into one
  `s32` with casts at the calls undershoots instead, 884 bytes with the width
  coalesced away entirely.
- Case 2 lacks retail's second `lwc1` of the counter and the `addiu $2, 0, -1`
  hoisted into the first `bc1f`'s delay slot. Retail reads the counter once per
  case and once more on the out-of-range path (in the range check's delay
  slot), which no placement of the read reproduces: before the switch, gcc
  common-subexpression-eliminates the per-case reads; after the switch, it
  replicates the load into all five predecessors instead of using the stored
  register, 20 bytes long; and an explicit `default: v = D_0054EEA4; break;`
  gets hoisted back out of the switch, 12 to 28 bytes long.

Measured and rejected: the two clamp arm orders in cases 2 and 3 (4
combinations), the two `nameD`/`nameU` arm orders, `&&` against nested `if`s
for the 0.99/0.01 range, three spellings of `x + 0x1EC - w`, the counter read
before, inside and after the switch, and the handle and width as one local
with casts.

## nbCameraShot_InvokeTarget_Set2 (newbattle/nbCameraSkill, 0x2C0)

171 of 176 words, at the exact 704-byte size.

The five differing words are where `100.0f` is materialised for the radius
clamp. Retail emits `mul.s $f21, $f1, $f0` for `width * pInvoke->scl` and then
`lui`/`mtc1` into `$f0`, reusing the register that held `scl`, so the constant
cannot be hoisted above the multiply. The draft gets `$f2` for the constant,
the anti-dependency disappears, and the scheduler moves the `lui`/`mtc1` pair
in front of the multiply; the compare and the later `div.s` then read `$f2`
instead of `$f0`.

Ruled out, all giving the same five words: `radius > 100.0f` and
`100.0f < radius`, `radius *= 100.0f / radius` and
`radius = radius * (100.0f / radius)`, folding `width` and `radius` into one
local, and swapping the declaration order of the four `f32` locals.

Everything else is exact, including the `bc1tl` fallback to `pInvoke->r` when
the all-table radius is zero, `targetPos.x > invokePos.x` for the load order
of that compare, `1.7f` written out in both arms so it takes two pool slots,
the two `SDF_VU0_STOREVECTOR_NOMEM` of the same sphere centre into `targetPos`
and `look`, and `effBTLModelLook2(pTarget, &invokePos, 0.5235987f)`.

## nbCameraShot_InvokeTargetReverse_Set2 (newbattle/nbCameraSkill, 0x2B8)

169 of 174 words, at the exact 696-byte size.

The same five words as `nbCameraShot_InvokeTarget_Set2` above: the `100.0f`
radius clamp lands in `$f2` rather than retail's `$f0`, so the `lui`/`mtc1`
pair loses its anti-dependency on `pTarget->scl` and the scheduler moves it in
front of the multiply. Read that entry for the shapes already ruled out, plus
`width = 100.0f / radius; radius *= width;` which costs nine words here.

What this function needed beyond its mirror: the `0.69813163f` fovy value has
to be a named local assigned before the `sdfAtan` call, not a literal in the
store. Retail keeps it in `$f20` across the call, which only the named local
reproduces; written as a literal in `pCamera->fovy = 0.69813163f;` the
constant stays caller-saved, the frame loses the third saved fp slot and
drops to 0x70, and 160 of 171 words differ. The same literal written directly
is correct in `nbCameraShot_InvokeTarget_Set2`, where retail materialises it
after the call into `$f2`.

## fldAutoMapClear (field/fldAutoMap, 0x9C)

37 of 39 words, at the exact 156-byte size.

Two loops zero 30 and 8 rows of 32 quadwords in `DDS3_GBWK`. The second loop
matches exactly from `u64 *row = (u64 *)(gb + 0x19D10 + i * 256);` and
`*row++ = 0;`, which is what gives retail's ascending inner pointer and its
strength-reduced outer base. The first loop differs in those two words only:
retail starts the row at `0xFC58` and bumps the pointer by `+8`, while the
draft starts at `0xFD50` (`0xFC58 + 31 * 8`) and bumps by `-8`, because
ee-gcc reverses the inner loop when the address is derived from the index
rather than walked.

Writing the first loop as a `*row++` walk does fix the direction but then
ee-gcc strength-reduces its outer base too, dropping the `sll $2, $4, 8`
recompute retail keeps and landing 34 words out at 148 bytes. Also measured
for the first loop: `row[j]`, a `u64 (*)[32]` cast, `(u64 *)(gb + C)[i*32+j]`,
`i * 256 + j * 8`, a `u32` index on either loop variable, `j != 32`,
`j = 31; j--`, a pointer walked across both loops, and the base cast as
`(u8 *)ofs + (u32)gb`. None reaches below 2 words, and only the ones that keep
the derived address keep the 156-byte size.

Caching `DDS3_GBWK` in a `u8 *` local, separately for each loop, is required:
without it ee-gcc reloads the pointer inside the inner loop, since the `u64`
store may alias it. That alone took the function from 35 differing words to 6.

## func_001F79A8 (event/evtEventViewerMenu, 0xE4)

20 of 56 words, exact size. The jump table, its eleven case labels and every
arm's call match; what differs is which saved register the `busy` flag and the
`viewerinfo` load take ($4/$5 swapped against retail), and two consequences of
that: retail materialises the default arm's `pad >= 0` as `slti`/`sltiu` where
the C gives `nor`/`srl`, and retail cross-jumps the two `return 1` blocks where
the C emits one epilogue inline.

Ruled out: `busy = 1` as an initializer, as a statement after the declaration,
and set only in the arms that need it; reading the switch value into a local
first; the 0x10/0x11 arm written as `if (state == 0) return 1; break;` and as
`if (state != 0) break; return 1;`. Six spellings of the default arm's compare
(`>= 0`, `!(< 0)`, `(< 0) == 0`, `(< 0) ^ 1`, `1 - (< 0)`, an `if`/`else`, a
ternary) all compile to the same `nor`/`srl` pair, so the compare is a byproduct
of the allocation rather than the lever. The eleven explicit case labels are
needed: with only the six reachable values the switch falls under
`case_values_threshold` and becomes a compare tree.

## evtFrameListHead (event/evtEventViewerMenu, 0x138)

39 of 78 words, exact size. The loop, both `D_0032C9E2` reads, the shared
`type * 10` offset and all four table lookups match. The one difference is which
loop invariant gets the ninth saved register: retail hoists the `lui`/`addiu`
pair for `evtFrameList`'s address and rematerialises `D_0054F9B8`'s address
inside the loop, while the C hoists `%hi(D_0054F9B8)` and rematerialises
`evtFrameList`. Both use nine saved registers, so this is gcc's choice of
movable rather than register pressure; the three remaining symbol addresses
(`D_0032C9E2`, `frameheadstr`, `framelist_xcnt`) stay in the loop on both sides.

Computing the column width into a local before the draw is what brings the size
to exact (it was one instruction long without it), so keep that statement order.
Ruled out: `s16 *table = evtFrameList;` as a local, declared first or second
(gcc then hoists both and the body is two words short); `*(s16 *)((u8 *)evtFrameList
+ type * 10)` for the bound; reading `viewerinfo->frameGroup->type` inline at
each use instead of a local (74 of 78); caching the `D_0032C9E2` byte in a local
re-read twice.

## evtSaveViewerHeader (event/evtEventViewerMenu, 0x400)

254 of 256 words, exact size. Every count call, the twelve byteLength products,
the 21-entry jump table, all fourteen directory arms and the `mtlo`/`madd`/`mflo`
payload advance match. What is left is two stores in the file header swapped:
retail writes `byteLength` (0x04) before `count` (0x10), the C writes them the
other way round, with the same registers holding the same values on both sides.

Two placements are load-bearing and should be kept: `payloadOffset = 0xE0` has to
be a statement after the `if (mode != 2) return;`, not an initializer (as an
initializer the function is two words long), and `i` has to be declared before
`payloadOffset` so the two take retail's spill slots at 0x34 and 0x38 (declared
the other way every later slot shifts by four and 54 words differ).

A sweep of all 210 orderings of the header's seven value stores (byteLength, the
four tag bytes in order, count, kind) bottoms out at these two words: the order
count, tag[0..3], byteLength, kind gives retail's constant-to-register mapping
and 2 differing words, while retail's emitted store order (byteLength, tag[0..3],
count, kind) gives the right stores and rotates all six constant registers
instead. No ordering gives both, so the remaining difference is the scheduler
interleaving the constant materialisations with the store run. Also ruled out:
`header.tag[3] = mode + '0'` (13 words; cse does not fold it against the early
return), `count` through a local assigned beside `payloadOffset` (an extra local
moves every spill slot, 235 words), moving the six zero stores to the front or
the back of the block, and computing `byteLength` in one expression or deferring
its last term.

## evtMenuDisp (event/evtEventViewerMenu, 0x1C0)

63 of 112 words, exact size. The signature is recovered in full: nine arguments
(list, column, row, colCount, rowCount, scroll, total, head, item), the ninth
reaching the callee at `0x60($29)`; the panel call takes arguments one to five
unchanged, the grid origin is `column * 16 + 0x7000` and `row * 8 + 0x7900`, the
head's return value advances `py` by `used * 0x60`, the item loop is a `for` whose
index the body does not use, and the scroll arrows sit at
`(column + colCount * 5) * 16 + 0x7060` with y `row * 8 + 0x78B0` and
`(row + rowCount * 12) * 8 + 0x78F0`. `evtMenuDisp_cnt++` is the last statement.

The whole difference is which loop-crossing value gets the ninth saved register.
Retail keeps `row` there and recomputes `row * 8` for the scrollbar; the C keeps
`row * 8` (gcse finds the expression available, since the only two uses are the
`py` initialiser and the arrow y) and spills `row` to a fourth stack slot, which
rotates every allocation from the prologue on. Both sides use the same nine saved
registers and the same frame size.

Writing both arrow coordinates back into `px` and `py` rather than into fresh
locals is worth keeping (60 differing words becomes 49). Ruled out: separate
`barX`/`topY`/`botY` locals in any declaration order; `index = scroll` as a
statement before the loop; a `while` instead of the `for`; `used = 0` after the
panel call; `px`/`py` as declaration initialisers; `(rowCount * 12 + row)` for
the bottom arrow; spelling the arrow y as `row * 8 + 0x78F0 - 0x40`; a shared
`rowY = row * 8` local; typed function-pointer parameters instead of `void *`
plus a cast; and `(evtMenuDisp_cnt & 0x10) != 0`. Every spelling of the two
`row * 8` expressions that folds to the same instructions also unifies under
gcse, so blocking the share needs a source form that keeps them distinct.

## nbE902Process_1 (newbattle/nbMakePacket, 0x90)

2 of 36 words, at the exact 144-byte size.

The whole body is right: the `nbE902Table[e->step](e)` dispatch, the `return -1`
arm, the `unk_08 == 0x25` test and both `dds3PadVibrationSet` calls. The two
words are the argument setup of the first call, which retail orders `addiu $5,
$0, 1` (in the `bne`'s delay slot) then `daddu $4, $0, $0`, and ee-gcc the
other way round. Nothing else moves.

Ruled out: a `switch` with and without an explicit `default: break;`, a plain
`if`, an `s32` local for `unk_08`, and inverting the test so the vibration arm
is the fall-through with the `nbUpdateEventProcessData` tail duplicated. All
five give the identical two-word swap, which puts this in the
instruction-scheduling class.

## Flow5_2 (newbattle/nbMakePacket, 0x98)

17 of 35 words, three words short of retail's 152.

Both `nbAddNegoIdKouho` ids, the `datSearchDevilStock(n->ework->id)` call and
the `evtBitCheck`/`evtBitOn` pair on `n->innen_id + 0xB00` are confirmed, as
are the three `nextnegoid` assignments (`nextkouho[1]` on the stock hit and on
the bit hit, `nextkouho[0]` otherwise).

Two things are still wrong. ee-gcc emits `bgezl` out of the
`datSearchDevilStock` test where retail has a plain `bltz` with the shared
`return 0`'s zero annulled into its slot, and it reads `n->innen_id` once where
retail reads it twice, once into `$4` for each of the two calls. Retail
materialises the zero three times, once per exit path, so the source has one
textual `return 0` reached from all three arms.

Ruled out: `>= 0` and `< 0` for the first test, an `if`/`else if`/`else` chain
and an early `return 0` per arm (both leave the `bgezl`), and nesting the bit
test inside the `< 0` arm. Two distinct `s32` locals for the two `innen_id`
reads is the next thing to try, per the two-reads row of
[matching levers](matching-levers.md).

## Flow10_21 (newbattle/nbMakePacket, 0x98)

11 of 37 words, one word short of retail's 152.

Both `nbAddNegoIdKouho` ids, the three-way `ework->id` chain over `0x5C`,
`0x3D` and `0x61` (plain `beq`s, since the values are not contiguous), the
`evtBitCheck(8) == 0` polarity and the `(u32)dds3GetRandIntA(0, 10) < 5`
unsigned test are all confirmed; the first 26 words are identical. The whole
condition has to be one `||` of the two disjuncts assigning
`nextkouho[1]`, with `nextkouho[0]` in the `else`: an early `return 0` after
the first assignment duplicates the tail and runs eight bytes over.

What is left is the rand test's polarity in the emitted code. Retail puts the
`sltiu` result in `$3` and branches `bnez $3` to the `nextkouho[1]` block,
which it places last; ee-gcc puts it in `$2` and emits `beql $2, $0`, keeping
`nextkouho[1]` as the fall-through, which shifts the two branch offsets. The
`>= 5` spelling with the arms swapped and an explicit `bit` local for the
`evtBitCheck` result are both much worse (10 of 38 over size, and 24 of 38).

## Flow10_10 (newbattle/nbMakePacket, 0x90)

15 of 35 words, one word short of retail's 144.

The `nbAddNegoIdKouho(n, 0x6004A)` call, the three stores (`workid[6] =
0x60009`, `cnt6_36 = 0`, `cnt6_37 = 0`) and both message arms are right, and so
is the single shared `nbDispNegoMessage` call fed by `mes` and `type` locals
set in each arm: writing the call out in both arms instead makes ee-gcc
duplicate the `jal` where retail shares it, which is worse.

What is left is that retail duplicates the fourth argument's `daddu $7, $0, $0`
into the `else` arm as well as setting it before the dispatch, and materialises
`0xD9` ahead of the three stores rather than inside the arm. Assigning the
`else` arm's values as defaults before the test instead reaches only 23 of 32
words, because the `flowmsg` load then happens before the stores.

## Flow8_7, Flow8_15 (newbattle/nbMakePacket, 0x7C each)

18 of 29 words each, two words short of retail's 124.

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

## nbFreeEffectSlot (newbattle/nbMakePacket, 0x2C)

Byte-exact on its own. It is kept behind the fallback because its presence as a
C definition costs four other functions in the unit: `Wait1_nbE901`,
`PartsWait` and `nbE912_Wait1` each go from exact to one differing word, and
`EffDel` from exact to four. All four then emit a plain `bne`/`beq` where
retail annuls the epilogue's `ld $31` into a branch-likely delay slot. With
nbFreeEffectSlot reduced to the `extern` declaration the unit already carries
at the top of the file, all four match and only nbFreeEffectSlot itself is
lost, so the fallback is worth three functions and unblocks the other nineteen
callers still in assembly.

This is the `slDrawWindow` mechanism of
[matching levers](matching-levers.md) with the sign reversed, and the second
instance in this unit after `nbMaeCheckDone`: what the compiler can see of the
rest of the unit changes its delay-slot decisions, and here seeing the callee
makes them worse.

Ruled out for keeping both: a K&R definition of nbFreeEffectSlot, deleting the
`extern` prototype above it, and every spelling of the callers' own guard
(inverted test, `> 0x3C` for `>= 0x3D`, a `goto`, a local for `e->frame`, a
nested `return 0`, and a `s32 dummy; s32 *p = &dummy;` sibcall blocker). The
callers' source shape does not reach this bit; only the callee's visibility
does. Twenty unused `extern` declarations added above the callee leave it
unchanged too, so it is the decl's identity and not cc1's heap layout.

The four discriminating callers all sit above nbFreeEffectSlot's `0x0028FA48`
in address order, which is evidence for a retail translation-unit boundary
between them; see [TU recovery](tu-recovery.md).

## nbCheckBattleSkillError1 (newbattle/nbMakePacket, 0x48)

1 of 18 words, at the exact 72-byte size.

The structure is settled: `if (id < 0x33) { if (id >= 0x31) { if
(nbFindStockSkillError() == 0) { return 2; } } } return 0;`, with exactly one
textual `return 0`, which is what gives retail's three separate
`daddu $2, $0, $0` (two in branch delay slots, one its own block placed after
the `return 2` tail) and leaves the `id < 0x31` compare in `$2` rather than
`$3`. Writing any of the exits as its own `return 0` or `goto zero` instead
hoists one zero into `$2` above the first branch and costs eight bytes.

The one differing word is the `nbFindStockSkillError` test: retail emits a
plain `bne $2, $0` and ee-gcc a `bnel`. The callee is still `INCLUDE_ASM`, so
this is the `slDrawWindow` row of [matching levers](matching-levers.md):
decompile `nbFindStockSkillError` and this should come out. It was attempted
and set aside at 19 of 21 words, 76 bytes against retail's 88. Retail keeps the
unit index and the byte offset as two induction variables and recomputes both
table bases per iteration (`addu $3, $8, $5` off `DDS3_GBWK + 4` for the flag,
`addu $4, $6, $5` off `DDS3_GBWK` for the badstatus at `0x12`); every shape
tried (`ofs` as a local, `w[i]`, a biased `0xA50` base, two raw pointer
expressions, `&&` for the nested tests) lets gcc fold the address into one
strength-reduced giv with negative displacements instead.

## nbSetBadCurseTimer (newbattle/nbMakePacket, 0x2C)

2 of 11 words, at the exact 44-byte size.

The reads, the increment, both stores and the `gp`-relative write-back are all
identical. Retail computes the row once, `addu $7, $2, $3` with the index
first, and reaches the `step` store through a plain copy of it,
`daddu $2, $7, $0`; ee-gcc computes the row twice instead, once in each operand
order, so the two words are that second `addu` and the missing copy.

Ruled out: the cast form `(u8 *)(i * 8) + (u32)nbBadCurseTable` for one, the
other or both accesses (one address and no copy, four bytes short); a
`&nbBadCurseTable[i]` pointer local; two pointer locals with an explicit copy
between them; `*(s32 *)p` for the `step` store; `nbBadCurseTimer++` in place of
the index local; and both store orders, which only swap the registers the
timer and the table base get.

