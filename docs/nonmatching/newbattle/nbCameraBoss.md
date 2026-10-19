# Non-matching drafts: newbattle/nbCameraBoss

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## nbCameraBoss_Cut1_Run (newbattle/nbCameraBoss, 0x1DC)

33 of 117 words, two words short of retail's 119.

The dispatch is a `switch` on `effBTLModelGetID` of the invoke formation whose
arms each run one `nbCameraBoss_*_Cut1_Run`. Retail does that as `jal callee`,
then `b` to one shared epilogue with `ld $16` in the delay slot, with the result
left in `$2` and never copied; the default arms put `daddu $2, $0, $0` in a
compare's delay slot.

Two facts got the draft from 59 words to 33:

The arms must only call, not assign. `return callee(pCamera);` tail-calls every
arm (30 words over) and `ret = callee(pCamera); break;` with one trailing
`return ret;` allocates `ret` to `$3`, costing a `daddu` per arm. Dropping both
and letting the function fall off the end of the `switch` leaves the callee's
value in `$2`, which is what retail returns.

The last arm's failing guard must fall into `default`, not run its own
`return 0`. With `case 0x158: ... else { return 0; } break; default: return 0;`
the `skill` local lands in `$4` and every compare against it shifts; writing the
arm as two `if (...) { call; break; }` tests followed directly by the `default:`
label puts `skill` in `$3` as retail has it. A nested `switch (skill)` for that
arm also frees `$3` but runs three words over.

What is left is two branch delay slots. On the tests whose arms call
`BSARI_MASEHA_Run` and `Lusi_Cut1_Set`, retail leaves a `nop` and uses `bnel`
where the draft fills the slot with the following `addiu $2, $0, <const>` and
uses `bne`. Those two callees and `Pish_MUHENK_Run` are the only arms still
`INCLUDE_ASM`, and this is the branch-annul signature of the "a caller can need
its callee's definition" row in [matching-levers.md](matching-levers.md): the
remaining two words should close on their own once those three are C. Permuting
the caller does not reach it. Ruled out at 33 words: forward prototypes for the
three assembly callees and removing the two that exist; `return callee(...)`
with the sibling call blocked by an address-taken dummy local (identical
output); early-`break` instead of `else if` in the `0x123` and `0x102` arms;
`switch (skill)` for those two arms (six words worse).

The case bodies are laid out in retail as `{0x103, 0x124}`, `0x123`, `0x102`,
`0x125`, `0x108`, `0x158`, and that order is what the draft uses. The `0x123`
and `0x102` arms share their `BSARI_MASEHA_Run` and `BSARI_SIMANO_Run` calls by
cross-jumping, which the draft reproduces.

## nbCameraBoss_Lusi_Cut1_Set (newbattle/nbCameraBoss, 0x1F4)

21 of 125 words, at the exact 500-byte size.

Two independent differences remain, and nothing in the rest of the function
differs:

The blend factor's bits are allocated to `$3` where retail uses `$4`, and the
switch value to `$2` where retail uses `$3`, so the whole allocation is shifted
down by one. That also shows up as `qmtc2.ni $3` instead of `$4` in all four
`SDF_VU0_INTERPOLATE` sites. Retail also duplicates the `mfc1` into both arms
of the clamp (`mfc1 $4, $f1` before the `bc1f`, `mfc1 $4, $f2` on the
fallthrough) where the draft's `if (rate > 1.0f) rate = 1.0f;` gives one
`mov.s` and one `mfc1`. Writing the clamp over the bits instead
(`PS2_FLOAT_BITS(bits, rate); if (rate > 1.0f) PS2_FLOAT_BITS(bits, 1.0f);`, or
the same as an `if`/`else`) does produce the two `mfc1`s but reorders the
divide and the 1.0 materialisation ahead of them, which is worse: 106 to 114
words differing instead of 21.

In the first of case 0's two lerps ee-gcc rotates two fp registers, emitting
two `lwc1` and two `swc1` at a time where retail loads all six constants and
then stores all six. Case 1's identical block comes out with retail's six and
six, and so do the same blocks in `nbCameraDANTE_DCOMBO_Set` and
`nbCameraDANTE_DCOMBO_KAGU_Set`, so this is pressure local to that one arm.

Ruled out: reading `nbCameraBoss_BSLUSI_Type` before the divide, between the
clamp and the `PS2_FLOAT_BITS`, and after it; declaring `type` before `bits`;
and a named `one` local for the 1.0 comparand.

## nbCameraBoss_Cut2_Run (newbattle/nbCameraBoss, 0x230)

92 of 138 words, two words short of retail's 140.

The same jump-table dispatcher as `nbCameraBoss_Cut1_Init`, on `id - 0x101`
over 0x58 entries, and written the same way: each arm is one or two
`if (skill == <n>) { callee(pCamera); break; }` tests followed by a `return 0`
or `return 1`, the arms only call, the function falls off the end of the
`switch`, and the `0x158` arm's failing guard falls into `default`. Every
difference is a displacement shift from one block.

The `0x11F` arm is the one block that is wrong. Retail reaches the shared exit
from both of its `return 1` paths with `addiu $2, $0, 1` in a branch delay slot
(`beqz $4, exit` for `skill == 0`, then `bnel $4, $2, exit` annulling a
`daddu $2, $0, $0` for `skill != 0xA5`, then `b exit` with the 1 again).
`if (skill == 0) return 1; if (skill == 0xA5) return 1; return 0;` and
`if (skill == 0) return 1; if (skill != 0xA5) return 0; return 1;` both let
ee-gcc if-convert the pair into `xori`/`sltiu`, four words short.
`if (skill != 0 && skill != 0xA5) { return 0; } return 1;` and a two-case
`switch (skill)` returning 1 both keep the branches but merge the first
`return 1` into a local block that the `beqz` jumps to, leaving its delay slot
empty and the `0xA5` constant in `$3`: two words short. The missing fact is
whatever makes that first `return 1` a bare jump to the shared exit.

The jump table differs as a consequence; the case set itself is confirmed by
the default entries, which all resolve to the one default block.
