# Matching notes: newbattle/nbCameraSkill

## nbCameraShot_PullOne_Init (newbattle/nbCameraSkill, 0xE8)

58 of 58 words match at the native 232-byte size. The complete owner has
35 matching functions and exact owned data, including the separate
SkillStart promotion below.

Retail stores the computed distance after `div.s`, then zeroes `angles.x`
and `angles.z`, before loading the formation rotation into VU0. The earlier
56/58-word draft used z-then-x source order and scheduled both zero stores
before the distance store. The matched form uses ordinary x-then-z
initialization and one `FAKEMATCH_ASM_LIVE(pNbCamera)` immediately after the
distance assignment. Removing only that operand constraint, with the same
x-then-z source, yields 55/58 at 232 bytes: only the three store words move.
Do not confuse that control with the earlier z-then-x draft.

The incoming camera pointer is already initialized and remains live for the
later direction stores and VU result. The empty tied register operand emits
no instructions, adds no memory accesses or runtime consumer, and does not extend
its lifetime. It is a localized scheduling aid, not a memory barrier or
original-source evidence. The actual pointer remains in callee-saved `$17`
through both calls, with the native 48-byte frame and saved-register layout.

Qualified compiler snapshots locate the store-order difference in the first
scheduler, rather than only the second scheduler as previously recorded.
Both inputs put the distance store before the zero stores in regmove.
With the constraint, sched1 records output dependencies from both zero
stores to the distance store; without it, those edges are absent and the
zeros move ahead. Sched2 preserves each order. These are observed dependency
lists and schedules, not an explanation of an unobserved alias-analysis
routine or a general promise about empty assembly.

`SetRotateMatrixXYZ` now has its provider's exact
`void(const sdfVector3_t *)` declaration. All six angle locals passed to it
are actual `sdfVector3_t` objects: each uses only XYZ scalar accesses and
this call, with no W member or VU transfer. This replaces the old four-vector
declaration without casts. The declaration and all six local-type changes
are complete-output neutral in production, `SKIP_ASM`, and
`NON_MATCHING`/`SKIP_ASM`, for both constrained and unconstrained controls.
The five existing callers receive no matching credit.

The radius/height comparison, scale product, division, negated Euler angle,
constants and call order remain native. All XYZ components are initialized
before the rotation provider reads them. The formation rotation and camera
direction retain their native quadword alignment and VU load/apply/store
operations. Direction stores remain x/z/y/w in source and native order.

Independent semantic/provider review, complete-owner/data checking,
three-mode implicit-declaration checks and full plain/nohook-dump output
parity pass. Full retail checksum and regression qualification accompany
the landing. Earlier statement-order, extra-zero, volatile-vector and load
barrier controls remain historical negatives; none is retained here.

## nbCameraState_SkillStart_Init (newbattle/nbCameraSkill, 0x78)

30 of 30 words match at the native 120-byte size. The complete owner has
34 matching functions, including this promotion, with owned data exact.

The ordinary draft matched 29 words. At +0x48 it emitted
`addu $16, $2, $16`, while retail uses `addu $16, $16, $2`: the same live
base and scaled index, destination and following `lbu 0x2`, with only the
commutative source operands exchanged. The local pointer base preserves
`datSkillTable + 0x4388` in the relocation addend; direct subscripting folds
that offset into the byte-load displacement instead.

One `FAKEMATCH_ASM_LIVE_VOLATILE(pVisual)` after `nbCamera_SkillInfo_Set`
and before the second `effBTLSkillGetID` preserves the native operand order.
The pointer is already initialized from the real embedded visual array and
already lives across these calls to feed the indexed `hatudo` byte read.
The empty tied operand emits no instructions, adds no memory accesses or
barrier claim, and does not extend that lifetime. The final code keeps the
pointer in callee-saved `$16`, preserves both skill-ID queries and their
intervening setup call, and retains the conditional `nbCameraSetState(7)`
tail call. This is the project's localized compiler constraint, not evidence
of the original source spelling.

Paired compiler snapshots locate the change at the first CSE pass: the
ordinary control begins with base-plus-index and becomes index-plus-base;
the constrained form retains base-plus-index through allocation. This does
not identify an internal comparator or backend decision. Removing only the
constraint reproduces the sole +0x48 mismatch. A nonvolatile operand and
other tested placements did not match and are not retained.

Earlier ordinary controls also remain negative: pointer addition and
subtraction spellings, byte or unsigned address views, separate scaled
offsets, reversed source operands and direct indexing. Their failure does
not justify changing the actual pointer type or either provider call.

The existing `sdk_libc.h` supplies provider-truthful `printf` and `sprintf`
declarations, fixing an inherited implicit `printf` in `SkillCut2_Init`.
That header is output-neutral in production, `SKIP_ASM`, and
`NON_MATCHING`/`SKIP_ASM`; only generated scratch-file names differ in debug
metadata. All three modes pass implicit-declaration errors and full
ordinary/nohook/diagnostic output comparisons. Independent review confirms
the pointer lifetime, provider types, calls, native byte read and complete
owner. Full retail checksum and regression qualification accompany the
landing; no compiler flags, ABI or shared header changed.

