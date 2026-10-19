# IDB corrections

The IDA database for `SLUS_209.11` is addressed by module name, not by port: the port it
answers on changes between sessions (13339 one day, 13337 the next, with another game's
database on the number it vacated). Always pass `SLUS_209.11`. It carries SMT3HD
symbol recovery. That naming is accurate but lossy: it comes from a later build whose structs
grew, so a few types describe memory this ELF does not have. Where the retail code proves a
name or type wrong, the fix belongs in the IDB, because `config/smt3/symbol_addrs.txt` is
generated from it and an edit to the generated file is lost on the next export.

Order of proof, strictest first: an allocation site that gives a size, an init function that
writes a field, a stride in an indexed access, a caller passing a known argument. A name that
only looks odd is not proof.

After each correction: re-run `tools/ida_symbols.py --instance SLUS_209.11`, re-run
`configure.py`, build, and confirm the image still hashes to
`5facfbf7916836613449c922e26a4ec412207b32`.

## Struct layouts

| Type | Old | New | Proof |
| --- | --- | --- | --- |
| `sdfMat_t` | 160 bytes, trailing `reserved` | 72 bytes, ends at `fdSoften` | `sdfMatNew` (0x002B31E8) allocates 0x48 for the material |
| `sdfMatPacket_t` | 128 bytes | 160 bytes: `f32 texTm[6]` at +0x68, `f32 mtexTexTm[6]` at +0x80, vifcodes at +0x98 and +0x9C | the same function allocates 0xA0 per packet and stores through +0x98 and +0x9C |
| `sdfModelRootNode_t` | 168 bytes | 152 bytes, ends at `f32 zBias` | `sdfModelRootNew` (0x002B0540) calls `sdfChipAllocZeroDirect(152)` |
| `sdfModelNodeDef_t` | `f32 bounds` at +0x40 | `sdfModelBounds_t *bounds` at +0x40 | `sdfModelNodeNew` (0x002B11D0) copies def+0x40 into `sdfModelNode_t::bounds`, which `sdfModelCheckBounds` reads as `min[3]`/`max[3]` |
| `sdfFileCmdHandle_t` | HD layout past +0x10 | 64 bytes: `filename` +0x10, `device` +0x14, `cmd` +0x15, `stat` +0x16, `offset` +0x18, `buffer` +0x1C, `length` +0x20, `cmdParm` +0x24, `errorCode` +0x28, `fd` +0x2C, `currentOffset` +0x30, `remainder` +0x34, `callback` +0x38, `userData` +0x3C | the command allocator at 0x002BF328 takes 64 bytes and writes those offsets |

`sdfModelRootNode_t`'s 152 bytes are not settled: the repo body carries a `lodIndex` byte at
+0x98, past the end of a 152-byte object, commented as a field the native model viewer and the
`MODEL_LOD_CHG` setters write. Either the allocation size or that field is wrong. See
[Type recovery](type-recovery.md).

`datUnitWork_t` was recorded here as the one case where the IDB's field
*order* does not describe this ELF. **That was wrong, and the record is
corrected below.** The IDB's order is right; `include/newdata.h` is what is
six bytes out.

The mistake is worth keeping because the method failed, not just the answer.
Three functions do load +0x38, +0x6A, +0x6C and +0x7A, which is what was
measured. Changing the struct layout without changing the field *references*
in the source then broke the build, and that was read as retail vindicating
the header's names. It only ever proved the offsets. A matching build says
nothing about which name sits at an offset, which is the same trap recorded
under [Type recovery](type-recovery.md): a type built to make bytes match
proves the offsets and never the names.

What the HD bodies actually say at those offsets:

| Offset | `newdata.h` says | HD body says | Evidence |
| --- | --- | --- | --- |
| +0x38 | `skill[2]` | `skillcnt` | `rstChkAddSkill` is `if (pCurrentStock->skillcnt <= 7) return 1; else return 2;`, and retail reads a halfword there and tests `sltiu 8` |
| +0x6A | `nowtform` | `nowcommand` | `SetAction_AI` is `work->nowcommand = -1; work->nowindex = 0; nbRunAi(a);`, and retail stores -1 and 0 at +0x6A and +0x6C |
| +0x7A | `levelupparam` | `mitamaparam` | `datGetBaseParam` sums `param`, `skillparam` and `mitamaparam` |

