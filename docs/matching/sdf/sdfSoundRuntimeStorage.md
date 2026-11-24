# Matching notes: Sound runtime storage

## Sound command and listener state

The sound command worker's existing 48-byte thread descriptor starts at
`0x5907C0`. Its initializer writes the entry, stack, stack size, priority and
GP before passing the base to `CreateThread`. The previous-listener packet
starts at `0x5907F0`. The listener update compares six scaled coordinates at
offsets eight through 28 and copies the full 32-byte packet after sending an
update. The following raw bytes through `0x590840` are not part of that packet.

## Sound transfer descriptor

The SIF scratch descriptor at `0x592840` holds one four-word transfer. The
sound caller and SIF provider now use their shared DMA descriptor type. The
caller still fills the same words, flushes the cache, submits one descriptor
and retries exactly as before. The 48 bytes before the SIF RPC stack stay raw.

## PCM request state

The PCM request at `0x593E20` retains the existing 256-byte prefix and the
size, pending state and data address at offsets `0x100`, `0x104` and `0x108`.
The PCM setup function writes those words, and the file-RPC service reads
the same data address. The sound map, following raw bytes and file-RPC
queue stay separate.

Explicit section placement is needed to keep the records out of COMMON.
Their native addresses need no extra alignment attribute. These four objects are zero-fill: no retail code or initialized data changes.
