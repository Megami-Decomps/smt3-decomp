# Matching notes: dds3DebugStuff

## Primitive debug text box tables (kernel/dds3DebugStuff, 15 bytes)

The three RGB attribute bytes at `0x002FA930` and three label pointers at
`0x002FA938` are writable C objects. The pointers still resolve to the
same R, G and B labels. The normal compiler puts the small attribute
array in `.sdata` without the section declaration, which also moves the
label table within `.data`. Its explicit `.data` placement preserves
both native addresses and the five naturally aligned bytes between them.

All 11 owner functions and the linked retail executable are unchanged.
The reports include this 20-byte data contribution but give it no
additional matched-data credit; the linked pointers and bytes are exact.
The twelve bytes between the labels and the next shadow data stay in assembly.
