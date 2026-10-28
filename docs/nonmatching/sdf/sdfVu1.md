# Non-matching drafts: sdfVu1

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## corePktLoadVerts (sdfVu1, 0x2C)

**Reference implementation, not a match candidate.** The retail body is
assembly, so the C here records what the routine does rather than how close
anything is to matching. Do not measure it against retail word counts.

Three words per vertex, read from a 4-aligned source that is not 8-aligned,
which is why retail takes the first eight bytes with an `ldr`/`ldl` pair
instead of an `ld`. `pcpyld $2, $3, $2` then builds the quadword with those two
words in the low half and the `lw` of the third word, sign-extended, in the
high half, so the fourth word of every destination quadword is 0 or -1 with the
sign of the third. The source advances 12 bytes per vertex and the destination
0x60, so each vertex lands in the first quadword of a 96-byte record.

Callers pass dst in `$4`, the vertex count in `$5` and src in `$6`
(`sdfCorePacketGen`).

For the loop itself retail uses `addi` for both pointer bumps and the
countdown, with the destination bump in the `bne` delay slot. See
[../../levers/impossible.md](../../levers/impossible.md) for why that marks the
body as assembly and which of these routines are worth attempting as matches.


## The corePkt VU1 packet builders written in assembly

Fifteen more routines in this unit are hand-written assembly, marked by the
`addi` that cc1 never selects. Each carries a C body behind
`#ifdef NON_MATCHING`: **reference implementations, not match candidates.** Do
not measure any of them against retail word counts. Why they are assembly and
what it would take to match one: [../../levers/impossible.md](../../levers/impossible.md)
and [../../levers/asm-macros.md](../../levers/asm-macros.md).

They all fill the same 96-byte `sdfCorePacketPointBuff_tag` record, one per
vertex: the transformed position at +0x00, the normal with the diffuse term in
its W lane at +0x10, the colour at +0x20, the UV pair at +0x30, and two clip
bytes at +0x50 (`region`) and +0x51 (`clipFlags`). Vertex, normal and morph
streams are three 4-byte words per entry, 4-aligned but not 8-aligned, which
is why retail reads them with an `ldr`/`ldl` pair plus an `lw` rather than an
`ld`; `pcpyld` then leaves that `lw`'s sign extension in the fourth word, so
word 3 of every quadword written from such a stream is 0 or -1 following the
sign of word 2. Argument roles come from `sdfCorePacketGen` and from this
unit's own callers, not from guesses.

## corePktLoadVertsNorms (sdfVu1, 0x44)

**Reference implementation, not a match candidate.** Two streams into one
record: the vertex at +0x00 and the normal at +0x10, both with the `pcpyld`
sign word in lane 3.

## corePktMorphVerts (sdfVu1, 0x44)

**Reference implementation, not a match candidate.** `position += weight *
delta`, but `vmaddx` writes only xyz while `sqc2` stores the whole quadword,
so lane 3 of the position ends up as the sign extension of the morph delta's
third word, not as anything the add produced. The weight arrives in `$f12`.

## corePktMorphVertsNorms (sdfVu1, 0x6C)

**Reference implementation, not a match candidate.** The same for the normal
at +0x10, with both delta streams advancing 12 bytes per vertex.

## corePktNormalizeNorms (sdfVu1, 0x34)

**Reference implementation, not a match candidate.** `vrsqrt`/`vwaitq` on the
normal at +0x10. `vmaddz.x $vf4, $vf1, $vf3z` only sums to x*x + y*y + z*z
because the engine keeps `vf1.x` at 1.0, which is register state this unit
never sets. `vmulq` is xyz only, so lane 3 survives untouched.

## corePktSkinVertFirst (sdfVu1, 0x58)

**Reference implementation, not a match candidate.** **Retail has a register
slip here.** The loop branches on `$4`, the destination pointer, instead of
`$5`, the count, and the store is `sqc2 $vf4, -0x60($7)`, based on the weight
pointer instead of the destination. `corePktSkinVertAccum` directly below it
does both correctly, so the C records the intended operation. The weight
stream is four words per entry, read as two `ldr`/`ldl` pairs: xyz is the
vertex in bone space and w the bone weight.

## corePktSkinVertAccum (sdfVu1, 0x60)

