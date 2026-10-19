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
`.x` respectively.
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