## nbCameraShot_All_Set2 (newbattle/nbCameraSkill, 0x248)

16 of 125 words match, 500 bytes against retail's 584.

The structure is mapped and believed correct; what the draft cannot reproduce
is one instruction, and losing it costs the 84 bytes that feed it. After the
formation loop retail emits a bare `c.lt.s $f20, $f21` (the enemy-height
clamp) with no branch after it and no later reader of the condition: the
compare is dead, and `$f20` (the accumulated width) is overwritten with
`0.5f` at `0x001B6424`. `nbCameraShot_All_Set3` is the same function
over a caller-supplied target vector and has the corresponding dead compare
with `$f23` as its second operand, so it is not a one-off. Direct retail
decoding confirms Set2 word `0x4615A034` at `0x001B640C` and Set3 word
`0x4617A034` at `0x001B665C`; the former was previously mislabelled here.

Written the obvious way, `if (total < maxEnemyHeight) { total = maxEnemyHeight; }`,
ee-gcc sees `total`'s only consumer is that compare, deletes the assignment,
the compare and then the whole accumulation: the `r * scl` sum in the loop,
the `nbGetEnemyInterval()` multiply and the unsigned-to-float conversion all
go, which is exactly the 84 missing bytes. A source shape that retains the
branch until after the relevant dead-code elimination is a working hypothesis,
not proof of the original source or compiler invocation.

Ruled out: an empty `if` body (drops even more), `total = (total <
maxEnemyHeight) ? maxEnemyHeight : total`, `!(total >= maxEnemyHeight)`,
`maxEnemyHeight > total`, and assigning to a third dead local in the arm. The
one shape that keeps the accumulation is `else { maxEnemyHeight = total; }`,
which reaches 68 of 146 words at 588 bytes (four over) and changes the
meaning, so it is not the draft.

