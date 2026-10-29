# Matching notes: sdfTex

## sdfTexSetPacket (sdfTex, 0x168)

**15 of 90 words match**, same emitted size as retail (360 bytes), same
instruction count, and the same dataflow: every GS field, both TEX0 OR trees
and the whole tail from the `ori $2, $0, 0x8000` onward agree
instruction-for-instruction. What is left is the schedule and the register
assignment of the first fifty instructions.

The routine fills a 64-byte `sdfTexPacket_t`: a GIFtag with `NLOOP` 3, `EOP`
and one A+D register, then TEX1 (`0x14`/`0x15`), TEX0 (`0x06`/`0x07`) and
CLAMP (`0x08`/`0x09`), the second register of each pair when `aContext` is
non-zero. TEX1 is `0x61` (`LCM`, `MMAG`, `MMIN` linear) with no mipmaps, and
`K | L | 0x360 | MXL` with them. `CLAMP` is `aWrapMode & 0xF` unless
`aWrapMode & 0xFF` is `0xFF`, which means "no clamp". When `aClutAddr` is
non-zero TEX0 gains `CLD` 4, load the CLUT only when `CBP` differs from
`CBP0`.

Two facts the instruction listing does not state:

- Each OR chain is written **most significant GS field first**
  (`cpsm | cbp | tcc | th | tw | psm | tbw | tbp0`, and
  `k | l | 0x360 | mxl` for TEX1), with `CLD` between `cbp` and `tcc`. cc1
  reassociates a `|` chain into a bottom-up pairwise tree, so the tree shape
  is a function of that order and of nothing else: parentheses in the source
  do not survive, and local declaration order has no effect at all. These
  orders were found by compiling every permutation and comparing the tree
  (96 of the 40320 orders give retail's TEX0 tree; 4 of those also give the
  `CLD` tree).
- `TW` and `TH` are the bit length of `dimension - 1`, computed with
  `plzcw` and the `addi` that materialises 31, through
  `PS2_WORD_BIT_LENGTH` in `include/ps2asm.h`. The macro reproduces the four
  retail instructions exactly and in place; see
  [../../levers/asm-macros.md](../../levers/asm-macros.md).

Why the schedule does not close: cc1 treats the four-instruction asm block as
a single insn with unit latency, so the priorities it computes for the
instructions feeding and following the two bit-length computations differ from
the order the hand-written routine used. Splitting the block into separate
one-instruction macros makes it worse, because cc1 then CSEs the two
`addi $0, 0x1F` into one.
