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
creation tables use different typed storage and call conventions and
remain raw; the unreferenced zero word after this table also stays raw.

## Selected, plain and facility outline indices (interface, 62 bytes)

DrawSelWin draws two four-index border strips. Two further color rows
in the raw region are not used and remain raw. DrawPlain draws three
closed five-index outlines, reading 15 bytes from each index table.
DrawFacility draws three four-vertex quads, reading 12 bytes each
from its separate vertex and color tables. All four native short
suffixes remain raw; the two zero bytes after the plain tables and
eight after the facility tables are not part of these C objects.
