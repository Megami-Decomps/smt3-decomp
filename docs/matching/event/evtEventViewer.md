# Matching notes: event/evtEventViewer

## evtEventViewerProcess3 (event/evtEventViewer, 0x16C)

88 of 91 words match, at the exact 364-byte size.

Three consecutive words, the second `viewerinfo->flags & 1` test after
`FrameSelect`: retail loads the field into `$2`, masks it there and branches
on it, letting the `return 0` in the branch's delay slot overwrite the same
register. The draft puts the temp in `$3` because it treats `$2` as taken by
the return value. Every other word, including both jump targets, the three
`evtViewerStopFrame` sites and the shared flags load under the pad `0x2C`
test, is identical.

The whole function body had to go through `viewerinfo->` directly: a local
`evtViewer_t *viewer` reassigned after each call costs a saved register and
misses by 58 words.

The frame advance at the end reads `if (frameCurrent >= frameLast)` with the
`evtViewerStopFrame` arm first; written as `frameCurrent < frameLast` with
the increment first, gcc places the arms the other way round and the whole
tail shifts.

Ruled out for the three words: `& 1` with and without `!= 0`, `== 1`, an
early `return 0` per arm in the frame advance, a `ret` variable for the
single exit, and restructuring the two flags tests as one `!= 0` call guard
plus one `== 0` tail guard. `FrameSelect` and `evtViewerStopFrame` are
defined in this unit, so the `s32`-return lever that blocks the `$2`
reservation is not available for them.

## RunFrameUnit (event/evtEventViewer, 0x94)

37 of 37 words match at 148 bytes in ordinary C. The complete owner has
51 matching functions and no differences, including its owned data.
Source commit: `7e989fba5a29f13ca18a64420e83727423f2fca5`.
Exact-source [CI run 40970257246](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40970257246)
passed build and deployment.

The traversal uses an initial jump to the test and a shared next-frame
label. Dispatch is a switch on the actual signed-byte values: 0 and 1 update
`mode`, 2 through 4 update `otherMode`, and all other values leave both
unchanged. In the measured controls, both source properties matter: the
same switch in an ordinary `for` traversal emits 140 bytes, while the
labeled traversal with `if`
comparisons emits 148 bytes but differs in eight words.

Qualified compiler dumps explain the last difference. The switch's
`jump2` output keeps the first range branch targeted at the real `mode`
assignment, with the second range test on the fallthrough path. Delay-slot
reorganization can absorb that assignment and redirect the branch to the
next-frame label, producing retail's `bnel` and its assignment slot. The
`if` form instead targets the second comparison, which fills the branch
slot while a separate backward jump carries the mode assignment. Moving
the assignment out of line explicitly is compiler-inert. Whole-owner
plain/dump assembly agrees apart from the scratch `.file` pathname, and
its entire `.text` agrees. This identifies the observed compiler property,
not the original source spelling. No compiler constraints are retained.

The null and frame-time tests precede the payload read. The unsigned
16-bit frame is promoted before comparison with signed `nowFrame`; the
payload remains signed 8-bit. Each eligible node advances once, and the
first future node stops traversal. The defaults are 0 and 2, with the last
qualifying update winning independently for each mode. The two
`evtRunUnitMode` calls retain their order and separate `track->basic`
reads; the second remains the native tail call.

## evtRunFrame (event/evtEventViewer, 0x2BC)

175 of 175 words match at 700 bytes in ordinary C. Separate real flag clears,
initialization of `nearestFrame` only at the second traversal, and ordering of
independent pre-callback blur resets produce the native sequence. There are no
constraints, aliases, new types or declaration changes. All 51 previously
matching peers, including RunFrameUnit, and owned data remain exact.

The distinct reset fields have no intervening callback observation. Separate
`flags &= ~2` and `flags &= ~4` yield the native two AND operations and one final
store, not extra volatile observations. `nearestFrame` is initialized before
every consumer; its address is first taken in the second traversal. Both
traversals retain the real frame/track links observed after callbacks, and the
second reloads the global first-object pointer after the first traversal.
Camera publication, RunFrame3, quake handling and later flag/blur observations
preserve their native order and reloads.

Differences involving promoted signed-16-bit end-frame and unsigned-16-bit
frame/index fields are within signed-32-bit range. Total-frame subtraction and
frame-plus-length addition instead use explicit unsigned-32-bit arithmetic,
then convert to signed with the pinned target's bit-preserving behavior. This
preserves native wrap without invented overflow bounds or a universal ISO-C
portability claim. Existing valid pointer/list domains remain inherited; cyclic
or adversarially mutated lists are not newly guaranteed to terminate.

Independent final-source owner checking gives 52 match, 0 differ. Diagnostic
complete-owner plain/stock-dump assembly, text and relocation parity is verified;
no historical spelling or internal scheduler-decision claim follows from that.
Final qualification passes the retail checksum, all 149 guarded owners / 586
drafts, seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs checks.
Source commit `5fac154cda0e857968ae35f52c62aa13163e78de` passed exact-source hosted CI
[40993213967](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40993213967),
including build and deploy.
