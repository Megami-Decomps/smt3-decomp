# sdk/libkernl

## _request_end_0 (sdk/libkernl, 180 bytes)

45 of 45 words match in ordinary C. The unsigned response-command switch
handles call completion, binding and read-data completion. The explicit
read-data case is a genuine no-op before common cleanup: `_request_rdata`
writes command 0x8000000C into the response and sends it to this handler.
Removing only that case emits 160 rather than 180 bytes, with 7 of 40
compared words matching. No fabricated case, constraint or compiler flag is
used; matching does not establish the original source spelling.

Full RPC clients now contain the existing 12-byte transaction header as their
first member. Common cleanup can therefore access either that actual subobject
or the standalone 12-byte header used by read-data requests. Only the call and
bind cases convert the first-member pointer back to its containing 40-byte
client. All field offsets are unchanged. The eight corresponding field
expressions in `sceSifBindRpc` are updated, with no output change.

The 48-byte response type describes the observed prefix of a received command.
The native command interrupt dispatcher copies incoming quadwords into stack
scratch before passing that scratch to the registered handler. This is not a
typed overlay on the separately declared outgoing `rpcPacket` word-array pool.
Strict aliasing remains enabled. Independent layout checks cover every field
of the transaction, full client and response, as well as packet size.

Call completion captures the full client and invokes its optional callback
with the saved argument, then rereads the response's client after the callback.
Binding copies server, buffer and callback-buffer fields in native order.
Common cleanup captures the transaction once, signals a nonnegative signed
semaphore, reads its packet after signaling, frees that packet and clears the
same transaction's packet field. It does not cache the packet prematurely or
reload the response client after signaling/freeing. The generic two-pointer,
void-return dispatcher ABI and all scalar access widths are preserved.

Independent semantic and complete-output review approves the end-only change;
`_request_bind` remains assembly-backed. The declaration/embedding-only control
is completely output-neutral. Baseline, declarations, final source and no-op
case ablation pass twelve strict production, SKIP_ASM and retained-C plain/dump
assembly/ELF parity pairs. Public and local complete-owner checks report 154
match, 0 differ. All 363 production, 153 existing SKIP_ASM and 158 retained-C
peer bodies preserve their instructions, extents and resolved relocation targets.
All owned allocated data is unchanged, including 664 production rodata bytes
and 98 relocations. The retained initializer's newly defined callback relocation
is explicitly resolved to the same function and offset, rather than masked.

Production provenance verifies the unique compiler-owned 180-byte span at
0x002DE808 and its exact linked retail bytes. Full retail checksum,
generated-source, regression, guarded-source and report gates pass. Exactly
one SDK function and 180 bytes are added; game-code coverage is unchanged.

## sceSifAddCmdHandler (sdk/libkernl, 0x2C)

11 of 11 words match, 44 bytes. The command's sign selects the existing
system or user handler table. Its unsigned 32-bit left shift by three forms
the native byte offset, dropping the upper three bits, including the selector,
through normal truncation.
The callback and argument use the existing eight-byte record, at offsets zero
and four. The emitted stores publish the argument before the callback, with
the callback store in the return delay slot, exactly as retail.

The original command is consumed by the shift and sign test. Both branches
then overwrite that same scalar with the selected table's actual EE address;
the later unsigned addition consumes only this new value. This disjoint-phase
reuse keeps the native table-base register without a fixed binding or empty
constraint. Actual configured SDK RTL shows the shift and sign uses before
the table-load definitions, then the address addition and real entry stores.
Ordinary-wrapper and diagnostic output agree across the complete owner;
no hidden allocator decision or original-source spelling is claimed.

Existing table setters and the native dispatcher establish the same table
fields and callback signature. The dispatcher checks its table count, but
registration itself has no bounds or null check. Preserve that native contract:
callers must select valid initialized table storage. This is EE 32-bit address
arithmetic, not a general host-pointer or concurrent-publication guarantee.
No table extent, record layout, qualifier, provider type or data ownership
changes. Provenance maps this routine to `libkernl.a/sifcmd.o` by SDK name;
it is not one of the syscall stubs.

## sceSifRemoveCmdHandler (sdk/libkernl, 0x28)

