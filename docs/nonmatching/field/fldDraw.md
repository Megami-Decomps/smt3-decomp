# Non-matching drafts: field/fldDraw

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## fldSkyDrawFilter (field/fldDraw, 0xAD8)

682 of 694 words match, at the exact 2776-byte size. Transliterated from
dds-decomp's `fldDrawSkyGradient`, the same function at the same size, which
is the one dds pairing that has held exactly.

The remaining twelve words are three reload spill slots rotated. Retail
numbers them `0x118` `fldSkyDataTable`, `0x11C` `row*8`, `0x120` `row*32`;
the draft gives `row*8`, `row*32`, `fldSkyDataTable`. Every instruction is
otherwise identical, so it is purely which pseudo-register gets which slot.
`-fno-gcse` moves 653 of the 682 words and `-fno-strength-reduce` changes
nothing, which puts the mechanism in GCSE and reload pseudo numbering rather
than anything the source can state. Someone attacking this needs to change
what order the pseudos are created in, not what the source says.

Ruled out: a `fldSkyData_t *` local at four scopes, which gcc folds back to
the global with byte-identical output; the flattened colour array; pointer
and `(p + i)->` subscript forms; `<<` for `*`; a `mode` local for the switch;
an if/else chain for the switch; `u32` loop indices; a row-base `s32 *` with
`cur[column+9]` and `cur[column+10]`; alpha extracted before rgb; hoisted `x`
and `y`; and nine declaration-order permutations.

Confirmed on the way: `fldSkyDataTable`'s row stride is
`{s32 mode; s32 colors[8][9];}`, 292 bytes; `rsetOtOfs` takes `0x39` here
where DDS1 passes `0x44`; the sway offset global is `fldRunDanteGunFlag[14]`,
not `[16]`; and the clamp list is three alphas with `a3` unclamped.

## fldStatusSet (field/fldDraw, 0xF8)

36 of 62 words match, at the exact 248-byte size.

Retail carries two induction variables where the draft has one: an integer
offset added to `DDS3_GBWK` serving `hp` and the first `badstatus` read, and
a pointer biased at the `badstatus` field serving the flag at `-0xE`. The
body never uses the loop index.

Ruled out: the typed `akuma[i]` form throughout; a `fldAnahoriAkuma_t *`
local at three scopes; all eight cast orders of `(u8 *)base + ofs` against
`(u8 *)ofs + (u32)base`, which gcc folds together here; a `u16 *` walking the
`badstatus` field with the flag at `p[-0xE]`; and an explicit second pointer
induction.

This draft proved `fldAnahoriAkuma_t.badstatus` at `+0x12`: retail tests that
`u16` against 0x40, 0x100, 0x20 and 0x8000, and `datCheckDying` is called on
`&akuma[0].flag`, which makes the record `datUnitWork_t` shifted by four and
puts `badstatus` at `0x0E + 4`.

## fldCamUpdateChase (field/fldDraw, 0x250)

120 of 148 words match, at the exact 592-byte size. The first 119 words are exact;
everything from `+0x16C` is one word rotated because retail schedules the
`lw $4, 0x78($20)` that fetches `control->camera` for
`dds3CameraObjectBasicGetCameraTM` to the top of the smoothing block, while
ee-gcc puts it last. The rotation then drags a handful of `$f2`/`$f3`
allocations with it.

Confirmed on the way: `D_0054C380` and `D_0054C384` are `.lit4` entries
holding `0.7f` and `3.14f`; the stick is read as the plain subscripts
`dds3PadStickState[0][0][0]` and `[0][0][1]`, not through a `u8 *` local,
which costs a word by making gcc form the gp-relative address in a register;
and both vectors are zeroed with `memset` and then given a `1.0f` in `[3]`.

Ruled out: a `u8 *` stick local in four spellings (plain, `&...[0][0][0]`, a
cast of the array, `volatile`); an incomplete array declaration; all 36
orderings of the two three-component smoothing blocks (the best is the target
block written y, x, z, which is the draft); `node = control->camera;` as its
own statement before each of the three uses; and a separate `dds3Basic_t *`
local for the camera.

## resetCamTgtPlayer (field/fldDraw, 0x1F8)

90 of 126 words match, at the exact 504-byte size, and every instruction in the
function is the one retail emits. Two things differ and nothing else.

