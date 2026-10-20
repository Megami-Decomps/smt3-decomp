# Non-matching drafts: newbattle/nbMakePacket

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## Flow6_36 (newbattle/nbMakePacket, 0xA8)

24 of 41 words match, one word short of retail's 168.

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

44 of 50 words match, at the exact 200-byte size.

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

22 of 48 words match, one word short of retail's 204.

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

39 of 43 words match, at the exact 172-byte size.

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

19 of 48 words match, at the exact 192-byte size.

Same bodies, same case order, same constants. ee-gcc fills the branch delay
slot in front of the last `nbDispNegoMessage` block where retail leaves a
`nop`, so every word from there on is one slot early. Retail has three such
nops after unconditional branches in this function and the draft reproduces
the first two.

The `stagemajor` range test needs an `s32` local: used directly, the `u16`
field gives `ori 0xFF10`, `addu`, `andi 0xFFFF`, `sltiu 7` instead of
retail's `addiu -0xF0`, `sltiu 7`.

## nbFreeFormResources (newbattle/nbMakePacket, 0xD8)

51 of 54 words match, at the exact 216-byte size.

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

## nbEventProcess1 (newbattle/nbMakePacket, 0x7C)

23 of 31 words match, at the exact 124-byte size and with the same instruction
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

## Flow13_8 (newbattle/nbMakePacket, 0xB8)

42 of 46 words match, at the exact 184-byte size.

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

35 of 43 words match, at the exact 172-byte size.

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

## nbGetAiEntry (newbattle/nbMakePacket, 0xE0)

43 of 54 words match, two words short of retail's 216.

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

18 of 56 words match, at the exact 224-byte size and with the same instruction
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

50 of 51 words match, at the exact 204-byte size.

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

46 of 47 words match, at the exact 188-byte size.

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

## nbE902Process_1 (newbattle/nbMakePacket, 0x90)

2 of 36 words differ, at the exact 144-byte size.

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

17 of 35 words differ, three words short of retail's 152.

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

11 of 37 words differ, one word short of retail's 152.

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

15 of 35 words differ, one word short of retail's 144.

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

1 of 18 words differ, at the exact 72-byte size.

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

2 of 11 words differ, at the exact 44-byte size.

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

## EffDel (newbattle/nbMakePacket, 0xB8)

42 of 46 words match, at the exact 184-byte size, and with the same instruction
multiset.

The whole body, both `bnel` field tests and the `effBTLCutGetFrame` block are
identical. The four differing words are one rotation in the loop preheader:
retail leaves the `beqz $16` that guards the packet walk with a `nop` in its
delay slot and then materialises the two compare constants, while ee-gcc moves
`addiu $19, $0, 1` into the slot and the assembler pads the 8-byte loop
alignment with the `nop` instead. That is a delay-slot fill, not a source
difference: the three preheader instructions and their order are the same.

Ruled out: a `for` loop, a `while`, an explicit `if` plus `do`/`while` guard,
three nested `if`s in place of the `&&` chain, a local for `p->cuteff`, and
dropping the `nbKoukaProcessData_t *` local so the list head is subscripted
straight off the call. All six give the identical four-word rotation.

Confirmed on the way: `nbMainProcessData_t` carries the kouka process id at
`+0xF4`, inside what was `unk_E0`, and `dds3GetProcessData` of it yields the
same `nbKoukaProcessData_t` whose `first` sits at `+0x08`.

## Flow1_23 (newbattle/nbMakePacket, 0xD8)

24 of 54 words match, at the exact 216-byte size.

The structure is settled: `nbGetUnitWorkPointer(n->data,
&n->data->party[n->boupartyindex])` before a two-case `switch` over
`nownegophase`, with `case 0` setting `waitframe` from `nbSetNegoNoMotion` and
`case 1` reading the `master_text` row for the returned unit's id. Every
instruction is present in retail's order.

What differs is one register. Retail keeps the work pointer in `$6`, reached
through an explicit `daddu $6, $2, $0` copy of the call's result, and spends
`$2` on the dispatch's compare constant and the default arm's zero. ee-gcc
coalesces the work pointer into `$2` instead, which also makes `case 1`'s
first instruction read `$2`, so the `$2 = 0` of the default arm is no longer
safe in the second dispatch test's delay slot and the branch comes out `beql`
with `case 1`'s load annulled where retail has a plain `beq`. The annul bit
and the register are therefore one fact, not two.

