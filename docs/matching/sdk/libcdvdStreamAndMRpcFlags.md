# CD/DVD stream and M-RPC initialization flags

The CD/DVD stream-active word at `0x52BA70` is read by the stream query
and cleared on stop. The separate M-RPC initialization word at
`0x52BA88` guards repeated initialization. The init routine sets it
before setup and the exit routine clears it. Both are native 32-bit
state words and start at zero.

Each belongs in initialized writable `.data`: removing its section
attribute moves it to `.sdata` with the pinned EE compiler. Only the
two state words move to C. The two intervening 20-byte ranges,
including their original SDK revision bytes, stay raw.