So the PS2 record is the IDB's order at PS2 widths: `namecode` is `u16[8]`
spanning +0x28 to +0x38, `skillcnt` is a `u16` at +0x38, `skill[24]` runs
+0x3A to +0x6A, and the now/prev block starts at +0x6A. That tiles to 0xEC
with no gap, which removes the invented `unk80[6]`, and 0xEC is the stride the
party records at `DDS3_GBWK + 0xA54` are walked with.

**Applied.** `include/newdata.h` carries the corrected layout, and the
references in `newbattle/nbStageProcess.c`, `newdata/datCalc.c` and
`code_001FAE80.c` are remapped to whichever field now occupies the offset they
load. `nbMakePacket.c` needed nothing: its local copy was already right.

The build was the oracle rather than the argument. Both layouts total 0xEC, so
relaying out the header and rebuilding named exactly the six functions whose
references had to move: `rstChkAddSkill`, `datGetBaseParam`, `SetAction_AI`,
`SetAction_AISCR`, `nbCountActiveUnits` and `MemoryCommand`. The first three
are the HD witnesses above, which is the cross-check that the layout and the
names agree.

Two traps in the remap worth knowing if another struct is ever relaid out this
way. `nbMainProcessData_t` has its own `prevcommand` and `previndex`, so a
rename by field name alone silently hit the wrong struct; the references have
to be selected by the variable's type. And `nbStageProcess` was already
reading two of them as `*(s16 *)&work->mitamaparam[2]`, a cast that says the
author knew a halfword lived there: those are `prevtarea` and `prevtform`, now
read directly.

The assertions in `tools/header_layout.c` are restored with the proven names,
plus `skillcnt` at +0x38, `skill` at +0x3A, `nowindex` at +0x6C,
`mitamaparam` at +0x7A, `levelupparam` at +0x80 and the 0xEC size.

## Member types

| Type | Member | Old | New | Proof |
| --- | --- | --- | --- | --- |
| `sdfGsMemSlice_t` | `pixelFormat` (+0x18) | `u32` | `s32` | `sdfSetLoadImage` (0x002AC530) reads it with `lw` at 0x002AC59C, and `sdfGrSetTextureScenePacket` again at 0x002AE900. Declaring it `u32` makes ee-gcc emit `lwu` and turns those two words into `0x9E4B0018`/`0x9E470018` |

Found while renaming the drifted member names onto the IDB's: the name was
drifted (`format` → `pixelFormat`) but the repo's signedness was right, so the
rename had to take the name alone. The IDB has been corrected and saved; the
struct's size is unchanged at 28 bytes and `symbol_addrs.txt` is unaffected,
since member types are not exported.

`sdfGsMemSlice_t.type` is the mirror case and is **not** corrected: the IDB
says `s32` where every repo copy says `u32`. No load of it is signedness
sensitive, so nothing in the retail code decides it, and changing it on the
IDB's word alone would be a guess dressed as a correction.

## Globals

| Address | Name | Old type | New type | Proof |
| --- | --- | --- | --- | --- |
| 0x0054E854 | `startinglistl` | none | `dds3ProcessID_t *` | `AddProcessList` (0x00100760) stores here the same process pointer it stores to `startinglistf`, then loads it back to write `->next` |
| 0x0054E860 | `endinglistl` | none | `dds3ProcessID_t *` | same function, same pattern against `endinglistf` |
| 0x0054E86C | `runninglistl` | none | `dds3ProcessID_t *` | same function; `DeleteProcessList` (0x001005E8) stores `setp->prev` here |

The three `*listcnt` words beside them stay untyped counters: `AddProcessList` loads,
increments and stores them and never dereferences them.

## Names

