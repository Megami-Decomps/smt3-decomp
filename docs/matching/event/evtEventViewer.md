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

## evtStopFrame (event/evtEventViewer, 0x2A4)

169 of 169 words match at 676 bytes in ordinary C. Two signed track-type
switches retain cases 3, 18, 20 and 21 and skip every other signed value.
The real basic pointer is reused only after its earlier traversals finish;
the same correctly typed control pointer feeds the genuine model and motion
consumers in disjoint phases. There are no dummy uses, artificial lifetimes,
constraints, new types, prototype changes or ABI changes. All 52 prior matches,
including evtRunFrame, and normal owned data remain exact.

Both active-scene calls are retained. Unit, matter and mover traversal order,
post-callback next loads, and fresh effect-viewer observations are preserved.
Unit model-first is reloaded after Seek before Pause, while matter motion is
intentionally snapshotted. Zero `sw` seeks then pauses; every nonzero value
unpauses. Conversion of unsigned nowFrame to float preserves the high-bit
path, and the existing signed mover parameter receives the same low 32 bits
under the target ABI. Inner switch default advances the child, not the track.

Effect case 18 uses its work pointer. Cases 3/20/21 skip negative signed-byte
slots, without inventing an upper bound. The 127-entry array's slot 127 remains
outside the inherited valid index contract; this is not a claim that arbitrary
inputs are safe. Null selected basics and null mover payloads skip as native;
the native control-provider dereference still relies on a valid eligible
payload. No defensive check, list termination guarantee or callback cache is
introduced.

Qualified complete-owner plain/stock-dump assembly, text and relocation parity
covers the two-switch, basic-reuse and control-reuse controls. Initial RTL
assigns the same correctly typed control pseudo in the unit and matter phases;
its measured allocated home changes from v1 to v0. This supports truthful scalar
reuse, not an invented internal scheduling explanation or historical-source
claim. Independent guard-removed checking gives 53 match, 0 differ.
Final qualification passes the retail checksum, 149 guarded owners / 583 drafts,
seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs checks.
Source commit `08f344607276167c36247afc66440c54fce5cdcd` passed exact-source hosted CI
[40995572614](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40995572614),
including build and deploy.

## evtEventViewerProcess4 (event/evtEventViewer, 0x280)

160 of 160 words match at 640 bytes in ordinary C. Signed action-result and
menu-state switches preserve their complete domains. A forward label inside
the loop places the real PopMenu call after the normal continuation and then
exits once. Direct accesses use the existing menuStack entries, with initialized
x, y and mode argument locals. Reordering independent state stores and real
argument initialization recovers the native output without constraints,
volatility, artificial dependencies, shifted structs or before-array pointers.
No layout, prototype or ABI change is introduced.

DrawEffect precedes traversal. Index starts at one and compares live stack
depth; callbacks are followed by fresh menu/viewer observations. The first
dispatch result survives the optional supplementary mode-four dispatch, whose
result is intentionally ignored. Action zero records the current mode and
continues; minus one pops once and exits; every other signed action calls the
existing callback. Callback result one returns Process5_0 immediately, minus
one sets done, and every other signed result leaves done unchanged. Every
non-early callback path exits the loop.

The u16 x coordinate plus s16 window offset fits signed32; y and mode retain
zero extension. Window movement keeps its else-if priority and representable
signed16 stores. Process killing is followed by a fresh viewer reload before
clearing its current process slot. Menu state zero resets the real fields,
sets state one and pushes the menu; state one clears it and sets done; all
other signed states remain unchanged. The final nowmenu test and Process3
transition retain the existing EE32 function-pointer return convention.

The eight-entry stack's active indices one through seven and live object
contract remain inherited. The callback table still requires an eligible mode
with a real nonnull callback; no invented bound, defensive fallback or arbitrary
malformed-input termination guarantee is added.

Complete-owner plain/stock-dump assembly, text and relocation parity is
qualified for the ordered, argument-load and cleaned final controls. This is
measured source/output evidence, not historical-source spelling or an unobserved
optimizer explanation. Independent target-only rebasing onto published StopFrame
preserves its surrounding bytes and gives 54 match, 0 differ, including RunFrame
and StopFrame. Full qualification passes the retail checksum, 149 guarded owners /
582 drafts, seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs
checks. Source commit `127dbb80bec3b72661d7586294dfad93c54398e4` passed exact-source hosted CI
[40996592892](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40996592892),
including build and deploy.
