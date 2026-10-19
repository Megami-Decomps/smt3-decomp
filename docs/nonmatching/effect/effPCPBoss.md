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

21 of 62 words match, at the exact 248-byte size.

One source fact is missing. Retail materialises `0x80808080` twice: once as
`lui`/`ori` before the parameter copy for `work->color`, and again inside the
`bedParams` arm for `place.color`, with the second `ori` filling the guard's
delay slot. Every C shape tried lets ee-gcc common the two constants into one
register that stays live across the guard, which costs a register and rotates
the whole arm. The structure around it is right: the 12-byte parameter copy as
one struct assignment (one `ldl`/`ldr` pair plus an `lw`/`sw`), the two VU0
identity macros, and the placement block written `payload[0..10]` then `color`,
which is the shape that matched for the five sibling copy routines.

Ruled out for the duplicated constant: the `work->color` store in all four
positions around the parameter copy and the two VU0 macros; `place.color`
written before the payload instead of after; an early `return work` in place of
the `if`; the literal as `0x80808080u`, `-2139062144` and `0x80808080 | 0`;
`place` and `bed` declared in the guarded block instead of the function body
(every combination). All land on 41 or more differing words except the four
listed positions, which land on 41, 41, 42 and 51.

Worth knowing before retrying the bigger `Create` routines in this unit: they
set a work colour and a placement colour the same way, so whatever defeats the
common subexpression here should carry several of them.

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

## effPCPDANTE_Thunder_ProcessDraw (effect/effPCPBoss, 172 bytes)

28 of 43 words match, one word short of retail's 44.

Retail computes `$sp + 0x40` twice: once into `$2` for the `sqc2` that stores
the mark position, and again into `$5` in the `effPCPDANTE_Thunder_SetPosition`
delay slot. Every C shape tried lets ee-gcc common the two into one register,
which it then allocates straight to `$5`, so the draft is one instruction
shorter and everything after the `sqc2` shifts. The matrix half is already
exact, including `SDF_VU0_STOREMATRIX_MEMOPS` for the four frame-addressed
`sqc2`s against `SDF_VU0_STOREVECTOR` for the register-addressed one.

Ruled out for the duplicated address: the position as a bare `u128`, as a
one-element array, and as the fifth element of the matrix array (so the two
locals are one `u128 buf[5]`); a `u128 *` local assigned just before the macro
and `&position` at the call.

The same obstacle blocks `effPCPBossKAGU2MUHENKDraw`, which is otherwise
straightforward: retail computes the mark index `i + 0x14` separately for its
`effBTLModelGetMarkPosition2` and `sdfModelGetMarkPosMatrix` calls, and ee-gcc
commons the two into one saved register, leaving the draft a word short. Its
structure is known: the shared invoke-model search that
`effPCPBossThunderboltDraw` now carries (with a `u32` index and ids 0x125 and
0x108), then a seven-iteration loop that stores the mark position, calls
`effDebugPointDraw`, loads `(0, -1, 0)` through `SDF_VU0_APPLYMATRIX` and draws
a 200-unit line with `effDebugLineDraw`.

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
