# Matching notes: newbattle/nbAi

## SetKoudou (newbattle/nbAi, 0x1C8)

110 of 114 words match at the exact 456-byte size. The four differences at
`+0x168` through `+0x174` are the final masked-skill branch and its delay
slots: retail branches on not-equal to the common low-twelve-bit publication,
then materializes skill 222 in the shared index-store branch delay slot. The
draft branches on equal with that constant annulled into the conditional slot.
The other instructions, relocations and entry/exit frame are exact.

The action entry is eight bytes at the sum of the devil's 164-byte row offset,
the forty-byte table offset and the eight-byte entry offset. Sum those offsets
before adding the table base. The skill is unsigned sixteen-bit. Preserve the
initial command/index clears and the second command clear before the normal
skill provider. Codes 0x8000 through 0x8003 select commands 0, 7, 3 and 9.
Other encoded skills set command 1 in each bit-15 branch, not before the test.
High-nibble dispatch selects the mapped skill or event; every high-bit path
publishes the low twelve bits to the summon ID, including unknown kinds.

A full switch with the 0xF000 skill arm before 0xD000 reproduces the shared
index-store layout. An ascending case-body order leaves 27 word differences;
hand-expanded range dispatch is larger in differences. Moving command 1
before the high-bit test also changes the native branch/store layout. Explicit
default returns and an empty default arm leave the four-word residual; a nested
0xF000 test or explicit shared-store label makes it worse. Do not repeat those
without evidence for a different compiler decision.

## SetSingleTarget2 (newbattle/nbAi, 0x26C)

143 of 155 words match at the native 620-byte instruction size. All twelve
differences are the stack adjustment and saved-register offsets: the draft
uses a 0x40-byte frame with saves starting at 0x10, while native uses 0x50
with saves starting at 0x20. Every other instruction matches.

The candidate buffer contains signed-byte formation indices. Native writes
only indices 0 through 15; the extra frame space does not establish a larger
source-level array capacity. The 16-element reconstruction remains guarded
until actual storage evidence explains that difference. The RNG declaration
now agrees with its provider: state pointer, unsigned limit and unsigned
return. This correction leaves the emitted instructions unchanged.

## Sel_AI (newbattle/nbAi, 0x358)

214 of 214 words match at 856 bytes in ordinary C. Separate compact-output
index and accepted count have independent real consumers: score-array
addressing and the later scan bound. The index form compiles
to the native advancing-pointer code. A direct-pointer control is semantically
correct and 856 bytes but matches only 208 of 214 words because initialization
is scheduled earlier. No constraints, asm, dummy uses or invented bounds are
needed. The 32-word score buffer is not gratuitously initialized; actual
formation loops scan 16 records of the existing 240-byte type.

The local nbGetVirtualAisyo declaration now truthfully returns u32, matching
its provider. An explicit s32 selector preserves native signed sparse dispatch,
rejecting exactly 0x10000/0x20000/0x40000. Command 0 selects skill zero, command
1 the actual work skill; other commands print and return zero. AI level 0
marks every current bit-4 formation, unsupported levels return zero. Level 1
retains all signed scores, including zero/negative. Level 2 skips the three
affinities and zero scores but retains negative scores. Formation and battle
data are freshly observed across callbacks, including between affinity and
score calls; no stale pointer is cached.

If no score is accepted, the function prints a diagnostic and returns -1
before group handling. A nonzero
group marks all current bit-4 entries, not merely accepted-score entries.
Single selection starts best=0/index=0 and uses strict greater-than, preserving
first ties and index zero when all scores are nonpositive. The native compact
rank is mapped through all current bit-4 formations without repeating affinity
or zero-score filtering; this can select an earlier rejected candidate and is
intentionally not repaired. The native branch delay slot also increments count on the selected path,
where that updated value is unused. The C need not add an increment before
its selected-path break; its ordinary loop already compiles exactly. These
findings do not establish original source spelling.

Independent complete-owner verification gives 47 match, 0 differ, preserving
all 46 peers, layouts and owned data. Full local gates passed for source
`b953764bb48d99d9e97febabe7b3950b3984221e`. Exact-source CI
[41053166184](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41053166184)
completed successfully, including every build/deploy step and retail checksum.