**Reference implementation, not a match candidate.** The same 4x4 transform,
weighted, accumulated into the position already in the record.

## corePktSkinPointsFirst (sdfVu1, 0x84)

**Reference implementation, not a match candidate.** The arithmetic is not in
this body at all: it loads seven quadwords of matrix state (the 4x4 at +0x00
into vf28-vf31 and three light basis rows at +0x40 into vf24-vf26), feeds the
weight quadword and the normal per vertex, and runs `vcallms 0`, taking the
position out of vf4 and the lighting vector out of vf7. The C is the operation
inferred from that setup and from `corePktSkinVertFirst`, which does the
position half inline; it is inference, not a reading of the microprogram. The
loop is software pipelined, so the inputs for iteration i+1 are fetched inside
iteration i. Here the weight stream is 16-aligned (`lq`, not `ldr`/`ldl`)
while the normal stream is still three words.

## corePktSkinPointsAccum (sdfVu1, 0xC4)

**Reference implementation, not a match candidate.** As
`corePktSkinPointsFirst` with `vcallms 0x48`, which also takes the two
destination quadwords in vf5 and vf6 so the bone's contribution accumulates.
Because the pipelining fetches one iteration ahead, retail decrements the
count once before the loop and repeats the whole body once after it.

## corePktXfmUV (sdfVu1, 0x60)

**Reference implementation, not a match candidate.** A 2x2 plus offset on the
xy lanes, from three 8-byte rows at the transform pointer. Rotated: the result
for entry i is read back with `qmfc2` and stored into the previous record
while entry i+1 is already in the pipe.

## corePktXfmLightClip (sdfVu1, 0xD0)

**Reference implementation, not a match candidate.** Two of its inputs are VU0
register state no C expression reaches, so the draft names them notionally:
vf28-vf31 hold the screen transform `MultMatrixRev` left behind, and
vf24-vf26 the world light basis from `corePktLoadWorldLightMtx`, whose W lanes
this routine rescales by `corePktFaceDivParm.scale` on entry.

`vf6` keeps the normal's xyz and only its W lane is overwritten with
`2 * diffuse - 1`, which is what makes the record's +0x10 quadword "the normal
with the diffuse term in W". `clipFlags` is `cfc2 $vi18` after the position's
`vclipw`; because that register is a 24-bit shift register taking six bits per
clip, the byte stored also carries the low two bits of the preceding clip in
its top two bits. `region` is the over/under pair from a second `vclipw` of
`2 * diffuse - 1` against 1.0. `vclipw` is not in the assembler's table:
retail emits it as a `.word`.

## corePktXfmLightClipPB (sdfVu1, 0xA8)

**Reference implementation, not a match candidate.** `corePktXfmLightClip`
with the position and the normal read back out of the record at +0x00 and
+0x10 instead of out of two source streams.

## corePktXfmLightClipUV (sdfVu1, 0xFC)

**Reference implementation, not a match candidate.**
`corePktXfmLightClip` plus `corePktXfmUV`'s 2x2-and-offset on an 8-byte UV
stream, stored at +0x30. The store is a whole quadword from a register whose
zw lanes the VU never wrote, so +0x38 carries over whatever was in it.

## corePktXfmLightClipUVPB (sdfVu1, 0xD4)

**Reference implementation, not a match candidate.** The in-place form of
`corePktXfmLightClipUV`.

## corePktRelightBackFaces (sdfVu1, 0x8C)

**Reference implementation, not a match candidate.** Negates the normal's xyz
at +0x10 and redoes the shade and the `region` bits from it. The position and
its clip flags are not touched, so this runs over a point buffer
`corePktXfmLightClip` has already filled.

## corePktApplyFaceDivColor (sdfVu1, 0x98)

**Reference implementation, not a match candidate.** 0.5 is broadcast into
`vf12` through a GPR, which is what maps the +0x10 quadword's
`2 * diffuse - 1` back to [0,1] before the colour interpolation against the
parameter block's `colorBase` and `colorDelta`. The colour loop is rotated by
one record like `corePktXfmUV`'s. A second loop then walks `extPointTop` and
copies each interpolated point's colour quadword from its `sourceA`, one
`lq`/`sq`.
