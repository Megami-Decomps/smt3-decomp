# Matching notes: CD/DVD N-command state

The native N-command block starts with busy cleared, the existing
command-reset word cleared, and a negative initialization sentinel.
The existing +4 and +8 address aliases remain in place. The +4 reset
slot is not the separate last-command variable.

The shared structure retains the original busy accesses and emits all
12 file-backed bytes in ordinary .data; no placement attribute is
needed. The following SCMD and callback bytes remain raw.
