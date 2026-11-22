# Item catalog and adjacent search data

The retail catalog at `0x3DC4A0` has 144 eight-byte records: two flag
bytes, a skill halfword, and a price word. The existing 148-byte
weighted search table starts at `0x3DC920`. Its flag, four groups and
weights are unchanged, and its original typed symbol names the actual
member at that address via the existing native symbol-alias ledger.
The four bytes after the search table remain outside the object.

The command and shop inventory scans still cover 160 positions.
For IDs 144 through 159, native field-width reads reach the adjacent
search table. The command path can carry those IDs through action
selection and skill lookup, and the shop sell path can reach the
price word. Keep both sets of actual fields, not 16 invented item
rows. Byte reads use the complete bank's character representation;
the two wider reads use its direct unsigned union members, preserving
the target's native 16- and 32-bit loads. The two bounded Camp paths
still use ordinary pointers into the actual catalog rows.

The five changed readers retain their complete allocated object
sections and ordered relocations in both ordinary and retained
draft modes. No new matching constraint or code change is needed.
The added authored data is 1,152 bytes; the search data was already C.
