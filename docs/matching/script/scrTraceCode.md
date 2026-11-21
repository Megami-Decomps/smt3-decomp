# Matching notes: script/scrTraceCode

## Script command table

`CodeFunc_Comm` indexes eight-byte records with the script operand, calls the
handler in each record, then drops its recorded number of arguments if the
handler finishes. Pending handlers leave the stack alone.

The table is 544 records at `0x0052E350`, with 438 named handler relocations
and 106 null entries. Its typed initializer moves unchanged to a
read-only data owner: the dispatch table matches independently of the
compiler-emitted switch tables that follow it at `0x0052F450`.
The retail addresses, argument counts, and switch targets stay unchanged.
