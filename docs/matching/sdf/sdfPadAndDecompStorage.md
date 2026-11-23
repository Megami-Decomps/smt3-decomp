# Matching notes: pad-port buffers and decompression history

- `sdfPadPortBuffers` covers `0x58F0C0..0x58F2C0`. The native initializer
  opens two pad ports in order and advances the buffer address by 256 bytes
  between calls. The neighboring pad-control records remain in raw storage.
- `sdfDecompHistory` covers `0x594780..0x5947C0`. The native decoder saves
  each command pointer into a four-byte entry and masks the updated ring
  index with 15. The IPU decode stack ends immediately before this ring,
  and the file RPC state begins immediately after it.

The new definitions own 576 bytes of zero-fill storage. Consumers are unchanged.
