# Non-matching drafts: field/fld_Npc

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## fldNpcLightSet (field/fld_Npc, 0x28C)

125 of 163 words, at the exact 652-byte size. Everything the source can say
is said: the light row is `D_003129D0 + (fldArea[0] + 1) * 8` with the
direction mode in byte 7 and the face-division nibbles in byte 6, the
`if (m == 0) ... else if (m == 3) ... else` chain with the last arm repeating
the first (gcc cross-jumps the two into one block, which is what gives
retail's `beqz` and its `bne $3, 3` both targeting `.L0014AF28`), each arm
carrying its own `3.14f` and `0.6f` so the four pool entries at `0x54C4F8`
through `0x54C514` come out one per arm, `dir = dir * 180.0f / 3.14f;` split
from `dir = dir - 70.0f;` so the division writes `dir`'s own register, and
`rad = dir * 3.14f / 180.0f;` placed after `sdfModelSetFaceDivParm` so `dir`
and the light power stay live across the three calls in `$f21` and `$f22`.

The remaining 38 words are one hard-register permutation of the four
callee-saved integers. Retail allocates `object` `$16`, the model handle
`$17`, the light row `$18`, the unit `$19`; the draft gets row `$16`, object
`$17`, unit `$18`, handle `$19`. Every instruction is otherwise the same, the
float registers included, and the two colour words land in `$17` and `$16` in
both. gcc 2.96 sorts allocnos by `floor_log2(n_refs) * freq / live_length`,
so this needs a live range changed, not a statement reworded.

Ruled out: a `mode` local for `row[7]` (60 words); a `div` local for
`row[6]` (39); the row as a pointer to an eight-`u8` struct, as a `u32`-cast
read, and as a plain `u8 *` (all 38); computing the row before
`dds3ObjectBaseGetModelHandle`, after `EulerAngleAxisY` and before the
`SDF_VU0_LOADVECTOR` (53, 47, 48); the ambient word computed before the light
word (42); the blue component or'd before the green one (38);
`dds3ObjectBaseGetModelHandle(unit->effObj)` with `object` read afterwards
(39); a `switch` over the four modes, which runs past the next function; and
seven declaration-order permutations of the nine locals, all 38.

## fldDrawEATexQuad (field/fld_Npc, 0x340)

143 of 208 words, at the exact 832-byte size. The structure is settled: ten
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

28 of 38 words, at the exact 152-byte size. `fldBlurEffectId` has to be an
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
