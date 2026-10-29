# Non-matching drafts: newbattle/nbPanelProcess

Conventions: [nonmatching drafts](../../nonmatching.md).

## nbPanelDrawPP (0xA84)

671 of 673 words match at the native 2692-byte size, with the same instruction
count and schedule. The two differing words are the reads of `hp` and `maxhp`
for the gauge ratio: retail loads each spill slot straight into the float
register (`lwc1`/`cvt.s.w`), the draft inherits the `lhu` result still live in
a general register and moves it across (`mtc1`/`cvt.s.w`). Both slots are
compiler spill slots, not frame variables, since they sit at 0x8C and 0x90
above the parameter spills, so no declaration can place them; the choice is
reload's and is not reachable from C.

What the body needed, all of it worth more than fifty words each:

- The akuma record pointer and its byte offset are **block-scope per gauge
  block**, not one pair shared by both. The offset is also its own local:
  `ofs = statindex * sizeof(fldAnahoriAkuma_t) + 0xA50;` then
  `(fldAnahoriAkuma_t *)((u8 *)DDS3_GBWK + ofs)`. Writing
  `&DDS3_GBWK->akuma[statindex]` folds the constant the other way round
  (`addu` then `addiu` instead of `addiu` then `addu`), and sharing one pointer
  across the two blocks exchanges two registers over the whole tail.
- `maxmp` is read and stored **before** the `mp != 0 && maxmp != 0` test, not
  short-circuited inside it. Testing the field inline left the maxmp temp in a
  different register, which removed the anti-dependency that orders the float
  conversions and cost 46 words through the two branch delay slots.
- One **function-scope `u32 col`** carries the pulse value in both sdfSin
  blocks, and the 0x200000 block then needs its own block-scope local for the
  final colour. That shared variable is what puts the pulse in a callee-saved
  register; a block-scope one per site lands it in the argument register.
- The second sprite of a pair has its `nbPanelXYZ_t *`, `nbPanelTMXuv_t *` and
  texture handle **assigned after the first sprite call**, not in the block's
  declarations. Declaring them up front reorders the first call's argument
  loads away from field order.
- The two `nbPanelXYZ_t *`/`nbPanelTMXuv_t *` pointers used after an `sdfSin`
  call are assigned **after** the statement holding that call, so they land in
  the same basic block as the call and the scheduler fills the pre-call slots
  with them.
- `(flag & 0x100000) || (flag & 0x200000)` folds to one `and` against
  0x300000. Retail tests the two bits separately, which an `else if` with the
  same call in both arms reproduces through cross-jumping.
- `char buf[112]`: the local area is 112 bytes and sprintf's buffer is the only
  local in it. The size is what the frame needs, not a measured buffer length.

Ruled out for the last two words: `volatile` on the two locals (moves the
slots into the frame variable area), a two-element array or a two-member struct
for them (16-byte alignment and a larger local area), one-step against two-step
division, an `f32` temp for the divisor, block-scope `rate`, and reading the
fields again at the division.

## nbPanelDrawPP1 (0xC98)

690 of 797 words differ at their retail positions. The draft emits 797 of the
native 806 words in the unit; the frame now matches retail at 0x110 and the
prologue and epilogue agree except for a register name. Assembly remains
active; this reconstruction carries no exact-match credit.

`char buf[16]` is the sprintf buffer: retail passes `$29 + 0x10` to sprintf and
the local area above it is pcnum at 0x20, bad at 0x40 and the five spill slots
at 0xA0..0xC0, which fixes the frame at 0x110. The earlier `char buf[112]`
carried over from nbPanelDrawPP inflated it to 0x170 and moved every stack
displacement in the body.

The record at 0xA50 is reached two ways and both appear in retail. The plain
field reads are `DDS3_GBWK->akuma[j].<field>`, which folds 0xA50 into the index
and leaves the member offset in the load displacement; only the two colour
helpers take `(datUnitWork_t *)&DDS3_GBWK->akuma[j].flag`, whose +4 shows as
the 0xA54 constant. Reading hp through the cast instead costs the 0xA50/0xA
shape at every site. That pins three more members of fldAnahoriAkuma_t: maxhp
at 0xC, mp at 0xE and maxmp at 0x10.

The animation alpha must be a named local assigned before `nbPnl_rsetAlpha`:
`anm = nbPnlWork[i].HpAnmCnt << 28;` ahead of the call, then `anm | 0xFFFFFF`
as the colour argument. Folding the shift into the argument computes it after
the call in a call-clobbered register and cost 106 differing words.

### What is left is one register-allocation choice, not a spelling

Retail keeps only the leaf values live and recomputes every composite in each
region. The spill slots are `i` at 0xA0, `j` at 0xA4, `i * 2` at 0xA8, `y + 2`
at 0xAC and `j * 16` at 0xB0. Each of the four regions that touch the akuma
record reloads `j` and `j * 16` and rebuilds `236 * j` with the same
subu/sll/subu/sll pair, and each reloads `i` and `i * 2` and rebuilds the
nbPnlWork row address; `(y + 2) << 3` is reshifted at both
frwGetSmallNumFontFRQ calls. The draft instead computes `236 * j`, the row
offset and `(y + 2) << 3` once and keeps them in callee-saved registers, which
is why it is nine words short and why `i` stays in $23 where retail spills it
and keeps `p` in $20.

That leaf-only sharing is what gcse alone produces: its expression hash matches
only single-insn expressions over the same registers, so `j * 16` is shared and
the dependent `t - j` at each site is not. The draft's extra sharing comes from
cse1, which with -O2 follows conditional branches (flag_cse_skip_blocks) and so
reaches every region from the first one on a single extended path.

