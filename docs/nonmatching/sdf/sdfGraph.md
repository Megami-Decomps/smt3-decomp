# Non-matching drafts: sdf/sdfGraph

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## sdfGrSetFrameSetMode (sdf/sdfGraph, 0x58)

Eleven of twenty-two words differ, at the exact size, and every one of them is
a register name or the one store position that follows from it. The draft and
retail emit the same twenty-two instructions.

Retail holds the row pointer in `$2`, the four `lh`/`lhu` values in `$3` to
`$6`, `zbuffFormat` in `$9` and the `aSet` copy in `$10`, skipping `$7` and
`$8`. The draft needs one register fewer: the last load reuses `$2` as the row
pointer dies in it, so `zbuffFormat` lands in `$6`, `pixelFormat` in `$2` and
the `aSet` copy in `$7`. With `pixelFormat` in the dying row pointer's
register the scheduler also moves its `sb` from fourth to second, which is the
only instruction-order difference.

The `la $2, sdfGrFrameModes($2)` form, where the row address lands in the
index's own register and ee-as spends `$at` on the `%hi`/`%lo` pair, is the one
thing the source does reach, and it needs the five fields read into locals and
a store order whose last two entries are swapped. Written as five direct
`aSet->f = def->f` copies the loads and stores interleave instead, since the
two pointed-to types are different alias sets.

Ruled out, all measured in the real unit:

- All 120 permutations of the store order with the reads ascending, and all
  120 permutations of the read order with the best store order. Nothing beats
  eleven; the five store rotations that match the read rotation reach fourteen
  and everything else seventeen or more.
- `s32`/`u32` locals in place of the `s16`/`u16` ones, a `const` row pointer, a
  `u32` copy of `aMode`, a `sdfGrFrameSet_t *` copy of `aSet`, and the
  `(T *)((u8 *)(i * sizeof(T)) + (u32)base)` cast form of the row address.
- The row pointer assigned after the guard instead of at its declaration,
  which loses the whole entry block.
- Reading the fields straight out of `sdfGrFrameModes[aMode]` at each use, and
  a `sdfGsMemSlice_t **` local for the handle clearing in either the one-local
  or the HD reconstruction's two-local shape. All three run past the next
  function.
- One, two and three extra unused parameters, with the prototype and both
  call sites widened, on the theory that `$7` and `$8` were live at entry.
  The allocation does not move, and the HD reconstruction's signature is the
  three parameters this unit already declares.

This is the "register allocation that needs one register fewer than the C
uses" row of [matching levers](matching-levers.md), reached from the other
side: the draft is one register short rather than one long.
