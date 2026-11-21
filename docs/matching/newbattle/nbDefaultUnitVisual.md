# Default battle unit visual

`nbGetUnitVisual` and `nbGetMotionTable` select the row at `0x3494B0` for
unit ID zero. Their ordinary table's `0x310` stride also gives the standalone
row's extent, ending at the next native owner (`datDevilRoster`). This row has
the same visual scalar and 26 motion slots as a model visual, including three
trailing reserved words. The C consumers use the same complete row type. The
old split part view at +0x32 started inside the motion sound fields and its
actframe field actually selected the next motion. Accesses now use the native
motion fields at the same offsets; the sound lookup retains the exact signed
byte at `0x31 + 0x1C * index`.

The existing event-spawn scale divisor relocation to `D_003494C0` selects the
same row's `formsize` at +0x10, not an independent array. A linker alias
retains that address and the original relocation. The four bytes immediately
before the row remain raw, outside C ownership.
