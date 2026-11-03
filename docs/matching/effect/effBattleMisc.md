# Matching notes: effect/effBattleMisc

## effBTLModelGroupGetSphereExp (effect/effBattleMisc, 0x270)

6 of 156 words differ at the native 624-byte size. Production
remains assembly-backed. The six residuals are at +0x1C0, +0x1C4, +0x1D0,
+0x1E4, +0x1EC and +0x1F4: the final X/Y/Z midpoint arithmetic and component
stores have a different order. The formation scan, bounds, extent selection,
return and VU0 handoff otherwise match.

The helper visits all 32 formation records in pointer order, filtering by the
group mask and active bit. The model bit is also required when `model != 0`.
Each selected record supplies a transformed center, scaled radius and upper
height. The first selected record initializes the extrema through ordered
comparisons; subsequent records update each maximum before its minimum.
Height is `(h * scl) * 0.5f - center.y`; the final midpoint Y negates the
height before multiplying by positive one-half. Keep those operations and the
strict comparisons: replacing them with algebraic min/max shortcuts changes
unordered and signed-zero cases. An empty group retains zero extents and a
negative-zero midpoint Y.

The radius local holds each model radius during the scan, then the selected
group extent after the scan; these live ranges do not overlap. A qualified
`cc1probe --alloc --sched` run traces both phases to float pseudo 95 and
preserves hooked/unhooked, dumped/plain output parity with compiler SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
A separate result local coalesces with another half-extent and emits 620 bytes;
the reused radius retains the distinct native result register and 624-byte
shape. This follows the canonical live-scalar-reuse permission.

Independent extent/midpoint statement orders do not close the residual. Explicit
midpoint scalar snapshots, aggregate assignment and a memory-operand VU load
leave the same six words. A volatile final output view emits 628 bytes and is
not retained. Reusing the per-model height or X/Z bounds as final half-extents
does not improve the native-size reconstruction. The remaining question is the
ordinary source property that retains the current bound allocation while
scheduling the final midpoint components in X/Y/Z order.

## effBTLModelLook2 (effect/effBattleMisc, 0x110)

2 of 68 words differ, at the exact 272-byte size, and the two words are each other's:
retail emits `abs.s $f1, $f2` then `mov.s $f13, $f0`, the draft the same pair
in the other order. Both are independent single-cycle moves off the two
`lwc1`s that precede them and that the draft already matches, so there is no
dependence to steer: `$f1` is `fabsf(direction.x)` for the compare and `$f13`
is `sdfAtan2`'s second argument copied out of `$f0` before `abs.s` clobbers
it.

Everything else in the function matches, including the `sdfSin`/`sdfCos`
divide into one register, the three-way clamp and the `$f20`/`$f21`/`$f22`
saved-register layout. Two source facts were needed to get there and are worth
keeping:

- The clamped value and the half-width are one variable, not two. Retail's
  `mul.s $f12, $f20, $f0` writes the register that later carries
  `sdfAtan2`'s first argument, so `x = tangent * direction.z;` followed by
  `x = direction.x;` / `x = fabsf(x);` / `x = -fabsf(x);` in the three arms is
  what matches. A separate `width` local coalesces into the tangent's register
  instead and costs seven words.
- `fabsf` declared as a plain `extern f32 fabsf(f32);` compiles to `abs.s`;
  `EE_ABS_F32` does not match here (29 to 31 words differing, and the function
  runs long).

Ruled out: `direction.z * tangent` instead of `tangent * direction.z` (four
words); a named `tangent` local, a named `absx` local, and `absx` computed
before the multiply; `fabsf(x) > fabsf(direction.x)` for the compare;
`!(fabsf(x) <= fabsf(direction.x))`; a `z` local for `direction.z`; and
declaring `x` before or after the other locals.

## effBTLPartPtrModelGroupGetCenterAndHeight (effect/effBattleMisc, 0x248)

142 of 146 words match, at the exact 584-byte size. One instruction is wrong and the
three stores after it are scheduled one slot early as a consequence: retail
rematerialises the zero it stores into the accumulator vector
(`mtc1 $0, $f0`), the draft copies it out of the saved register that already
holds `front` (`mov.s $f0, $f21`). The copy has the same cost but a
dependency, so ee-gcc's scheduler fills the next three slots with the two
`addiu`s and the first `swc1` in the other order.

cc1's copy propagation is what does it: `front = 0.0f;` leaves 0.0 available
in a pseudo that lives across the `nbGetMainProcessData` call, and the later
`zero = 0.0f;` becomes a copy of it. The ordinary-source controls below
do not stop that; the later operand control stops the copy but regresses
other instructions. Ruled out:
spelling the second zero `0.0`, `0`, `-0.0f` (two words, worse), a chained
`sum.x = sum.y = sum.z = 0.0f`, a literal at each of the three stores (which
reverts to the shared register and costs 114 words), `sum.* = front`, and all
120 orderings of the four initialiser groups (`count`/`back`, `front`/`total`,
the `nbGetMainProcessData` call, the `top` select, the `sum` zeros) — six of
them tie at this same 4-word result and the rest are worse.

The rest of the function matches, and four source facts were needed:

- The tail is `if (count != 0) { ... } else { ... }`, not an early return for
  the empty case. Retail branches forward to the empty arm, so the populated
  arm has to be written first; the early-return form differs in 85 words.
- The `sum` zeros come from a named `f32 zero` local that the empty arm also
  stores through. Giving the empty arm its own local costs 114 words.
- `sum.x`, `sum.y`, `sum.z` in that source order; `z, y, x` costs one more
  word.