Ruled out: an `s32 phase` local for the switch value, used in the `!= phase`
test as well; a separate `id` local for `work->id`; a `nbMainProcessData_t *`
local for `n->data`; assigning `phase` before the call rather than after; an
`if`/`if` chain in place of the `switch` (39 of 52); `return 0` against
`break` in `case 0` (one word better, kept); and a `nbMasterTextEntry_t *`
row pointer against the subscript written twice (identical).

Confirmed on the way: `nbMasterTextEntry_t` carries an `s32` message index at
`+0x00` and a `u8` negotiation phase at `+0x08`, inside what was
`unk_00[0x9]`; retail reads the first as a `flowmsg` subscript and compares
the second against `nownegophase`.

## Flow5_6, Flow5_18 (newbattle/nbMakePacket, 0xD8 each)

23 of 52 words match each, two words short of retail's 216.

Both are the same body with a different `nbAddNegoIdKouho` id (`0x50009` and
`0x50015`): a `for (i = 0; i < 6; i++)` over `D_003E3158[n->innen_id].phase[i]`
that stops at the first zero `item`, stores it into `n->getitem` and passes it
with the neighbouring `cnt` to `datAddItem`. ee-gcc peels the first test into
the preheader exactly as retail does, hoists `&D_003E3158[0].phase[0]` as the
one loop invariant, and re-reads `n->innen_id` at the loop bottom.

The `cnt` argument has to be written
`((nbInnenPhase_t *)&D_003E3158[n->innen_id].phase[i])->cnt`. Written as the
plain `.cnt` field the `+2` folds into the hoisted base, so gcc keeps a second
loop invariant at `symbol + 0xA`, spends a fourth saved register on it and
grows the frame by 16 (9 of 54 words). The cast keeps the association
`(base + index * 4) + 2`, which is retail's `lhu $5, 2(reg)`.

The two remaining words are both register copies ee-gcc coalesces away:
retail computes the element address into `$3`, copies it to `$2`
(`daddu $2, $3, $0`) for the `cnt` load, and at the loop bottom loads
`n->innen_id` into `$2` and copies it to `$4`. The draft loads straight into
the register it uses in both places. That is the ruled-out
register-allocation class, and every word that differs after `+0x3C` is one of
those two copies or an offset shifted by them.

Ruled out: an `item` local for the value stored and passed; `n->getitem` read
back as the first argument; an `nbInnenPhase_t *` element pointer assigned in
the loop, which lets gcc strength-reduce the whole address and comes out 180
bytes against retail's 216; a `u16 *` cast plus `+2` arithmetic; a `while`
loop with the `i >= 6` break at the bottom; and an `id` local for
`n->innen_id`.

Confirmed on the way: `nbInnenRec_t`'s `phase[13]` entries are
`{u16 item; u16 cnt;}`, not `{u16 item; u16 pad;}`: retail passes the second
halfword as `datAddItem`'s count argument.

## Flow6_32 (newbattle/nbMakePacket, 0x118)

63 of 70 words match, at the exact 280-byte size.

The body is settled. The `(u32)dds3GetRandIntA(0, 0xA) < 2` arm has to be
written **last**, with the whole `hojoskillid` chain nested inside
`if (!(... < 2))`, which is what gives retail's `bnel` to a block placed after
the chain; written as an early `return 0` at the top the arm stays inline and
the function is 54 of 69 words out. The six `hojoskillid` arms are an
`if`/`else if` chain with the whole `nbDispNegoMessage` call written out in
each, not a `mes` local: retail duplicates `lw $2, 0x184` and
`daddu $4, $16, $0` per arm and cross-jumps only the suffix from
`daddu $7, $0, $0` onwards, which is what the duplicated call produces.

The seven remaining words are the shared store block at the end. Retail
annuls `lw $2, nextkouho[n]` into both branch delay slots and reaches
`sw $2, 0x1C($16)` with the zero materialised **after** the store; ee-gcc
loads into `$3`, materialises the zero first and pays a `nop` for it. That is
the register-allocation residual class.

Ruled out: an `if`/`else` for the rand test with one trailing `return 0`
(13 of 70 and over size), and the natural early-`return 0` form of the rand
arm (54 of 69).

## Flow6_37 (newbattle/nbMakePacket, 0x10C)

58 of 67 words match, at the exact 268-byte size.

Settled: two `nbAddNegoIdKouho` calls, a four-way `||` on `n->ework->id` over
`0x5C`, `0x3D`, `0x61` and `0x5B`, then
`evtBitCheck(8) == 0 && n->negoreqtype == 0 && n->cnt6_36 >= 3` with its own
`return 0` (retail reuses `evtBitCheck`'s zero result as that path's return
value, so the function has only two textual `daddu $2, $0, $0`), then
`n->nextnegoid = n->nextkouho[0];` followed by the two level reads and the
`n->cnt6_36 == 3` block. The level test has to be written
`if (evtBitCheck(8) != 0) { if (!(plevel - 0x11 < elevel)) { ... return 0; } }`
with `n->nextnegoid = n->nextkouho[0];` after the inner nest: the `&&`
spelling with an `else` is 22 words out, the nested form with its own
`return 0` is 9.

