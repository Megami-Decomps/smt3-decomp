# Matching notes: fclCombineInit

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

### Matched result footer

`cmbDrawResultFooter` is ordinary exact C (116 bytes). Its four corner colors
remain `[color, color & ~0xFF, color, color & ~0xFF]`, between panel sprites
`0x48` and `0x44`. No source arithmetic or store-order change was needed.
The complete default owner checks182 match,0 differ with only this former
guard promoted; other guarded bodies need not be visible.

The old four-word register residual is resolved by the current truthful
`void fclDrawPanelSprite2(s32, s32)` contract, adopted in777f9340. In the
current owner, restoring only that declaration to `s32` reproduces25/29
matching words; restoring only `cmpDrawBoxGDefault` to `s32` retains29/29.
The historical report that the void sprite declaration was inert does not
reproduce in this context and must not be used to park the current body.

Qualified actual-owner allocation traces pass plain/hooked/unhooked output
parity for both declarations. The combined mask/result quantity85 has the
same five references and lifetime10..36 in both; local allocation selects
`$2` with the actual void sprite contract and `$3` in the non-void control.
Its priority remains3846. This isolates the declaration-dependent register
choice without inventing a scalar use, constraint or compiler flag.

## cmbDrawResultPanel (fclCombineInit, 0x228)

138 of 138 words match: 552 emitted and native bytes. The complete owner
checks 183 match, 0 differ. Source commit:
`6b234980219e4cf7cc9a003d0e7daa790456e374`. Exact-source
[CI run 40956924261](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40956924261)
passed build and deployment. The local target/retail, report, tooling and docs gates pass;
the final rebase rechecked three changed guarded owners and carried the
remaining unchanged input closure from the full 149-owner / seven-IRX pass.

Retain the native three-way footer dispatch: mode 1 and default use
`0x5FA5E240`, while mode 2 uses `0x698AEC40`. The identical colors in case 1
and default do not justify collapsing their separate call sites. Initialize
`alpha` to 256 after the footer calls, as native does. These two ordinary C
changes together reproduce the owner; either change alone remains non-matching.

Keep the signed `(0x200 - phase) / 2` calculation, the later recomputation
from `sacrificePanelAnim`, and the existing global-pointer reloads across
calls. No dummy work, opacity, constraints or ABI change is required.

## cmbStartResultScript (fclCombineInit, 0x84)

33 of 33 words match at 132 bytes in ordinary C. The complete owner has
184 matching functions and no differences, including its owned data.
Source commit: `6f214449f849679f552b9d80cf03e49b14f7c88e`.
Exact-source [CI run 40972478153](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40972478153)
passed build and deployment.

Use the actual two-case switch, with case 2 written before case 1. Case 2
selects script entry 6, case 1 selects entry 5, and all other signed-byte
values retain the initialized entry 0. This source order gives retail's
`beql` with the entry-6 assignment in its slot and the correct shared
publication block. The inherited `if`/`else if` form emits 120 bytes;
the switch with case 1 written first emits 140 bytes. No empty cases,
compiler constraints, ABI changes or dummy operations are needed.

Qualified dumps show the case-order effect. With case 2 first, `jump2`
leaves its inequality branch targeted at the shared join and the entry-6
assignment on the fallthrough path. Delay-slot reorganization reverses and
annuls that branch into the native equality test with entry 6 in its slot;
the entry-5 block then falls into common state publication. The opposite
case order leaves both assignments out of line, adding a jump/alignment
and duplicating the state-byte store. For both switch orders, whole-owner
plain/dump assembly agrees apart from the scratch `.file` pathname, and
text, data and relocations agree. This describes observed compiler output,
not the original source spelling.

The signed enable byte still gates all state changes and the script call.
When enabled, the state byte at offset 5 is set to 1 before script setup.
The current global work pointer is reloaded for the script and argument
address, and again after `fclScriptStart` before its actual return value is
stored at offset 8. The declaration and return-value consumer remain
unchanged.

## cmbGetBirthPhase (fclCombineInit, 0x88)

