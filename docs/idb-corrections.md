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


## Low 64-bit integer multiplication

`0x002D3398`, formerly `libc_2D3398`, is curated as inferred `__muldi3`.
Its native partial products compute the low 64-bit integer product, and all
four compiler-generated integer calls in the now-matched `dpmul` select that
address. The provider stays assembly backed; the name adds no matching credit.
See [double arithmetic](libgcc-arithmetic-matching.md) for the caller and
qualification evidence. The IDB correction remains pending; the curated row
prevents an export from restoring the old generated name.


## The mini-title step, not a snow flag

`0x0054EFAC`, formerly `fldSnowActive`, is the mini-title animation step, and
`0x00150BB8`, formerly `fldSetSnowActive`, is the function that starts it. Both
are renamed in the IDB to `fldTitleMiniStep` and `fldSetTitleMiniStep`.

The whole evidence is the reference set. Only three functions read the word:
`fldTitleMini_Process` switches on it with cases 0 to 3 and increments it at the
end of each phase, `fldTitleMiniStart` zeroes it, and `0x00150BB8` is nothing but
`if (!step) step = 1;`. No snow routine touches it, and `fldSnowInit` at
`0x00150BD0` keeps its own state. HD's `fldTitleMiniDrawProc` switches on
`gfldTitleMiniStep` over the same four cases and keeps the per-phase frame
counter separately, which retail has at `0x0054EFA8`.

`config/smt3/symbol_addrs.txt` is not regenerated here. The live IDB currently
merges several function boundaries that the checked-in file keeps separate
(`func_00116C30` into `dds3GlobalWorkInit`, four symbols into
`dds3Particle_SpiralRandomize`, two into `slShutDownProcess`), so an export today
would delete symbols other work depends on. The names are correct in the IDB and
will carry on the next export taken when those boundaries are settled.

## Two literal pools read as globals

`D_0054C468` and `D_0054C46C` are not data symbols. Each holds `cccccc3d`, the
binary32 `0.1f`, and they are the per-function `.lit4` pool entries of
`fldHitChkUnit` and `fldHitChkUnitVWM`; `D_0054C40C` and `D_0054C410` are the
same arrangement for `3.1399996f` in `fldPlayerCalc_Wm2`. Adjacent identical
floats, one per referencing function, are the signature: a shared global would be
referenced from both.

Writing the literal in the source removes the extern and gains a word in each
case. These auto-named addresses should not be declared as globals.

## Element symbols merged into the object code indexes

The IDB split 39 objects into one symbol per element, but retail code indexes from the first name across the rest. `config/smt3/symbol_addrs.txt` now carries one sized symbol per object under the first name (the name the C uses), and the element names below are dropped from it. `tools/check_data_sizes.py` finds them (BEYOND and C size classes). The IDB itself is not edited from this tree: merge the names there too, and note that an `ida_symbols.py` export from the unmerged IDB would re-add the dropped lines.

Format: base (size in bytes): dropped element names.

- `fldCamPhokanSelX` (0x8): `fldCamPhokanSelY`
- `fldCamPhokanEyeX` (0x8): `fldCamPhokanEyeY`
- `fldCamPhokanTgtX` (0x8): `fldCamPhokanTgtY`
- `fldHitChkWork` (0xC): `fldHitChkWork2`, `fldHitChkWork3`
- `effBTLFieldColorFilter` (0x24): `effBTLFieldFilterAlpha`, `effBTLFieldFilterFlag`
- `effBTLSystem_Kyusyuu_PartList` (0x14): `effBTLKyusyuuParam`
- `nbCameraHelperViewerPBParam` (0x3C): `nbCameraHelperViewerPBTable`
- `cmpStatusAnimCount` (0x8): `cmpStatusAnimFrame`
- `pipeCamShake` (0xC): `pipeCameraQuakeX`, `pipeCameraQuakeY`
- `pipeCamShakeVec` (0x10): `pipeCamViewport`
- `sdfCameraTm` (0x20): 
- `fldRunDanteStartX` (0x4): `fldRunDanteStartZ`
- `blur2target` (0x28): 
- `filter2target` (0x18): 
- `slNewGameBit2561` (0x18): `slNewGameBit2563`, `slNewGameBit2565`, `slNewGameBit2567`, `slNewGameBit2569`, `slNewGameBit2571`, `slNewGameBit2573`, `slNewGameBit2575`, `slNewGameBit2577`, `slNewGameBit2579`, `slNewGameBit2581`, `slNewGameBit2583`, `slNewGameBit2585`, `slNewGameBit2587`, `slNewGameBit2589`, `slNewGameBit2591`, `slNewGameBit2593`, `slNewGameBit2595`, `slNewGameBit2597`, `slNewGameBit2599`, `slNewGameBit2601`, `slNewGameBit2603`, `slNewGameBit2605`, `slNewGameBit2607`
- `fldDspark01File` (0x28): `fldDspark01Data`, `fldDspark01Size`
- `fldDsmoke2File` (0x28): `fldDsmoke2Data`, `fldDsmoke2Size`
- `fldDspark02File` (0x28): `fldDspark02Data`, `fldDspark02Size`
- `fldGun02aFile` (0x28): `fldGun02aData`, `fldGun02aSize`
- `fldGun01a0File` (0x28): `fldGun01a0Data`, `fldGun01a0Size`
- `fldMuzzleB010File` (0x28): `fldMuzzleB010Data`, `fldMuzzleB010Size`
- `fldDsmoke1File` (0x28): `fldDsmoke1Data`, `fldDsmoke1Size`
- `fldDflash1File` (0x28): `fldDflash1Data`, `fldDflash1Size`
- `nbCameraBAState` (0x38): `nbCameraBATimer`
- `gFGC` (0x20): `fmFontMemHandle`, `fmRestBufNum`, `fmFontMemPool`
- `pbDebugPanel3X` (0x4): `pbDebugPanel3Y`
- `mdlDebugScaleWork` (0xC): `mdlDebugScaleFlag`, `mdlDebugScaleIndex`, `mdlDebugTextColor`
- `movieViewerDebugFlag` (0x50): `movieViewerDrawFlag`, `movieViewerDrawMode`, `movieViewerCtrlPtr`, `movieViewerRegPtr`, `movieViewerInfoLine`
- `fileQueueHead` (0x40): `pacReadState`, `fileLoadQueue`, `fileCallbackState`, `fileSysBuf`
- `sdfFileRpcState` (0x18): `sdfFileRpcSema`, `sdfFileRpcReadSize`, `sdfFileRpcReadRest`
- `dds3BlurState` (0x58): 
- `dds3MultiBlurBuf` (0x2C): `mulblurbase`
- `dds3DistBlurBuf` (0x2C): `distblurbase`
- `dds3RipBlurBuf` (0x2C): `ripblurbase`
- `ripblurtarget` (0x58): 
- `dds3MFilterBuf` (0x24): `mfilterbase`
- `fldAutoMapPosX` (0x10): `fldAutoMapPosY`
- `nbCamera_DOUTYO_B4P` (0x38): `nbCameraDoutyoMode`, `nbCameraDoutyoStep`
- `softenBiasWork` (0x38): `softenBiasState`

