# Matching notes: facility/fclEncyc

## ApplyChangeListIndex (facility/fclEncyc, 0x128)

The draft emits the native 296 bytes, with 56 of 74 words matching. It scans
signed 16-bit record indices, clamps the selected list position to the visible
range, updates the byte-sized drawing and logical shifts, then invokes the
independent edge-correction helper for every found entry. When the list is
shorter than the visible range, the upper clamp is zero.

The remaining eighteen words begin at +0x44: comparison operand order, timing
of the list-count and visible-count loads, signed versus unsigned comparison
selection, and byte-store order. Native reloads Shift after writing Index;
the draft retains it. The cursor, geometry and line-step pointers address real
subobjects of the existing list window. No layout or extent changes are used.

A more complicated visible-end formula emitted 320 bytes. The direct clamp
recovers the native size. Capturing both count bytes early or chaining the
shift stores retains 296 bytes but worsens overlap to 51 or 46 matching words;
those variants are not retained.

## DebugProcMenu_fclEncyc (facility/fclEncyc, 0x1A8)

The draft emits the native 424 bytes; 103 of 106 words match. It wraps the
three menu choices on horizontal input, returns one on cancellation, selects
the debug state on confirmation, then draws the three labels and selection
marker. The label table occupies sixteen bytes: three text pointers and a
null trailing entry. Its storage remains assembly-owned.

The allocator requests twelve bytes for the debug dispatcher work. A dedicated
work type now describes those bytes, with state and selection halfwords,
instead of borrowing the larger ribbon work type. The dispatcher and existing
debug callbacks use the same complete twelve-byte object.

Only +0x11C through +0x124 differ. Native prepares X before the color's low
half and zero attribute; C prepares the color, attribute and X in that order.
Qualified plain/hooked/unhooked scheduling traces agree: sched1 ties their
priority and register weight, but X has two dependents versus three for the
other argument setups. Sched2 retains that ordering through its original-order
tie break. These are independent constant arguments to the same print call.

Capturing the returned packet or the label pointer in a local does not change
the result. The retained source uses the direct print/link expression. Provider
coordinate and color parameters follow the unsigned 32-bit C definition.

## DrawRegistListwinElem (facility/fclEncyc, 0x18C)

The draft emits the native 396 bytes with 83 of 99 instruction words matching.
It draws seven registration rows. The existing work record's `fonts[7]` starts
at +0x11E0; the request at +0x11DC precedes that array and is not an eighth row.
Each iteration reads the current texture handle for the background, then reads
the font request after that draw callback. A non-null request receives two
foreground sprites and a font draw. The tag is finally submitted through the
ordering-table callback. The work view is captured before tag allocation.

The font provider's native sign extension establishes its `s8` mode parameter;
its existing font-unit declaration and native return paths establish
`func_001C7E38(frFontHandle_t *, s8, u32) -> s32`. The owner declarations now
agree with that contract, preserving all 105 normal C matches.

Sixteen words remain different at +0x20, +0x24, +0x28, +0x2C, +0x30, +0x34,
+0x38, +0x3C, +0x48, +0x60, +0x68, +0x70, +0x74, +0x8C, +0xCC and +0x10C.
The unresolved question is the native saved-register allocation and address /
sprite-argument scheduling; the operations and frame size agree. A separate
font-array cursor regressed to 71 of 99 words matching and was discarded.
A default scratch promotion qualified the complete owner at 105 match / 1
differ with this target as the sole difference. The retail build retains its
assembly fallback.

## ApplyEdgeListCursor (facility/fclEncyc, 0xA8)

42 of 42 words match at 168 bytes in ordinary C. The complete owner checks
110 match, 0 differ, including owned data. Source commit: `f173c1fadb534e63fd19fc84463ac9e4af0a46cf`.
Exact-source [CI run 40976126797](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40976126797)
passed build and deployment.

Keep explicit pointers to the existing cursor, geometry and vertical-step
subobjects. Snapshot the actual shift and maximum-shift bytes. At the upper
edge the observed shift is zero; `Shift = 1`, the index decrement, then
`DrawShift = shift + 1` express the actual transition to row one. Both one
values have real consumers. This source ordering produces the native store
ordering without a constraint, dummy value, artificial branch or new type.
At the lower edge, assign the truncated new row to the byte-sized `Shift`
then copy that stored value to `DrawShift` before calculating geometry.

Signed index comparisons remain distinct from the native unsigned-halfword
observation used for index updates. The geometry calculation preserves the
existing vertical-step multiplication and default Y addition. This does not
invent range guarantees for arbitrary list contents or alter the existing
valid-list contract. Independent review establishes StepY 28 and DefY 11
for the initialized Encyc windows; even row 253 gives Y 7095, safely within
signed 32-bit range. This is evidence for that UI domain, not a whole-program
proof against later arbitrary memory mutation or an arbitrary-raw-state
signed-overflow guarantee.

