# Non-matching drafts: fclRagInit

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## ragBuildList2, ragBuildList3 (fclRagInit, 0x174 each)

22 of 93 words differ, at the exact 372-byte size, for each of the pair. They
differ only in the destination offset (`+0x3B` against `+0x44`), the weight
table and which `ragGetListItem` they call, so one analysis covers both.

The function is settled apart from one instruction. It builds a list of four
fixed items plus `rand % 4` random ones, sorts it with `qsort` and returns the
count. The shape that reproduces retail's frame, its `break 0, 7` trap, both
`bgez` downcounting loops, the `s8` write cursor with its `sll`/`sra`
re-truncation, the `slt` guarded third loop and the `(s8)` return is

    s32 roll = dds3GetRandA(0);
    u32 n = 4;
    s8 pick[n];
    s32 extra = (u32)roll % sizeof(pick);

The variable-length array is what puts the trap there: ee-gcc expands a
division by a VLA's size as a divide-by-zero check against the saved size
plus the division, then folds the division itself. `count` has to be a `u8`
set in two steps, `count = n;` before the random loop and `count += extra;`
after, which is what keeps retail's `addiu $20, $0, 0x4` in the loop preheader
and the `andi $20, $2, 0xFF` after it; written as one `count = n + extra;`
ee-gcc proves the result fits a byte, drops both the `andi` and the `sll`/`sra`
of the return, and the function comes out seven words short.

What is left is the modulo. Retail emits `andi $19, $2, 0x3` with no divide,
this emits `divu`/`mfhi`. The fold happens only when the VLA's size variable is
dead after the array declaration; here `n` is also read by `count = n;`, which
keeps it live in a callee-saved register, so the divisor is a register and the
divide stays. The two cost the same number of words, so the size still matches,
and the rest of the difference is the register renaming that follows.

Ruled out: `% n` and `% (u32)n` instead of `% sizeof(pick)`, both of which
divide for real whatever else changes; every combination of `n`, `4` and
`sizeof(pick)` for the loop bounds, the `shuffleIndices` count and `count`'s
initial value, nine of which plateau at 22 and three of which fold `count` away
and lose seven words; a second `u32 m = 4;` for `count`, which ee-gcc merges
with `n`; declaring the array's size variable separately from `n` so the fold
does happen, which then loses the parameter copy and both loop-alignment `nop`s
and measures 86 of 91; and `&work[0x3B]`, `(u8 *)((u32)work + 0x3B)` and a
`u8 *list = work;` local, none of which reproduce retail's `daddu $16, $5, $0`
copy of the second parameter.

## ragGetListItem2 (fclRagInit, 0xCC)

29 of 51 words match, at the exact size. Rolls `dds3GetRandA` modulo the
count byte at +6 of a 35-byte row of `D_0032FC98`, then walks the row's seven
weights and returns the matching byte out of the seven-entry table the second
argument selects.

The loop, the `divu` with its divide-by-zero `break`, the `mfhi`, the
`weight++` hoisted into the `bnez` delay slot and the two `nop`s are all
identical. What differs is one register more in flight: retail holds the row
offset in $17 and the count in $16, carries `pick` in $6 and the counter in
$5, and re-derives the item pointer as `(7 * kind + table) + row` then `+ 7`
after the divide; every shape tried allocates the pair the other way round,
runs `pick` and the counter one register lower, and folds the `+ 7` in before
the divide.

Settled on the way: the division has to be unsigned (`(u32)dds3GetRandA(0) %
limit` with `u32 limit`), otherwise it is `div` where retail has `divu`; and
the row has to be an `s32` byte offset into the array rather than a `u8 *`
row pointer, which otherwise merges the table base into one pointer and runs
20 bytes short.

Ruled out: `row` as a variable and as `index * 35` repeated inline, both of
which give the same 22 words; `weight` declared before `pick`; `pick`
assigned in a statement after the declarations, which adds a register; `item`
declared before `weight`; and `item = weight + kind * 7 + 7`.

## ragGetListItem3 (fclRagInit, 0xCC)

29 of 51 words match, at the exact size. Byte-identical to
`ragGetListItem2` above except for the table, `D_0032FCE0`, and it differs
in the same 22 words for the same reason. Fix one and the other follows.