27 further bases are not merged yet (C uses the element names, or a `D_` label inside is referenced); see the datasizes handoff.

Four more bases, merged the same way (C never names the dropped elements): `sdfDrawFaceListParam` (0x20: `sdfDrawFaceListParam2`), `dds3ScreenBlurOfsX` (0x8: `dds3ScreenBlurOfsY`), `fadergba` (0x4: `dds3FadeColorG`, `dds3FadeColorB`, `dds3FadeState`), `nbStageState` (0xC: `StageData`, `nbStageNo`, `nbStageSub`, `nbStageParam`).

Embedded blobs sized to their header length (u8): 81 scrEmbedded_*, itfMesEmbedded_* and similar symbols. Inner names dropped as parts of larger blobs: itfMesEmbedded_326858, itfMesEmbedded_522AF0, dds3ArcTable, D_005004D8.
Element merges whose C used the dropped names, rewritten to `base[i]` or `base.field` (the field names are the existing struct's):

- `gCommandFlag` (`fclTerminalCommandCount`), `dds3ConfigValue` (`dds3ConfigBackup`), `dds3PadVibrationMode` (`dds3PadVibrationLevel`), `fldAutoMapWindowTex[3]`, `gFT` (`frFontHandleNum`, `frFontEntryTable`), the pb camera rows (`pbCameraVec`, `pbGameCameraParam`), `sdf3DScreenParam` (`sdfDrawFaceListParam`).
- `fldGlobalBase` (0x138, 47 element symbols) as `fldAreaWork_t`, `fldRunDanteGunFlag` (0x60, 14) as `fldDanteGunState_t`, `fldRun_Dante` (0x74, 23), `movieViewerWork` (0x14, 6), `corePktFaceDivParm` (0x30: `coreLightConst1`, `coreLightConst2`), `itfMesMngManager` (0xA68: `pinst`, `g_manager`, `itfMsgMngWork`, `msgWindowState`).

Where retail loads a covered element as its own symbol, cc1 changes addressing if C uses `base[i]`, so the unit keeps a `SYMBOL_AT` alias: `dds3PadVibrationCurrent` (`dds3PadVibrationState`+1), `fldSequenceState` (`fldAutoMapSeqFlags`+4), `pbDebugFovWork` (`itfMesMngManager`+0xA60), and `SYMBOL_AT_REGION` `pinst` (`itfMesMngManager`+0x2C).

The five bases with a `D_` label inside, decided by whether code indexes across the label:

- `fldSceneParam`: merged to 0xA0. `fldSceneParam_t` is that size and `fldFileResolver.c` indexes `[22]`, past `D_002FC41C`, which became `.pointResrcID`.
- `dds3PadVibrationState`: merged, `D_00551B91` replaced by the alias above.
- `pNbCamera`: sized 0x5C, the larger `nbCamera_t`. No C names `D_00320E50`, which sat inside.
- `D_0056B350`: sized 0x70, the two 0x38 bezier parts the code already indexes.
- `ragListFlags`: sized 5, the array C declares. The labels at +8 and later are separate objects.

Resizes that were skipped for a referenced `D_` label: `dds3PadBitTable` is now 0x20, with `D_005298C9`, `D_005298CB` and `D_005298D5` rewritten to `dds3PadBitTable[1]`, `[3]` and `[0xD]`, and `D_005298D8` to `&dds3PadBitTable[0x10]`. `fldEncountTimer` is 8 (the sequence call passes 8 bytes). `nbPanelPTPartList` stays 4: the code indexes it as a table, but `nbPressTurnTable` and `D_004F000C` are referenced as their own arrays right after it.

Embedded blobs sized to their header length (u8): 81 scrEmbedded_*, itfMesEmbedded_* and similar symbols. Inner names dropped as parts of larger blobs: itfMesEmbedded_326858, itfMesEmbedded_522AF0, dds3ArcTable.

IDB: the element merges above were applied to SLUS_209.11 (20 bases, 124 inner names cleared, each base recreated as a byte array of the merged size; `flt_56B350` renamed `D_0056B350`; the stale `sdfDrawFaceListParam2` name inside `sdf3DScreenParam` cleared). Field and struct types were not applied there.

`nbPanelPTPartList` (0xDC, eleven 20-byte `nbPanelXYZ_t` rows; first merged at 0xB0 on a 16-byte row guess, corrected): merged. `nbPanelProcess.c` indexes `[0]` to `[10]`, past `nbPressTurnTable2` (+0x10, the `z` of row 0) and `nbPressTurnTable` (+0x14, row 1). Both names are dropped, and the one C use of `nbPressTurnTable[0]` (under NON_MATCHING) is a `SYMBOL_AT_REGION` alias at +0x14.

IDB types: the merged bases were typed in SLUS_209.11 from the decomp's declarations (parsed from the C text, so sizes and offsets match). `fldSceneParam_t`, `itfMesManager_t` and `nbPanelProcess_nbPanelXYZ_t` already existed there with other layouts (`itfMesManager_t` 0xA20 in the decomp, named members differing for `fldSceneParam_t`) and were replaced. Typed: `fldGlobalBase` (`fldAreaWork_t`), `fldRunDanteGunFlag` (`fldDanteGunState_t`), `fldSceneParam`, `fldAutoMapSeqFlags` (`fldSequenceController_t`), `mulblurtarget`, `distblurtarget`, `mfiltertarget`, `fldRun_Dante` (`fldTest_FLDRUN_DANTE_t`), `D_0056B350` (`effBezier4PPart_t[2]`), `gFT` (`frFontSystem_t`), `itfMesMngManager` (`itfMesManager_t`, with the 0x48 bytes past the struct left as a byte array), `movieViewerWork`, `corePktFaceDivParm`, `nbPanelPTPartList` (`[11]`), plus plain arrays for `pbGameCameraParam`, `pbCameraVec` (`f32[3][4]`), `dds3ConfigValue` (`s32[2]`), `dds3PadVibrationMode` (`u8[2]`), `gCommandFlag` (`u8[4]`) and `fldAutoMapWindowTex` (`void *[3]`). `sdf3DScreenParam` was left as a byte array here and is typed below.

`sdf3DScreenParam` (0x529670, 0xA0): typed `sdf3DScreenParam_t`. The struct already existed in `include/draw3d.h` and `sdf3D.c` and `tools/header_layout.c` pins its size and offsets (now including `flags` at 0); only the global was still `u8[]`. `sdf3D.c`, `sdf_Decomp.c` and `sdfVu1.c` now declare it with the struct (`sdfVu1.c` through an incomplete tag, since `draw3d.h` clashes with its local packet types). Readers confirm the layout: `sdf3DSetCamera` copies the 0xA0 bytes in, `sdf_Decomp.c` loads `scale` and `trans` (+0x70, +0x80), and `sdfVu1.c`'s `VU0_SET_CLIP_PLANES` loads the same two rows. Retail ELF unchanged.

IDB: `sdf3DScreenParam_t` was 112 bytes there, because its `TM` member is typed `sdfMatrix4_t` and the IDB's `sdfMatrix4_t` is a 16-byte struct (`parm`, `regs` u64), not the 64-byte matrix. The struct was replaced with `TM` as `float[16]` (160 bytes) and applied at 0x529670. The wrong `sdfMatrix4_t` still sits in 35 other members (`TM`, `tm`, `matrix`, `cameraTM`, ...) and is not fixed here.

Follow-ups to the IDB types: `itfMesMngManager` is sized 0xA20, the size of `itfMesManager_t` (128 nodes of 0x14 plus 0x20). The 0xA68 it was merged to swallowed a second object: `pbDebugFovWork` at +0xA60 (0x56C8C0, 0x10), which `itfTestMesManager.c` now declares as its own symbol instead of a `SYMBOL_AT` alias. The 0x40 bytes at 0x56C880 between them are referenced by no C and no `D_` label, and stay unnamed. `nbPanelPTPartList[1]` replaces the `nbPressTurnTable[0]` alias (NON_MATCHING draft, compiled by check_nonmatching). The IDB parse error in `frFontData_t` was `addr_t`, which the extractor had stubbed as an opaque struct; it is `u32` on the EE.

The 0x40 bytes at 0x56C880 after `itfMesMngManager` are used: `itfMesMngGetVarLocalize` writes a NUL-terminated name into them (two bytes per character at most, from the save slot) and `itfMesMngSetVar` case 10 reads it with `strlen` (xrefs from 0x1CE54C and 0x1CE7A0 in the IDB; the C reached them as `pinst + 0x9F4`). Named `itfMesMngVarText`. No PCSX2 watch was run; the two static xrefs and the code that writes then reads the buffer settle it.

`sdfMatrix4_t` and `sdfVector4_t` (IDB types): both were typedefs of a 16-byte struct named `QWORD_ALIGNED` (`parm`, `regs`, both u64, 8-aligned). The attribute macro had been parsed as a type name, so every matrix was 16 bytes. Redefined at the same ordinals as `struct { f32 m[16]; }` (64 bytes) and `struct { f32 x, y, z, w; }` (16 bytes), and the stray `QWORD_ALIGNED` type deleted. Alignment is 4 for both, not 16: `QWORD_ALIGNED` expands to nothing in the PS2 build, and ee-gcc 2.96 gives `struct { char c; sdfMatrix4_t m; }` 68 bytes and the vector version 20. Making them 16-aligned in the IDB moved `sdfModelRootNode_t` to 160 against the decomp's 152 and `Sprite3D_t` to 80 against 72.

Result over the 1414 IDB structs, compared by name with the 138 sizes `tools/header_layout.c` pins: mismatches 158 before, 131 after, none newly wrong. 115 structs changed size or offsets. The 29 holding an `sdfMatrix4_t` member (35 members) grew to fit it, among them `sdfModelNode_t` 160 to 256, `sdfModelRootNode_t` 104 to 152, `sdf3DViewportPacket_t` 112 to 208, `sdf3DLightParallelPacket_t` 176 to 224 and `dds3PoseSet_t` 160 to 208, all now equal to the decomp's. The `sdfVector4_t` change (8-aligned to 4-aligned) shrank about 85 more by 4 to 12 bytes each (`Sprite3D_t` stays 72). `dds3DrawEnv_t` 5760 to 6192. The remaining 131 mismatches are IDB structs whose HD layout is a different type from the decomp's (`nbMainProcessData_t`, `dds3AdminiData_t`, `scrProcessWork_s` and others) and were already wrong before.

No decomp definition exists for 22 of the changed effect and particle structs (`effPCP*`, `effMaga*`, `dds3Particle_Basic_t`, ...), so their new sizes follow from the HD member list and are not checked against the retail compiler. Global data items typed with a changed struct were checked for overlap with the next named address: none.

## Re-import of mismatching IDB structs, and what stays unverified

`tools/header_layout.c` size claims that the IDB contradicted: 131 before, 0 after, over 145 checked sizes (the 158 of the original IDB fell to 131 with the `sdfMatrix4_t` fix). 106 structs were re-imported from the decomp's own declarations, with their missing dependencies created first. `sdf3DScreenParam_t.TM` is `sdfMatrix4_t` again. Only IDB types changed, so the ELF and smt3-port need nothing.

### IDB names the decomp lacks (candidates for the decomp)

The re-import replaced these IDB field names with the decomp's generic ones. Offset, IDB name, decomp name:

- `dds3UnitObjectBasicWorkData_t` +28: `dmy` (decomp `reserved1C`)
- `corePktCb_t` +144: `payload` (decomp `dataPtr`)
- `fldResHead_t` +4: `resourceType` (decomp `word04`)
- `fldResHead_t` +12: `flag` (decomp `word0C`)
- `fldResHead_t` +20: `areaname` (decomp `word14`)
- `fldResHead_t` +28: `SBlockName` (decomp `word1C`)
- `dds3GlobalWork_t` +25980: `name_code` (decomp `unk657C`)
- `dds3GlobalWork_t` +105744: `amapEX` (decomp `reserved19D10`)

### Effect and particle structs with no decomp definition (unverified)

The IDB holds these, nothing in `src/` or `include/` defines or mentions them, so their sizes are the IDB's alone and no header claim pins them. Sizes in bytes.

Eff_* family (22):

- `EffBTL_BillFireball_Part_t` 44
- `EffBTL_BillFireball_t` 80
- `EffBTL_BillTrack_Part_t` 64
- `EffBTL_BillTrack_t` 116
- `EffTable_t` 4
- `Eff_FlashPolyParam_t` 48
- `Eff_FlashPolyPart_t` 12
- `Eff_FlashPoly_t` 28
- `Eff_ModelTrackPolyABGR_t` 16
- `Eff_RippleSprite_t` 20
- `Eff_Ripple_t` 40
- `Eff_SparkleSphereBillParam_t` 28
- `Eff_SparkleSphereBill_t` 8
- `Eff_ThunderPolyParam_t` 80
- `Eff_ThunderPolyPart_t` 12
- `Eff_ThunderPoly_t` 32
- `Eff_ThunderRadiatePolyParam_t` 68
- `Eff_ThunderRadiatePolyPart_t` 20
- `Eff_ThunderRadiatePoly_t` 32
- `Eff_ThunderSpherePolyParam_t` 72
- `Eff_ThunderSpherePolyPart_t` 44
- `Eff_ThunderSpherePoly_t` 60

dds3 effect and particle types (17):

- `dds3EffMajorFunc_t` 48
- `dds3EffectTable_Reset_t` 4
- `dds3EffectTable_Scale_t` 4
- `dds3EffectTable_t` 12
- `dds3EffectWork_t` 48
- `dds3Effect_DistortionBlur_t` 16
- `dds3Effect_PartialBlur_t` 8
- `dds3Effect_t` 16
- `dds3Particle_Blur_t` 8
- `dds3Particle_EffItem_t` 16
- `dds3Particle_Exp_t` 16
- `dds3Particle_Expansion_t` 20
- `dds3Particle_Nop_t3` 4
- `dds3Particle_SphereAtom_t3` 28
- `dds3Particle_Track2_t` 20
- `dds3Particle_Track3_t` 20
- `dds3Particle_Track_t` 16

### Outcome of the name candidates

Renamed in the decomp, where retail code proves the name: `dds3GlobalWork_t.unk657C` to `nameCode` (`nmeBuildNameFont` fills it with the entered name's glyph codes) and `fldResHead_t.word14` to `areaName` (`fldFileResolver.c` reads it as the area reference). Not renamed, no retail access proves them: `fldResHead_t` `word04`, `word0C`, `word1C`; `reserved19D10` (`amapEX`); `reserved1C` (`dmy`). `corePktCb_t.dataPtr` keeps its name over the IDB's `payload`.

## Check of the 39 unverified effect structs against SLUS_209.11

Method: the IDB applies none of these types to a retail function except `Eff_ModelTrackPolyABGR_t`, `dds3EffectWork_t` and `dds3Particle_SphereAtom_t3`, so each struct was matched to its creator or accessor by name, read in Hex-Rays, and sized from allocation sizes, strides and the largest offset. The `Eff_*` types are an older naming of the `effPCP*` types retail uses, with different sizes in five cases. main.elf (port 13339) did not answer `server_health` and was not used; a cross-check there is still owed, and field names on every verified entry need it.

### Verified from retail

- `Eff_ModelTrackPolyABGR_t` 16: `dds3HermitianTrackPolySetVC` reads `s0,s1,e0,e1` at +0,+4,+8,+12 as u32, so 16 from the largest offset. Field names need main.elf.
- `dds3Effect_t` 16: `dds3EffectCreate` allocates 16 and writes major +0, minor +4, a work pointer +8 and the float 1.03 at +12. `dds3EffectLoad` reads the file header the same way and the payload starts at +16. Field names need main.elf.
- `dds3EffMajorFunc_t` 48: `dds3EffectCreate` indexes `EffMajorTable` at 12 words per entry and calls the entry at +0, so 48. Slot names beyond create need main.elf.
- `dds3EffectTable_t` 12: `dds3ParticleExpCreate` indexes `dds3ParticleFuncTable` at 3 words per entry and calls +0, so 12. Slots +4 and +8 are not read in the functions checked; their names need main.elf.
- `dds3Particle_Exp_t` 16: `dds3ParticleExpCreate` reads type at +0 (compared with 3) and size at +4, and the payload starts at `pData + 16`. `reserve1`/`reserve2` are unread; names need main.elf.
- `Eff_ThunderPolyPart_t` 12: Part stride is 12 in `effPCP_ThunderPolyCreate` (alloc `12*n + 104`). Verified as the size of the effPCP sibling, which retail uses; no retail code names the Eff_ type.
- `Eff_ThunderRadiatePolyPart_t` 20: Part stride is 20 in `effPCP_ThunderRadiatePolyCreate` (alloc `20*n + 88`). Same caveat: the effPCP sibling is what retail uses.
- `Eff_ThunderSpherePolyPart_t` 44: Part stride is 44 in `effPCPThunderSpherePolyCreate` (alloc `44*n + 100`). Same caveat: the effPCP sibling is what retail uses.

### Contradicted by retail

- `dds3EffectWork_t` 48: Retail allocates the object's work as 80 bytes (`dds3EffectObjectBasicConstructor`: `sdfChipAllocDirect(80)`, memset 0x50), IDB says 48. Only `control` at +0 is read by `dds3EffectObjectBasicDestructor`; other fields unverified.
- `dds3Effect_DistortionBlur_t` 16: `dds3EffectDistortionBlurCreate` allocates 4856: the 44-byte param is copied inline at +0, texture at +44, memory handle at +48, parts pointer at +52, then 100 parts of 48 bytes from +56. IDB has a 16-byte pointer layout.
- `dds3Particle_SphereAtom_t3` 28: `dds3Particle_SphereAtomCreate` copies 0x20 bytes after the 336-byte `dds3Particle_Basic_t` (alloc 368), IDB says 28. `SphereAtomBuffInit` tests a byte at ext+0 as the type and reads timing at ext+8, so the IDB's leading `basic` pointer is not there.
- `Eff_ThunderSpherePolyParam_t` 72: `effPCPThunderSpherePolyCreate` copies 76 bytes of param (the effPCP sibling is 76), IDB Eff_ type says 72.

### Still unverified

- `EffBTL_BillFireball_Part_t` 44: No retail function found by name or by type reference (`effBTL*Bill*` does not exist in SLUS_209.11).
- `EffBTL_BillFireball_t` 80: No retail function found by name or by type reference (`effBTL*Bill*` does not exist in SLUS_209.11).
- `EffBTL_BillTrack_Part_t` 64: No retail function found by name or by type reference (`effBTL*Bill*` does not exist in SLUS_209.11).
- `EffBTL_BillTrack_t` 116: No retail function found by name or by type reference (`effBTL*Bill*` does not exist in SLUS_209.11).
- `EffTable_t` 4: No retail accessor found.
- `Eff_FlashPolyParam_t` 48: Only effPCPFlashPoly* siblings exist in retail, with other sizes; no retail code names this type.
- `Eff_FlashPolyPart_t` 12: Only effPCPFlashPoly* siblings exist in retail, with other sizes; no retail code names this type.
- `Eff_FlashPoly_t` 28: Only effPCPFlashPoly* siblings exist in retail, with other sizes; no retail code names this type.
- `Eff_RippleSprite_t` 20: Retail has only `effPCPRippleBlurExp` and `dds3EffectRippleBlur`, which are other types.
- `Eff_Ripple_t` 40: Retail has only `effPCPRippleBlurExp` and `dds3EffectRippleBlur`, which are other types.
- `Eff_SparkleSphereBillParam_t` 28: `effPCPSparkleSphereCreate` allocates 32 with a billboard handle; no retail object matches the 8-byte bill.
- `Eff_SparkleSphereBill_t` 8: `effPCPSparkleSphereCreate` allocates 32 with a billboard handle; no retail object matches the 8-byte bill.
- `Eff_ThunderPolyParam_t` 80: Retail uses the effPCP sibling, whose size differs or was not checked; no retail code names this type.
- `Eff_ThunderPoly_t` 32: Retail uses the effPCP sibling, whose size differs or was not checked; no retail code names this type.
- `Eff_ThunderRadiatePolyParam_t` 68: Retail uses the effPCP sibling, whose size differs or was not checked; no retail code names this type.
- `Eff_ThunderRadiatePoly_t` 32: Retail uses the effPCP sibling, whose size differs or was not checked; no retail code names this type.
- `Eff_ThunderSpherePoly_t` 60: Retail uses the effPCP sibling, whose size differs or was not checked; no retail code names this type.
- `dds3EffectTable_Reset_t` 4: Single delegate slot; the slot is called through the particle/effect tables but the wrapper struct has no retail footprint.
- `dds3EffectTable_Scale_t` 4: Single delegate slot; the slot is called through the particle/effect tables but the wrapper struct has no retail footprint.
- `dds3Effect_PartialBlur_t` 8: `dds3EffectPartialMultiBlurCreate` allocates 48 with the blur inline and a texture at +44, a different type from this 8-byte pointer pair.
- `dds3Particle_Blur_t` 8: Reached only through VU0 inline code and untyped offsets (`dds3Particle_TrackSetExp`, `BlurCreate` use buffers of other types); no access pins this struct's size.
- `dds3Particle_EffItem_t` 16: Reached only through VU0 inline code and untyped offsets (`dds3Particle_TrackSetExp`, `BlurCreate` use buffers of other types); no access pins this struct's size.
- `dds3Particle_Expansion_t` 20: Reached only through VU0 inline code and untyped offsets (`dds3Particle_TrackSetExp`, `BlurCreate` use buffers of other types); no access pins this struct's size.
- `dds3Particle_Nop_t3` 4: Reached only through VU0 inline code and untyped offsets (`dds3Particle_TrackSetExp`, `BlurCreate` use buffers of other types); no access pins this struct's size.
- `dds3Particle_Track2_t` 20: Reached only through VU0 inline code and untyped offsets (`dds3Particle_TrackSetExp`, `BlurCreate` use buffers of other types); no access pins this struct's size.
- `dds3Particle_Track3_t` 20: Reached only through VU0 inline code and untyped offsets (`dds3Particle_TrackSetExp`, `BlurCreate` use buffers of other types); no access pins this struct's size.
- `dds3Particle_Track_t` 16: Reached only through VU0 inline code and untyped offsets (`dds3Particle_TrackSetExp`, `BlurCreate` use buffers of other types); no access pins this struct's size.

### Size fixes applied to the IDB

Replaced in place (ordinals kept), each from the retail evidence above:

- `dds3EffectWork_t` 48 to 80: the existing fields are unchanged and `reserved30[32]` is added.
- `dds3Effect_DistortionBlur_t` 16 to 4856: `param[44]` inline, `hAlphaTex` +44, `hMem` +48, `pwork` +52, `parts[4800]` +56 (100 parts of 48 bytes, element type not recovered).
- `dds3Particle_SphereAtom_t3` 28 to 32: `type` moved to +0 (retail tests the byte there) and the `basic` pointer dropped. `loop` kept at +5 and `timing` at +8 (retail reads +8); `loop` is not proven by retail.

Data names absorbed into the symbol that code indexes past (retail reads them through the owner's base, so they are fields, not objects): NM_PrimPolyParm, RPE_PrimPolyParm, SPL_PrimPolyParm, cmpSkillDrawWork, dds3BezierWork, dds3FlashPolyTag, dds3HermitianTag, fldAutoMapTexHandle, fldCamPhokanEyeW, fldCamPhokanSelZ, fldCamPhokanTgtW, g_debug, gencScale, sdfSndMapIop, and the unnamed D_002FA244, D_00529758, D_005519BE. Owners: the preceding symbol at each address.

## Data symbol renames

Names given to `D_xxxxxxxx` data, each from a use that shows what the data holds.
The IDB carries the same names. Types, sizes and `can_be_referenced` are untouched:
the host data generator's output for the tree before and after is identical once the
new names are mapped back.

| Old | New | Evidence |
| --- | --- | --- |
| `D_0054AF60` | `two_over_pi` | 198 ints from `0xA2, 0xF9, 0x83, 0x6E`, passed as `twoOverPi` to `__kernel_rem_pio2f` |
| `D_0054B278` | `npio2_hw` | `(ix & 0xFFFFFF00) != D[n - 1]` in `__ieee754_rem_pio2f`; words `0x3FC90F00`, `0x40490F00` |
| `D_00551928`, `D_00551930`, `D_00551938`, `D_00551940` | `sdfFileCdromPrefix`, `sdfFileHostPrefix`, `sdfFileHddPrefix`, `sdfFileCdromName` | bytes `cdrom0:`, `host0:`, `pfs0:`, `cdrom`; copied by the `sdfFileSet*Filename` functions |
| `D_00526530`, `D_00526540` | `test_devilIds`, `test_devilSkills` | the `devilList` and `skillList` arguments of `nbSetBattleDummy` (ids until a negative word; 30 skills per devil) |
| `D_005267D0`, `D_005267E0` | `test2_devilIds`, `test2_devilSkills` | the same two arguments in the second call |
| `D_0032D270` | `cmbRaceCombineTable` | `cmbLookupCombine` indexes `[flag][race - 1][race - 1]` with the two devils' races |
| `D_0052F850` to `D_00530A98` (14 tables, `0x168` apart) | `dds3HijriParts`, `dds3HijBParts`, `dds3PlayerAParts`, `dds3PlayerBParts`, `dds3TiakiParts`, `dds3YuukoParts`, `dds3IsamuParts`, `dds3IsamuFParts`, `dds3IsamuFzParts`, `dds3HikawaParts`, `dds3Model113Parts`, `dds3TiakiBParts`, `dds3MajyoParts`, `dds3HutomimiParts` | each is a `dds3UnitPartTable_t` whose first field is that name (`hijri`, `hij_b`, `player_a`, ... `0x113`, ... `hutomimi`) |
| `D_00529030` | `pipePartsInfoTable` | `getPartsInfo` scans 60 `tagPipePartsInfo` for a `partsID` |
| `D_003202C8`, `D_00320770` | `evtUnityCalcArea`, `dblurParam2` | returned by `evtGetUnityCalcArea` and `dblurGetParam2`, written by their setters |
| `D_00325B10`, `D_00325E50` | `frTagCodeTable`, `frTagCodeTable2` | `frFindTagCode` and `frFindTagCode2` search 0x1A0 `u16` codes |
| `D_003278D4` | `pbMCardErrorRows` | `pbMCard` takes `text` from `0x84`-byte rows by error code |
| `D_00551F80` | `fclFlags` | `fclClearFlag` and `fclGetFlag` index it by `s8` |
| `D_0054F4D0` | `itfMesSelectionMarkers` | two string pointers (`"* "`, `"> "`) picked by `mode != 0` into `selectionMarker` |
| `D_0054F200`, `D_0054F208`, `D_0054F210`, `D_0054F218` | `effPadVibeFrames`, `effPadVibeRemain`, `effPadVibePower`, `effPadVibeFadeIn` | `effPadVibeFadeIN/OUT` store frame, frame, power, flag; `effPadVibeFadeUpdate` scales power by remain / frames |
| `D_0056AD58` | `effDebugPoly` | an `effDebugPolyParams_t`, zeroed with `primitive = 0x4000` |
| `D_005311D0` | `fldTamaSeVol` | an `fldTamaSeVol_t` copied by value |
| `D_00551EC4`, `D_00551EC6` | `pbMyCharPosX`, `pbMyCharPosY` | out parameters of `pbGetMyCharcterPos2`, then added to `pbKeyTable` offsets and bounded by 20 |
| `D_0030FBD4`, `D_0030AB54` | `fldAutoMapFloorTable`, `fldAutoMapAreaTable` | 28-byte rows indexed by `fldAutoMapFindFloor` and `fldAutoMapFindArea`, `strlen`'d and drawn |
| `D_0052D2F0` | `TLBFunctionsImage` | 0x330 bytes of MIPS code that `InitTLBFunctions` copies to `0x80075000` |
| `D_00312150`, `D_00551CC0` | `fldAutoMapModels`, `fldAutoMapSpriteTex` | model root list and sprite texture handle used by the automap draw |
| `D_00567EA0`, `D_00567EF8`, `D_00567F50` | `fldAutoMapAreaNameLen`, `fldAutoMapFloorNameLen`, `fldAutoMapPlaceNameLen` | `strlen` results cached per row of the area, floor and place tables |
| `D_0030D714`, `D_0030A670`, `D_00551CC8` | `fldAutoMapPlaceTable`, `fldAutoMapTitleTable`, `fldAutoMapTitleLen` | 28-byte string rows indexed by `fldAutoMapFindPlace` and the title lookup |
| `D_00551CA0`, `D_0054EE78`, `D_0054EE7C` | `fldAutoMapFontReq`, `fldAutoMapFloorCur`, `fldAutoMapFloorOld` | font request and the current and previous floor index in `fldAutoMap.c` |
| `D_00551948`, `D_00551954`, `D_00551958` | `sdfFileCmdActiveHead`, `sdfFileCmdCacheHead`, `sdfFileCmdCacheTail` | `sdfFileCmdDequeue` unlinks from the active list and appends to the cache list |

More data names absorbed the same way: fldCamPhokanEyeZ (fldCamPhokanEyeX+8), fldCamPhokanTgtZ (fldCamPhokanTgtX+8), itfMesMngDebugState (D_00326700+4), cmpAnalyzeTexHandle (D_00345D88+8), WPL_PrimPolyParm (effDebugPoly+4), dds3NeedleWork and dds3NeedleDrawCount (D_0056AD88+0x10, +0x14), dds3BezierTrackPolyCount (dds3BezierVertexCount+0x10), gencArea (D_0055DAD8+0x1C), LastLoad (sdfSndMap+0x190), dds3MatterDrawParam (D_002F69B8[7]), effBTLSkillTargetNumber and effBTLSkillTargetBit (fields of D_005698E0), p2sdPcmBuffer (D_00593E20), D_00343E74 (D_00343E40). D_004F0268 is one 46-row `nbPanelXYZ_t` array and the other D_004F02xx to D_004F05D8 panel tables are slices of it.

| `D_00551968`, `D_00551970`, `D_00551978`, `D_00551980`, `D_00551988` | `sdfFileCmdCdromPrefix`, `sdfFileCmdHostPrefix`, `sdfFileCmdPfsPrefix`, `sdfFileCmdMoviePath`, `sdfFileCmdCdimgPrefix` | bytes are `cdrom0:`, `host0:`, `pfs0:`, `/movie/`, `cdimg:`, compared and copied by `sdfFileCmdAnalyzeFileName` and `sdfFileCmdMakeRealFileName` |
| `D_0054F634`, `D_00551F04`, `D_0054F638` | `mdlHddFileIndexHandle`, `mdlHddFileIndex`, `mdlHddFileListHandle` | memory handles and the locked index pointer allocated in `mdlManager` for the HDD file index and list |
| `D_00551650` | `fileManagerProcName` | bytes `fileMan`, the name passed to `dds3StartProcess` for `fileManagerProc` |

### Preserve independently used adjacent objects

Eighteen high-offset extent extensions are withdrawn because the following word is an independently used object. `fldEncountTimer` remains eight bytes; the other bases listed below remain four bytes. Native accesses and distinct scalar consumers establish these boundaries. Seeing an access immediately after a symbol does not establish that it belongs to that symbol.

- `fldEncountTimer`: the following `D_0054EB98` is the file resolver's texture handle, not its +4 sequence word.
- `nbPanelPTP_0_Process`: `D_005511D8` is panel tint.
- `cameraDegreeInit`: `D_00551EF4` is an integer cooldown, not another float angle.
- `nbResultTexHandle`: `D_00551110` is a camera-name pointer, not another texture handle.
- The remaining independently used scalar pairs start at `rsetFrameMaskOtOffset`, `fldPlayerSyukanState`, `fldAutoMapBuf`, `fldAutoMapBBox`, `fldAutoMapDrawState`, `fldNpcCamTarget`, `evtNextEventNo`, `moonprocess_id`, `dds3TitleFadeLevel`, `nbTobiwarpLabelIdx`, `nbFormEnemyInterval`, `sdf3DFogDefault`, `BSARI_Cut1_Rate` and `nbPressGaugeState`.

The other three entries in that batch (`dds3YuukoParts`, `sdfModelShapeWeightEpsilon`, `nbCameraBoss_BSLUSI_Type`) are unchanged: this review does not establish an independent tail object for them. This is not a certification of all remaining high-offset metadata as complete ownership. Generated source includes are kept in their canonical post-split positions.

### Screen global declaration compatibility safeguard

The IDB screen type and pinned 0xA0 layout remain valid. The three C owners use a shared `u8[]` storage view for `sdf3DScreenParam`: the local tagged screen struct in `sdf3D.c` and the anonymous matrix/vector struct in `draw3d.h` have equal EE offsets but incompatible C member types. Declaring the same external object with both types creates a cross-unit type mismatch, reproduced by a native LTO diagnostic. This restores the consistent byte view without changing the 0xA0 copy, +0x70/+0x80 VU rows, or clip-plane base address. A future shared typed declaration must first unify the actual struct definitions; equal sizes alone are insufficient.

The guarded file-manager declaration of `EIntr` now agrees with its `s32` provider result. Callers still ignore that result. The correction neither changes the peer's interrupt-masking behavior nor certifies its host scheduling assumptions.

`mdlHddFileListHandle` (`D_0054F638`) stays unnamed in the IDB: it is the second word of the 12-byte struct at `0x54F634` (`mdlHddFileIndexHandle`), which is proven, so the word is recorded here as that struct's field name.
| `D_0054ED5C` to `D_0054ED8C` (11 words) | `fldSkyFilterAnimeSaved` ... `fldSkyTableSaved` | `fldSky.c` stores the previous filter, filter animation and table index of each global before it overwrites it |
| `D_0054EAF0`, `D_0054EAE8` | `fldDanteUnit`, `fldUnitControlHandle` | `D_0054EAF0` is the unit `fldMain` creates with id `DANTE_UNIT`; `D_0054EAE8` is copied from `fldUnitControl.handle` |
| `D_00531690`, `D_005316B0` to `D_00531758` (7 strings) | `fldProcSequenceName`, `fldPlayerAPath` ... `fldWave02Path` | bytes are `fldProcSequence` and the `/model/field/*.PB` and `/fld/f/bin/*.PB` paths passed to `sdfDevLoadFileEx` |
| `D_00531820`, `D_0054EB38`, `D_0054EB40` | `fldWaveUnitName`, `fldPlayerPosName`, `fldCameraUnitName` | bytes `WAVE_UNIT`, `ppos01`, `P_Cam`, passed to `dds3SetUnitObjectId` and `dds3AddPointBasic` |
| `D_0054EA38`, `D_0054EA40`, `D_0054EA48`, `D_0054EA50`, `D_0054EA58` | `dds3DebugLabelB`, `dds3DebugLabelG`, `dds3DebugLabelR`, `dds3DebugFmtInt3`, `dds3DebugFmtFloat3` | bytes `B:`, `G:`, `R:`, `%3d`, `%5.3f`, used by the colour channel readout in `dds3DebugStuff.c` |
| `D_002FF3E0`, `D_00531860`, `D_00531950` | `fldNpcTalkLabel`, `fldKiretaLabel`, `fldProcDrawName` | bytes `npctalk_label`, `kireta_label`, `fldProcDraw`; script labels and the draw process name |
| `D_00531C70`, `D_00531C80`, `D_00531D58`, `D_00531D70` | `fldDamageOnMsg`, `fldDamageOffMsg`, `fldNoroiDamageOnMsg`, `fldNoroiDamageOffMsg` | debug `printf` strings in `fldPlayer.c` |
| `D_00531E98` | `fldInfPathFormat` | bytes `%sF%03d.INF`, the format `fldInf.c` builds INF paths with |
| `D_0054ECDC`, `D_0054ECFC` | `fldCamPitchTarget`, `fldPlayerMoveStateSaved` | camera pitch eased toward in `fldPlayer.c`; move state stored at the end of the same pass |
| `D_0054ED00`, `D_0054ED04`, `D_002FCA30`, `D_002FCA40` | `fldWmapMatterLast`, `fldWmapBlendStep`, `fldWmapPosFrom`, `fldWmapPosTo` | last hit matter, 0 to 8 step counter, and the two positions `fldWmapPlayerPos` is interpolated between in eighths |
| `D_00531F18`, `D_00531F28` | `fldElevatorPosName`, `fldElevatorStartLabel` | bytes `01pos_01` and `elvstart_label`, passed to `fldSetSceneParam5` |
| `D_0054EC80` | `fldEncDrawName` | bytes `encProc`, the process name `fldEnc.c` starts `encDrawProc` under |
| `D_00532780`, `D_00532790`, `D_005327A0`, `D_005326B8` | `fldNpcUnitName`, `fldNpcPlFormatB`, `fldNpcPlFormat`, `fldGenNpcPath` | bytes `NPC_UNIT`, `%sF%03db.NPL`, `%sF%03d.NPL`, `/fld/f/bin/gen_npc.PB` |
| `D_003121D0`, `D_00314100` | `fldNpcDefaults`, `fldNpcOverrides` | `fldNpcTable_t` defaults indexed by NPC id; override records read from file (0x2A00 bytes) in `fldGlobal.c` |
| `D_0053F3C8`, `D_0053F3D8`, `D_0053F3E8`, `D_0054FD98`, `D_0054FDA0` | `movieDrawProcName`, `movieManProcName`, `movieDirPath`, `movieExtIpu`, `movieExtIps` | bytes `movieDraw`, `movieMan`, `pfs0:/movie`, `.ipu`, `.ips` in `mviManager.c` |
| `D_0054A360`, `D_0054A380` | `sdfIopModulePathCdrom`, `sdfIopModulePathHost` | bytes `cdrom0:\USERIRX\SDFIOPFL.IRX;1` and `pfs0:/userirx/sdfiopfl.irx`, tried in turn by the `sdfTmx.c` IOP loader |
| `D_00343E40`, `D_00343BE8` | `movieCb`, `movieEntryTable` | the `sdfMovieCb_t` that `mviManager.c` opens, closes and queries; `dds3MovieEntry_t` rows indexed by `dds3PlayMovieByIndex` |
| `D_00549A48`, `D_00549A60` | `sdfSndIopSizeMsg`, `sdfSndSeqMapInitMsg` | bytes `SDF_SND_IOP_SIZE` and `sdfSndSeqMapInit` in `sdfSound.c` |
| `D_00549B50`, `D_00549B70`, `D_00549B88`, `D_00549BB0` | `sdfDmaDumpStartFmt`, `sdfDmaNotQwordFmt`, `sdfDmaTagLineFmt`, `sdfDmaNestOverflowMsg` | the DMA chain dump printf formats and nest overflow message |
| `D_0054A088`, `D_0054A100`, `D_00551B40` | `sdfGsMemInfoTitle`, `sdfGsMemNodeFmt`, `sdfGsMemTypeFmt` | the GS memory information dump title and row formats |
| `D_00551A20`, `D_00551A40` | `sdfDumpAddrFmt`, `sdfDumpByteFmt` | the `%08X : ` and `%02X` formats of the memory dump |
| `D_00551A58`, `D_00551A60` | `sdfVifMaskedName`, `sdfVifXferOkName` | bytes `mask` and `xfer-ok`, the `%s` of the `VIF MSKPATH3` printf |
| `D_00529388`, `D_00580FF0`, `D_00577F80`, `D_0055188A` | `sdfGrFrameSet`, `sdfGrDispEnv`, `sdfGrStoreImage`, `sdfGrBufferSlot` | the `sdfGrFrameSet_t` every render unit reads, the `sceGsDispEnv` and `sceGsStoreImage` that `sdfGraph.c` fills, and the slot `sdfDraw.c` sets from `bufferSlot` |
| `D_00552080`, `D_00578FF0`, `D_00577FF0` | `sdfDispThreadCB`, `sdfDrawThreadStack`, `sdfDispThreadStack` | the control block and the 0x1000 stack passed to `sdfStartThreadAndWait`, and the 0x8000 stack of `sdfDrawThread` |
