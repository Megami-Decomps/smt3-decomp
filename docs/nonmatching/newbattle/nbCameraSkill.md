# Non-matching drafts: newbattle/nbCameraSkill

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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

29 of 30 words match, at the exact 120-byte size.

The one differing word is the `addu` operand order:
retail `addu $16, $16, $2` (base, index), the draft `addu $16, $2, $16`. Both
write the same register and keep the `lbu 0x2` displacement, so only the
commutative operand order inside the address differs.

Retail also keeps `datSkillTable + 0x4388` in the relocation addend, which
only a local pointer base reproduces: `datSkillTable.visual[id].hatudo`
written directly folds the whole `0x4388 + 2` into the `lbu` displacement and
leaves the relocation at addend 0.

Ruled out for the operand order, all giving the identical `addu $16, $2, $16`:
`pVisual += effBTLSkillGetID()`, `pVisual = pVisual + id`,
`pVisual += (s32)id`, `pVisual -= -(s32)id`, a `u8 *` base with
`+= id * 4` and `+= id << 2`, a `u32` base with `+= id * 4`, a separate
`u32 ofs = id * 4` added to either base type, both casts
`(u8 *)pVisual + ofs` and `(u32)pVisual + ofs`, and the reversed
`id * 4 + (u32)pVisual`. Subscripting (`pVisual[id].hatudo`) instead of
assigning the pointer costs two more words by moving the result into `$3`.

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
