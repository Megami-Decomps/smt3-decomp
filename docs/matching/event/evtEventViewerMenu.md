# Matching notes: event/evtEventViewerMenu

## func_001F79A8 (event/evtEventViewerMenu, 0xE4)

20 of 56 words differ, exact size. The jump table, its eleven case labels and every
arm's call match; what differs is which saved register the `busy` flag and the
`viewerinfo` load take ($4/$5 swapped against retail), and two consequences of
that: retail materialises the default arm's `pad >= 0` as `slti`/`sltiu` where
the C gives `nor`/`srl`, and retail cross-jumps the two `return 1` blocks where
the C emits one epilogue inline.

Ruled out: `busy = 1` as an initializer, as a statement after the declaration,
and set only in the arms that need it; reading the switch value into a local
first; the 0x10/0x11 arm written as `if (state == 0) return 1; break;` and as
`if (state != 0) break; return 1;`. Six spellings of the default arm's compare
(`>= 0`, `!(< 0)`, `(< 0) == 0`, `(< 0) ^ 1`, `1 - (< 0)`, an `if`/`else`, a
ternary) all compile to the same `nor`/`srl` pair, so the compare is a byproduct
of the allocation rather than the lever. The eleven explicit case labels are
needed: with only the six reachable values the switch falls under
`case_values_threshold` and becomes a compare tree.

## evtFrameListHead (event/evtEventViewerMenu, 0x138)

78 of 78 words match at the native 312-byte size in ordinary C. The real
column-width read belongs after both `sdfDevPrintf` and `sdfDrawTagLink`.
The entry track type remains captured once. Each iteration reads its header
column before the calls, then reloads the same column and its signed width
after them. The count is also reloaded for the next iteration. The exact
`evtFrameListItemSub` sibling already has this post-call width observation.

The former draft initialized `w` before the draw calls. That snapshot does
not express native callback-sensitive read ordering. Moving only its actual
assignment after the calls restores native output without a binding, empty
operand, volatile qualifier, provider change, or new storage view. The
existing ten-byte row type and its eight-column member remain unchanged.
The old advice to keep the width computation before drawing was incorrect
for the native memory contract and is retired.

The current complete-owner baseline emits 308 bytes, with 31 matching and
46 differing words over its 77-word prefix and four native bytes uncovered.
The historical 39/78 score referred to an older source context and is
not the current baseline. A separate genuine column-member pointer control
preserves post-call reloads and native 312-byte size, but matches 48/78; the
simpler indexed form is preferred. No other coordinate, declaration-order,
base-pointer, register, or constraint sweep is needed.

Complete-owner candidate checks preserve all 59 exact peers and owned data.
Production and SKIP_ASM canonical/plain/dump builds of the baseline,
candidate and member-pointer control have matching full allocated-section,
non-file-symbol, relocation and normalized-assembly contracts. All nine
three-mode strict builds are silent. Six existing row-layout assertions are
object/assembly-neutral in both modes, and a wrong row-size assertion fails.
These compiler observations do not establish original source spelling or
prove that the tables are immutable. Full retail and regression qualification
accompanies promotion.

## evtSaveViewerHeader (event/evtEventViewerMenu, 0x400)

256 of 256 words match at 1024 bytes in ordinary C. A block-local signed
`headerCount = 12` is initialized before the header writes. The writes then
follow retail order: byteLength, tag[0..3], count, kind, and the remaining
reserved fields. The real count local dies at its store, before evtWriteRecord.
It changes no calls, types, field widths, values, or arithmetic.

The previous draft matched 254 of 256 words, with count and byteLength stores
exchanged. Merely reordering the direct constant stores changed their register
allocation. Separating the real count initialization from its store preserves
both the desired allocation and store sequence. A diagnostic native-register
binding first established the candidate, but binding-only ablation remained
exact, so the final source uses no register binding or empty asm. This is a
source/code-generation reconstruction, not a claim about historical spelling.

Keep payloadOffset = 0xE0 after the mode != 2 early return, and i declared before
payloadOffset: the existing body, all twelve byte-length products, 21-entry jump
table, fourteen directory arms and payload advance remain unchanged. The native
writer consumes the same initialized 32-byte header and directory records.

Qualification: complete owner 59 match, 0 differ including owned data;
normal and RTL-dump compilation agree in all allocated/relocation sections,
normalized instruction assembly and target bytes. Full retail and regression
qualification accompanies the promotion.
