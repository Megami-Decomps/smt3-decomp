# Matching notes: newbattle/nbEncount

## nbGetEncountJoken (newbattle/nbEncount, 0x174)

24 of 66 words match, at 264 bytes against retail's 372, and every field read
and every branch retail takes is present in the draft.

What is left is how gcc materialises `&nbEncountTable[id]`. Retail keeps only
`id`, `id * 8` and `%hi(nbEncountTable)` in saved registers, then rebuilds the
whole `id * 420 + nbEncountTable` address in each of the six blocks that read a
field, reusing the first block's pointer register for the first event-bit
result once that pointer is dead. The draft keeps the finished pointer in a
saved register across the two `evtBitCheck` calls and reads every field off it,
so the six rebuilds collapse into one and the function comes out 27
instructions shorter.

Confirmed on the way: `nbEncountTable`'s stride is 420 bytes; the moon mask is
a `u16` at `+0x6` shifted right by `evtGetAgeOfMoon()` and tested against bit
0, with the result read as a `u16` at `+0x8`; the two event-bit ids are `u16`s
at `+0xA` and `+0xC`, each only passed to `evtBitCheck` when non-zero; and the
four results are `u8`s at `+0xE` through `+0x11`, selected as
`0xE + (bitA != 0) + 2 * (bitB != 0)` but laid out as a branch tree, not an
index.

Ruled out: the plain subscript at every use, and the same with a
`nbEncountEntry_t *` hoisted for the first block only (both 264 bytes); the
moon test as an `&&` chain rather than nested `if`s; the result tree written
with the `a == 0` arm first and with explicit `else` arms; a sized
`extern nbEncountEntry_t nbEncountTable[512]`; `(nbEncountTable + id)->field`
and an integer-cast `((nbEncountEntry_t *)((s32)nbEncountTable + id * 420))`
macro, which both lose the first block as well (236 bytes); unprototyped
`extern s32 evtBitCheck()` and `evtGetAgeOfMoon()`; extra locals for the moon
phase and for the two bit ids; evaluating the second event bit before the
first; moving the four results into a nested sub-struct, which hoists that
sub-struct's address instead (236 bytes); and reaching the same rows through a
second 420-byte `extern` of a selector-only type (268 bytes).
