# Matching notes: read-only linker fill

## Puzzle Boy and event command data

Puzzle Boy's debug owner emits 16,393 read-only bytes, and the event
command owner emits 1,967. Both match retail in normal and SKIP_ASM
builds. The retail linker places another seven and nine zero bytes,
respectively, before the next input's alignment boundary. Neither
C owner provides those bytes.

Only the two comparison references distinguish this fill from the
compiled data. The build verifies the complete current link inputs,
actual placement, and absence of references into either gap. The
fill stays in the data denominator and remains unmatched. The
production linker inputs and executable are untouched.
