# Non-matching drafts: newbattle/nbMainProcess

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## nbMainProcess1 (newbattle/nbMainProcess, 0x420)

113 of 263 words match, at the exact 1052-byte size.

The 16-arm jump table over `data->phase` (`s8` at +0x69, `sltiu 0x10`, every
index 0 through 15 a real case) reproduces exactly, and so do cases 0, 1 and
2. The whole remaining difference is one cross-jumping choice in `case 3`.
Retail has four `jal nbSetNextPhase` sites in the `case 3` / `case 9` /
`case 10` region: the `0xB` arm of `case 3` keeps its own call with
`addiu $4, 0xB` in the call's own delay slot, while the `5` arm of `case 3`
and the `0xA` arm of `case 9`'s inner `endtype` switch share one call with
their `addiu $4` in the predecessor's delay slot, and both fall into
`case 10`'s body, which is `next = 1; break;` on its own. ee-gcc either
shares none of them (the `if`/`else` with one trailing `next = 1`, which is
the kept draft: correct size, `case 3`'s else arm four words too long and
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
