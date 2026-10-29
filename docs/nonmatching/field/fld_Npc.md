# Non-matching drafts: field/fld_Npc

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## fldItemBoxAdd (0x680)

294 of 416 words differ at the native 1664-byte size, which the draft
reproduces exactly. Assembly remains active; this reconstruction carries no
exact-match credit.

The parameters are `(s32 id, const sdfVector4_t *rot, f32 x, f32 y, f32 z)`:
the caller passes the rotation by pointer in $5, not by value as HD does.

Three member groups are pinned by the stores. fldNpcSlot_t holds the spawn
position as three f32 at 0x04 and a full sdfVector4_t rotation at 0x10, copied
component by component, then seven zeroed words from 0x30 to 0x48 and the item
id at 0x4C. fldTakaraSlot_t holds the same position as f32 at 0x10, 0x14 and
0x18, and the item id again at 0x28. fldTakaraTable has a 16-byte stride and
its first word is the chest kind, which both selects the pack and lands in
fldTakaraSlot_t::mode; it is read twice because the calls in between clobber
memory.

The pack minor is 3 before the switch and 0x1C, 0x1D, 0x1E in the kind 0/3,
1 and 2 arms; the constant 3 also serves as the case-3 comparison value, which
is why it appears once.

Most of the remaining differences are one register-numbering shift: retail
holds the rotation pointer, the item id, id * 16, &rotation and &scl in $19,
$21, $23, $22 and $30, the draft in $18, $20, $22, $21 and $23, so retail has
one more high-priority address temporary in $16/$17 over the field-store run
than the draft produces. The field-store addressing differs with it: retail
reads the row base once and reaches the rotation members at displacements
0x10 to 0x1C, while the draft forms a second base at row + 0x10. The effect
load arm also differs in shape: retail branches around dds3EffectCopy with the
fldTakaraSlotCount store in the taken delay slot.

## fldNpcCalc (field/fld_Npc, 0x7B0)

487 of 492 words match, at the exact 1968-byte size. The whole structure is
settled: the four early returns, the splash branch with `vec.y` reusing the
same local the loop later uses for `fldModelSlotTable[i].y` (which is what puts
it in `$f21` rather than `$f20`), `fldNpcBlurHandle != 0` with the two guard
tests joined by `&&` so the single `fldNpcBlurHandle = 0` arm sits past the body
and ee-as can take its store into the second test's branch-likely delay slot,
`D_0054EF64 = D_0054EF64 + length` in that order so the sum lands in the
global's own register, the chest branch written as
`if (type == 999 && fldEncountTestFlag(0x40) == 0) { ...; continue; }` so a set
encount flag falls through into the slot-kind handling, `fldNpcCamTarget = 2`
after the three position stores, and the slot kind as a `switch` rather than an
`if` chain, which is what emits both case tests before either body.

Field offsets proved here: type 0x00, csize 0x24, cntf 0x34, cnti 0x38,
cntAnmf 0x3C, cntAnmi 0x40, itemBoxIdx 0x4C, camera 0x54, name 0x60; and
`fldGlobalBase` water height at 0xC4 and last move length at 0xC8, both `f32`
and both reached by cast because the header's members there are not floats.

Two scheduling pairs remain. Retail puts `swc1 $f0, 0xC($29)` (`vec.w = 1.0f`)
between the `lui %hi` and the `addiu %lo` of `fldBlurEffectId`, where the draft
keeps the address pair adjacent and the store after it. And in the chest-kind
switch retail keeps case 3's own copy of `fldCamNpcTarget = -110.0f` as the
fall-through block and branches the default away, where the draft's copy is
cross-jumped into the shared `case 0: case 2:` block and the default stays
inline; instruction counts are equal either way.

Ruled out: every ordering of the four case bodies (0/2, 1, 3 and three
permutations, each either 5 or 265 words out); the four cases split into four
separate labels, which gives the same tree and the same merge; `continue`
instead of `break` in one arm, in all arms, and an explicit `default:
continue;`; a `default:` arm holding `if (kind == 3)`, which costs 13 more
words; the chest row as a 16-byte `fldTakaraEntry_t` array instead of the
`u8` table the neighbouring `fldItemBoxAdd` uses (identical output); three
declaration positions for `vec`; a literal `0x10` for the `memset` size; a
`void **` local holding `fldBlurEffectId` and a `f32 *` element type for it;
reading the element through a `u8 *` cast; and a local for the chest kind.

The second pair is a scheduling tie that only an alias edge can break. Retail's
store and the `fldBlurEffectId[0]` load have equal critical paths to the call,
so insn order decides and the store wins; in the draft the load's chain is
longer by one and it goes first. Nothing spellable in C makes an `f32` store
into an address-taken local conflict with a `void *` array read.

## fldDrawEATexQuad (field/fld_Npc, 0x340)

