# Matching notes: field shadow indices

## Shadow quad and triangle indices (field/fldTest, 128 bytes)

`fldSdwQuadIndices` at `0x002FBD80` and `fldSdwTriIndices` at
`0x002FBDC0` are two writable 64-byte objects. The quad table splits
eight groups of four vertices into two faces each. The triangle table
indexes sixteen independent triangles. Every face uses three index
bytes and a zero fourth byte. The existing union preserves the eight
paired 64-bit reads and the sixteen packed 32-bit indices on the target.

The release build keeps the original assembly body of `fldSdwDraw`.
Both table references still resolve to their retail addresses, and the
complete linked executable is unchanged. The game and full reports
both gain 128 matched-data bytes and no function or code bytes.
