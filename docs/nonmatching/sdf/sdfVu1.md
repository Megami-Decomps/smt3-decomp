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
