# Non-matching drafts: event/evtCommand

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## evtCommand_MODEL_RGB (event/evtCommand, 188 bytes)

Two of forty-seven words differ, at the exact size: the two argument copies in
front of `jal evtSetUnitColorRGB` come out in the other order. Retail emits
`daddu $4, $16, $0` then `daddu $5, $2, $0`; the draft emits the `$5` copy
first. Every other word, every register and the frame size match.

The shape is not in question. Its two siblings in the same unit,
`evtCommand_MODEL_A` and `evtCommand_MODEL_LIGHT_RGB`, have the identical tail
(`daddu $4` from a saved register, `daddu $5` from the call's `$2`, the colour
word in the `jal`'s delay slot) and both compile exact from the same source
shape, `callee(unit, scrGetIntPara(1), color)`, so the two copies are
scheduled apart by something upstream of the call rather than by how the call
is written. `evtCommand_MODEL_A` even has the same frame size, the same two
saved registers and the same single-element colour carrier.

The draft depends on `MMI_RGBA_PACK_128_S2` in `include/ps2asm.h`, the pack by
128 through the `$2` scratch that this unit uses where `MMI_RGBA_PACK_128`
uses `$3`, and on the `evtColorWork_t` carrier declared in the unit.

Ruled out, all leaving the same two words:

- Five prototypes for `evtSetUnitColorRGB`: an `s32` first parameter, a
  `void *` first parameter, an `s32` return type, a `u32` duration and an
  unprototyped `extern`.
- A named `s32 duration` local for `scrGetIntPara(1)`, assigned before the
  colour-carrier store or after it.
- Three declaration orders of `work`, `unit` and `color`; `s32` for `color`;
  `s32` for the unit handle with a cast at the call.
- `f32 r`, `g`, `b` locals for the three colour components in place of the
  call results written at the macros.
- A dead store after the call, an unused `s32 dummy; s32 *dp = &dummy;` pair,
  a self-assignment of `unit` to raise its reference count, and reusing
  `color` for the `scrGetIntPara(0)` key.

Shapes that are worse, so the draft's is right: packing straight into
`work[0].word` and passing that (44 of 45 words, the value is reloaded from the
frame), dropping the colour carrier entirely (16 words and the frame shrinks),
`work[2]` or a second carrier (10 words), and any extra store to `work[0]`
before the pack (21 words and up).
