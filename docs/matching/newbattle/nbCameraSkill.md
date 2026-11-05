# Matching notes: newbattle/nbCameraSkill

## nbCameraShot_PullOne_Init (newbattle/nbCameraSkill, 0xE8)

56 of 58 words match, at the exact 232-byte size.

One store is scheduled differently. Retail emits `div.s`, then
`swc1 pCamera->distance`, then the two `sw $0` that zero `angles.x` and
`angles.z`, then the `lqc2` of `pForm->rot`. The draft emits the two `sw $0`
first: cc1's second scheduling pass fills the `div.s` latency with them and
sinks the `swc1` behind, which retail's cc1 run did not do.

All six orders of the three statements give either `sw, sw, swc1` or, once
the `SDF_VU0_LOADVECTOR` moves above the two zero stores,
`swc1, lqc2, sw, sw` with the function one word shorter. Also ruled out:
zeroing `angles.y` as well (dead store, one word longer),
`angles.x = angles.z = 0.0f` (one word longer), a `volatile sdfVector4_t`
(longer), `SDF_VU0_LOADVECTOR_BARRIER` in place of the plain load (same
sinking), and writing the zeros through a `sdfVector4_t *` local so the two
stores alias `pCamera->distance` (unchanged).

The rest is exact, including `pForm->r > pForm->h` for the load order of the
compare, the `0.69813163f` and `0.78539807f` pool literals, and the
`-1.0f`/`0.0f` direction stores in `x, z, y, w` source order.

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
formation loop retail emits a bare `c.lt.s $f20, $f23` (the enemy-height
clamp) with no branch after it and no later reader of the condition: the
compare is dead, and `$f20` (the accumulated width) is overwritten with
`0.5f` five instructions later. `nbCameraShot_All_Set3` is the same function
over a caller-supplied target vector and has the identical dead compare, so it
is not a one-off.

Written the obvious way, `if (total < maxEnemyHeight) { total = maxEnemyHeight; }`,
ee-gcc sees `total`'s only consumer is that compare, deletes the assignment,
the compare and then the whole accumulation: the `r * scl` sum in the loop,
the `nbGetEnemyInterval()` multiply and the unsigned-to-float conversion all
go, which is exactly the 84 missing bytes. Retail kept the compare, so in its
compile the branch must have survived to a pass after the last dead-code
elimination.

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
`bltz`/`srl`/`andi`/`or` unsigned-to-float conversion; the single local reused
for the accumulator, then `0.5f`, then the `SDF_VU0_GETLENGTH` output, which
is why all three land in `$f20`; and `maxEnemyHeight` reused for the
`sdfAtan(0.5235987f)` result so the quotient divisor stays in `$f21`.

## nbCameraState_SkillAttack_Init (newbattle/nbCameraSkill, 0x224)

117 of 137 words match, at the exact 548-byte size.

Everything up to the width test matches, including the frame: `sdfVector4_t
look` at `0x0`, an eight-word `u32 ptr[8]` whose members 2 to 5 take the two
`dds3GetRandA` remainders and two zeros at `0x18` through `0x24`, and
`nbCameraSkillInfo_t work` at `0x30`. Reading the first remainder back out of
the aggregate, `D_003246D0[ptr[2]]`, is what gives retail's `sw`/`lw` pair
around the second `dds3GetRandA` and the `addu $1, $1, $2` table rows.

The 20 remaining words are the two arms of `if (width < 1500.0f)`. Retail
emits `bc1f` to the `printf` arm with an empty delay slot and falls through to
`bCameraShot_AttackInvokeTargetPartyNear_Set`; both arms then set `$2 = 0xB`
and cross-jump to one shared `sw $2, nbCameraSkillAttackStep`. The draft gets
the arms the other way round, with `daddu $4, $17` hoisted into a `bc1tl`
delay slot, and the two `nbCameraSkillAttackStep = 0xB;` stores are not
cross-jumped because the constant lands in `$2` in one arm and `$3` in the
other. Retail also keeps `D_005349A0` in `$16` across the `sdfFloatToULong`
call, where the draft materialises it after.

Ruled out for the arm order: inverting the test so the `printf` arm is the
`if` body, hoisting `nbCameraSkillAttackStep = 0xB;` below the `if`/`else`
(47 words out), and a `char *msg = D_005349A0;` local for the format string.
The matching `nbCameraState_SkillAttack_Calc` needed its actual ground-collision
callee visible as C; see [callee visibility](../../levers/callee-visibility.md).

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
