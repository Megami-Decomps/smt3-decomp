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

## Flow1_22 (newbattle/NbNegoFlowOne, 324 bytes)

81 of 81 words match in ordinary C. Separate real rejection arms retain the
shared loop tail: one unsigned-halfword flag snapshot tests presence, enemy
status and mask 0x38, followed by selected-party exclusion. Unit lookup and
talk eligibility precede the unit-ID and master-table reads. The loop scans
party indices 4 through 13, reloads negotiation data each iteration and stores
only accepted indices. Count is bounded by ten; every randomly selected byte
was initialized. Empty lists do not call the RNG or change the selected party.
The two candidate-ID appends, default selection and callback-visible reloads
remain in native order. An explicit halfword conversion proved unnecessary
and was removed from the final source.

The live candidate list uses 32 bytes. Native storage begins at sp+0 and saved
registers at sp+0x20 in a 0x60 frame. Matched CheckKoketaNego already uses a
32-byte candidate list for the same family of ten-slot randomized selection.
That sibling scans formations and records mapped party indices; this function
scans parties directly. Actual party backing remains fourteen records, and
the loop bound stays fourteen. The family convention and native placement
support this ordinary capacity choice without proving the original array
bound or spelling. Unused bytes could also have been padding in the original.
No separate unused object or unused-local exception is invoked.

With the same rejection arms and a ten-byte list, only seventeen frame/save
offset words differ; all other instructions and the 324-byte extent agree.
This is a source/output observation, not an instrumented compiler-pass claim.
The existing RNG declaration is unchanged. Its null-state, positive-count call
here agrees with the actual EE provider contract; this does not establish a
general corrected signature for other calls.

Independent actual-body optimized and ASan/UBSan tests each pass 897,280 cases,
covering every flag pattern, eligibility subsets, exclusions, legal RNG
selections, status/type fields, append counts and callback substitutions.
Physical fourteen-party objects, ten-byte reference storage and canaries are
preserved. These are host semantic tests, not whole-game execution or a claim
that invalid pointers or indices are safe.

Complete owner: 41 match, 0 differ. All forty peers and owned data remain
unchanged in production, SKIP_ASM and retained-C contexts. Six strict
baseline/final plain/dump pairs compare complete assembly and entire raw ELF
files literally, using identical source/output paths and the actual wrapper
arguments. Results also equal ordinary-wrapper output. All 5,566 resolved
inputs remain stable, including the compiler-directory symlink and runtime.
Earlier pathname-only metadata and advisory-claim snapshot differences were
preserved separately and superseded by the final literal qualification.
