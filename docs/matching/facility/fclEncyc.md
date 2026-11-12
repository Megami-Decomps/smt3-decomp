# Matching notes: facility/fclEncyc

## ApplyChangeListIndex (facility/fclEncyc, 0x128)

74 of 74 words match at the native 296-byte size in ordinary C. Complete
owner checking gives 118 match, 0 differ, preserving all 117 existing exact
C functions and owned data. No new constraint, qualifier, layout, provider,
array bound or compiler flag is introduced.

The signed 16-bit record scan keeps the requested record as the left compare
operand. At a found record, actual ListNums and ShiftMax bytes are promoted
into separate signed 32-bit locals before the position calculation. Their
range is exactly the byte range; signed subtraction and comparison mirror
native without inventing a stronger list contract. Position is clamped to
zero or the nonnegative visible-range endpoint. The unclamped position is
retained only for the real shift correction.

The updated logical Shift is truncated through its actual byte storage and
then copied to DrawShift, matching the existing lower-edge branch of
ApplyEdgeListCursor. Grouping the original-minus-clamped delta reproduces
native's subtraction before addition. Native still reloads Shift after
publishing Index; both byte publications precede geometry calculation. The
edge-correction helper runs for every found record, including unchanged
positions, and no helper runs when the record is absent.

Fresh ordinary baseline is 56/74. The signed-count snapshot control reaches
68/74; fixing compare operand order and grouping the live correction gives
72/74; logical-then-drawing source publication closes the two remaining
store-order words. All retain the native 296 bytes. A separate explicit byte
temporary emits 304 bytes and is rejected. A precise Shift storage operand
was investigated but regressed and is not used. These are measured source
and output facts, not a claim about an unobserved optimizer rule or recovered
historical spelling.

Fifteen complete plain/dump/strict triples qualify the fallback, old ordinary
source, signed-count snapshot control, 72/74 combined control and final in
production, SKIP_ASM and retained-C modes. Full assembly, allocated sections,
non-file symbols and ordered relocation records agree within each triple.
Independent function-address resolution preserves all 121 production peers,
117 SKIP peers and 121 retained-C peers plus non-text allocated data. The SKIP
promotion inserts the target body, so associated text HI16/LO16 addends and
calls to the formerly undefined target are compared by exact resolved function
and offset, retaining opcode/register bits. No raw SKIP object identity is
claimed. Production's resolved target contract equals the native fallback.

Independent native, ABI and semantic review approves the complete source. An
actual-body differential oracle passes 4,001,720 paired cases under optimized
and ASan/UBSan builds, including signed IDs, count and clamp boundaries, byte
truncation, first-match and absent-entry behavior, and adversarial callback
observation and mutation. Host fixtures are not retail execution and do not
claim safety for arbitrary corrupt counts or overflowing geometry. Full retail
checksum and repository release gates remain required for publication.

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

99 of 99 words match at the native 396-byte size. The complete owner checks
117 match, 0 differ, preserving its 116 existing exact C functions and owned
data. Exactly seven font entries begin at +0x11E0; the request at +0x11DC is
not an eighth row.

Only the initialized instance pointer receives a local binding to native's
callee-saved register. It feeds the pre-allocation work capture and each
iteration's fresh texture read. Its lifetime genuinely spans the draw calls;
the native prologue and epilogue preserve this register. No empty operand,
artificial consumer, loop bound, type, provider or memory-access change is
needed. This is a local compiler allocation aid, not recovered historical
source spelling or a claim about an unobserved allocator decision.

The ordinary promoted source gives 85/99 at 396 bytes. One empty live operand
on the instance pointer gives 93/99, leaving only the instance/countdown
register swap. Binding the pointer gives 99/99 with or without that operand,
so the operand is removed. An explicit UV-pointer local regresses to 73/99
and is not retained.

Work remains captured before packet allocation; every font entry is read
after its background draw. The loaded request survives the two foreground
callbacks, and the next iteration reloads texture and font state. The font
provider remains `s32 (frFontHandle_t *, s8, u32)` and the tag's ordering-table
callback and argument order are unchanged. Independent native and semantic
review confirms the register lifetime and seven-row accesses.

Fifteen complete plain/dump/strict triples qualify the fallback, ordinary
promoted source, final source and both constraint controls across production,
SKIP_ASM and retained-C contexts. Entire assembly and ELF section headers and
contents agree within each triple. The final preserves all 116 production
and SKIP C peers and 121 retained C peers after generated local-label
normalization. Against the ordinary promoted baseline, peer text and
relocations, non-text allocated data and non-file symbols remain exact in
all three modes. Target relocation positions change with the corrected
setup ordering; whole-object identity with the old draft is not claimed.
Native-compiler layout assertions confirm size 0x1200 and both font offsets.
Aggregate retail and repository release gates remain separate.

An independent extracted-actual-body oracle passes 65,536 callback-mutation
cases at O2 and under ASan/UBSan. It verifies old-work capture, fresh texture,
font and OT observations, retained requests across foreground callbacks and
position mutation. This is host semantic evidence, not native execution.


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

