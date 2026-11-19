# Matching notes: effect/parManager

## dds3Particle_SparkOffsetBuffInit (effect/parManager, 0x790)

Exact at 484/484 words (1936 bytes). Spark and SparkOffset share the
three-mode switch, two aligned stack vectors, nonempty step integration
loop, and the jitter local shared by the mode arms and the tail.

The tail retains the two real RNG states: the particle state supplies the
unsigned activation-delay draw; the effect state supplies the scale and
optional spin draws. A local input/output constraint on the particle state
pointer preserves the native `$20`/`$21` assignment of the two symbols.
A second local constraint names the actual effect argument and unsigned
spread after the integer draw returns, so cc1 finishes the argument setup
ahead of the divide. The argument still comes from the shared effect-state
address and the spread is read once; no fixed register, extra call or memory
read is needed. Removing the first constraint restores the global register
swap, while replacing the second with input-only operands fails to match.

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

## dds3Particle_SparkBuffInit (effect/parManager, 0x800)

512 of 512 words match at 2,048 bytes. Each initialized particle uses
the native 0x40-byte physics record and the original mode 0, 1, or 2
launch path. The mode-2 random step loop is a do/while: it executes at
least once, not zero times for a nonpositive step bound. The vector
stores and VU0 operations are unchanged, including the mode-0 matrix
operation on the full four-lane packet position. No new value is
invented for the native W lane.

The real effect and particle RNGs keep their original call targets,
state objects, and mode-specific draw order. The tail draws the
particle integer first, derives initial age from its remainder against
the then-current spread plus one, and then draws effect scale and
conditional spin before resetting the particle. Valid callers choose
an existing packet index and a nonzero age modulus. Each floating
draw uses the actual provider's low 24 bits scaled by 2^-24 and the
native per-mode binary32 operations.

Localized operands on real computed random-derived values and the
actual tail argument preserve the two RNG address allocations and
dependent floating-point schedule. These are compiler reconstruction
aids: they emit no instructions, add no draws, and do not imply the
original source had the same spelling.

The C declarations use the actual integer and float RNG provider types.
The local argument type bridge forwards the same four-word storage
address, evaluates its argument once, emits no wrapper, and changes no
existing peer or provider.
