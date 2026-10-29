# Non-matching drafts: effect/billManager

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3BillboardManagerInit (effect/billManager, 0xB8)

44 of 46 words match, at the exact 184-byte size. The two differing words are
the `lui` halves of the two walking pointers' addresses, emitted in the other
order: retail has `lui $3, %hi(D_00316FF0)` in the `beqz`'s delay slot and
`lui $2, %hi(hGeneralBillboard)` after it, the draft the reverse. Both
`addiu %lo` halves, the registers ($18 the `hGeneralBillboard` cursor, $17 the
`D_00316FF0` row cursor) and every other instruction are already retail's, so
only which of two independent `li`-style address materialisations the
scheduler issues first is left.

The register assignment and the `lui` order are coupled through statement
order and pull opposite ways. Assigning the `hGeneralBillboard` cursor first
gives retail's `$18`/`$17` pair and the wrong `lui` order (2 words); assigning
the row cursor first gives retail's `lui` order and swaps the two saved
registers (8 words). No spelling found separates them.

Ruled out, each measured: both declaration orders of the two cursors (no
effect at all, the registers follow statement order); both cursors assigned
above the guard, either one above it with the other inside, and a `while` in
place of `if` plus `do`/`while` (6 to 12 words); `if ((work = request->queueHead)
!= NULL)`, `!= 0`, and an inverted guard with a `goto` to a trailing label (all
still 2); six loop-body forms, a local for `row->kind`, a local for the created
object, both increments moved to the tail, the increments swapped, `&array[0]`
for either base, and a post-increment inside the call expression (all still 2,
so the loop body does not reach it); an `s32` index for either cursor in place
of the pointer walk (8 words, and an index for the row runs the function long);
and `dds3BillboardTag = NULL;` moved below the `pacLoad` (13 words).

A current `--sched --alloc --no-dumps --cflag -DNON_MATCHING` trace filtered
to this function passes hooked/unhooked/plain output parity (`verify: OK`).
Sched1 compares the slot high-half UID49 and row high-half UID52 at equal
priority 2, register weight 1 and cost 1; original order picks UID49 first.
Sched2 repeats the same tie after allocation. The low halves also retain
slot-before-row order. Separately, global allocation ranks row cursor pseudo
86 (7 references, live length 28, priority 5000) ahead of slot cursor 87
(7 references, live length 30, priority 4666), assigning registers 17 and 18.
Thus the load order is already selected before hard-register allocation and
retained afterward; no unresolved register preference explains the two words.
A new source proposal must change that scheduling input while preserving the
real cursor lifetimes, rather than repeat the measured assignment swaps.

The row type is a unit-local `BillGeneralRow` of stride 8 reading its `s16` at
`+4`, which is the same table `dds3BillboardGeneral` reaches as
`BillboardTable[index].kind` at `0x316FF4`, four bytes into the same rows.

## dds3Billboard_P2AAnimDraw (effect/billManager, 0x1D0)

Byte-exact as C. It is kept behind the fallback because its visibility as a
definition costs `dds3Billboard_P2ADraw` one annul bit: with this function
compiled, `P2ADraw`'s `modeFlags & 0x40` test emits `beq $2, $0` where retail
has `beql`. Parked, `P2ADraw` is byte-exact. The trade is 464 bytes here for
696 there at the same matched-function count, so it is worth +232 bytes.

Two earlier readings of this pair were wrong and are corrected here, both by
direct measurement:

- The fallback **does** reach callee visibility. An earlier entry said the
  effect needs a compiled body at that point in the unit rather than merely
  the symbol gone, and that only renaming the definition would close
  `P2ADraw`. Parking this function with `#ifdef NON_MATCHING` closes it, same
  as renaming: 60 match, 0 differ, checksum passing.
- Only this function is the effective neighbour. Parking
  `dds3Billboard_P2ASetNumber` instead, which an earlier reading said would
  work equally and would be the cheaper trade at 432 bytes, leaves `P2ADraw`
  at 1 of 174 words differing. The two neighbours are not interchangeable.

Ruled out, each measured: hiding `dds3Billboard_P2AColorMul` the same way (no
effect); and `dds3BillboardTagLink` present as a C definition or not, which
changes nothing here either way.

`P2ADraw`'s own four levers are recorded here because the draft they came from
is now in the build.

Paired current-provider reorg probes confirm the same residual. Both the
parked source and the same unit with only the animation body unwrapped passed
`verify: OK` with `tools/cc1probe.sh --func dds3Billboard_P2ADraw --reorg
--no-dumps` (the candidate also uses `--as-unit src/smt3/effect/billManager.c`).
Hooked and unhooked assembly had output parity.

The branch at instruction UID227 has `REG_BR_PROB=3999` and
`mostly_true_jump=0` in both compiles. The later `0x80` mask, UID389, changes
ordinary delay-slot eligibility when the animation definition is visible;
the visible-body run selects it with flags `0x11`. The observed difference is
eligibility, rather than the branch-probability classification. Which liveness
input causes that difference remains unresolved.

Destructively narrowing the existing local flags for the final `0x80` dispatch
worsens the caller to three differing words. Early returns after selected modes
and a one-pass dispatch block with breaks both leave the original single-word
residual. None of these forms qualified the full unit, so they were discarded.

The rest of the function needed four separate levers and they are all in the
draft: the plural loop's `out` pointer has to be the index-cast form
`(BillOut *)((u8 *)(i * sizeof(BillOut)) + (u32)obj->resolvedEntries)` for
retail's `addu rd, index, base`; the `frameIndex < 0` arm has to be written
*last* (`if (frameIndex >= 0) { draw } else { frameIndex++ }`), or gcc places
the small arm first and emits `bgez` where retail has `bltz`; the plural
branch's two trailing restores have to be written rotation-then-colour, which
emits colour-then-rotation and stops gcc cross-jumping the colour store into
the shared exit the other three branches use; and the packet-list selection has
to assign `obj->pair.kind` directly in all three arms and let gcc cross-jump
the three `sh` into one. A `u16` or `u32` local for that value, with or without
`goto`s, is if-converted into a `movz` pair (23 words out), and a `switch` over
`flags & 6` gets the size right but reshapes the compare chain (18 words out).
