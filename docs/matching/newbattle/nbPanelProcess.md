# Matching notes: newbattle/nbPanelProcess

## nbPanelDrawPP (0xA84)

671 of 673 words match at the native 2692-byte size, with the same instruction
count and schedule. The two differing words are the reads of `hp` and `maxhp`
for the gauge ratio: retail loads each spill slot straight into the float
register (`lwc1`/`cvt.s.w`), the draft inherits the `lhu` result still live in
a general register and moves it across (`mtc1`/`cvt.s.w`). Both slots are
compiler spill slots, not frame variables. An actual-owner allocation trace
records `alter_reg` assigning the HP and max-HP pseudos to offsets 0x8C and
0x90. Reload has therefore already produced the expected homes.

A separate post-reload CSE trace identifies the two substitutions precisely.
At instruction UID784, `reload_cse_simplify_operands` changes the float
conversion's input from the word at stack+0x8C to `$4`. At UID786 it changes
the word at stack+0x90 to `$3`. Those transformations produce the two `mtc1`
instructions. Both traces preserve plain/hooked/unhooked assembly parity
without RTL dumps. This establishes the rewriting pass, not the original
source context that prevented those substitutions in retail. The trace does
not observe the equivalence cache's reset decisions.

Writing the HP and max-HP guards as nested positive tests leaves the same
671/673-word result. No source change qualified; the existing draft remains
preferred. A future explanation must account for the post-reload equivalences,
rather than forcing the two already-correct spill homes.

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

All 232 instruction words match at the native 928-byte extent in ordinary C.
The final argument of the first normal sprite call constructs the two real
next-row pointers, then returns the already captured texture. The next sprite
consumes those pointers. This is a compiler-lifetime reconstruction aid, not
historical source spelling or a portable argument-evaluation-order guarantee.
No other argument reads the assigned locals, and their assignments complete
before the call, so the semantics do not depend on argument evaluation order.

Qualified complete-owner compiler dumps locate the dependency before sched1.
In the old form, geometry and UV pointers die at the height and UV-height
loads. Those dying uses win the scheduler ties and leave six words reordered.
In the final form, the genuine next-row computations carry the dying uses
instead, and all current-row argument loads acquire the native order. Moving
only those computations back before the call restores precisely the six-word
mismatch at the same extent. Naming both dimensions instead moves the geometry
pointer's death to the Y load and leaves eight differences. No register binding,
empty constraint, fabricated consumer, alias promise or compiler flag is used.

The two packed colors remain distinct. A hidden panel with zero base alpha
returns before changing the common color. Each texture is captured before its
color callback. Both sine phases independently read the unsigned battle timer
and use 0.2f, native bits 0x3E4CCCCC. Float conversions, packed RGB arithmetic,
callback order and final 0x80808080 base-color reset are unchanged.

Geometry rows 45 and 44 belong to the complete 46-row D_004F0268 object.
UV rows 2 and 0 belong to the three-row D_004EFE60 object. Polygon rows 1 and 0
of the two-row D_004F0600 object are submitted in reverse order. No pointer
crosses an unrelated data owner. Three existing sdf2D declarations are exposed
unconditionally with their actual provider types; the declaration-only control
is completely output-neutral in all three modes.

Independent review verifies semantics, array extents, provider ABI, native
calls and access order. Strict production, SKIP_ASM and retained-C builds pass
complete plain/dump assembly and ELF parity. Both complete-owner checkers give
60 match, 0 differ. All 70 production/retained peers, all 59 SKIP peers and owned
data preserve their code and resolved relocation targets. The minimal ablation
also preserves every peer and data object. Only the two inherited pointer
conversion warnings remain. Integrated retail and regression qualification is
recorded with the landing.

### Why the akuma fields are reached by offset here

`fldAnahoriAkuma_t` 0x0C, 0x0E and 0x10 are the devil's max HP, MP and max MP:
the stores in this unit prove it. Naming them in `include/newdata.h`, which is
where that struct lives, cannot be done yet. Splitting `u8 unk0C[6]` into three
`u16` members changes the alias set of every read through it, and that reorders
`Flow6_54` in `newbattle/NbNegoFlowTwo` enough to move 20 bytes of the image.
So the draft reads them as `*(u16 *)&...unk0C[n]` and the header keeps the byte
array until that unit is reconstructed too.


## nbPanelPressGauge (newbattle/nbPanelProcess, 0x1518)

The target remains assembly-backed. The corrected privately promoted draft
emits 5320 of the native 5400 bytes: 102 of 1330 words match, 1228 differ,
and 80 native bytes are uncovered. This is a diagnostic score, not matching
credit.

