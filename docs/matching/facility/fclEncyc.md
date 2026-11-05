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

106 of 106 words match at the native 424-byte size. The routine wraps three
menu choices on horizontal input, returns one on cancellation, selects the
debug state on confirmation, then draws the labels and selection marker.
The existing twelve-byte dispatcher work remains the complete object, with
its state and selection halfwords. Valid initialized menu selection is 0..2;
no arbitrary-input bounds guarantee or new capacity is introduced.

Only the print loop's actual fourth argument is constrained: initialized
`s32 attr = 0` is bound to its ABI register `$7`, then passed through one
nonvolatile empty live operand before `sdfDevPrintf`. It initializes anew in
each iteration and has no call before its sole print consumer. Nothing relies
on that fixed value after the print or the outer `sdfDrawTagLink` call.
X and color remain ordinary literals. The empty template emits no instruction
and is not a memory barrier. This is a local reconstruction aid, not evidence
of historical source spelling.

The ordinary baseline is 103/106 words. Binding alone reaches 104/106 at
424 bytes; removing the binding while retaining the live operand emits
436 bytes. The combined minimal form gives the native X, color-low-half,
zero-attribute setup order. Earlier qualified scheduling evidence established
why the ordinary argument setups were reordered; these new controls establish
the narrow source/output dependency without claiming an unobserved comparator
rule for the constrained form.

The provider is exactly `void *sdfDevPrintf(u32, u32, u32, s32,
const char *, ...)`. The assembly-owned sixteen-byte label table remains
three pointers followed by null, with only its first three entries consumed.
Calls, native access widths/order and marker-selection reload after drawing
callbacks are unchanged. No table, provider, callback ABI or owned data moves.

Seven unconditional forward declarations expose existing same-owner external
providers: GetRatio, ReleaseWork, SetScreenOffset, DrawExplainBack,
DrawReadStatusMoney, DrawReadMainHelp and ReleaseFontExplain. Their full
prefixed names, return/parameter types, linkage and attributes agree with
existing definitions and lower declarations. These fix inherited strict-mode
visibility errors, not a new runtime defect. Independent three-mode pre/post
comparisons preserve complete assembly and allocated-section/header/data,
non-file symbol and relocation contracts. The unrelated dds3KillProcess
pointer warning remains; strict qualification is not a warning-free claim.

Independent semantic and complete-output review approves the final source.
Strict production, `SKIP_ASM`, and `NON_MATCHING` plus `SKIP_ASM` builds pass.
Final, ordinary baseline and both minimal controls have qualified complete
plain/dump assembly and object parity. Complete owner/data checks give
114 match, 0 differ, preserving all 113 peers. Full retail checksum,
generated-source, regression and reconciled-report checks accompany the
landing. Exactly one game function and 424 bytes are added.

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

## InitPhaseReadStatus (facility/fclEncyc, 0x334)

195 of 205 words match at the native 820 bytes. A fresh private promotion
checks 113 existing C peers exactly, with this target the sole difference
and no reported owned-data failures. The production assembly fallback stays
in place; this investigation earns no matching credit.

The ribbon provider copies each of the four complete 16-byte vectors
synchronously, using two unaligned 64-bit reads per vector. It retains the
texture pointers, not the vector pointers. All four components of each
vector are initialized before each call, and the same 64-byte stack area
can be reused after return. The provider's complete owner checks 13 match,
0 differ. Private layout assertions confirm the current 16-byte vector,
72-byte ribbon element and 380-byte work type against native accesses and
allocation; this is not a reconstruction of the original type spelling.

All ten residual words lie in early work-field initialization, before the
already-exact vector construction. Status alpha 128 and message ID -1 use
interchanged registers. The two speed-32 stores and intervening zero stores
are scheduled differently. Qualified RTL snapshots show CSE sharing the
speed constant and sched1 placing the message-ID store before both speed
stores; the native order is help[1] speed, message ID, zeros, help[0] speed.
The existing source already expresses that native store order. The snapshots
locate the observed differences, not an internal scheduler tie-break cause.

