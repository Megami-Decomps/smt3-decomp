# Non-matching drafts: mc/slMain

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## slSaveData_writeFile (mc/slMain, 0x238)

106 of 142 words, at the exact 568-byte size.

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

## slDrawWindow (mc/slMain, 0x4EC)

87 of 313 words, two words short of the 1260-byte size.

Everything up to the window-height arithmetic is exact, and the arithmetic
itself is reconstructed: the counter is bumped and stored, the first box's
`y` and `h` come from `0xE9 - cnt * 0xB5 / 4` and `cnt * 0x16B / 4` clamped to
`0x34` and `0x16B` from four on, and the four later box pairs divide by 5 and
by 100 exactly as retail does. The draft's argument lists and the `slti`,
`sltiu` tail are right.

The two missing words are one register copy and the `nop` that follows from
it. Retail increments into `$2`, stores and compares that, then copies it to
`$4` for the two multiplies; the draft increments straight into `$4` and needs
no copy. Because the block is then one instruction shorter, the scheduler
hoists the second `li` above the first multiply and puts the second multiply
on `mult1`, where retail leaves both on pipe 0, and `.L00152408` loses the
8-byte alignment `nop` in front of it. Every division and both `movn` biases
are identical. After that point the draft and retail differ only in the order
of the twelve stack-argument stores of each `aDrawBoxG` call, which is the
scheduler working from the shifted block.

Ruled out for the copy: `cnt` as a local, `gSlWindowCnt + 1` stored then
re-read, `gSlWindowCnt++`, `++gSlWindowCnt` as the test expression, a second
local assigned from the first, the increment split into two statements,
products hoisted into named locals, `0xB5 * cnt` operand order, decimal
literals, `h` before `y`, the subtraction split off into its own statement,
all 120 declaration orders of the five locals, and separate locals per block
(that one reaches 88).

## slDrawInfo (mc/slMain, 0x2F4)

183 of 189 words, at the exact 756-byte size, and **exact when slDrawWindow is
compiled as C in the same unit**. That is the finding worth acting on: the
draft here is complete and correct, and banking it only needs slDrawWindow
above it to match.

The six remaining words with slDrawWindow still behind `INCLUDE_ASM` are six
`bnel` where retail has `bne`, on the `gslInfoCode` tests for 1 through 6. The
delay slot holds the next comparison constant in both, so only the annul bit
differs; the tests for 0xB, 0xC and 0xD match either way. With slDrawWindow
defined above it in the unit, ee-gcc drops the annul on all six.

Ruled out while slDrawWindow was still assembly: `gslInfoCode == N &&
slDrawWindow() != 0` fused into one test, the call's result in a local,
`== 0` with an empty arm and an `else`, `if (slDrawWindow())` without the
comparison, `u32 gslInfoCode`, an `s32` return type with `return 0` for the
early exit, and the whole body nested in `if (gslInfoCode != 0)` instead of an
early `return`.

## slDrawError (mc/slMain, 0x4AC)

293 of 299 words, at the exact 1196-byte size, and **exact when slDrawWindow
is compiled as C in the same unit** - measured, not assumed. Same situation as
slDrawInfo above and the same six-`bnel` difference, on the `gslErrorCode`
tests for 1, 2 and 7, which are the three arms that branch on `slDDS3To31Flg`
inside. Nothing else in the draft differs.

slDrawWindow is therefore the one thing standing between this unit and three
matched functions: itself, slDrawInfo and slDrawError, 801 words of retail
code between them.
