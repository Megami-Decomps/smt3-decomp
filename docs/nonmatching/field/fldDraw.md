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

## fldEveHit_ChkDir2 (field/fldDraw, 0x264)

121 of 153 words match, at the exact 612-byte size. Its sibling
`fldEveHit_ChkDir` is byte-exact from the same shape, so the control flow, the
three switch arms, the VU0 sequence and every constant here are retail's.

Two things differ. The pos vector's frame address: retail keeps one pseudo in
`$18` for the `memset` argument and the `sq` of the quadword copy, and
rematerialises `addiu $3, $29, 0x10` for the `lqc2` that reloads it. The draft
gets the pseudo and the `sq` right, but the `lqc2`'s copy of the address comes
out as `daddu $16, $19, $0`, one instruction in the same place and of the same
length, so the size is right and the block is one register off. And the two
callee-saved registers: retail numbers them `$18` pos, `$19` hit, while the
draft gives `$19` pos and `$18` hit, which rotates the three loads that read
`hit`.

`SDF_VU0_LOADVECTOR_RWADDR` on a `f32 *` assigned inside `case 0` is what gets
the size exact: with the plain `SDF_VU0_LOADVECTOR(pos)` the asm shares the
`memset` pseudo, the rematerialisation is gone and the function is two words
short at 55 of 151. That difference is the opposite of `fldEveHit_ChkDir`,
where the shared pseudo is what retail has, so the two macros sit side by side
in one pair of siblings.

Confirmed on the way: `gEvnRng[i]->data` is a `fldTaskInfo_t *` and
`info->shape->kind` is the three-way dispatch, which `balance_case_nodes`
splits 1, 0, 2 from `case 0:`/`case 1:`/`case 2:` written in that order;
`fldRoomState_t.worldObjectKey` at `+0x108` is compared against
`hit->serialID`; `hit->workdata` is the probe position; `unit->poseset->rot`
feeds `ToMatrix` and `unit->poseset->pos` is the eye; `D_0054C478` and
`D_0054C480` are `.lit4` entries holding `3.14f` and `D_0054C47C` holds
`0.01f`; both arms end `if (x < 0.0f) { return 0; } return 1;`, which is what
lets ee-gcc cross-jump them onto one `c.lt.s`/`bc1f` against a callee-saved
`$f20` holding `0.0f`; and the quadword copy has to go through `u128`, since
`Quad` is a four-`u32` struct and gcc copies it with `ldl`/`ldr`.

Ruled out: the plain, `_BARRIER` and `_MEMOPS` loads of `pos`; `pos` declared
as `u128`, `Quad` and `sdfVector4_t` with the copy as a whole-object
assignment; `memcpy`; five declaration positions for the pointer, all inert;
and seven placements of the pointer's assignment and of the `memset`/`sq`
spellings across it, where initialising the pointer at its declaration lets
gcse fold it back into the `memset` pseudo and costs the size again.

## fldEveHit_AddEveHitNpc (field/fldDraw, 0x184)

86 of 97 words match, at the exact 388-byte size, and the instruction multiset
is retail's.

Eleven words are left. Three are the caller-saved registers the record's
address pseudos take: retail numbers them `$3`, `$8`, `$9`, `$10`, `$11`,
`$12`, `$13`, the draft permutes the same seven. The other eight are where the
scheduler splits the `put` stores: retail finishes all four of them and then
starts the transform block, while the draft lifts `sisei->pos.x` and `.y`
between `put.type` and `put.visible`. Every instruction is present in both.

This draft is what pins the 0x90-byte record behind `D_002FCFA0`, which the
unit now declares as `fldEveHitSource_t`:

    0x00  fldResPutEveBall_t ball   { u32 type; f32 r; }
    0x08  fldResPut_t        put    { u32 type; s32 eveIndex; s32 visible;
                                      fldResPutEveBall_t *ball; }
    0x18  fldResourcePoint_t sisei  { sdfVector4_t pos, rot, scl; }
    0x48  fldResHead_t       hit
    0x6C  fldResHead_t       event

