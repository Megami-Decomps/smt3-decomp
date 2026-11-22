# Empty font message string

`frEmptyString` at ROM `0x450430` supplies the empty glyph sequence to
`frLinkFontString` for message handling. The parser measures it with
`strlen` and reads no glyphs. The C object owns just the terminating byte.
The next seven bytes align the separate optional name at `0x450438`, which
remains assembly-owned.

The historical compiler aligns the literal to eight bytes. The original
`frFont` small-data prefix starts at an address aligned to four bytes only;
the data-only fragment starts at the literal's native eight-byte boundary.
The original text unit, both original symbol addresses, and display semantics
are preserved.