10 of 10 words match, 40 bytes. The same unsigned offset, sign-selected table
and disjoint command/address scalar phases locate the existing entry. Only
the callback word is cleared; the argument word is left unchanged. The native
store remains in the return delay slot. The SDK object map independently
byte-identifies this routine in `libkernl.a/sifcmd.o`.

Both new bodies use ordinary C. A prior diagnostic local register binding
also matched, but the scalar reuse makes it unnecessary and it is not retained.
All 362 pre-existing full-production functions retain their bytes, offsets and
sizes. All 664 rodata bytes and 98 rodata relocations remain unchanged. The
four changed raw object text bytes are equivalent data-symbol relocation
addends within the new targets; resolved targets and the linked retail image
are checked separately, rather than treating raw-object equality as proof.

Strict implicit-declaration builds pass in production, `SKIP_ASM`, and
`NON_MATCHING` plus `SKIP_ASM`. Full plain/dumped assembly agrees in all three
modes after only generated `.file` scratch-path normalization. The unchanged
public checker reports 148 match, 0 differ for the complete owner. Full retail
checksum, affected-owner/data and regression qualification accompany the
landing. Exactly two SDK functions and 84 SDK bytes are added; game-code
coverage is unchanged.

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

Matched all 148 native instruction bytes in ordinary C. The initial head
pointer is tested separately, then the walking cursor is initialized only
inside the nonempty branch. This preserves the native head-to-current copy
before the peeled first link read. The subsequent next-link/current copies
already appeared in the earlier ordinary loop.

Qualified SDK 2.9 output retains the genuine branch-local cursor lifetime.
Moving only its initialization before the null test, while retaining both
locals, returns to 140 bytes and 17 of 35 matching words. A narrow head-binding
diagnostic also matched, but removing the binding preserves exact output,
so no compiler constraint is needed or retained.

The checked six-word queue initializes thread ID, active, server list,
pending start/end and next, then appends to the packet-pool header's queue
list at +0x28 through queue +0x14. DIntr's prior-state result is discarded,
and EIntr is called unconditionally at the tail. The function returns void;
sdfPrint's stale signed-return declaration is corrected without changing any
output, local record layout or callback cast.

## sceSifRegisterRpc

Matched all 208 native instruction bytes with the same ordinary head/current
lifetime split. Moving the cursor initialization before the initial null test
returns to 200 bytes and 27 of 50 matching words. No register binding,
artificial dependency, memory qualifier or provider visibility change is used.

The checked 0x44-byte server record retains its handler, buffer, optional
callback/buffer, service ID and owning queue fields. Only native-written
fields are initialized. Registration uses the link at +0x38; pending requests
use the distinct +0x3C link. The owning queue is at +0x40. The native
unconditional EIntr tail call and all memory accesses are unchanged.

Both promotions qualify individually and together. Combined public and local
complete-owner checks report 153 match, 0 differ, preserving all 151 existing
matches. Baseline, ordinary ablation, separate promotions and combined source
pass strict production, SKIP_ASM and retained-C plain/dump full assembly/ELF
parity. All 362 production peer bodies, 664 owned rodata bytes and 98 data
relocations are unchanged. The sdfPrint declaration correction is completely
output-neutral in all three contexts. These are matching reconstructions,
not evidence of historical source spelling.


Independent semantic, ABI and complete-owner review approves both changes.
All 151 prior SKIP_ASM and 156 other retained-C function bodies and resolved
data/relocation targets also remain unchanged. Full retail, generated-source,
regression, guarded-source and report gates pass with both affected owners.
Production provenance verifies unique compiler-owned linked spans for both
functions. Only SDK coverage gains two functions and 356 bytes; game coverage
is unchanged.

## sceSifInitIopHeap

34 of 34 words match at the native 136-byte size. The routine binds service
`0x80000003` synchronously, returns -1 for a negative bind result, and retries
after the native countdown while the client's server is null. The bind ABI,
server field at +0x24, countdown and four native NOPs are unchanged. Only a
successful binding clears the initialization flag and returns zero.

