# Matching notes: effect/billManager

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
