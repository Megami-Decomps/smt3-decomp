# Non-matching drafts: interface/itfMesManager

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## DrawMesFlow (interface/itfMesManager, 0x18C)

Thirty-seven of ninety-nine words match at the exact 396-byte size, and the
instruction list is retail's: the same three outer `flags` guards, the same
`frDrawFont`/`frDrawFontEx` return tests, the same `panel->fade.kind` byte
tests and the tail `j DrawSelectCursor`.

The whole 62 is one difference repeated and its cascade. Retail fills the
delay slot of the first and third outer guards from the *fall-through*
(`lw $4, 0x1C($16)` and `lw $4, 0x48($16)`) and keeps the next guard's mask
`lui` as a one-instruction block at a shared label; the draft emits `bnel`
instead and annuls a copy of that `lui` into the slot. Same instruction
count, so every later branch displacement shifts and the word diff explodes.
The second guard already comes out as retail's `bnez`, which is why 37 words
match.

This is reorg's `mostly_true_jump`: an RTL `NE` condition is predicted taken
and filled from the target, which is what the draft gets. Ruled out as
levers, all four giving exactly 62: fusing the guard and its inner test with
`&&`, spelling the guard `(flags & X) == 0` rather than `!(flags & X)`, a
local for the speaker chain inside the first guard, and `flags &= 0x38;` as a
statement before the third guard's two tests.

Not the callee-visibility mechanism in the usual direction: the two arms that
differ call `frDrawFont`, which lives in `interface/frFont` and was in another
translation unit for retail too. Converting `DrawRetPageCursor` and
`DrawSelectCursor` in this unit, and adding forward prototypes for
`DrawWindow`, `DrawRetPageCursor` and `DrawSelectCursor`, changed nothing
here (64 to 62, and the 62 is unmoved by the drafts being present).

Confirmed on the way, and worth not rediscovering: `flags` is one `u32` local
read once at the top, and the `(flags & 0x38)` value is written twice rather
than kept in its own local. The selection arm needs `s16 index =
sel->selectedIndex;` *and* the compare written against the field again, which
is what gives retail's `lh` for the test and `lhu` plus `sll`/`sra` for the
argument. Adding the `void DrawWindow(itfMesWork_t *panel);` prototype above
this function is required and not optional: without it the implicit `int`
declaration reserves `$2` and every address temp in the body moves to `$3`.

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

## DrawMsgMng_Process (interface/itfMesManager, 0x70)

Twenty-four of twenty-eight words match at the exact 112-byte size. The
remaining four are one difference: retail puts the loop's argument load
`lw $4, 0xC($16)` in the `jal DrawMesFlow` delay slot and closes the loop with
a plain `bnez`, where the draft emits `bnel` and annuls the argument load into
the branch-likely slot. `DrawMesFlow` is the loop body's only call and is
still behind its own fallback, so this is the callee-visibility shape: recheck
it the moment `DrawMesFlow` matches rather than permuting this function.

Confirmed on the way: the loop has to read `node->next` into its own local
*before* the call, `itfMesLink_t *next = node->next; DrawMesFlow(node->state);
node = next;`. Advancing after the call (`node = node->next;`) is one word
long and 12 words out, as are a `for`, a guarded `do`/`while`, and
`unk8 += 1`. Nesting the body inside `if (dds3NullDrawProc() == 0)` reaches
the exact size too but moves the `$2 = 0` out of the early exit's delay slot
and spends `$2` on the counter; the early `return 0` is what keeps it.

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
