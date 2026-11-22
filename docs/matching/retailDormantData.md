# Isolated retained retail data

The 16-byte door row at 0x002FBF30 uses the same structure as the six
neighboring Dante door rows: an eight-byte name field, signed cell
coordinates and a binary32 angle. It retains the generated address label;
no reader or gameplay effect is asserted. The early owner includes the
row, and the next owner still starts at 0x002FBF40.

The date at 0x0052DC80 is the 12-byte NUL-terminated retail compiler build
date. A separate four-byte gap and the complete following time-and-tail
interval remain raw. The independently verified literal data is at its
original address; no generated current date or runtime code uses it.

The early warp-label copies at 0x002FF3A0 and 0x002FF3F0 use seven
16-byte slots, the same width as the later live selector labels. The
early copies retain their own writable bytes and the original address
labels; the live NPC label between them and the following `fldInf`
owner are unchanged. No new reader or source-level alias is inferred.

The four retained memory-card lines at 0x00528180, 0x00528198,
0x005281B0 and 0x005281C0 are writable strings with the native eight-byte
alignment. C owns 82 bytes including internal alignment, from the first
string through the last terminator. The following six zero bytes remain
separate so the active card-message owner still begins at 0x005281D8.
No live message or reference is redirected.

The 48-byte writable vector group at 0x00529640 follows the live camera
up vector and precedes the screen-state records. Its three aligned
binary32 rows are Z-only, W-only and all ones. The source retains the
generated address label and both neighbors without inferring a reader
or historical declaration.
