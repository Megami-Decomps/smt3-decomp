# Matching notes: field/fld_Npc

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

All 492 instruction words match at the native 1,968-byte extent. The
original field-frame gates, blur and splash callbacks, slot iteration,
chest animation, and NPC camera updates retain their order.

In the blur branch, the real scene base stays in its native callee-saved
register across the callbacks. The initialized W lane of the position
vector is an operand alongside the effect slot bank, retaining the
native W store between the address high and low instructions. The actual
first effect handle is loaded separately for the state and position
calls. The existing local water-height snapshot is unchanged.

The chest switch leaves unknown types alone. Types 0, 2, and 3 write
-110.0f; type 1 writes -90.0f. On the 0/2 path, an empty F32 input retains
the actual value used immediately in the distance store. In the case-3
path, an empty immediate operand uses the symbolic address of the very
object the ordinary C store writes; it does not load the old distance or
replace the store. The first operand prevents merging the two native
-110.0f stores. The address operand retains the native non-annulled
branch and common animation-offset reload instead of the compiler's
annulled form. These are local compiler reconstruction aids, not evidence
of the original spelling.

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

Matched at 152 bytes. The stack vector remains the real argument to
the position provider. Scoped operands keep Y in its native saved float
register through the zeroing call and reproduce the order of initialized
X, W and Y stores before reading the reset handle. No vector field or handle
table read is made volatile. The later position and loop calls each read the
current handle from the table, as in retail.

### Earlier guarded reconstruction

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


### Effect-provider contracts and fresh splash observation (2026-11-06)

The field owner's declarations for `dds3EffectReset`, `dds3EffectSetPosition`
and `dds3EffectSetLoop` now agree with their canonical `effManager.c`
providers: the handle is `struct effNode_t *`, and SetPosition's vector
parameter is `sdfVector4_t *`. The existing incomplete node declaration is
reused. All passed vectors are mutable local objects. This changes no body,
handle storage, dereference, callback, access width, call order or provider.
It removes a cross-unit function-type disagreement, not a claim that every
position callback writes its argument.

Independent native review confirms the node type/instance loads and callback
slots. The three declarations are literally byte-neutral for complete
assembly and entire ELF objects in production, `SKIP_ASM`, and retained-draft
contexts, both with and without activating the splash diagnostic. All twelve
source/context qualifications and twelve strict builds preserve full outputs.
The complete guarded owner remains 17 match, 0 differ. No new matching credit
is claimed, and `fldSibukiStart` remains guarded at 28 of 38 matching words.

A fresh qualified actual-compiler observation separates the splash residuals.
After reload, the s0 save has priority 13, ahead of the memset destination
and zero arguments at 12; its address-formation dependency then starts early.
The effect-handle load has priority 11 versus the vector stores' 9. Among
stores with equal priority and weight, dependent-count comparisons prefer
coordinate stores over the w store, leaving w for the Reset delay slot rather
than native z. This is not a source-assignment-order tie.

The observation preserves all 28 complete artifacts, including full assembly,
ELF and stock dumps, with opcode, target-entry, completion and owned-process
cleanup checks. Its conservative frozen inputs and matching runtime route
were independently audited. It supplies no new genuine source dependency
that closes these scheduling gaps. Earlier assignment-order, volatile-handle,
fixed-array-bound and pointer-local probes remain closed controls.

## fldNpcDraw (field/fld_Npc, 0x8DC)

