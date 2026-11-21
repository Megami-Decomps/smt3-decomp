# Matching notes: result comment record

The result lookup checks one four-byte record: an unsigned 16-bit key,
a signed comment byte, and the remaining native byte. The original
zero initialization is writable .data. Without the section attribute,
EE GCC emits the record in .sdata. Both adjacent words remain raw.