`fldResHead_t` is nine words: `resourceID`, `resourceType`, `resourceName`,
`flag`, `sisei`, `areaName`, `resourceLink`, `blockName`, `put`. Retail proves
`resourceID` at `+0x0` (the `dds3AllocObjectId` result), `resourceName` at
`+0x8`, `sisei` at `+0x10` (reloaded and handed to `dds3AddPointBasic`, whose
second parameter is the repo's still-opaque `fldResourcePoint_t`) and `put` at
`+0x20`. The two self-pointers the record carries are `put.ball` at `+0x14`
pointing at `+0x00` and `hit.put` at `+0x68` pointing at `+0x08`. `sisei` is
filled `pos = {x, y, z, 1}`, `rot = {0, 0, 0, 0}`, `scl = {1, 1, 1, 1}`.

Confirmed on the way: the parameter list is
`(char *resname, s32 eveidx, f32 x, f32 y, f32 z, f32 r, s32 hojiID, s32 npcidx)`
and the function returns nothing, which is what frees `$2` for the `* 0x130`
chain that indexes `fldEveHitTable`; `fldEveHitCount` is read into one `s32`
local for the record index and re-read from memory for the increment; and the
three `sh` into the box at `fldEveHitTable + 0x120` are `unk04 = hojiID`,
`hitKind = 1`, `npcIndex = npcidx` in that source order.

Ruled out: `&D_002FCFA0[i]` as one record pointer, which collapses to 304 bytes
because every field then shares one base register; direct `D_002FCFA0[i].sisei`
subscripts for the transform, which cost 136 excess bytes by materialising
`symbol + offset` per field; pointer locals for `ball`, `put` and `head` as
well as `sisei`; nine positions for the `sisei` pointer's assignment, all
inert except the two ends; an `s32` return, which costs one `move`; all six
orders of the box stores and of the three transform vectors; and sixty orders
of the seven `ball`/`put`/`hit` stores, of which the best is the one in the
draft.

## fldEveHit_AddEveHitItemBox (field/fldDraw, 0x1F8)

101 of 126 words match, at the exact 504-byte size, from the same record type
as `fldEveHit_AddEveHitNpc`. It differs from its sibling by registering an
event basic first: `dds3AddEventBasic(id, D_002FCD90, D_002FCD70)`, the id into
`event.resourceID`, and `fldEveHit_AddEveData(&rec->event, basic)` feeding
`put.eveIndex`, where the Npc path takes that index as a parameter.

What is left is the same two shapes as the sibling: which caller-saved register
each address pseudo takes, and one `li $6, 2` for `hitKind` that retail
schedules before the `fldEveHitCount` reload and the draft after, which rotates
the eleven words of the epilogue.

Confirmed on the way: the two `dds3AllocObjectId` results have to be two
separate locals. Reusing one `id` variable makes gcc keep the second result in
the callee-saved register the first one earned, spills `idx` to the frame and
costs 8 bytes and the whole prologue; two locals give retail's frame of 0x70
and 93 of 126 words straight away. The box stores here are
`takaraIndex = idx`, `hitKind = 2`, `npcIndex = -1`.

Ruled out: a local for the `fldEveHit_AddEveData` result and three positions
for that call inside the record block; all six orders of the box stores; and
sixty orders of the six `ball`/`put`/`hit` stores.

## fldPlayerFastSet (field/fldDraw, 0x24C)

23 of 148 words match and the draft is 576 of retail's 588 bytes. Aligned on
the instruction sequence instead, 142 of retail's 148 are the draft's in order,
and the six missing are all one shape.

Retail splits `&fldGlobalBase` into a `lui` kept in `$18` for the whole function
and a separate `addiu rd, $18, %lo(fldGlobalBase)` per basic block: one into
`$3` for the `area` test, then one into `$16` at the head of each arm and once
more after the `if`/`else` inside the first arm. The draft gets the shared
`lui` and the re-materialisation after the `if`/`else`, but computes the test's
address into the same callee-saved `$16` the arms use, so the two arm-head
`addiu` are gone; the base register then being live early also lets the
scheduler put the three `fldGlobalBase` position stores ahead of the three
frame stores, where retail has them the other way round.

`-fno-cse-follow-jumps` adds exactly the two missing instructions, which puts
the mechanism in cse extending its block through the conditional branch rather
than in anything the source states. In retail the test's address pseudo is dead
by the first `jal`, which is why it lands in a caller-saved `$3`.

Confirmed on the way: the split is `fldGlobalBase.area < 0xA`, with the
twenty-unit probe on the low side and the ten-unit probe on the high side;
`fldHitChkUnit` takes the three-float probe at `0x10` on the frame while the
`dds3GetPosPoseSet` result sits at `0x0`; the hit response is
`x = hitX`, `y = hitY + 44.0f` with another `- 11.0f` when `area == 1` in the
low arm only, and `z = hitZ`; and the tail is
`pose = ctl->handle->poseset; SDF_VU0_COPYVECTOR((u8 *)pose + 0x70, pos);
dds3OffPoseSetFlag(pose, POSESET_FLAG_POS2);`, one `poseset` local serving both.

Ruled out: `fldGb` for the outer test, for the `area == 1` test and for both,
`fldGb` being its own symbol at `fldGlobalBase + 4` and so a two-instruction
load against retail's three; a `fldAreaWork_t *` local for the whole function
and one assigned at each arm head, both of which give a `move` from the test's
register instead of a fresh `addiu`; an `s32 area` local; and both orders of
the two store groups in each arm, which the scheduler collapses to the same
output.

## fldPlayerCalc (field/fldDraw, 0x2A4)

152 of 169 words match, at the exact 676-byte size, and the whole control flow
is retail's.

Four of the seventeen left are the `motionIndex != 0xA` test: retail orders the
two setup instructions `daddu $16, $2, $0` then `addiu $2, $0, 0xA` and compares
`$4` against `$2`, the draft the other way round with `$2` against `$3`. The
other thirteen are the layout of the second half's `mode` dispatch. Retail
branches *to* both arms, `beq mode, 1` to the `fldPlayerSvsOff` arm and
`beq mode, 3` to the `fldPlayerCalc_Nml` arm it falls into, then `b` to the
default arm which sits last; the draft puts the `mode == 1` arm inline behind a
`bne`. A `switch (mode)` with `case 1:` falling into `case 3:` gives retail's
dispatch exactly, through all thirteen words, but then ee-gcc cross-jumps the
case arm's `fldPlayerMirChk(); return 0;` into the default arm's and the
function is two instructions short at 136 of 167: retail keeps two copies, one
in the case arm and one shared between the default arm and the
`fldPlayerMotionSe` path. Forcing that pairing with a `goto` into the first
half loses five instructions instead. Someone finishing this needs the switch
dispatch *and* two surviving `fldPlayerMirChk` calls at once.

Confirmed on the way: the three nested guards are
`fldRunDanteGunFlag[0] != 0`, `fldAutoMapChkMode() != 0` and
`fldEncountTestFlag(0x40) == 0 && fldGlobalBase.word80 > 0`; `fldGlobalBase +
0x80` is the curse-damage counter the second block clears; the model handle's
`mdlObj_t.first` is an `sdfMotionCtrlr_t *`, whose `speed` at `+0x20` takes
`1.0f` on the damage path and `0.0f` on the world-map path and whose `stat` at
`+0x30` is tested against 5; the damage block is
`printf`, `speed = 1.0f`, `mdlSingleMotion(mdl, 0, 0xA)`,
`fldDmgStart(x, y, z)`, `fldSePlay(0x22)`, two `dds3PadVibrationSet` and
`fldStatusSet()`; the `stat == 5` guard has to be written with the body inside
the `if` and a bare `else { return 0; }`, which is what gives retail's `bnel`
where the `!= 5` early return gives `bne` and costs 35 words; the world-map
branch's `fldWmWarpMode == 1` test has to be inverted to `!= 1` with the
`fldPlayerCalc_Wm3()` call last, which moved it from 92 differing words to 43;
and `fldGlobalBase.area < 0xA` selects the world-map paths in both halves, with
the second one a tail call, `return fldPlayerCalc_Wm2();`.

The two strings are kept as `extern char D_00531D58[]` and `D_00531D70[]`
rather than inline literals on purpose: retail pads eight bytes after
`D_00531D70` that no C emits, so promoting this draft needs
`INCLUDE_RODATA(const s32, "field/fldDraw", D_00531D58);` and the same for
`D_00531D70` alongside it, and inline literals make the unit's whole `.rodata`
differ from `0x00531D58` on.

Ruled out: `fldPlayerCalc_Nml_syukan`/`fldPlayerCalc_Nml` in the other order in
the default arm; the default arm written before the two cases; a `switch` with
`default: break;` and with the default body after the switch; a `goto ret0` on
the `stat` guard, which costs 16 bytes; and both `goto` spellings of the mode
dispatch, which ee-gcc normalises back to the draft's layout.

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

## fldInfChkFastPanel (field/fldDraw, 0x2E0)

183 of 184 words differ positionally and the draft is 8 instructions (32 bytes)
long. Aligned on the instruction sequence instead, 178 of retail's 184
instructions line up by opcode in order, with 14 extra in the draft, 6 of
retail's missing, and every register one or two higher than retail's because of
the two extra callee-saved registers.

What is left is one hoisted value. `-fno-gcse` takes the draft to 157 of 184
words with the frame size right, so gcse is keeping the loop test's result live;
nothing the source says moves it. Both `goto ret0` rewrites of the two trailing
`return 0`s and both polarities of the `value < 0xA` back edge are inert.

Confirmed on the way: the whole control flow is retail's. `fldInfSet_t` is 312
bytes and `fldInfTable.sets` starts at `+0xAC0`, so `+0xAD0` is
`sets[gFldInfIdx].flags[row - 1]` and `+0xAE4` is
`sets[gFldInfIdx].messages[row - 0xA].targets[0]`; both index expressions have
to be their own `s32` local, since written inline gcc folds the `- 1` and the
`- 0xA` into the load displacement where retail keeps a register. The outer
dispatch is a `switch` over `0`, `0x64` and `0x65` through `0x69`, not an
`if` chain: gcc's `balance_case_nodes` splits seven singleton cases at the
fourth, which is what makes `0x66` the first value tested, and every arm but
`0x64` is `return value;` with cse substituting the constant in the arms a
plain `beq` proved equal and leaving `daddu $2, $5, $0` in the two reached
through an `slti`. The inner dispatch on `gFldWapPanel` needs `case 0:`,
`case 1:`, `case 2:` and `case 4:` written out alongside the `default:` to get
the `sltiu $2, $3, 0x8` bound test and the eight-entry `jtbl_00531E40`; with
only 3, 5, 6, 7 and a default gcc emits a compare chain. The back edge is a
self tail call, `return fldInfChkFastPanel(value);`, which is the row now in
`docs/levers/control-flow.md`.

This draft depends on four fields of the unit's own `fldInfTable_t` changing
from `u8` to `s8` (`fldInfSet_t.eventHit`, `fldInfFlagSelector_t.offTarget` and
`.onTarget`, `fldInfMessageRow_t.targets`), which retail's four `lb` prove and
which no compiled function in the unit reads. The size and offset of each is
unchanged and the unit still matches, but a harvest that takes the body without
the declarations will see `lbu` on all four.

Ruled out: `row - 1` and `row - 0xA` written inline at each use; a `for (;;)`
with `continue` and with a `goto` for the back edge; the final block written as
the branched-to arm rather than the fall-through, which costs another 8 bytes;
`if (fldDoor_GetHasigoTbl(0) != 0) { return 0x69; }` for the `case 5` select,
which gives `movz` against retail's `movn`; and `goto ret0` on either of the
two trailing `return 0`s.

