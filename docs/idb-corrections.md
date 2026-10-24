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

After each correction: re-run `tools/ida_symbols.py --instance SLUS_209.11` and
`tools/ida_types.py --instance SLUS_209.11`, re-run `configure.py`, build, and confirm
the image still hashes to `5facfbf7916836613449c922e26a4ec412207b32`.

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

## Type names

| Repo name | IDB name | Evidence |
| --- | --- | --- |
| `effBTLLightResource_t` (`effBattle.c`) | `fldResourceLight_t` | identical member for member, 13 members, 52 bytes, same order and types; the repo name was invented. Its members were lowercase against the IDB's `Flag`, `AnimeType`, `InsideR`, `DiffuseR` and the rest |

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

## Applied on 2026-10-20: datUnitWork_t and five fldGbFlags_t accessors

`include/newdata.h`'s `datUnitWork_t` now carries the corrected layout, which
the retail code and the HD bodies agree on against the old one: `namecode` as
`u16[8]` at +0x28, `skillcnt` as `u16` at +0x38, `skill` as `u16[24]` at +0x3A,
the eight `now*`/`prev*` halfwords from +0x6A, `mitamaparam` at +0x7A,
`levelupparam` at +0x80 and `keisyoskill` at +0x86. It tiles to 0xEC with no
gap, which removed the invented `unk80[6]`, and 0xEC is the stride
`nbGetParamAvg2` and `nbGetLvAvg2` walk the party records with.
`tools/header_layout.c` asserts the size and the moved offsets.

The proofs were `rstChkAddSkill` at 0x0021FFB8 reading the +0x38 halfword and
testing `sltiu 8` against the HD body's `skillcnt <= 7`; `SetAction_AI` at
0x00253BF8 storing -1 and 0 at +0x6A and +0x6C before `nbRunAi`, against the HD
body's `nowcommand = -1; nowindex = 0;`; and `datGetBaseParam` at 0x00243E78
summing `param`, `skillparam` and the +0x7A array, which is `mitamaparam`.

Five `fldGbFlags_t` accessors in `code_00116AD0` were renamed in the IDB, which
`tools/ida_symbols.py` then exported, and every C reference was updated in the
same pass. Each body is `fldGetModelFlag`'s with one field changed, which is
what identifies it, and the old names described something else:

| Was | Now | Body |
|---|---|---|
| `dds3ObjectBaseAddModelOt` | `fldGetAnimeFlag` | returns bit `b` of `animeFlag` (+0x02) |
| `dds3EffectObjectAddFld` | `fldGetModelFlag2` | returns bit `b` of `modelFlag2` (+0x06) |
| `fld_120C30` | `fldGetAnimeFlag2` | returns bit `b` of `animeFlag2` (+0x08) |
| `dds3EffectObjectFindFld` | `fldGetAnimeId2` | reads `animeId2[slot]` (+0x0A), -1 on 0xFF |
| `fldGetAnimeId2` | `fldSetAnimeId2` | writes `animeId2[slot]`, so `Get` was backwards |

The names follow the setters the unit already carries (`fldSetAnimeFlag`,
`fldSetAnimeFlag2`, `fldSetModelFlag2`) and the getter `fldGetModelFlag`. The
setter was renamed before the getter took its name, to avoid a collision. The
two callers outside the unit, in `basic/dds3ObjectBase.c` and
`field/fldFileResolver.c`, already used the function as a flag getter
(`flag = ...(area, floor + 1, bit)`), which is further evidence the old name
was wrong. The image is byte-identical afterwards and all five affected units
report `0 differ`.

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

## Applied on 2026-10-24: the three SDF IRX databases

These are separate IDBs from `SLUS_209.11` and nothing in them feeds
`symbol_addrs.txt`, so none of this affects the build. The same rule applies
anyway: where the module's own code proves a name wrong, fix it in the IDB.
The map is in [The SDF IRX modules](sdf-irx-modules.md).

### Function boundary

`SDFSDMAN.IRX` had its entry split in two: an 8-byte `sub_0` holding the
prologue and the body as `sdfSdManInit` at 0x8. Hex-Rays answered with 24
phantom arguments because the stack adjustment was outside the function. Both
were deleted and one function defined over 0x0..0x110, which decompiles into
three clean `CreateThread` blocks.

### Names the code contradicted

