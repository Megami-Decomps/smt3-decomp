# Matching notes: pb/pbRecord

## pbAddRecord_char (pb/pbRecord, 0x144)

All 81 words match at 324 bytes. A narrowly scoped initialized snapshot-array
base bound to `$2` closes the eight register-allocation differences in the
post-position-callback coordinate stores. The ordinary indexed base local
remains 73/81; binding the final element address instead reaches 80/81 but
commutes the final address add. Binding the actual array base preserves the
native indexed add without an arithmetic rewrite or an empty asm.

The base is loaded from the live current record slot after
`pbGetMyCharcterPos2`, consumed by the two actual coordinate stores, and dead
before `pbMyCharacterDispStat`. The status assignment still rereads the
record position, slots and characters after that callback. Input entries and
the live global count retain their callback-sensitive reloads. The snapshot
allocation still occurs for a zero character count. The fixed register is
only a target compiler reconstruction aid, not an original-source claim.

Complete owner checking gives 11 match, 0 differ with owned data unchanged;
the remaining block-snapshot draft compiles. Normal and stock `-da` complete
owner builds have equal allocated sections, relocations, target bytes and
normalized assembly. This establishes output parity, not an unmeasured
internal compiler mechanism.

## pbPopRecord (pb/pbRecord, 0x2F0)

188 of 188 words match at 752 bytes using ordinary indexed loops and the real
complete map object's byte representation. All nine previously matching peers
and owned-data checks remain exact. There are no assembly aids, false types,
artificial consumers, constraints, aliases or prototype changes.

The whole-map traversal keeps a separate `wholeMap` snapshot and computes each
cell address from `(u8 *)wholeMap + 0x1C + (i * 20 + column) * sizeof(u16)`.
Both unsigned loop indices range from 0 through 19, so linear indices are
0 through 399 and byte starts are 28 through 826. The final two-byte access ends
at 827, inside the actual 800-byte map member at offset 0x1C. Target-compiler
layout assertions independently verify those facts, the two-byte element size,
and the saved-cell, character, group, record and process layouts. Each address
has the alignment and effective type of the real `u16` element it accesses.
This uses a character pointer into the complete object, not an invented flat
`u16` array crossing row objects or a shifted/fabricated owner structure.

Group and saved-cell traversal use natural indexed `for` loops. Native signed
coordinate loads and promoted sums are retained without invented bounds,
clamps or signedness changes. Saved coordinates and status are read again after
the first map write; counts are reread in loop conditions. Character callbacks,
record and character reloads, map masks and state conversions, conditional floor
restoration, freeing the record and returning the saved current character keep
their native order. No callback-purity, `restrict` or volatile assumptions are
introduced. Valid snapshot arrays and map coordinates remain inherited input
requirements, not guarantees for corrupt records; saved-character union-byte
interpretation intentionally follows the target's little-endian behavior.

Independent guard-removed full-owner checking gives 10 match, 0 differ.
Diagnostic complete-owner plain/stock-dump assembly, text and relocation parity
is verified; this is output parity, not an internal compiler-cause proof.
Final qualification passes the retail checksum, all 149 guarded owners / 585
drafts, seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs checks.
Actual-publication verification retains all 779 tracked files, 21 owner
dependencies and the owner 10/0 result. Source commit `56f840f163958cf92bdcd620399dca066a98cedf`, whose exact-source hosted CI
[40994125386](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40994125386)
passed both build and deploy.
