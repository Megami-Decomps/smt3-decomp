# Matching notes: sdf/sdf3DPrim

## Default triangle indices

When the caller passes no face index array, the polygon builder copies
`faceCount * sizeof(u32)` from the default table. Each word stores the
three consecutive vertex indices in its low three bytes, with the high
byte zero. The native writable extent has exactly 32 such words,
covering vertex indices 0 through 95 and ending immediately before
`sdf3DPrimIdentityTm`. The typed owner keeps all 32 native rows and the
existing consumer ABI unchanged.

The old split interpreted interior values as unrelated short, string
and address-like data. No native consumer references those interior
labels; the only external table reference is the polygon builder's
base address.
