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

The boolean-result local that matches `sceSifIsAliveIop` does not recover this
routine's native branches: it still emits 44 bytes, with 5 of 11 compared
words matching and 16 bytes uncovered. The preferred direct-return draft is
unchanged; do not repeat that local-result control without a new premise.

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

## sceSifInitIopHeap

31 of 34 words match at the native 136-byte size. The last three differing
words construct the initialization-flag address, store zero and return zero:
native uses the return register for the address before clearing it; the C
uses a separate address register and clears the return register before the
store. The bind/retry loop and its four countdown NOPs match ordinary C.

Naming the global client at each use reproduces the native separately saved
high address and per-iteration client address. Keeping one local client pointer
instead emits 128 bytes. Moving the successful flag store and return into the
loop emits 144 bytes and changes its control flow; neither is preferred.

The routine binds service `0x80000003` synchronously, returns `-1` for a negative
bind result, and retries after the native countdown while the bound server is
null. Only successful binding clears the heap initialization flag. No new
volatile accesses, instruction padding or forced register use are justified.

## sceSifSetIopAddr

51 of 55 words match at the native 220-byte size. The final four differences
are the error constant / `-1` setup order and the zero/error conditional-move
selection. Address/width header writes, input-width dispatch, request payload,
RPC arguments and all earlier instructions match. The native initializes the
error result and conditionally selects zero; the ordinary ternary uses the
opposite selection direction.

An explicit result initialized before binding retains an extra saved register
and emits 224 bytes. Assigning that result after capturing RPC status emits
216 bytes, as does a direct nonnegative-success return. Keep the native-sized
ternary until a new compiler premise explains the selection; no forced use,
volatile result or register assignment is justified.

Binding failure returns `-65536`. After a successful bind, the address and kind
header words are written before validating kind. Kinds zero, one and two copy
one, two and four bytes respectively into request offset eight. An invalid
kind returns `-65538` after the header writes, without reading the input or
calling RPC. The valid transaction sends 32 bytes, receives 16 bytes, and
returns zero for a nonnegative RPC status or `-65537` otherwise.

## sceClose

376 bytes against retail's 380, with 80 of 94 emitted words matching and
14 differing; four native bytes remain uncovered. Through the completion
result load at +0x14C, all instructions match apart from four exit-branch
displacements caused by the shorter tail. Native loads -1 into a register,
compares it with the signed completion result, and selects zero for a
nonnegative result. The draft emits `slti result,0` and `movz`, shortening
the clamp by one instruction and shifting the epilogue.

The 20-byte shared close request contains completion semaphore, result
pointer, result size, native descriptor and request-table index. Only
maximum count, initial count and option are initialized in the semaphore
descriptor. RPC failure leaves the slot active, deletes the semaphore,
unlocks and returns -11. RPC success clears the slot, captures the uncached
reply before unlocking, and either rejects a zero reply or waits for the
completion callback before reading its stack result. Uninitialized service
and invalid-slot exits return -1 and -9 respectively, after unlocking.

Modifying the address-taken result directly emits a stack write and a
384-byte body. Returning either ternary spelling emits 376 bytes with a
comparison against one; copying the completion into a separate status scalar
and clamping it emits the preferred comparison against zero. Writing the
condition as `status > -1` is inert. No qualifier, false result type or
artificial dependency was added. The remaining question is which genuine
compiler/source property retains the native register comparison at this tail.

## sceIoctl

844 bytes, the native size, with 165 of 211 words matching and 46 differing.
The differences end at +0xEC: early saved-register allocation, special-command
dispatch and pending-table setup remain unresolved. The subsequent status
stores, both aligned and unaligned 1024-byte copy paths, completion transaction
and epilogue match. Ordinary constant-size `memcpy` generates both copy paths.

The argument pointer is published before optional filesystem initialization.
After validating the slot, the request words at +0x414 and +0x418 are cleared.
Command 1 waits on the pending-table semaphore, skips entries equal to -1,
and writes zero only when all 32 entries are free; any occupied entry writes
one. Its output pointer is reloaded from the shared argument global after
waiting. Commands 2 and 3 copy the uncached 32-bit or 64-bit status word to
the caller. These special paths release the filesystem semaphore and return
zero without RPC. Other commands forward the native descriptor and command,
copy 1024 argument bytes when nonnull, and send the complete 0x420-byte request.
Completion/error ordering follows the recovered filesystem transaction.

An initial early pending-result local and command-first request stores emit
836 bytes. Removing that unnecessary local and assigning descriptor before
command gives the preferred ordinary 844-byte body. No return type, qualifier,
register constraint or artificial dependency was introduced to close the
remaining allocation/control differences. A genuine compiler premise is
needed before another pass over the early dispatch.

## supplement_crt0 (sdk/libkernl, 0x48)

4 of 18 words differ: four independent descriptor stores have a different
order, beginning at +0x10. All 72 bytes are emitted; the other 14 words match.
Two ordinary six-word `rpcSemaParam` locals receive initial and maximum
counts of one before the first `CreateSema`; all remaining fields stay
untouched. The two semaphore results are stored separately. The native
0x50-byte frame needs no invented descriptor padding or alignment.

One chained four-field initialization produced the same residual as separate
assignments. The readable shared-field initialization retains the same four
store differences. Extern globals use their existing data ownership without
section overrides; there is no volatile scratch, barrier, copy assembly or
extra flag. A follow-up needs evidence for native initialization scheduling.