One bounded ordinary control names the actual status-alpha and message-ID
values, each feeding only its original store. It regresses to 192 of 205
words at 820 bytes, preserving all 113 peers: alpha gets the native register,
but the earlier message-ID birth displaces the speed constant and leaves the
store-order problem. No vector-constructor or initializer-order sweep follows.
Both baseline and control have qualified plain/diagnostic parity for all
allocated sections, non-file symbols, ordered relocations and normalized
assembly. Layout assertions plus uninitialized diagnostics also preserve
baseline output; no uninitialized warning appears, although existing
unrelated declaration and callback-argument warnings remain. Further work
needs a genuine initialization/liveness premise or deeper compiler evidence,
not fabricated layout, dummy dependencies or vector-lifetime changes.

A separate bounded speed-value premise is also rejected. Naming one initialized
`s32` speed for its two existing stores gives 186 of 205 words. Adding one
`FAKEMATCH_ASM_LIVE_VOLATILE(speed)` between those stores gives 184 of 205;
both retain 820 bytes and all 113 exact peers. The value has no address escape
or call within its lifetime. This empty read/write register operand pins no
register and clobbers no memory; it is not a memory barrier. Removing it
reproduces the ordinary shared-speed source byte-for-byte. Both controls have
qualified plain/diagnostic parity and unchanged non-text allocated sections,
non-file symbols and relocations relative to baseline. Neither is retained.

## DrawReadStatusMoney (facility/fclEncyc, 0x178)

94 of 94 words match at the native 376-byte size, in ordinary C without a
new compiler constraint, type declaration, qualifier, provider or flag.

The local record contains precisely the two actual eight-byte position
records followed by the actual sixteen-byte color. Its size is 32, alignment
four, and color offset sixteen, matching native's SP+0 and SP+0x10 objects.
There is no padding directive, fabricated member, widened bound or dummy use.
This is a useful ordinary representation, not proof of historical spelling.

Native copies all sixteen template bytes, stores alpha at SP+0x1C, and reloads
that word at 0x21D2E8 after both sprite calls, ordering-table registration and
font positioning. The final font color therefore consumes the actual color
component rather than a separately retained scalar. Both vertex arrays are
fully initialized before use. Coordinates remain snapshots across callbacks;
texture and each successive font handle retain their native fresh reads.

On the current owner, the target-only old draft emits 380 bytes, with 17 of
94 native-prefix words matching and four excess bytes. Changing only the final
consumer to the actual color component reaches 42/94 at 376 bytes. Removing
the separate scalar and deferring declarations independently leave that same
complete output. Grouping the real positions and color closes all remaining
words. Restoring the separate alpha in that grouped form emits 380 bytes,
13/94 matching prefix words and four excess bytes. Both substantive features
are load-bearing; no register or source-order sweep was used.

The sprite providers synchronously copy color values into packets. They do
not mutate or retain the local color, so this is native storage/dataflow
reconstruction, not a demonstrated gameplay bug. Independent exact-body
ASan/UBSan tests cover all 65,536 signed-halfword alpha inputs and sixteen
combinations of deliberate callback color mutation, for 1,048,576 paired
cases. They also check retained coordinates and successive font-handle
reloads. Those mutations are adversarial fixtures, not claims about actual
providers, and bounded host fixtures are not retail execution.

Signed-halfword alpha gives `128 - alpha` in [-32639,32896]. Converting it to
the unsigned component and ORing `0x80808000` preserves native low-word bits,
including negative values. No byte mask or 0..128 restriction is added.
Coordinate additions retain their inherited valid-UI-domain signed-overflow
limits. The existing allocator caller pointer return versus provider signed
EE address return is unchanged; this is not global prototype closure.

Complete final owner/data checking gives 115 match, 0 differ. Sixteen full
plain/dump pairs pass, including original, final and minimal controls in
production, SKIP_ASM and retained-C contexts. Twelve strict declaration
builds pass with unchanged output; the existing process-pointer warning
remains. All 114 production/SKIP and 121 retained peers are unchanged after
normalizing generated local-label names. Original and final production
allocated sections and raw relocations agree, but non-file symbols do not:
labels renumber and undefined D_003309E0/F0 become size sixteen instead of
zero. No whole-object-contract identity or hooked compiler-decision claim
is made. Full integration and retail gates remain required for promotion.
