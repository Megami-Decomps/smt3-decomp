# Matching notes: newbattle/nbCameraBoss

## nbCameraBoss_Cut1_Run (newbattle/nbCameraBoss, 0x1DC)

After the exact Lusi provider promotion below, a private caller-only promotion
emits the native 476 bytes with 9 of 119 words differing. The complete owner
has 108 match, 1 differ. Production keeps this caller assembly-backed.

The current truthful body assigns each selected callee result to `shot` and
returns it; default paths return zero. The caller's consumer uses that result.
An older experimental fall-through-return description is obsolete and is not
a valid reconstruction to restore. The case order, shared MASEHA/SIMANO calls,
and the final arm's fall-through to default remain native.

The remaining observed differences are MASEHA-arm scheduling at +0xD8..+0xF4
and the ordinary-versus-likely branch at +0xFC. Earlier all-draft compilation
with both guarded Lusi and MASEHA definitions visible produced a code-zero
context diagnostic. That did not authorize promoting either nonmatching
provider or this caller. The new Lusi-only production-context test still
fails, so Lusi's match does not automatically unlock the caller. MASEHA remains
a separate source problem; further context effects must be measured again.

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

68 of 131 words differ; 524 bytes emitted against retail's 536.

The body came off the matched sibling `nbCameraBoss_BSSAMA_Command_Set`, which
has the same shape plus an `effBTLModelGetID` test. Everything up to the
`length` choice matches except two callee-saved names: retail holds
`pNbCamera` in `$18` and the model id in `$17`, the draft the other way round.
That swap is a live-length tie-break and follows the one real difference.

Retail reloads `pos.y` from the frame for the `> -1.5f` clamp after storing all
three components, then uses `bc1tl` with the clamp store in the delay slot. The
draft keeps `pos.y` in a register, so the whole scale-and-add block schedules
`pos.y` first and the clamp folds into it, and retail's `b` to the join plus its
`nop` are three words the draft never emits. The HD reconstruction of this
function also reads `pos.y` back through a separate accessor, so the reload is
in the original, but nothing in C reproduces it here: the matched
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

204 of 272 words match at the exact 1088-byte size. All eight arms, both
shared tails, the `.lit4` order and every literal are right; the 68 are a
one-instruction rotation at the head of each of the three lerp arms, plus the
float registers and the two frame-address temps that rotation drags with it.

Retail opens each lerp arm with `addiu $3, $29, 0x10`, the address the
following `SDF_VU0_LOADVECTOR` wants, and only then materialises the arm's
constants; cc1 schedules that `addiu` last, next to the `lqc2`, which also
swaps `$2` and `$3` between the two vectors. Nothing else in the arm moves.

Ruled out: `PS2_FLOAT_BITS` after the `LOADVECTOR` pair instead of before it
(worth 31 words on its own, so that part is settled), the `rate` division
below the vector stores rather than above them (3 more words), and three
declaration orders for `mark`, `from`, `to`, `bits`, `type` and `rate`.

Confirmed on the way: the dispatch needs a `u32 type` local, because
`switch (nbCameraBoss_BSARI_MASEHA_Type)` on the signed extern makes
`balance_case_nodes` add an `slti` range split that retail does not have, the
same shape the matching `nbCameraBoss_BSARI_SIMANO_Run` next door uses; the
blend factor reaches both lerps of an arm through one `PS2_FLOAT_BITS` GPR;
`nbCameraBoss_BSARI_MASEHA_Frame` is signed, since every frame threshold is
`slti`; and spimdisasm's ten-digit `.lit4` decimals are each one ULP low for
ee-as, which truncates toward zero, so the literals have to be spelled from
the exact float value rounded away from zero.
