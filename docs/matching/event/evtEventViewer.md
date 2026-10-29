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
