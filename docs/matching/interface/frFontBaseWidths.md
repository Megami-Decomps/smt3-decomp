# Font base advances

The six writable advances at 0x326060 cover four default and two staff
fonts. The alternate staff font uses slot 5, whose initial advance is zero.
The two bytes before the next palette are outside this C array.

The extension dictionary can select slot 8. The native glyph lookup adds the
unsigned index to the EE address, then treats the byte as signed. Slot 8
therefore reads the first byte of the neighboring palette, not an array
element. The skill list's row setup writes the real element at index 1;
the generated comparison uses the corresponding HI16/LO16 pair with addend
one rather than a separate variable at the same address.

The native-data tail check recovers the two array bytes omitted by the
stock four-byte reference, without including the following two-byte gap.
