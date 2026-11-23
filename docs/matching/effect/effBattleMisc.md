# Matching notes: effect/effBattleMisc

## effBTLModelGroupGetSphereExp (effect/effBattleMisc, 0x270)

The active C matches all 156 retail words, with 80 matching functions
and no differences in the complete owner. The function scans 32 formation
records in pointer order, filtering by group, active and (when requested)
model bits. It obtains each selected center through VU0, then reads radius,
scale and height. The first selected record initializes the bounds; later
records update each maximum before its minimum with strict float comparisons.
The final Y midpoint independently negates height before multiplying by
positive one-half. Empty groups retain zero extents and negative-zero Y.

The earlier 152/156 reconstruction issued the Z sum before the Y negation:
a pinned pre-reload observation showed equal priority 9, but register-pressure
weights -1 and +1 respectively. The sum killed two real bounds, whereas
height was still needed for its positive half-extent. The live X difference
and Z sum share the native scalar register in the exact output. Computing
the half-X extent consumes that register before it is reused for the Z sum.
This real register reuse postpones the Z addition without adding arithmetic,
an empty operand, or any memory access. Keeping the real Z maximum in its
native callee-saved register also restores the scan allocation. Both bindings
are needed in this reconstruction: removing span alone emits 620 bytes;
removing the Z maximum binding alone retains 624 bytes but exchanges scan
bounds and loses the full match. Stock plain/dump assembly and object
contracts agree, as do the owned data and VU load/store contracts.

Before the midpoint fix, a separate result scalar coalesced with another
half-extent and emitted 620 bytes. A qualified allocation/scheduling probe
tracked the existing per-record and final radius phases to the same pseudo,
while preserving hooked/unhooked and plain/dumped output. The exact source
retains this already-established radius reuse.

Independent optimized and ASan/UBSan actual-body fixtures each pass
131,072 cases, including callback changes to current radius and scale and
later formation flags. They check 32-row filtering, the full four-component
VU state handoff and the returned radius. Host checks neutralize EE register
allocation; the native owner and retail checks qualify EE output.

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
same physical registers and frame. At this historical RTL-only stage, no
internal ranking or tie-break cause was proved, and no further controls
were run. The pair was rejected, not retained or promoted. Later live
measurements and two rejected controls are recorded below.

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
GDB was unavailable during this historical investigation, so its evidence
was limited to qualified RTL passes. The later QEMU observation below
resolves scheduler comparisons, not original source spelling.

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

### Later qualified scheduler and allocation observations (2026-11-03)

Fresh complete-owner checks reproduce the baseline at 142/146 words and
historical two-operand pair at 140/146, both 584 bytes with 77 exact peers.
The [upstream QEMU observer](../../cc1probe.md#qemu-observation-without-gdb)
provides actual comparator and issue records without GDB. For each of these
cases and the two controls below, all 5,565 frozen input hashes and all 28
fresh baseline/observed artifacts agree, including whole assembly/object,
22 RTL dumps and four logs. Ordinary wrapper, direct plain and direct dump
assembly/object also agree. All four target passes enter and exit, 34
exercised hooks pass opcode checks, and compilation ends normally (`W00`).
Owned loopback listeners and compiler processes are absent after cleanup.
This qualifies these allocation-mode cases, not every hook or source.

- Before reload, baseline group-copy/front-zero priorities are 3/3;
  register weights 0/1 select group first. In the historical pair, front
  priority rises to 4 while group remains 3, so priority selects front.
  After reload, baseline save-group/save-front ties resolve by LUID;
  the pair instead selects save-front and front-zero by priority. The
  deciding tiers are measured, but priority-construction recursion was
  not traced: a unique cause for the increase is not established.
- Both cases select the Z sum store first by register weight (-1 versus
  X/Y at 0), then X before Y by LUID. Postreload LUID preserves Z,X,Y.
  This explains a pre-existing mismatch, not a new pair regression.
- Interleaving the real copy between the two operands restores 3/3 and
  group-first scheduling, but the total copy moves after the call.
  Total's compiler live length falls from 172 to 84 and calls crossed
  from two to one. Allocation priorities reorder total/top/front to
  f20/f21/f22 and also reorder back and the loop-index pseudo. The result
  is rejected at **119/146 relocation-aware words**, not the raw 117/146.
- Making only the interleaved total operand volatile restores a before-call
  anti-dependence and two calls crossed, but total's live length is 83
  and the wrong FPR allocation persists. This control is rejected at
  **106/146 relocation-aware words**. Before-call placement alone is
  insufficient. Both controls remain 584 bytes with 77 exact peers.

The compiler live-length fields are not C lifetime proofs. Grouped operands,
a group/register constraint, or textual postcall placement have no proved
matching rationale here. The conditional Z,Y,X second controls were never
run because their eligibility checks failed. No source candidate is retained;
the 142/146 baseline remains. No new exact match, original-source recovery,
full retail relink or regression pass is claimed for these rejected sources.
The raw W/flags, valid-object, alias/order and mixed-pointer qualifications
above still apply; three incompatible-pointer warnings persist. Neither
empty operands nor volatility establish a general memory barrier or portable
whole-call semantics.

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

## Battle-effect position dispatch

The six native slots of GetPositionPtrTable select a provider using the
aim request's scale kind. The consumer passes a formation and an aim
request and reads the resulting vector from vf10. Both party providers
ignore the formation: their native bodies preserve the request in a
saved register, call the corresponding party selector, and forward its
return value and that same request to the shared party provider.
The final two slots intentionally share the model-mark provider.

The shared callback type expresses the formation and request arguments
without casts. The complete consumer object remains unchanged in all four
compilation modes, and all six table relocations target the original
functions in native order.

## Formation part center offsets

The native reset initializes the VU0 source vector to zero and loops over
32 stores of one quadword each, advancing the address by 16 bytes on each
iteration. The single-formation reader indexes the same array by the
formation index. The C definition owns the exact native zero-fill range
`0x569A20..0x569C20`. The preceding model-show flag and neighboring raw bytes remain
outside this range, and the neighboring point-light storage keeps its
existing C owner. No formation consumers or their declarations change.
