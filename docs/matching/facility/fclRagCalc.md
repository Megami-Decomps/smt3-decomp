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

## ragDrawHeartsRows (fclRagCalc, 0x21C)

135 of 135 words match at 540 bytes. The ordinary conditional and named-value
controls already produce 540 bytes, differing only at target offsets 0xEC and
0xF0: the initialized true value occupies $3 rather than native $4, followed
by its real conditional-move consumer. The final source uses the localized
binding `register s32 unavailable __asm__("$4") = 1;`. Removing the initially
tested empty asm preserves the match, so none remains. A generic operand
constraint instead produces 548 bytes and is rejected.

The binding starts after the real cmbChkGetHearts query returns and supplies
the actual selected disabled value. It is consumed before $4 becomes the
cursor argument and before the next call. No value is fixed across a call,
and there is no dummy consumer, false prototype, instruction replay or
artificial lifetime extension. This is a narrow target allocation aid under
the SMT3 localized-constraint policy, not evidence of original source spelling.

ragFindItemSlot's zero test initializes disabled. datGetItemSlotIndex truthfully
takes u16 and returns s32; an ineligible item returns zero and bypasses the
hearts query. cmbChkGetHearts retains its s32 argument and result, including
the provider's internal s8 conversion. Every nonzero query result selects one;
zero retains disabled. This preserves the complete signed result domain,
not merely boolean results. The real selection reaches item-box mode bit zero,
with mode != 0 supplying bit one.

The row loop retains signed item-table indexing and unsigned requirement
indexing, the u16 item load, and that item's lifetime across the queries.
Positions remain snapshots taken before the window callback; row count and
StepY reload after the sprite callback. Font calls, sprite arguments, footer,
row reset and optional cursor tail call remain unchanged. Existing valid
list, row, table and string contracts and target signed arithmetic are
inherited; no new bound or malformed-input guarantee is asserted.

Qualified complete-owner ordinary/preferred builds have plain/stock-`-da`
assembly, text and relocation parity. The final greg dump has the query
return at UID156, real initialized one in hardreg4 at UID161, and disabled
selection at UID356; ordinary uses hardreg3. The 4064-byte owner text differs
only at object offsets 0x774/0x778 (target 0xEC/0xF0), with equal relocations.
Independent final guard-removed compilation reproduces the preferred text
and relocations. These are measured output and stock-dump observations, not
an unobserved optimizer-causation claim.

The complete owner checks 9 match, 0 differ, retaining all eight prior peers
and owned data. Full qualification passes the retail checksum, 149 guarded
owners / 581 drafts, seven IRX owners, 26 tooling files / 427 tests, shell
mocks and documentation checks. Actual published-checkout checks confirm all
779 tracked files, 30 owner dependencies, source identity and no-op builds.
Source commit `1dd32aaa03b376c388af52c4dbbeba557d3d1423` passed exact-source hosted CI
[40997533214](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40997533214),
including build and deploy.