`D_00551178` is a single eight-byte signed XY point containing 77 and 37,
bounded by the independent `nbPanelBaseABGR` object at `0x00551180`.
The sixth parameter of `nbPanelSpriteBlendDraw` reads only offsets zero and
four and does not retain or forward that pointer. All twelve direct native
calls are in this function and pass that origin. A dedicated point type now
replaces the incomplete array of twenty-byte rectangle records for this
object and parameter; all existing rectangle types and object storage remain
unchanged. Independent retained-owner verification gives 59 match, 0 differ.

The old incomplete declaration also hid the small-data size from the compiler.
Qualified plain/dump comparisons found identical target compiler assembly in SKIP_ASM
and full compilation, but the assembler expanded address pseudos differently
when the full unit defined the small-data label. The truthful complete point
declaration removes that context discrepancy; both forms now emit 5320 bytes.
This establishes the measured visibility effect, not original source spelling.

The extra fifth local spill in the older baseline was fadedColor, not the
carry-over extraColor. A genuine saved-register binding removes that spill
and restores the native frame, but leaves broad differences and is not
retained. The entry zero and carry-over extraColor values must be preserved:
later global frame observations follow calls. Moving UV address arithmetic
after the color helper or moving the count observation after GaugeRun also
remains nonmatching. GaugeRun itself is read-only, so that count-ordering
control is not evidence of a behavior bug. Continued investigation concerns
actual loop-address lifetimes and allocation, not more register bindings.

The type-only retained-source correction passes independent and integrated
complete-owner checks (59/0), retail SHA1/byte comparison/no-op rebuild,
436 tests, the inline-asm gate, all 144 guarded owners / 501 drafts with
stable inputs, 7 IRX checks and reconciled reports. Source commit
`655853776250951c1ec39b1484ded04ec0c186fd`; exact-source
[CI 41407188989](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41407188989)
attempt 1 passed build and Pages deployment. No exact-match credit is added.

A qualified shared-UV control assigns each of the three birth-animation
`nbPanelSpriteScaleDraw` UV arguments to the actual `uv` pointer before
passing it to the same call. This preserves index loads, calls and consumers;
it adds no alias, volatile access, constraint or memory barrier. The result
is entirely codegen-inert, not merely equally scored: baseline and control
have identical six allocated sections, 746 non-file symbol contracts,
1507 ordered relocations and normalized assembly. Both independently have
plain versus `-da -fsched-verbose=5` output parity. The target remains
5320 bytes with 102 of 1330 words matching; all 59 C peers and owned data
remain exact. This control differs from the earlier lexical scopes around
ordinary sprite-draw temporaries, which also did not recover the native frame.

The real shared pointer survives through the life dump, but the table's full
base address is a separate invariant. In this control, loop optimization
moves its definition UID859/pseudo282 to the outer-loop preheader UID3586
with reported life 2 and savings 2; the next table-base construction is
recognized as the same invariant. Combine folds the shared pointer assignment
and argument copy into a direct address addition, so that source-level
pointer identity does not survive to allocation at these calls.

Native outer birth draws instead form the table address inside their draw
paths at `0x27C844` and `0x27C8A4`. The nested trail loop *does* hoist it at
`0x27C934` and reuses it across draws. The unresolved property is therefore
outer versus inner loop placement, not a blanket absence of native table
hoisting. Repeating pointer spellings or lexical scopes without new evidence
about the separate base-address dependency is not a useful next control.

## nbPanelCursorDraw (0x168)

All 360 instruction bytes match in ordinary C. Allocate the draw tag before
forming the coordinates, compute the far Y coordinate between the first two
color calls, and compute the far X coordinate between the second and third.
These real scalar lifetimes reproduce the native saved-register allocation.
Moving the coordinate calculations before allocation or destructively reusing
the input locals does not match. This establishes a source/output dependency,
not historical source spelling or an unobserved internal compiler decision.

Shifts and screen-offset additions remain unsigned until final s32 narrowing,
preserving the earlier draft's modulo-32 arithmetic. Each color call observes
nbPanelBaseABGR afresh. Both primitive submissions precede the final ordering
table callback lookup. No null check, constraint or dummy consumer is added.

The owner's sdfPrimLinkG4 declaration agrees with its actual provider: a signed
32-bit guest tag address, signed scalar arguments and a typed allocator
callback. All four calls explicitly use EE_ADDR_S(tag). This correction is
output-neutral in production, SKIP_ASM and retained contexts. The complete
owner has 61 matches and no differences. All 70/60/70 peers, owned data and
relocations are preserved. Independent review verifies all native bytes,
12 complete ordinary/direct assembly and ELF parity pairs, callback behavior
and 721 stable inputs.
