# Non-matching drafts: newbattle/nbCameraHelper

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3EffectFileListCreate (newbattle/nbCameraHelper, 1080 bytes)

Six of 270 words differ at the exact 1080-byte size and the exact 0x1A0
frame; positional and aligned counts agree, since nothing shifts. Five of
the six are one register rename and the sixth is a branch annul bit.

The whole body is otherwise exact: the duplicated `sdfHddValid` arms (both
of which allocate `strlen(path) + 1` and `strcpy` into it, and which gcc
does not cross-jump), the `while ((length = dds3EffectDirRead(directory,
&entry)) > 0)` walk with the test at the bottom, the three `continue`
guards on the directory flag and on `length`, the six-way `else if` chain
over the type bits with the `'t','m','x'` / `'T','M','X'` pairs written as
two `&&` triples joined by `||`, and the node allocation and list linkage.

What is not established:

- The register pair in the extension-dot test. Retail holds the frame
  address in `$2`, the loaded character in `$3` and the `'.'` constant in
  `$4`; the draft coalesces the character into the address's `$2` and puts
  the constant in `$3`, so the address pseudo dies at the `lb` where
  retail's survives it. The `'.'` constant has to be materialised in both
  arms, which the `period = '.';` assignment written in each arm does
  reach (that alone took the draft from ten words to six, and is the
  "constant assigned in both arms" lever read on a compare operand).
  Ruled out on the register pair: `s32` and `char` in all four
  combinations for the character and the constant local, the two
  assignment orders inside each arm, both compare operand orders, a
  `(s32)` cast at the read, declaring the pair before `type`, reusing
  `type` as the character local (36 words), writing the whole
  `if (... != '.') continue;` in each arm (four words long), the same with
  a `goto` to a trailing label, and nesting the rest of the body inside
  `if (dot == period)`.
- `bgtzl` where retail has `bgtz` on the loop's exit test, with the same
  `lw` of `entry.flags` in the delay slot either way. This is the
  delay-slot filler's annul decision; `for (;;)` with an explicit `break`,
  a `while` with the read duplicated at the bottom (241 words), a `flags`
  local for the directory-flag read (ten words) and inverting that test
  all leave it.

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