Ruled out: the per-block byte offset and akuma pointer that closed the same
share in nbPanelDrawPP, `ofs = j * sizeof(fldAnahoriAkuma_t) + 0xA50;` with its
own block-scope pointer in each of the three regions. It shares *more*, not
less: cse1 merges the three offsets outright and the body falls to 784 words,
twenty-two short. Also ruled out: spelling `(y + 2) << 3` inline versus through
a local, `y2 * 8`, every declaration order of the locals, `(u32)jun < 4`
against `jun >= 0 && jun < 4`, `OfsX > 15` against `OfsX >= 16`, initialising
`w` at its declaration, and inlining the effMagaCylinderCalcColor results into
the nbPnlSprite arguments. A gcse probe shows the two shares are not PRE
decisions: PRE's eight deletions are the frame addresses of the two local
tables, the nbPanelLayoutTable2 %hi, `i * 2`, `j << 4` and three DDS3_GBWK
loads, and by then `236 * j + 0xA50` already has a single occurrence.

Blocks nbPanelPartyDraw and, through it, nbPanelDrawProcess: both need
nbPanelDrawPP1 as a visible C definition in the normal build, which a parked
draft is not.

## nbPanelPartyDraw (0x54C)

332 of 339 instruction words match at the native 1356-byte size. Assembly
remains active; this reconstruction carries no exact-match credit.

The rows at 0x005753A0 are a four-element array of 0x18-byte records, not the
`s32[4][6]` the symbol name suggests; the members in order are ActiveMode,
OfsX, OldHp, OldMp, HpAnmCnt and MpAnmCnt. Addressing them as struct members
is what folds the member offset into the `%hi`/`%lo` pair, which is how retail
reaches rows 2 and 3; the array spelling adds the offset to the induction
variable instead and costs about 130 words.

The y coordinate needs two locals, one counting 0x17 per row from 0 and one
from 0x153, with the second feeding the shaken row so that the -3 stays a
separate `addiu` instead of folding into 0x153. The 0..11 order scan reuses
the outer `i`, which is what puts its counter in a callee-saved register.

One word differs: at +0x3FC retail has `beqzl` where the draft emits `beqz`
on the MP-animation test. Compiling the same body with nbPanelDrawPP1 present
as a C definition in this unit turns it into `beqzl`, so the annulled form
follows the visibility of the callee the taken arm reaches. Six further words
of the loop tail, the order of the two spill stores against the pointer
increments, also fall into place under that same change.

Blocks nbPanelDrawProcess, which needs nbPanelPartyDraw visible in turn.

## nbPanelDrawProcess (0x108)

65 of 66 instruction words match at the native 264-byte size. Assembly remains
active; this reconstruction carries no exact-match credit.

The one differing word is at +0xE4: the draft emits `bnel` where retail has
`bne` on the test after nbPanelStockCheckRun. Compiling the same body with
nbPanelPartyDraw present as a C definition in this unit gives a full match, so
the branch form follows the visibility of the callee in the not-taken arm and
not the source spelling. Reordering the test into an early return, and both
spellings of the comparison, were ruled out.

## nbPanelAuto (0x3A0)

226 of 232 instruction words match at the native 928-byte size. Assembly
remains active; this reconstruction carries no exact-match credit.

The two packed colors are distinct: PanelAutoABGR supplies the animation
color, while PanelAutoBaseColor fades the common panel color. A hidden panel
with zero base alpha returns before changing that common color. Each texture
handle is captured before its color callback. Both sine phases read the
unsigned battle timer independently, and both use 0.2f (native pool bits
0x3E4CCCCC). Keep the float-to-integer conversions and the packed RGB additions.

The two polygon rows start at D_004F0600 and D_004F0614 and are submitted in
reverse order. The three UV rows start at D_004EFE60, D_004EFE70 and
D_004EFE80. The curated base symbols separate these rows from preceding
objects without replacing their data. The source uses complete array bases;
no pointer indexes before an unrelated data owner. Separate drawing-stage
scopes keep the geometry and texture lifetimes bounded to their actual uses.

All remaining differences are in the first sprite of the normal branch:
retail loads width before height, while the draft loads height before width;
the draft also loads UV height before its other three UV components. The
values and call arguments agree. Offsets +0x2D8/+0x2E0 exchange the dimension
loads; +0x2E8 through +0x2F4 rotate the four UV loads.

A qualified no-dumps scheduler trace passes hooked/unhooked/plain assembly
parity. In sched2, width UID 567 and height UID 569 both have priority 8;
height wins the original-order tie (LUID 171 versus 177). UV-height UID 577
and UV-u UID 571 likewise tie at priority 8 and height wins the LUID tie
(173 versus 179). A named width local leaves the six words unchanged.
Caching all six dimension/UV inputs swaps the geometry/UV saved registers
and worsens the result to sixteen differing words; that form was retired.

Reproduce the trace with `tools/cc1probe.sh
src/smt3/newbattle/nbPanelProcess.c --func nbPanelAuto --cflag -DNON_MATCHING
--sched --no-dumps --out <output>`. Preserve `verify: OK` before relying on
its instruction IDs; declaration or source changes can renumber them.

### Why the akuma fields are reached by offset here

`fldAnahoriAkuma_t` 0x0C, 0x0E and 0x10 are the devil's max HP, MP and max MP:
the stores in this unit prove it. Naming them in `include/newdata.h`, which is
where that struct lives, cannot be done yet. Splitting `u8 unk0C[6]` into three
`u16` members changes the alias set of every read through it, and that reorders
`Flow6_54` in `newbattle/NbNegoFlowTwo` enough to move 20 bytes of the image.
So the draft reads them as `*(u16 *)&...unk0C[n]` and the header keeps the byte
array until that unit is reconstructed too.
