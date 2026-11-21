# Event 912 cut-packet slots

The effect callback stores two live packet handles: the time-2 path writes
`nbE912CutPacket[0]`, and time 0x27 writes the second slot by its direct
`D_00527664` address. At 0x6D the callback reads both slots from the same
base to set both effects' end frames. The second label therefore denotes
one four-byte pointer at offset 4 in a two-pointer, zero-initialized writable
array. The next eight raw zero bytes do not participate in these accesses
and remain outside the C owner. Only the storage changes ownership; the
compiled event code is unchanged.
