# Non-matching drafts: pb/pbGameDraw

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## pbRebuildOutline (pb/pbGameDraw, 0x188)

16 of 95 words match, at 380 emitted bytes against retail's 392.

The function walks `g->blk[i]` for `i < g->num`, clears `outline`, takes
`pbGetBlockAt(g->cellX + blk->x, g->cellY + blk->y, g->grpID)` and, for each of
the four bits of that mask, appends two vertices from the 4x2 template at
`D_00327780`, adding `blk->x * 40` to x and `blk->y * 40` to z. All of that
reproduces; `blk->x * 40` even comes out as retail's `(x << 2 + x) << 3`.

Two things do not. Retail keeps the whole `D_00327780` address live in `$20`
across the outer loop (`lui` plus `addiu %lo` in the preheader, then a plain
`daddu $9, $20, $0` per iteration); ee-gcc hoists only the `lui` and sinks the
`addiu %lo` into the loop, which is the two words of the size difference and
also costs retail's load-delay `nop` before the loop label. And retail computes
`n * 16 + &g->blk[i]` once and reaches all six vertex words from it at
displacements 0x10 through 0x28, through a chain of five `daddu` copies of that
one address; every spelling tried either folds the 0x10 into the pointer or
splits the index into two or three separate sums.

Ruled out: `&g->blk[i].vtx[n]` as a pointer with `v[0]`/`v[1]` (96 words, 80
differ, but gcc folds the 0x10 displacement into the address and keeps the
outer induction variable as `i * 9` scaled by 16 instead of retail's `i * 144`);
a `b = &g->blk[i]` pointer local with `b->vtx[n]` and `b->vtx[n + 1]` (98 words
but 84 differ, gcc builds six separate addresses); and `g->blk[i].vtx[n]`
written out in full at every store, which overruns the function. The kept draft
uses `(sdfVector4_t *)(n * 16 + (s32)&g->blk[i])` with `v[1]` and `v[2]`, which
is the spelling that gets retail's index-first `addu` and the 0x10 through 0x28
displacements.
