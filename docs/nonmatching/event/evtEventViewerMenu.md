# Non-matching drafts: event/evtEventViewerMenu

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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

254 of 256 words match, exact size. Every count call, the twelve byteLength products,
the 21-entry jump table, all fourteen directory arms and the `mtlo`/`madd`/`mflo`
payload advance match. What is left is two stores in the file header swapped:
retail writes `byteLength` (0x04) before `count` (0x10), the C writes them the
other way round, with the same registers holding the same values on both sides.

Two placements are load-bearing and should be kept: `payloadOffset = 0xE0` has to
be a statement after the `if (mode != 2) return;`, not an initializer (as an
initializer the function is two words long), and `i` has to be declared before
`payloadOffset` so the two take retail's spill slots at 0x34 and 0x38 (declared
the other way every later slot shifts by four and 54 words differ).

A sweep of all 210 orderings of the header's seven value stores (byteLength, the
four tag bytes in order, count, kind) bottoms out at these two words: the order
count, tag[0..3], byteLength, kind gives retail's constant-to-register mapping
and 2 differing words, while retail's emitted store order (byteLength, tag[0..3],
count, kind) gives the right stores and rotates all six constant registers
instead. No ordering gives both, so the remaining difference is the scheduler
interleaving the constant materialisations with the store run. Also ruled out:
`header.tag[3] = mode + '0'` (13 words; cse does not fold it against the early
return), `count` through a local assigned beside `payloadOffset` (an extra local
moves every spill slot, 235 words), moving the six zero stores to the front or
the back of the block, and computing `byteLength` in one expression or deferring
its last term.