The nine remaining words are a rotation of three block-local temps: retail
gives `$4` to the `nextkouho[0]` value, `$3` to the compare constant `3` and
`$2` to the `pwork` pointer, and ee-gcc gives them `$2`, `$4`, `$3`. All six
orders of the three statements between the guard and the `cnt6_36 == 3` test
were measured (9, 10, 10, 10, 12, 12), so the order the draft uses is the
best of them and the rotation is below the source.

Confirmed on the way: `nbNegoProcessData_t + 0x8C` is `cnt6_36`, not
`cnt6_58`; the three counters at `0x8C`, `0x90` and `0x94` are `cnt6_36`,
`cnt6_37` and `cnt6_58`.

## Flow6_43 (newbattle/nbMakePacket, 0x10C)

45 of 68 words match, one word over retail's 268.

The two-case `switch` on `nownegophase`, both `nbDispNegoMessage` message
numbers (`0x4E`, `0x61`, `0x62`), the two `nbAddNegoIdKouho` ids and the
`n->data->enemypcnt == 1 && n->cnt6_36 >= 4 && (u32)dds3GetRandIntA(0, 0xA) < 6`
chain are all confirmed, as is the `enemypcnt` test reusing the phase
register that `case 1` already knows holds 1.

The extra word is the `flowmsg[ekucyo]` load in the `0x62` arm. Retail emits
that load three times, twice annulled into the two `bnel` delay slots and once
in the plain `beqz`'s slot, and the `0x62` block then reads `$2` straight from
whichever predecessor's slot set it; ee-gcc keeps all three slot copies and
re-emits the load at the top of the block as well. That is the delay-slot
filler's redundant-insn elimination, not a source shape, and it shifts every
branch offset after it.

Ruled out: nested `if`s in place of the `&&` chain (identical), and the two
`nbDispNegoMessage` calls shared through a `mes` local, which hoists the
argument setup above the test.

## Flow12_3, Flow12_4 (newbattle/nbMakePacket, 0x194 each)

24 of 97 words match each, four words short of retail's 404.

The two are the same body with different `nbAddNegoIdKouho` ids and message
numbers. The whole `case 2` arm, from the four `nbAddNegoIdKouho` calls through
the two `datGetParam` results, the 2x2 rate table and the two
`dds3GetRandIntA(0, 0x64)` tests, reproduces retail word for word but **one
word early**, so the positional count understates it badly: the real difference
is the switch's default arms plus the `plevel`/`elevel` register pair.

