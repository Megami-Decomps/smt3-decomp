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

## The ADPCM state is one struct

`SDFSDMAN` had 25 separate globals from 0x1A810 to 0x1A847, named one at a
time while the module was being mapped. The code proves they are the fields of
a single object: `sdfAdpcmFadeOut` loads 0x1A812 into a register and reaches
0x1A820 as `0xE` off that same register, which gcc only does for two fields of
one object. They are now `sdfAdpcm`, a `sdfAdpcm_t` at 0x1A810, and the field
names are the old global names with the prefix dropped.

The header, the two semaphore parameter blocks and the two semaphore ids after
it stay separate: `sdfAdpcmSpu2IntrHandler` reaches 0x1A88C through its own
`%hi`/`%lo` pair while holding the struct base in a register, so the struct
ends at 0x1A848.

## sceHSynSeq_t is the MIDI message queue

The sixteen 1032-byte blocks at `sdfSndHsynSeqWork` were typed `sceHSynSeq_t`
on the assumption that a synthesiser entry pointing at them made them the
synthesiser's sequence buffers. Recovering `MODMSIN.IRX` settled what they are:
its `sceMSInBuf_t` is 1032 bytes of `{u32 size; u32 len; u8 data[1024]}`, and it
reaches them through the work area's table, entry by entry.

The driver's own wiring is the proof. `sdfSndInitWorkAreas` hands the same array
to both modules, as `sdfSndHsynEntry[i].seq` and as `sdfSndMidiVoice[i].work`,
and writes 1032 into the first word, which is the buffer's capacity rather than
anything sequence shaped. `sceMSIn_PutMsg` takes the MIDI work area, so the
table the sequencer calls its voice table is that module's buffer table.

So one queue per slot, which `MODMSIN` appends to and the synthesiser drains.
The type is now `sceMSInBuf_t` with `size`, `len` and `data`, the array is
`sdfSndMsInBuf`, and both pointers into it are called `buf`.

## Sony's sound modules, from the retail code

All of these come from a byte-identical build of the function that uses them,
in the `DDS3_IRX/MODHSYN` and `DDS3_IRX/MODMIDI` databases.

Prototypes:

- `sceHsynChannelSetPortamento` is `void(sceHSynChannel_t *, u8)`; a signed second
  parameter gives `sll/sra 24` where retail has `andi 0xFF`.
- `sceHsynUnrefTableSetB`, `sceHsynUnrefTableSetC` and `sceHsynVolumeScale` return `u8`;
  `sceHsynTableEntryFill` returns `int` and masks a `u8` local itself, which is
  why its callers mask again.
- `sceHsynCmdReset` takes `u8 **`: it reads a variable-length value and writes the
  cursor back through the pointer every byte.
- `sceHsynParamSlotResolve` is
  `int(sceHSynWork_t *, int, sceHSynSlot_t **, sceHSynChanSet_t **)`. Its
  second output is `(sceHSynChanSet_t *)&slot->unk14`, so `+4` from there is
  `chan[0]`; `modhsyn_e10`/`e11`/`e12`/`e13`/`e14`/`e20`/`e21` all reach the
  channels that way rather than through the slot.
- `modhsyn_e20`, `modhsyn_e21` take `(sceHSynWork_t *, int)`.
- `sceHsynChannelKeyOffAll` and `sceHsynChannelStopVoices` take
  `sceHSynChannel_t *`.
- `sceHsynLfoStart` takes `sceHSynLfo_t *`, not an env.
- `sceHsynSdSetParamEnvelope` is `void(sceHSynVoice_t *)`.
- `sceHsynEffectDepthSet` is `int(sceHSynLfo_t *)` and `sceHsynEffectFeedbackSet`
  `int(sceHSynVoice_t *)`, the latter working on `voice->lfoB`.
- `sceHsynVoiceSteal` is `int(u16, u8)` and `sceHsynVoiceStealByEnvPhaseEx`
  `int(u16, u8, int, int)`.
- `sceHsynBuildAdsr1`/`sceHsynBuildAdsr2` take the note as `int`: a `u8` parameter
  would be masked before the subtraction and retail copies it as is.
- `sceHsynAdjustFieldByKey`'s shift is `int`, not `s8`; `srav`/`sllv` use it with
  no sign extension.
- `sceHsynChannelSetPan` is `void(sceHSynChannel_t *, u8)` and `sceHsynVolumeScale`'s
  third parameter is `u8 *`.

`sceHSynChannel_t` fields, each settled by a signed load or by a `-1` constant
shared with a byte store:

- `unk26` and `unk28` are `s16` (`lh`, and `addiu -1` rather than `ori 0xFFFF`)
- `unk30` and `pan` (+0x31) are `s8`

## Applied on 2026-10-24: sceHSynEnv_t is the 40-byte LFO

`sceHSynEnv_t`'s `unk00[16]` blob and its trailing `unk20[8]` both hid real
fields, and the names past +0x10 were one slot out. `sceHsynLfoSine` settles the
tail: it reads a `u32` phase at +0x18, a `s16 *` table at +0x20 and a `u16`
count at +0x24, interpolates between two entries and halves the result.
`sceHsynLfoSetupA`/`B` settle the head: they write a byte at +0, a byte at +1,
a halfword at +2, words at +0xC, +0x14, +0x18 and +0x1C, a halfword at +0x10
and the table pair at +0x20/+0x24. The layout is now

    u8 unk00; u8 unk01; u16 unk02; u16 unk04; u16 unk06; u32 unk08; u32 unk0C;
    s16 level; s16 unk12; int (*wave)(sceHSynEnv_t *, int); u32 phase; u32 rate;
    s16 *tbl; u16 nTbl; u16 unk26;

which is `sceHSynLfo_t`'s 20 bytes extended, so the voice's `envA`/`envB` are
LFO state and the 20-byte `lfoA`/`lfoB` are something else. The function names
have since been corrected to match (`sceHsynLfoSetupA`, `sceHsynLfoStep` and the
six waveform evaluators); the two member names are still as the later SDK build
had them, because nothing in the retail code says what `lfoA`/`lfoB` hold.

`wave` at +0x14 is a function pointer, not the `phase` the old names put there:
`sceHsynLfoSetupA` stores one of `sceHsynLfoSawUp`, `sceHsynLfoSawDown`,
`sceHsynLfoTriangle`, `sceHsynLfoSquare`, `sceHsynLfoNoise` or `sceHsynLfoSine` into it, and
`sceHsynLfoStep` calls through it with a second argument saying the phase
wrapped. All six now take `(sceHSynEnv_t *, int)`, since one field cannot hold
two signatures and `sceHsynLfoNoise` is the one that reads the flag.

That also supersedes the earlier record that `sceHsynLfoStart` takes
`sceHSynLfo_t *`. It was only provable while the env's first 16 bytes were an
opaque array; `sceHsynLfoSetupA` passes it the same object it writes +0x24 of,
so it takes `sceHSynEnv_t *`.

### Two thbase import stubs

| Address | Was | Now | Proof |
| --- | --- | --- | --- |
| 0x0000B1DC | `thbase_USecToSysClock` | `thbase_GetSystemTime` | ordinal 34, and `sceHsynSpu2SettleDelay` calls it with one pointer argument, not a microsecond count and a pointer |
| 0x0000B1E4 | `sub_B1E4` | `thbase_SysClock2USec` | ordinal 40, called as `(clock, &sec, &usec)` with the clock the line above produced, and the two outputs differenced against `sceHsynSettleTimeHi`/`Lo` |

`func_0000B1E4` was counted as a real function; it is `jr $ra` plus an import
id, so it never becomes C.

### Prototypes the retail code corrected

