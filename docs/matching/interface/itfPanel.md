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
