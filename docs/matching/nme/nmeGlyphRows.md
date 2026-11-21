# Name entry glyph rows and name-list source

Mode 3 uses a pointer to its encoded name list in the same writable
data run as the three default glyph-row pointers. The cursor indexes
the first two bytes of each 32-byte row, while initialization measures the
encoded string with strlen. The pointed-to array stays const.

Both stock builds preserve the four ordered native pointer relocations and
all allocated bytes and relocations of the initialization and drawing units.
Without explicit .data placement the four-byte pointer goes into .sdata.
The guarded cursor draft now uses HI16/LO16 to address the actual non-small-data
object rather than an out-of-range GPREL16 relocation. With the corrected
declaration its old volatile qualification is redundant and removed; the
guarded function is not promoted.
