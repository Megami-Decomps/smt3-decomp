# Non-matching drafts: effect/effPCPBoss

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## effPCPBossBaelCopy (effect/effPCPBoss, 128 bytes)

21 of 32 words match, at the exact 128-byte size.

Pure scheduling. Retail splits the `lui`/`ori` pair that builds `0x80808080`
around the eight-instruction `ldl`/`ldr`/`sdl`/`sdr` block that copies the
16-byte parameter vector: `lui` right after the allocation, the copy, then
`ori` and the colour store. The draft keeps `lui` and `ori` adjacent and emits
them before the copy, which rotates the eight copy words by two and also swaps
which instruction fills the first `dds3EffectPartCopy` delay slot: retail takes
`sw $0, 0x20` (the frame reset), the draft takes `lw $4, 0x28`.

The sibling `effPCPBossBaelCreate` matches with the same statement order, so
the trigger is the one-argument `dds3EffectPartCopy` rather than the stores.

Ruled out: all six orderings of the vector copy, the colour store and the frame
store, each with the copy written both as `memcpy` and as a struct assignment
(twelve compiles, every one 11 or 12 words off); the colour in a `u32` local
initialised at its declaration; the two `dds3EffectPartCopy` results through
named locals; `sizeof` instead of a literal allocation size. Caching
`src->modelResource` and `src->auxResource` in locals first is far worse, 31 of
32.

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

## effPCPCrackCopy (effect/effPCPBoss, 292 bytes)

63 of 73 words match, at the exact 292-byte size.

Scheduling of the `0x80808080` materialisation. `effPCPCrackCreate` next to it
matches with the identical body, so the 0x8C-byte head copy as one struct
assignment and the store order are right. In the copy, ee-gcc builds the colour
into `$2` before the tail of the block copy, so the tail's two temporaries land
in `$5` and `$6`; retail uses `$2` and `$5` and builds the colour after the tail
stores. The other six words follow from that: the `src->part` load and the
colour store swap sides of the `dds3EffectPartCopy` delay slot, the same way
`effPCPBossBaelCopy` does, and both are one-argument calls.

Ruled out: all six orderings of the head copy, the colour store and the
`unk9C` store (four of them are far worse, two land on 10); `unk9C` as an `f32`
written `0.0f`, which reaches 11.

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
