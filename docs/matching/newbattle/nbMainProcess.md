# Matching notes: newbattle/nbMainProcess

## nbMainProcess1 (newbattle/nbMainProcess, 0x420)

113 of 263 words match in the 1052-byte emitted prefix. The native function
is 1056 bytes (0x420, 264 words), from 0x00248E38 through 0x00249258; four
bytes remain uncovered. The earlier description of 1052 bytes as the exact
native size confused the emitted prefix with the native extent.

The dispatch range is correct: `data->phase` is `s8` at +0x69, guarded by
`sltiu 0x10`, with a real case for each index 0 through 15. This does not mean
the table entries match. At retail table address 0x005434D0, the compiled
case-target addresses differ in eight of sixteen entries (cases 4 through 10
and 15). Cases 0, 1 and 2 are within the matching entry prefix. The
earlier source-shape investigation identified a call-tail sharing mismatch
in `case 3`; it did not establish that this is the sole remaining difference.
Retail has four `jal nbSetNextPhase` sites in the `case 3` / `case 9` /
`case 10` region: the `0xB` arm of `case 3` keeps its own call with
`addiu $4, 0xB` in the call's own delay slot, while the `5` arm of `case 3`
and the `0xA` arm of `case 9`'s inner `endtype` switch share one call with
their `addiu $4` in the predecessor's delay slot, and both fall into
`case 10`'s body, which is `next = 1; break;` on its own. ee-gcc either
shares none of them (the `if`/`else` with one trailing `next = 1`, which is
the kept 1052-byte draft: `case 3`'s else arm four words too long and
everything after it shifted) or shares all three (any spelling that
duplicates `next = 1` into both arms, which comes out four words short).

Ruled out: `next = 1` duplicated per arm, placed before the call, placed
after it, and with a `break` in either arm; the two arms as a `switch` over
`endtype` with `case 1`/`case 8` and a `default`; and the inverted test with
an early `break` in the `5` arm. All four of those produce the same
259-word over-merged shape. The case order in the draft is the body label
order of the retail jump table (0, 1, 2, 3, 5, 4, 6, 7, 8, 9, 10, 15, 11,
12, 13, 14), which is what gets the first 113 words; ascending order is
worse. `limit` in `case 9` has to be assigned in both arms of an explicit
`if`/`else` around `nbCheckActionStack()`, or it lives across the call and
takes a third saved register. `data->unk118` and `data->unkC8` are `u8[4]`
in `nb.h`, so they have to be read through `*(s32 *)`, not compared
directly.

### Same-symbol phase-provider visibility control

A bounded complete-owner control called the real `nbSetNextPhase` through a
distinct external C identifier with the same assembler symbol. Its type remains
`void(s32)`, with no added attributes, and the exact ordinary provider remains
in the owner. All 21 target call expressions retain their original arguments,
ordering and linker destinations. No provider demotion, unit-boundary change
or compiler-flag change was used.

The control does not improve matching: both forms retain 113 matching and
150 differing emitted-prefix words at 1052 bytes, with the same eight of sixteen
jump-table entries differing. It is not byte-inert: two branch-annul bits and
one unconditional branch target change in the case-9 region at target offsets
0x260, 0x268 and 0x270; all three words remain nonmatching. The case-3 call-tail
sharing mismatch is not repaired. All 43 existing exact C peers remain exact;
the two controls have identical symbol inventories, ordered relocations and
allocated data. Only those three text words change.

Plain and diagnostic builds agree for each form in both the ordinary assembly-
fallback context and the C-only comparison context: allocated-section contents
and attributes, all non-file symbols, ordered relocations and normalized
assembly. The initial RTL drops the 21 `REG_EH_REGION` notes on the phase
provider calls, establishing that the visibility control was exercised. After
excluding source/debug notes, that EH metadata and the assembler-name spelling,
the instruction RTL agrees after the second jump pass and machine pass; the
three branch changes appear in the subsequent delay-branch output. These are
parity-qualified pass snapshots, not a live trace of the internal decision.
The unchanged score alone is not evidence that visibility had no effect.

The control is rejected and the original guarded source is retained. Do not
repeat alias-name, declaration-placement or ordinary source-order variants
without a different concrete premise.
