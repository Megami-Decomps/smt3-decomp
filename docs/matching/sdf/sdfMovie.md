# Matching notes: sdf/sdfMovie

## movieWaitSema (sdf/sdfMovie, 212 bytes)

Thirty-nine of fifty-three words differ at retail's exact size, and every one of
them follows from one three-word block in the `obj->ringLength < aSize` arm.
Retail there is

    lw    $2, 0x6C($19)
    daddu $18, $0, $0
    lw    $3, %gp_rel(movieSemaCount)($28)
    daddu $4, $3, $0
    xor   $3, $2, $3
    movn  $4, $2, $3
    sw    $4, %gp_rel(movieSemaCount)($28)

which is a conditional move whose two arms are the field at `0x6C` of
`movieSemaId` and the old value of `movieSemaCount`: semantically just
`movieSemaCount = frame`, with the select left in. ee-gcc 2.96 will not leave it
in. Every spelling that assigns the same value folds to a single
`sw $2, %gp_rel(movieSemaCount)($28)` and comes out five words short, because
combine's `simplify_if_then_else` rewrites `(ne A B) ? A : B` to `A`, and cse
reaches the same result through the explicit `xor` too.

Measured and ruled out, all giving the identical 48-word folded body: the
`if (movieSemaCount != frame) { movieSemaCount = frame; }` store form (which
emits a plain `bnel` plus `sw`, no cmov at all, since a memory destination is
not a cmov candidate in 2.96); the same with a `count` local and one trailing
store; both ternary polarities; `(frame ^ count) != 0` written inline; the xor
in its own `s32` local, tested bare and against `!= 0`; `u32` for the global and
for the local; a `(u32)` cast on the moved value; an explicit `if`/`else` to a
local and to the global; and `count` read before or after the field. A `u64` or
`s64` local does keep the `xor` and the cmov, but pays for it with `lwu` and two
`dsll32`/`dsra32` extension pairs and lands at 54 words.

Mixed arms fold too: with `count = movieSemaCount;` read into a local first,
all four of `(count != frame) ? frame : movieSemaCount`,
`(movieSemaCount != frame) ? frame : count`, `(count ^ frame) ? frame : ...`
and `(count != frame) ? frame : count` emit the same single
`lw $2, 0x6C($19)` plus `sw $2, %gp_rel(movieSemaCount)($28)`, four words where
retail has eight. A local on one side and the global on the other is not enough
to stop it, so the simplification is on the value, not on the spelling.

So the missing fact is below the C: either the arms were not the same value in
retail's source, or that compile did not run the simplification. Do not sweep
cmov spellings again; look for a reading of the arm in which the compared value
and the stored value are genuinely different.

### The draft now kept, and the block shape it establishes

21 of 52 words match, one word short, first difference at +0x44. The arms are
nested rather than an `else if` chain, and `ptr` is uninitialised:

    } else {
        ptr = obj->ringBuffer + obj->ringOffset;
        if (obj->ringLength < aSize) { ptr = NULL; ... } else { ... }
    }

That is retail's shape. The sum lands in `addu $18, $3, $4` ahead of the
`ringLength` test, with `daddu $18, $0, $0` in the short arm, which is why the
prologue, the whole `scratchSize` arm and the epilogue now match; the earlier
`else if` draft with `ptr = NULL` in the prologue matched only 14 of 53 and the
same chain with `ptr` uninitialised only 3 of 50. What is left is the missing
`movn` word and the register pairs that follow it: retail needs `ringOffset` in
`$4` and `ringBuffer` in `$3`, and the draft the other way round, because
retail carries one more value across the arm.

A `sdfIpuDecodeCb_t *node = &movieSemaId->soundNode;` local reaches 33 of 52
differing but reads the frame index as `0x48($3)` where retail has `0x6C($19)`,
so it buys words by getting the addressing wrong.
