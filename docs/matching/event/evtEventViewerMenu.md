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

39 of 78 words differ, exact size. The loop, both `D_0032C9E2` reads, the shared
`type * 10` offset and all four table lookups match. The one difference is which
loop invariant gets the ninth saved register: retail hoists the `lui`/`addiu`
pair for `evtFrameList`'s address and rematerialises `D_0054F9B8`'s address
inside the loop, while the C hoists `%hi(D_0054F9B8)` and rematerialises
`evtFrameList`. Both use nine saved registers, so this is gcc's choice of
movable rather than register pressure; the three remaining symbol addresses
(`D_0032C9E2`, `frameheadstr`, `framelist_xcnt`) stay in the loop on both sides.

Computing the column width into a local before the draw is what brings the size
to exact (it was one instruction long without it), so keep that statement order.
Ruled out: `s16 *table = evtFrameList;` as a local, declared first or second
(gcc then hoists both and the body is two words short); `*(s16 *)((u8 *)evtFrameList
+ type * 10)` for the bound; reading `viewerinfo->frameGroup->type` inline at
each use instead of a local (74 of 78); caching the `D_0032C9E2` byte in a local
re-read twice.

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
