# Non-matching drafts: effect/effPCPBezier

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## effPCPBezierBillDraw (effect/effPCPBezier, 0x1764)

1482 of 1497 words match at the exact size, 5988 bytes, with the same
instruction count, so the positional and aligned counts are both 15 of 1497:
there is no misalignment anywhere and no instruction is missing, added or
reordered outside one seven-word block. Nine of the fifteen are a register name
and nothing else.

What the draft establishes, all of it confirmed word for word:

- The slot loop is `i = 0; if (count != 0) { do { ... i++; slot++; } while (i <
  count); }` over `work->params.groupCount`, with the respawn guarded by
  `slot->curve.parameterStep == 0.0f` and the live path by
  `slot->age++ >= 0`.
- The five-way `switch (mode)` over `work->params.mode` is ascending, cases 0 to
  4 with no `default`, and cases 1, 2 and 4 cross-jump their seventh control
  point into one shared block, which is why only cases 0, 2 and 3 keep a copy.
- **Every scale term in a control-point block is a reused `f32` local, not a
  literal at the use.** This is the single largest lever in the function: with
  literals the per-point scale constants take a fresh register each and the
  whole body's float temps rotate, 936 of 1497 words; with `scale`, `bend` and
  `reach` assigned per point and read three times each, retail's `$f7`, `$f5`
  and `$f8` fall out and the count goes to 1276. The seventh point's
  `mov.s $f8, $f20` is the same lever read on a copy: it comes from
  `reach = radius;` and not from `axis[k] * radius` written out.
- `reach * axis[k]`, not `axis[k] * reach`: the operand order of a commutative
  `mul.s` follows the source, and flipping those 95 multiplies was worth 90
  words. `axis[k] * bend` and `spin[k] * scale` stay in that order, except in
  case 0's points 3 to 5, where the `SDF_VU0_APPLYMATRIX_XYZ` between the points
  puts the constant in a register before the `spin` load and `scale * spin[k]`
  is what matches.
- In case 0 the `reach` and `scale` assignments sit **above** the
  load/apply/store triple, not below it, which is where retail materialises both
  constants.
- `-50.0f`, `-30.0f`, `-20.0f` and `-10.0f` are upper bounds written
  `if (K < cp.y) { cp.y = K; }`; the inverted reading gives `bc1t` for retail's
  `bc1f` throughout.
- `$f20` holds two values with disjoint ranges and needs **two** locals: one for
  `-work->params.height * dds3GetRandFloatA(dds3EffectBase)`, consumed at
  `spawn[1]`, and one for the later `SDF_VU0_GETLENGTH`. One local for both
  leaves the three callee-saved float registers rotated by one, because the
  short-lived value is what claims `$f20` and the long-lived one then reuses it.
- The three `tri` stores in the sampling loop and in the trailing segment are
  `SDF_VU0_STOREVECTOR_NOMEM` / `_B_NOMEM`, and so are the four `spin` stores.
  The plain form's `"memory"` clobber makes the shared frame address a loop
  invariant in a callee-saved register, which spills one more pointer, adds a
  third quadword spill slot at `0x170` and grows the frame to `0x200` against
  retail's `0x1F0`. Worth 46 words and the whole frame.
- `place.payload[0..2]` feed the facing vector by **reading the placement back**,
  `facing[0] = center[0] - place.payload[0]`, not by reading
  `slot->position[0]` a second time. The store to the addressable `place` kills
  the position load, so the second read costs two reloads and two words; the
  read-back is store-to-load forwarded into the register retail uses. Worth 15
  words. Reading the positions into three `f32` locals first reaches 47 but not
  33, because all three are then live at once where retail reuses `$f0` for the
  second and third.
- The fade colour is **one statement with no intermediate local**:
  `slot->fragment->color = ((u32)(fade * 128.0f) << 24) | 0x808080;` followed by
  `slot->fragment->color = effMagaCylinderCalcColor(tint, slot->fragment->color);`.
  With a `u32 color` local the `lw` of `slot->fragment` cannot be hoisted above
  the `slot->fadeScale` store into the block head and the whole 19-word block
  shifts by one instruction. Worth 18 words, and a `u32 level` local for just
  the conversion does not reach it.
- The pool constants are plain decimal literals: `0.15f`, `0.3f`, `0.45f`,
  `0.6f`, `0.14f` through `0.84f`, `0.5235987f` and `0.333333333f` /
  `0.666666666f` for the two end-cap blends. ee-gcc's truncation reproduces
  retail's words directly, and the `.lit4` order follows the order of use.
- The seven placement fields `payload[4]` to `payload[10]` are written before
  the loop even though the quaternion store overwrites `payload[4..7]` every
  iteration, and `place.payload[3] = 0.0f` plus `place.color` are written inside
  it.

The fifteen remaining words, and what they are:

- Six are the GPR the four `SDF_VU0_INTERPOLATE` blends move their bits through:
  retail uses `$3`, `$4`, `$2`, `$3` and the draft uses `$2` four times. Every
  surrounding word, including both `addiu rd, $29, N` address setups in each
  block, is exact, so the scratch is reusing a dead address register in both
  compiles and only the choice differs. Ruled out: one `u32` local reused, four
  separate `u32` locals, a single-asm macro with a `"=&r"` scratch, the same
  with `"=r"`, and computing the bits before the two `lqc2` instead of after.
- Two are `tint`'s reload register at the `place.color` store, `$3` against
  retail's `$4`. Reading `work->tintColor` at the store instead of the local is
  59 words.
- Seven are `i`'s reload register and the loop tail that follows from it. Retail
  reloads `i` into `$2`, stores it back before the `sltu`, and so gets to reuse
  `$2` for the comparison result, which leaves `slot++` for the branch's delay
  slot. The draft reloads into `$4`, cannot coalesce the comparison, and the
  delay slot takes the `sw` while `slot++` moves to the top of the block. The
  two differing words at `+0x1188` and `+0x1704` are the same reload. Ruled out:
  `++i < count`, `i = i + 1`, `count > i`, `slot++` before `i++`, a `for` with
  `i++, slot++`, `s32` for `i` and for `count`, and `count` or `slot` declared
  last. The spill slots themselves are already retail's (`0x130` work, `0x134`
  delayRange, `0x138` i, `0x13C` count, `0x140` segments, `0x144` mode, `0x148`
  tint, `0x150` and `0x160` the two quadword pointer spills).

So nine words are the register-name residual the levers page records as
finished, and the other six follow from one reload register in the loop tail,
which is the same class. Read the three clusters above before spending budget
here: the body is exact everywhere else.
