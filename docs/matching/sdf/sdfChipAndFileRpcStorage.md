# Matching notes: chip allocator and file RPC state

- `sdfChipCB` holds seven 12-byte allocation-class records at
  `0x577EC0..0x577F14`. The chip allocator indexes those records with a
  12-byte stride. Four neighboring raw bytes and the next allocator
  control block stay separate.
- `sdfFileRpcState` holds six native words at `0x5947C0..0x5947D8`.
  The existing file-RPC implementation shares the definition's type;
  its read and completion paths retain the original accesses. This
  definition starts at the end of the decompression history; the
  following raw interval and receive buffer remain separate.

Both definitions are zero-fill storage and do not change the retail image.

## File RPC read staging buffer

The continue-read path limits each read into `sdfFileRpcReadBuf` to
`0x4000` bytes. This 16,384-byte buffer occupies
`0x594940..0x598940`; the DMA environment starts at its end. The
separate RPC receive buffer and packet storage before it are unchanged.
The interior generated numeric labels have no separate references.
