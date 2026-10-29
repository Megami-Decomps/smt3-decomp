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
