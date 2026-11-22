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
