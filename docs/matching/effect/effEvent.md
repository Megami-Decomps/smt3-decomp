# Matching notes: effect/effEvent

## effEveBEDCutDraw (effect/effEvent, 0x1DC)

119 of 119 words match at 476 bytes. Independent complete-owner verification
gives 86 match, 0 differ, including owned data and full compiler context.

The byte observation now occurs inside the non-null `works[0]` guard, as in
retail. A genuine `u32 markKind = 5` comparison value has a local `$2` register
binding and feeds only `kind != markKind`. This retains the native constant
in the null-branch delay slot and the following conditional byte load into
`$3`. No empty asm is needed. The constant may execute on the null path, but
has no effect there; the byte read remains conditional. Its lifetime ends
at the comparison, before every call. Matrix/VU work, callback tests, frame
reloads and updates, and all interfaces remain unchanged.

The ordinary below-guard byte/local-constant form has 116/119 words matching;
removing the binding from the exact source reproduces those three register
differences. Adding a live-operand constraint to the comparison constant is
also exact but unnecessary and omitted. Constraining the loaded byte instead
was not equivalent as a compiler aid: the narrow read/write operand introduced
an extra zero-extension, and a truthful widened comparison snapshot removed
that repair but still changed the branch layout. Those controls are not retained.
This is a localized reconstruction aid on an actual value, not original-source
spelling or an unmeasured internal-pass claim.

Integrated retail SHA1/byte comparison and no-op rebuild, 436 tooling tests,
inline-asm gate, all 143 guarded owners / 499 drafts with stable inputs,
7 IRX checks and reconciled reports pass. Source commit
`e748342375682ca6e80eee0411da3b39639066c0`; exact-source
[CI 41413305990](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41413305990)
attempt 1 passed build and Pages deployment.

### Earlier residual and controls

The earlier draft has 117 of 119 words matching, at the exact 476-byte size. The two differing words are
adjacent and hold the same two instructions in the other order: retail puts
`addiu $2, $0, 5` in the `beqz`'s delay slot and `lbu $3, 0x8($18)` after it,
the draft the reverse. The registers, both operands and every other word are
identical, so what is left is which of two independent instructions the
delay-slot filler takes.

The two are coupled and pull against each other. Reading the compared byte into
a local *above* the `works[0] != NULL` test gives retail's registers (the
constant in `$2`, reusing the dead pointer's register, and the byte in `$3`) and
the wrong order, because the `lbu` is then the last instruction before the
branch and the filler takes it. Reading it *below* the test gives retail's order
and swaps the registers, since the byte's pseudo is then created first and wins
`$2`. The local's type does not matter: `u8`, `s8`, `u16`, `u32` and `s32` all
give the same two spellings.

Ruled out, all 3 of 119 or worse: `5 != kind` and `(u8)5 != kind`;
`!(kind == 5)`; `kind - 5 != 0`; the assignment inside the condition, either
operand order; a `markKind = 5` local, above and below the test, which gcc
hoists out of the loop; an `aim = &entry->aimParams` pointer local (20 words); a
`dds3EffectPart_t *part` local for the tested slot; `*works` for `works[0]`;
reading the byte at the top of the loop body, before the `zeroKindCount` test
(78 words); a `goto` out of the `works[0]` test and a `goto` into the mark arm;
and both switch shapings of the dispatch, `default` first with a `goto` back
(78 words) and `case 5` falling into `default` (41 words).

A current `--sched --reorg --no-dumps --cflag -DNON_MATCHING` trace filtered
to this function passes hooked/unhooked/plain output parity (`verify: OK`).
Sched1 gives the slot load UID86 priority 2 and byte load UID81 priority 0,
then schedules both before null branch UID87. Sched2 retains that block
with priorities 3 and 1; reload has placed constant 5 in UID297 in the next
block. Reorg accepts the already preceding byte load for UID87's delay slot
with flags 0x11. The constant is therefore not an independent ready
instruction competing with the byte load at that point: it is beyond the
branch. A bounded one-pass per-entry block with a null-slot `break` leaves
the same 117/119-word result and was discarded. The earlier source-placement
and type controls remain ruled out; the native ordering was still unresolved by that control. The comparison-value
binding described above closes it without another empty operand.

What the draft establishes, since the rest is exact:

- `D_005343D0` is a quaternion `Mult` combines with the record's own
  `init.orientation` quadword at +0x10, as `D_00532EA0` is in the sibling
  `effBTLCutDraw`. The frame is `sdfVector4_t position` at 0, `sdfMatrix4_t
  base` at 0x10 and `sdfMatrix4_t marked` at 0x50, in that declaration order.
- `effEveBEDCut_t.actor` is an `evtUnit_t *`: the mark arm reads
  `work->actor->owner->inner` and hands it to `sdfModelGetMarkPos` and
  `sdfModelGetMarkPosMatrix`, which pins +0x8C as `evtUnit_t.owner` and +0x18 as
  `mdlObj_t.inner`. The draft needs `include/event.h` and `include/model.h`,
  which the unit did not include before.
- The mark arm is the `else`, not the `if`: written `scaleKind == 5 &&
  actor != NULL` with the mark work in the `then` arm, gcc lays the mark block
  out first and branches over it, where retail falls into the generic block.
  Inverted to `scaleKind != 5 || actor == NULL` it is laid out retail's way,
  which alone moved 30 words.
- The tail is `cut->currentFrame++` with no `effBTLCutSetMax`, and the
  trigger-frame callback is tested as `cut->triggerFrame == frame &&
  cut->triggerFrameCallback != NULL` against the frame read at entry, with
  `currentFrame` re-read afterwards.

The draft depends on `SDF_VU0_STOREMATRIX_RWADDR`, added to `include/ps2asm.h`
for it. The plain and `_NOMEM` matrix stores both share one address pseudo with
the following call's `&marked` argument; that pseudo is loop invariant, gets
hoisted into the preheader, takes the last callee-saved register and pushes
`color` into a stack slot, which is 71 of 119 words and a 16-byte larger frame.
With the address as a read-write asm operand cc1 gives the asm its own copy, so
both uses are rematerialised as retail's two `addiu rd, $29, 0x50`, and the
frame and every register fall into place. `SDF_VU0_STOREMATRIX_MEMOPS` also
frees the register but addresses the frame directly, so the four `sqc2` and the
whole tail shift by one word (46 of 118). A `sdfMatrix4_t *mp = &marked;` local
with either existing macro does not reach it: cse folds the local back into the
shared pseudo.
