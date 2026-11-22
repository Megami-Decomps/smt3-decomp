# Empty font message and optional name strings

`frEmptyString` at ROM `0x450430` supplies the empty glyph sequence to
`frLinkFontString` for message handling. The parser measures it with
`strlen` and reads no glyphs. The C object owns just the terminating byte.
The next seven bytes align the separate optional name at `0x450438`.

`frCNameTable[0]` points at that second empty string, `D_0054F438`.
`frInitNameData` measures it with `strlen` and copies zero bytes. The defined
C array is one byte, the NUL; the seven following zero bytes are native
alignment before the next small-data input. The generated padding marker
preserves their placement without attributing them to the string.
The reader's declaration now gives the one-byte extent. Its four pointer
relocations and allocated bytes are unchanged.

The historical compiler aligns the literals to eight bytes. The original
`frFont` small-data prefix starts at an address aligned to four bytes only;
the data-only fragment starts at the first literal's native eight-byte boundary.
An explicit alignment attribute on the second string was byte-inert and
is not retained. Normal and `SKIP_ASM` objects place the string at the same
offset in their data fragment, and the complete image matches retail.
