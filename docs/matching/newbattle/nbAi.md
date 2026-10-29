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
