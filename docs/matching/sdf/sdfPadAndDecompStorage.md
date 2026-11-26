# Matching notes: pad-port buffers and decompression history

- `sdfPadPortBuffers` covers `0x58F0C0..0x58F2C0`. The native initializer
  opens two pad ports in order and advances the buffer address by 256 bytes
  between calls. Each port row is two 0x80-byte instances of the SDK's
  pad DMA layout, not a separate byte array reinterpreted by the provider.
  Both row addresses meet the native 64-byte alignment check. The two
  pad-control records begin at the end of the array. The initializer's
  linked instructions and the full retail image are unchanged; this is
  a backing-type recovery, not a match of the SDK port-open function.
- `sdfDecompHistory` covers `0x594780..0x5947C0`. The native decoder saves
  each command pointer into a four-byte entry and masks the updated ring
  index with 15. The IPU decode stack ends immediately before this ring,
  and the file RPC state begins immediately after it.

- `sdfPadCb` covers `0x58F2C0..0x58F310`. The pad initializer writes two
  typed records at a 40-byte stride; the print RPC client starts at the
  end of the array.

The three definitions own 656 bytes of zero-fill storage. Consumer accesses
are unchanged.
