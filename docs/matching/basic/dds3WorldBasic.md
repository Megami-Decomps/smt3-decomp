# Matching notes: basic/dds3WorldBasic

## worldBasicTable data (basic/dds3WorldBasic, 24 bytes)

The native writable table at `0x002FA610` holds four existing typed
World callbacks and two reserved null entries. The C initializer uses the
existing `dds3BasicFunction_t` layout. The 24-byte split leaves the preceding
alignment and the separate Scene table at `0x002FA628` unchanged.

All 25 owner functions and their relocations remain exact. Both built
`.data` images and their four callbacks agree after relocation, and the
full retail image is byte-identical. The game report now includes the 24
bytes in this unit's data total; the full report moves the same bytes out
of its original raw holder and adds a suffix unit. The owner's raw
`.data` fuzzy score is 87.5%; linked equality is verified separately.
No matched-code, function, or aggregate matched-data count changes.
