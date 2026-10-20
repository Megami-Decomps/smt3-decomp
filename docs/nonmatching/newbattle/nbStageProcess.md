# Non-matching drafts: newbattle/nbStageProcess

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## nbAddOrder (newbattle/nbStageProcess, 0x9C)

17 of 37 words match, 148 bytes against the native 156.

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

10 of 44 words match, 176 bytes against the native 172.

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

4 of 44 words match, 176 bytes against the native 180.

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

6 of 42 words differ, at the exact 168-byte size.

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

## DispComm1PanelComm (newbattle/nbStageProcess, 0xE0)

53 of 56 words match, at the exact 224-byte size.

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

## nbSetWaitMotion (newbattle/nbStageProcess, 0xD8)

8 of 54 words differ, at the exact 216-byte size, with the same instruction multiset.

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

57 of 74 words differ, 296 bytes against the native 300.

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

9 of 58 words differ, at the exact 232-byte size.

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

9 of 57 words differ, at the exact 228-byte size.

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

29 of 97 words differ, at the exact 388-byte size and with the same instruction
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

## nbGetMotionNo (newbattle/nbStageProcess, 0xC0)

23 of 46 words differ, 184 bytes against the native 192, and every differing word is
the shift caused by one missing register copy.

Retail builds three address pseudos for the three reads out of the motion
record: `$4 = 28 * mot + visual` with the `cr` field's whole `0x38` in the load
displacement, `$5 = visual + (28 * mot + 0x30)` for `movey` at `0xC`, and
`$2 = $5` for `movez` at `0x10`. The draft reaches the first two, so the two
associations (`(index + base) + const` against `(index + const) + base`) are
right, but the third read shares `$5` and the `daddu $2, $5, $0` is missing,
which also drops the `nop` `ee-as` pads the first arm with.

Ruled out: both cast orders of the second address, a `u8 *` pointer local for
one read and the expression for the other, two distinct `u32` offset locals,
two distinct pointer locals, a second `nbGetUnitVisual(work)` call at the third
read (which reloads and runs over), the two reads in the other source order,
`0x3C`/`0x40` displacements off a bare `28 * mot` offset (which merges all
three into one base and is two words shorter), and an `s32` offset local.

The 0x1C-byte motion records at `datUnitVisual_t + 0x2C` are named here by a
unit-local `nbUnitMotion_t`, since the shared header splits the same bytes as
`part[]` six bytes later. `nbGetMotionActframe` matches through the same view.

## nbDrawKoukaSprite (newbattle/nbStageProcess, 0x108)

24 of 66 words differ, at the exact 264-byte size and with the same instruction
multiset.

The digit loop, the `n == 0` fixup and the draw loop are all exact. What is
left is the three-way test on `align`: retail fills the `beqz`, `bgtz` and
`beq` delay slots with `sll $2, $16, 1` (the `n * 2` the two bodies share) and
a `-1` materialisation, and places the `n * 96` block ahead of the `n * 192`
block. The draft hoists the draw loop's `n--` into those slots instead, which
forces `bgtzl` and `beql` where retail has the plain branches, and lays the
`n * 192` block first.

The test chain itself is right as nested `if`s: `if (align != 0) { if (align <=
0) { if (align == -1) ... } } else { x -= n * 96; }` gives retail's `beqz`,
`bgtz`, `beq` with the two bodies cross-jumped onto one `subu`.

Ruled out: a `switch (align)` over 0 and -1, which ee-gcc 2.96 leaves as an
ascending compare list (two case nodes are below the balancing threshold) and
tests -1 first; the same switch with an empty `case 1:` or `case 2:` added to
raise the node count, both of which run over size; `default: break;`; the
`else if (align <= 0)` spelling of the same chain; `align <= 0 && align == -1`
fused into one condition; and four spellings of the draw loop (`for (n = n - 1;
...)`, `for (n--; ...)`, `while (--n >= 0)`, a separate index local, and an
`if (n > 0)` wrapping a `do`/`while`), none of which keep the decrement out of
the delay slots.

## AnimeCommandList (newbattle/nbStageProcess, 0x18C)