A nested block after the final call holds the actual flag address and then
the zero return value in one `saddr_t` scalar, bound locally to `v0`. It is
initialized in its declaration through `EE_ADDR_S`, consumed by the real
`EE_ADDR_AS(s32 *, result)` store, then overwritten with the actual return.
No live fixed value crosses a call. This uses the existing signed EE address
representation and its matching pointer conversion, without address arithmetic,
comparison, an alias promise or a new input bound. The externally owned flag
remains a four-byte `s32` initialized to -1. No data ownership changes.

The binding is a target-specific reconstruction aid, not historical source
evidence. It emits no additional instruction or empty asm. Independently
removing only the binding restores the three-word tail mismatch: the compiler
separates the flag-address register from the return register and clears the
return before the store. Ordinary address/result reuse, reuse of the earlier
bind result, a pointer-only binding and an empty live-result control also fail.
The unsigned address representation and moving the initialized scalar into
this narrow tail block are complete-output neutral. No signedness trick,
artificial consumer, volatile memory access or memory barrier is involved.

Earlier global-client and loop controls remain closed: keeping one client
pointer emits 128 bytes, while moving the store and return into the loop emits
144 and changes control flow. The retained source keeps each real global-client
use and the original retry structure.

Independent semantic and output review approves the final source. Baseline and
candidate pass strict production, `SKIP_ASM`, and `NON_MATCHING` plus `SKIP_ASM`
compilation, with complete plain/dump assembly, ELF-header, allocated-section,
non-file-symbol and relocation parity. Both owner checkers report 149 match,
0 differ. All 363 other production functions, including Add/Remove handlers,
retain their bytes and extents; all 664 rodata bytes and 98 relocations remain
unchanged. Full retail checksum, generated-source, regression and report checks
pass. The production-proof machinery independently verifies compiler provenance,
receipt hashes, unique object/link ownership and all 136 retail bytes at
0x002E2B10. Exactly one SDK function is newly supplied by verified C; game-code
coverage is unchanged.

The current whole-binary aggregate does not add these 136 bytes: SDK units keep
raw objdiff scores, while production reconciliation is limited to game units.
Raw objdiff gives this function 99.70588% solely because the target disassembly
represents the countdown at +0x18/+0x20 as `%hi(D_FFFFF)` / `%lo(D_FFFFF)` rather
than the compiler's immediate 0x10 / -1. The absolute pseudo-symbol 0xFFFFF
resolves to identical instructions. These countdown relocation encodings also
explain the three raw object-byte differences; the flag-address tail is exact.
No report category, counter, check or target metadata was changed to claim credit.

## sceSifSetIopAddr (sdk/libkernl, 220 bytes)

All 55 of 55 words match at the native 220-byte size. The final RPC status
is captured in an ordinary signed local, then the selected signed return
value is initialized in its ABI result register. This one local register
binding is a target-specific matching aid, not evidence of original source
spelling. No empty operand, extra consumer, new type, layout or compiler flag
is needed. The fixed value is initialized after the last call and immediately
returned; it never carries a live value across a clobbering operation.

The ordinary explicit-status control remains 51/55 at the same size: the
error/threshold setup order and zero/error conditional selection differ.
Binding the actual selected result closes those four words. An additional
empty live operand is unnecessary and was removed. Earlier negative controls
constrained an error arm before selection, rather than the actual selected
return value. Embedding the call in the bound initializer is not equivalent
for matching and was also discarded. Complete plain/dump output parity
qualifies these observations; no hooked compiler-decision claim is made.

Binding failure returns -65536. After a successful bind, the address and kind
header words are written before validating kind. Kinds zero, one and two copy
one, two and four bytes respectively into request offset eight. An invalid
kind returns -65538 after the header writes, without reading the input or
calling RPC. The valid transaction sends 32 bytes, receives 16 bytes, and
returns zero for every nonnegative signed RPC status or -65537 otherwise.
Input can alias the shared request: the header stores remain before the
width-selected input load. No null, alignment, non-alias or provider-return
range assumption is added; native halfword/word alignment requirements remain.

Independent native and ABI review covers the unchanged nine-argument RPC,
all accesses and error paths. Public and local complete-owner checks report
151 match, 0 differ. Strict production, SKIP_ASM and retained-C plain/dump
checks preserve complete assembly and ELF contracts. All 363 production peers
and the 664-byte owned rodata with 98 relocations are unchanged. Full retail,
generated-source, regression, guarded-source and report gates accompany this
source-only landing; private checker changes and raw traces are excluded.

