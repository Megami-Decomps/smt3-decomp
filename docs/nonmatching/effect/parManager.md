# Non-matching drafts: effect/parManager

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3Particle_SparkOffsetBuffInit (effect/parManager, 0x790)

462 of 484 words match, at the exact 1936-byte size, and the 22 that differ
are the same `$20`/`$21` swap of `dds3EffectBase` and `ParRand` as its
sibling `dds3Particle_SparkBuffInit` above: retail holds `dds3EffectBase` in
`$21` and `ParRand` in `$20`, the draft the other way round. Nothing else
differs anywhere in the function. Written straight off the SparkBuffInit
draft once that one was settled, so the four shapes recorded in that entry
(the `switch` with cases 0, 1, 2; `position[4]` declared before
`direction[4]`; the step integration as a `do`/`while`; the per-arm jitter
scale sharing the tail's `jitter` local) all carry over unchanged and are not
re-derived here.

What is different from the sibling, all confirmed by the bytes: the
displacement lives in `offsetX`/`offsetY`/`offsetZ` at `+0x30` and the
emitter origin is copied into `packet->pos` whole with
`SDF_VU0_COPYVECTOR(packet, effect->head.origin)`, so the matrix transform
runs on `&packet->offsetX` and neither mode-0 arm adds the origin back
component by component; `offsetAcceleration` at `+0x3C` replaces the
sibling's `verticalAcceleration` at `+0x38`; the mode-1 arms zero the
displacement (`offsetX`/`Y`/`Z`) or `packet->pos` rather than seeding it; the
mode-2 integration starts from `position[] = 0` instead of the origin; and
the direction table is `D_003171B0`, the same twelve icosahedral unit vectors
as `D_003170E0` at a second address. `offsetAcceleration = gravity;` is the
last statement of both mode-1 arms, as in the sibling.

Nothing further was tried here: whatever flips the register pair in
`dds3Particle_SparkBuffInit` flips this one too, and the list of shapes
already ruled out for it is in that entry.

## dds3Particle_SparkBuffInit (effect/parManager, 0x800)

490 of 512 words match, at the exact 2048-byte size. The 22 that differ are
one thing: retail keeps `dds3EffectBase` in `$21` and `ParRand` in `$20`, the
draft the other way round, so every `lui %hi`, every `addiu %lo` and the two
`addiu $4` argument setups name the other register. Nothing else in the
function differs, in any arm. The pre-existing source note calling this an
"s4/s5 RNG-address allocation residual" is this exact swap.

Four shapes had to be right, and each is worth recording:

- The `mode` dispatch is a `switch` with cases 0, 1 and 2 in that order and no
  default: ee-gcc builds retail's `beq 1` / `slti 2` / `beqz 0` / `beq 2`
  decision tree from it. All five other case orders are far worse (233 words
  up to 471 words differing), so the order is evidence, not taste.
- The two local vectors are `f32 position[4]` declared **before**
  `f32 direction[4]`, which is what puts `direction` at `0x10($29)` and makes
  `SDF_VU0_LOADVECTOR` materialize retail's `addiu $2, $29, 0x10`. With
  `direction` at offset 0 cc1 addresses it through `$29` directly and the
  whole body shifts one word (2008 bytes, 55 words matching).
- The step integration in case 2 is a **`do`/`while`**, not a `for`: retail
  enters the loop unconditionally and keeps the up-counter with
  `slt $2, $3, $4`. A `for (step = 0; step < steps; step++)` adds the `blez`
  zero-trip guard and lets gcc flip the counter to `addiu $2, $2, -1`, which
  cost 8 words and 40 bytes of size. This was the single biggest jump:
  164 differing words to 48.
- The per-arm jitter scale and the tail's `head.speedJitter`/`head.spinJitter`
  are **the same local**. Retail holds that value in `$f20`, a callee-saved
  register, which only happens because the allocno crosses the tail's calls.
  Given its own local the value is a cross-block allocno that does not cross
  a call and lands in `$f5`; given one local per arm it is block-local and
  lands in `$f0`. Both cost the three `mul.s` registers and the schedule of
  the `verticalAcceleration` store around them.
- `packet->verticalAcceleration = gravity;` is the **last** statement of both
  case 1 arms, after `packet->vel[2]`, even though retail emits the store
  between the `vel[2]` load and its multiplies. Writing it between `vel[1]`
  and `vel[2]`, where the emitted order suggests, costs four words in each
  arm (48 to 26 to 22 as the two arms were moved).

Also settled: `effect->jitter` is read inline at both of its uses in the scale
expression rather than through a local, which is what gives retail's
short-lived `$f3`; a `jitter` local there takes `$f20` and displaces the
scale.

Ruled out for the register swap, all leaving it at 22 words:
`(effRandState_t *)&ParRand` in place of `(effRandState_t *)ParRand`;
`packet->color = 0;` before the `packet->age` line that is `ParRand`'s only
use in the tail (32 words); one scale local per arm (52); merging the scale
with `angle` (69) or with `angle` and `jitter` together (77); moving the
scale declaration to the top of the function (no change). The two address
pseudos have identical reference counts and identical first-reference order
in the draft and in retail, so what is left is the allocator's priority
tie-break, the same class of difference as the spill-slot rotation in
`fldSkyDrawFilter`: whoever picks this up needs to change which pseudo is
created first, not what the source says.
