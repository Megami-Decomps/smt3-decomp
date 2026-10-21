# Non-matching drafts: fclTerminalUpdate

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## trmGetJumpTerminalNo (fclTerminalUpdate, 0x44)

8 of 17 words differ, at the exact 68-byte size. Returns a terminal number from
`fldAllCnt` when the work's `unk_21F1` is set, and otherwise indexes the work's
own `unk_21F4` table by `unk_11D6 + unk_11D8`.

Both arms are structurally right, the `beql` with the other arm's `lh` in its
delay slot included. What differs is one hard register: retail keeps the whole
`unk_21F1 != 0` arm in `$2` (`lhu`, `sll`, then `lh $2, fldAllCnt+2($2)`), and
because the load's destination is also its index register `ee-as` has to
expand the macro through `$at`. This puts the index in `$3`, so the assembler
uses `$2` itself as the temp and the `lui`/`addu`/`lh` triple differs in all
three words. The sum in the other arm accumulates into `$3` rather than `$2`
for the same reason.

The two halves cannot be had at once. Writing the sum `unk_11D8 + unk_11D6`
puts retail's `lh unk_11D6` in the delay slot but makes the `addu`'s first
operand `unk_11D8`; writing it `unk_11D6 + unk_11D8` gets the operand order
right and puts `lbu unk_11D8` in the delay slot instead. gcc emits the two
loads in the reverse of the source order here and takes the `addu`'s first
operand from the source order, so no spelling satisfies both.

Ruled out: both sum orders; the two values in named locals in either
assignment order, with `+=` and with one expression; `fldAllCnt` as
`s16 [][2]`, as a flat `s16 []` indexed `[i * 2 + 1]`, as a 4-byte struct
array, and as a cast pointer, all four of which make cc1 emit the same
`lh $2, fldAllCnt+2($3)`; a `trmUpdateWork_t *` local for the global; the
guard inverted so the `fldAllCnt` arm is the fall-through; and an `(s32)` cast
on either field.
