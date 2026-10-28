# Non-matching drafts: field/fldAutoMap

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## fldAutoMapCloseBlock2 (field/fldAutoMap, 0xA8)

40 of 42 words match, at the exact 168-byte size.

Retail computes `floor - 1` into `$7` in the instruction before the `div` and
`flagNumber - 1` into `$6` in the `beql` delay slot of the divide-by-zero
check. The draft emits the same two `addiu`s with the same registers, in the
other order: `flagNumber - 1` before the `div` and `floor - 1` in the delay
slot. Every other word, both arms included, is identical.

ee-gcc sinks whichever of the two locals is written later in the source into
the delay slot, so retail's order asks for `floor - 1` first. Declaring it
first does produce that order, but then the index lands in `$5` instead of
`$7` and the whole `>= 30` arm reallocates, which costs 18 words. All 6
declaration orders and all 36 declaration-order/assignment-order splits were
measured: the three with `bit` declared ahead of `index` reach 2 words and
nothing reaches 0.

Also ruled out: `floor - 1` or `flagNumber - 1` written inline at the use
sites instead of as locals (23 and 29 words), both inline (35), and the
`u32 ofs` local in the `0xFC58` arm as well as the `0x17F10` one (which costs
a word of size).

`fldAutoMapOpenBlock2` is the same function with `|=` for `&= ~`, and it does
match, from `room`, `index`, `bit` locals in that order. The extra `nor` in
the close path is what moves the allocation.

## fldAutoMapDrawIconOfsR (field/fldAutoMap, 0x244)

10 of 146 words match, at the exact 584-byte size.

The function builds one textured quad for an icon. What it does is fully
recovered; what is left is register allocation.

`fldAutoMapIconOfsTable` is `s16 [][8]`: `[0]` and `[1]` are the texture
origin, `[2]` and `[3]` the size, `[4]` and `[5]` the anchor offset. The u
coordinates and the x coordinates scale by 16, the v and y coordinates by 8.
`sdfPrimLinkFt4` takes 21 arguments, the last 13 in the outgoing frame at
`0x0` through `0x60` in 8-byte EABI slots:

    sdfPrimLinkFt4(tag, color, mode,
                   x0, y0, u0, v0,  x1, y1, u1, v1,
                   x2, y2, u2, v2,  x3, y3, u3, v3,  mask, next)

`mode` selects the quad's winding: the default lays the four corners out
top-left, top-right, bottom-left, bottom-right; `mode == 8` flips the rows;
`mode == 4` transposes, taking the anchor from `[4]`/`[5]` the other way
round and the extent from `[3]`/`[2]`. The tail is two separate
`fldDrawGetOt` calls, the first supplying the argument and the second the
`regFunc` to call through, so the source reads
`((...)fldDrawGetOt()->regFunc)(ot, tag)` with `ot` from an earlier call.

What is needed to get the three `lh` loads and their shifts right: `s32`
locals for `t[0]`..`t[3]`. Read as `s16` at each use, ee-gcc loads `lhu` for
the `* 16` use and then sign-extends with `sll 16`, `sra 13` for the `* 8`
one, which adds two instructions per arm. `s16 *t = fldAutoMapIconOfsTable
[index];` as an initializer at the declaration is what gives retail's four
saved registers and the 0xB0 frame; assigning it after the three leading
calls spends one fewer and lands at 564 bytes.

The remaining difference: retail copies `x` into `$f21` and `y` into `$f20`,
the draft the other way round, so every `mul.s` and the two `cvt.w.s`/`mfc1`
pairs in each of the three arms name the other register. Both compute `y`
first, in the same statement order, and swapping the statement order does not
move the assignment. Retail also keeps `index * 16` in `$16` and rebuilds the
table address into a temp after the calls, while the draft keeps the whole
pointer in `$16`; recomputing `fldAutoMapIconOfsTable[index]` inside the
default arm does reproduce that, but costs size elsewhere.

Measured and rejected: the row addressed as `fldAutoMapIconOfsTable[index][k]`
at every use (ee-gcc builds a chain of pointer copies, 620 bytes), `x0` before
`y0` in each arm, and the four combinations of those with the pointer either
initialized or assigned.

## fldAutoMapSeqStart (field/fldAutoMap, 0x30C)

148 of 195 words match, at the exact 780-byte size. Everything up to `+0x1A4` is
identical; the rest is the closing block that fills three vectors.

Recovered along the way, and used by the matching `fldInitPacLoader`:
`fldAreaWork_t.pacRes[10]` at `+0xD0` is the pac loader's handle and mapped
pointer per entry, and this function reads the mapped pointers `pacRes[7]`,
`[9]`, `[1]`, `[3]`, `[5]` straight into `sdfTexSet`. It also reads
`amFloor` at `+0x64`.

