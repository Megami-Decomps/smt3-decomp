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

## Shadow mesh vertices and faces (field/fldShadowMeshes, 5088 bytes)

Sixteen field shadow meshes supply 168 XYZ vertices and 256 triangles
through 32 writable arrays. Each vertex is three single-precision
coordinates; each face is three signed 32-bit indices into its mesh.
All array extents and vertex references are checked against the retail
data and its existing `fldSdwDraw` consumer.

The pinned compiler rounds some nearest-round-trip decimal spellings
one ULP toward zero. In the control build this affected 215 coordinates.
The chosen spellings lie on the outward side of the exact binary value
while still rounding to that same single-precision value under nearest
rounding. Their emitted words match retail without bit casts or code
generation constraints.

The data-only build partition at `0x002FA950` makes no claim about the
original translation-unit boundary. The linker includes only its
`.data`; Objdiff compares it against Splat's generated assembly for
that native data range, not a synthetic text reference. The generator
uses this reference only for a unique, exact data-only linker input and
requires the native reference to exist.

The 5104-byte range includes four native four-byte interarray gaps.
The preceding 12 raw bytes, following 64 raw elevator bytes, existing
shadow index tables, and assembly draw fallback are unchanged. The
retail image is byte-identical. The game and full reports credit 5104
matched-data bytes and no code or functions.