| Function | Was | Now |
| --- | --- | --- |
| `modhsyn_e7` | `(s16, u16 spuAddr, u32 iopAddr, u32)` | `(s16 core, u8 *iopAddr, u32 spuAddr, u32 size)`, forwarded positionally to `sceHsynSdVoiceTrans` |
| `sceHsynEffectAttrRead` | `(sceSdEffectAttr_t *)` | `(int sel, u8 mode)`; the attr block is its own stack local |
| `sceHsynEffectAttrWrite` | `(sceSdEffectAttr_t *)` | `(u8 sel, u8 val)`, same |
| `sceHsynChannelSetProgram` | `(sceHSynSlot_t *, ...)` | `(sceHSynChanSet_t *set, ...)`: it reaches `set + 0x384 + idx*28`, and `bank[16]` sits at +0x384 of the chan set, not of the slot |
| `modhsyn_e17` | `(int, int, int)` | `(sceHSynWork_t *, int slotNo, u8 *out)` |
| `modhsyn_e9` | `(u16 *)` | `(sceHSynCore_t *)`, a new 24-byte block `{core, mode, depthL, depthR, delay, feedback, volL, volR}` |
| `sceHsynVoiceKeyOffOne` | `(sceHSynVoice_t *)` | `sceHSynVoice_t *(sceHSynVoice_t *, int keyOff)` |
| `sceHsynPitchBendApply` | `(int, int, u16, int)` | `(sceHSynVoice_t *, int rate, u16 depth, int dir)`, writing `lfoA` |
| `sceHsynVolumeCurveLookup` | fourth argument `int` | `u8 *`, which also made `sceHSynSlot_t::unk10` a `u8 *` |
| `sceHsynVoiceRefresh` | `(int, int, u8 *, unsigned int)` | `(sceHSynSlot_t *, int idx, u8 *p, u8 *end)`; `sceHsynServiceTick` passes the sequence bounds it just compared |
| `sceHsynServiceTick`, `sceHsynChannelAllSoundOff`, `sceHsynChannelStopVoices`, `sceHsynEnvAttackStep`, `sceHsynEnvDecayStep`, `sceHsynSdSetParamVolumeLeft`/`Right`, `sceHsynSdSetParamAdsr`, `sceHsynLfoKeyOff` | `int`/`s32` | `void`: no path sets `$2`, and the two tails leave different values in it |

One conflict the database cannot hold: `sceSmgProgEnt_t`'s +4 byte is read
`lbu` by `sceHsynChannelSetProgram`, which tests it as a key count, and `lb` by
`sceHsynVoiceSetPanBase`, which uses it as a signed transpose. The field stays `u8 nKey`
and `sceHsynVoiceSetPanBase` casts.

## Applied on 2026-10-24: MODHSYN voice allocation and command prototypes

All in the `DDS3_IRX/MODHSYN` database, all proved by a byte-identical build of
the function concerned.

### Functions that return nothing

Fifteen more where no path sets `$2`, so the IDB's `int`/`s32` was a guess. In
most of them the last write to `$v0` is the loop's own `slti` leftover, which is
what Hex-Rays renders as `result = ++i < 2`.

`sceHsynChannelNoteOff`, `sceHsynChannelSetHold`, `sceHsynChannelAllNotesOff`,
`sceHsynChannelKeyOffByPitch`, `sceHsynChannelKeyOffAll`, `sceHsynChannelKeyOffByTag`,
`sceHsynChannelKeyOffByTagEntry`, `sceHsynChannelKeyOffByTagMask`, `sceHsynChannelKeyOffByTagMaskEntry`,
`sceHsynCmdNoteOff`, `sceHsynCmdNoteOn`, `sceHsynCmdProgramChange`,
`sceHsynCmdChannelPressure`, `sceHsynVoiceUpdatePass`, `sceHsynCmdSetParamRamp`.

The two `*Entry` wrappers follow their callees: each is one `jal` and nothing
else, so whatever the callee leaves in `$2` is incidental.

### Prototypes the retail code settled

| Function | Now |
| --- | --- |
| `sceHsynChannelNoteOffMatch` | `void(int base, int unused1, int state, u8 note, u8 ch, int unused2, u32 tag)`; the second and sixth arguments are never read, `base + 0x10` is the list sentinel and `base + 0x18` the channel |
| `sceHsynChannelKeyOffByTag` | `void(int base, u32 tag)` |
| `sceHsynChannelKeyOffByTagMask` | `void(int base, int unused, u32 tag, u32 mask)`; it matches `(v->tag & mask) == (tag & mask)` |
| `sceHsynCmdNoteOff`, `sceHsynCmdNoteOn` | `void(int sel, int base, int unused, int state, u8 note, u8 ch, s16 val, u32 tag)` |
| `sceHsynCmdProgramChange` | `void(int base, int unused, int rate, int state, u8 note, u8 ch, u8 vol, u32 tag)` |
| `sceHsynCmdChannelPressure` | `void(int base, int unused, int dir, int rate, int state, u8 note, u8 ch, u16 depth, u32 tag)` |
| `sceHsynVoiceUpdatePass` | `void(sceHSynSlot_t *slot, int status, char sel, u8 note, u8 vel)` |
| `sceHsynVoiceEventDispatch` | third parameter `u8`, not `char`: its caller passes the note with `andi 0xFF`, where a `char` formal would give `sll`/`sra 24` |
| `sceHsynChannelNoteOn` | third parameter `int`, not `s8`: `sceHsynVoiceUpdatePass` forwards its status byte with a plain `move`, and gcc re-extends for an `s8` formal |
| `sceHsynCmdSetParamRamp` | `void(int base, int unused, u32 a3, int a4, int state, u8 note, u8 ch, u8 a8, s8 a9, int a10, u32 tag)`; it forwards six of those to `sceHsynVoiceSetParamRamp` |
| `sceHsynChannelSetPanDirect`, `sceHsynChannelSetExpressionDirect` | `void(sceHSynChannel_t *chan, u8 ch, u32 tag, int val)` and `void(..., u8 val)`: `sceHsynChannelSetPanDirect` subtracts 0x40 from the raw argument register, so a `u8` formal's mask is one instruction too many, while `sceHsynChannelSetExpressionDirect` does mask |

`sceHsynVoiceStealByVnum` was already `sceHSynVoice_t *(s16, char)` in the database but the
generated header still said `int`; regenerating it was the whole fix.

### Names the code disproved

The bodies were read here and the names corrected in the pass recorded at the
end of this file. What the evidence was:

- `sceHsynVoiceStealByVnum` (named `hsynPanApply` when ported from the later SDK
  build) picks the active voice with the lowest `vnum` across the enabled cores,
  unlinks it and blanks its tag. It is a voice stealer, and so are the two
  copies beside it: `sceHsynVoiceStealByVnumActive` is the same function with an
  extra `unk57 == 0` filter, and `sceHsynVoiceStealByEnvPhase` ranks on
  `flags & 0x300` then `vnum` then `unk12`.
- `sceHsynChannelLimitByPitch` counts the voices on two of a channel's three
  lists whose `pitch` matches, and if that count exceeds its third argument
  walks them again through `sceHsynVoiceKeyOffOne`. It is a voice-count limiter,
  which is why it no longer carries the name `hsynLfoStep`.
- The whole `hsynVoiceAlloc*` family *released* voices rather than allocating
  them. Each walks a channel list, keys the voice off and moves it to the off
  list or hands it back to the free list, so each is now named for the event it
  serves: `sceHsynChannelNoteOff`, `sceHsynChannelSetHold`,
  `sceHsynChannelAllNotesOff`, `sceHsynChannelNoteOffMatch`,
  `sceHsynChannelKeyOffByTag`, `sceHsynChannelKeyOffByTagMask` and
  `sceHsynChannelKeyOffAll`. `sceHsynVoiceStealScan` and
  `sceHsynVoiceStealFromChannel` are the two that really do free a voice for a
  new note, and both keep their names.
