# Data-object signedness mismatches

Measured against revision `1f6683a7a55b8e2157cfe20d67495720d10754fd`
(`git rev-parse HEAD`). Retail checksum `5facfbf7916836613449c922e26a4ec412207b32`
(`config/smt3/checksum.sha1`) still matches the build artifact at
`build/smt3/SLUS_209.11` (`sha1sum build/smt3/SLUS_209.11`); this pass made no
code changes, so the image cannot have moved.

This is a narrow slice of the 538 signedness-only differences that
`docs/data-width-mismatches.md` left unaudited. It covers only scalar
objects, not arrays or structs, and only cases where some consumer performs
a sign-dependent operation on the value: a comparison against zero or a
signed bound, a right shift, or a division or modulo. An assignment, an
equality test, a bitwise mask, and a left shift do not qualify an object,
even when the two authorities disagree on its sign, because they produce
the same bit pattern regardless of which interpretation is used.

## Method

1. Parsed `config/smt3/symbol_addrs.txt` for the `type:` field on every
   scalar integer symbol (1313 entries).
2. Extracted every file-scope scalar integer declaration from tracked
   `src/smt3/**/*.c` (309 files) and `include/**/*.h` (76 files), grouped by
   symbol name across translation units.
3. Built the candidate pool: symbols where, at a single agreed width, the
   sign disagrees somewhere among `symbol_addrs.txt`'s `type:` field and the
   C declarations. 353 candidates (`candidates.py`, "Total candidate
   symbols (signedness disagreement, equal width): 353").
4. Searched every usage line of each candidate for a right shift, a
   division or modulo, or a relational comparison (`<`, `>`, `<=`, `>=`)
   against something, explicitly excluding `==`/`!=`, `<<`, and `->`.
   84 candidates have at least one such usage (`filter_ops.py`, "Symbols
   with at least one candidate sign-dependent usage: 84").
5. For each of the 84, read the full surrounding function (not just the
   matched line) to determine whether the object's value can actually
   cross the sign boundary (top bit set) at that comparison. Most cannot:
   they are counters or frame timers that only ever hold small positive
   values, decremented under a `> 0` guard in the same block that reads
   them, so the comparison yields the identical result under either sign
   interpretation. 13 can cross the sign boundary; the rest cannot. See
   "Non-qualifying count" below for the exact tally.

`INCLUDE_SDATA`/`INCLUDE_RODATA` directives were read only to confirm a
symbol's real storage exists and to see its size; the type string the macro
carries (always `const s32`, per `tools/include_sdata.py`) was never
trusted as evidence, per the precedent in `docs/data-width-mismatches.md`.

### Authority trust

Per the task order (`symbol_addrs.txt` type field, then the split `.s` for
real storage, then C declarations), but with the same caveat the prior pass
documented: `symbol_addrs.txt`'s `type:` field is an IDA default that
defaults to unsigned, not a measurement. For every finding below, every
file that declares the object in C agrees with every other file (no
cross-TU disagreement among the C declarations themselves), and the
project's own checksum-matching build already compiles with those C
declarations. That combination was trusted over `symbol_addrs.txt`, exactly
as `docs/data-width-mismatches.md` trusted the multi-file-consistent C
declaration for `dds3ScreenBlurAlpha`/`dds3ScreenBlurScale` over
`symbol_addrs.txt`'s label. Four of this pass's findings
(`dds3ScreenBlurAlpha`, `dds3ScreenBlurScale`, `gfldPlayerDamegeCnt`,
`fldHitChkMatter`) reuse that exact corroboration; the rest are new
instances of the same shape (a consistent signed C declaration against an
unsigned `symbol_addrs.txt` label) in different subsystems. No case in this
pass needed the split `.s` as a tiebreaker; the C declarations were never
split on sign for any flagged symbol.

### Reach

- `config/smt3/symbol_addrs.txt`: read in full as one tracked text file.
  No reach gap.
- `src/smt3/**/*.c` and `include/**/*.h`: tracked files, searched and read
  in full for each candidate. No reach gap for this region.
- Gitignored `/asm/`, `/assets/`, `/build/`, the retail image, and other
  sessions' scratchpads were not searched at all. One finding
  (`threadMgrSema`) has no `INCLUDE_SDATA`/`INCLUDE_RODATA` directive and no
  non-`extern` definition anywhere in the tracked tree; its real storage
  definition, if one exists outside the split asm this scan cannot see, was
  not inspected. This scan cannot state the full byte-level initial value
  of any object it is reporting on; findings rest on the C declarations'
  sign and on the consumer code's logic, not on inspecting raw bytes.
- The IDB itself was not queried; `symbol_addrs.txt`'s committed text was
  used as its proxy, consistent with the prior two passes.

## Qualifying findings

### `myMaka` (and `rBoxNum`, same site)

- Authorities: `symbol_addrs.txt` labels `myMaka` `type:u32`; the five C
  sites (`src/smt3/pipe/pipeObject.c:86`, `src/smt3/pipe/pipeSection.c:105`,
  and others) all declare it `extern s32`. `rBoxNum` is labelled
  `type:u16`(/u8-width family) in `symbol_addrs.txt` but declared
  `extern s8` consistently in C. Both carry a fake `const s32` type via
  `INCLUDE_SDATA` in `pipeSection.c:1681` and `:1687`, which was not
  trusted. C was trusted for both, per the authority-trust rule above.
- Consumer: `pipeDropMaka` in `src/smt3/pipe/pipeSection.c:728`,
  `myMaka = myMaka < 0 ? 0 : myMaka;` after a subtraction, a comparison
  against zero. `rBoxNum` is decremented unconditionally at line 735
  (`rBoxNum--;`) and gated by `rBoxNum > 0` at lines 816 and 829, another
  comparison against zero. Both sites are inside `MyHitAction`, itself
  inside the `#ifdef NON_MATCHING` block starting at `pipeSection.c:700`
  (reconstructed C, compiled only when `NON_MATCHING` is defined; the
  matching build uses `INCLUDE_ASM` instead, so this logic is invisible on
  retail hardware but live in any native port build, which must define
  `NON_MATCHING`).
- Runtime consequence: if `myMaka`/`rBoxNum` were read as unsigned, a
  negative value produced by the subtraction would show up as a huge
  positive number instead of clamping to zero or gating a further
  decrement; `myMaka < 0` and `rBoxNum > 0` would behave as if the
  resource/counter were still large and positive. This is the "bounds
  check that always passes" shape: a quantity meant to be clamped at zero
  instead reads as nearly `UINT_MAX`, which would then be spent, compared,
  or displayed as if valid. Plausible, not visibly wrong: a huge but
  technically in-range-looking number doesn't announce itself as corrupt
  the way a crash would.

### `dds3ScreenBlurAlpha` and `dds3ScreenBlurScale`

- Authorities: `symbol_addrs.txt` labels both `type:u16`;
  `src/smt3/kernel/dds3KernelDraw.c:197-198` declares both `extern s16`,
  consistently with every other site.
- Consumer: `src/smt3/interface/itfTestMesManager.c:415-455`, the debug
  panel that adjusts both values by `-= 10` / `+= 10` and then clamps with
  `< 0`, `> 255`, and (for `dds3ScreenBlurScale`) `< -255`, comparisons
  against zero and against a negative literal bound. This function is not
  behind any `#ifdef NON_MATCHING` guard; it compiles into the ordinary
  matching build, and the build's checksum already matches with the
  current signed C declaration, independently corroborating that signed is
  the correct type (not just the IDA-default-unsigned explanation that
  usually accounts for these).
- Runtime consequence: if either value were unsigned, the decrement path
  that is supposed to walk it down through zero into small negative values
  (`dds3ScreenBlurScale` down to -255) would instead wrap to just under
  65536 and the `< 0` / `< -255` branches would never fire, so the clamp
  that stops the value from decrementing further would never engage.
  Visibly wrong: the screen blur amount would jump to a huge value for one
  frame (interpreted downstream as 16-bit alpha/scale) and only recover
  once it wrapped back down over many more decrements, producing an
  obvious visual glitch, not a quietly wrong number.

### `winoff_x`

- Authorities: `symbol_addrs.txt` labels it `type:u32`; the C declaration
  in `src/smt3/event/evtEventViewer.c` is `extern s32`.
- Consumer: `evtEventViewer.c:2400-2435`, a debug offset adjuster gated by
  `winoff_x < 11` and `winoff_x >= -99`, a comparison against a negative
  literal bound.
- Runtime consequence: if `winoff_x` were unsigned, the `>= -99` guard
  would be read as `>= 4294967197`, which no ordinary small value satisfies,
  so the branch that is supposed to allow `winoff_x` to go as low as -99
  would behave as if the lower bound were far below any value the counter
  ever reaches; the practical effect is the same as removing that lower
  bound. Plausible: the debug offset would simply keep decrementing past
  where it was meant to stop, with no obvious visual signal that the clamp
  failed.

### `gMsgID` and `gSelMsgNo`

- Authorities: `symbol_addrs.txt` labels both `type:u32`;
  `src/smt3/facility/fclMisc.c:28,30` and `src/smt3/code_0022C578.c:145,147`
  declare both `extern s32`, consistently.
- Consumer: `fclMisc.c` uses `-1` as the explicit "no active message"
  sentinel for `gMsgID` (set at `fclDestroyMessage`, line ~444:
  `gMsgID = -1;`) and gates nearly every operation in the message
  subsystem on `gMsgID < 0` (lines 350, 368, 381, 402, 418, 438, 450, 461,
  474; also `>= 0` at line 390), a comparison against zero. `gSelMsgNo` is
  assigned `-1` as its own "no selection" sentinel and checked with
  `gSelMsgNo >= 0` at line 486.
- Runtime consequence: if `gMsgID`/`gSelMsgNo` were unsigned, the `-1`
  sentinel would read back as a huge positive id, and every `< 0` guard
  that is meant to detect "no message is active" would never fire. The
  message subsystem would then call `itfMesMngSetPhase`,
  `itfMesMngMsgWndCls`, `itfMesMngRequestMessageRelease`, and similar
  functions with that bogus id as if a message were active. Visibly wrong:
  passing an out-of-range id into a message-slot lookup is the kind of bug
  that shows up immediately as a crash or garbage message state, not a
  subtle miscalculation.

### `fldHitChkMatter`

- Authorities: `symbol_addrs.txt` labels it `type:u32`; four C sites
  (`src/smt3/field/fldCamera.c:146`, `fldWap.c:290`, `fldPlayer.c:1368`,
  `fldHit.c:133`) all declare it `extern s32`.
- Consumer: `-1` is the explicit "no surface hit" sentinel, assigned at
  `fldCamera.c:274`, `fldHit.c:147,1109,1477`. Two consumers guard an array
  index with it: `fldPlayer.c:1616` (`fldHitChkMatter >= 0 && ...`,
  followed immediately by `fldMatterTable[fldHitChkMatter]` at line 1620)
  and `fldCamera.c:164-166` (`index = fldHitChkMatter; if (index < 0)
  return;`, followed by `fldMatterTable[index]` at line 166). Both are
  comparisons against zero. Both consumer sites are inside
  `#ifdef NON_MATCHING` blocks (`fldPlayer.c`'s `fldPlayerCalc_Wm2`,
  guarded from line 1364; `fldCamera.c`'s `fldCamCalcSaka`, guarded from
  line 151), invisible to the matching retail build.
- Runtime consequence: if `fldHitChkMatter` were unsigned, the `-1`
  sentinel would read as a huge positive index, the `>= 0` / `>= 0` (the
  `< 0` early-return's negation) guard would always pass, and the code
  would index `fldMatterTable` with that huge value while no surface had
  actually been hit. Visibly wrong: an out-of-bounds array read at a
  multi-gigabyte-equivalent offset is a crash or memory-corruption bug in
  any native port, not a plausible-looking wrong number.

### `threadMgrSema`

- Authorities: `symbol_addrs.txt` labels it `type:u32`;
  `src/smt3/sdk/libkernl.c:479,533` both declare it `extern s32`. No
  `INCLUDE_SDATA`/`INCLUDE_RODATA` directive for it exists anywhere in the
  tracked tree, and no non-`extern` definition is tracked either; its real
  storage definition lives outside what this scan can see (see Reach).
- Consumer: `libkernl.c:554-556`, `threadMgrSema = CreateSema(&sema); if
  (threadMgrSema < 0) { return -1; }` inside `InitThread`, a comparison
  against zero on the direct return value of the Sony SDK's `CreateSema`,
  which returns a negative error code on failure and a non-negative
  semaphore id on success. The same semaphore id is then used in
  `WaitSema`/`iSignalSema` calls elsewhere in the file.
- Runtime consequence: if `threadMgrSema` were unsigned, a `CreateSema`
  failure would never be detected (the error code's negative bit pattern
  reads as a huge positive number, which is `>= 0`), and `InitThread` would
  proceed to use that bogus value as a semaphore id for the rest of the
  program's life. Visibly wrong: using an invalid semaphore id in
  subsequent kernel calls is the kind of failure that surfaces immediately
  as a hang or an SDK error, not a quiet miscalculation. This failure path
  is rare in practice (semaphore creation failing at all), but it is
  exactly the path the original check exists to catch.

### `movieStreamSema`, `sdfDelayedResrcSemaId`, `sdfFileSemaId`

Three independent subsystems that all use the identical idiom: a cached
semaphore id, created once on first use and guarded by a negative sentinel
so later calls skip re-creating it.

- Authorities: all three are labelled `type:u32` in `symbol_addrs.txt`; all
  C declarations are `extern s32` (`sdfMovie.c:34`, `sdfDelayedResrc.c:46`
  and `sdfPrim.c:440`, `sdfFile.c:28` and `sdfIpu.c:25` and `sdfRand.c:8`).
  All three also carry an `INCLUDE_SDATA(const s32, ...)` directive, whose
  type string was not trusted (per the task's directive) but which does
  confirm each is real tracked storage.
- Consumer: `sdfMovie.c:539` (`if (movieStreamSema < 0) { movieStreamSema =
  sdfCreateSema(1, 1, 0); }`), `sdfDelayedResrc.c:86` (same shape with
  `sdfDelayedResrcSemaId`), `sdfFile.c:282` (same shape with
  `sdfFileSemaId`). Each is a comparison against zero gating a one-time
  semaphore creation, and the resulting value is passed into
  `WaitSema`/`SignalSema` elsewhere in the same file (e.g. `sdfFile.c:275`,
  `:284`).
- Runtime consequence: if any of the three were unsigned, the `< 0` check
  would never be true regardless of the sentinel's actual bit pattern, so
  the semaphore would never be created and every later `WaitSema`/
  `SignalSema` call would operate on whatever uninitialized or zero value
  the storage started with instead of a real semaphore id. Visibly wrong,
  same reasoning as `threadMgrSema`: an invalid semaphore id used in a
  kernel wait call is expected to hang or fail immediately, not produce a
  plausible but wrong result.

### `gfldPlayerDamegeCnt`

- Authorities: `symbol_addrs.txt` labels it `type:u32`; three C sites
  (`fldWap.c:207`, `fldMain.c:487`, `fldPlayer.c:47`) all declare it
  `extern s32`.
- Consumer: `fldWap.c:473-475` assigns it `100` or `-100` depending on
  which direction a ladder-climb event is (`gfldPlayerDamegeCnt = 100;` /
  `= -100;`). `fldPlayerCalcHasigo` in `fldPlayer.c:639,652` branches on
  `gfldPlayerDamegeCnt > 0` (climbing up: `pos[1] -= 2.0f`) versus
  `gfldPlayerDamegeCnt < 0` (climbing down: `pos[1] += 2.0f`), both
  comparisons against zero. This function is inside the `#ifdef
  NON_MATCHING` block starting at `fldPlayer.c:631` (14 of 58 words differ
  from the matching build's `INCLUDE_ASM`), invisible to the retail build.
- Runtime consequence: if `gfldPlayerDamegeCnt` were unsigned, `-100`'s bit
  pattern reads as a huge positive number, which satisfies `> 0` (true) and
  never satisfies `< 0`. Both the "climbing up" and "climbing down" cases
  would take the `> 0` branch (`pos[1] -= 2.0f`, moving the player up)
  regardless of which direction was actually intended. Visibly wrong: a
  player climbing down a ladder would instead be animated moving upward,
  an immediately obvious direction-reversal bug, not a subtle one.

## Non-qualifying count

353 candidate symbols total (`candidates.py`). 269 of those have no
candidate sign-dependent usage at all (353 - 84, from `filter_ops.py`'s 84).
Of the 84 that do have at least one syntactic right-shift, division/modulo,
or relational-comparison usage, 71 were read in full and found to never
reach a value that crosses the sign boundary at that usage: most are frame
counters or timers that only ever hold small non-negative values, each
decremented under an explicit `> 0` guard in the same function that reads
them (for example `cmpStatTimer`, `dds3QuakeFrame`, `evtDebugState`,
`evtMoonMove`, `gslCheckCardWait`, `gfldPlayerHasiCnt`, `fldPlayerStepDist`,
`fldProcAmWait`, `fldProcCampWait`, `gencStageInnAlp`, `nbBgmSaved`,
`nbEncountMax`, `nbPanelStockFrame`, `nbResultState`, `test_phearts`,
`StockIDListTop`, `StockIDListCursor`, `StockIDListCount`,
`nbCamera_Cut1Enemy_Frame`, `fldAutoMapField`); two are clamped to
non-negative by construction at their only assignment sites
(`offsetCount`, whose one non-trivial assignment is
`next >= 0 ? next : 10`); and one (`sdfFileCmdCount`) is an unconditional
decrement paired 1:1 with an unconditional increment in the same cache
structure, with no demonstrated path to going negative under normal
operation. 71 + 269 = 340 non-qualifying; 353 - 340 = 13 qualifying,
matching the count above.

## Cross-linking

This document is not yet linked from `docs/README.md` or its siblings
(`docs/return-width-mismatches.md`, `docs/data-width-mismatches.md`).
`docs/README.md` was confirmed clean (`git status --short`) and was not
edited, because another session was live in `docs/data-width-mismatches.md`
at the time of this scan. Add the cross-links once that work has landed.
