# Non-matching drafts: event/evtPolygonMovie

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## evtHokanColor (event/evtPolygonMovie, 0xC8)

40 of 49 words match; 196 emitted bytes against retail's 200.

The body is correct: the first forty words, both `EE_MMI_RGBA_UNPACK` blocks
with their frame slots at `0x0` and `0x10`, the `1.0f - t` and `t` scales, the
`COPYVECTOR_A_TO_B` and the `ADDVECTOR` are byte-identical. What differs is one
register at the tail. Retail puts the `128.0f` unit in `$4` and the
`MMI_RGBA_PACK_UNIT` output in `$3`, stores `$3` to the pack slot at `0x20` and
ends `daddu $2, $3, $0`; the draft puts the unit in `$3` and the output in `$2`,
so the copy into the return register does not exist and the function is one word
short. `$2` already holds `a` from the entry `beqz`'s delay slot on both sides,
so the shape of the early exit is not the lever.

Ruled out: an early `return a;` with `return eePackRGBAWord(128.0f);` (10 words
differ), an `if`/`else` setting one `color` in each arm (10 differ and one word
over), a `goto` to a trailing `return color;`, an `s32` rather than `u32` result
local, a second local between the pack and the return, a `f32 unit = 128.0f;`
local (46 differ), dead stores to `t` and to `b` after the pack, and writing
`MMI_RGBA_PACK_UNIT` inline on a `u32 rgba[4]` at function scope (43 of 50) or
in an inner block after the unpacks (unchanged at 9). Every one of those leaves
the output register coalesced with `$2`.

## evtHokanFilter (event/evtPolygonMovie, 0x120)

71 of 72 words match, exact size. The one differing word is the outer corner
loop's backedge: the draft emits `bnel $2, $0` where retail has `bne`.

This one is **not a source problem**. It closes the moment `evtHokanColor` is a
C definition in the unit, measured directly by compiling the `evtHokanColor`
draft unguarded: `evtHokanFilter` then came out byte-exact while
`evtHokanColor` itself stayed 9 words out. So the two are a pair, and cracking
`evtHokanColor`'s last register banks this function with it. Do not permute
this function; see the `evtHokanColor` entry above.

The body is otherwise confirmed: the `corners[2][2]` double loop comes first
(gcc strength-reduces it to one flat `addiu 4` offset reset per outer
iteration, the same shape `evtHokanBlur2` already matches), then
`out->color = evtHokanColor(...)`, then `out->blendControl = a->blendControl;`.
Writing `blendControl` before `color` runs one word over and 30 words out.

## evtGetPolygonMovieWorkData2 (event/evtPolygonMovie, 0xF0)

58 of 60 words match, exact size. Two adjacent words are swapped: retail fills
the `bnez type` delay slot with `daddu $4, $17, $0` (the `pm2file` frame
address, already in a saved register) and emits `addiu $5, $29, 0x84` after it,
while the draft puts the `addiu $5` in the slot and the copy after. Everything
else, including the frame layout (`pm1file` at `0x0`, `pm2file` at `0x40`, the
two data-pointer slots at `0x80` and `0x84`) and all four stores at the end, is
identical.

Ruled out: swapping the `type` arms (36 words out, the fallthrough has to be
the `sdfDevLoadFileEx` arm), `&pm2file[0]` for `pm2file`, a `void **out`
local for `&pm2data`, declaring the two handles before the data pointers, and
nesting the tail inside the `pm1data != NULL` guard instead of an early
`return NULL`.

Confirmed on the way: the result the function returns is `InitPolygonMovieWorkData`'s
pointer, not `evtGetPolygonMovieWorkData1`'s, and the `work == NULL` test sits
*after* that call, so `evtFreePolygonMovieWorkData(NULL)` really is reachable.

## CheckLoad_evtPolygonMovie (event/evtPolygonMovie, 0x110)

36 of 68 words match, exact size. The field reads, the call order and both
`flags` masks are right; what differs is where jump2 lands the cross-jump of
the two load arms' tails.

Retail's shared block is `and $2, $2, $3` / `b` / `sw $2, 0($16)` placed after
the *second* arm, so that arm falls into it and the first arm reaches it with a
`b` carrying `addiu $3, $0, -3` in the delay slot. The draft instead keeps an
`and` in each arm, sinks it into each arm's own branch delay slot, and shares
only the `sw`. Everything from there is shifted by two words, which also costs
the `beql` annul bits on the `flags & 2` test and puts the `work->file04`
guard's load in `$2` where retail loads it straight into `$4`.

Ruled out: `work->movieFlags = work->movieFlags & ~2;` for the compound
assignment, clearing the flag before the `work->file04 = NULL;` store (34
words out), a `u32 flag` local for the three mask tests, caching
`work->file04` in a local and testing that, an early `return` for the
`flags & 8` guard, and nested `if`/`else` in place of the `else if` chain.
All four of those leave the cross-jump exactly where it is.