205 of 205 words match at the native 820-byte size. The current complete
owner rises from 115 to 116 exact C functions; the ribbon provider is 14/0.
The four-vector construction and 380-byte work layout are unchanged.

An initialized signed-halfword status-alpha binding and three target-local
empty constraints preserve real alpha/work-address dependencies across the
initialization phases. The alpha is consumed by its first store before any
call. The work pointer has actual stores and later calls as consumers before
and after each constraint. These operands emit no instructions, have no
memory clobber and are not memory barriers. They are localized compiler
reconstruction aids, not evidence of original source spelling.

The reduced source keeps the message ID as an ordinary -1 store, with no
message local or binding. Removing the alpha binding, initial work/alpha
pair, middle pointer operand or final pointer operand gives respectively
193, 198, 203 and 200 of 205 matching words, always at 820 bytes with all
115 existing C peers exact. The ribbon provider synchronously copies all
four fully initialized 16-byte vectors and retains only texture pointers;
the same 64-byte stack area can be reused after each call.

Four direct-provider declarations are now truthful throughout the owner,
including all local duplicates: `dds3GetActiveScene` returns `dds3Basic_t*`,
`dds3VisibleSceneBasic` takes `(dds3Basic_t*, s8)`, `sdfMemAllocDirect` returns
`sdfMemHandle_t*`, and `sdfMemLock` takes `sdfMemHandle_t*`. The target scene
local is `dds3Basic_t*`. The existing `void *allocPhaseBuffer` provider and
its allocation/lock chain are type-correct. These declaration repairs are
byte-neutral over complete assembly and ELF in production, SKIP_ASM and
retained C contexts. No shared provider body or header changes; unrelated
owner declarations are not globally certified by this target review.

Independent native, field-width, callback and lifetime review approves the
final source. Six fresh complete ordinary/direct and strict/direct pairs
cover all three contexts, and each strict output equals its plain output.
Uninitialized-value and implicit-declaration diagnostics are clean.
Production's complete 33,216-byte allocated text and every other allocated
section are literally unchanged from the native-fallback baseline. All 121
production peers and 121 retained peers are preserved. SKIP_ASM inserts the
820-byte target plus four alignment bytes at text offset 6,968. All 115
peers remain exact after explicitly reversing only that 824-byte insertion
in 136 R_MIPS_26 fields and two LO16 fields. Opcode/register bits are not
masked. Non-file symbol and ordered relocation contracts use the same exact
insertion mapping, normalizing only generated local-text-label names.

Qualified stock scheduler evidence predates the declaration repair. It is
carried through independently verified complete-output neutrality and stable
compiler/header inputs, not presented as a fresh repaired-source trace. It
shows the actual alpha store and the two speed-store initialization groups;
no unobserved scheduler comparator or original-source claim is made.
Aggregate retail and repository checks remain separate release gates.

### Earlier guarded draft, superseded

195 of 205 words match at the native 820 bytes. A fresh private promotion
checks 113 existing C peers exactly, with this target the sole difference
and no reported owned-data failures. The production assembly fallback then stayed
in place; that investigation earned no matching credit.

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


### DrawReadMainHelp retained source advance (2026-11-12)

The guarded ordinary-C draft now emits the native 1344 bytes, with 332 of
336 words matching, versus 1328 bytes and 54 matching words before this pass.
It remains `NON_MATCHING` with its production assembly fallback and earns no
matching credit.

Native rewrites the second rectangle's X coordinate at 0x0021DAFC before
the third sprite call for help panel zero. The prior draft left that value
in the escaping position array across the preceding callback. Restoring
`pos[1].x = x + 0x1960` reproduces the actual store and its repeated scalar
consumer. The providers currently read rather than mutate the positions;
this is a native-access/source-contract correction, not a claimed game bug.

Each panel now captures its actual signed alpha after allocation, tag
initialization and position reads, before initializing the color array.
Separate block-local alpha values express the disjoint real lifetimes. All
other partial array writes, texture reloads, calls and ordering-table
registration remain unchanged. No new constraint, qualifier, type, flag or
fabricated consumer is used.

The four remaining differences are a local scheduling swap in the second
panel. Qualified stock compiler dumps show alpha load UID262, pointer reuse
UID341, UV-component store UID285 and coordinate calculation UID491. The
store has an anti-dependency on the alpha load; the pointer reuse also
follows that load. Native instead places the store and coordinate calculation
before the load and pointer reuse. This describes the measured compiler
output, not an assertion about historical source spelling. Moving the UV
initialization earlier changes allocation elsewhere; narrow alpha type and
block controls are inert, and a live-alpha read regresses output. These
controls are not retained.

Complete owner checks preserve all 118 matched peers and owned data.
Baseline versus retained production/SKIP raw assembly and objects agree;
plain versus stock diagnostic output agrees in both contexts for the
target-active investigative body. Independent actual-body tests use a separate
sprite-call oracle and pass 200,000 cases each at O0, O2 and UBSan. Fixtures
include callback mutation of work, positions, UV and colors; removing the
position rewrite or moving alpha capture before allocation is rejected.
These are adversarial model tests, not retail execution.
