# libgcc startup callback lists (16 bytes)

The constructor and destructor lists begin at `0x002F5600` and
`0x002F5608`. Each starts with the EE GCC runtime's all-ones sentinel
and ends with a null callback. The constructor loop reads word zero
as a count or sentinel; the destructor cursor starts at word one.
Neither consumer calls the sentinel or the terminator.

The section attribute is needed: without it, stock GCC puts these
eight-byte objects in `.sdata`, not at the original `.data` addresses.
Normal and SKIP_ASM builds agree on both lists. The following 112
bytes of reginfo and alignment remain raw and unchanged.
