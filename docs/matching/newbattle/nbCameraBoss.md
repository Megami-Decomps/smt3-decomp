# Matching notes: newbattle/nbCameraBoss

## nbCameraBoss_Sp_DOUTYO_TPFix_Set (newbattle/nbCameraBoss, 0x64)

All 25 words match at the native 100-byte extent. A truthful float local
holds the direction.x read after nbCameraSetState and its zero store, then
supplies the existing target.x and direction.y stores. A localized f1
binding and empty live-float operand preserve native allocation and
scheduling. The value is initialized after the call, has two real field
consumers, and crosses no further call. These are target-specific
reconstruction aids, not evidence of original source spelling. Removing
either constraint leaves five differing words. The ordinary ordering of the
two 1.0 stores completes the match; all native loads, stores, literals,
call ordering and return remain unchanged.

The complete owner passes 112 match, 0 differ. Production, SKIP_ASM and
retained-C checks preserve all peers and nonliteral owned data. Production
and retained literal pools are unchanged; SKIP_ASM adds only the target's
12 native literal bytes. Complete strict plain/dump assembly and ELF parity
passes for final source and both minimal ablations. Independent review
resolves all 100 target bytes against retail in all three modes and checks
the ABI, actual memory accesses and local lifetime.

Host semantic fixtures pass 300,045 cases per variant at -O2, -O3 and under
address/undefined sanitizers. Modern host GCC rounds -888.94f to
0xc45e3c29, while pinned ee-gcc 2.96 and retail use 0xc45e3c28. The exact-
source fixture therefore checks candidate-versus-original behavior; a
separately labeled native-literal fixture substitutes the exact value
-888.93994140625f to check the native integer-store oracle. The production
source keeps its original literal. Host tests do not establish EE literal
conversion; the independently resolved native bytes do.

Integrated retail checksum and repository gates remain required before
publication.

## nbCameraBoss_Cut1_Run (newbattle/nbCameraBoss, 0x1DC)

All 119 words now match at 476 bytes with the exact MASEHA provider visible
alongside the already exact Lusi provider. The existing truthful C body is
unchanged: each selected callee's result is assigned to `shot` and returned,
and default paths return zero. The skill-camera consumer uses that result.
This is a real-provider promotion, not the earlier all-draft diagnostic.

## nbCameraBoss_Lusi_Cut1_Set (newbattle/nbCameraBoss, 0x1F4)

125 of 125 words match at 500 bytes. Independent complete-owner verification
gives 108 match, 0 differ, including owned data and full compiler context.

The signed frame is reloaded after the conditional AllShow call, converted
to a real `f32 rate`, then divided by 103.0f in a separate assignment. A local
`$f1` binding retains the native conversion/division value. A compiler-supported
union representation read supplies the real rate bits to a local `$4` binding;
if the original floating `rate > 1.0f` comparison succeeds, the existing
`PS2_FLOAT_BITS` macro supplies the bits of 1.0f instead. No integer comparison
of floating values or numeric float-to-integer conversion is substituted.

The unconditional type read remains after AllShow, moved before the clamp
without crossing a call, memory write or memory-clobber operation. Native
executes that read on both comparison outcomes. The rate binding dies before
coordinate register reuse; the word binding feeds the four textual existing
interpolation sites (two on a selected path), without a call or intervening
clobber. COP1 f1 and COP2 vf1 are different register files. The six-coordinate
loads and stores, vector operations and memory contracts, final distance/fovy
stores, default-type behavior and return remain unchanged. Negative frames
are not newly clamped or assumed absent. The representation read is qualified
for the pinned EE compiler/ABI, not claimed portable to every C implementation.

No empty asm remains. Earlier conversion and postdivision empty constraints
were removed while preserving exact output. Removing the final rate binding
or merging conversion/division gives six differences at 500 bytes. A clean
independent deletion of the final bits binding gives 508 bytes and a failing
owner. These controls establish current-form allocation effects, not an
absolute minimality proof or an internal optimizer-pass explanation.

