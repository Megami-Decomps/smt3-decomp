# Matching notes: effect/polyManager

## Track geometry and draw callbacks

The initialized bank at `0x00317380` contains the existing five packed-face
tables, six aligned vectors and draw dispatch tables. It is 832 bytes.
Its 33 data relocations name the same 29 callback slots and four
ordering-table operands as retail. Dispatchers and provider signatures
are unchanged, including the two-argument acute-width callback.
