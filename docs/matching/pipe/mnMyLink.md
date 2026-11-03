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
