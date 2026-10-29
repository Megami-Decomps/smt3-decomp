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
