# Non-matching drafts: newbattle/nbMakePacket

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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

## EffDel (newbattle/nbMakePacket, 0xB8)

42 of 46 words, at the exact 184-byte size, and with the same instruction
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
