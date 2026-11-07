# Matching notes: kernel/dds3TitleMain

## dds3Titlemain (kernel/dds3TitleMain, 0xD20)

**Not a match candidate yet; the dispatch itself was checked and is correct,
but the function needs real matching work before it should be trusted.**

The outer `switch (initialMode)` has 28 cases (values 0-27) and compiles to a
jump table (`jtbl_0053DDD0`, 29 words: one per case value plus one unreachable
slot shared with the out-of-range default, guarded by `sltiu $2,$3,0x1D`).
Because a mismatched jump table can mean the compiled dispatch silently
routes to the wrong case body, each retail table entry's target address was
deduplicated and compared against the source's case groupings rather than
trusted from the word-diff percentage:

- `case 11: case 20: case 23:` in the draft shares one body in retail too
  (`jtbl_0053DDD0[11] == [20] == [23] == 0x00228480`). Correct.
- `case 18: case 22:` in the draft is a single C case label, but retail gives
  value 18 and value 22 *different* jump targets (`0x002281B8` vs
  `0x00228428`). Read both retail bodies in full: they share an opening
  (bump `unk_2C`, call `dds3TitleDrawMenu`) and a tail (the `unk_20 < 0x18`
  branch decays `unk_28` by 5, floored at 0, identically in both), and differ
  only in the `unk_20 >= 0x18` branch: case 18 sets `mode = mode + 1` (18+1 =
  19), case 22 sets `mode` to the literal `0x13` (19). Both reach the same
  numeric result because the draft already guards this exact asymmetry with
  `if (initialMode == 18) { ...mode + 1... } else { ...mode = 19... }` inside
  the shared case body. So retail's separate jump targets are two near-
  duplicate case bodies in its own source, not evidence the draft's merge is
  wrong; the draft's single guarded body is behaviorally equivalent to both.
  No other jump table entry repeats outside the `{11,20,23}` group, so no
  other case-merge is hiding a similar split.

That clears the one finding serious enough to call a dispatch bug. What is
still unresolved: only 307 of 834 words match and the native function is 24
bytes longer (3360 vs 3336), with the tail of the diff drifting out of
alignment case by case, consistent with a 28-way switch where each case body's
own register allocation differs from retail's and the misalignment compounds
across cases rather than a single local tie. The 24-byte gap was not run down
to a specific missing or extra instruction. Given the size of the function and
that this is exactly the kind of case where a wrong guess would be silent
wrong behavior (per the project's accuracy standard for non-matching drafts),
this is reported as needing a dedicated word-by-word pass per case body rather
than fixed outright. The assembly fallback remains production in the matching
build either way; this note exists so the next pass does not have to re-derive
the jump-table grouping check from scratch.

### Current-owner follow-up (2026-11-07)

A fresh check on `a8e279a` reproduces the current guarded draft at 3336 bytes,
307 matching and 527 differing overlap words. The historical claim reporting
839/840 words is not reproduced: its private source is absent from the restored
workspace. Do not use that score as the current baseline.

Native mode 28 is an explicit no-op handoff state: the unsigned bound is 29,
and table entry 28 targets the common return at `0x228638`. Case 21 writes 28
after the administrative sequence calls. An explicit case restores the table's
29-entry extent. Separating cases 18 and 22 also removes the saved dispatch-mode
lifetime while preserving their different post-callback transitions. Case 18
increments the freshly observed mode; case 22 writes literal 19. Their draw
callback can change the work pointer or mode, so they must not both assign 19.
The current merged draft already preserves this distinction.

The provider audit found three inherited declaration discrepancies:
`dds3PlayMovieByIndex` and `p2sdPlayBGM` are `void(s32)` in their canonical
providers, not `s32(s32)`, and `dds3AdminiCallSequence` takes an unsigned 32-bit
size. The title calls ignore both returns and pass zero sizes. Correcting the
movie declaration changes scheduling substantially: a private case-local
reconstruction with the old declaration emits 3368 bytes, but the truthful
version emits 3328 bytes, with 286 matching/546 differing overlap words and
32 uncovered native bytes. All 11 peers remain exact. Its 29-entry table still
has 20 differing destination addresses; it is not an exact owner/data result.
No source change was promoted, and production retains assembly.

All current title helpers are external to this owner. The older visibility
experiment therefore does not establish a current same-unit dependency, even
under the newer conservative ABI-view allowance. No false return declaration,
provider move, or owner change is justified by that historical score.

The reconstruction retains signed fields and pad tests, modular timer/alpha
arithmetic, frame-count capture before movie-wait calls, and fresh observations
after callbacks. A bounded comparison of the actual baseline and reconstructed
bodies passed 200000 callback-mutating cases in optimized and ASan/UBSan builds.
This checks baseline equivalence and callback order, not exhaustive native
execution or callback arguments. Broad register, store and tail-sharing
residuals remain; further work needs a new native/control premise.