- `sceHSynVoice_t`'s `note` (+0x42) and `ch` (+0x43) may be the wrong way round.
  `sceHsynSdSetParamBend` interpolates the velocity split from `vel` against the
  sample entry's `velLow`/`velHigh`, and the *key* split from `ch` against the
  program key's `keyLow`/`keyHigh`, which is the field a key number belongs in.
  Nothing in the retail code settles it, so both names stand.

## Three field mix-ups in SDFSDMAN's C

Found by compiling the driver natively in the sdfplay side project, where the C
runs rather than only being read. All three are in `NON_MATCHING` bodies, so the
byte-identical build never noticed, but anything that compiles the C does.

`sdfSndInitWorkAreas` stored the MIDI slot work area in `sdfSndMidiSeq[i].data`
and cleared `.slot`. Retail does the opposite, and the folded base offset is why
it reads the wrong way round at a glance: the loop's induction variable starts
at 8, so `sw $7, %lo(sdfSndMidiSeq + 0x4)($1)` lands on `seq[i] + 0xC`, and the
two stores through `sdfSndMidiTbl + 0x8` and `+ 0xc` land on `seq[i] + 0x0` and
`+ 0x4`. Swapped, every `seq[i].slot` is NULL and the sequence thread faults on
its first tick. Fixed.

`sdfSndUnloadBank` zeroed `banks[kind].spuAddr`. Retail zeroes `+0x14` off
`workArea + kind*16`, and with `banks` at `workArea + 0x10` that is
`banks[kind].waveSize`. Zeroing `spuAddr` instead sends the next bank's wave
upload to SPU2 address 0. Fixed.

`sdfSndLoadSmg` writes `stat[bank].kind` as one `u16`. Retail does two byte
stores, `sb` at `stat[kind] + 4` and `+ 5`, from two separate `lbu`s of the
header's `kindPacked` word. So the field is two `u8`s rather than a halfword.
Left alone: the evidence says the shape but not what the second byte means, and
the bank selection uses `kindPacked >> 24`, which is neither of them.

## Two swapped fields in MODHSYN's C

`sceSmgSmplEnt_t::pitch` (+0xE) and `voiceOfs` (+0xF) were the wrong way round
in `sceHsynChannelNoteOn` and `sceHsynVoiceStartPathA`, in all three places each
function uses them. Both bodies are `NON_MATCHING`, so the byte-identical build
never saw it; the sdfplay side project, which compiles the C and plays a bank,
did.

Retail settles it at the instruction level. In `sceHsynChannelNoteOn`,
`lbu $v0, 0xE($s2)` at 0x67FC is the value tested and passed to
`sceHsynChannelKeyOffByPitch`, and `lbu $a1, 0xF($s2)` at 0x6814 is what
`slot->voiceBase` is added to for the voice number. `sceHsynVoiceStartPathA`
has the same pair. The voice's own `pitch`, which
`sceHsynChannelKeyOffByPitch` matches on, therefore comes from +0xE.

Swapped, every note of a chord that resolves to one sample shares a kill group,
so each note-on freed the chord's earlier notes before the next synthesiser tick
could key them on. A ten second render of a bank keyed 66 of 198 note-ons; with
the fields the right way round it keys all 198, and the playing voice count goes
from three to nine. `sceHsynVoiceStartPathB` was not affected: it reaches the
same two bytes as `ent[7]` and `ent[9]` off an untyped pointer.

## Applied on 2026-10-24: MODMIDI and MODHSYN prototypes, and three retail bugs

All in the `DDS3_IRX/MODMIDI` and `DDS3_IRX/MODHSYN` databases, all found while
writing the C for Sony's sequencer. The byte-identical build of the function
concerned is the proof except where noted.

### Functions that return nothing

Five more where no path sets `$2` and every caller discards the result, so the
database's `int` was a guess:

| Function | Caller evidence |
| --- | --- |
| `sceMidiSendChMsg` | `$2` ends as `trk->chActive | bit` on one exit and the loop mask on another; `sceMidiSongStopNotes` and `sceMidiTrackServiceTick` discard it |
| `sceMidiParseExcEvent` | `$2` ends as the advanced `trk->cur`; `sceMidiPlayEnv` discards it |
| `sceMidiApplyPlayState` | `$2` is the loop's own `sltiu` leftover; `modmidi_sceMidi_MidiPlaySwitch` discards it |
| `sceMidiPlaySong` | `sceMidiSongServiceTick` discards it |
| `sceMidiScanLoops` | `$2` is `0xFF` on the reset path and stale otherwise; `sceMidiPlayEnv` discards it |
| `sceHsynCmdVoiceAlloc` | the epilogue is a bare `jr $31`; `sceHsynCmdDispatch` discards it |

### Prototypes the retail code settled

| Function | Was | Now |
| --- | --- | --- |
| `sceMidiScanLoops` | sixth parameter `int allowCb` | `sceMidiVoiceTbl_t *vt`: `sceMidiPlayEnv` passes its own `vt` through, and the body only tests it against NULL |
| `sceHsynCmdVoiceAlloc` | fourth parameter `u8 a4` | `int a4`: it is compared against `voice->unk58` with no mask, where a `u8` formal costs an `andi 0xFF` |

`sceMidiTbl_t` is really two `{count, pointer}` segments rather than one
sixteen-byte block. `modmidi_sceMidi_Init` steps a single pointer from the
sequence segment to the buffer segment with `addiu $17, $17, 8` and reads both
words through the new base, which is only reproducible by advancing one
pointer. The type is left alone because every other function reaches the
second pair as `&tbl->nVoiceWord` and matches that way; `sceMidi_Init` uses an
untyped `u32 *seg` instead.

### Three retail bugs, reproduced as they are

- `modmidi_sceMidi_Unload` passes the *unit* index to
  `modmidi_sceMidi_MidiPlaySwitch`, which derives the unit from a slot number
  itself. The stop therefore targets slot `2n+1` rather than `n`, so unloading
  a playing slot never stops it.
- `sceMidiParseExcEvent`'s buffer walk skips four mask bits when the low nibble is
  *non-zero*; `sceMidiSendChMsg`, which is otherwise the same loop, skips them
  when the nibble is empty. The exclusive-event path can therefore step over a
  buffer that is enabled.
- `sceMidiScanLoops`'s loop-start arm falls back to an inline scan for a free
  table entry, and when the table is full the cursor runs one past the end
  instead of becoming NULL. The `aLoopTableOverf` complaint beside it is
  therefore unreachable and a full table is written past.

### Two more MODHSYN corrections

| What | Was | Now | Proof |
| --- | --- | --- | --- |
| `sceHSynVoice_t::unk1E` | `u16` | `s16` | `sceHsynVoiceSetPan` reads it with `lh` and negates it when it comes out below zero; a `u16` field makes gcc emit `lhu`. Nothing else in the module loads the field, so the change is contained |
| `sceHsynVoiceRefreshHook` | `void(void)` | `void(sceHSynSlot_t *slot, u8 *data, int len)` | the body is `jr $ra` and settles nothing, but `sceHsynVoiceRefresh` sets up all three argument registers before the call: the slot, the start of the system-exclusive run and the byte count it just scanned |

## Applied on 2026-10-25: the last of MODHSYN

All in the `DDS3_IRX/MODHSYN` database, found while finishing the module's
remaining 25 functions. A byte-identical build of the function concerned is the
proof except where noted.

### Member types

| Type | Member | Was | Now | Proof |
| --- | --- | --- | --- | --- |
| `sceHSynVoice_t` | `unk28` (+0x28) | `u16` | `s16` | `sceHsynInitCores` stores -1 there; a `u16` field makes gcc build the constant with `ori 0xFFFF` where retail has `addiu -1` |
| `sceHSynVoice_t` | `vol` (+0x56) | `u8` | `s8` | the same function stores -0x80; a `u8` field makes gcc emit `addiu 0x80` |

