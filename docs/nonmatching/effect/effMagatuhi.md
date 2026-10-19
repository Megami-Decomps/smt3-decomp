# Non-matching drafts: effect/effMagatuhi

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## effDebugDrawRectLine (effect/effMagatuhi, 0x154)

72 of 85 words match, at the exact 340-byte size and with the same instruction
multiset and the same six callee-saved registers (`$16` radius, `$17` `x1`,
`$18` `y1`, `$19` the flag, `$20` `y0`, `$21` `x0`).

The thirteen differing words are the argument setup for the first
`dds3PrimLine`, reordered. Retail emits `lui $7` for `0x60008080`, then the
`sll`/`addiu` pairs that finish `x1` and `y1`, then `ori $7`, then the two
`lui`s for `0xFF0000`, and sets `$4` and `$5` from `$21`/`$20` last; the draft
sets `$4` first and lets `ori $7` slip between the two `sll`s and the two
`addiu`s. Nothing is added or dropped.

The shape that got it this far is **separate `x1`/`y1` locals**. Reusing `left`
and `top` for the second coordinate pair (the natural reading of retail, which
updates `$17`/`$18` in place) costs 14 more words, because the flag then takes
`$17` and `left`/`top` shift up to `$18`/`$19`: giving the second pair their own
locals shortens `left`/`top`'s live ranges enough to reorder the allocation back.
Declaration order of the locals has no effect at all here, in line with the
`frSetFontString` row in [matching levers](matching-levers.md).

Ruled out for the remaining thirteen, all still at 13 (or worse): `y1` before
`x1`; `top +=` before `left +=`; the add and the scale interleaved per axis;
`x1 = (left + radius * 2) * 16 + 0x7000` as one expression; `radius * 2` inline
at both adds; the `* 16` and the `+ 0x7000` as separate statements; the pair
computed inside the `if`; `0x60008080`, `0xFF0000` and the mode `0` as named
locals; `if (drawCross == 0) return;` and bare `if (drawCross)`. A packet temp
per call (43) and a `tag` local for `effDebugDrawTag` (42) are much worse, and
drawing the two lines in the other order is 18.

This looks like the ruled-out "pure instruction scheduling" row: the registers
and the instructions agree and only their order in one block does not.