| Address | Old | New | Proof |
| --- | --- | --- | --- |
| 0x0032C420 | `off_32C420`, with `modelFileKindTable` one word further on | `modelFileKindTable` | `mdlGetNumOfMinor` (0x001DDB90) indexes the table at stride 8 and returns the second word of the entry, so the entry starts at 0x0032C420 and the old symbol named its `count` field. `mdlMakeHddFileList` (0x001DE5B8) and `mdlReleaseHddFileList` (0x001DE398) write both words of entry 5, the HDD slot, as `{entries, count}` |
| 0x0054E854 | `startinglistl_0` | `startinglistl` | the `_0` suffix is an IDA uniquifier with no surviving twin, and the word is the tail of the list whose head and counter are `startinglistf` and `startinglistcnt` |
| 0x002C04E8 | `SDF_QUAT_INNERPRODUCT` | `Normalize` | the body is `vrsqrt Q, vf0w, vf3w` then `vmulq.xyzw vf10, vf10, Q`, so it scales all four lanes of vf10 by one over their length and returns nothing. An inner product returns a scalar; this cannot be one. `Slerp` and `Lerp` both end with it, which is where a normalize belongs |
| 0x002C0538 | `sdf_2C0538` | `SDF_QUAT_INNERPRODUCT` | the body reduces `vf10 * vf11` over all four lanes through the accumulator and returns the result as an `f32`. HD's `sdfQuat$$SDF_QUAT_INNERPRODUCT` ends in `UnityEngine_Quaternion__Dot(A, B)`, and its `sdfQuat$$InnerProduct` is only a wrapper around that worker |
| 0x002C04B8 | `sdf_2C04B8` | `Inverse` | squared norm into vf3.w, `vdiv Q, vf0w, vf3w`, `vsub.xyz vf2, vf0, vf10` to negate xyz while `vmove.w vf2, vf10` keeps w, then `vmulq.xyzw vf10, vf2, Q`: the conjugate over the squared norm. HD's `sdfQuat` has `Inverse` |

The three quaternion rows are one shuffle, not three independent renames: the name
`SDF_QUAT_INNERPRODUCT` was on the normalize and had to move to the dot product. Six call
sites across `basic/dds3Basic.c`, `basic/dds3EffectObjectBasic.c`, `kernel/dds3KernelDraw.c`
and `code_002BD340.c` called it meaning the normalize, and renaming the symbol without them
re-pointed three `jal` targets from 0x002C04E8 to 0x002C0538. The checksum caught it.

`sdf_2C0510` (0x002C0510) is left alone. It reduces vf10 to one float, which by elimination
would make it `Length`, the only scalar reducer left in HD's `sdfQuat` list. But its
accumulator chain starts `vmulay.x ACC, vf2, vf2y` where the dot product uses `vadday.x`, so
it computes `(x*x)*(y*y) + z*z + w*w`, which is neither a length nor a dot. HD's
`sdfQuat$$Length` is an empty stub and settles nothing. None of these three functions has a
single cross-reference in the ELF, so there is no caller to appeal to either.

Entry 0 of `modelFileKindTable` is `{0x0032A858, 2}` and the table runs seven entries to
0x0032C458, where `mdlResrcMajorList` starts. `modelFileTable` (0x0032A820) has the same
`{entries, count}` shape.

## Proposed, with the proof, not yet applied

`include/newdata.h`'s `datUnitWork_t` is six bytes out from +0x28 on, and the
local copy in `newbattle/nbMakePacket.c` is the correct one. The retail code
and the HD bodies agree against the header in three places:

| Offset | Header calls it | It is | Proof |
|---|---|---|---|
| +0x38 | `skill[2]` (`skill` at +0x34) | `skillcnt`, a `u16` | `rstChkAddSkill` (0x0021FFB8) reads the halfword and tests `sltiu 8`; the HD body is `if (GBWK->pCurrentStock->skillcnt <= 7) return 1; else return 2;`, which is also what the function's name says |
| +0x6A, +0x6C | `nowtform`, `prevcommand` | `nowcommand`, `nowindex` | `SetAction_AI` (0x00253BF8) stores `-1` and `0` there and calls `nbRunAi`; the HD body is `work->nowcommand = -1; work->nowindex = 0;`. Ten AI and bad-status setters in `nbMakePacket` write the same pair as a command plus its skill index |
| +0x7A | `levelupparam` | `mitamaparam` | `datGetBaseParam` (0x00243E78) reads `param[i]` at +0x16, `skillparam[i]` at +0x1C and this array; the HD body sums `param`, `skillparam` and `mitamaparam` |

