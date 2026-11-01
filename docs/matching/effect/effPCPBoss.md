# Matching notes: effect/effPCPBoss

## effPCPBossKAGUThunderboltCopy (effect/effPCPBoss, 176 bytes)

21 of 44 words match, at the exact 176-byte size.

Register allocation. Retail keeps the source work in `$17` and the new work in
`$16`; the draft has them the other way round, which touches every instruction
that names either. The three sibling copies in the same unit
(`effPCPBossDengekiCopy`, `effPCPBossKAGUAttackCopy`,
`effPCPBossKAGU2AttackCopy`) match with the identical statement shape and the
opposite assignment, so the trigger is the extra `effPCPCrackCopy` call rather
than the body. The leftover two words are a rotation of the `src->bed` load
against the `1.0f` materialisation.

Ruled out: the `effPCPCrackCopy` result stored directly against held in a
named local; reading `src->bed` before the crack copy instead of after;
storing `work->crack` before `work->bed` instead of after.

## effPCP_BED_Create (effect/effPCPBoss, 248 bytes)

33 of 63 words match, one word short of retail's 62.

Retail materialises `0x80808080` twice: once as `lui`/`ori` before the parameter
copy for `work->color`, and again for `place.color`, with the second `ori`
filling the guard's delay slot. ee-gcc commons the two into one pseudo in every
shape tried, and the shared pseudo then survives the two VU0 identity macros in
a register the head does not use, which rotates the whole function.

