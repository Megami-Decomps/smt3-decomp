# Needle effect ordering tables

The needle model selects one of four render ordering table pointers by its
blend mode. The target addresses are the full 32-byte `sdfGrOtTable`
entries 43, 43, 44 and 45. The original call loads the filing callback
from offset 0x10 and passes the same table and the draw tag. The consumer
uses the existing `sdfGrOt_t::regFunc` C member rather than a private
prefix description of the same entry.

The new table owns exactly 16 bytes of data with four native
`R_MIPS_32` references to `sdfGrOtTable` and the original addends.