| Module | Address | Was | Now | Proof |
| --- | --- | --- | --- | --- |
| `SDFSDMAN` | 0xC40 | `sdfSndMidiLoad` | `sdfSndSeqThread` | `sdfSndInitSynth` creates it as a thread at priority 30 and stores its id where `sdfSndTimerSetup` registers the hardware timer handler, which only calls `iWakeupThread` on it. The body opens with `SleepThread` and runs the periodic service work. It loads nothing |
| `SDFIOPFL` | 0x474 | `loadcore_ReleaseLibraryEntries` | `loadcore_RegisterLibraryEntries` | ordinal 6, not 7, and the failure path prints `RegisterLibraryEntries fail` |
| `SDFSDMAN` | 0x5678 | `sifman_SifSetDChain` | `sifman_sceSifSetDma` | called as `(sifdma_t *, 1)` and its result is the id passed to the next call |
| `SDFSDMAN` | 0x5680 | `sifman_SifSetDma` | `sifman_sceSifDmaStat` | takes that id, is polled until negative, and the complaint is `sceSifDmaStat didn't finish` |
| `SDFSDMAN` | 0x54A8 | `modhsyn_e7` | `modhsyn_sceHSyn_VoiceTrans` | its return is branched on straight into `sceHSyn_VoiceTrans error!` |
| `SDFSDMAN` | 0x5514 | `modmidi_e16` | `modmidi_sceMidi_MidiSetLocation` | same, into `sceMidi_MidiSetLocation error!` |
| `SDFSDMAN` | 0x5540 | `modmsin_e7` | `modmsin_sceMSIn_PutMsg` | same, into `sceMSIn_PutMsg fail.` |
| both small modules | | `sysclib_e42` | `sysclib_vsprintf` | ordinal 42, and the caller formats into a stack buffer then calls `printf` |
| `SDFIOP` | 0x428, 0x430 | `modload_e8`, `modload_e10` | `StartModule`, `LoadModuleBuffer` | ordinal 10 takes only a buffer and its failure prints `LoadModuleBuffer fail`; ordinal 8 then takes `(modid, path, arglen, args, &result)` |

`modhsyn` ordinals 5, 8, 13, 15 and 18 and `modmidi` ordinal 5 are left as
ordinals. They are used in ways consistent with a tick, an attribute set and a
voice stop, but no string or signature pins them, and a guess dressed as a
recovered Sony name is worse than `e13`. `thbase` ordinal 43 is likewise left
alone: it returns a free-running clock that `p2sdIrxSpuIopMap` differences
against 33000000 for its timeout, which does not identify it.

### Naming pass

All 182 functions across the three databases now carry names, up from 121, and
every referenced global that is not a string literal is named in `SDFSDMAN`.

## Applied on 2026-10-24: types and prototypes across the SDF IRX databases

Every function in the three databases now carries a prototype and every global a
type; the reconstructed structs and the two findings that came out of typing
are in [The SDF IRX modules](sdf-irx-modules.md). Two more import names were
disproved in the process, both in `SDFSDMAN`:

| Address | Was | Now | Proof |
| --- | --- | --- | --- |
| 0x5724 | `thbase_JoinThread` | `thbase_DelayThread` | called as `(1000)` inside a DMA retry loop and `(10000)` in an idle loop, return value discarded. Those are not thread ids, and joining a thread is not what paces a retry |
| 0x572C | `thbase_iCancelAlarm` | `thbase_USec2SysClock` | `sdfSndTimerSetup` passes 4167 and the address of an uninitialised stack word, then reads that word back and hands it to `SetTimerHandler` as the compare value. `iCancelAlarm` does not write through its second argument. 4167 µs is the 240 Hz sequencer tick, and the same 4167 goes to `sceHSyn_Init` and `sceMidi_Init` |

Both are named for the behaviour the data flow proves, not for a guessed Sony
spelling: ordinal 33 delays by the microseconds it is given and ordinal 39
converts microseconds to timer ticks, whichever of the SDK's spellings each one
actually is.

One trap worth knowing when retyping through MCP. `declare_type` will edit a
named type's body but will not change its kind, so redeclaring a `union` as a
`struct` of the same name leaves every global still typed as the union, and
Hex-Rays merges adjacent field stores into one wide store through the union's
largest member. Delete the named type first, then declare the replacement.
Deleting it also orphans any pointer already applied to a global, which then
prints as `struct #24 *`; re-apply the pointer type afterwards.