`D_00532230` is the `{0, 0, -1, 1}` up vector, assigned to a local
`sdfVector4_t` (which gives the `ldl`/`ldr`/`sdl`/`sdr` run), and the three
locals go out through `SDF_VU0_COPYVECTOR` to `D_002F5E00`, `fldAutoMapRect`
and `D_002F5E10`. `D_0054C4C4` and `D_0054C4C8` are this unit's `.lit4`
entries at file offsets `0x44D4C4` and `0x44D4C8`, holding `-24000.0f` and
`20000.0f`; the other four constants in the block (200.0, 255.0, 1000.0,
2048.0) have zero low halves and come out as `lui`/`mtc1`, so they add
nothing to the pool.

Two things were needed to reach the exact size. `fldAreaWork_t`'s light block
has to be addressed through a local pointer to `&dds3LightParam2[0x20]`, which
is what folds the `0x20` into the `%hi`/`%lo` pair instead of leaving it in
the store displacements. And the three `0.0f` stores have to go through one
`f32` local: written as literals ee-gcc emits three `sw $0`, where retail
materialises the zero once with `mtc1 $0` and stores it with `swc1`, which is
one word more. Assigning `target.y` and then copying it into the other two
works equally well (49 words out instead of 50 before the rest was ordered).

What is left: retail holds the two world coordinates in `$f3` and `$f0` and
the zero in `$f1`, the draft in `$f2`, `$f1` and `$f0`, and retail does the
up-vector copy before the float arithmetic rather than after. All 720
orderings of the six statement groups in that block were measured (the best
six tie at 47 words), as were all 24 orderings of the four `D_00551CAC`
through `D_00551CB8` stores. Nothing moves the register choice.

A forward prototype for `fldAllIconDel` is required: the unit defines it below
this function, and the implicit declaration makes ee-gcc treat it as returning
`int`.

## fldAutoMapDrawIconOfsRF (field/fldAutoMap, 0x33C)

187 of 207 words match, at the exact 828-byte size.

The rotated-icon sibling of `fldAutoMapDrawIconOfsR`: it builds the same
textured quad, but in world space, rotating the four corners through the VU0
matrix before projecting. Fully recovered apart from register allocation.

The shape, which carried over to the matching `fldAutoMapDrawBgOfsRF`:

- Four `sdfVector4_t` locals get `.x`, `.y = 0.0f`, `.z` in declaration order
  `v0`, `v3`, `v1`, `v2`, which is what matches retail's store schedule. The
  natural `v0`..`v3` order is 8 words worse here and 8 worse in the sibling.
- Each is rotated with `SDF_VU0_LOADVECTOR(&v)`, `SDF_VU0_APPLYMATRIX_XYZ()`,
  `SDF_VU0_STOREVECTOR_NOMEM(&v)`. The `_NOMEM` store is required: the plain
  form's `"memory"` clobber lets gcse share the `addiu rd, $29, N` between the
  load and the store, where retail rematerialises it for each.
- The screen coordinates are then written back into the same fields,
  `v.x = v.x + x * 16.0f;` and `v.z = v.z * 0.5f + y * 16.0f;`, x before z for
  every vector *except* `v1`, where z comes first. That one inversion is worth
  14 words; every other combination of the four is worse, and inverting all
  four costs 23.
- `color` needs the `+ 0x80000000` as its own statement,
  `color = (s32)(...) << 24; color += 0x80000000;`, or the shift goes to a temp
  and the add writes the variable, which is 2 words off.
- `0.2f` and `3.14f` are this unit's first two `.lit4` entries, `D_0054C498`
  and `D_0054C49C`.

What is left: the four texture-coordinate locals hold the other register of
each pair. Retail has `u1` in `$16`, `v0` in `$17`, `v1` in `$18` and `u0` in
`$19`; the draft swaps `$16` with `$17` and `$18` with `$19`, which moves ten
`lh`/`sll`/`addu`/`sw` words and the two `daddu` argument copies. Nothing
reached it: all 24 declaration orders and all 24 assignment orders of the four
locals (no effect at all, gcse normalises them), separate `tw`/`th` locals for
the widths, `u1 += u0` for `u1 = u0 + u1`, the sums computed before the float
coordinates, `u1 = t[0] * 16 + t[2] * 16` with the bases written at the call
sites (135 words), the bases as locals with the sums at the call sites (192),
`u32` locals (171), the shifts as their own statements (38), and
`fldAutoMapIconOfsTable[index][k]` at every use (160).

The sibling `fldAutoMapDrawBgOfsRF` had the same permutation in its eight
constant texture coordinates and *was* fixed by declaration order, because
there they are initializers materialised in the prologue: declaring them
`au, av, du, dv, bu, bv, cu, cv` (the `d` pair moved ahead of `b` and `c`) took
it from 10 differing words to exact. The same lever does not exist here, since
these four are computed from the table mid-function.

## fldAutoMapProc (field/fldAutoMap, 0x48)

