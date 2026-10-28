# Non-matching drafts: event/evtEventViewerFrameVar

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## FrameVarProcess (event/evtEventViewerFrameVar, 0x3D4)

10 of 245 words match, 956 emitted bytes against retail's 980. The body matches
instruction for instruction: both guards, all five `sdfPrimLinkRect` argument
blocks, the four `frame * 360 / totalframe` divisions with their `break 0, 7`
pairs, both `movz` selectors, the frame-list loop and the two `D_002F6978[4]`
indirect calls. The whole listing is shifted because retail saves three more
callee-saved registers: `$20` holds 0x74E0 from the entry block and `$21`/`$22`
hold 0x85F8/0x85A0 across the frame-list loop, so retail reaches the bar
coordinates with `addu`/`daddu` where the C leaves them as `addiu`/`ori`
immediates. Six of the twenty-four missing bytes are the `sd`/`ld` pairs for
those three registers; the other three words are the two extra `addu`s retail
needs because the base is in a register.

Writing `(span + 2) << 4` instead of `(span + 2) * 16` is needed and is kept:
with the multiply, `(x + 2) * 16 + 0x74E0` folds to `x * 16 + 0x7520` and the
draft is three words shorter still.

Ruled out: the three coordinates as plain `s32` locals assigned before the loop,
assigned in both arms of the loop's cursor test (two reaching definitions), and
assigned immediately before the first use; `barX + span * 16` against
`span * 16 + barX` at each of the four sites. gcse's constant propagation folds
every spelling of such a local back to an immediate before the loop pass can
hoist it, so nothing in the source reaches the choice; compiling the same draft
with `-fno-gcse` keeps the locals in registers and grows the function to 992
bytes, which confirms the mechanism but is not the build's flag set. This is the
same residual as `evtFrameListHead` in
[evtEventViewerMenu.md](evtEventViewerMenu.md): which loop invariant gets a
saved register is gcc's choice of movable, not something the source spells.
