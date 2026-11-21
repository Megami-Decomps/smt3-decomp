# Matching notes: Terminal texture path

The Terminal loader's loop reads a single pointer to the existing
terminal texture path. The path is read only. The writable pointer
slot is in the native .data section; EE GCC places the standalone
four-byte object in .sdata if the section is not specified. The word
after the slot remains raw.