## sceClose

Matched all 380 native instruction bytes. The 20-byte close request contains
completion semaphore, signed 32-bit result pointer/size, native descriptor and
request-table index. Only maximum count, initial count and option are set in
the semaphore descriptor. Submission failure preserves the active slot and
cleans up; success clears it, captures the uncached acceptance before unlock,
and either rejects zero acceptance or waits for completion and deletes the
semaphore. Negative completion results are returned unchanged; others return 0.

The prior ordinary draft emitted 376/380 bytes because the signed clamp folded
to `slti`/`movz`. Direct-result mutation and ternary spellings did not recover
the native tail. As independently evidenced in `sceDclose`, native materializes
signed -1 for a real register comparison. A narrow nonvolatile empty `+r`
constraint on that threshold immediately before `threshold < status` retains
the native `slt`/`movn` sequence without fixed registers, false uses, calls
crossed, memory effects or width changes. Combined with `sceDclose`, the owner
has 145 exact functions; both native tails and all existing data match.
Plain/dump normalized assembly and allocated-section/relocation parity pass.
The same target-specific reconstruction aid is used here; original source
spelling is not established.

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

18 of 18 words match at the native 72-byte size in ordinary C. The four
assignments are first maximum, first initial, second maximum, second initial.
No constraint, qualifier, type, provider declaration or compiler flag changes
are needed. This is a matching reconstruction, not evidence of historical
source spelling.

Both `rpcSemaParam` objects retain their six 32-bit fields, with signed maximum
at +4 and initial at +8. Only those two fields in each descriptor are assigned
one; all other fields remain untouched. Native allocation places the records
at SP+0 and SP+0x20 in the existing 0x50 frame. No padding or extra initialization
was introduced. The unchanged CreateSema syscall 0x40 provider consumes each
actual descriptor pointer and returns its signed result. The first result is
stored to D_0052C378 before the second call, and the second to D_0052C37C after
it, preserving native call and store order.

Qualified SDK 2.9 dumps show the earlier chained source's independent stores
expand at stack offsets 0x24, 4, 0x28, 8, then schedule as 8, 0x24, 4, 0x28.
The ordinary assignment order above expands as 4, 8, 0x24, 0x28 and schedules
as native 0x28, 4, 8, 0x24, with the last store in the first call's delay slot.
This observed ready-list ordering motivated one readable source control;
no hooked comparator mechanism is claimed. Swapping only the first record's
two assignments preserves size but leaves two differing words. The original
chained control remains 14/18. No permutation sweep or artificial dependency
is required.

Independent semantic and full-output review approves the source. Baseline and
candidate pass strict production, `SKIP_ASM`, and `NON_MATCHING` plus `SKIP_ASM`
plain/dump checks for complete assembly, ELF headers, allocated section headers
and bytes, non-file symbols and every relocation. Both complete-owner checkers
report 150 match, 0 differ. All 363 other production function bodies and layouts,
including heap initialization and Add/Remove handlers, remain unchanged, as do
664 rodata bytes and 98 relocations. Independent production ownership and linked
retail-byte proof also pass. Full retail checksum, generated-source, regression,
guard and report gates accompany the landing. Exactly one SDK function and
72 bytes are newly supplied by verified C; game-code coverage is unchanged.

## sceSifInitRpc

412 emitted and native bytes, with 95 of 103 words matching and eight
words differing, first at `+0xA0`. The remaining differences reorder queue
field stores and argument setup before the first command-handler registration.
No additional operation, padding, field volatility or callback ABI change
is justified. Normal builds retain assembly.

The shared packet-pool header initializes its sequence to one, three
32-packet pools to their uncached aliases, the corresponding counts to 32,
and words `+0xC`, `+0x10` and `+0x24` to zero. The queue-list pointer at
`+0x28` remains untouched. The 64-byte packet records and each 2048-byte
pool are already established by the allocation and forwarding providers;
initialization does not clear their packet contents.

Interrupt order is disable, flag check/set, enable, command initialization,
then disable for queue setup and all four handler registrations, and enable.
An existing SIF readiness register skips the handshake. Otherwise only word
three of the cached second packet is set to one before its 16-byte send;
software register zero is polled before setting the readiness register.
The input mode is unused. The typed read-data callback is converted only
at the generic two-pointer registration interface.

