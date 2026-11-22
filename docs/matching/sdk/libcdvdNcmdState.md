# Matching notes: CD/DVD startup state

The native N-command block starts with busy cleared, the existing
command-reset word cleared, and a negative initialization sentinel.
The existing +4 and +8 address aliases remain in place. The +4 reset
slot is not the separate last-command variable.

The shared structure retains the original busy accesses and emits all
12 file-backed bytes in ordinary .data; no placement attribute is
needed.

The adjacent S-command state and callback-reason pair are part of the same
44-byte writable startup block at 0x0052A030. Five S-command initialization
words start at -1 and the invocation count starts at zero. The pending
reason is shared with the interrupt callback; the saved reason is passed
to the registered callback. Both reason words start at zero. The C owner
preserves the original subrecord labels at +0xC and +0x24 as non-owning
views, including the volatile accesses already used by the interrupt path.

The three record types retain natural four-byte member alignment. Declaring
the 24-byte record as its own object made the configured compiler start it
at the next eight-byte boundary; the containing record preserves the
actual internal offsets without packing, padding fields or extra bytes.
The following 36 raw bytes are separate and the next reply begins at its
original address.