15 of 18 words match, at the exact 72-byte size.

Three words, and all three are the branch-annul shape the levers page describes
from the other side: retail has `beqz` and `bne` with a `nop` in the delay
slot, the draft `beqzl` and `bnel` with the epilogue's `ld $31, 0x0($29)`
annulled into it. Both differing tests guard the arm that calls
`fldAutoMapCalc`, which is still `INCLUDE_ASM`, so this is the `mc/slMain`
mechanism (a caller needing its callee's definition, not just a prototype):
decompile `fldAutoMapCalc` and try this again before touching the source.

The body is settled. `fldGb[0]` is the field id, and the two bounds have to be
separate `if` statements with early returns, which is what gets both `slti`s
computed before the first branch; `&&` folds them and costs 13 words.
`fldAutoMapCalc` must stay without a forward prototype, since the implicit
`int` declaration is what reserves `$2` across the call and stops the sibcall.

## fldDrawAreaBlockFont (field/fldAutoMap, 0x388)

57 of 226 words match, 908 bytes against retail's 904.

The area-name panel: it steps the name's fade counter by the panel mode, then
draws the place name (or the area name) and its backing panel. Fully recovered
as behaviour; what is left is one saved register and the two fade arms.

What the function reads, since none of it was named before:

- `fldGlobalBase + 0x90` is the panel mode, 0 to 7, which drives the jump
  table. The HD reconstruction calls it `AllPanelMode`. The draft spells it
  `*(s32 *)((u8 *)&fldGlobalBase + 0x90)` rather than naming the field, since
  nothing compiled uses it yet.
- `D_0054EEA4` is the fade counter, 0.0 to 1.0 (`gFldAreaNameCnt` in the HD
  source), and `fldAreaFontWork` is its direction, -1, 0 or 1
  (`fldDrawAreaBlockFont_pm_mode` there).
- Mode 0 and 1 snap the counter to 0.0 and 1.0; 2 and 3 step it by 0.1 toward
  0.0 and 1.0, clamping and zeroing the direction at the end; 4 through 7 step
  it in whichever direction the counter already holds, but only while it is
  strictly between 0.01 and 0.99.
- The panel position is an interpolation whose endpoints are all zero except
  one: `x = (s32)(v * 0.0f + (1.0f - v) * 250.0f)` and
  `y = (s32)(v * 0.0f + (1.0f - v) * 0.0f)`, which is what shares the one
  `mul.s` by zero between the two. The HD source has the same expression with
  a 3.75 and a 2.25 resolution scale added.
- `fldGlobalBase.nameD` selects the place name, `D_0030D714` with
  `fldAutoMapFindPlace` and colour `0xF0DC6480`; `-1` there falls back to
  `nameU`, `D_0030AB54` with `fldAutoMapFindArea` and `0xA09DC380`. Both
  tables are `char [][28]`, already declared, and both are skipped when the
  matching `D_00567F50`/`D_00567EA0` length is below 2, read with an `(s16)`
  cast at the use site for retail's `lh`.
- The six `.lit4` entries from `D_0054C4D8` to `D_0054C4EC` are this
  function's, in the order 0.1, 0.1, 0.99, 0.01, 0.1, 0.1.

The `nameD` arm has to be written first (`if (nameD != -1)`), or gcc hoists
the `nameU` load above the test and emits the two arms in the other order,
which is 15 words worse. Case 3's clamp has to be written inverted,
`if (!(v > 1.0f)) { direction = 1; } else { v = 1.0f; direction = 0; }`, so
that the constant arm is the branch target; case 2 reads better the plain way
and measures the same.

What is left:

- The frame saves one callee-saved register too many, which is the whole
  2-word size difference. Retail keeps the font request handle and the string
  width in one register (`$17`): the handle is copied into the destroy call's
  argument and the register is immediately rewritten with `width + 8` in that
  call's delay slot. Two locals cannot reach that, and merging them into one
  `s32` with casts at the calls undershoots instead, 884 bytes with the width
  coalesced away entirely.
- Case 2 lacks retail's second `lwc1` of the counter and the `addiu $2, 0, -1`
  hoisted into the first `bc1f`'s delay slot. Retail reads the counter once per
  case and once more on the out-of-range path (in the range check's delay
  slot), which no placement of the read reproduces: before the switch, gcc
  common-subexpression-eliminates the per-case reads; after the switch, it
  replicates the load into all five predecessors instead of using the stored
  register, 20 bytes long; and an explicit `default: v = D_0054EEA4; break;`
  gets hoisted back out of the switch, 12 to 28 bytes long.

Measured and rejected: the two clamp arm orders in cases 2 and 3 (4
combinations), the two `nameD`/`nameU` arm orders, `&&` against nested `if`s
for the 0.99/0.01 range, three spellings of `x + 0x1EC - w`, the counter read
before, inside and after the switch, and the handle and width as one local
with casts.
