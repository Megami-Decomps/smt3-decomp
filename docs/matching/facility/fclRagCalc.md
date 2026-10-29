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
