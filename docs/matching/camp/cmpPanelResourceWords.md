# Camp panel resource words

The six writable words at `0x00345D88` are all zero in retail.
`cmpTexLoad` fills the first five with texture handles; the corresponding
free loop visits exactly five. The sixth is not a texture slot or
terminator. The loader clears it, and `rstSetResultWork` and
`rstClearResultWork` store the result work address and zero there.

The C definition owns these 24 bytes and keeps the separate preceding
four-byte pad and following skill message table unchanged. Pointer-form
texture consumers convert the same 32-bit handle words locally.
