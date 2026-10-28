# Non-matching drafts: fclCombineInit

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## func_00206308, func_002065C8 (fclCombineInit, 0x2C0 each)

35 of 176 words differ, at the exact 704-byte size, for each of the pair. They
are the same copy in opposite directions: `func_00206308` moves the fields of
one work struct down into a smaller one, `func_002065C8` moves them back, with
every source and destination displacement exchanged. One analysis covers both.

Every instruction matches. The whole function is twelve copies between the two
pointers, and both big ones are 576-byte block moves that ee-gcc expands with a
runtime alignment test, `or` of the two pointers against 7, then an unaligned
`ldl`/`ldr` loop and an aligned `ld`/`sd` loop. That shape, the twelve copies,
their order, the two 36-byte and one 32-byte straight-line `ldl`/`ldr` blocks,
the loop bounds and the branch delay slots all reproduce from

    *(s32 *)(dst + 0x00) = *(s32 *)(src + 0x10);
    *(u16 *)(dst + 0x04) = *(u16 *)(src + 0x14);
    *(cmbCopy36_t *)(dst + 0x06) = *(cmbCopy36_t *)(src + 0x16);
    ...

with `cmbCopy36_t`, `cmbCopy32_t` and `cmbCopy576_t` as all-`s16` structs, whose
alignment of 2 is what selects the unaligned path.

What is left is one register pair and the scheduling it drags with it. Retail
reuses the incoming `$4` for the first loaded word and for the second chunk of
the 36-byte block, and keeps `$3` for the alignment test; this uses `$3` for the
loaded word and `$4` for the test. Retail also emits the test's `andi`
immediately after its `or`, nine words in, where this emits it after the
36-byte block, 37 words in. The second group of copies allocates and schedules
identically in both.

Ruled out: statement order, searched over the twelve copies including every
swap of the two scalar pairs and moving the block copies ahead of them, which
plateaus at 35 or loses 100 words; `s32` against `u32` for the 4-byte copies;
`u8 *`, `s8 *` and `void *` parameters; explicit `u8 *` locals for the big
copy's two pointers; and the first six bytes as a single block move, which
ee-gcc compiles with `lh` rather than retail's `lhu` whether the alignment
comes from the type or from an `aligned(4)` field, so it cannot be one.

## cmbFindCameraPoint (fclCombineInit, 0x64)

9 of 25 words differ, at the exact 100-byte size. Looks up a type-0x11 scene basic by
name, and on a hit copies the point resource's quadword to the caller's vector
and returns 1.

The quadword copy is `SDF_VU0_COPYVECTOR(out, basic)`, and the result variable
has to be the search result itself so that the failure path returns the `$2`
the call already left at zero.

Retail copies that result into `$3` before the null test and passes the copy to
`dds3PointBasicGetResource`, so the pointer and the return value are two live
pseudos from the first call onwards. Written as `void *basic = (void *)found;`
gcc coalesces the copy away, because the two hold the same value until `found`
is set to 1; moving `found = 1;` above the second call does keep both live and
reaches the exact size, but then gcc parks the pointer in `$16` and hoists the
constant, which is the draft's nine words.

Ruled out: a separate local for the resource pointer instead of reusing
`basic`; the pointer declared first with `found` cast from it; both as separate
declarations with the assignments split out; and the test written on `found`
rather than on the pointer. All four coalesce the copy and run one word short.

## cmbDrawFullScreenPanel (fclCombineInit, 0x68)

20 of 26 words match, at the exact 104-byte size. Calls `cmpDisableRGB`, draws
sprite 0x34000, fills the screen with `cmpDrawBoxF`, allocates a ref GIF tag
and tail-calls the same sprite draw again.

Every instruction is retail's, in retail's registers. The six differing words
are the delay slots of the two `fclDrawPanelSprite` calls and the epilogue
they shift. Retail schedules `lui $4` then `ori $4, $4, 0x4000` and fills the
`jal` delay slot with `daddu $5, $16, $0`; this schedules `lui`, the flags
copy, and then fills the delay slot with the `ori`. Same instructions, same
count, so what is left is ee-gcc's second scheduling pass, as in `cmp_23A918`
above.

Ruled out: an `extern s32 fclDrawPanelSprite(s32 id, s32 arg);` prototype above
the function, which the unit otherwise leaves implicit; an `s32 id = 0x34000;`
local for the sprite id, which the `cmp_23A918` entry also rules out; and
passing `flags` to `cmpDisableRGB`, which the unit's other caller does not.

## cmbDrawResultFooter (fclCombineInit, 0x74)

25 of 29 words match, at the exact 116-byte size. Draws the combine result
footer: two panel sprites around a `cmpDrawBoxGDefault` whose four corner
colours come from a stack array.

The instruction stream, the store order into the array (`corner[2]`,
`corner[0]`, `corner[3]`, `corner[1]`, the last in the call's delay slot) and
every other register match. The four differing words are one register name:
retail builds `color & ~0xFF` in `$2` (`addiu $2, $0, -0x100` then
`and $2, $16, $2`) and stores it from there, while this builds it in `$3`. The
`$2`/`$3` row in the levers page blames a non-void callee whose result is
ignored, but it does not apply here: making `fclDrawPanelSprite2` `void`, which
keeps its own body's `j cmpDrawSprite2` and leaves it byte-exact, changes
nothing in this function.

Ruled out: the mask written `color & 0xFFFFFF00`; an `s32 dark` local for it,
which hoists the `and` above the first call and runs one word long;
`corner[2] = corner[0]; corner[3] = corner[1];` in place of re-reading `color`;
and the `void` callee above.