97 of 99 words match, at the exact 396-byte size and with the same instruction
multiset.

The four flag arms (`0x20` on `helpa`, `1` and `2` on `alpha`, `8` on `ox`) are
exact, including the `movn`/`movz` clamps and the `(s32)(ox * 0.5f)` step with
its `d == 0 ? -1 : d` fixup.

The two remaining words are the order of the two loads that open the `0x4` arm.
Retail loads the `0.01f` literal first and `smritu` second; every source form
tried loads `smritu` first, because gcc expands the compare's left operand
first and then rematerialises the constant at the compare.

Ruled out: `0.01f > fabsf(...)` and `!(fabsf(...) >= lim)`, both of which
`fold` rewrites back; a block-local and a function-local `f32 lim = 0.01f`,
which gcc rematerialises (the function-local form also hoists the load into the
entry block and costs 95 words); `f32 a = fabsf(s->smritu)` as a separate
statement; an `f32 ritu` local for the field; both source orders of the
then-arm's two stores; and `extern f32 D_0054E500`/`D_0054E504` globals in
place of the literals, which are MEMs rather than constants and get hoisted
into the `beqz`'s delay slot, where retail has a `nop` (19 words).

## RunSake (newbattle/nbStageProcess, 0x104)

45 of 65 words match, at the exact 260-byte size and with the same instruction
multiset.

Three facts are settled. The quadword position copy is
`SDF_VU0_COPYVECTOR(&pos, &form->pos)`: the macro's two `"r"` operands are what
give retail's `addiu $3, $5, 0x20` address temp per copy site, where a `u128`
cast folds the offset into the `lq` and a struct assignment of a 16-byte
aligned type drops to `ld`/`sd` pairs. `form->basic` is a local read once at
the top, which keeps `form` in `$5` with no copy. And `form->range` is read
inline inside each arm rather than through a shared local, which is what leaves
the `bc1f`'s delay slot empty for the else arm's `neg.s` and gives retail's
`bc1fl`.

What is left is the float register assignment: retail has `phase` in `$f1`,
`1.0f` in `$f2` and `-power` sharing `$f4` with the `0.99f` constant, while the
draft pairs `1.0f` and `-power` in `$f1` and puts `phase` in `$f2`. The
schedule shifts with it, and in both arms retail's literal load comes one word
before the `form->damageRange` load where the draft has them the other way
round.

Ruled out: all six statement orders in the two arms, in each of three spellings
of the `form->range` update (`+=`, the written-out form, and the multiply
commuted) and both operand orders of `form->damageRange * form->range`, which
all produce the identical object; an `f32 q = -power` local for the negation;
and `f32 phase`/`f32 power` locals shared by both arms, which reload nothing
and cost 2 words.

## CheckAction_TARSEL (newbattle/nbStageProcess, 0x1CC)

111 of 115 words match, at the exact 460-byte size and with zero instruction
differences: every remaining word is a register name.

The whole dispatch and all three arms are exact. Two source facts carry it: the
`nowcommand` test is written `if (work->nowcommand != 1) { if (... == 5) ... }
else { no = work->nowindex; }` with `no = -1` above it, so the `== 1` arm is the
branched-to block retail places last and the `-1` lands in the `beq`'s delay
slot (the `== 1`/`else if` spelling costs 14 words); and the sixteen
`form[i].stat &= ~8` clears are a plain ascending `for`, which ee-gcc turns
into retail's countdown.

What is left is that retail holds two short-lived constants in `$2` where the
draft holds them in `$3`: the `4` the `err` test compares against, and the `1`
stored to `a->seq` in the `case 2` arm. Both are dead immediately and `$2` is
reused for the return value one word later in retail.

Ruled out: `return ret;` in place of `break;` in the error arm, the select arm
and the `case 2` arm, each of which rearranges the whole epilogue; `u32 ret`; a
separate `ret = 0;` statement after the declaration; `if (err != 4) {} else`;
and both source orders of `nbSetCommSelStat(2)` against `a->seq = 1` and
against `a->stat |= 0x80`.

