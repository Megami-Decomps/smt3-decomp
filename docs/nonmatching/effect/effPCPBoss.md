# Non-matching drafts: effect/effPCPBoss

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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

71 of 90 words match, at the exact 360-byte size.

Two callee-saved registers swapped. Retail keeps the parameter pointer in `$20`
and the accepted-target count in `$21`; the draft has them the other way round,
so every instruction that names either differs, and the swap pulls `swaps =
count * 2` into the other dead register and the byte-swap temporary out of
`$19` into `$5`. Everything else is exact, including the two 32-iteration
conditions, the branch-likely forms and the shuffle loop.

Two source facts carried it most of the way, both reusable:

- `delay = 1` has to be assigned before the `nbGetMainProcessData` and
  `effBTLSkillGetTargetBit` calls. With the assignment after them the value
  lands in `$4` instead of a callee-saved register, the frame shrinks by 16
  bytes and 54 words differ; retail fills the first call's delay slot with
  `addiu $19, $0, 0x1`.
- The formation walk is a pointer increment, not `unit[i]`. Retail reads
  `lw $2, 0x78($16)` with `$16` advancing by `0xF0`; the subscript form biases
  the pointer by `0x78` and adds a word of setup.

Ruled out for the register swap: `count` declared first, last and in the
middle (declaration order is inert here, as elsewhere); `count` as `s32`;
`swaps` as `count + count`, inlined into the loop condition, and through a
separate index variable for the second loop; the parameter typed and the
`memcpy` taking it directly or through a `void *` cast; the array index form in
the first loop; `slot` initialised before the calls; a `range` local for the
two `dds3GetRandIntA` arguments; `held` as `s32`; `a`/`b` as `s32` and declared
at the top; `NULL` against `0` for the random stream; the second loop as a
`while`; the two conditions merged with `&&` and spelled `!= 0`. Hoisting
`param->delayStep` into a local and deriving `count` from the slot pointer are
both far worse (78 and 80 words), which confirms the loop body is right.

After recovering the complete 16-byte parameter record, replacing the generic
copy with `work->param = *param` still leaves 71 of 90 words matching at the
360-byte size. Typed aggregate assignment does not close this
allocation residual either.

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