Keeping the ready path as an early return restores the native tail call.
A distinct selected-packet pointer preserves the native pool-base lifetime;
mutating the pool cursor left register and packet-address differences.
The three equal pool counts share an assignment. Separate field-order
controls retained 20 or more differing words; the preferred source has the
eight-word store/setup residual. The configured 2.9 compiler has no supported
qualified allocator hook, so a pass-level explanation remains open.

## sceOpen

640 emitted bytes against 644 native bytes; 111 of 160 emitted words match,
49 differ and four native bytes remain uncovered. Most differences exchange
the saved registers holding the path/request and semaphore/reply across phases.
The native successful tail also copies the slot index into a return scratch
before waiting on the slot semaphore; ordinary source retains the slot directly.

The variadic interface saves six remaining argument registers in eight-byte
slots. As in the matched kernel formatters, the configured 2.9 compiler's
`__builtin_next_arg` points after that save area; subtracting six `sizeof(long)`
slots reads the first optional mode word. Native reads it unconditionally,
including for two-argument callers. The graph caller declaration agrees with
this interface; its calls are unchanged.

The 0x418-byte request contains masked flags, mode, a bounded 0x400-byte path
and the reserved slot index. Only an exhausted path copy forces the last byte
to NUL. Version rejection precedes reservation. RPC transport failure and a
rejected reply leave the reservation intact. A negative completion releases
the slot under its semaphore; success stores the native descriptor, ORs the
original flags into the active word and returns the slot index. The uncached
acceptance word is captured before releasing the filesystem semaphore.

A condition-first bounded copy restores native loop peeling and most setup
scheduling compared with the initial combined assignment/increment condition.
Reading the mode after the copy worsens allocation. No synthetic scratch move
or dependency was added for the final slot-return difference.

## _sceFs_Rcv_Intr

948 emitted bytes against 960 native bytes. Of the 237 overlapping native
words, 212 match and 25 differ; twelve bytes remain uncovered. The first
difference is at `+0x344`, in the final pending-semaphore search. All earlier
copy paths and the command jump table match. Normal builds retain assembly.

The callback ignores both arguments and snapshots four uncached completion
fields separately. Nonnegative semaphore values copy the result first.
Command 2 repairs signed-positive head/tail read fragments with byte loops
that preserve length reloads. Command 11 copies the 324-byte directory entry;
command 12 copies the 64-byte status record. Commands 23, 25 and 26 snapshot
an output pointer/count and cap the unsigned count at 1024 before copying.
Other commands use only the common completion path. Negative semaphore IDs
are normalized and remove the first matching entry from the 32-slot pending
table without signaling; nonnegative IDs call `iSignalSema`, including zero.

The shared output record gives the native scratch offsets and 48-byte frame
without padding. Separately decoded output size and source pointers preserve
the native bounded-copy setup. The remaining search keeps the normalized key
in a different lifetime and shares the return epilogue more aggressively.
Returning from inside the match instead of breaking emits 936 bytes and
changes the scan into a pointer loop; dropping the separate search key emits
944 bytes. An early return after the whole negative-ID branch is inert.
The preferred 948-byte source is retained until a genuine SDK 2.9 allocation
or loop-control premise explains this tail. No qualified allocator hook is
available for this compiler; no register pins, volatile scratch or padding
were introduced.

## sceDclose

Matched all 360 native instruction bytes. The typed filesystem request uses
command 10 and a 20-byte send; its slot word is deliberately left untouched.
Submission failure preserves the active descriptor, deletes the completion
semaphore and unlocks. Successful submission clears the descriptor, captures
the uncached acceptance before unlocking, and either rejects zero acceptance
or waits, deletes the semaphore and reads its signed 32-bit completion.
Only native maximum-count, initial-count and option semaphore fields are set.

