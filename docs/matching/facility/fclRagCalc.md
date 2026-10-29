# Matching notes: fclRagCalc

## ragDrawList2, ragDrawList3 (fclRagCalc, 0x150 each)

10 of 84 words differ, at the exact 336-byte size, for each of the pair. They
differ only in the scroll and cursor offsets, the row stride, the list offset,
the caption word, the kind constant and the header colour, so one analysis
covers both.

Every instruction matches. The ten differing words are the `ragDrawWork`
pointer landing in the other caller-saved register in each of the two blocks
that load it: retail holds it in `$3` for the first block and `$2` for the
second, this holds it in `$2` and then `$3`. Everything the pointer feeds moves
with it, which is the whole difference; the `lh`/`lbu` of the scroll and cursor,
the row pointer, the `lb` of the selected item in the first `fclDrawPanel`
delay slot, and the three `lw`s of the second panel's position are otherwise
identical.

Ruled out: the work pointer as a local against `(u8 *)ragDrawWork` written out
at every use; declaring the local before and after the scroll read; a separate
local for the cursor byte; and `s16` against `s32` for the scroll. All five give
the same ten words, so nothing in the source reaches the choice.

The header text and sprite-table arguments use the matched `trmDrawListText`
provider's pointer types and integer return contract. The final caption object
is a pointer table, and the other caption is read as a pointer from the packed
exchange data. Correcting these declarations preserves both ten-word residuals;
it does not justify splitting a new data owner to alter address reuse.
