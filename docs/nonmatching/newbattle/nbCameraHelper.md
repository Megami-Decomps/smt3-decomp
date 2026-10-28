# Non-matching drafts: newbattle/nbCameraHelper

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3EffectFileListCreate (newbattle/nbCameraHelper, 1080 bytes)

269 of 270 words match at the exact 1080-byte size and 0x1A0 frame. The body
was adapted from current DDS `effBuildResourceDirectoryList` against the matching
relocation sequence. The SMT3 directory-entry, list-root, and filename
node layouts supply the matching field offsets; the donor's resource-kind
enum maps to the SMT3 type values 0, 1, 2, 4, 8, 0x10, and 0x20. Its direct
extension-dot ternary and adjacent-list insertion relation reproduce the
retail instructions, removing the five dot-test register differences.

The sole remaining difference is at +0x3F8: the draft emits `bgtzl` for the
loop test where retail has `bgtz`. Both have the same `lw` of `entry.flags` in
the delay slot; the distinction is whether that load is annulled when the loop
exits. The provider declaration matches the separate SMT3 `dds3EffectDirRead`
implementation; enabling a prototype did not alter this instruction.

## dds3EffectFileListWindowDrawSprite (newbattle/nbCameraHelper, 280 bytes)

Two of 70 words differ at the exact 280-byte size; positional and aligned
counts agree. The two are the colour's blue store (`sw` at 0x8 of the
prim) and the second corner's `u` store (0x30) emitted in the opposite
order, same registers, same instructions.

Everything else is exact: the `textureHandle == NULL` guard, the
packet-buffer allocation and `sdfDrawTagInit`, both `lh` reads of the
texture's `width` and `height` scaled by 16, the `0x8000 - (work->x << 4)`
and `0x8000 - ((work->y - 0xC4) << 3)` corners with `left` updated in
place by `+= 0x800` and `top + 0x400` written at its store, and the
`sdfGrOtTable[71]` callback through a block-scope pointer.

What is not established:

- The order of those two stores. The colour order in the source is what
  moves it: natural `r, g, b, a` is four words out and `a, g, b, r` is two,
  with the second corner's `u`/`v` pair written after `corner[0].u/.v`.
  Swept and ruled out: all 720 orders of the six stores
  `{r, g, b, a, corner[1].u, corner[1].v}`, all 24 colour orders against
  all 15 insertion positions of the `u`/`v` pair in both internal orders,
  and all positions of `u` and `v` independently. The floor is two in
  every family. Also ruled out: a chained
  `vertex->r = vertex->g = vertex->b = vertex->a = 0x80;`, a `color` local
  for the `0x80`, a `texture` local for `work->textureHandle` (50 words
  when it covers the `lh` reads), the `width`/`height` values in locals
  with the shift at the store and with the shift in the local, a named
  local for the `0x8000`, grouping the `x` pair and the `y` pair together
  (23 words), `* 16` and `* 8` in place of the shifts (46 words), and
  swapping the `left`/`top` declaration order.