Both fields are read *both* ways by `sceHsynVoiceUpdatePathA`, which has `lb` and
`lbu` of +0x56 and `lh` and `lhu` of +0x28 within twenty instructions. The
store settles the declared type and the unsigned reads take a cast, the same
split already recorded for `sceSmgProgEnt_t`'s +4.

### Functions that return nothing

Six more where no path sets `$2` and the callers discard the result:
`sceHsynVoicePitchCommit`, `sceHsynVoiceVolumeCommit`, `sceHsynVoiceUpdatePathA`,
`sceHsynVoiceUpdatePathB`, `sceHsynVoiceKeyOnPathA`, `sceHsynVoiceKeyOnPathB`.

`sceHsynVoiceKeyOff` is `int(void)`, not `int(sceHSynVoice_t *)`: it walks both
cores' lists itself and never reads `$4`.

### Prototypes the retail code settled

| Function | Was | Now |
| --- | --- | --- |
| `sceHsynVoiceDirectPass` | `int(int, int, u8, u8 *)` | `void(sceHSynSlot_t *slot, int status, u8 sel, u8 *p)`; it reaches `slot + 0x18 + idx*56`, which is `slot->chan[idx]` |
| `sceHsynVoiceStealByEnvPhaseEx` | `int(u16, u8, int, int)` | `sceHSynVoice_t *(u16, u8, int, int)`; it is the third copy of the `sceHsynVoiceStealByEnvPhase` voice stealer and returns the voice it took |
| `sceHsynLfoStep` | `int(char *)` | `int(sceHSynEnv_t *env)`; it steps the env and calls through `env->wave` |
| `sceHsynLfoSetupA` | `int(int, ...)` | `void(sceHSynEnv_t *env, ...)`, matching `sceHsynLfoSetupB` |
| `sceHsynLfoSetupB` | ninth argument unnamed | `void(sceHSynEnv_t *, int prog, u8 *seg, int n, char sel, u16 period, u16 depth, unsigned int tick, int which)` |
| `sceHsynVoiceEventDispatch` | second parameter `u8 *` | `sceHSynChannel_t *`; every arm is a channel-field write or a `sceHSynChannel_t *` call |

### The nine-argument forwarder and its two callees

`sceHsynVoiceStart` is `int(sceHSynSlot_t *slot, int a2, u32 a3, u8 a4, u8 a5, u8 a6, int a7, u16 a8, u32 a9)`:
it reads `slot->path` and forwards to `sceHsynVoiceStartPathA` for 0, `sceHsynVoiceStartPathB`
for 1, and returns -1 otherwise. The retail code pins the two callees' narrow
parameters by which conversions the forwarder emits:

| Parameter | Forwarder | `sceHsynVoiceStartPathA` | `sceHsynVoiceStartPathB` |
| --- | --- | --- | --- |
| a2 | `int` | `int` | `int` |
| a5 | `u8` | `u8` (passed with no conversion) | `int` (passed with `andi 0xFF`) |
| a6 | `u8` | `int` | `int` |
| a7 | `int` | `s8` (`sll`/`sra 24` at the call) | `s8` |
| a8 | `u16` | `int` (`andi 0xFFFF`) | `int` |

A conversion appears only where the two types differ, so each `andi` or
`sll`/`sra` in the forwarder names one callee formal. `a2` must be `int` in all
three: `char` to `char` would be re-extended, which is three instructions the
forwarder does not have.

### Names the code disproved

Read here, renamed in the pass recorded at the end of this file:

- `sceHsynVoiceStartPathA`, `sceHsynVoiceStartPathB` and `sceHsynChannelNoteOn` are one family:
  walk the program's key list, the sample set and the sample entries, resolve a
  VAG, free a voice by one of four escalating strategies, then fill the voice.
  `sceHsynVoiceStartPathA` and `sceHsynVoiceStartPathB` set `unk57 = 1` and bump `sceHsynRelCount`
  where `sceHsynChannelNoteOn` sets 0 and bumps `sceHsynActiveCount`, so the first
  two start a release-tail voice and the third a plain one.
- `sceHsynLfoSetupA` and `B` are not interpreters, which is what
  `hsynSeqInterpretA`/`B` called them. They install one of six
  waveform readers in `sceHSynEnv_t::wave`, convert a period and a depth into
  `unk02` and `unk0C` through `sceHsynTickScale`, and seed the phase. They differ
  only in which four bytes of `sceSmgProgEnt_t` they read, B's being A's plus 4.
- `sceHsynCmdDispatch` is the system-exclusive dispatcher: it resyncs on 0xFE, reads
  a 32-bit tag and a command byte, and routes the 0x90/0xA0 nibbles to
  `sceHsynVoiceStart`, 0xB0 through a 35-entry table of channel commands, and 0xC0
  to a smaller table of voice-allocation commands.
- `sceHsynVoiceSetParamRamp` is a parameter-ramp setter writing a 14-word block at
  `sceHSynVoice_t::param`, so `param[56]` is a struct of fourteen `int`s.
- `sceHsynUpdateVoiceState` (`func_00004D90` until it was read) returns the
  number of voices it serviced, which `sceHsynServiceAll` stores in
  `sceHSynStat_t`'s word at +0; `sceHsynServiceVoices` returns the number it
  *skipped*, stored at +4. The two field names were therefore backwards. They
  are swapped in the pass recorded at the end of this file.

## Applied on 2026-10-25: Sony's three sound modules renamed

Two changes in one pass, in the `DDS3_IRX/MODHSYN`, `DDS3_IRX/MODMIDI` and
`DDS3_IRX/MODMSIN` databases. All six IRX modules stay byte-identical, which is
what proves a rename touched only symbols.

### The module prefix

210 symbols gained Sony's `sce`: `hsyn*` to `sceHsyn*`, `midi*` to `sceMidi*`,
`msin*` to `sceMsin*`, at the start of a name only. The export-ordinal names
(`modhsyn_eNN`), the `modhsyn_sceHSyn_*` import stubs, the foreign stubs
(`libsd_*`, `intrman_*`, `thbase_*`), the `D_<address>` tables, `*LibraryTable`
and the `sce*_t` type names were left as they were.

### The names the code disproved

Each was read before it was renamed. The dispatcher at
`sceHsynVoiceEventDispatch` is what settles most of the channel half: its switch
is on a MIDI controller number, so each arm names the function it calls.

