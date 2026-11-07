# Matching notes: sdk/libgcc

## __do_global_ctors (sdk/libgcc, 0xC0)

All 48 words match at the native 192-byte extent in ordinary C, with the
configured ee-gcc 2.96 compiler and no constraint or flag override.

The dispatcher registers the exception-frame metadata first, decodes the
constructor-list header, and scans from entry one to the NULL terminator when
that header is -1. It invokes the entries in reverse order and registers
__do_global_dtors with atexit. The retail constructor list is empty, but the
source retains the general algorithm and callback reload order.

The count is an unsigned long, which is 64 bits under this EE ABI. The backward
index is an unsigned int, which is 32 bits. The four-byte function-pointer
header is decoded through s32 before conversion to the count, preserving the
native sign-extending load and sentinel representation without a pointer-width
warning. These conversions are target-specific runtime representation, not a
portable function-pointer serialization API. No constructor-array bound is
inferred from the larger startup-data aggregate. D_0059AD40 is the existing
24-byte frame-registration object, described by the existing object type.

Two width phases are observable in qualified compiler output: the forward
scan uses a DI induction variable and DI byte-offset reduction, while the
reverse callback loop uses an SI index and SI address reduction. A minimal
32-bit-count control emits 184 bytes, losing eight native bytes and matching
11 of its 46 emitted words at corresponding positions.

The frame-registration provider and its forwarding __register_frame wrapper
return void. Their previous signed-return declarations were unsupported by the
provider's valueless return, its runtime API, and the recovered callers. With those stale
contracts, the constructor emits 192 bytes but leaves seven allocation words
different. Qualified RTL represents the old call as call_value, setting v0;
the truthful void call removes that false result and gives the native table
address, base and sentinel allocation. Existing provider statements, accesses,
semaphore calls and tail-call barrier are unchanged. The unrelated frame-table
registration API is not rewritten.

The constructor itself returns void. Its sole recovered direct caller is
__main; the frame-info provider is called by the constructor and forwarding
wrapper. Native direct-transfer, literal-address and source audits support
these contracts without claiming to exclude every dynamically constructed
indirect address. Incidental return-register contents from SignalSema are not
a promised frame-registration result. Stale constructor/frame-info declarations
in libc, sdfSortOt and sdfTmx are corrected consistently.

Declaration/provider-only repairs are complete assembly/ELF neutral in all
three modes for all four affected owners. The final constructor and both
minimal controls pass strict production, SKIP_ASM and retained-C plain/dump
complete-output parity. Both complete-owner checkers give libgcc 37 match,
0 differ; the other repaired owners give libc 78/0, sdfSortOt 10/0 and sdfTmx
7/0. All existing peer functions, owned data and resolved relocation targets
remain unchanged. Full retail and integration qualification accompanies the
landing.

The source is independently expressed from native behavior. Existing
sdk_objects.tsv identifies the runtime archive member as ee-gcc 2.96 __main.o;
this pass does not claim a fresh archive comparison. GCC 2.95's
[gbl-ctors.h](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95/gcc/gbl-ctors.h#L71-L81)
corroborates the two-width count/index idiom, and
[libgcc2.c](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95/gcc/libgcc2.c#L2715-L2720)
corroborates the void wrapper and exit registration. Those references are
historical leads, not proof of Sony's exact patched source or original spelling.

## __muldi3 (sdk/libgcc, 0x74)

The assembly-only helper returns the low 64 bits of the product of its two
64-bit arguments. A private ordinary-C reconstruction uses four unsigned
16-by-16 partial products for the low-word product, two unsigned 32-bit
cross products, and unsigned carry propagation. It never multiplies two
64-bit C operands. This matters because that apparent simplification lowers
to a recursive call to the very helper being reconstructed.

The limb control is a call-free leaf under the configured compiler, but emits
136 bytes against 116 native bytes: 0 of 29 words match within the native
extent, with 20 excess bytes.
All 37 existing matching peers and 240 bytes of owned rodata are preserved.
The source remains private and the assembly fallback remains unchanged.

Independent arithmetic review proves the modulo-2^64 result for all input
bits and bounds both carry stages. Native dpmul and _strtol_r consume the
full-width result. Actual-body tests pass 1,000,100 cases in each optimized
and ASan/UBSan run; an independent UBSan oracle passes 1,008,388 cases. Leak
checking was disabled for the ASan run because of the execution environment.
These are source-semantic checks, not native execution or a matching claim.

The remaining problem is a justified nonrecursive widening lowering. The
native insertion sequence also carries dead incoming register bits that the
ordinary body does not fabricate. Do not repeat the closed recursive widening
control, invent that state, or replay scalar instructions to claim a match.