The first is a three-way rotation of the fp registers the eye block uses for
`fldGlobalBase.y`, `D_002FC9C0[1]` and `D_002FC9C0[2]`: retail numbers them
`$f3`, `$f4`, `$f5` and the draft gives `$f4`, `$f5`, `$f3`. The second is
where the scheduler puts `lw $4, 0x78($19)`, the `control->camera` argument of
each of the three calls: retail fills the `div.s` hazard slot with it and
leaves the `swc1` to `camAngle` for the `jal` delay slot, while the draft puts
the `lw` in the delay slot and the `sub.s` for `eye.f[2]` in the hazard slot.
The whole tail from the second `dds3SetPosPoseSet` to the epilogue, the
`sqrt.s`, the `GetSphericalPosMove` argument setup and the `fldCamPitch`
store, is byte-identical.

`-fno-gcse` and `-fno-expensive-optimizations` leave the difference exactly
where it is; `-fno-schedule-insns` and `-fno-schedule-insns2` make it much
worse. So this is register allocation plus the list scheduler's tie-break, not
anything the source states.

Confirmed on the way: `D_0054C2E8` is a `.lit4` entry holding `3.14f`;
`fldCamParamTable[fldGlobalBase.cameraParamIndex][1]` is the height offset
added to `fldGlobalBase.y`; the eye and target vectors are both written as the
degenerate lerp `a + (b - a)` with no factor, which is what retail's bare
`sub.s`/`add.s` pair per component is; and the two angle fields written here
are `fldGlobalBase.camAngle` at `0x124` and `camAngle2` at `0x128`, both taking
`EulerAngleAxisY() * 180.0f / 3.14f`.

Ruled out: six orderings of the three eye components; the `camAngle` stores
before, after and in either order relative to the eye block; seven spellings
of the angle expression (inline, three statements, a named `180.0f` local
assigned before the leading calls, a named `3.14f` local, and the
`180.0f / 3.14f` constant folded together, which costs the whole function); a
`f32 *` local for `D_002FC9C0`; a `* 1.0f` factor on each lerp; a local for
the parameter-table height and for `y + height`; `node = control->camera;` as
its own statement before each of the four uses; and `dx`/`dy`/`dz` locals for
the three deltas, which costs another 19 words.

## fldPlayerCalcHasigo (field/fldDraw, 0xE8)

14 of 58 words differ, at the exact 232-byte size, and every instruction through the
second arm's `add.s` is retail's.

The whole tail (the three `prev` saves, the three position stores, the two
write-backs into `pos` and `dds3SetPosPoseSet`) is written out in both arms, so
ee-gcc cross-jumps it and leaves the `fldGlobalBase` address, the `&pos`
argument and the `pos[2]` load duplicated into each arm, which is what retail
has. The remainder is which float register each value takes in the shared tail:
retail numbers them `$f1` `fldGlobalBase.x`, `$f5` `pos[0]`, `$f2` `y`, `$f3`
`z`, `$f4` `pos[2]`, and the draft gives `pos[0]` the lowest of the five and
pushes `pos[2]` to `$f5`. The scheduler then reads `pos[0]` ahead of
`fldGlobalBase.x` rather than between it and `y`.

Confirmed on the way: the vector is a 4-float stack local at `0x0` filled with
`SDF_VU0_STOREVECTOR` after `dds3GetPosPoseSet`; the sign test is
`if (cnt > 0) ... else if (cnt < 0) ... else return 0;`, which is what retail's
`blez` plus `bgezl` pair is; `fldAreaWork_t + 0x10C` is the previous position,
named `prevX`/`prevY`/`prevZ`; and retail writes `pos[1]` and `pos[2]` back out
of `fldGlobalBase.y` and `.z` after storing them, which is where the two
otherwise unexplained stack stores in the shared tail come from.

Ruled out: a shared tail after the `if` (54 words, three words short, and the
address setup stays in the merge); duplicating only the `prev` block, only
`prev` plus the position stores, or the whole tail including both `Off` calls;
`y` and `z` locals for the two components, with and without `y = pos[1]; y -=
2.0f;` split in two; `SDF_VU0_STOREVECTOR_NOMEM`; a `handle` local for
`ctl->handle` at three positions; and all six orders of the three position
stores crossed with both orders of the write-back pair and six arrangements of
the three statement blocks.

## fldCamGetPhokanParam (field/fldDraw, 0x17C)

92 of 95 words match, at the exact 380-byte size. Every instruction is retail's; the
three differing words are `addu` operand order on two of the four
`fldCamParamTable` row addresses.

Retail forms the row three different ways in one function. The `[0]` column,
which multiplies the sine and the cosine, comes out `addu rd, index, base`; the
`[1]` and `[2]` columns come out `addu rd, base, index` with the 4 or 8 left in
the load displacement. The draft gets `[0]` right and `[1]` wrong, and in the
target block it computes the row once and copies it (`move v0, v1`) where retail
computes a second `addu` with the operands the other way round, which is why the
two were not CSEd together in retail at all.

