# sdk/libpad

## scePadPortOpen

120 words against retail's 122. The same saved-register swap, plus the
double-buffer initialisation loop: retail keeps the buffer pointer and the
counter in registers the draft spends differently, so the loop body is a
register short and reloads.

The earlier `_send_to_iop` residual is resolved. Snapshot the DMA destination
before publishing the sequence, flush the command buffer, then construct the
descriptor in native field order. Under EE 2.9 that lifetime and ordering give
the native saved-register allocation and all 276 instruction bytes. The
diagnostic remains an external symbol because its data owner includes other
strings and padding.

## EE runtime storage

The two RPC clients at `0x598980` are 0x28 bytes each. The bind calls use
both records, and initialization checks the server pointer in each. The
client array and the RPC provider share the complete transaction layout. The
port state at `0x5989D0` contains two ports with four 0x1C-byte slot records
apiece. This accounts for 0xE0 bytes; the 0x10-byte gap before the command
blocks stays in raw zero-fill.

The eight 0x20-byte command blocks start at `0x598AC0`. The DMA submission
uses the selected block as its source and transfers exactly 0x20 bytes.
The separate RPC packet at `0x598BC0` is passed to the RPC service as a
0x80-byte send and receive buffer. The voltage-reference payload uses its
actual union member rather than casting the byte array. These definitions
preserve the existing consumer layouts and do not change the production instruction or
initialized-data count.