Retail's three-case dispatch over `nownegophase` reaches its default from two
compare-tree edges, and each edge carries its own `ld $16` (in the preceding
branch's delay slot), `b` and `daddu $2, $0, $0`, two words per edge that the
draft does not emit: ee-gcc instead makes the `beqz` a `beql`, annuls `case 0`'s
`lw $2, 0x184($18)` into it, and sends both default edges to the shared zero at
the tail. That is the same annul-selection residual as the `Flow8_7` and
`Flow13_8` entries.

The rate table is `pparam < eparam` crossed with `plevel < elevel`, giving
`0x14`, `0x28`, `0x3C` and `0x50`; retail if-converts the two
`pparam < eparam` arms into one `movn $16, $2, $17` selecting `0x14` over
`0x28`. Seven spellings were measured: `plevel` outer written `<` or `>=`,
with nested `if`s or a ternary on `pparam`, all give 73 of 97 with `n` correctly
in `$18`; `pparam` outer, either polarity, nested or ternary, gives 63 of 99 but
moves `n` into `$17` and rewrites the prologue, so the `plevel`-outer form is
the one kept.

Also ruled out on the dispatch: `default: return 0;`, `return 0` in place of
`break` in every arm, an extra `case 3:` with a dead store, and an explicit
`default: break;`. All four leave the `beql` and three of them reshape the
compare tree.

Confirmed on the way: `datGetParam(work, 1)` is what the rate test compares,
`datUnitWork_t + 0x14` is the level both sides read, and the four
`nextkouho` slots are selected `[0]`/`[1]` when `nowselect` is zero and
`[2]`/`[3]` when it is not, against `rate` and `0x64 - rate`.

## Flow7_20 (newbattle/nbMakePacket, 0x130)

49 of 76 words match, three words over retail's 304.

The table read is confirmed: `D_003E2540` is a `[][7]` array of
`{u16 type; u8 lo; u8 hi;}`, indexed `[n->ekucyo][n->qindex]`, and the three
fields are read `type`, `hi`, `lo` with `hi` and `lo` in callee-saved
registers. `type` dispatches a `switch` with `case 0` comparing
`datGetParam(n->pwork, 1)` against `datGetParam(n->ework, 1)`, `case 1`
comparing the two `level` fields with `sltu`, and `default: rate = 0x32;`.
The tail is `dds3GetRandIntA(0, 0x64) < rate` selecting `nextkouho[0]` over
`nextkouho[1]`.

Both comparisons have to be written inverted, `if (!(a < b)) { lo arm } else
{ hi arm }`, so the `hi` arm is the branch target, and each `nowselect`
selection written `if (n->nowselect != 0) { rate = 0x64 - x; } else { rate =
x; }` so the one-instruction arm lands in the branch delay slot.

What is left is that retail reaches **one** shared pair of selection blocks
from both `switch` arms: `jump2` cross-jumps the two arms' identical tails
down to the `bnez`, so `case 0` ends in a bare `b` to the shared `lo` block
and `case 1` falls into it. The draft keeps a `nop` and an extra `b` instead,
three words. Writing the selection out in both arms is what should produce the
merge and does not here; a `use_hi` flag local and a `goto` funnel were not
tried and are the next things to measure.

## Flow1_29, Flow8_7, Flow8_15 (newbattle/nbMakePacket, 0x7C each)

11 of 29 words match, two words short of retail's 31.

The three are the same function with different message ids and return
values: a `switch` over `nownegophase` whose case 0 calls
`nbSetNegoNoMotion` and whose case 1 displays a message and returns a
non-zero code. Because case 1 returns a different value from the default,
retail materialises `return 0` twice: once in the second dispatch test's
delay slot and once at a shared tail. ee-gcc instead annuls the case 1
arm's `lw ekucyo` into that slot (`beql` where retail has `beq`), which
leaves the default's `return 0` for the following `b` and collapses
retail's three exit labels into two, so the whole epilogue shifts.

The same `switch` shape over phases 0 and 1 matches in `Flow1_15` and
`Flow12_30`, where every arm returns 0 and the shared tail needs no second
zero, so the shape itself is fine; the differing return value is what
exposes the delay-slot choice.

Ruled out: case 1 written first; an explicit `default: return 0;`; a
`ret` local assigned in each arm and returned once; `if`/`else if` in
place of the `switch`; a phase local; and `return 0;` per arm instead of
`break`. Adding an empty `case 2:` reaches retail's three-label exit and
31 words with 11 differing, but it also turns the dispatch into
`beq 1`/`slti 2`/`bnezl`, which retail does not have.

## Flow6_33 (newbattle/nbMakePacket, 0xE0)

35 of 56 words differ positionally; aligned, every instruction matches and
only the six dispatch delay slots and the branch offsets are wrong. The
draft is six words long at 62.

The dead `sltiu $2, $2, 0x5` after `dds3GetRandIntA(0, 0xA)` is reproduced
by writing both arms of `if ((u32)dds3GetRandIntA(0, 0xA) < 5)` with the
identical body `n->nextnegoid = n->nextkouho[1];`: ee-gcc cross-jumps the
two blocks, deletes the now-pointless branch and leaves the comparison
dead. The cast is required, since the signed form gives `slti`. The
message arm needs its own `return 0` so the `nextkouho[2]` value lands in
`$3` and the store does not cross-jump with the random arm's.

What is left is the six-term `||` chain over `n->ework->id`. Retail fills
each `beq` delay slot from the fall-through with the next comparison's
constant; ee-gcc annuls the target arm's `lw 0x184` instead, giving six
`beql` and six duplicated loads.

Ruled out: storing before the call in the message arm; a `u16` id local;
an `ekucyo` local; the `hojoskillid` test as a nested `if`; and a `switch`
over the id, which sorts the cases and so cannot give retail's
0x3A, 0x3C, 0x3D, 0x5B, 0x5A, 0x5C order. Inverting the whole condition so
the random arm is written first does give plain `beq` with retail's
fall-through slots (28 of 56 differ), but it also mirrors the block
layout, which cannot reach zero.

## Flow15_6 (newbattle/nbMakePacket, 0x100)

19 of 62 words differ, two words short of retail's 64. Every instruction
matches in order; all nineteen differing words are branch offsets shifted
by the two missing words.

Case order is the lever here: with `case 2` last (any order of 0, 1 and 3
before it) the second dispatch test comes out as retail's plain
`beq $3, $2` with `addiu $2, $0, 3` in the slot. With `case 2` anywhere
else that test becomes `beql` with the arm's `lw 0x184` annulled, costing
four words.

The two missing words are retail's `b` out of the last arm plus a
duplicated `lw 0x184`: retail jumps the `case 2` arm over the
cases-0-and-3 join block, where ee-gcc lets the arm fall into it and
reuses the load.

Ruled out: `default: break;` after `case 2`; `default: mes = 0;` in place
of the leading `mes = 0`; and an empty `case 4:` after `case 2`.

## Flow10_1 (newbattle/nbMakePacket, 0x100)

15 of 63 words differ, one word short of retail's 64.

Three arms each need their own `return 0`, and the last test is written
inverted (`if (datGetRace(n->ework) != 0x21) return 0;`) to get retail's
`bne` with the zero in its delay slot. The `dds3GetRandIntA(0, 0xA)`
comparison is unsigned, as the `sltiu` shows.

What is left is where the `nextnegoid` store goes. Retail keeps the first
arm's and the ternary's values in `$3` with `daddu $2, $0, $0` scheduled
in front of each store, so neither store cross-jumps with the last arm's;
ee-gcc puts every value in `$2` and merges the first arm's store into the
shared tail. Retail's first arm also returns the zero that
`evtBitCheck(8) == 0` left in `$2` through an annulled `bnel` slot rather
than materialising it.

Ruled out: the ternary as an `if`/`else` pair of stores; no early return
in the first two arms (33 of 60 differ); and the ternary without an early
return.

## Flow1_4 (newbattle/nbMakePacket, 0x120)

46 of 72 words differ, at the exact size.

The outer `switch` over `nownegophase` comes out exactly, retail's
three-label exit included, and so does `case 0`. What fails is the
twelve-term `&&` chain over `n->ework->id` ending in
`evtGetAgeOfMoon() == 8`: ee-gcc annuls the target arm's `lw 0x184` into
every `beq` slot, where retail fills each from the fall-through with the
next comparison's constant.

The identical chain in `Flow16_2` matches, so the chain itself is right;
there it is the whole function body and here it is a `switch` arm that
returns 1. Nothing in the source reaches the difference.

Ruled out: `case 1` written first; `return 0;` instead of `break` in
`case 0`; and `if`/`else if` in place of the `switch`.

## Flow7_11, Flow2_5 (newbattle/nbMakePacket, 0x150 each)

Flow7_11 is 1 of 84 words differing, Flow2_5 is 2 of 84, both at the exact
size and with every instruction in place.

Each differing word is the annul bit of the inner `switch`'s range test:
retail emits `beql $2, $0` where the reconstruction emits `beq $2, $0` with
the identical `addiu $2, $0, <next case>` in the slot. gcc only annuls when
the slot insn sets a register the fall-through thread still needs, and here
`$2` is dead on that path, so nothing in the source reaches the bit.
Flow2_5 hits it once in each of its two phase arms.

Case order is otherwise decisive and is already right: 1, 2, 3 in both of
Flow7_11's inner switches, 0, 1, 2, 3 in Flow2_5's. Any other order costs
seven words or more.

Ruled out: a separate `mes` local per phase arm; `default: break;` in the
inner switches; a `qtype` local; and `if`/`else if` for the outer phase
test.

## Flow9_16 (newbattle/nbMakePacket, 0x168)

45 of 89 words differ, one word short of retail's 90.

Same residual as Flow1_29: retail materialises `return 0` in both of the
dispatch delay slots and keeps three exit labels, because `case 0` reaches
the epilogue with `$2` already zero from `datCheckItemFull`'s test.
ee-gcc annuls each arm's first instruction into those slots instead
(`beqzl`/`beql` for retail's `beqz`/`beq`), which leaves the `b` to take
`ld $16` and shifts everything after the dispatch by one word.

Ruled out: all six orders of the three case arms.

## Flow12_4 (newbattle/nbMakePacket, 0x194)

73 of 97 words differ, four words short.

The `case 2` body is right: the four `nbAddNegoIdKouho` calls, `plv` and
`elv` as named locals assigned in front of the two `datGetParam` calls so
they get callee-saved registers, the nested ternaries giving retail's
`movn $16, $2, $17`, and the two `dds3GetRandIntA` ternaries over
`nowselect`.

Two residuals remain. The `nownegophase` dispatch is the Flow1_29 shape
again: retail pulls `ld $16, 0x0($29)` into both dispatch delay slots and
leaves `daddu $2, $0, $0` for the following `b`, where ee-gcc annuls each
arm's first instruction instead. And the allocator puts `plv` in `$16` and
`elv` in `$17`, which is retail's pair swapped.

Ruled out for the register pair: `elv` declared before `plv`; the test
written `elv > plv`; and the two names exchanged throughout.

## Flow6_65 (newbattle/nbMakePacket, 0x190)

79 of 100 words differ, four words long.

The `datGetRace(n->ework) != 0x13` arm is written first so the `== 0x13`
arm is the branched-to one, which is retail's polarity. Two residuals
remain and neither moved under the source forms tried.

ee-gcc if-converts the `== 0x13` arm's `aisyo` ladder into two `movz`,
where retail keeps it branchy and cross-jumps its `aisyo == 2` test with
the other arm's. And the allocator gives `ritu` `$17` and the
`datGetParam` result `$18`, which is retail's pair swapped; `ritu` is
already assigned in front of the leading call, so it does get a
callee-saved register, just the wrong one.

Ruled out: `ppar`/`epar` declared before `ritu`, declared as block locals
inside the `if`, and declared in the other order; and the second arm
written as a braced `else` block rather than a flattened `else if`. The
documented rule that declaration order is inert for register allocation
holds here.

## Flow4_1 (newbattle/nbMakePacket, 0x348)

1 of 210 words differ, at the exact 840-byte size.

Thirteen `n->ework->id == <id> && n->pwork->id == n->ework->id` arms in
retail's order, each setting `waitframe` to 0xF, displaying
`n->flowmsg[n->ekucyo]` with its own message id and returning 2, then a
shared 0x31 arm returning 0. Every word matches except a single annul bit:
the second test of the first arm is `beql` where retail has `bne`. Both
branches carry the same delay slot, `addiu $2, $0, 0x38`, which is the
second arm's compare constant; retail leaves it unannulled in the first arm
and annuls it in all the later ones.

Ruled out: the chain written as twelve plain `if`s instead of `else if`; the
first arm alone written plain, or nested as two `if`s, or with
`n->pwork->id == <id>` against the constant instead of against
`n->ework->id`; and `n->ework->id == n->pwork->id` with the operands
exchanged, in the first arm and throughout.

## Flow1_32 (newbattle/nbMakePacket, 0x400)

231 of 252 words match, four words short of retail's 256.

Twelve `id`-pair arms in the Flow4_1 shape returning 2, then a twelve-term
`||` chain over `n->ework->id` in retail's unsorted order giving the flat
`beq` ladder and message 0x80, then the `datGetRace(n->ework) == 0x13 ||
== 0x12 || n->ework->id == 0x9A` test against `negoreqtype` from
`Flow1_33` giving message 0x127, then message 0x20. Every word up to
+0x2CC matches.

What is left is which pair of the three message blocks cross-jumps. All
three end in the same `addu`, `daddu $7`, `jal nbDispNegoMessage`,
`lw $5, 0x1B0($3)`, `addiu $2, 0x1` tail. Retail merges the 0x80 block into
the 0x127 block's tail and leaves the 0x20 block its own full copy; ee-gcc
merges the 0x127 block into the 0x20 block's and leaves 0x80 its own, which
is one tail fewer overall.

Ruled out: the three blocks as separate `if`s with a `return` in each; as an
`if`/`else if`/`else` with one `return` at the end, and with a `return` in
each arm; the 0x20 block hoisted in front of the 0x127 one by inverting the
race test; the race test split into two nested `if`s; and the 0x20 block as
the `else` of the race `if`. The `||` chain must stay a chain: making the
0x80 block an `else if` arm instead turns the ladder's `beq`s into `beql`s
with `lw $3, 0x184($16)` hoisted into each slot, which costs 50 more words.

## nbCollectBadCurseUnits (newbattle/nbMakePacket, 0xA4)

30 of 41 words match, at the exact 164-byte size, and every differing word
is a register name.

The 0x20-byte `s8 list[32]` gives retail's frame, the
`ofs + (s32)a->data->form` integer form gives its `addu` operand order, and
`dds3GetRandIntA(0, cnt)` indexing `list` gives the tail exactly. The
allocator puts `i` in `$4` and `ofs` in `$6`, which is retail's pair
swapped, and the three post-loop temporaries follow that choice.

Ruled out for the register pair: `ofs = 0, i = 0` in the other order in the
`for` init; the increment moved to the `for`'s third clause; `i * 0xF0`
left for the strength reducer instead of an explicit `ofs`; plain
`a->data->form[i]` indexing; `list[cnt] = i; cnt++;` split in two; an
inverted test with `continue`; and `(s8)i` cast at the store.

## ChkCost (newbattle/nbMakePacket, 0xC8)

34 of 49 words match, one word short of retail's 50.

The `nowcommand` dispatch is right with `case 0` written before the shared
`case 1`/`case 5`: that order gives retail's `beq 1`, `slti 2`, `beqz`,
`beq 5` tree, where `case 1` first costs 29 words. `skill` is
`a->work->nowindex` for 1 and 5 and 0 for case 0, the guard is
`nbCheckSkillCost(a->form->formindex, skill)`, and the table read is
`D_003989E0[a->work->id].ailevel` at the already-named 164-byte stride.

Retail emits `slti $3, $4, 0x3`, `sltu $4, $0, $4`, `xori $3, $3, 0`,
`movn $17, $4, $3` for the tail. ee-gcc emits the `sltu` first and no
`xori`, which is the missing word, and the swapped order gives the table
address a different register.

Ruled out for the `xori`: `ailevel < 3`, `!(ailevel >= 3)`,
`!(ailevel > 2)`, an empty `if (ailevel >= 3)` with the work in its `else`,
and `(ailevel >= 3) ? ret : (ailevel != 0)` as a ternary. All five give the
same two-instruction form.

## nbCheckPackLoad (newbattle/nbMakePacket, 0x100)

62 of 64 words match, at the exact 256-byte size.

The outer loop counts `i` from 9 down to 0 with a separate `ofs` stepped by
`sizeof(nbStatWork_t)`, and `(nbStatWork_t *)((s32)data->statwork + ofs)`
gives retail's `addu` operand order and its reload of `statwork` per
iteration. The inner loop over `file[0..0x19]` and the `motfile` block that
follows it are the same three calls.

The two differing words are a delay-slot swap in the array block only:
retail loads `fileHandleFree`'s argument before the `jal` and fills the slot
with the `buf[j]` store, where ee-gcc fills the slot with the argument load
and stores `buf[j]` ahead of the call. The identical `motfile` block comes
out with retail's order, so the difference is the inner loop's two pointer
induction variables, not the statement order.

Ruled out: a `void *h` local for `fileGetMemHandle`'s result stored after
the free, which costs 58 more words; the same local stored before the free;
a local holding `file[j]` for both calls, equally bad; the two `if`s folded
into one `&&`; and `file[j] = NULL` moved in front of the free.

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

## nbSetActionCommand (newbattle/nbMakePacket, 0x100)

38 of 62 words match, two words short of retail's 256.

Every instruction is retail's except one. Retail keeps `command * 784 + 0x10`
as a single index and adds it to two symbol bases, `datModelParamTable` and
`datModelParamTable + 8`, so the pair of `f32` reads are both at offset 0 of a
computed address. ee-gcc reassociates the same expression the other way,
folding the `+0x10` into the symbol (`datModelParamTable + 0x10`) and dropping
retail's `addiu $2, $2, 0x10`. That one missing instruction also removes
retail's loop-alignment `nop`, and every word from the first guard branch on is
the same instruction with its offset shifted by two.

The rest is confirmed: the 10-iteration `stat &= ~0x800` sweep over
`form[4..13]`, the `data->unk_84` and `nbEncountFormTable[encno].unk_22` guards
in that order with the table byte loaded before both, and the second loop
written as `for (i = 4; i < cnt + 4; i++)`, which is what gives retail's hoisted
`slt $2, $6, $2` guard against `cnt + 4` and the count-down trip register.

Ruled out for the fold: an `ofs = command * 784 + 0x10` local, which stops
ee-gcc hoisting the addresses out of the loop at all (45 words differ);
`&datModelParamTable[...]` array syntax; `(u32)` and `(s32)` casts on the
symbol; writing the second read as `+ 8 + command * 784 + 0x10`; the constant
before the product; and the word-indexed forms
`((f32 *)datModelParamTable)[command * 196 + 4]` and
`(command * 196 + 4) * 4`. All seven give the identical 24-word diff, so the
fold is not reachable from the address expression.

## nbE915Process_1 (newbattle/nbMakePacket, 0x118)

56 of 69 words match, one word short of retail's 280.

The whole body is retail's, including the `e->unk_08` chain
(`< 0x6E` with a nested `== 0`, then `== 0x6E`, then `>= 0xFA` with a nested
`== 0xFA`) and the three `e->unk_08` reloads it forces. The single difference is
the branch in front of `effBTLSkillStart(0x203, 0)`: retail leaves its delay
slot a `nop` and ee-gcc fills it with the `addiu $4, $0, 0x203` from the
fall-through. Every later word is that same instruction one slot early, and the
two branch offsets at +0x38 and +0xE4 are off by one as a result.

`$4` is dead on the taken path in both, since the shared tail sets it for
`nbUpdateEventProcessData`, so nothing in the function blocks the fill.

Ruled out: `!e->unk_08`; `!= 0` with an empty arm and the call in the `else`;
a `switch` over `e->unk_08` with only `case 0`; and duplicating the
`nbUpdateEventProcessData` tail into an early-return arm so the two tails
cross-jump. All five give the identical 13-word diff.

## nbMakeNegoBackPacket (newbattle/nbMakePacket, 0x104)

63 of 65 words match, at the exact 260-byte size.

Every instruction is retail's. The two differing words are the GPR that
`SDF_VU0_SCALEVECTOR` moves the binary32 bits through: retail uses `$2` for
both the `mfc1` and the `qmtc2.ni`, and also for the `addiu $2, $16, 0x20` that
follows it, where ee-gcc takes `$3` for the bits and leaves `$2` for the
address. The macro deliberately lets cc1 choose that register, and nothing in
this function steers it.

Its sibling `nbMakeNegoForwardPacket` matches with the same macro getting `$2`,
so the shape is right and only the allocation differs. The difference between
them is that Forward feeds `nbMakeUnitMoveGetCyDist`'s `f32` return into the
scale through two named locals, which is what gives Forward one more pseudo at
that point.

Three source facts were pinned here and carried over to the matching sibling:
`extern void ToMatrix(void)` and an `f32` return on
`nbMakeUnitMoveGetCyDist` are both needed, or cc1 treats `$2` as holding a call
result and avoids it for the `&dir` address as well;
`SDF_VU0_STOREVECTOR_NOMEM(&pos)` is required, because the plain store's
`memory` clobber lets cc1 share one `addiu $29, 0x10` pseudo in a callee-saved
register where retail rematerialises the address for the store and again for
the packet argument; and the three frame vectors must be declared `v`, `pos`,
`dir` so they land at `0x00`, `0x10` and `0x20`.

Ruled out for the scratch register: a `f32` local for the product, for the whole
scale, and an assignment inside the macro argument; `SDF_VU0_STOREVECTOR`,
`_MEMOPS` and `_RWADDR` for the `pos` store; a `nbFormation_t *` local aliasing
`form`; hoisting `&form->pos` into a local before the scale;
`SDF_VU0_LOADVECTOR_B_RWADDR`; and `void`, `s32` and `f32` returns on
`nbMakeUnitMoveGetCyDist`. All give the identical two-word diff.

## nbPickAiAction (newbattle/nbMakePacket, 0x118)

68 of 70 words match, at the exact 280-byte size.

Both differing words are `daddu rD, $0, $0`: retail zeroes the loop index
first and puts `acc = 0` in the `blez $16` zero-trip guard's delay slot, and
ee-gcc zeroes `acc` first and puts the index in that slot. The instruction
sequence is otherwise retail's.

Two things were pinned getting here. The weighted pick has to test before it
accumulates, `if (r < acc + row[list[i]].ritu) { return list[i]; }` then
`acc += row[list[i]].ritu;`: ee-gcc then CSEs the sum into a temp and leaves
retail's `addu $3, $6, $3` plus `daddu $6, $3, $0` copy, where
`acc += ...; if (r < acc)` adds in place and is 23 words out. And the row base
is `D_00398A08 + work->id * 164 + tbl * 40` over 8-byte entries, so the
`datDevilAITbl_t` added for it is `{ u16 ritu; u16 skill; u32 target; }`, the
same record the HD sources carry.

Worth recording about the data: the AI table region is 40-byte strided with
5 entries of 8 bytes, which the unit's existing `datDevilAI_t` overlay models
as `entry[13]` of 12 bytes over the same 0x04..0xA0 span. The two views cannot
both be right, and `D_00398A08` (`D_003989E0 + 0x28`) is where the 40-byte
tables start, so the 164-byte record is most likely a 0x28-byte header
(including three 12-byte check rows) followed by three 40-byte action tables
and a trailing word. Nothing here needed the overlay changed, and it was left
alone.

Ruled out for the two zeros: `i = 0;` before the `dds3GetRandIntA` call, after
it, and in a `for (i = 0, acc = 0; ...)` or `for (acc = 0, i = 0; ...)` init;
a `while` with the increment written out; and an explicit `s32 *` cursor over
`list` in place of the subscript, which costs two more words.
