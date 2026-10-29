# Movie viewer process — 588 bytes

`MovieViewerProc` at `0x0022BDA8` now matches all 147 words. The shared record/API
prerequisite is described in [movie-records.md](movie-records.md); no padded
alias, fabricated return, compiler option or artificial dependency is used.

## Native reconstruction and donor credit

PiM adapted the ordinary control flow from DDS1 `mnuMovieViewer`,
`src/dds1/game/code_00270098.c:217`, at Megami-Decomps/dds-decomp commit
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`, checking every title-specific path
against SMT3 native `movie/MovieViewer/MovieViewerProc.s`. The source also
builds on the pre-existing SMT3 draft; neither donor nor prior matches are
counted as new work here.

- Two real cadence arms write `decodeCb.fps` through the actual nested movie
  record, setting 1 while the speed button is held and 30 otherwise. This gives
  native's split high/low address store without a guessed independent object.
- The previous-button snapshot is an ordinary live scalar used by the actual
  `(current ^ previous) & current` stop condition.
- The viewer step provider returns a linked record whose filename begins at
  `+4`. The existing 128-byte path and `strcpy`/`strcat` sequence retain native's
  valid-filename precondition; no global bound or clamp is invented.
- SMT3's indexed replay increments the actual `movieViewerPhase`, wraps at 25,
  then skips 10, 14 and 18. These are global-state updates, not an unrelated
  local phase copy or a donor-only state machine.
- Successful downward movement excludes upward movement. Down at the end of
  the list may still fall through to up. The genuine new scroll-top value is
  computed once from the incremented cursor minus seven. The old draft's two
  independent successful movement arms were not native control flow.
- The genuine process callback parameter is supplied by the dispatcher and
  remains unused, rather than concealing an incompatible `s32(void)` callback.

The normal tag allocation, information drawing, registration and previous-pad
store all remain in native order. The player and index-wrapper calls use the
independently established void contracts, not incidental `v0` residue.

## Qualification

The final whole viewer owner is 8 match / 0 differ, including all seven existing
peers and native data. The other four prerequisite owners remain exact, and
`ninja` reproduces retail SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`.
Final stock plain/`-da` full-assembly parity is checked with only the random
canonical scratch-directory names normalized. This is output parity, not a
hooked compiler decision trace. The 25 tool test scripts and both EE/IRX draft
checks are required separately; only this function contributes new credit:
**one function, 588 bytes**.
