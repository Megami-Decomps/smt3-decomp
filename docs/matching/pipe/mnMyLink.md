# Matching notes: pipe/mnMyLink

## pipeMakeNodeLink (pipe/mnMyLink, 0x80)

25 of 32 words match at the native 128-byte size; the draft remains guarded.

The guarded ordinary reconstruction remains 25/32 words at the native 128 bytes; production retains assembly and gains no matching credit. Increment nextId through `(s32)((u32)id + 1U)` to represent native 32-bit wrapping without signed-add overflow. The real u16 node ID still receives the old low 16 bits and nextId retains all 32 bits. Out-of-range unsigned-to-signed conversion is EE/compiler-defined, not a portable ISO C guarantee. Valid live info/handle/node allocation and maxNum within its capacity remain required; initialization at 1 does not prove the cumulative counter never reaches INT_MAX. Complete-owner output is unchanged by this arithmetic-only edit in production, SKIP_ASM and NON_MATCHING+SKIP_ASM modes.

Closed bounded investigations, each at 128 bytes and preserving the 28 existing exact C peers:

- Ordinary baseline and explicit-wrap counterpart: 25/32 each.
- Bind the actual initialized nextId pointer to native r4 in the existing free-node arm: 20/32. Final pointer definition is hoisted before the scan, but first scheduling still sees an arm-local hard-register transfer; local ID allocation displaces the node.
- Explicit post-provider pointer placement, ordinary then r4-bound: 21/32 and 17/32. The transfer is outside the arm before scheduling, but node/ID interference persists and pointer setup occurs earlier than native.
- Reuse the dead scan scalar for the ID on the terminal success arm, ordinary then r4-bound: 25/32 and 23/32. Qualified dumps show genuinely shared scan/ID pseudo ownership and restore node r5, but the separate increment result occupies r3 and forces the shared scalar to r6.
- Update that shared scalar in place after storing the old node ID, ordinary then r4-bound: 19/32 each. Qualified dumps show scan, ID and successor share one pseudo, removing the separate-result conflict. Global allocation still assigns node r3 before shared scalar r5, leaving a node/scalar swap.

Whole-owner plain/dump output parity was verified for the ordinary and bound investigations. These findings describe this compiler/source context, not original source spelling. No failed binding or phase-reuse rewrite is retained; the family is closed. The arithmetic-only guarded cleanup is semantic hygiene, not matching progress, and has independent arithmetic-only review approval. Final owner 28/0 and target-only 25/32 checks pass. All three strict modes and plain/dump comparisons, full retail checksum/byte comparison/no-op, 450 tests, 817 layout assertions, fresh stable 141-owner/481-draft checks and seven IRX checks pass; reports remain unchanged.

There is no uniqueness or nonzero-ID guarantee: low 16-bit IDs repeat every
65536 counter values, and a low-16-zero allocation remains discoverable as
free later, as in native code. No clamp or new capacity bound is introduced.


## LoadLinkData (pipe/mnMyLink, 404 bytes)

101 of 101 words match, through the return delay slot at 0x002A68C8; the
following alignment nop earns no function credit. The existing guarded draft
emitted 412 bytes with broad differences. The reconstruction promotes only
this function; the closed `pipeMakeNodeLink` draft and all other peers remain
unchanged. Both complete-owner checkers now report 29 match, 0 differ.

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