Confirmed on the way: `D_0054C2D0` is a `.lit4` entry holding `3.14f`; the angle
is `fldCamPitch * 3.14f / 180.0f`, written inline at both of the eye block's
calls and assigned to one local for the target block's two, which is retail's
`$f20` and its two `mov.s $f12, $f20`; the radius is `80.0f`; `[3]` of both
vectors is `1.0f`; and the eye uses `-sin`/`-cos` while the target uses `+sin`
/`+cos` scaled by `[0]`.

Ruled out for the operand order: every combination of three spellings of the
`[1]`/`[2]` access (plain subscript, `*(f32 *)((u8 *)table + i * 16 + 4)`,
`((f32 *)((u8 *)table + i * 16))[1]`, `((f32 *)table + i * 4)[1]`) with three of
the `[0]` access (`table[i][0]`, `*table[i]`,
`*(f32 *)((u8 *)(i * 16) + (u32)table)`); every cast form is four words longer
rather than closer. A `struct { f32 distance, height, targetHeight, fovy; }`
declaration is the untried lever, and needs the unit's two later
`extern f32 fldCamParamTable[][4];` declarations changed with it.

## rsetOtOfs (field/fldDraw, 0x8)

Byte-exact on its own, and parked anyway. Its two instructions set
`rsetFrameMaskOtOffset`, but having them in the unit as C turns the two `bnel`
of `encDrawProc` into `bne`: the `gencMode == 0` guard at `+0x18` and the
`encDrawState` box-draw guard at `+0xE8`, both of whose arms call it. With
`rsetOtOfs` reduced to the `extern` the file already needs, `encDrawProc`
(0x1D8, 472 bytes) is exact, so the trade is 8 bytes for 472 and the unit's
match count is unchanged.

This is the callee-visibility mechanism in `docs/matching-levers.md`, measured
here: hiding `rsetTest`, `rsetAlpha`, `fldDrawBokasi` or `aDrawBoxF` instead
leaves `encDrawProc` at the same two words, and hiding `fldDrawFrameSpriteAlpha`
makes it worse (5 words). Only `rsetOtOfs` moves it.

Five functions still in assembly here call it (`fldSkyDrawFilter`,
`fldEveHit_ChkEveHit`, `fldHitChkUnit`, `fldHitChkUnitVWM`, `fldHitWM_Mask`), so
whoever converts one of those should re-measure both ways before restoring the
definition. The other three drafts in this unit do not call it and are unchanged
by the parking.

## fldHitChkUnitElv (field/fldDraw, 0x184)

63 of 97 words match, at the exact 388-byte size. The first 63 words, the whole
`fldHitChkUnitElvOnly` guard and the elevator-delta block, are retail's.

Two things differ. The record address: retail computes it once as
`addu a0, index, base` and copies it (`move v1, a0`), so two registers hold the
same pointer, and the draft keeps one register and forms it `addu v1, base,
index`. And in the tail, retail keeps nine float registers live at once -
`$f0`-`$f2` the three summed components, `$f3`-`$f5` the deltas, `$f6`-`$f8` the
three `fldHitChkWork` reads - while the draft interleaves the adds with the
`fldHitChkWork` loads and reuses six, which rotates every register in the
eighteen stores that follow. The store list itself is retail's.

Confirmed on the way: the frame holds three 4-float vectors, a result at `0x0`
and a `fldHitChkWork` snapshot at `0x10` that nothing in the function reads,
plus the delta at `0x20` that `memset` clears and whose `[3]` is `1.0f`; the
`-1` lives in a callee-saved register because `index` is a local read before
`fldHitChkUnitElvOnly` and re-read inside the guard (writing
`fldGlobalBase.hitMatterIndex == -1` twice instead makes gcc compare the second
load against the first, which it knows is `-1`); and the three components have
to be `x`/`y`/`z` locals, since `position[0] = fldGlobalBase.hitX;` reloads the
global and costs twelve words.

Ruled out: `(u8 *)(index * 0xE4) + (u32)fldMatterTable` for the record, and the
subscript form at the `worldNode` and `sceneFlag` uses (the `sceneFlag` one
costs 42 words by adding a second `addu`); `delta[0] + position[0]` for the sum;
all six orders of the three `fldHitChkWork` reads, before and after the sums;
and hiding `fldHitChkUnitElvOnly`'s definition, which leaves the one `beql` for
`beq` at `+0x5C` exactly where it is.
