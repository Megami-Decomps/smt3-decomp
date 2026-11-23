# Battle light

## Formation point-light storage

The light initializer fills 32 records of 0x48 bytes with 0xFF starting at
`0x569C20`. It walks the corresponding 32 formation records and
fills the initial state in a separate array of 0x20-byte records.
That array starts at `0x56A520` and ends at the untouched player
sphere labels at `0x56A920`. Each base record starts with a full
16-byte VU vector.

The separate definitions keep the existing types, addresses, vector
alignment, and ordinary external references in the consumer. The
3,328 bytes are zero-fill storage; no light operations, initialized
bytes or instructions are added.
