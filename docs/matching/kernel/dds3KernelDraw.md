# Matching notes: kernel/dds3KernelDraw

## dds3DrawFade (kernel/dds3KernelDraw, 0x2A8)

A hundred and eighteen of 170 words match, at the exact 680-byte size, with
every instruction's opcode, operand shape and position identical: all 52
differing words are register names. Retail puts the frame id in `$5`, the
`0x80 - fadergba[3]` difference in `$3` and the packed colour in `$7`, and
reuses `$5` for the address temporary each spill is computed in; the draft
gives the frame id `$7`, the difference `$5`, the colour `$6` and the address
temporary `$7`. The eight stack spill slots at `0x0` to `0x1C`, the frame, the
twenty-six packet stores and the three calls all land where retail has them.

Two things had to be found. The packet must be addressed as
`dds3FadePacket[frameId].field` at every store, not through a
`dds3FadePacket_t *` local: the pointer form folds the whole body into
displacements off one base and emits 105 words instead of 170, where retail
computes `(frameId * 0xB0 + offset) + base` per field and spills eight of those
addresses. And the colour's or-tree is `((alpha << 24) | r) | ((b << 16) | q)
| (g << 8)`, which only the flat source order
`0x3F80000000000000 | (g << 8) | r | (b << 16) | (u64)(alpha << 24)` reproduces;
the explicitly parenthesised form that spells retail's tree out is 120 words
out, and the five other flat orders tried are 57 to 120.

The packet is a unit-local `dds3FadePacket_t`: a DMA tag, a GIFtag, four A+D
registers (`0x42` = 1, `0x49` = 0, `0x4A` = 0, `0x47` = 0x31003), a second
GIFtag and four more (RGBAQ, XYZ2, RGBAQ, XYZ2), 0xB0 bytes, two entries. The
alpha byte and the three colour bytes are `fadergba[3]` down to `fadergba[0]`:
retail addresses all four off one base register holding `&fadergba[3]`, which
is what reading `fadergba[3]` first produces.

Ruled out, all 52 or worse: nine declaration orders and the `u32`/`s32`
spellings of the frame id (identical at 52, except `u32 alpha`, which is 149);
the guard as an early `return`, as `(0x80 - fadergba[3]) < 0x80` with the
difference recomputed inside, and with the shift written out at its use; and
computing the difference before the `dds3GetFrameID` call, which costs a word.
Both orders of `.data` and `.reg` within each A+D entry were measured: `.reg`
first is the 52, `.data` first is 151.

The draft depends on `dds3FadePacket_t`, defined in the unit beside the
`fadergba` externs, so a harvest of the body alone loses the type.
