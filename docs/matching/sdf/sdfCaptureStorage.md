# Matching notes: SDF heap, file client, and capture packet

Three native EE zero-fill records have typed C definitions:

| Native address | Bytes | Object |
| --- | ---: | --- |
| `0x577F18` | `0x28` | SDF heap control and its two embedded sentinels |
| `0x577F40` | `0x3C` | Persistent IOP file-service M-RPC client |
| `0x577F80` | `0x70` | SDK store-image packet shared by the capture paths |

Heap initialization and allocation use the embedded head and tail handles
through the existing heap-control type. Its size ends precisely at the
next native symbol, the file-service client.

The client uses the same complete SDK definition as the print-service
client. Native bind sets the server at offset `0x24`, and its provider
also writes the completion semaphore and bind parameters through
`0x38`. The earlier `0x28` metadata described only the prefix through
the server. The four bytes after the actual record remain raw.

The SDK builds a four-word DMA/VIF header, a GIF tag, and five GS
register/value pairs for capture. Its packet extent is seven quadwords,
ending at the independent display-thread stack without consuming any of
that stack. The SDK and capture consumers now agree on the same type.