34 of 34 words match at 136 bytes in ordinary C. The complete owner checks
185 match, 0 differ, including owned data. Source commit: `85baa43edce88819d8bde6f068752a63814a2a1c`.
Exact-source [CI run 40974879934](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40974879934)
passed build and deployment.

Use the real switch cases 5, 6 and default, in that order. Each arm performs
its own selected stock-byte lookup from the group's signed 16-bit row plus
unsigned 8-bit cursor column. Mode 5 selects group 0, mode 6 selects group 1,
and every other signed mode selects group 2. Interpret the selected stock
byte as signed 8-bit, select that party entry's unsigned 16-bit unit ID and
tail-call `cmpModelSetup(id, 0)`. The valid cursor and party-index contracts
remain unchanged; no fabricated bounds or new pointer assumptions are used.
There are no intervening callbacks, new stores, constraints or ABI changes.
The same per-arm expression shape appears in matched `cmbDrawStatus`.

The inherited selected-cursor draft emits 120 bytes. Real per-arm row/column
snapshots reach the native 136 bytes but only 12 of 34 words; the actual
switch order reaches 22. Reversing the two cases reaches 14. Narrower truthful
snapshot types and a separate unit-ID local do not improve 22; factoring only
the sum into the arms reaches 15. Putting the complete real signed stock-byte
lookup in each arm produces all 34 words without artificial cases or work.

Qualified compiler dumps retain three stock-byte accesses through `sched2`,
then merge their common suffix in `jump2`. The resulting shared sum uses $2,
the signed stock byte uses $4, and the DDS3 work base loads into $3 at the
native point. Early source-level factoring instead allocates the sum to $3
and the DDS3 base to $6, hoisting the latter. Whole-owner plain/dump assembly
agrees apart from the scratch `.file` pathname; text and relocations agree,
and text agrees with the canonical checker's full object, for both the
snapshot-switch control and exact per-arm form. This is observed compiler
behavior, not proof of the original source spelling.

## cmbUpdateStockCursor2 (facility/fclCombineInit, 0x22C)

139 of 139 words match at 556 bytes. The sole body correction removes the local
cursor-subobject pointer and expresses the same two observations directly
through `CMB_GBWK->stockCursorGroup[2].cursor`. The original unsigned-character
low-byte view of signed16 `unk_22` and unsigned8 conversion of signed8 `unk_24`
are retained. No alias, constraint, new type or fabricated address is added.

The real cursor starts at global+0x30D0; its fields are at +0x30F2 and +0x30F4.
Retail reads the byte at +0x30F4 before the byte at +0x30F2. The draft's local
subobject pointer materializes an extra +0x30D0 address and shifts the prefix
until alignment absorbs the instruction, leaving 117 of 139 words matching.
Direct whole-object fields produce the exact native addresses, observation
order and registers. Target-compiler layout assertions verify these offsets and
the corresponding stock-slot fields. Complete-owner plain and stock-dump
assembly, text and relocations agree for both diagnostic forms. This is paired
output evidence, not a claim about an unmeasured compiler comparator or original
source spelling.

Both observed bytes promote to int, so their sum 0..510 cannot overflow. The
existing target signed-byte narrowing is preserved. The selected byte is
published and reloaded for the stock map, the mapped stock index is published
and reloaded for `cmbStockIsLocked`, and a fresh global is observed after that
callback before the result lookup. Later movement, input and sound calls and
all native callback-dependent reloads remain unchanged. The disabled-result
early return and lock-failure button 0x8000 retain their behavior. Existing
valid cursor, map, stock and result-table index contracts remain required;
arbitrary negative or oversized indices are not claimed safe. Provider types
and call targets are unchanged.

Independent final-source owner checking gives 186 match, 0 differ with normal
owned-data checks, including the earlier result-panel, result-script and
birth-phase matches. Final-source gates pass the retail checksum, all 149
guarded owners / 588 drafts, seven IRX owners, 26 tooling test files / 427 cases,
shell mocks and documentation checks. Source commit `dd0e8255e7b22c6bbb7cffbe2cc6564281a4b28d` passed exact-source
hosted CI [40991191105](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40991191105),
including build and deploy.