The old ordinary float clamp was 21/125 words different. A direct volatile
bit-clamp changed division/constant scheduling; an ordinary union bit-clamp
with the actual bits binding reduced the residual, and the split conversion
plus mixed union/conditional transfer and type observation completed it.
The older unbound-union follow-up was explicitly released without a match;
its negative result did not test this final source form.

Integrated retail SHA1/byte comparison and no-op rebuild, 436 tooling tests,
inline-asm gate, all 142 guarded owners / 493 drafts with stable inputs,
7 IRX checks and reconciled reports pass. Source commit
`c6b04227ce2449fb276c31a7d1c47ffd13db8ea4`; exact-source
[CI 41424180915](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41424180915)
attempt 1 passed build and Pages deployment.

## nbCameraBoss_Cut2_Run (newbattle/nbCameraBoss, 0x230)

92 of 138 words differ, two words short of retail's 140.

The same jump-table dispatcher as `nbCameraBoss_Cut1_Init`, on `id - 0x101`
over 0x58 entries, and written the same way: each arm is one or two
`if (skill == <n>) { callee(pCamera); break; }` tests followed by a `return 0`
or `return 1`, the arms only call, the function falls off the end of the
`switch`, and the `0x158` arm's failing guard falls into `default`. Every
difference is a displacement shift from one block.

The `0x11F` arm is the one block that is wrong. Retail reaches the shared exit
from both of its `return 1` paths with `addiu $2, $0, 1` in a branch delay slot
(`beqz $4, exit` for `skill == 0`, then `bnel $4, $2, exit` annulling a
`daddu $2, $0, $0` for `skill != 0xA5`, then `b exit` with the 1 again).
`if (skill == 0) return 1; if (skill == 0xA5) return 1; return 0;` and
`if (skill == 0) return 1; if (skill != 0xA5) return 0; return 1;` both let
ee-gcc if-convert the pair into `xori`/`sltiu`, four words short.
`if (skill != 0 && skill != 0xA5) { return 0; } return 1;` and a two-case
`switch (skill)` returning 1 both keep the branches but merge the first
`return 1` into a local block that the `beqz` jumps to, leaving its delay slot
empty and the `0xA5` constant in `$3`: two words short. The missing fact is
whatever makes that first `return 1` a bare jump to the shared exit.

The jump table differs as a consequence; the case set itself is confirmed by
the default entries, which all resolve to the one default block.

## nbCameraBoss_BSMIZ_Command_Set (newbattle/nbCameraBoss, 0x218)

134 of 134 words match at the native 536-byte extent in the exact candidate.
Localized constraints act on the initialized camera pointer, the selected
initialized length and the assigned `pos.y` cell. The camera pointer uses
the native callee-saved allocation; length is assigned after the provider
calls and dies after its three XYZ products. The memory operand names only
the real four-byte `pos.y` clamp input. These reproduce native allocation
and reloading, not original source spelling or an internal compiler-pass cause.

The VU stores declare their full-vector memory outputs before C reads or
updates those vectors. The exact typed load operands name the actual,
fully written 16-byte `center`, `base` and `camera->target` vector objects,
not an unrelated cast type or a larger owner object.

Native `pos.w` remains unwritten. The raw full-vector load consumes it,
subtraction produces W and XYZ-only normalization leaves W unchanged;
the final direction store propagates it. This preserves native hardware
behavior, not an initialized C float value or a claim that W is ignored.
No W initializer or typed full-`pos` input is introduced.

### Historical ordinary-C controls

68 of 131 words differ; 524 bytes emitted against retail's 536.

The body came off the matched sibling `nbCameraBoss_BSSAMA_Command_Set`, which
has the same shape plus an `effBTLModelGetID` test. Everything up to the
`length` choice matches except two callee-saved names: retail holds
`pNbCamera` in `$18` and the model id in `$17`, the draft the other way round.
This was the historical allocation mismatch.

