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

## sceSifSetRpcQueue

140 of 148 native bytes, 17 of 35 compared words match; eight bytes are
uncovered. The native append walk keeps separate next-link and current-node
copies. The direct C walk coalesces them and uses another register for the
initial head, shifting the following instructions. An explicit next cursor
in the loop condition shrinks to 132 bytes, so it does not recover those
copies. No additional cursor/declaration sweep is justified.

The routine uses the shared six-word server queue and appends to the global
queue list at packet-pool header offset 0x28. It initializes thread ID, active,
server list, pending start/end and next under interrupts disabled, then
restores interrupts through a tail call. It returns void. This server queue
is distinct from the packet-pool header.

## sceSifRegisterRpc

200 of 208 native bytes, 27 of 50 compared words match; eight bytes are
uncovered. It has the same append-walk copy difference as `sceSifSetRpcQueue`.
The explicit next-cursor condition instead emits 192 bytes; keep the direct
walk until a new compiler property explains the native copies.

The shared 0x44-byte server record supplies the handler, buffer, optional
callback/buffer, service ID, registration link, request link and owning queue.
Only native-written fields are initialized. The function appends through the
registration link at 0x38, while pending requests use the separate link at
0x3C. The owning queue is at 0x40. Interrupt restoration is a void tail call.
