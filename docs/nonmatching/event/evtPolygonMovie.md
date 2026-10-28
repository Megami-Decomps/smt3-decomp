# Non-matching drafts: event/evtPolygonMovie

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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