Retail reloads `pos.y` from the frame for the `> -1.5f` clamp after storing all
three components, then uses `bc1tl` with the clamp store in the delay slot. The
draft keeps `pos.y` in a register, so the whole scale-and-add block schedules
`pos.y` first and the clamp folds into it, and retail's `b` to the join plus its
`nop` are three words the draft never emits. The HD reconstruction of this
function also reads `pos.y` back through a separate accessor. None of the
historical ordinary-C forms below reproduced that reload: the matched
`BSSAMA_Command_Set` has the identical clamp over a literal distance and keeps
the value in a register, as the draft does.

Ruled out: `if/else`, `?:`, reversed `!=` tests, nested `if`, and a `switch` with
cases 50 and 74 for the 560/550 choice (all identical output); declaration order
of the locals; `u32` for the id; `pos`, `dir` and `center` as `f32[4]` instead of
`sdfVector4_t`; `volatile pos`; `pBase`/`pForm` assignment order;
`STOREVECTOR` against `STOREVECTOR_NOMEM` on each of the three stored vectors;
`length * dir.x` and `dir.x * length + center.x` operand orders; scaling `dir`
in place first (exact size, 41 of 134 words differ, but it emits three extra
`dir` stores retail does not have); `do { } while (0)` around the three
component assignments.

## nbCameraBoss_Kagu_E909_Run (newbattle/nbCameraBoss, 0x2D4)

44 of 180 words differ; 720 bytes emitted against retail's 724.

Everything up to the shared tail matches, including the two-case `switch` on
`pNbCamera.frame` that puts both arms out of line, and the four interpolation
endpoints in `D_00325A00`, `D_00325A10`, `nbCameraKaguVec` and
`nbCameraKaguVec2`.

The one missing word is a register copy. Retail loads the frame counter with
`lw $2, %gp_rel(D_00551E80)`, then `daddu $4, $2, $0` before `mtc1 $4, $f0` and
`cvt.s.w`, and both the conversion and the trailing `addiu $2, $4, 1` read `$4`,
so `$2` dies at the copy. The draft loads straight into `$4` and converts that,
one word shorter, and every word after shifts.

Ruled out: reading the global once against twice; `frame / (f32)len` without
the cast, `(f32)frame` assigned to a float local first, and a second `s32`
local copied from `frame` before the cast (all coalesce the same way);
`frame = frame + 1; D_00551E80 = frame;` instead of one expression;
`D_00551E80++`; `u32` for the counter (which reverses the clamp and runs two
words long).

## nbCameraBoss_Kagu_MJINKO_Set (newbattle/nbCameraBoss, 0x31C)

39 of 199 words differ at retail's exact 796 bytes.

The dispatch, the three arms, the four stacked interpolation endpoints and every
float literal match. `u32` on `nbCameraBoss_Kagu_MJINKO_Type` is what drops the
range test out of the three-case `switch`, the way it does in
`nbCameraBoss_Beru_HAENO1_Set`.

What is left is one instruction's placement. The arms for type 0 and type 1 end
with the same eight-statement tail, which gcc cross-jumps into one block.
Retail's merged block begins with the `lui`/`mtc1` pair that materialises the
`1.0f` for `distance`, so ee-as finds nothing to put in the delay slot of the
`b` that reaches it and leaves a `nop`. The draft's merged block begins with the
six `lwc1` loads of `target` and `direction`, ee-as hoists the first load into
that delay slot, and every word of the tail then sits one position earlier.

Ruled out: `distance = 1.0f` before, between and after the three component adds
and the `fovy` store (all six orders of those three statements); computing the
three sums into float locals and storing them after the `fovy` store.

## nbCameraBoss_Cut2_Init (newbattle/nbCameraBoss, 0x3A8)

179 of 230 words differ at 920 bytes against 936, but the body is right: the
jump table over model IDs `0xCB` to `0x161`, every arm's skill test and every
`.lit4` entry line up, and the 179 are almost all branch displacements carried
by a uniform four-word shift.

