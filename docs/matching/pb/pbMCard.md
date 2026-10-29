# Matching notes: pb/pbMCard

## pb_draw_error_Process (pb/pbMCard, 540 bytes)

The draft emits 536 bytes, one word short, and 27 of 135 words line up before
that word shifts everything after it. Both halves of the body are otherwise
retail's instruction for instruction: the `switch` on `panel->state` with the
`case 1` arm out of line and its `bnel`-guarded `panel->flags = 0`, the
`case 5` and range-check exits both setting -1 into the shared epilogue, the
`error->code == 9` sprite path with `rsetOtOfs(0x47)`, `rsetAlpha(0)`, the
eight-argument `rsetTest` and `DrwSprite2D` with its two stack arguments, and
the message path's `strlen`, two `sdfGrAllocPacketBuff` calls,
`sdfPrimSetOpaquePacket`, three `sdfDrawTagLink`s, `dds3PrimFrameRect`,
`sdfDevPrintf` reading the attribute word from the row before the text, and the
`sdfGrOtTable[39]` callback.

The missing word is the x centring. Retail keeps
`((0x200 - len * 12) >> 1) << 4` as a separate `sra` and `sll`; combine folds
the draft's into one `sll 3`, which is valid because it can see that
`len * 12` clears the low two bits.

Ruled out for that fold: splitting the shifts into separate statements, a
twice-assigned intermediate for `0x200 - len * 12`, `* 16` instead of `<< 4`,
`-(len * 12 - 0x200)`, and shift counts held in locals. All of them compile to
the identical object, so the block that stops the fold has to come from
somewhere other than the expression's spelling.

Also ruled out: an `if`/`else if` chain for the state test, which puts the
`case 1` arm inline and is five words further out, and a fourth empty case,
which adds a word.
