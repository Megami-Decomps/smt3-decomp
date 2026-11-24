# Matching notes: SDF sound, disc, and print storage

Three existing EE zero-fill objects have native typed C definitions:

| Native address | Bytes | Object |
| --- | ---: | --- |
| `0x58F310` | `0x3C` | Persistent print M-RPC client |
| `0x58F780` | `0x0C` | Shared direct-file directory entry |
| `0x593880` | `0x5A0` | Sound engine work map |

The print service reads the server pointer after bind and supplies the
same client to the M-RPC call. The provider's shared definition accounts
for completion and bind parameters through offset `+0x38`; the old print
prefix alone stopped before those live fields. The four bytes before the
print buffer at `0x58F350` remain raw. The extent of that buffer is not
inferred from the gap to the next symbol.

The direct file lookup writes the size member at offset `+8` and returns
the same three-word directory record type used by the image lookup.
The bytes between that record and the sector buffer at `0x58F7C0` remain raw.

The sound initialization and control transfer pass exactly `0x5A0` bytes
to the IOP; the cache invalidation uses the same end address. The union
retains both the named channel, buffer, slot and state groups and the
existing word-indexed consumers. The next object, the PCM request,
begins at `0x593E20` and remains separate.