| Was | Now | What the code does |
| --- | --- | --- |
| `hsynVoiceParamReset` | `sceHsynChannelSetProgram` | resolves `chan->prog` and `chan->bank` from a program number |
| `hsynVoiceSetPitch` | `sceHsynChannelSetPitchBend` | 14-bit value less 0x2000 into `chan->unk26`, broadcast to the channel's voices |
| `hsynVoiceEnvelopeApply` | `sceHsynChannelApplyVolume` | recomputes every voice's `unk18` through `sceHsynSdSetParamEnvelope` |
| `hsynVoiceSetAttack` | `sceHsynChannelSetVolume` | controller 7, writes `chan->vol` |
| `hsynVoiceSetRelease` | `sceHsynChannelSetExpression` | controller 11, writes `chan->expression`, the second volume scale |
| `hsynVoiceFlagsGet` | `sceHsynChannelSetBendRange` | controller 5, writes `chan->bendRange = 20 * semitones` |
| `hsynVoiceFlagsSet` | `sceHsynChannelSetPortamento` | controller 65, arms `chan->portaNote` at 0x80 or disarms it at 0xFF |
| `hsynVoiceIndexGet` | `sceHsynChannelSetPortamentoNote` | controller 84; it stores, where the name said it loaded |
| `func_00000C58` | `sceHsynChannelSetPan` | controller 10, writes `chan->pan` (+0x31) and broadcasts |
| `func_00000D24` | `sceHsynChannelSetPanDirect` | the same, filtered by tag and channel |
| `func_00000E40` | `sceHsynChannelSetExpressionDirect` | expression, filtered by tag and channel |
| `func_00004AE4` | `sceHsynVoiceSetPan` | turns a signed pan into the voice's two volume scales |
| `hsynVoiceApplyAll` | `sceHsynChannelResetControllers` | controller 121: every controller back to its default |
| `hsynVoiceAllocScan` | `sceHsynChannelNoteOff` | what `sceHsynVoiceUpdatePass` runs for status 0x80 |
| `hsynVoiceAllocByPriority` | `sceHsynChannelSetHold` | controller 64; zero releases everything parked on the sustain list |
| `hsynVoiceAllocByChannel` | `sceHsynChannelAllNotesOff` | controller 123 |
| `hsynVoiceStop` | `sceHsynChannelAllSoundOff` | controller 120 |
| `hsynVoiceAllocForCmd` | `sceHsynChannelNoteOffMatch` | keys off on tag, state, note and channel together |
| `hsynVoiceAllocExport20` | `sceHsynChannelKeyOffAll` | |
| `hsynVoiceAllocPathB`, `...Entry` | `sceHsynChannelKeyOffByTag`, `...Entry` | |
| `hsynVoiceAllocPathC`, `...Entry` | `sceHsynChannelKeyOffByTagMask`, `...Entry` | |
| `hsynVoiceReleaseExport21` | `sceHsynChannelStopVoices` | |
| `hsynVoiceRetrigger` | `sceHsynChannelKeyOffByPitch` | |
| `hsynLfoStep` | `sceHsynChannelLimitByPitch` | |
| `hsynLfoApply` | `sceHsynVoiceKeyOffOne` | keys off or frees one voice and returns the next on the list |
| `hsynVoiceEngineStep` | `sceHsynChannelNoteOn` | what `sceHsynVoiceUpdatePass` runs for status 0x90 |
| `hsynVoiceMaskClear`, `...One` | `sceHsynVoiceAlloc`, `sceHsynVoiceAllocEntry` | pops a voice off a core's free list; this is the allocator |
| `hsynVoiceMaskUpdate`, `hsynVoiceMaskSet` | `sceHsynVoiceFree`, `sceHsynVoiceFreeEntry` | |
| `hsynVoiceRelease`, `hsynVoiceReleaseOne` | `sceHsynVoiceChFree`, `sceHsynVoiceChFreeEntry` | the same, for a voice still on a channel list |
| `hsynVoiceMaskGet` | `sceHsynVoiceUnlink` | |
| `hsynVoiceSlotGet`, `hsynVoiceSlotSet` | `sceHsynVoiceChLink`, `sceHsynVoiceChUnlink` | |
| `hsynCoreIndexGet`, `hsynCoreIndexSet` | `sceHsynVoiceInsertAfter`, `sceHsynVoiceInsertBefore` | |
| `hsynPanApply` | `sceHsynVoiceStealByVnum` | |
| `hsynVoiceVolumeAdvance` | `sceHsynVoiceStealByVnumActive` | |
| `hsynVoiceEnvelopeAdvance` | `sceHsynVoiceStealByEnvPhase` | |
| `hsynModulationApply` | `sceHsynVoiceStealByEnvPhaseEx` | the same ranking with two more filters |
| `hsynModulationStep` | `sceHsynVoiceSteal` | decides those two filters from the release budget |
| `hsynVoiceReleaseGroup` | `sceHsynVoiceStealFromChannel` | |
| `hsynSeqByteRead` | `sceHsynLfoSawUp` | `(phase - 0x8000) >> 1` |
| `hsynSeqWordRead` | `sceHsynLfoSawDown` | the same, inverted |
| `hsynSeqVarLenRead` | `sceHsynLfoTriangle` | folds the middle half |
| `hsynSeqSkip` | `sceHsynLfoSquare` | plus or minus 0x4000 |
| `hsynSeqSeek` | `sceHsynLfoSine` | interpolates two entries of `env->tbl` |
| `hsynSeqEmit` | `sceHsynLfoNoise` | holds `env->level`, resamples on wrap |
| `hsynSeqEmitTail` | `sceHsynRand` | the module's linear congruential generator |
| `hsynEffectDispatch` | `sceHsynLfoStep` | advances the phase and calls through `env->wave` |
| `hsynSeqStateReset` | `sceHsynLfoStart` | |
| `hsynSeqStart` | `sceHsynLfoKeyOff` | |
| `hsynSeqInterpretA`, `B` | `sceHsynLfoSetupA`, `B` | |
| `hsynSdLockAcquire` | `sceHsynSdLogNext` | takes the next entry of a ring buffer |
| `hsynSdLockRelease` | `sceHsynSdLogCall` | records the libsd call and its five arguments |
| `hsynSdLockWait` | `sceHsynSdLogResult` | records the return value and clears the busy bit |
| `hsynLoadBank` | `sceHsynTagMatch` | compares two four-character chunk tags |
| `midiParseHeader` | `sceMidiTagMatch` | the same comparator with a debug print |
| `midiOutputPush` | `sceMidiVoiceLookup` | a bounds-checked index into the voice table |
| `midiLoopTableAdd` | `sceMidiLoopFind` | finds the entry whose `id` matches, over eight |
| `midiSongSetTempo` | `sceMidiSongSetVolume` | clamps to 0..128 and writes the song volume, which `sceMidiSongApplyVolume` fades |
| `midiSongSetFlags` | `sceMidiSongSetBpm` | clamps to 10..255 and writes the tempo, 120 by default |
| `func_00000050` | `modhsyn_sceHSyn_Init` | MODHSYN export 4: sets the tick scale from the interval and validates the work table |
| `func_00004D90` | `sceHsynUpdateVoiceState` | polls each voice's SPU2 envelope and advances its state |
| `func_0000273C` | `sceHsynCmdDispatch` | the system-exclusive dispatcher |
| `func_0000AB90` | `sceHsynVoiceSetParamRamp` | writes the 14-word ramp block at `sceHSynVoice_t::param` |
| `func_000078D4`, `func_00007F54` | `sceHsynVoiceStartPathA`, `PathB` | the two note-on paths `slot->path` selects between |
| `func_00008350` | `sceHsynVoiceStart` | the nine-argument forwarder that selects them |

Controllers 65 and 84 are what settle the byte at +0x33, which the earlier
record left as a `priority` fitting neither name. It is the portamento note, and
is now called `portaNote`. Three member renames came out of this pass and are
recorded below.

## Applied on 2026-10-25: three member corrections in MODHSYN

A function rename touches only symbols; a member rename reaches the C that uses
the member, so these were deferred from the naming pass above to a pass that
could carry `src/irx/MODHSYN.c` with them. All six IRX modules stay
byte-identical, which is what proves the renames are names only.

| Type | Was | Now | Proof |
| --- | --- | --- | --- |
| `sceHSynChannel_t` | `pan` (+0x2C) | `expression` | controller 11 writes it, and `sceHsynSdSetParamEnvelope` is `voice->unk18 = (chan->vol * voice->unk54 * voice->unk16) >> 14` with `unk54` copied from it. It is a volume scale beside `vol`, not a position |
| `sceHSynChannel_t` | `unk31` (+0x31) | `pan` | controller 10 writes it and broadcasts it through `sceHsynVoiceSetPan`, which turns it into the voice's two volume scalers `unk4C` and `unk4D` |
| `sceHSynChannel_t` | `priority` (+0x33) | `portaNote` | `sceHsynChannelSetPortamento` arms it at 0x80 and disarms it at 0xFF, `sceHsynChannelNoteOff` opens `if (chan->portaNote < 0x81) chan->portaNote = note`, and `sceHsynVoiceStateClear` computes `(v->chan->portaNote - v->ch) << 7` as the glide delta |
| `sceHSynStat_t` | `nActive` (+0), `nFree` (+4) | `nFree`, `nActive` | `sub_17EC` stores `sceHsynServiceVoices`' count of voices it *declined* to key on at +0 and `sceHsynUpdateVoiceState`'s count of voices it serviced at +4. The playing count is the second, so the pair was the wrong way round |

