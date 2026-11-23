# Matching notes: pad-port buffers and decompression history

- `sdfPadPortBuffers` covers `0x58F0C0..0x58F2C0`. The native initializer
  opens two pad ports in order and advances the buffer address by 256 bytes
  between calls. The two pad-control records begin at the end of this array.
- `sdfDecompHistory` covers `0x594780..0x5947C0`. The native decoder saves
  each command pointer into a four-byte entry and masks the updated ring
  index with 15. The IPU decode stack ends immediately before this ring,
  and the file RPC state begins immediately after it.

- `sdfPadCb` covers `0x58F2C0..0x58F310`. The pad initializer writes two
  typed records at a 40-byte stride; the print RPC client starts at the
  end of the array.

The three definitions own 656 bytes of zero-fill storage. Consumer accesses
are unchanged.