## SetDamagePacket (newbattle/nbStageProcess, 0x1E0)

116 of 120 words match, at the exact 480-byte size and with zero instruction
differences in the multiset.

Both kuisibari guards are exact, including the two separate `GetTimeListHp`
calls (one for the `hp >= ` compare, one for the `- 1` assignment), the
`else if` chain between the two skills, and the dead arm written first with
the live arm as the branched-to block, both ending in a `j SetTimeList`
sibling call.

What is left is where the `moji` argument's `1` is materialised. Retail emits
`addiu $3, $0, 1` before the `lw $6, 0x28($17)` that fetches `a->form`, so
`$2` still holds the `CheckTimeListDead` return and the constant lands in
`$3`; every source shape tried emits the load and the `sltu` first and then
puts the constant in `$2`, which is free by then. The four differing words are
those three rotated and the one `sw` that names the register. ee-gcc 2.96 runs
only sched1 (before reload) at -O2, so the register choice follows the
schedule and nothing in the source reaches it.

Ruled out: `dead` as the raw return with `!= 0` inlined at the call (which
moves the `sltu` into the argument block and costs 52 words), `!!`, `== 0 ? 0
: 1`, `> 0`, a `u32` and an `s16` `dead`, `if (dead)` against `if (dead != 0)`,
a named `moji = 1` local, `1u`, a `nbFormation_t *form` local for the third
argument, both declaration orders of `start` and `dead`, `start` assigned
rather than initialised, an extra live local to shift the pseudo numbering,
and dropping the `nbMakeDamagePacket` and `nbAddSkillNamePacket` prototypes so
the callees are implicitly declared.

## StageLoad (newbattle/nbStageProcess, 0x1B4)

97 of 109 words match, at the exact 436-byte size and with zero instruction
differences in the multiset.

The shape is settled. The three stack vectors are `pos`, `rot` and `q`
declared in that order, which puts them at the frame's 0x00, 0x10 and 0x20;
both constant vectors arrive by `memcpy` from `extern char D_00543668[]` and
`D_00543678[]`, whose alignment of 1 is what gives retail's unaligned
`ldl`/`ldr` and `sdl`/`sdr` block moves rather than an `lq`/`sq` pair; `q` is
zeroed by a real `jal memset` and then gets `q.w = 1.0f`; and the dispatch is
`s32 state = nbStageState.unk00; switch (state)` with `case 0:` and a shared
`case 1: case 2:`, the `s32` local being what stops gcc dropping the `bltz`
low-bound test that an `unsigned char` index would make dead.

What is left is that `&rot` and `&q` swap between `$18` and `$19`. Retail
holds `&rot` in `$18` with three references of its own (the four `sdl`/`sdr`
words and the `lqc2`) and `&q` in `$19` with two (the `memset` argument and
the `sqc2`), rematerialising the `dds3SetRotPoseSet` argument as
`addiu $5, $29, 0x20`. Every source shape tried lets cse share that argument
with the `&q` pseudo, which raises `&q`'s reference count to the same
`floor_log2` bucket as `&rot`'s and flips the two registers.

Also measured: `-0.34906585f` and `0.69813165f` are the spellings that give
retail's `0xbeb2b8c2` and `0x3f32b8c1`, since cc1 truncates rather than rounds
and the nearest-decimal forms land one ulp low.

Ruled out: `SDF_VU0_STOREVECTOR_MEMOPS(q)`, which does assign the two
registers retail's way but spells the store `sqc2 $vf10, 0x20($29)` and still
copies the call argument from `$19` (two words); `SDF_VU0_LOADVECTOR_MEMOPS`;
`(u8 *)` casts on either macro operand; `&q.x`, a `(u128 *)` cast and a
`(void *)` cast at the call; a `sdfVector4_t *qp` local used for the memset,
the store, or the call; an `f32 q[4]`; all 24 orders of the four
initialisation statements; all six declaration orders of the three vectors
(which only move the frame slots); moving either initialisation into the
`case 0` arm; and `void *`, `u128 *`, `f32 *` and absent prototypes for
`dds3SetRotPoseSet`.
