# Non-matching drafts: mc/slMain

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## slSaveData_writeFile (mc/slMain, 0x238)

106 of 142 words match, at the exact 568-byte size.

The 36 remaining words are one register renaming carried through the whole
record-copy block. The instruction sequence is identical; retail holds the
byte offset `52 * slot` in `$5` and `%lo(D_00569740)` in `$2`, and the draft
holds the offset in `$2` and the base in `$7`, so every address temp after it
shifts by one number. Nothing in the block differs otherwise.

What the shape needed, and is worth keeping:

- The save header lives at offset 0 of the save work, so the stores go through
  `((slFileHead_t *)DDS3_GBWK)` written out per statement; that reloads
  `DDS3_GBWK` for every field, which retail does too.
- Two types for the same 52 bytes. The header in the save work is 4-aligned
  (its 20-byte block at `+0x10` copies as `ldl`/`ldr` plus a plain `lw` for
  the trailing word), while the on-card record array is byte-aligned (the same
  block copies as `ldl`/`ldr` plus `lwl`/`lwr`). One struct for both gives the
  wrong half either way.
- The record side is a `u8 (*)[52]` view. Five of the thirteen field stores
  (`+0x1`, `+0x2`, `+0x8`, `+0x10`, `+0x24`) go through the subscript and the
  other eight through a cached `u8 *rec`: that mix is what produces retail's
  two `addu` operand orders and its three copy temps. All thirteen through the
  pointer is six words short, all thirteen through the subscript is twelve
  words over.
- Source order ends `playTime`, `endcnt`, `newfile`, `mode`, `name`,
  `mapOpen`; putting `mode` back in field order after `level` costs nine
  words.

Ruled out: the declaration and assignment order of the two pointer locals;
`rec` as `u8 *` and as `u8 (*)[52]`; a separate `base` local so the symbol
address is created first; `slot` as `u32`; writing the `+0x8` store
base-first and the `+0x1`/`+0x2`/`+0x24` stores index-first with explicit
casts; and five orderings of the last six stores.
