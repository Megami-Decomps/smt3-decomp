# Matching notes: Magatuhi billboard UV storage

The UV bank at `0x56AF30` contains fifteen rows of eight signed
halfwords (240 bytes of EE zero-fill). The initializer writes one
16-byte row per iteration. It now advances a pointer to an outer-array
row rather than advancing a pointer to the first row's elements.

The two draw call sites pass the complete bank to `sdfBillBoardMakeN`.
A full batch has 15 billboards; the tail batches have 1 through 14.
The packet builder copies 16 bytes per billboard and never extends into
the separate saved view target at `0x56B020`. The preceding raw bytes
also remain separate. Without the explicit `.bss` definition, the
compiler emits a COMMON symbol instead of storage in this owner.

The row and whole-bank pointer changes preserve the native instruction
and relocation streams. The storage definition adds no instructions or
initialized bytes.
