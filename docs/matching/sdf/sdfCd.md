# Matching notes: sdf/sdfCd

## CD streaming sector buffer

The streaming reader passes aligned whole-sector destinations directly to
the CD service. It uses this one-sector buffer for unaligned destinations
and reads that end partway through a sector. A short read keeps the pointer
and byte count for the remainder, which the next read copies before fetching
another sector.

This buffer owns the 2,048 native zero-fill bytes at
`0x58F7C0..0x58FFC0`. The preceding direct-search directory entry and the
following HDD power-off stack remain in raw storage. The existing
`sdfCdImageRead` function and external buffer declaration are unchanged.