The ordinary reconstruction emitted 356/360 bytes: its final nonnegative
clamp folded to `slti`/`movz`, shifting four exit branches and the epilogue.
The native tail instead materializes signed -1 and consumes it in a register
comparison followed by `movn`. A narrow nonvolatile empty `+r` constraint on
that real local threshold, immediately before `threshold < status`, preserves
the genuine value and consumer. It binds no register, crosses no calls, and
adds no instruction, memory effect, artificial use or changed result width.
The complete owner advances from 143 to 144 exact functions; plain/dump
normalized assembly and allocated-section/relocation parity pass.

This is a target-specific reconstruction aid, not evidence of the original
source spelling.

## kputchar

The ordinary C reconstruction matches all 56 native bytes. It repeatedly reads
32-bit UART status at `0x1000F130` until bit `0x8000` clears, writes exactly one
byte to `0x1000F180`, and returns the original signed 32-bit argument. Values
outside the byte range are narrowed only for the transmit store. Both MMIO
accesses retain `volatile`; no compiler constraint or instruction helper is used.

The compiler's assembly places `sb` before the return, with its volatile
assembler directives commented out. The configured 2.10 assembler moves that
store into the `jr` delay slot. Removing volatility from only the final store
produces 52 bytes and does not match. This case corrects the earlier blanket
inference that a retail delay-slot store proves a nonvolatile source pointer.
It does not establish historical source spelling or general MMIO ordering rules.

Independent semantic, ABI and complete-owner review approves the exact source.
Both checkers report 155 match and 0 differ. Baseline and candidate pass strict
production, SKIP_ASM and retained-C plain/dump parity for complete assembly and
literal complete ELF bytes, including padding, after normalizing only the exact
randomized source path. All existing peer bodies and resolved data/relocation
contracts remain unchanged. The entire 39,036-byte production text section,
664 owned rodata bytes and 1,773 relocation descriptors are unchanged.

Full retail checksum, generated-source, regression, guarded-source and report
gates pass. Production provenance verifies the compiler-owned linked span.
Only SDK coverage gains one function and 56 bytes; game coverage is unchanged.

## sceTtyInit

The ordinary typed initialization matches all 188 native bytes. It flushes the
cache, registers protocol `0x210` with the existing callback/context, publishes
and reloads the signed handle, and returns zero only for a negative handle.
Success clears the three progress words, selects the two uncached buffer
addresses, initializes the send-packet header, publishes the 256-byte receive
queue and returns one. Packet length, payload and receive-buffer bytes remain
untouched. No additional error handling or allocation is introduced.

Each buffer has a proven 320-byte backing extent. Its typed packet contains the
12-byte header and 308 remaining bytes; this does not change the writer's
256-byte payload cap. Native field widths and offsets agree with the
[DECI2 TTY layout](https://github.com/ps2dev/ps2sdk/blob/master/ee/kernel/src/tty.c),
used only to corroborate field meanings. The native backing extent, not that
reference's smaller buffer, determines the declarations. Character accesses
observe the declared packet's representation without a conflicting array type.

The existing context prefix reaches its queue at `+0x18`. Initializer stores,
relative callback loads and direct reader references independently prove that
`D_0059B6A8` is an interior field label, not a separate C object boundary.
Only the 28-byte prefix is established; the remaining storage is not assigned
invented fields.

The handle and progress words use native-width volatile views of genuinely
published asynchronous context state. The callback reads the handle and
reads/writes progress. No competing handle writer is proven, so the preserved
reload is not described as required by concurrent mutation. Volatile supplies
neither a general memory barrier nor a repaired initialization protocol. The
packet and pointer fields retain ordinary accesses. Constructing the actual
local send-packet pointer after setting the receive pointer gives native
address scheduling without a compiler constraint or helper.

Independent storage, shared-state, ABI and complete-owner review approves the
source. Both checkers report 156 match and 0 differ. Baseline/candidate strict
three-mode complete assembly and literal entire-ELF plain/dump parity passes,
with hashes bracketing every compile. All production code/data and 1,773
relocation descriptors are unchanged; 155 SKIP_ASM and 160 retained peers keep
their bytes and resolved targets. All 13 target relocations are independently
resolved and the resulting 188 bytes match retail without masking.

Full retail checksum, generated-source, regression, guarded-source and report
gates pass, with a unique compiler-owned production span. SDK coverage gains
one function and 188 bytes; game coverage is unchanged. This is a matching
reconstruction, not a claim about original source spelling.
