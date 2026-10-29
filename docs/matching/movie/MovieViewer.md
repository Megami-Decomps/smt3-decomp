# Matching notes: movie/MovieViewer

## MovieViewerProc (movie/MovieViewer, 0x24C)

6 of 144 words match; the draft emits 576 bytes against retail's 588.

The control flow is recovered; what blocks it is the very first statement. Retail
stores the step byte with a split address, `lui $3, %hi(D_00343E7A)` hoisted into
the first branch's delay slot and `sb $2, %lo(D_00343E7A)($3)` after the join, so
the `%lo` is folded into the store. Every spelling tried puts the whole address in
a register first (`lui` + `addiu`, then `sb 0($2)`), which costs a word and takes
the `lui` out of the delay slot, and the register renumbering that follows
accounts for the rest of the diff. Tried and rejected: a plain `extern u8`
scalar (gp-relative, one instruction), an unsized `extern u8 []` with `[0]` and
with `*`, a sized `u8 [0x40]`, a byte at a constant offset of the unsized
`D_00343E40`, a one-member struct at the symbol, a pointer object plus `&`, and
the absolute constant `*(u8 *)0x00343E7A` (right encoding, wrong register and
not hoisted). gcc only folds the `%lo` when the address goes through
`memory_address`, and none of these reach it.

Recovered and believed right, so the next attempt need not redo it:

- Signature is `s32 MovieViewerProc(void)`, returning 0; the process state lives
  in one object at `movieViewerWork` whose word at +0x10 is `movieViewerFrame[0]`:
  `scrollTop` 0x08, `cursor` 0x0A, `count` 0x0C and the playing flag 0x0E, all
  `s16` but the flag.
- The step byte is `dds3PadButtonState[0x2D] != 0 ? 1 : 0x1E`.
- While playing, the process stops on `dds3WaitMovieEnd()` or on any newly
  pressed button, computed as `(pad ^ prev) & pad` over `D_005518D8[0]` against
  `D_00551F96`, which the function writes back on the way out. `D_005518D8[0]`
  gives retail's `addiu $19, $28, %gp_rel` address form on its own.
- Otherwise, `dds3PadButtonState[0x21] < 0` starts a movie:
  `strcpy(path, "pfs0:/movie/")` (gcc inlines the 13-byte copy), `strcat` with
  the step entry plus 4, then `moviePlayFile_27041744(path, D_00343EF0)`. `path`
  is 0x80 bytes.
- With no entry left it replays by index and advances `movieViewerPhase`,
  wrapping at 0x19 and skipping 0xA, 0xE and 0x12 by one.
- The cursor arms are `work->cursor++` / `work->cursor--` as read-modify-writes,
  which is what gives retail's `lhu` next to the `lh` of the same field, with the
  window following at `cursor - 7` and `cursor`.
