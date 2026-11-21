# Matching notes: SDK boot TLB entries

The native `InitTLB32MB` descriptor names 13 kernel mappings and 18
default RAM mappings. Each iteration loads the four 32-bit fields of
one record, passes them to `_SetTLBEntry`, and advances by 16 bytes.
The descriptor derives its three counts from the actual array extents.
The kernel and default pointers, extended table and consumer are unchanged.
The loaded-count field starts at zero and the following four linker bytes
are left raw.

The relocation overrides preserve hardware register values as numbers.
With the overrides omitted, the data disassembler mistakes some entry
words for message or code addresses. The compiled arrays contain no
pointers or relocations.
