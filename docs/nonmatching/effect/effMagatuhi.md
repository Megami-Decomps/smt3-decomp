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

## effMagaCylinderDraw, effMagaSpiralDraw and effMagaSmokeDraw (effect/effMagatuhi, 0x2B4, 0x2C4 and 0x2DC)

132 of 172, 136 of 176 and 140 of 182 words match, each one word short of retail
(688 against 692, 704 against 708, 728 against 732 bytes). All three are the same
reconstruction with a different particle advance, and all three sit on one
mechanism, so fixing one fixes the three.

Retail keeps `work->managedResource` in `$4` from the prologue load right through
to the `effMagaCylinderDrawTrack` call. The pointer is therefore still in `$4`
on the `count == 0` path, which jumps straight to the call, so retail reloads it
in the loop-exit block and leaves the `jal`'s delay slot a `nop`. ee-gcc here
loads it into `$2` instead, the pointer dies at `valueWork =
...->valueWork`, and the delay-slot filler takes the reload for both paths. That
is the whole missing word, and the rest of the diff follows from `$4` being free
in the draft: the three spilled values (count at `0x24`, respawn at `0x20`, the
tint colour at `0x28`, the same slots as retail) use `$4` as their temp where
retail uses `$2` and `$5`, the `SDF_VU0_COPYVECTOR` destination lands in `$4`
rather than `$5`, and the saved registers rotate, with `valueWork` taking `$30`
where retail gives it `$22` and `fadeIn`/`fadeOut` moving down to `$23`/`$22`
from retail's `$30`/`$23`.

Everything else is exact, including the fade chain (`if (age < fadeIn)`, then
`else if (life - age <= fadeOut)`, then `1.0f`), the `(u32)` float conversion
with its `c.le.s`/`cvt.w.s` pair, the `age` cached across
`effMaga*PartInit`, and the `-(dds3GetRandA(dds3EffectBase) % spread)` respawn.
The two nops ee-as pads between `cvt.s.w` and `div.s` are reproduced.

The sibling `effMagaSmokeConvDraw` in the same unit has the identical tail and
**matches**, with `lw $4, 0x18C($18)` in the `jal`'s delay slot. So retail's own
compile goes both ways on this; the three Draw functions are the side where
gcse kept the load available across the empty path, and nothing in the source
says which.

Ruled out, all leaving the count at exactly 40/40/42: an
`effMagatuhiOwner_t *owner` local for the pointer (one assignment, or reassigned
after the loop, or assigned and then `work->managedResource` written inline at
the call); every ordering of the nine prologue loads (nine sampled, including
`valueWork` first and last); fifteen declaration orders of the locals, which
have no effect whatsoever, as in the `fldAutoMapDrawIconOfsRF` row in
[matching levers](matching-levers.md); `while` in place of the `for`; a
`remaining` local for `life - age`; an `s32` `count` with the loop bound cast;
the `s32 dummy; s32 *p = &dummy;` sibling-call blocker. An explicit
`if (count != 0)` wrapper round the loop is far worse (153 of 174), and
`if (respawn && age >= life)` in place of `if (age >= life && respawn)` is 46.
