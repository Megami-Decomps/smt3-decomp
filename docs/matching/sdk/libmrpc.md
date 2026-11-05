# sdk/libmrpc

## sceSifMInitRpc (sdk/libmrpc, 196 bytes)

All 49 of 49 words match at the native 196-byte size in ordinary C.
Initializing the actual uncached packet base before count and sequence, then
the two observed zero fields, preserves native setup. Writing handshake
word four before word five gives the native emitted store order. The ordinary
count/sequence-first control remains 38/49; reversing only its handshake
assignments reaches 40/49. No register constraint, volatile, invented consumer,
provider change or compiler flag is needed. These are measured source/output
pairs, not an internal scheduler-cause or original-source claim.

The initialized flag is set before disabling interrupts. Repeated calls return
without changing interrupt state. The first call registers the unchanged
completion handler, reenables interrupts, sends the handshake and polls SIF
register one until nonzero. Send failure is not separately handled, no timeout
is added, and the mode argument remains unused as in native and its caller.

The real packet allocation is 0x800 bytes, matching 32 packets of 0x40 bytes
and the existing allocator's stride. Queue storage is 0x40 bytes; its type
exposes only the observed 20-byte prefix, including zeros at +0xC/+0x10,
without inventing tail fields or changing ownership. The queue packet base
uses the native uncached address alias, while the handshake uses the cached
second packet. Its two one-valued words and 24-byte send are preserved. No
packet allocation, freeing, new bounds or extra memory accesses are added.

Provider declarations match the actual command-handler, send-command and
register-reader definitions. The unchanged completion callback has the real
two-pointer dispatcher ABI. Declaration/type-only changes are completely
output-neutral in production, SKIP_ASM and retained-C modes. Independent
native/layout/ABI review and both owner checkers approve six exact C functions.
All nine strict baseline/declaration/final plain/dump comparisons preserve
complete assembly and ELF contracts. All eight production peers, five SKIP
peers and six retained peers, owned data and relocation targets are preserved;
only expected code-relative positions change when an omitted body becomes C.
The 168-byte owned rodata remains exact. The minimal ordinary control is also
qualified across all three modes. Full retail, generated-source, regression,
guarded-source and report gates accompany this source-only landing; no private
tooling changes, binaries or traces are included.

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

## _request_end (sdk/libmrpc, 264 bytes)

All 66 of 66 words match at the native 264-byte size in ordinary C. The
unsigned command dispatch retains cases 0x8000001A, 0x80000019, 0x8000001D
and 0x8000001C, followed by the shared cleanup. No constraint, compiler flag,
volatile storage, callback view or new optimizer promise is needed.

The command 0x8000001A arm captures the client, conditionally invokes its
completion callback with its current argument, then reads that same client's
completion semaphore after the callback. The bind arm copies the returned
server, buffer and callback-buffer pointers. The result arm copies the signed
result. Those arms signal the completion semaphore before reloading the client
from the response. Command 0x8000001C and unknown commands go straight to the
common cleanup. The reload after external calls remains observable; it is not
replaced with a long-lived client snapshot.

Cleanup captures the response's current client, signals its old semaphore if
nonnegative, then loads and frees that same client's current packet before
clearing its packet field. It does not reload the response's client after the
optional old-semaphore signal. No extra null test, reset, error handling or
resource release is added. Callback, signal and free
ordering and all native access widths are preserved, including the native
completion-semaphore load before bind/result stores in emitted code.

The response is an observed 48-byte prefix, not a new whole-packet capacity
claim. Its +0x24 union describes the actual server-pointer/result alternatives;
the client and buffer fields keep their truthful pointer types. The existing
client declaration is moved before this body without changing its layout.
The two-pointer command-handler ABI and signed semaphore-provider signature
match their actual dispatcher/providers. No allocation or data ownership moves.

Independent native, callback/reentrancy, layout and ABI review approves this
source. Both complete-owner checkers report seven exact C functions, including
the earlier initializer. Baseline, declaration-only and final sources pass all
nine strict three-mode plain/dump complete assembly and ELF comparisons; the
declaration-only changes are wholly neutral. All 8/6/7 peers and owned data
are preserved across production, SKIP_ASM and retained-C contexts, resolving
callback-address relocations to the same function when it becomes C-defined.
The production 168-byte rodata is unchanged. Full retail, generated-source,
regression, guarded-source and report gates accompany this source-only landing;
private tools, binaries and raw traces are excluded.
