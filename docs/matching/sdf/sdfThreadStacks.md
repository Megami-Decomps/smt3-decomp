# Matching notes: SDF thread stacks

The retail EE thread constructors pass these exact workspace bases and
byte counts. The sound command initializer writes the workspace address
and byte count to the thread descriptor before calling `CreateThread`;
the other sites pass them to `sdfStartThreadAndWait` or
`CreateThreadSimple`.

| Workspace | Native span | Bytes |
| --- | --- | ---: |
| System thread | `0x5756C0..0x5776C0` | 8,192 |
| Vertical-blank thread | `0x5776C0..0x577EC0` | 2,048 |
| Display thread | `0x577FF0..0x578FF0` | 4,096 |
| Sound command thread | `0x591840..0x592840` | 4,096 |
| Sound SIF RPC service | `0x592880..0x593880` | 4,096 |
| IPU decode thread | `0x593F80..0x594780` | 2,048 |

The six definitions own 24,576 bytes of native zero-fill storage. They
leave the chip and image state, draw workspace, sound transfer scratch
and map, file RPC queue, and decompression history outside the new C
objects. Existing declarations and consumers are unchanged.