`chan->expression` and `chan->vol` are the channel's two volume scales and
multiply together in `sceHsynVoiceSetParamRamp`; the pan is what
`sceHsynVoiceSetPan` splits into left and right. Controller 84
(`sceHsynChannelSetPortamentoNote`) writes `unk34`, a second copy of the glide
source that nothing in the module reads, so `unk34` is left as it is.

## Applied on 2026-10-25: the last fourteen `func_` names in Sony's modules

Twelve in `DDS3_IRX/MODHSYN` and two in `DDS3_IRX/MODMIDI`, the only functions
in the six modules still named for their address. All six modules stay
byte-identical.

| Address | Now | What the code does |
| --- | --- | --- |
| `MODHSYN` 0x17EC | `sceHsynServiceAll` | export ordinal 5, the per-tick service `sdfSndSeqThread` calls: clears the key-on settle latch, rebuilds both cores' mix masks from `sceHsynSdGetSwitch`, then runs `sceHsynServiceTick`, `sceHsynServiceVoices` and `sceHsynUpdateVoiceState` and publishes their counts in `sceHsynStat` |
| `MODHSYN` 0x21B8 | `sceHsynCmdSetParamRamp` | the `sceHsynCmd*` sibling for system-exclusive commands 0..5 and 12..13: walks both channel lists and calls `sceHsynVoiceSetParamRamp` on every voice matching tag, state, note and channel |
| `MODHSYN` 0x4614 | `sceHsynVoiceVolumeCommit` | folds `unk18`, `lfoB`, the slot volume, `envB.level` and the two pan scalers into a left and a right level, caches them in `unk1A`/`unk1C` and writes the voice's two SPU2 volume registers |
| `MODHSYN` 0x5AA0 | `sceHsynUnrefEnvDecayStep` | unreferenced third copy of `sceHsynEnvDecayStep`, identical but for a `u16` scale |
| `MODHSYN` 0x5B4C | `sceHsynVoiceSetPanBase` | the path-B counterpart of `sceHsynSdSetParamAdsr`: derives `unk1E`, the pan `sceHsynVoiceSetPan` offsets by, and the `unk53` polarity from the program entry's signed +4 byte |
| `MODHSYN` 0x8BB0 | `sceHsynCurveSpanLinear` | `sceHsynCurveEval2[6]`: `((from - to) << 16) / n`, where `n` is the steps left to the end of the range rather than the fixed 126 of entries 0..5 |
| `MODHSYN` 0x8C1C | `sceHsynCurveSpanLinearInv` | entry 7, the same with the subtraction the other way round |
| `MODHSYN` 0x8C88 | `sceHsynCurveSpanSquare` | entry 8, that slope squared, signed by direction |
| `MODHSYN` 0x8D54 | `sceHsynCurveSpanSquareInv` | entry 9, the `0x10000 - t * t / 16384` form of it |
| `MODHSYN` 0xA810 | `sceHsynVoiceSetVolumeRamp` | arms `lfoB` as a volume ramp: a period from the rate and `sceHsynTickScale`, a target from the volume byte scaled by the voice curve and `chan->vol * chan->expression`, and the per-tick step between them |
| `MODHSYN` 0xA930 | `sceHsynPanSplit` | writes a signed pan as the left and right volume scaler pair, the same clamp and wrap `sceHsynVoiceSetPan` does but to a caller-supplied pair |
| `MODHSYN` 0xA9FC | `sceHsynVoiceParamRampStep` | steps the 14-word ramp block `sceHsynVoiceSetParamRamp` wrote: advances the counter, spends the repeat count, wraps the value, ends the ramp with state 2 and pushes the result through `sceHsynPanSplit` |
| `MODMIDI` 0x1FE4 | `sceMidiSendChannelVolume` | sends controller 7 for one channel with the volume `sceMidiChannelReset` computes from `chVol`, `chExpr` and the track volume |
| `MODMIDI` 0x31DC | `sceMidiSongSetMasterVolume` | export ordinal 9: writes `trk->songVol`, the scale `sceMidiSongApplyVolume` multiplies over the faded song volume, and re-applies it |

Two names the pass read but did not change, both outside its scope:
`sceHsynSdSetParamAdsr` sets no ADSR, it is the path-A `sceHsynVoiceSetPanBase`,
and `sceMidiChannelReset` resets nothing, it returns a channel's effective
volume.

## MODHSYN's stdio import was named for the wrong library

`DDS3_IRX/MODHSYN` 0xB1B8 was `thbase_DelayThread`; it is `stdio_printf`.

The import stub sits inside the `stdio` table, whose header is at 0xB1A4 and
whose zero terminator is at 0xB1C0, and it is that table's only entry. Its
ordinal is 4, which every other module in the build already imports as
`stdio_printf`. MODHSYN's `thbase` table starts after it at 0xB1C8 and imports
only ordinals 0x22 and 0x28, so it has no `DelayThread` at all.

The C agreed: the one call site passed a string, `thbase_DelayThread((u32)aSesqUnknownCom)`,
and needed the cast only because the prototype was wrong. It is now
`stdio_printf(aSesqUnknownCom)`, which is how the other modules call it, and
MODHSYN still builds byte-identical.

Found by cross-checking every import stub's symbol against the import table
that contains it, which `IRX_IMPORT` made possible.

## loadcore ordinal 6 is RegisterLibraryEntries, not Release

`DDS3_IRX/MODHSYN` 0xB194, `DDS3_IRX/MODMIDI` 0x3790 and `DDS3_IRX/MODMSIN`
0x390 were all `loadcore_ReleaseLibraryEntries`. All three are
`loadcore_RegisterLibraryEntries`.

The build contradicted itself: `SDFIOPFL` imported loadcore ordinal 6 as
`RegisterLibraryEntries` while the three Sony modules imported the same ordinal
of the same library as `ReleaseLibraryEntries`. The retail code settles it.
Ordinal 6 is each module's only loadcore import, and `SDFSDMAN` imports the
library each of them publishes, so each one must register its entries at load.
A module that only ever released them could never be imported at all.
`SDFSDMAN` separately imports ordinal 5 as `FlushDcache`, which leaves 7 for
`ReleaseLibraryEntries`.

`iop.h` had both names declared with the same signature; the Release
declaration is gone, since nothing in the build imports ordinal 7.

Found the same way as the stdio correction above, by checking each import
stub's name against its ordinal once `IRX_IMPORT` made the ordinal visible.

## Applied on 2026-10-25: LIBSD's volatile globals, and two types

Writing C for all 60 of LIBSD's functions settled twelve things in the
`SOUNDIRX/LIBSD` database. Every one of them is proved by a byte-identical
build, and all seven IRX modules still verify.

### Ten globals are `volatile`

`sceSdTransIdle`, `sceSdBlockHalf`, `sceSdBlockSize`, `sceSdSpdifMode`,
`sceSdTransCallback`, `sceSdTransIntrHandler`, `sceSdCleanStepCallback`,
`sceSdCleanChunk`, `sceSdCleanChunkIndex` and `sceSdCleanChunkCount` were all
plain. Every one of them is shared with the DMA or IRQ handler, and retail
reloads each one after writing it, where gcc's CSE keeps the stored register.
`sceSdCleanEffectStep` is eleven instructions short without the qualifier;
`sceSdSetSpdifMode`, `libsd_sceSdSetTransCallback` and `libsd_sceSdQuit` are
each one or three short.