Retail leaves a `nop` in four branch delay slots, at `+0x38`, `+0xD0`, `+0xEC`
and `+0x250`, where the draft fills the slot with the arm's next comparison
constant. Retail materialises every comparison constant in `$2` and reaches
the shared `return 0` with `bnel` and the zero in the annulled slot; cc1 gives
the second constant of each pair `$3`, which makes the `addiu` legal in the
preceding branch's delay slot and lets `relax_delay_slots` drop the annul bit.
Both halves follow from one register assignment, so this closes as one change
or not at all.

Ruled out: `||` against `&&` against nested `if` for the two-constant arms,
`s32` for `id` and `skill`, four declaration orders, a one-pass `do` block
around the switch, and renaming `nbCameraBoss_Foru_SIMON_Set`'s definition so
only the `extern` remains, which is the callee-visibility test and changes
nothing here.

Confirmed on the way: the jump table's 151 entries decode to the case list
`0x100`, `0x101`/`0x111`/`0x113`/`0x126`-`0x128`, `0x10D`, `0x11F`, `0x115`,
`0x146`-`0x148`, `0x120`, `0x124`, `0x103`, `0x123`, `0x102`, `0x125`,
`0x108`, `0x156`-`0x158`, `0xCB`/`0x161` in that source order; retail calls
`nbCameraBoss_Sp_JAREIH_Set` for skill `0x99` where the HD rebuild just
returns 1; and the four pooled constants are `0.55f`, `0.85f`, `0.55f` and
`0.9f` in code order.

## nbCameraBoss_BSARI_MASEHA_Run (newbattle/nbCameraBoss, 0x440)

All 272 words match at 1088 bytes. The interpolation helper now takes the
actual `f32 rate` through its existing GPR input constraint. ee-gcc transfers
its binary32 representation without a numeric conversion, following DDS's
"Float inputs through GPR constraints" idiom. The separate integer bits
local and explicit transfer outputs are unnecessary. Rate still has its real
one- or two-interpolation consumers and never crosses the mark-position call.

Only the first lerp arm places its unchanged signed-frame conversion,
subtraction and division after the six private endpoint-coordinate stores.
This crosses no callback, global store or floating-point operation on those
coordinates. All endpoint values, native FP operations, scalar accesses,
callback order, post-callback target reload and final signed-frame reload /
increment remain. The hardware-only scratch w lanes retain their native
unspecified incoming contents; no initialization or C read was invented.

The original explicit-bits draft differs by 66 words in the current owner.
Direct float inputs alone reduce this to 12 words, confined to the first
lerp arm. Moving that arm's rate expression completes the match. Moving both
analogous expressions instead leaves 20 differences. These paired compiler
outputs establish the source-form dependency, not a claimed internal
optimizer-pass mechanism or original source spelling. No new constraint,
header, declaration, source-owner or flag change is needed.

### Paired MASEHA / Cut1 qualification

Together these promotions add 1,564 matching C bytes. The complete owner
passes 111 match / 0 differ independently. Both production
targets preserve the native fallback's resolved instruction and relocation
contracts and extents. All 124 production, 109 SKIP_ASM and 124 retained-C
peers preserve their resolved contracts, including Lusi and BSKAGU_EP_IT.
Nonliteral owned data are unchanged in all modes. Production's literal pool
is byte-identical; SKIP_ASM preserves its previous literal sequence and adds
exactly the target's 236 literal bytes. Retained C reorders eight literal
entries without changing their multiset or any peer's resolved values.

All six baseline/final, production/SKIP_ASM/retained complete plain/dump
assembly and ELF pairs agree. A fixture extracts the actual MASEHA and Cut1
bodies and passes 494,760 cases at -O2 and under address/undefined sanitizers:
negative and boundary frames, invalid types, callback mutations and ordering,
initialized xyz outputs, all model/skill combinations in the dispatch range,
and four distinct callee return values. The fixture is a C control/data-flow
oracle, not a hardware VU rounding or unspecified-w oracle. Native exactness
supplies the target-instruction evidence. Independent review checks provider
ABIs, native memory operations and the real caller's return-value use.

