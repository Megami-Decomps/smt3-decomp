# Matching notes: sound/sndManager

## dds3TitleDrawFade (sound/sndManager, 0x4F8)

318 of 318 words match: 1,272 emitted and native bytes at `0x00226EC0`.
The complete owner check passes all 14 C functions and its owned data.
Source commit: `224abeee0e9ef9e6367c74bc6cf534de158535db`.
The exact-source local gates also pass: retail checksum/provenance, no-op build,
149 guarded owners (612 drafts), seven IRX modules, `test_cc`, and 26 Python
tool suites (427 unittest cases).
Hosted [CI run 40949890330](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40949890330)
passed for this exact source commit, including build verification, source
reports/scope checks, site build and deployment.

The ordinary C uses one live `age` scalar for the successive clamped and
periodic time calculations, and one `alpha` scalar for the initial box opacity
and later sprite opacities. Each value has a real draw consumer; there are no
constraints, dummy operations or dead computations. Color packing remains an
inline `((u32)alpha << 24) | 0x00808080` argument. The counter increment uses
explicit unsigned wrapping before conversion back to `s32`.

The two animated sprite phases have independent guards. After the first draw,
reload `titleWork->unk_30` before testing the second phase: a callback can change
both the pointer and counter. Preserve the intervening calls and texture loads.
Within each periodic branch, `age -= 0x78; age %= 0x42;` retains the native
subtraction/remainder def-use chain rather than a separate remainder scalar.

The first periodic rise is `0x19 + age * 0x66 / 0x13`. The old non-matching
source draft incorrectly used `0x33`; retail's shift/add chain computes
`3 * age * 17 * 2`, or `102 * age`. This corrects the draft to retail behavior,
not the game. The other phase's `0x33` multiplier is distinct and remains intact.

The already-matched `dds3TitleDrawMenu` sibling supplied useful examples of
real scalar reuse, signed arithmetic and packing, but is a separate existing
match. Its retained dead division was not copied: this target needs none.

## dds3TitleUpdateFade (sound/sndManager, 0x278)

158 of 158 words match at 632 bytes in ordinary C. The first alpha-calculation
frame observation is placed inside each exclusive mode branch, so exactly
one of those two reads executes. The earlier sentinel test and later frame
reloads remain distinct. No constraint, binding, extra executed observation,
volatile access, endpoint substitution or artificial dependency is added.
Mode and frame observations have no intervening callback; the existing
render-state calls and subsequent callback-sensitive reloads are preserved.

All three frame scaling expressions use `(s32)((u32)frame << 7)` before signed
division, and the final counter increment wraps in unsigned arithmetic before
conversion back to `s32`. Thus no signed-multiplication overflow or invented
nonnegative-frame assumption is needed for the upper-only clamps. These
conversions describe the pinned target compiler and native low-word behavior,
not an unrestricted portable-C conversion guarantee. Real alpha/color and
frame consumers, floating endpoint expressions, sprite parameters and calls
are unchanged by the branch-local observation control.

Qualified exact-source RTL retains the alternate branch's real frame load
through sched1. In that block the load and clamp constant issue at modeled
cycle 0, the trap constant at cycle 1, and compare/trap at cycle 2, followed
by conditional clamp, shift and division at cycles 3/4/5. This exposes the
actual two-cycle load availability rather than a guessed scheduler ranking
rule. Allocation then uses the native compare and two constant registers.
The later delay-branch pass moves that frame load into the mode branch's
delay slot and removes the duplicate fallthrough load, preserving one dynamic
read while producing the native common-load arrangement.

The earlier defined-wrap source had six differing words at the exact size;
these are repaired by the real branch-local load timing without any diagnostic
constraint. The division's constant and its zero-trap constant remain real
compiler consumers; no dead initializer was invented to force duplication.
This is measured reconstruction evidence, not historical source spelling.

Canonical complete-owner checking gives 15 match, 0 differ, preserving all
14 peers and owned data. Plain and exact-source diagnostic assembly agree
excluding only the first `.file` metadata line; raw assembly identity is not
claimed. All six allocated sections, 143 non-file symbol contracts and 197
ordered relocations agree, with addends covered by the compared section bytes.

Independent review accepted the exact source and its defined wrapping arithmetic.
Final local gates passed: retail SHA-1, all 427 tests, 145 guarded owners /
514 drafts with stable inputs, all seven IRX checks, and reconciled reports.
The primary report now records 8,560 / 9,045 functions and 1,465,680 /
1,851,228 code bytes (79.173392%). Source commit
`ee02c5c97038af410de1d59c9d9a6b91587fc4ab` passed exact-source hosted
[CI run 41278901834](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41278901834),
attempt 1, including build and deployment. Raw logs confirm this exact checkout,
tooling, guarded EE/IRX checks, retail checksum, report scope and uploads; the
deployment identifies this commit and reports success.

## titleAllocWork (sound/sndManager, 0x110)

68 of 68 words match at 272 bytes in ordinary C, without compiler constraints.
The allocation, texture setup, mode-to-cursor mapping, movie/fade calls and
process-start tail retain their native behavior and truthful provider types.

The previous 57/68 draft had three distinct scheduling differences, not just
three adjacent zero stores: seven initial zero stores rotated by one, the
22/128 constant materializations swapped, and the final two zero stores
reversed. Left-rotating the seven independent initialization statements gives
64/68; reversing the final zero statements gives 66/68. These remain ordinary
stores of the same widths to distinct fields, with no intervening observer.

After the movie call, a local `state = titleWork` captures the actual freshly
loaded work pointer for the four following field stores. Native code likewise
loads it once after that call. The live `alpha = 0x80` and `nextMode = 22`
locals initialize the existing alpha and mode fields; neither value crosses
a call or gains an artificial consumer. Declaring alpha before nextMode in
this phase gives the exact native materialization order. Reversing those
local declarations retains 66/68. A plain nonzero-store swap gives 64/68;
alpha locals without the state snapshot change pointer allocation and give
61/68. All these controls retain the native 272-byte size.

The existing pre-call allocation pointer is not reused across callbacks:
the post-movie snapshot observes the current global exactly where retail does.
There is no extra memory access, volatile qualifier, register binding, empty
operand, ABI change or compiler-flag change. This measured source/output
relationship does not establish original-source spelling or an internal
optimizer-pass mechanism.

The complete affected owner checks 16 match, 0 differ, independently
reproduced with allocated/relocation-section and normalized-assembly parity
between ordinary and diagnostic builds. The retail checksum and direct image
comparison pass, followed by a no-op build. All 27 tooling test files (436
cases), the shell wrapper, inline-assembly gate, 144 guarded owners / 504
drafts, seven IRX checks and reconciled game/all-code report scope pass.
Hosted CI is checked on the published commit separately.
