# Non-matching drafts: effect/effBattleMisc

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## effBTLModelLook2 (effect/effBattleMisc, 0x110)

2 of 68 words, at the exact 272-byte size, and the two words are each other's:
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

4 of 146 words, at the exact 584-byte size. One instruction is wrong and the
three stores after it are scheduled one slot early as a consequence: retail
rematerialises the zero it stores into the accumulator vector
(`mtc1 $0, $f0`), the draft copies it out of the saved register that already
holds `front` (`mov.s $f0, $f21`). The copy has the same cost but a
dependency, so ee-gcc's scheduler fills the next three slots with the two
`addiu`s and the first `swc1` in the other order.

cc1's copy propagation is what does it: `front = 0.0f;` leaves 0.0 available
in a pseudo that lives across the `nbGetMainProcessData` call, and the later
`zero = 0.0f;` becomes a copy of it. Nothing tried stops that. Ruled out:
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