Integrated full-retail checksum and repository gates remain required for
landing. Local frozen source SHA256:
`19f5849760bd759fd542274301344ea4269075ad149e066a4c25d7a14b890139`.

## nbCameraBoss_BSKAGU_EP_IT_Set (newbattle/nbCameraBoss, 0x144)

All 81 words match at the native 324-byte extent. Two RNG arms supply real odd
and even choices to one shared conditional selection. For cut flag 1, odd/even
selects 0/3; otherwise it selects 1/2. Exactly one RNG call executes, and all
four coordinate cases, scalar store order, default exit and return 1 remain.

Two localized constraints preserve the native allocation. An initialized
unsigned one has real consumers in the cut-flag comparison and the second
arm's odd choice. A nonvolatile empty live operand immediately before the
comparison keeps that actual value in a register across its uses. It emits no
instruction and is not a memory barrier; the complete output verifies that
the flag load still follows model-show callbacks. The selector is locally
bound to register 4, but its value is assigned only after each RNG call and
is consumed before any further call. No fixed-register value crosses a call.
These are target-specific reconstruction aids, not original source spelling.

Ordinary shared selection loses the saved-one/frame shape and emits 320 bytes.
Removing only the live operand has the same failure. Keeping the live operand
but removing the selector binding restores the extent while leaving nine
allocation-dependent words different. The original draft is 78/81, correcting
its obsolete 19/79 comment. Qualified complete plain/dump pairs establish the
shared conditional in initial RTL without claiming a debugger-hook mechanism.

The RNG parameter declaration now matches its actual void-pointer provider;
its unsigned result and real 16-byte state are unchanged. This declaration-only
correction is output-neutral in production, SKIP_ASM and retained-C contexts.
Independent review checks the actual scalar lifetimes, callers, provider ABI,
all eight camera stores and all 25 native literal addresses and bit patterns.
Production and retained literal pools are identical. SKIP_ASM adds exactly the
100 native bytes for this target, with every prior literal preserved.

Both complete-owner checkers give 109 match, 0 differ. All 125 production,
108 SKIP_ASM and 125 retained peers preserve extents, instructions, owned data
and resolved relocation targets. Final source and both minimal ablations pass
strict three-mode complete plain/dump assembly and ELF parity. RNG and model
show provider owners also pass independently. No header, layout, source owner
or compiler flag changes are needed. Integrated retail and regression
qualification is recorded with the landing.

### nbCameraBoss_Command_Init (0x001C11F8, 460 bytes)

All 115 target words match. The formation-type switch preserves the native
9/10/13 dispatch graph; the ten-entry scan uses its native unsigned loop
counter. A shared epilogue returns either zero or the selected camera
provider's actual result. A localized binding keeps that result in the ABI
result register `$2`, avoiding an extra result copy and preserving the common
return path. Each assigned result is immediately returned; none is live
across a later call. This is a target-specific reconstruction aid, not a
claim about historical source spelling. Without the binding, the same C
control flow emits 456 bytes and does not match.

The initial null/order-index checks, active-bit tests, provider calls and
return values are preserved. The formation base is reloaded after an
unsuccessful ID-provider call, so a callback that changes that base still
selects the same subsequent ten records. The first matching record wins.

The complete owner passes 116 matches and zero differences with owned data
checked. Eight full raw assembly/ELF parity comparisons pass across normal
and diagnostic compilers in production and SKIP_ASM modes. An independently
transcribed native-call model agrees with the extracted candidate on 250,000
fixtures each at O0, O2, under UBSan and ASan/UBSan. Fixtures include null and
invalid-index inputs, every dispatch ID, inactive records, callback-induced
formation-base changes, arbitrary callback results and camera writes.
Leak checking is disabled for the ASan run because the environment's
LeakSanitizer cannot operate under its process tracing; the model allocates
no heap memory. These host fixtures supplement, not replace, native exactness.

