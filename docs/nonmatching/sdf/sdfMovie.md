# Non-matching drafts: sdf/sdfMovie

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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

So the missing fact is below the C: either the arms were not the same value in
retail's source, or that compile did not run the simplification. Do not sweep
cmov spellings again; look for a reading of the arm in which the compared value
and the stored value are genuinely different.

The draft kept here is the natural shape at retail's exact size. Two other
points on the curve were measured: the same body with `u8 *ptr;` uninitialised
is 47 of 50 words (three short, and the `ptr = NULL` then lands in the arm
rather than the prologue), and a `sdfIpuDecodeCb_t *node = &movieSemaId->soundNode;`
local reaches 33 of 52 but reads the frame index as `0x48($3)` where retail has
`0x6C($19)`, so it buys words by getting the addressing wrong. The rest of the
difference is the prologue's saved-register order and `ptr` taking `$19` where
retail gives it `$18` and keeps `movieSemaId` in `$19`; those follow from the
block above, since retail needs one more value live across it.

## release_movieobj2 (sdf/sdfMovie, 596 bytes)

With the shared SDK bit-length scratch contract, **0 of 148 words match**, 592 bytes against native 596. The prior allocator-selected-scratch
measurement had one matching word among 150; it is superseded. The guarded body still uses
the old 64-bit TEX0 temporaries and raw packet draft. The notes below describe
that historical reconstruction, not a proof that the corrected shared primitive
or ordinary packet source cannot match.

The retail body is ordinary C apart from two `plzcw`/`addi` bit-length idioms,
which `PS2_WORD_BIT_LENGTH` in `include/ps2asm.h` reproduces exactly and in
place. What it builds is a 0xB0 packet that draws the IPU output as one GS
sprite: a DMAtag and a VIF `DIRECT` for ten quadwords, a GIFtag for a textured
`FST` sprite with nine A+D writes, then TEX1, TEX0, CLAMP, TEXA, RGBAQ and two
UV/XYZ2 pairs. TEX0's `TW` and `TH` are the bit length of
`decodeCb.width - 1` and `decodeCb.height - 1`, `TBW` is `width / 64` forced
up to 1 with a `movz`, `TBP0` is `decodeCb.resourceWord >> 6`, `TCC` is set,
and `PSM` becomes `PSMCT16` only when `decodeCb.rgbMode` is non-zero. The two
UV writes are `(0, 0)` and `(width, height)` in 10.4 fixed point, and the two
XYZ2 writes are the rectangle corners with `obj->z` in the Z field.

What is left to close: retail saves exactly three registers, `$16` for the
movie object (reused for the draw tag once the object is dead), `$17` for the
packet and `$18` for the ordering table, and nothing else lives across a call.
This draft needs six, because cc1 hoists the constant 1 that the `TBW` `movz`
needs, and two more values, into callee-saved registers ahead of
`sdfGrAllocPacketBuff`. Keeping everything but those three off the
cross-call live ranges is the next thing to try. The TEX0 OR chain's term
order is also unverified; `sdfTex.md` records how to recover one.
