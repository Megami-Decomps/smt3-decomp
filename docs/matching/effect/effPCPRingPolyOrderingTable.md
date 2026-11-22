# Ring polygon draw ordering

The ring polygon list selects one of four native render ordering tables by
blend mode, then calls its existing register-tag callback with the chosen
table and draw list. The four pointers resolve to `sdfGrOtTable` entries 43,
43, 44 and 45. This preserves the original targets and relocation addends.
The eight following raw zero bytes stay outside the C owner. No draw or
creator code changes.

The six packed triangle words at `0x0031FEE0` and the four at
`0x0031FF60` use the same low-byte index encoding as the preceding ring
and boss-strip lists. They now have their own C definitions in the
corresponding data units. The separate labels preserve the 48-byte ring
and 64-byte boss list bounds; the drawing code does not acquire a new
reference to either tail. Both C data sections and their native reference
sections match byte for byte, and the linked executable remains identical
to retail. This adds 40 bytes of C-owned data, not code.