Integrated retail-image equality and all repository gates remain required
before publication.

Independent review adds 300,000 native-control and effectful-callback cases
at O0, O2 and ASan/UBSan, with three deliberately incorrect variants rejected.
No private trace or fixture binary is included in this source change.

### Foru E913 guarded interpolation reconstruction (2026-11-14)

The retained `nbCameraBoss_Foru_E913_Run` draft now emits the native 196 bytes
and matches 43/49 native words, versus 200 bytes and 6/49 for the earlier draft.
It remains guarded and earns no exact-match credit. A single ordinary camera
pointer supplies both vector destinations and the distance/fovy fields. A
localized unsigned frame-capture binding and empty operand constraint preserve
the native load/copy distinction; the copied frame has real signed conversion
and final increment consumers. The capture variable is reused for the final
unsigned increment only after interpolation, with a fresh constraint for its
store. Its two disjoint live ranges do not cross the VU macros' address loads.
This is a target-specific allocation aid, not original-source evidence.

The signed low-word-to-float conversion, division by 190, upper clamp at one,
two four-lane interpolation sequences, scalar camera stores and modulo-32 frame
increment are unchanged. Existing VU macros and literal values are unchanged.
All six residual words are in entry scheduling; the interpolation and completion
sequence matches. The ordinary pointer-only version emits 192 bytes; unsigned
capture without the binding also loses the native copy. Volatile constraints
worsen the result. Do not infer a compiler scheduling cause from these scores
without a separately qualified trace.


### Foru E913 entry scheduler priorities (2026-11-14)

A qualified complete-owner replay now identifies the deciding comparator tier
for the guarded candidate's remaining entry ordering. The camera-address HIGH
has priority 1; the division literal has priority 14, the frame copy 17, the
signed conversion 16 and the clamp literal 6. The first scheduler directly
compares camera HIGH against the division literal with result -13, selecting
by priority. It compares clamp literal against conversion with result -10,
also by priority. This is not a final ready-list tie or declaration-order
choice. The camera HIGH is already ready at cycle zero but issues at cycle
three, after the frame copy and conversion chain. Camera LOW follows at cycle
four and later moves into the branch delay slot.

The original compiler and instrumented replay produce identical complete
assembly, object and diagnostic artifacts; direct/wrapper and plain/dump
assembly/object parity also pass. The source remains guarded at 43/49 words
and 196 bytes. This explains the retained candidate's compiler behavior, not
historical source spelling. Adding an artificial dependency merely to inflate
the camera-address priority would not be justified. Further source work needs
a real camera consumer or matched source-family evidence that changes the
relevant block shape, rather than another pointer-declaration permutation.

### Kagu E909 shared-tail capture qualification (2026-11-14)

The guarded E909 draft now emits the native 724 bytes, with 172/181 words
matching (nine scheduling differences remain), versus 136/180 at 720 bytes.
A short signed frame capture in register 2 restores the native copy to the
ordinary frame value consumed by both conversion and increment. Its lifetime
starts after all switch-arm calls and ends before the VU scratch operations
clobber register 2. This localized constraint is a reconstruction aid, not
original-source evidence. Both switch arms and the signed floating-point
conversion remain unchanged.

Separately, spelling the increment as `(s32)((u32)frame + 1u)` defines the
native low-32-bit wrap at `INT_MAX`; the wrap-only control emits identical
output, as do capture-only and capture-plus-wrap. The first seven residual
words are shared-tail scheduling around duration conversion, the clamp literal
and the frame copy; the final two exchange the end-distance load and increment.
The assembly fallback remains in production; no exact-match credit is claimed.

### Kagu E909 qualified ready-list comparisons

A qualified observation of the retained 172/181-word candidate identifies the
first two shared-tail choices as critical-path priority decisions: the real
frame copy has priority 18 versus duration conversion's 17, and frame conversion
has 17 versus the clamp literal's 7. Neither comparison reaches a tie-breaker.
A bounded control retaining the signed duration conversion in a local float
with an empty float operand constraint regresses to 171/181 words at 724 bytes;
it is not retained.

