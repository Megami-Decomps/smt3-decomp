# Matching notes: pipe/mnMyLink

## pipeMakeNodeLink (pipe/mnMyLink, 128 bytes)

32 of 32 words match in ordinary C. After storing the old ID's low halfword,
the initialized local ID is updated in place with `(s32)((u32)id + 1U)` and
then published to `nextId`. The separate successor-expression control remains
25/32 at the same size. This is an ordinary update between two real consumers,
not disjoint-phase reuse or a compiler constraint. No binding, dummy use, false
type, new bound or compiler option is added. Earlier scan/ID phase-reuse trials
remain rejected; they are not part of this reconstruction.

The three inherited memory-provider declarations now match the canonical
`sdfMemHandle_t` contracts for GetPtr, AllocDirect and FreeDirect. These
corrections are byte-neutral for all existing peers and the target. GetPtr is
called once before reading signed `maxNum`, so effects through reachable state
remain visible. Nonpositive counts return null without node access. Positive
counts select the first zero halfword ID or return null after exhaustion.

Success clears `next`, stores the old low 16 bits, writes the full modulo-32
successor to `nextId`, clears `prev`, and returns the selected node. Parts ID,
padding and other nodes remain untouched. The unsigned addition avoids signed
overflow; conversion back to signed relies on the pinned EE compiler's
bit-preserving implementation-defined behavior. IDs need not be unique or
nonzero: a zero low halfword leaves the slot discoverable on another call.
Native word/halfword store widths and ordering are unchanged.

Defined-C reasoning requires live, aligned, compatible info/handle objects and
node-array backing for every visited element and any one-past pointer formed.
It does not promise arbitrary incompatible object overlays or invalid-address
behavior. An early success need not visit all elements implied by `maxNum`.
The signed scan counter reaches but does not increment beyond `INT_MAX`.

Independent actual-body fixtures pass 1,247 cases in both optimized and
ASan/UBSan builds: all hole positions, exhaustion, signed count boundaries,
ID wrap/truncation, repeated zero-ID allocation and provider mutation. Host
pointer layout differs from EE; target output establishes the actual 16-byte
node layout and 32-bit pointer stores. Host tests do not certify EE offsets.

Complete owner checks report 30 match, 0 differ, preserving LoadLinkData's
404 bytes and every other peer. Baseline, corrected ordinary control and final
have nine strict production/SKIP/retained plain-versus-dump comparisons, each
literally equal in whole assembly and object ELF. The fixed-scratch compiler
route is checked against the canonical wrapper, differing only in `.file`
path. All 29 peers, resolved call/addend contracts, function inventories and
owned data remain unchanged in every mode. Production raw text equals the
fallback baseline; retained target output is invariant. All 282 frozen actual
inputs, including resolved compiler, assembler, QEMU and runtime, were stable
during the initial qualification. Upstream integration rechecks the affected
input dependencies and complete owner before publication.
These observations establish the source/output relation, not historical
spelling or an unobserved internal compiler decision.


## LoadLinkData (pipe/mnMyLink, 404 bytes)

101 of 101 words match, through the return delay slot at 0x002A68C8; the
following alignment nop earns no function credit. The existing guarded draft
emitted 412 bytes with broad differences. That landing promoted only
this function; the then-guarded `pipeMakeNodeLink` draft and all other peers
remained unchanged. Both complete-owner checkers now report 29 match, 0 differ.

The eight-byte source supplies a signed count and data pointer. The real
caller constructs it from loaded file-header words, receives the four fields
of the 16-byte output info record, and releases the temporary stream only after
return. Nodes are actual 16-byte allocations, with a halfword ID and parts ID
followed by next/previous pointers. The function counts the encoded stream,
creates nodes, decodes each byte and connects consecutive nodes. Its mode
argument is unused and its native return is always one.

Byte decoding preserves the native signed-byte conversions, zero-to-six
selection and turn flag. Reverse direction also wraps through the target's
signed-byte representation; its incoming-direction shift is explicitly
unsigned. No validity bound excludes malformed byte values. Exhaustive checks
cover all 256 first-node byte values and 65,536 code/reverse combinations.
First-node ID one and parts bit eight, halfword store widths, call order and
last-node ID 0xFFFF remain intact. Native allocation/null behavior is unchanged:
with one node, `last` stays null and the terminal write faults. The existing
`FIX_BUGS` alternative remains opt-in; it is not part of the matching path.

The initial stream base and advancing cursor have real, separate consumers.
The cursor is initialized from the actual source pointer before use, reads the
first byte and then continues from base plus one. One local binding keeps it
in callee-saved register 17 across the real count/allocation/link calls. It has
no artificial consumer or uninitialized path, and no empty asm is required.
Removing exactly that declaration's binding emits 396 bytes rather than 404.
Distinct first-node and loop byte scopes preserve their actual decode roles.
This is a localized compiler-allocation aid, not historical source spelling.

Independent review qualifies candidate, baseline and the one-declaration
ordinary control in production, `SKIP_ASM` and retained-C modes. Complete
assembly and literal entire-ELF plain/dump comparisons pass with stable
119-input hash closure. Every native target word is compared after resolving
all five real calls. All actual owner relocations are R_MIPS_26, with their
original destinations verified; no relocation mask hides changed targets.
All 29/28/29 prior peers, owned allocated data, NOBITS layouts and section/symbol
contracts are preserved. The previously nonmatching allocator is not credited.

Full retail checksum, generated-source, 569 regression-test, guarded-source,
IRX, inline-asm and report gates pass, with a unique compiler-owned production
span. Game coverage gains one function and 404 bytes; SDK totals are unchanged.
