# Camp 3D view packets

## Double-buffered view and light storage

The camp view builder calls the packet providers for two frame buffers.
Each uses a 32-byte view tag and init packet, a 32-byte light tag, a
224-byte light packet, and a 208-byte viewport packet. The native
1,056-byte zero-fill range spans `0x574A60..0x574E80`, ending at the
separate battle result storage. The definitions keep the original packet
types and alignment without changing the setup or link instructions.