`tools/irx_headers.py` carries the qualifier out of the database on its own, so
the fix was the type and a regenerate. Three agents reached the same conclusion
independently from three different parts of the module, which is what made it
worth applying rather than working around in the C.

`sceSdTransEventFlag` is deliberately *not* volatile: its store-then-reload
matches through a register-held pointer at no cost.

### `sceSdTransferOp` takes no declared parameters

It was `s32 sceSdTransferOp(s32 op, u32 addr, u32 size)`. The op word says how
many arguments are really passed, and retail calls it with one
(`libsd_sceSdQuit`, `libsd_sceSdStopTrans`, both effect-clear functions), two
(`sceSdCleanEffectStep`) and three. Only an unprototyped `s32 sceSdTransferOp()`
allows that, the same deliberate `()` the database already had for
`sceSdDelayUnit`. Its definition in `LIBSD.c` is old-style for the same reason:
a prototyped definition would reject the one-argument calls that follow it.

### `sceSdTransIntrArg_t.arg` is a pointer

It was `u32`. `libsd_sceSdSetTransIntrHandler` stores the caller's argument
there and `libsd_sceSdGetTransIntrHandlerArgument` returns it as a pointer, so
`void *` is what it holds. Codegen is unchanged; the C is honest instead.

## sceSdBlockTrans reads a fifth argument, but four is the right declaration

`iop.h` declares `libsd_sceSdBlockTrans(s16 chan, u16 mode, u8 *iopaddr, u32 size)`
and that stays, but the retail function does read a fifth argument.

It is easy to miss and I got this wrong once by grepping for a literal
`0x58($29)`, which is not there. The evidence is the prologue and a computed
load. `sw $7, 0xC($29)` is the *first* instruction, before `addiu $29, $29, -0x48`:
that spills the fourth register argument into the caller's incoming slot, which
is gcc 2.8's `va_start` making the named arguments addressable. The fifth is then
read with `addiu $2, $29, 0x5B` / `addiu $3, $0, -0x4` / `and $2, $2, $3` /
`lw $2, 0x0($2)`, where `0x5B & ~3` is `0x58`, which is caller_sp+0x10. It is
passed on as `sceSdBlockTransWrite`'s resume position.

So a search for a literal incoming-argument offset proves nothing when the
address is computed; `va_arg` always computes it.

Four parameters is still the correct declaration for this game: MODHSYN's only
call site passes four and matches byte-identically. The faithful spelling would
be four plus an ellipsis, which builds byte-identical but then disagrees with
the four-parameter definition in `LIBSD.c` and fails
`tools/check_irx_nonmatching.py`. Writing the body against `va_list` would settle
it; nothing in this game needs it.

## nbMaeState and nbMaeCounter are MIDI numbers

`nbMaeState` -> `nbMaeMidiNo`, `nbMaeCounter` -> `nbMaeSubMidiNo`, and the
unnamed `dword_551540` -> `nbMaeLoadDone`.

Both of the old names say the wrong kind of thing. The two variables are only
ever written with a packed MIDI number and only ever read as the argument to
`p2sdLoadCheckMidi` and `p2sdLoadSMGFile`. `nbSetMaeFlag` case 6 writes
`0x280000` to the first and `0x290000` to the second, which are MIDI0028 and
MIDI0029; the fifteen cases cover the battle themes, each a pair. Neither holds
a state and neither counts anything.

The proof is contained: both symbols are referenced only from `nbSetMaeFlag`
and `nbMaeCheckDone`, 8 and 7 times, with no other reader in the image.

What the pair is for is worth recording, because it is not visible from either
name. A battle keeps two banks resident at once. `nbSetMaeFlag` loads
`nbMaeMidiNo` and `nbMaeCheckDone` then loads `nbMaeSubMidiNo` in its state 3
and waits for both, so the sequence in the first bank can reach the samples in
the second through a bank select. MIDI0028's sequence opens with exactly that:
controller 0, value 0x80.

`nbMaeFlag` is left alone. It is the state machine's own variable, taking 0, 1,
3, 4 and 5, so "flag" is loose rather than wrong, and `nbSetMaeFlag` anchors
the spelling.

## Applied on 2026-10-28: six libgcc helpers named from their archive object

`config/smt3/sdk_objects.tsv` says which archive object each function in the SDK
region was compiled into, and the `bytes` rule is strong evidence: same length
and the same instruction words once jump targets and immediate low halves are
cleared. Where such an object exports exactly one symbol, the object names the
function, because libgcc's object file name *is* its symbol: `fp-bit.c` and
`libgcc2.c` are compiled once per entry point with `-DL_<name>` into `_<name>.o`.

| Address | Old | New | Object | Prototype agrees |
| --- | --- | --- | --- | --- |
| 0x002D34A0 | `libc_2D34A0` | `__pack_df` | `_pack_df.o` | `(int *) -> u64`, called by `dptoul` with an `fp_number_type_df *` |
| 0x002D3638 | `libc_2D3638` | `__fpcmp_parts_df` | `_fpcmp_parts_df.o` | `(int *, int *) -> s64` |
| 0x002E59B8 | `libc_2E59B8` | `__moddi3` | `_moddi3.o` | `(s64, s64) -> s64` |
| 0x002E65C8 | `libc_2E65C8` | `__umoddi3` | `_umoddi3.o` | `(u64, u64) -> u64` |
| 0x002E6B30 | `libc_2E6B30` | `__fixunsdfdi` | `_fixunsdfdi.o` | `(double) -> u64` |
| 0x002E6D88 | `libc_2E6D88` | `__floatdidf` | `_floatdidf.o` | `(s64) -> double` |