The inherited draft emits 160 bytes. Explicit real subobjects restore the
native size with 28 of 42 words matching. Store ordering reaches 33; using
`shift + 1` reaches 37. A separate unsigned index snapshot emits 176 bytes,
while treating the index update as only a signed observation misses native's
independent halfword load. Moving the index decrement between the real shift
and draw-shift publications closes the final two store differences. These
are measured source/output comparisons, not an attribution to an unqualified
internal compiler pass or a claim about historical source spelling.

## ChangeModelMotion (facility/fclEncyc, 0x104)

65 of 65 words match at 260 bytes in ordinary C. Completing the whole
`flags & 0x80` arm with its existing speed assignment and return lets the
compiler share the genuine loop-motion calls. The other arm retains the
final assignment, so each returning path stores speed exactly once. No
constraint, artificial dependency, fabricated bounds, prototype or layout
change is involved; the source still contains real separate calls.

Qualified full-owner baseline/final dumps locate the observed sharing at
`27.jump2`: final has two `mdlLoopMotion` callsites through `25.sched2`, then
one at `27.jump2`; baseline retains two through `29.dbr`. Both plain and
`-da` builds preserve normalized whole assembly, text, relocations and
initialized data. Independent review reproduced final output parity. This
establishes the measured late jump-pass boundary, not a unique historical
source spelling or an unobserved optimizer heuristic.

The 0x80 arm reads table motion and speed before getters/actions; 0x400 then
selects the conditional smooth action versus unconditional loop action.
Without 0x80, motion getter precedes scale getter and the signed-index table
comparison; loop is conditional and 0x400 is irrelevant. True u8/u16 getter
conversions and all signed-id accesses remain unchanged. Speed survives the
callback, but `model->first` is reloaded afterward before storing speed:
loop/smooth providers can actually create a controller and replace `first`.
Caching the old controller would be wrong. No new validity assumptions or
invalid-index safety are claimed, and no float arithmetic is introduced.

Independent complete-owner verification gives 111 match, 0 differ,
preserving all 110 peers and normal owned-data checks. Full qualification passes the retail
checksum, 149 guarded owners / 573 drafts, seven IRX owners, 26 tooling files /
427 tests, shell mocks and docs checks. Actual-published verification repeats
the owner, retail, report, all 65 owner dependencies, all 780 tracked files
and final Ninja no-op checks.
Source commit `8d93e95e72468d79834e91faad818113c4e2533a` has exact-source hosted CI
[41004448827](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41004448827):
build and deploy both completed successfully.

## GetViewModeKey (facility/fclEncyc, 0x1D4)

117 of 117 words match at 468 bytes in ordinary C. The three mode arms
retain their own real input snapshots instead of merging modes zero and one
into a shared arm. Analog mask results are held in `s32` locals before the
key-mask tests; no register binding, empty asm or new input observation is
introduced. This records the landed source shape, not an attribution to an
unobserved compiler pass or a claim about original source spelling.

Modes zero and one map analog bits 0..3 to output bits 1..4 and pad bytes
4..7 to output bits 5..8. Mode zero takes zoom from pad bytes 9 and 11;
mode one takes it from analog bits 6 and 7. Mode two maps analog bits 0..3
to output bits 5..8 and bits 4..7 to output bits 1..4, taking zoom from
pad bytes 6 and 7. Every recognized mode takes output bit zero from the
negative test on pad byte 0x22. Other modes return zero after the initial
`GetAnalogPad` call. Signed byte observations and nonzero tests remain
distinct; the local directional names do not redefine the provider's mapping.

Source commit `4d093b726e02d74376266a0ca337c7854042a807` has successful
exact-source hosted [CI 41012173652](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41012173652).
The recorded landing check was 112 match, 0 differ for the complete owner.
This historical result is not a new local validation run.

## CalcPhaseReadTop (facility/fclEncyc, 0x144)

81 of 81 words match at 324 bytes in ordinary C. The accepted source keeps
the list position and selected record as separate real values. It computes
`Index + Shift` before `PushListwinCursor`, then reads the signed sort byte
after that callback and selects an existing `s16` index-array pointer with
an explicit conditional. The record load remains after the cursor push.
The refresh arm uses the same explicit pointer selection for its font-list
call. No array extent, structure layout or provider declaration changes.

Ribbon calculation precedes sort-key handling. A sort change returns early.
Otherwise cancellation publishes phase 8 and sets status bit 1; confirmation
publishes phase 4, updates status, then stores the selected record index.
The refresh action rebuilds the font list. The common list-color update
remains after the switch. In particular, the status update is not replaced
by a snapshot retained across the cursor callback. Existing valid-list and
sorted-index contracts are preserved, not strengthened into an arbitrary-input
safety guarantee. These are source/output facts, not a compiler-trace claim.

Source commit `1420c409a6fd4866e5e8c90d674b8f5336666396` has successful
exact-source hosted [CI 41013516551](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41013516551).
Its retail build/checksum, tooling tests, guarded EE and IRX compilation,
reports, report-scope check and progress-site deployment all completed
successfully rather than being skipped. The recorded landing check was
113 match, 0 differ for the complete owner. This historical owner result
and hosted evidence do not claim a new local build in the recovery workspace.
