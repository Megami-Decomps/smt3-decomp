# Pipe part presets

The 60-entry part catalog points to four contiguous 12-byte geometry and
timing records. The presets retain the existing two float fields and unsigned
frame count. In particular, the common frame walker follows the catalog's
selected record and loads its word at offset 8.

Stock GCC aligns separate struct globals to eight bytes. Give the four
records one typed array allocation instead, retaining the three real interior
names as bounded aliases at offsets 12, 24 and 36. All 60 native catalog
relocations are unchanged, including both sets of addresses with offsets
that would acquire an unwanted gap if emitted separately. The preceding
four bytes remain raw and receive no match credit.
