# Matching notes: fclRagCalc

## ragDrawList2, ragDrawList3 (fclRagCalc, 0x150 each)

Both functions match all 84 words (336 bytes) under the canonical void
`fclDrawPanel` contract. The old raw-offset drafts retained six differing
words in the first acquisition block after that declaration correction.

Use the existing `ragDrawWork_t` members for that acquisition: `list.window2`
and `list.window3` supply the signed scroll and unsigned cursor, `list.list2`
and `list.list3` supply the signed item arrays, and `panel[2]` is the panel.
The established offsets are respectively 0x2492/0x2494/0x3B and
0x362A/0x362C/0x44; the panel is at 0x4700. These typed accesses produce
retail's initial work-pointer and index allocation without artificial uses
or altered declarations. Later global reloads remain across calls, preserving
their possible effects on `ragDrawWork`.

The complete owner checks as 7 match, 0 differ. Plain and `-da` compiler
assembly agree after normalizing only the randomized `.file` scratch path.

## ragDrawHeartsList (fclRagCalc, 0x1D0)

116 of 116 words match: 464 emitted and native bytes. The complete owner
checks 8 match, 0 differ. Source commit: `74afe280f12b140eadabe46b6b9609933d739447`.
Exact-source [CI run 40955287970](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40955287970)
passed build and deployment. Local retail/provenance,
no-op build, 149 guarded owners (610 drafts), seven IRX modules, `test_cc`,
documentation checks and 26 tool suites (427 cases) pass.

Select the signed item directly via `heartItems[top + cursor]`, independently
of the retained rows. Preserve the entry snapshots of rows, index and price.

The caption was already fixed, not row-dependent. Its separate raw-byte
caption-pointer view permits native address rematerialization instead of
retaining the price-table base. The native table at `0x0032FD28` has extent
`0x8C0`; its caption pointer slot at `0x003305E0` (offset `0x8B8`) points to
`0x0053D260`, the string `ITEM EXCHANGE`. Later panel accesses remain direct
global reads across calls, preserving possible callback changes. This is
ordinary C with no ABI change, constraints or dummy uses.
