# Matching notes: effect/effMagatuhi

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


## func_001AA6D0 (effect/effMagatuhi, 0x628)

The pad-driven camera controller is reconstructed using existing VU0 macros.
The guarded body emits 1,564 of the native 1,576 bytes; differences begin at
entry, and its frame is 0x30 instead of 0x40. It is not an exact match.

The first right-stick channel translates eye and target along the normalized
look direction. The other channels construct normalized cross-product axes,
load the actual `SetRotateMatrixAxis` transform, then rotate target about eye.
The two digital controls translate along the derived up axis, with button 10
winning over button 8. The unsigned-byte dead zone includes both 64 and 192.
Each stick pair is captured before its first operation; the second pair is
loaded after the first pair's optional rotation call. The saved final channel
must not be reread after later rotations.

The scale scratch writes XYZ only. Its volatile stores are the existing
bare-address VU0 hand-off idiom; the full quadword load still observes the
uninitialized W lane, as native does. No invented initialization or padding is
added. The three angular rates have the same binary32 value, represented by
`0.06f`; linear movement uses 22.5 and the original multiplication order.

A qualified no-dumps CSE/allocation measurement on this owner verifies hooked,
unhooked and ordinary compiler output parity. CSE rewrites input loads from a
real local pointer into direct references to the small-data buffer: UID13
becomes buffer+3, UID191 buffer+1 and UID194 buffer+0. The shared horizontal
sample (pseudo86, six references, live187, one crossed call) receives `$16`.
The native additionally retains the input address in `$17`, while this source
folds that pointer away, losing its save/restore and changing the frame.
A native-preserved address lifetime needs a new source premise; forcing the
pointer through volatile storage or an artificial dependency is not justified.

Separate channel locals and chained volatile XYZ assignments were examined.
The latter introduce scalar reloads absent from native; separate XYZ stores
preserve the hand-off without those reloads. An incomplete buffer declaration
also loses native GP addressing, so the existing eight-byte provider view is
retained. All 78 existing owner functions and owned data remain exact.