143 of 208 words match, at the exact 832-byte size. The structure is settled: ten
`s32` parameters plus one `f32`, the ninth and tenth integers read from
`0x120` and `0x128` because this EABI pads each outgoing stack argument to
eight bytes (which is also why `sdfPrimLinkFt4`'s thirteen stack arguments
occupy `0x0` through `0x60`); four `sdfVector4_t` corners built in the XZ
plane from `-(ax * 16)`, `-(ay * 16)`, `+w * 16` and `+h * 16`; a
`SetRotateMatrixY((angle + 180.0f) * 3.14f / 180.0f)` with the `3.14f` out of
this unit's `.lit4` pool; `SDF_VU0_LOADVECTOR` /
`SDF_VU0_APPLYMATRIX_XYZ` / `SDF_VU0_STOREVECTOR_NOMEM` per corner, the
`_NOMEM` form being what rematerialises the corner address for the store
instead of sharing one register with the load; and the GS bias folded in as
`pos[i].x + xf + 28672.0f` and `pos[i].z * 0.5f + yf + 30976.0f` written back
into the corner.

Two things remain. The shift and negate block has to sit after the three
packet calls, not before them, or the parameters are not live across a call,
cc1 coalesces their entry copies into the shifts and the function comes out
four words short; with the block moved the entry copies appear and the shifts
fill the three delay slots exactly as retail does. What is left after that is
a swap of two callee-saved pairs, `u` and `v` taking `$21` and `$20` where
retail gives them `$23` and `$22` and the stack-read `uw` and `vh` taking
`$22` and `$23` where retail gives them `$20` and `$21`, and the store
schedule for the four corners: retail emits the zero stores in the order
`v0.y`, `v3.y`, `v1.y`, `v2.y` and pairs each coordinate store with the other
corner that shares its value, where the draft emits them in corner order.

Ruled out: the shift block before the calls (185 of 204 words and sixteen
bytes short); in-place updates of the parameters instead of six locals (which
runs past the next function when the block is after the calls, and 198 of 202
words when before); `ev` computed before `eu`; and `uw * 16 + su` instead of
`su + uw * 16`.

## fldSibukiStart (field/fld_Npc, 0x98)

28 of 38 words match, at the exact 152-byte size. `fldBlurEffectId` has to be an
incomplete array so the three handle reads go through one `lui`/`addiu` pair
in `$16` rather than `$gp`; with a plain `extern void *` the function is
136 bytes and 31 words out.

The ten remaining words are two scheduling decisions, both of the same kind:
the draft schedules the address and the load earlier than retail does.
Retail sets up `memset`'s three arguments before saving `$16` and starting
the `%hi`/`%lo` pair; the draft saves `$16` and emits the `lui` first. After
`memset`, retail stores `x`, then `w`, then `y`, loads the handle and puts
the `z` store in the call's delay slot; the draft loads the handle right
after the `mtc1`, stores `x`, `y`, `z` and puts the `w` store in the delay
slot. Instruction counts are identical in both blocks.

Ruled out: `position.w` assigned first, last, and between the coordinates;
`extern void *volatile fldBlurEffectId[]`; `extern void *fldBlurEffectId[2]`
(which goes back to `$gp` and 31 words); and a `void **p = fldBlurEffectId`
local feeding the three calls.

## fldNpcDraw (0x8E0) is not started

No draft yet; the assembly is still the only version. What a reading of retail
settles, so the next attempt does not have to re-derive it:

The frame is 0x110 with no parameters. The locals are a `sdfVector4_t` mark
position at 0x0, a second one at 0x10 whose `w` is 1.0f, a normalised direction
at 0x20, and a sprite3D record from 0x30 to 0x78 (three `f32` at 0x30, eight
`s16` at 0x40 to 0x4E, eight `f32` at 0x50 to 0x6C, two packed colours at 0x70
and 0x74). The body is `memset(&mark, 0, 0x10)` then
`if (fldGlobalBase.field04 < 200 && fldAutoMapSeqTestFlag(1) == 0)`, and
everything else sits inside that.

The chest loop walks `fldTakaraSlotTable` (0x30 stride) and switches on `mode`
with cases 0, 1, 2 and 3, case 1 an empty `break`: that four-case list is what
puts the `== 1` test at the root of the tree and leaves `mode > 1` as an
`slti 2` with an inverted branch. `activationState` then splits three ways,
`>= 3` leaving the case, `> 0` taking the item-box arm and `== 0` the effect
arm. The item-box arm reads `*(s32 *)&fldTakaraTable[slot.unk28 * 16 + 8]` and
picks `D_0054EEDC` for 2 and 3, `fldItemBoxCount` otherwise.

The billboard block computes `eye - mark` into a vector whose three components
are stored negated, takes `sqrt` of the dot product for the distance, then
normalises through the VU0 macro pair and scales the result by 50.0f. The fade
is `alpha = 0` below 500.0f, `0x80` above `D_0054C570`, and
`(s32)((dist - 500.0f) * 128.0f / D_0054C574)` clamped to 0..0x80 between,
packed as `alpha << 24 | 0x808080` and passed with `rsetOtOfs(0x2C)` before
`drawSprite3D(hEffPart, &sprite)`.

The NPC loop walks `fldModelSlotTable` (0x70 stride) and keeps five separate
base pointers into the row, one each at +0x0, +0x4, +0xC, +0x20 and +0x50,
which is what gives the loop its five `addiu` increments; `type == 0x3E7` skips
the row. Its float constants are 10.0f and 1.5f in callee-saved registers and
60.0f, 90.0f, 50.0f, 150.0f, 3.0f, 250.0f, 95.0f, 29.0f, 75.0f, 60.0f and
110.0f at their use sites.