The original audit left additional names unresolved for `_div_df.o`,
`_df_to_usi.o`, `_addsub_df.o` and `__main.o` because each was attributed
more than one function. In particular, the former `dpdiv` at `0x002D1B38`
and `libc_2E6C48` were both attributed to `_div_df.o`. The comparator's
name-only attribution was later withdrawn; the actual division provider is
resolved in [Double-division runtime name](#double-division-runtime-name).
`_addsub_df.o` genuinely exports two symbols, `dpadd` and `dpsub`, which
already retain their names.

The original audit kept Sony's `dp*` spellings where they were. The `dpdiv`
conclusion below was name-only and is superseded by the native comparison proof
in [Double-comparison runtime name](#double-comparison-runtime-name). `dptoul` is attributed by the `name`
rule, which means the recovered name is itself a symbol of `_df_to_usi.o`: the
EE libgcc exports Sony's aliases for the double arithmetic and conversions, so
`dpadd`, `dpsub`, `dpmul`, `dptoli` and `dptoul` keep their existing names.
The former `dpdiv` name-only inference is corrected below. The six above are
fp-bit and libgcc2 helpers that Sony did not alias.

The same evidence fixed four record names in the C, which needed no IDB change:
`frame-dwarf2.o` makes `AtExitEntry` that file's `struct object` and `AtExitList`
its `fde_vector`, and `_df_to_usi.o` makes the soft-float pair `fp-bit.c`'s
`fp_number_type`.

## Applied on 2026-10-28: libgcc's ctor/dtor and EH frame registration

`frame-dwarf2.o` accounts for fourteen functions in the region, so the object
cannot name any one of them; these came from the bodies instead, each of which
is gcc's own definition verbatim.

| Address | Old | New | Proof |
| --- | --- | --- | --- |
| 0x002D13C8 | `runAtExitList` | `__do_global_dtors` | walks a null-terminated function table, then under a one-shot `completed` guard calls `__deregister_frame_info(__EH_FRAME_BEGIN__)` |
| 0x002D1458 | `initAtExit` | `__do_global_ctors` | calls `__register_frame_info(EH frame, &object)`, walks the ctor list, tail-calls `atexit` with 0x002D13C8 |
| 0x002D1518 | `initFloatOnce` | `__main` | the `initialized` guard calling `__do_global_ctors` once; byte-matches `__main.o`, as 0x002D1458 does |
| 0x002D2518 | `registerAtExit` | `__register_frame_info` | stores the FDE base at +8, clears pc_begin, pc_end, fde_array and count, pushes on the object list |
| 0x002D2580 | `libc_2D2580` | `__register_frame` | `__register_frame_info(begin, malloc(0x18))`, and 0x18 is `sizeof(struct object)` |
| 0x002D25B0 | `libc_2D25B0` | `__register_frame_info_table` | the same, and also stores the base into `fde_array` |
| 0x002D2618 | `libc_2D2618` | `__register_frame_table` | `__register_frame_info_table(begin, malloc(0x18))` |
| 0x002D2648 | `libc_2D2648` | `__deregister_frame_info` | returns the object its caller frees |
| 0x002D2708 | `ctime_4` | `__deregister_frame` | `free(__deregister_frame_info(begin))`, which is the whole function |

`ctime_4` was the one plainly wrong recovered name here: the body has nothing to
do with `ctime`. The three addresses 0x002D13C8, 0x002D1458 and 0x002D1518 also
run in libgcc2.c's own source order for `#ifdef L__main`.

Still unnamed in `frame-dwarf2.o`: 0x002D1E50, 0x002D1EA8, 0x002D1FA0,
0x002D2728, 0x002D2768, 0x002D27F8, 0x002D2850, 0x002D29B8, 0x002D2B60,
0x002D2C70 and 0x002D31E0. These are the sort and search internals
(`decode_uleb128`, `fde_split`, `frame_heapsort`, `fde_merge`, `search_object`,
`execute_cfa_program` and friends); 0x002D2768 reads like `add_fdes` but its
first argument is the FDE rather than the object, so the match is not settled.


## Double-comparison runtime name

The 76-byte function at `0x002D1B38` is now named `dpcmp`, not `dpdiv`. Its
matched `sdk/libgcc.c` body unpacks its two binary64 arguments and passes the
resulting records to `__fpcmp_parts_df` at `0x002D3638`. The 288-byte latter body
compares classes, signs, exponents and fractions; it returns -1, 0 or 1 for
ordered values and 1 for unordered input. It contains no division. Arguments
arrive in `$4`/`$5`, and the signed comparison result leaves in `$2`. The
provider's `long` return spelling and implementation are unchanged. The four
active consumer declarations now also return `long`, correcting their inherited
32-bit declarations. Plain/`-da` compiler output parity confirms the actual
comparison libcall returns `DI` in `$2`, with `DF` inputs in `$4`/`$5`; this is
the EE compiler's signed 64-bit comparison ABI, not an inference from the small
result range alone. No signature was tuned to select instructions.

All active C consumers test the result's sign or zero: `effect/billManager`,
`effect/effBlur_Filter`, `newbattle/nbMisc` and `script/scrTraceCode`. The
inactive legacy SDK source is renamed consistently too. The pinned compiler
emits the runtime name `dpcmp` for ordinary double comparisons, and the current
DDS donor links that name to its corresponding comparator. This corrects a
recovered name; it adds neither a new helper nor a compatibility alias.

The former `_div_df.o` attribution in `sdk_objects.tsv` was based only on the
recovered name. As [SDK matching](sdk-matching.md) states, that rule does not
prove that these bytes came from that object. The unsupported attribution is
withdrawn rather than relabeled as a byte match; no SDK archive was available
for a new object attribution in this pass. The 80-byte table interval includes
four alignment bytes beyond the 76-byte function.

`tools/names.py apply smt3` records the correction as **inferred** in
`name_sources.txt` and inserts a hand-curated symbol row. This follows
[Names](names.md#recording-provenance), and `ida_symbols.py` preserves such rows
outside its generated block. **The IDB was not updated: no IDA session/tool was
available.** Its correction remains pending; the curated override prevents a
future export from restoring the wrong build symbol.

All five active owners, a forced assembly re-split, and the full retail checksum
qualify the correction independently of the billboard reconstruction. It earns
zero new function or byte credit. No compiler, assembler, options, unit
boundaries or runtime instructions are changed.

## Double-division runtime name

The 320-byte provider at `0x002E6C48`, formerly `libc_2E6C48`, is now named
`dpdiv` through the inferred curated-name workflow. Its native body unpacks
two binary64 operands with `sdfUnpackDouble`, handles floating-point classes,
divides the mantissas and returns `__pack_df`'s binary64 result. The existing
SDK index identifies these bytes with `_div_df.o`; this differs from the
withdrawn name-only attribution of the comparator.

Ordinary `double` division in `sdk/libc::_ratio` emits the runtime call
`dpdiv`. Native `_ratio` calls `0x002E6C48` after constructing the same two
scaled binary64 operands. Resolving that compiler name to this verified
provider gives an exact 204-byte routine and a byte-identical retail image.
No compatibility alias, integer argument substitution or direct-call
workaround is required. The comparator at `0x002D1B38` remains `dpcmp`.

`_ulp` also matches its complete 208-byte native body using the same genuine
binary64 union view. Exponent fields above 52 yield a normal power of two;
fields 1 through 52 yield the corresponding subnormal spacing, while field 0
also returns the minimum subnormal spacing. Its result words are initialized
in their native branches without extra storage or instructions.

## Native SDK names: conversions, packing and kernel syscalls

The curated symbol map corrects these names. Its export writer suppresses IDB
rows at curated addresses, so regeneration cannot restore an old name at the
wrong function. The IDB snapshot still needs these corrections when available.

| Address | Previous name | Native name |
| --- | --- | --- |
| 0x002CD680 | gcc2_compiled__7 | ipuWriteFromIpuChcr |
| 0x002D1550 | sdfFloatToULong | fptodp |
| 0x002D1CE0 | dptoul | __make_dp |
| 0x002D1D10 | sdfDoubleToULong | dptofp |
| 0x002D1D68 | sdfDoubleToUInt | dptoul |
| 0x002DBAA0 | libc_2DBAA0 | SetGsCrt |
| 0x002DBCB0 | libc_2DBCB0 | DeleteThread |
| 0x002DBCC0 | StartThread | _StartThread |
| 0x002DC2C0 | libc_2DC2C0 | _ExecOSD |

The instruction bodies pair with the named DDS1 routines after masking only
resolved code/data relocations. Scalar constants, member offsets, branch
positions and registers remain equal. The syscall wrappers retain their exact
native syscall numbers; `_StartThread` is the low-level wrapper, not the
higher-level SDK entry.

The old `dptoul` at 0x002D1CE0 writes class, sign, exponent and fraction at
0/4/8/0x10 of a stack record and calls `__pack_df`. This establishes the
packing operation; its `__make_dp` name is inferred from the corresponding
four-argument GCC helper. The GOFAST aliases also establish `fptodp`, `dptofp`
and `dptoul`. See [GCC 2.95.3 fp-bit.c](https://github.com/gcc-mirror/gcc/blob/releases/gcc-2.95.3/gcc/config/fp-bit.c).
The former name-only `_df_to_usi.o` attribution for the packing helper is
withdrawn. The actual conversion at 0x002D1D68 retains its byte-supported
archive attribution. Earlier statements that both helpers should retain the
old name are superseded by this native distinction.

Existing C argument and return types are preserved, including declarations
expressing soft-float values as integer bits. This change removes the obsolete
float-conversion equates without introducing ABI changes.
