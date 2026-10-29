# sdk/libkernl

## sceSifCheckStatRpc

44 of 60 native bytes, with 5 of 11 compared words matching and 16 bytes
uncovered. The native routine separately branches on the active bit and
returns literal zero or one. The ordinary C compiles the same condition to a
direct masked-bit return, with fewer branches and a shared return.

The client is the shared packet-address/sequence prefix. A null packet,
a sequence mismatch against packet offset 0x18, or a clear active bit at
packet offset 0x10 returns zero; otherwise it returns one. Packet storage is
the actual 64-byte protocol record already used by allocation and release.
No field volatility or callback return changes are justified by this gap.
The assembly fallback remains active.
