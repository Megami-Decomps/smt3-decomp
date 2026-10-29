# Non-matching drafts: newbattle/nbPanelProcess

Conventions: [nonmatching drafts](../../nonmatching.md).

## nbPanelDrawProcess (0x108)

65 of 66 instruction words match at the native 264-byte size. Assembly remains
active; this reconstruction carries no exact-match credit.

The one differing word is at +0xE4: the draft emits `bnel` where retail has
`bne` on the test after nbPanelStockCheckRun. Compiling the same body with
nbPanelPartyDraw present as a C definition in this unit gives a full match, so
the branch form follows the visibility of the callee in the not-taken arm and
not the source spelling. Reordering the test into an early return, and both
spellings of the comparison, were ruled out.

## nbPanelAuto (0x3A0)

226 of 232 instruction words match at the native 928-byte size. Assembly
remains active; this reconstruction carries no exact-match credit.

The two packed colors are distinct: PanelAutoABGR supplies the animation
color, while PanelAutoBaseColor fades the common panel color. A hidden panel
with zero base alpha returns before changing that common color. Each texture
handle is captured before its color callback. Both sine phases read the
unsigned battle timer independently, and both use 0.2f (native pool bits
0x3E4CCCCC). Keep the float-to-integer conversions and the packed RGB additions.

The two polygon rows start at D_004F0600 and D_004F0614 and are submitted in
reverse order. The three UV rows start at D_004EFE60, D_004EFE70 and
D_004EFE80. The curated base symbols separate these rows from preceding
objects without replacing their data. The source uses complete array bases;
no pointer indexes before an unrelated data owner. Separate drawing-stage
scopes keep the geometry and texture lifetimes bounded to their actual uses.

All remaining differences are in the first sprite of the normal branch:
retail loads width before height, while the draft loads height before width;
the draft also loads UV height before its other three UV components. The
values and call arguments agree. Offsets +0x2D8/+0x2E0 exchange the dimension
loads; +0x2E8 through +0x2F4 rotate the four UV loads.

A qualified no-dumps scheduler trace passes hooked/unhooked/plain assembly
parity. In sched2, width UID 567 and height UID 569 both have priority 8;
height wins the original-order tie (LUID 171 versus 177). UV-height UID 577
and UV-u UID 571 likewise tie at priority 8 and height wins the LUID tie
(173 versus 179). A named width local leaves the six words unchanged.
Caching all six dimension/UV inputs swaps the geometry/UV saved registers
and worsens the result to sixteen differing words; that form was retired.

Reproduce the trace with `tools/cc1probe.sh
src/smt3/newbattle/nbPanelProcess.c --func nbPanelAuto --cflag -DNON_MATCHING
--sched --no-dumps --out <output>`. Preserve `verify: OK` before relying on
its instruction IDs; declaration or source changes can renumber them.
