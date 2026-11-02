# Matching notes: fclRagInit

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

## ragGetListItem2, ragGetListItem3 (fclRagInit, 0xCC each)

Both weighted item lookups have 51 of 51 words matching at 204 bytes each
using ordinary C. A named signed row byte offset and a real `itemBase = &table[kind * 7]`
intermediate, followed by a separate `item = itemBase + row + 7` assignment,
retain the native post-random-call address grouping and allocation. The table
base, row offset and item pointer retain their actual native meanings. There
are no compiler constraints, volatile accesses, fabricated objects or ABI changes.

Each function snapshots the row count byte before `dds3GetRandA`, as retail
does; the weight and selected item bytes are still loaded after that call.
The unsigned modulo and seven-entry walk are unchanged, including the zero
fallback. The `u8` row index makes `index * 35` fit in signed 32 bits; the
count byte plus one is 1..256, and the remainder fits in signed 32 bits.
Valid table and kind indices remain the existing caller contract, not new
assumed bounds. No value is pinned across the random callback.

The old direct item-pointer initializer matches 29/51 words. Diagnostic
row/weight bindings and a live item-base operand isolated the address-shape
residual, but all were removed: the separated ordinary-C item assignment alone
matches both helpers in the complete owner. A live-only control without the
weight binding regresses to 35/51; retaining it is unnecessary. These are
output-based source-shape observations, not claims about original spelling or
an unmeasured compiler pass.

Final complete-owner checking gives 26 match, 0 differ, including owned data.
Independent normal/diagnostic allocated-section, relocation and normalized
assembly parity passes. Integrated retail checksum and direct byte comparison,
436 tests in 27 files, shell wrapper, inline-assembly gate, all 144 guarded
owners / 502 drafts, seven IRX checks and reconciled report scope pass.

## ragBuildHeartsList (fclRagInit, 0x124)

73 of 73 words match at 292 bytes. A truthful `s8 *items = work->heartItems`
local is shared by the existing indexed store and `qsort`; the original
`count++` expression stays intact. The declared signed-byte array begins at
work + 0x1A. Retaining that real subobject address reproduces retail's base and
old-count copy/increment/address/store/narrow sequence, rather than the draft's
work + 0x19 walking-pointer form. No shifted or fabricated object, type change,
constraint, dummy consumer or provider change is used.

The loop executes 32 iterations and increments its initially zero signed-byte
count at most once per iteration. Thus count stays in 0..32 and every actual
store index is in 0..31; the count cannot overflow. Existing item-byte narrowing
is retained. Valid table/type indices and a live work object remain inherited
contracts, not newly assumed bounds. Keeping the array address does not cache
its contents or the post-sort work fields. The same slot lookup, conditional
registered-heart check, zero-item filtering and flag updates remain; `qsort`
sorts count one-byte elements with the existing comparator, and the final work
value call still reloads the signed unregistered flag afterward.

Reduction controls establish that the pointer alone suffices; an earlier
explicit old-count local was removed from the preferred source. Qualified
plain/dump complete-owner assembly, text and relocations agree for each
measured source. This is output parity and an output-based explanation, not a
claim about a debugger-observed optimizer decision.

Independent final-source owner checking gives 24 match, 0 differ with normal
owned-data checks. Final-source gates pass the retail checksum, all 149 guarded
owners / 596 drafts, seven IRX owners, 26 tooling test files / 427 cases, shell
mocks and documentation checks. Source commit `30a7de849aff13665770c5f9bf987f4592b0025a` passed exact-source
hosted CI [40985106175](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40985106175),
including build and deploy.
