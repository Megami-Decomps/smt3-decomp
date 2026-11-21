# Matching notes: SDK debug handler table

The writable table at 0x52D628 holds sixteen initially null opaque
handler addresses. The existing SetDebugHandler bounds check admits
indices 1 through 13. Its native load and store use the same four-byte
stride and exchange the caller's address without establishing a C
callback prototype. Keep the existing `void *[16]` declaration.

Plain initialization emits 64 bytes in writable .data with the native
eight-byte alignment in both stock modes. A trial explicit section
attribute did not change output and is omitted. The raw objects before
and after the table retain every byte and relocation. In particular,
`execArgumentBlock` still starts at the original 0x52D668.
