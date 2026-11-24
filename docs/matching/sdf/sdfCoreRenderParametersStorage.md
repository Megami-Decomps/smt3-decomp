# Matching notes: SDF render parameters

The core renderer keeps three adjacent records in zero-filled storage:

| Address | Size | Record |
| --- | ---: | --- |
| `0x58F030` | `0x30` | Face-division parameters |
| `0x58F060` | `0x10` | Fog vector |
| `0x58F070` | `0x30` | World-light basis cache |

The face setup writes scale and scaled Y into the first quadword and VU0
writes the base and delta colors into the next two. Packet generation
also takes the addresses at offsets `+0x10` and `+0x20` of the same
record. The world-light routine stores three consecutive VU0 vectors;
reflection mapping reads the same three vectors. The fog update copies
one quadword. The records are all quadword-aligned because the VU0 and EE
vector transfers require it. The face packet exposes the same bytes through its scalar-field and
quadword views; the quadword member supplies the alignment.

The preceding triangle workspaces and the 32 unidentified bytes after
the cache are separate. The explicit section places the records in this
input's BSS rather than in COMMON; removing it puts all three in COMMON.

Production consumer code, initialized data and the retail image are
unchanged. In the existing nonmatching `sdfCorePacketGen` draft, the
quadword member replaces the cast from the scalar struct. This swaps
two private stack slots (six instruction displacements) without changing
any call, relocation target, or the draft's positional score. The
production function still uses the original assembly.