- `group & 0x100` is a `u32` local, read twice (once for the `top` select,
  once inside the loop), which is what keeps it in a saved register.


### Two-live-value operand control (2026-11-03, rejected)

The complete owner at `04bca9b7494be522d465bd74a1a9ac906ed3fb16`
reproduced the retained draft's 142/146 words at 584 bytes, with 77 exact
peers. One bounded control applied the existing nonvolatile
`FAKEMATCH_ASM_LIVE_F32` separately to initialized `front` and `total`,
immediately before the unchanged `nbGetMainProcessData` call. Both have
real later consumers; neither was fixed to a register or given a memory
clobber. The control restored the independent zero but worsened to 140/146
words, still 584 bytes. Relative to retail, prologue words at +0x04,
+0x08, +0x0C and +0x10 changed, and the sum X/Z stores at +0x80 and +0x84
were exchanged. This store order is not a new regression against the
baseline: both first-scheduler outputs already store Z,X,Y, while native
stores X,Z,Y. The restored zero exposes that pre-existing order at the retail
store slots. Both rtl.16 inputs place group-copy UID4 before front-zero
UID26; rtl.17 linearizes them as 4 then 26 in the baseline and 26 then 4 in
the operand control. This is the earliest observed prologue precursor;
post-reload save/write scheduling yields the four different words with the
same physical registers and frame. No internal ranking or tie-break cause
is proved. Grouping the operands has no supported causal premise, so no new
control was tried. The control was rejected, not retained or promoted. No
further placement, register or store-order controls were run.

Qualified whole-owner plain versus `-da` output establishes the deciding
pass: in the baseline's `rtl.02.jump`, UID57 assigns positive-zero
`const_double` to SF pseudo98; `rtl.03.cse` changes that source to SF
pseudo91 (`front`). UID26 initializes pseudo91, and UID29 copies it to
pseudo92 (`total`). In the operand control, UIDs31/33 constrain those two
real values and UID61 retains positive-zero `const_double` through the
first CSE pass. Thus the first CSE transformation is measured, not inferred
only from final bytes. Plain/dump assembly, allocated sections, non-file
symbols and ordered relocations agree for each complete owner. Comparing
the two controls changes only target text; all allocated nontext data,
non-file symbols and ordered relocations agree. Compiler SHA-256:
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
GDB was unavailable, so this is qualified RTL-pass evidence, not a
qualified debugger-hook log or recovered original source spelling.

The unchanged semantic contract includes exactly 32 formation rows,
selection before each center callback, radius/model observations after
callbacks, strict float comparisons, separate count conversions/divisions,
and native output store order. Valid live formation/model objects,
in-range center-offset indices, writable aligned output objects and the
native alias/order contract remain required. The constraints are not
memory barriers and do not justify moving those observations.

The native sum and scale scratch vectors initialize XYZ only. Raw VU
quadword operations also process W: lane-wise addition/multiplication does
not feed W into XYZ, but the nonempty output includes W and VU flags may
reflect W operations. The known same-owner caller consumes center XYZ,
not W; this does not prove all external W or flag consumers absent. No W
initialization, scalar C read of an uninitialized W lane, deterministic W
result or portable uninitialized-lane semantics is asserted. Preserve the
raw target hardware handoff and its side effects in any future review.

The measured warning modes must not be conflated. Both controls compiled
in ordinary production context and with `-da`, with existing owner
warnings; the `SKIP_ASM` whole-owner checker reported 77 match, 1 differ
for each. A production-context baseline compile with
`-Werror-implicit-function-declaration` exited 33 at the existing
`effBTLModelGroupGetWidthX` call to `effBTLGetGroupPos` (canonical line 342).
Its truthful `extern void effBTLGetGroupPos(nbFormation_t *form, f32 *out)`
exists at canonical line 322 but is inside `#ifdef NON_MATCHING`; its
unconditional definition is at canonical line 757. Consequently production
mode encounters the call before a visible declaration. Existing later
`GroupRotate`/`GroupDiameterDisposition` calls also warn because their
`sdfVector4_t *` arguments do not match that `f32 *` parameter. This control
did not change the declaration, provider or unrelated guards. Strict pair,
strict `SKIP_ASM` and full `NON_MATCHING` modes were not measured in this
investigation. No full retail build or regression pass is claimed for the
rejected source.

### Declaration visibility follow-up

The existing `void effBTLGetGroupPos(nbFormation_t *, f32 *)` declaration is
now visible before the early call; only its NON_MATCHING guard pair was
removed. This is a measured EE/GNU visibility correction, not a body change,
runtime bug repair or certificate of ISO-C pointer validity. The three
calls in this affected owner still pass incompatible `sdfVector4_t *`
arguments and still warn. Native frame locations and the raw quadword
provider store were reviewed; four adjacent float members do not establish
a C float array or a general 16-byte alignment guarantee. Existing mixed
cross-unit declarations remain unresolved and unchanged. No casts, signatures,
layouts or unrelated call sites were modified.

Independent baseline/candidate output contracts and normalized assembly
agree in production, SKIP_ASM and retained modes. Strict implicit-declaration
status changes from 33/33/0 to 0/0/0; this is not blanket warning-free output.
The guarded GroupCenter body and its raw W-lane qualification remain unchanged.

Final integrated owners 77/0 (center), 20/0 (name) and unchanged Hansya
owner 290/0 pass. Fresh stable 140-owner/480-draft qualification, 450 tests,
817 layout assertions, canonical strict/plain/dump parity, inline/shell checks,
seven IRX modules, retail SHA1/byte comparison/no-op and reconciled reports
pass. Coverage remains 8594 functions / 1485744 bytes; zero matching credit.
