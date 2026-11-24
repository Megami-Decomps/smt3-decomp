# Matching notes: interface/itfPanel

## Plain full-color and normal quad index rows (interface, 56 bytes)

CreateColorPlain passes exactly two 16-byte color-ratio rows to the
fixed-point multiplier. DrawNormal submits three rectangles with four
indices per row, from a vertex and a separate color index table. The
two actual RGBA rows and three rows of each index table are matched as
ordinary C. The untouched zero spans after each table remain raw.

## Panel draw callbacks (interface, 36 bytes)

Each of the nine panel kinds dispatches to a draw routine with the
same two-argument contract: the panel and the draw-tag list. The draw
table is recovered with nine native function pointers. The preceding
creation tables have separate callback contracts. The unreferenced
zero word after the draw table remains raw.

## Selected, plain and facility outline indices (interface, 62 bytes)

DrawSelWin draws two four-index border strips. Two further color rows
in the raw region are not used and remain raw. DrawPlain draws three
closed five-index outlines, reading 15 bytes from each index table.
DrawFacility draws three four-vertex quads, reading 12 bytes each
from its separate vertex and color tables. All four native short
suffixes remain raw; the two zero bytes after the plain tables and
eight after the facility tables are not part of these C objects.

## Panel initializer dispatch tables (interface, 72 bytes)

The panel kind indexes two independent, writable callback tables:
`CreateVertexTable` at `0x00326428` and `CreateColorTable` at
`0x00326450`. They are distinct from the panel draw table. Each has nine
32-bit slots: kinds 6 and 7 are null in both tables; kind 8 selects the
facility initializer. Vertex kinds 3 and 4 share `CreateVertexPlain`; the
color table selects `CreateColorPlain` and `CreateColorPera1` respectively.
All fourteen non-null entries retain their native callback relocations.

Each C table owns 36 bytes. Four bytes of alignment separate the objects;
another four bytes after the color table remain separate.

**Vertex initializer arguments.** Both position setters pass the kind-specific payload and four signed corner
coordinates. The absolute setter preserves those coordinate registers up to
its indirect call; the relative setter explicitly fills them with the updated
corners. The table now spells that shared five-argument contract, and each
initializer uses the corresponding typed payload locally. This removes the
too-few-arguments C call without changing any stores, table entries or linked
bytes. The separate draw callback contract is unchanged.


**Color initializer arguments.** The native dispatch passes the payload
in `$4` and the four incoming RGBA words in `$5` through `$8`. The C table
now uses that shared type. The providers keep their own payload layouts
and the same channel bits; kind 8 ignores the channels and keeps its fixed
tint. The two null entries and all seven callback pointers are unchanged.
This removes the mismatched indirect call without changing the linked
instructions or data.