145 of 561 words match, six words short of the native 2268 bytes. The whole
structure is settled and reproduced: `memset(&pos, 0, 0x10)` with `pos.w = 1.0f`
under `fldGlobalBase.area < 200 && fldAutoMapSeqTestFlag(1) == 0`; the chest loop
over `fldTakaraSlotTable` as `switch (mode)` with `case 1` an empty `break` and
cases 0, 2 and 3 sharing the body; the three-way `activationState` split written
`if (as >= 3) break; if (as <= 0) { if (as != 0) break; A } else { B }`, which is
what puts retail's `bgtzl` before the `bnel` and leaves the `as == 0` arm as the
fall-through block; the item-box arm's chest kind as
`switch (*(s32 *)&fldTakaraTable[slot->unk28 * 16 + 8])` with cases 2 and 3 against
a `default`; the billboard block with `dir` holding the negated eye delta and
`w = 1.0f`, `fsqrtf` of the dot product, `SDF_VU0_LOADVECTOR` /
`SDF_VU0_NORMALIZE` / `SDF_VU0_STOREVECTOR` into a third stack vector, and
`pos -= norm * 50.0f` written back into both `pos` and `sprite.pos`; the fade as
`alpha = 0; if (dist < 500.0f) {} else if (dist > 3000.0f) ... else
(s32)((dist - 500.0f) * 128.0f / 2500.0f)` clamped to 0..0x80, where the empty
then-arm is what gives retail's `c.lt.s dist, 500.0f` with an inverted branch
rather than the `c.le.s 500.0f, dist` an `if (dist >= 500.0f)` produces; and
`eye = aViewEye` as `SDF_VU0_COPYVECTOR(&eye, &aViewEye)`, the only spelling that
gives retail's `lq $2`/`sq $2` through two `addiu` address temporaries.

The frame comes out at the native 0x110 with the same five aggregates at the same
offsets: `pos` at 0x0, `dir` at 0x10, `norm` at 0x20, a `Sprite3D_t` at 0x30 and
`eye` at 0x80. `3000.0f` and `2500.0f` are the unit's last two `.lit4` entries,
`0x54C570` and `0x54C574`; every other constant has a zero low halfword and comes
out as `lui`/`mtc1`.

Two things remain, and the second follows from the first.

The chest loop hoists one float invariant too many. Retail hoists only `64.0f`
and `-64.0f`, the eight sprite corner values, into `$f24` and `$f23` and
rematerialises `50.0f` with a `lui`/`mtc1` pair inside the body; the draft hoists
`50.0f` as well, two extra words in the preheader. With `$f21` spent on it, the
eye deltas and the distance land in `$f7`, `$f6`, `$f4` instead of retail's
`$f20`, `$f21`, `$f22`, which is most of the remaining word count: the corner
stores, the three scaling multiplies, the `500.0f` compare and the store schedule
around the `sprite.pos` writes all differ by register name only.

The NPC loop builds three induction variables where retail builds five. Retail
keeps byte offsets for +0x0, +0x20 and +0x50 plus row pointers at +0x4 and +0x30;
the draft keeps +0x0, +0x50 and the +0x30 pointer, and recomputes `base + 4` and
`i * 0x70 + 0x20` inside the body. The missing +0x20 offset variable is why retail
re-reads `object` as `addu $2, giv, base` before each `dds3ObjectBaseOnFlag` while
the draft shares the one address it computed for the `dds3GetPosPoseSet` call, and
the missing `base + 4` pointer is why `csize` reads through an `addu` instead of
retail's `0x20($21)`.

Ruled out: the uv and corner block before the position scaling (295 words out
against 278); `sprite.pos.x = pos.x = ...` chained assignments and three named
`f32` temporaries for the scaled components (both identical to the plain form);
reordering the three scalings x, z, y (282); `alpha = 0` moved from before the
scaling to after the corner stores; inlining the `evtUnit_t *` local into the
`sdfModelGetMarkPos` argument; `if (kind < 4) { if (kind >= 2) ... }` with the
`default` arm written twice, which gcc cross-jumps into the same code the `switch`
gives; and `count > 0x80 ? ... ` as a single clamp plus one store, which gives a
`movz` where retail cross-jumps two `fldNpcDrawCount` stores into one and puts the
`0x80` in a branch-likely delay slot.

## Treasure-box object ID

The retained `fldItemBoxAdd` passes `fldTakaraUnitName` to
`dds3SetUnitObjectId`. The setter stores the pointer, not the string's
contents. The const initializer owns the 12-byte `TAKARA_UNIT` string
including its terminator. The next four alignment bytes stay unchanged
and are not part of the C object. Both bounding vectors and the native
item-box constructor are unchanged.
