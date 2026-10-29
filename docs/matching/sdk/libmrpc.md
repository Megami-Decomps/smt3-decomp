# sdk/libmrpc

## sceSifMUnBindRpc

60 of 71 words match at the native 284-byte size. The remaining eleven
words differ in the request/client prefix load/store order and its temporary
registers. Calls, waits, cleanup and the returned client result match.

The client extends the shared RPC prefix through three bind parameters at
0x30, 0x34 and 0x38. Its packet address and the request's self address are
opaque pointers, consistent with `SifRpcClientData_t` in
[the shared IOP types](../../../include/irx/iop.h). The request prefix has the
16-byte SIF command header, flags, self address, sequence, client address,
server ID and server address. Keeping those pointer fields typed recovers the
native alias contract; treating the entire request as word slots changes
scheduling.

The result starts at -1 after packet allocation and changes to the client's
result only after successful completion. Early return on allocation failure
emits 288 bytes; a success block alone remains 288 bytes. Initializing the
result before external calls instead of after allocation also leaves 288
bytes and adds a saved-register lifetime. These controls are measured; do
not repeat them.

The first semaphore is retained on allocation, creation and send failures,
exactly as native. Creation failure frees only the packet and returns -3;
send failure frees the packet and deletes only the new semaphore, returning
-2. Success waits for and deletes the new semaphore, deletes the old one,
and returns the client result. No extra resets or failure cleanup are added.
