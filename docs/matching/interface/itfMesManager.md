# Matching notes: interface/itfMesManager

## DrawSelectCursor (interface/itfMesManager, 0x2FC)

One hundred twenty-eight of one hundred ninety-one words match at the exact
764-byte size. The frame layout is retail's, including the `itfMesCursor_t *`
spilled to `0x30($29)` and reloaded twice, and every saved-register
assignment matches: `$16` reused for `&panel->selection` then
`&D_00326728`, `$17` the row base, `$18`/`$19` the top and bottom, `$20`/`$21`
the two left edges, `$22` the `0x80` constant, `$30` the draw tag.

Three source facts were needed to get there and should not be re-searched:

- `s32 index = sel->savedIndex;` read once for both the `== -1` guard and the
  `* 168` row offset. Reading the field twice costs a load and rotates every
  saved register.
- `s32 rowOfs = sel->rowCount * 160 - 0xC0;` as a named local, with
  `y = sel->y - rowOfs;` and `y += index * 168;` as two statements. That is
  what puts the row term in `$2` and the item term in `$3`; six other
  spellings of the same arithmetic (one expression either way round, the
  terms as two locals, an `itemOfs` local alone) are 81 to 170 words out.
- The `0x80` has to stay a literal. A named local for it is rematerialised
  and does not reach `$22`; what puts `0x80` in a saved register is the
  `index` local above freeing the pressure that spills the cursor pointer.

The remaining 63 are the order the three stack arrays' elements are stored in
and where the shared constants are materialised, in all three draw blocks.
Retail's emitted order for the first block is `color[2]`, `rect[2]`,
`color[0]`, `color[3]`, `rect[0]`, `rect[1]`, `rect[3]`, `color[1]`, which is
not a rotation of any natural source order: all sixteen rotations of
colour-then-rect and rect-then-colour, plus the rotation the store rule
predicts, give 77 of 191 or worse, and an adjacent-swap hill climb over all
five blocks' statement lists bottoms out at 63. The levers page lists pure
instruction scheduling as not worth a second attempt and that is what this
looks like; if it is steerable it is through something other than statement
order.

## itfMesMngGetVarLocalize (interface/itfMesManager, 0xAC)

One of thirty-four words matches and the draft emits 136 bytes against
retail's 172, so this is a structural draft rather than a near miss. It is
kept for the data layout it pins down, which took the longest to find:

- The output buffer is `pinst + 0x9F4`, written as
  `(char *)((u8 *)pinst + 0x9F4)`. `itfMesMngSetVar`'s `case 10` reads the
  same address and then `strlen`s it, which is what proves the two belong
  together; the function is `void` and leaves nothing useful in `$2`.
- The source table is in `dds3GlobalWork_t`'s `unk1D7C` padding: two-byte
  glyph pairs at `+0x6604`, twenty bytes (ten pairs) per id, and a byte count
  per id at `+0x6654`. Four ids fit exactly between `0x6604` and `gb[48]` at
  `0x6658`, so the table is `{ u8 ch[20]; } name[4];` followed by
  `u8 len[4];`. Nothing is named in the header yet.
- The body copies each pair into the buffer, emitting the lead byte only when
  it has the `0x80` high bit set, and re-reads the count byte as the
  `do`/`while` bound every iteration.

What the draft cannot reproduce is retail's redundancy. Retail keeps three
separate address inductions stepping by two (the lead byte and the second
byte *twice*, once per arm of the high-bit test), recomputes
`addiu rd, gbwk, 4` and `addu rd, gbwk, (id + 0x6650)` inside the loop, and
carries a dead `daddu $9, $14, $0` copy of the global. gcc shares all of it.
Measured and ruled out: a `u8 *base = (u8 *)DDS3_GBWK + 4;` local with
subscripts (120 bytes), the same with the three offsets as `s32` locals
stepped by two (136 bytes, the draft, and the closest), the full constants
`0x6604`/`0x6654` against the bare global re-read at every use (120 bytes),
and merging the two arms into one `if` plus a shared second-byte store (120
bytes). The `+4` split is gcc's own: retail folds it into the pair address
but leaves it in the count's load displacement, from one uniform source
expression.