The final increment/load exchange is different. The end-distance load has
priority 16 and wins its comparison against the increment's 3. Nevertheless,
after the start-distance load issues at cycle 6, the increment issues in that
cycle and the end-distance load waits until cycle 7. Thus the final order cannot
be explained by the increment winning the observed priority comparison. The
observation does not identify the intervening issue-eligibility decision; that
is the next diagnostic, not another declaration-order permutation.

The observer preserves all six output artifacts; direct replay agrees with the
ordinary wrapper. Complete-owner assembly and objects are byte-equal in plain
and dump builds for production, SKIP_ASM, and active-draft modes. This is compiler
evidence, not a new match or evidence of historical source spelling.


### Kagu E909 qualified issue-eligibility decision

A fresh read-only observation of the retained 172/181-word candidate resolves
that intervening decision. In the compiler's pre-reload scheduler, the
start-distance load issues at cycle 6 on modeled unit 0. The end-distance load
then receives hazard cost 1 on that same unit and is queued for the next cycle;
the clamp literal is deferred the same way. The increment uses modeled unit 6,
receives cost 0, and can issue at cycle 6 despite its lower priority. At cycle 7
the end-distance load receives cost 0 and issues; the literal then receives cost
1 again and waits until cycle 8.

The positive-unit hazard calculation is inlined in the scheduling block, so
observing only the out-of-line hazard function misses these decisions. The
qualified observation checks the inline decision and queue insertion as well as
the helper's entry and exit. All six observed output artifacts remain identical
to the unobserved replay, which also agrees with the ordinary wrapper. Fresh
complete-owner assembly and object comparisons pass plain/dump parity in
production, SKIP_ASM, and active-draft modes.

This establishes modeled resource eligibility, not a priority reversal, for the
final increment/load exchange. It does not establish actual PS2 timing, identify
a valid C-level remedy, or improve the match. A further control needs evidence
that it changes this eligibility without artificial dependencies; repeating
priority-only or declaration-order controls is not justified by this result.

**DANTE Init ordinary control-flow pass (2026-11-14).** The guarded draft now
uses the native unsigned invocation-ID switch and explicit failed skill cases,
separate bad-status/state exits, and the native per-arm addition of +/-300 to
the Z position. This preserves the accepted ID/skill sets, getter and callback
order, repeated target acquisition, VU operations and asymmetric radius formula.
No constraint, type, provider or unit-boundary change is introduced. It improves
from 41/155 words at 620 bytes to 96/161 at native 644 bytes, but remains guarded
and nonmatching. The first residual divergence is the special-target branch:
the compiler shares its final Look call with the other case, unlike retail.
A target-ID switch was tested and rejected (48 words, 668 bytes).

**DANTE Init qualified cross-jump observation (2026-11-14).** A repaired,
read-only observer now follows the final jump pass with shallow RTL kind/UID
reads and hooks gated to this function. Its 103 events include 45 paired
cross-jump searches and four applications. In particular, the search between
jump UIDs 218 and 410 with minimum 2 returns call UIDs 213 and 405; the next
application consumes that pair. The ordinary sched2 dump identifies both calls
as `effBTLModelLook`; jump2 replaces the first path with a jump to a new label
immediately before the retained second call. This directly qualifies the
previously localized tail-sharing decision rather than merely inferring it
from the final assembly.

All six observed artifacts equal the unobserved replay, and its assembly and
object equal the ordinary active-draft wrapper outputs. Fresh complete-owner
plain/dump assembly and object parity also passes in production, SKIP_ASM and
active-draft modes. The source is unchanged; no match improvement or C-level
remedy is claimed. The next useful control must address why these two call
tails are equivalent to this compiler while retail retains them separately,
without changing callback semantics or introducing dummy dependencies. The
failed rich-decoder observation is superseded only for this qualified replay.
