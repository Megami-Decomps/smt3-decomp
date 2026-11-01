# Matching notes: newbattle/NbNegoFlowOne

## nbFlowGetUnitEntry (newbattle/NbNegoFlowOne, 0x384)

225 of 225 words match at 900 bytes in ordinary C. Explicit case-1 skip and
five separate constant-mask cases recover real native rejection arms; nested
positive guards followed by a common zero return preserve branch structure
instead of a branchless Boolean shortcut. An ordinary if/else initializes
attributes to 0x14 for work flag bit 4, otherwise to the actual table word.
The later direct flag expression retains the native full-word snapshot and
second mask test. Initialized-default and ternary controls change emitted
CSE/allocation. No constraints, asm, artificial consumers, false types,
padding, changed flags or owner reassignment remain. Exactness establishes
behavior and emitted shape, not original source spelling or an untraced
compiler-pass mechanism.

Unsigned dispatch handles exactly 0x80000000/1/2; all other values take the
normal path. The first special case queries moon once, rejects nonzero and
only then reads the other unit's ID/race, observing callback changes. The
second loads u16 levels, widens before subtraction by 10 and calls RNG(NULL,100)
only if its level guard permits; signed result <70 accepts. The third rejects
nonzero data before any ID/table read, otherwise tests the real attribute bit.

The normal path initializes attributes even for zero type. Its unsigned shifts
test bits 0..30; bit 1 and unknown bits are no-ops, while the five masks
2/4/8/16/32 each require their matching attribute. Bits 64/128 preserve signed
widened level +/-10 comparisons without overflow over the u16 domain.
Bit 256 checks work flag, other flag, then race equality in native order.
There are no calls or stores on this path, so the repeated nonvolatile work
flag expression preserves its one native snapshot. Bits 512/1024 require
IDs 0x8F/0x14. All exits return normalized zero or one; no bounds or new
invalid-input guarantees are added.

Independent review compared target instructions against the checksum-verified
retail ELF and reproduced complete-owner 39 match, 0 differ, preserving all
38 peers and owned data. Integrated retail checksum and full regression,
guarded EE/IRX and report gates passed for source commit
`6fcee69e0072b358992d4d4cd1ebda371a64e28b`. Exact-source hosted CI
[41036629572](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41036629572)
completed successfully, including actual retail verification and all build/deploy steps.