`place.color = work->color` is what pins down the rest: it kills the colour
pseudo at the `work->color` store, and the head then comes out exact, including
retail's `$2`/`$3`/`$4` in the 12-byte parameter copy and the `addiu $6, $18,
0x40` ahead of the colour store. Only the guarded arm is left, where retail's
`lui`/`ori` pair is one `lw` in the draft and everything after it shifts by a
word. So the structure is settled and the one remaining question is how retail
gets a second constant.

Ruled out for the duplicated constant: the literal in a `u32` local, assigned
at the top of the function, before the two VU0 macros, between them, before the
`if`, at the end of the payload block, inside the guarded block, and on the
declaration (all eight land back on 41 of 62, so the local is CSE'd into the
same pseudo); the `work->color` store in all four positions around the copy and
the macros; the two identity macros in both orders; `place` and `bed` declared
in the guarded block; an early `return work` in place of the `if`;
`SDF_VU0_STOREIDENTITY(work->matrix)` instead of `SDF_VU0_STOREIDENTITY(work)`;
`sizeof` for the allocation size; and the parameter copy as `memcpy`.

Worth knowing: the five sibling `Create` routines in this unit that set both a
work colour and a placement colour all match with one shared constant, so
whatever forces the second materialisation here is local to this function, not
the family.

## effPCPDANTE_ShowTime_Draw (effect/effPCPBoss, 300 bytes)

71 of 75 words match, at the exact 300-byte size.

One instruction's schedule slot. Retail places the `mov.s $f12, $f20` that
passes 0.45f to the first `nbPanelFlashInit` directly after the two `lui`s for
its colour arguments; the draft emits it three slots later, right before the
`jal`, which rotates four words. Everything else is exact, including the three
`.lit4` constants as literals: `D_0054C9D0`, `D_0054C9D4` and `D_0054C9D8` sit
in this unit's pool at file offsets 0x44D9D0, 0x44D9D4 and 0x44D9D8 and hold
0.45f, 0.35f and 0.55f, with 0.45f shared by the first two calls, so ee-gcc
keeps it in `$f20` and the pool gets one entry for it.

Ruled out: 0.45f through a named `f32` local, declared before `frame`, after it,
and at the top of the guarded block. The first two are far worse (67 words),
because the load then happens outside the guard; only the block-scoped one stays
at four.

## effPCPBossMETATOBeamDraw (effect/effPCPBoss, 0x25C)

7 of 151 words differ, at the exact 604-byte size.

Where the two interpolation factors finish being built. Retail materialises both
`0x3EA8F5C2` and `0x3F28F5C2` with `lui`/`ori` pairs before the
`effColorBlend` argument setup, so the `and $4, $5, $4` lands in the call's
delay slot; the draft's scheduler sinks `lui $20` and both `ori`s past the
`jal`, which rotates seven words. Everything else is exact.

Two facts carried this one most of the way, both reusable:

- The lerp factors have to be `u32` locals holding the binary32 bits, assigned
  just before the `effColorBlend` call. Written as literals inside
  `SDF_VU0_INTERPOLATE` in the loop body, gcc hoists the materialisation into
  the loop preheader and the whole saved-register assignment rotates by one
  (`work` takes `$18` where retail uses `$19`): that costs 30 words rather than
  7. `PS2_FLOAT_BITS` cannot be the source shape here, since retail has no
  `mtc1`/`mfc1` pair for either factor.
- `found` has to be declared after every other local. It is spilled in this
  function, and with `found` declared next to `model` it takes the lower of the
  two spill slots (`0x10`) where retail gives that slot to `limit`.

Ruled out for the seven words: both assignment orders of the two factors; the
assignments before the `frame < limit` guard, before the fade if-chain, between
the blend and the loop, and split either side of the call (the three early
positions are far worse, 88 to 96 words, because the factors then occupy
registers the fade chain needs); and `found` declared first rather than last.

## effPCPDANTE_Damage_Create (effect/effPCPBoss, 360 bytes)

90 of 90 words match, at the native 360-byte size (2026-10-30).

Only the truthful `u8` held-byte local is bound to native `$19`, a localized
target-specific compiler reconstruction aid, not evidence of historical source
spelling. No empty asm is needed. Count is an ordinary `u32` local: it
initializes after setup callbacks and has real increment/range/multiply
consumers. Its native `$21` allocation is preserved across both RNG providers.
Held initializes from the real unsigned-byte load after both
calls and dies in its byte store before the next iteration's calls. The old
delay value in `$19` and parameter pointer in `$20` are dead in this phase;
the overlapping live swap bound naturally occupies `$20`.

An ordinary `u32 shiftBase = 1u` value, assigned after count initialization and
before slot formation, supplies the actual mask-shift operand. It preserves
defined unsigned shifting for all 32 iterations, including bit 31. Actual RTL
shows its definition before slot formation throughout loop optimization and
scheduling. The prior literal was introduced inside the loop then hoisted
after slot formation, leaving the two setup instructions reversed.

The local delay accumulator is `u32`, while the parameter record's signed
`wait` field is unchanged. This reproduces native `addu` modulo-32-bit
accumulation and low-byte `sb` storage for every signed wait value, without
assuming a parameter range that would prevent signed overflow. The correction
leaves all allocated and relocation section bytes identical to the signed
control. Count is at most 32; its doubled swap bound is at most 64. No RNG is
called when count is zero; otherwise native unsigned remainder yields indexes
below count, within the 32-byte delay array. Equal indexes are safe.

Qualified plain/`-da` assembly and allocated/relocation sections agree. The
complete owner checks at 158 match / 0 differ; no ABI, layout, access width,
call sequence or extra consumer is introduced.

Retained negative controls explain the minimal final form: count-only reached
83/90; held with nonvolatile empty `+r` reached 85/90 but reordered three loop
instructions; volatile reached 76/90. Removing the empty asm while retaining
the held declaration reached 88/90, leaving only setup order. The ordinary
shift-base local closed those last two words. Final necessity ablations with
unsigned delay and shiftBase found both bindings removed at 73/90, count-only
at 85/90, and held-only at 90/90; the redundant count binding was therefore
removed. Earlier declaration permutations,
changed count/index types, `count + count`, merged conditions, parameter-copy
forms, array-index formation walking, and a held `s32` did not close the match.
The initial delay assignment before setup calls and formation-pointer increment
remain important native source shapes.

## effPCPBossBUNSINDrawPoly2 (effect/effPCPBoss, 468 bytes)

108 of 117 words match, at the exact 468-byte size.

Scheduling inside one basic block. Retail emits the eight stores that set up
the primitive block and the first quad vertex in source order: `uv[7]`,
`positions`, the two negated half-extents, `colors`, then `uv[2]`, `uv[5]`,
`uv[6]`. The draft pulls `colors` and `uv[6]` to the front of that run and
pushes `uv[7]` and `positions` behind the two vertex stores, which also rotates
the three small constants (`&uv`, `2`, `4`) that feed them.

The rest is exact, including `SDF_VU0_LOADIDENTITY` followed by
`VU0_ADD_VF31_VF10_XYZ` for the translation, the four `SDF_VU0_APPLYMATRIX`
vertices each with its own address computation for the load and the store, and
`sdfDrawTagLink(tag, (u32)effPkt3DPrimMakePoly(&effPCPBossWork))`.

One fact got it to nine words and is worth keeping: the two half-extent
multiplies have to be written second argument first (`width = width * 0.5f`
before `height = height * 0.5f` reads backwards but is what puts retail's
`mul.s $f21` ahead of `mul.s $f20`). The other order is six words worse and
flips which negation reaches `x`.

Ruled out: `colors` moved to the front and the back of the run; `uv[6]` beside
`uv[7]`; the four zero `uv` stores hoisted to the front; `vertexCount` and
`primCount` in both orders against `uvs`; the vertex `z`/`w` stores before
`x`/`y`; `uv` as `f32[4][2]`; the points as `f32[4][4]`; `uv` declared before
the points (73 words, it takes `0($29)`); and the whole primitive block written
through a local `effPCPBossWork_t *`, which is far worse because the pointer
then comes from a pseudo rather than a `%hi`/`%lo` pair.

## effPCPBossKAGUInvokeDraw (effect/effPCPBoss, 0x2E0)

181 of 184 words match at the exact 736-byte size; first difference +0x1A4.
The normal build retains the assembly fallback. The guarded body scans for
an eligible invoke model, preserves the signed lifetime/fade calculation,
captures marks 20 through 27 and center mark 0, and updates six nine-point
poly lists for each requested group. Each group repeats the same six direction
templates. The fourth direction component is an additive radius offset; the
VU operation multiplies W by zero before scaling and adding the center.

The structured parameter view preserves the original 0x3C0-byte region: 54
four-float direction records, the existing 0x54-byte thunder header, then
limit/fade-in/fade-out words at +0x3B4/+0x3B8/+0x3BC. The raw byte member
remains available to the matched constructor. Output slots start at +0x3C4;
handles are reloaded before parameter lookup, color update, and drawing.

Only address expressions remain different: the draft copies the center's
already-computed stack address into the loop pointer, where native computes
it again from SP. Its output-slot cursor starts at the work base and adds
0x3C4; native starts four bytes later and adds 0x3C0. Both yield the same
addresses. Separating the center from the eight mark vectors does not change
these choices; moving the slot increment before the callbacks grows the
body and spills the count. No biased pointer or artificial clobber is used
to force the native forms. The unresolved question is the ordinary source
lifetime that produces those two address materializations.

The shared unsigned loop index is live in two disjoint phases: capturing eight
marks, then traversing the requested groups. Qualified compiler tracing
(`cc1probe.sh --cflag -DNON_MATCHING --alloc --sched --no-dumps`) verifies
ordinary/probed parity. Pseudo 97 supplies the mark bound 8 (UID 242), resets
at UID 541, then supplies the group bound (UID 324). Global allocation combines
16 references, live length 114 and four crossed calls (priority 5614), assigning
it `$19`. Independent review confirms that each phase defines the value before
use, the type is truthful, and no address escapes or artificial uses are added.

## effPCPBossONGYDraw (1432 bytes)

306 of 358 instruction words match at the exact native size. The reconstruction
is guarded; normal builds retain assembly and receive no exact matching credit.

The effect work contains a formation pointer, material pointer, floating angle
and signed frame counter. Its two constructors retain their exact bytes with
these contracts. Position and rotation come from the formation. The model under
its helper unit supplies the motion number and controller frame; reload the
render-model global at the native callback boundaries. The first model-position
call uses the model pointer checked before the VU transfers. Cache the formation
scale once before writing its three equal vector components.

Preserve both position snapshots and the zero scale-vector fourth lane. The
model texture is rendered before testing the twelve-frame ribbon limit. Only
the packet path advances the effect counter. Material and render-texture reads
follow their preceding callbacks. The existing primitive descriptor retains its
constructor-owned fields at offsets 4 and 0xC.

The frame ratio drives an unsigned alpha conversion and an unsigned phase count.
The angle advances once in the work record; a separate local angle walks the
ribbon. Cache the native width, UV step and vertical step across VU operations
and packet callbacks. Caching the UV and vertical steps raises the initial comparison from 42 to
246 matching words at the same total size; the later snapshot and literal fixes
raise it to 306.
Use literal-pool values rather than new global data. The decimal spellings
`0.043633227f` and `0.0033333333f` reproduce words `0x3D32B8C1` and
`0x3B5A740D` with the original compiler; their exact binary values written as
long decimal fractions truncate one word low in that compiler's parser.

The 300 iterations each generate four transformed vertices, four UV pairs and
four colors. Full packets contain 32 vertices and 16 primitives. Reset all three
array cursors and the count after linking a full packet. The final sixteen
vertices form eight primitives. File the tag through ordering-table entry 43,
then reload and increment the live frame counter.

The remaining words concern scale-store and argument scheduling, primitive
setup register choices, and floating-register allocation in the loop. Descriptor
store reordering and a named next-Y value worsen the result. Separating the
angle-step numerator does not improve it. No artificial dependency or register
constraint is used.

Reproduce with `tools/check_unit.py src/smt3/effect/effPCPBoss.c
--cflags=-DNON_MATCHING --func effPCPBossONGYDraw -v`.

## effPCPBossKAGU2AttackDraw (effect/effPCPBoss, 0x424)

265 of 265 words match at 1060 bytes in ordinary C. The signed bounded search
keeps distinct current/found pointers and a fallback-only NULL return; the
direct skill path bypasses that guard. Frame/duration/fade snapshots precede
blend, while count/randomness follow the real VU endpoint store. Actual aligned
place/marks/center/end objects explain the native 0x110 frame without padding.
Existing VU helpers hand produced vf10 values to real scratch immediately,
then read XYZ for accumulation. Real lists remain 16 slots, four entries per
group, with marks 1/2/4/5 and angles zero/positive/negative/zero. No new clamp,
consumer or type recovery is claimed. List pointers and BED/frame fields
retain their actual callback reloads and native increment timing.

Independent BED payload stores before SetParams recover native scheduling
without barriers or constraints. Qualified **pre-scheduler** regmove has
field9 UID772 carrying the actual base's REG_DEAD274; field8 UID767 does not.
Sched1 moves field9 before fields7/10/XYZ/color, then zeros4/5/6, field8 and
zero3. Its **post-scheduler** death marker transfers to later field8: that
output annotation is not evidence that field8 was the dying input use.
Complete-owner plain/dump normalized assembly, allocated-section contracts,
relocations and function inventory agree, independently checked. This records
observed scheduling, not an unseen comparator rule or original source spelling.

Independent verification gives 155 match, 0 differ, preserving all peers and
owned data; the first KAGU remained unchanged at this source. Retail checksum,
427 tests, 147 guarded owners / 545 drafts, seven IRX owners and report gates
passed. Source `910bd87f2ed6d9271eaa2794aaa67076ef33c3c4` has green exact-source CI
[41082732651](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41082732651).

## effPCPBossKAGUAttackDraw (effect/effPCPBoss, 0x35C)

215 of 215 words match at 860 bytes. Two explicit local `$22` bindings are
SMT3 reconstruction aids on **nonoverlapping real lifetimes**, not historical
source evidence. Fallback found initializes NULL, receives only an actual
matching formation and feeds the NULL/early-return guard; the direct path
bypasses it. Qualified life RTL marks that hard-register value dead at the
guard. Only later does groupIndex initialize zero for its genuine outer-loop
comparison, addressing and increment. Both use ABI-preserved callee-saved
storage across actual callbacks; prologue/epilogue save and restore `$22`.
No live caller-clobbered binding, overlap, fake consumer, artificial lifetime
or instruction-emitting scalar asm is introduced. Existing VU helpers are
actual hardware handoffs, not part of this allocation allowance.

Unsigned i genuinely indexes eight marks, dies at that phase's exit, then
reinitializes for each eight-polygon loop with the native index-6 exclusion.
Qualified life/global-allocation observations establish the disjoint consumers
and final pseudo93-to-hard19 allocation. The later group loop and actual
linear-list index retain independent unsigned increments in cursor order.
No phase value leaks across reset; no extra memory operation or callback is
added. Real eight-entry groups, 32 slots, fallback domain, BED stores and VU
scratch remain unchanged, without fabricated bounds or padding.

Controls isolate the dependency: fallback binding plus faithful BED order
reaches 197/215 words, the disjoint group binding 203/215, genuine index reuse
210/215, and native independent cursor increment order completes the match.
A broad work-pointer binding disrupted induction and was rejected. Final merged
owner plain/dump assembly, allocated sections, relocation contracts and complete
function inventory pass independent parity; KAGU2's body is unchanged. The
older first-only study owner contained a stale sibling and is not the final
owner acceptance evidence. No unobserved allocator comparator is claimed.

Independent final verification gives 156 match, 0 differ, including owned data.
Retail checksum, 427 tests, 147 guarded owners / 544 drafts, seven IRX owners
and report gates passed for source `cd029e510e43fd3259bbd3d6fc9989b28e655100`.
Exact-source CI
[41083876389](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41083876389)
attempt 1 completed successfully, with every actual step passing and none skipped.

## effPCPDANTE_ShowTime_ShowTime_Draw (effect/effPCPBoss, 0x358)

214 of 214 words match at 856 bytes. The first fade quotient is assigned to
the existing real `f32 rate` scalar before its color expression. Later phases
assign that same scalar before their existing scale/color consumers. There
is no new entry initialization, artificial consumer, constraint, register
binding, helper or provider change.

The first quotient is consumed by the actual alpha calculation and is not
carried into a later phase. Existing unsigned frame-window tests, floating
operation order, alpha-mode changes and sprite calls remain intact. Every
later rate use follows its own assignment. The texture/frame snapshots and
the final fresh frame load with unsigned increment are unchanged. No callback
observation or memory access is added by naming the quotient.

Qualified expansion records the baseline first quotient in temporary SF
pseudo93, while the candidate writes the same division into the existing
rate variable, SF pseudo89. Its real color subtraction carries REG_DEAD;
later phases define fresh values before their own consumers. Post-global
allocation places the candidate quotient in f21, with native division
`div.s f21,f2,f1`, rather than the baseline's f1. This establishes the actual
shared pseudo and allocation outcome, not an unobserved allocator tie-break
rule or original-source spelling.

Independent semantic review and canonical complete-owner checking give
157 match, 0 differ, preserving all 156 peers and owned data. Independently
verified complete-owner plain/dump assembly and allocated-section, relocation
and function contracts agree for both baseline and candidate, covering
157 functions and seven contracts in each pair.

Final local gates pass the retail checksum, 26 tooling test files / 427 cases,
all 147 guarded owners / 532 drafts with stable source/compiler inputs,
seven IRX owners, and reconciled production reports.

Source commit `21fd7386d3f34b42d9269dd681bb8a467cf67f2a` passed exact-source
hosted CI [41114522908](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41114522908),
attempt 1, including build and Pages deployment.