What the draft does have right, and is worth keeping: the four zeroed `f32`
accumulators as `maxPlayerHeight = 0.0f;` plus three copies from it, which
gives retail's one `mtc1 $0` and three `mov.s`; `width = r * scl; width +=
width; total += width;` for the in-place double; `(u32)(enemies - 1)` for the
`bltz`/`srl`/`andi`/`or` unsigned-to-float conversion; retail's reuse of `$f20`
for accumulated width, then `0.5f`, then the `SDF_VU0_GETLENGTH` result; and
`maxEnemyHeight` reused for the
`sdfAtan(0.5235987f)` result so the quotient divisor stays in `$f21`.
The current draft uses separate `total` and `value` locals; an actual-phase
reuse control remains output-neutral.

### Qualified postreload deletion boundary (2026-11-05)

In the current complete owner, the clamp assignment has disappeared by the
first life-analysis dump. Compare UID 214, conditional branch 215 and the
width/interval accumulation still survive in the global-allocation dump.
A read-only observation of the pinned compiler separates the subsequent
operations that share the next dump interval:

- Postreload `jump_optimize` deletes branch 215 through `delete_jump`,
  `delete_computation` and `delete_insn`. Compare 214 and its feeders remain.
- `cleanup_cfg` leaves that compare and chain present.
- The following `life_analysis` removes compare 214 through
  `propagate_block_delete_insn` and `flow_delete_insn`, then removes total
  accumulation 210, interval product 209 and the conversion/width feeders.

The observed jump site queries the previous active instruction at the branch
target and compares it with the current jump before deletion. The arm already
contains no active instruction. This supports empty-branch collapse followed
by dead-chain removal; no unobserved local flag or unavailable vendor source
is assigned a meaning here. Cleanup and the final delay-slot pass are not
the deleting pass for this draft.

Qualification uses compiler SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
The run records 45 target-gated events and verifies the original bytes at all
eleven hook sites. All 28 baseline/observed artifact pairs agree, including full-owner
assembly, object, RTL dumps and logs. Fresh ordinary-wrapper, direct-plain,
direct-dumped and observed assembly also agree after only generated `.file`
scratch-path normalization. Independent review rehashed all 56 paired files,
checked the hook bytes and corrected RTL/argument decoding, and corroborated
the before/after instruction chains. This reviews the qualified run; it is
not a second hooked execution.

One new ordinary control has specific matched-camera-family support:
`nbCameraBoss_Sp_MURET2_Set` selects a half-height or half-width radius in two
arms. Applying that copied selection before Set2's existing radius overwrite
adds no consumer, but both assignments disappear before postreload. The
control retains 500 bytes and the original complete-owner allocated-section
bytes, with 35 exact peers. Fresh full plain/dump assembly parity passes.
It therefore supplies no missing active-block property. Matched `nbCalc`
equal-arm cases show that later merging is possible, but do not justify
duplicating unrelated camera calls or stores solely to retain a predicate.

Keep the fallback. The unresolved requirement is a native-supported ordinary
block shape that survives this branch-collapse boundary without changing the
algorithm. Repeating clamp spellings, adding a dummy live operand, or importing
the sibling's real radius consumer does not establish such a shape. This
investigation changes no source, compiler flags, owned data or matching credit.

## nbCameraState_SkillAttack_Init (newbattle/nbCameraSkill, 0x224)

137 of 137 words match at the native 548-byte size. The complete owner
has 36 matching functions and exact owned data.

The frame uses the existing primary `nbCameraSkillPtr_t` between `look`
and `work`, rather than the retained draft's oversized eight-word view.
The live first random index is stored at stack+0x18 and read back after
the second RNG call. Both native angle lookups use this first index;
the apparent angle-index reuse is preserved. The H/W fields are initialized
to zero and used as the corresponding table indices.

The second `% 14` remainder/store is present in retail but its field has
no native consumer. It copies the immediately adjacent live `% 18`
random-index initialization in the same truthful record. This is the
canonical narrow dead-code copy-shape exception, not an invented consumer:
removing the dead assignment while retaining the second RNG advance loses
the native division/store sequence and yields 524 bytes. The source comment
identifies the native trace and local live initialization. Same-function HD
output corroborates the record role, but is not claimed as independent native
sibling evidence or recovered source spelling.

An ordinary format pointer followed by a separate `fptodp(width)` result
reproduces the native diagnostic argument lifetime. The format high half
occupies the conversion call's delay slot, its low half follows that call,
and the format argument move fills the `printf` delay slot. Both locals have
real consumers; no constraints or altered ABI are used here.

One one-pass block around only `nbCameraShot_InvokeTargetReverse_Set2`
preserves the native arm order. Qualified stock compiler snapshots show
both ordinary and final inputs expanding the near arm before the diagnostic
and reverse arm. The ordinary control reverses them by `rtl.01.sibling`;
the final retains loop-boundary notes around the reverse call and preserves
the near-first order through that stage and the following jump pass. This
is an observed boundary/order dependency, not a claim about an unobserved
internal algorithm. It adds no runtime iteration or operation and is not
evidence of historical source spelling.

Removing only the one-pass block returns the 20/137-word residual at the
same 548-byte size. The format-local-only, near-call-block and explicit
forward-branch controls remain negative. A reverse block without the
format locals gives 540 bytes. Previously tested callee opacity and late
operand constraints do not address this early layout dependency.

The width calculation and ordered `< 1500.0f` comparison preserve the
native NaN/far-arm behavior. The calls, RNG advances, party/pointer caching,
state writes, rotations and quadword look vector remain native. Both near
and reverse providers have their canonical signatures and ignore the work
record; its partial initialization does not supply an uninitialized field
to either implementation.

Independent semantic/provider review and a 112,640-case actual-body host
oracle pass, including callback mutation and NaN/infinity/boundary cases;
this is not native floating-conversion bit emulation. The complete owner,
all unaffected function contracts and owned data pass. Production,
`SKIP_ASM`, and `NON_MATCHING` plus `SKIP_ASM` each pass strict implicit-
declaration checks and complete plain/stock-dump/strict assembly and object
parity. The SKIP-only added diagnostic string and 1500.0f literal preserve
all previous data and relocation targets. Full retail checksum and
regression gates remain required at landing.

## bCameraShot_AttackInvokeTargetPartyNear_Set (newbattle/nbCameraSkill, 0x308)

194 of 194 words match at the native 776-byte size. The frame remains
160 bytes, with no uncovered or excess native bytes.

The three aligned vectors are base position at stack+0, camera position at
+0x10 and temporary center at +0x20. The formation calls produce separate
invoke and target pointers, followed by a party mask. Call delay slots save
preceding provider results; the returned mask is never a formation pointer.

The 32-record scan has separate first-found and strict-lower arms. Only the
first arm clears `first`; it accepts the first eligible height even when it
is negative or NaN. The minimum-height midpoint publishes camera Y before
querying the selected center, then reloads it afterward. The later radius
correction compares camera X to invoke-center X and writes camera X.

Keep the combined `span = length + distance` before the second atan call.
This gives it the native live range across that call; radius, span and distance
remain separate ordinary locals. A radius/span reuse variant emits identical
bytes and was reduced to the separate locals. The final length macro writes
the camera distance field directly before direction normalization; the fovy
store precedes the final distance addition.

One ordinary `f32 angle` holds the two real `sdfAtan` results in separate,
disjoint phases. The first value is the divisor for the radius-derived
distance and dies before the second atan call. The second value is the divisor
for the group bound and, conditionally, the multiplier that corrects the span.
Every use follows the corresponding assignment; no value is expected to
survive a later callback. The native second-phase multiply executes in the
sphere-center call's delay slot. No register constraint or empty assembly
operand is retained, and providers, memory accesses and arithmetic are unchanged.

Separate ordinary angle locals, including separate lexical scopes, give
188/194 words. Reusing the actual angle-result local alone gives 194/194 at
776 bytes. Diagnostic binding of either separate result gives 191/194; binding
both gives 194/194 but is unnecessary and was removed. This records a measured
source/output dependency, not an internal-pass attribution or original-source
claim. See [the geometry-span trace](../../levers/cross-block-locals.md#snapshot-the-camera-span-before-the-angle-call).