So the PS2 record is `namecode` as `u16[8]` at +0x28, `skillcnt` as `u16` at
+0x38, `skill` as `u16[24]` at +0x3A, then the eight `now*`/`prev*`
halfwords from +0x6A, `mitamaparam` at +0x7A, `levelupparam` at +0x80 and
`keisyoskill` at +0x86. Every field tiles to 0xEC with no gap, which also
removes the header's invented `unk80[6]`, and 0xEC is the stride
`nbGetParamAvg2` and `nbGetLvAvg2` walk the party records at
`DDS3_GBWK + 0xA54` with.

This is not applied because the header's names are already used by matched
code in three units this pass may not touch: `nowtform`/`prevcommand` in
`newbattle/nbStageProcess.c`, `levelupparam` in `newdata/datCalc.c`, and
`skill[2]` in `code_001FAE80.c`. Both layouts agree on every offset any of
them loads, so the rename is a one-pass edit of the header plus those three
files with no codegen change, and it has to be done with nothing else in
flight. `tools/header_layout.c`'s `OFF` rows for `nowcommand`, `nowtform` and
`levelupparam` move with it.

Five `fldGbFlags_t` accessors in `code_00116AD0` have the same body as
`fldGetModelFlag` and differ only in the field they touch, which the retail
code makes plain. Their current names describe something else entirely:

| Address symbol | Reads or writes | Current name is wrong because |
|---|---|---|
| `dds3ObjectBaseAddModelOt` | `animeFlag` (+0x02) | it is a flag getter, not an object-base draw call |
| `dds3EffectObjectAddFld` | `modelFlag2` (+0x06) | same |
| `fld_120C30` | `animeFlag2` (+0x08) | unnamed, and the field identifies it |
| `dds3EffectObjectFindFld` | `animeId2[slot]` (+0x0A), returning -1 on 0xFF | same |
| `fldGetAnimeId2` | writes `animeId2[slot]` | it is a setter, so the `Get` prefix is backwards |

The HD build names the sibling cluster `fldGbSetAnime`, `fldGbSetHit`,
`fldGbSetNpc` and `fldGbChkTakaraOpen`, so names from that family fit.

These are not applied yet because a rename in the IDB changes
`config/smt3/symbol_addrs.txt` on the next export, which renames the symbols
every C reference and every `INCLUDE_ASM` line uses. That has to be done in
one pass with the source updates and a relink, while no other work is in
flight, or `check_unit.py` reports `STALE` against the old names.

## Examined and left alone

- `sdfChipCB` (0x00577EC0) is already `sdfChipCB_tag[7]`, which `sdfChipInit` (0x002A8D38)
  confirms: a 7-iteration loop stepping one 12-byte control block at a time over 0x54 bytes.
- `sdfMemCB` (0x00577F18) is already `sdfMemHandleCB_ps2_t`, 40 bytes, matching the item size.
  The HD spelling `sdfMemHandleCB_t` does not exist in this database and would change nothing.
- `dds3CleanupList` (0x00552070) is already `sdfDelayedResrcCB_t`, which `sdfChipInit` proves by
  passing its address to `sdfDelayedResrcInitCB`. Renaming it to the HD's
  `sdfChipDelayedResrcCB` is not something the code proves, so the name stands.
- The kernel list heads `startinglistf`, `endinglistf` and `runninglistf` are already
  `dds3ProcessID_t *`.
- `sdfShape_t` exists as a local type but no function prototype or global in the database refers
  to it, so the retail code says nothing about it either way. Left in place.
