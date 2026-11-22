# Recovery row X offsets

The four words at `0x0053D450` are the signed X offsets for the
four columns drawn by `rcvDrawRowText`: 36, 158, 253 and 346. The
native routine copies the complete vector to its local layout and adds
each word to the X origin when drawing the corresponding column. This
is not the string suggested by the first byte in the raw split.

The adjacent Y offsets at `0x0053D460` already came from C. The
`force_not_migration` entry keeps the X initializer out of the
assembly fallback while the fallback continues to reference the
same address. No row-rendering instructions or Y offsets change.
