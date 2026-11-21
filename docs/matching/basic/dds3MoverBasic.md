# Matching notes: basic/dds3MoverBasic

## Mover callbacks and names

The writable bank at `0x002FA658` combines the native six-slot Mover
callback table with the immediately following ten 16-byte name rows.
The basic type registry refers to the callback table, and
`dds3AddMoverBasic` indexes the name rows with its modulo-ten counter.

Preserve the original 184 bytes and the three named callback relocations in
a dedicated data owner. The provider declarations retain their existing
types, the Camera dispatch remains at `0x002FA710`, and no code or small
data moves.
