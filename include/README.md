# Headers

Reconstructed types for the SLUS_209.11 decompilation. `common.h`, `include_asm.h`
and `macro.inc` belong to the build; the rest are recovered game types.

Reconstructed C units share these headers. Changes to a layout or declaration
must keep every affected whole unit clean under `tools/check_unit.py` and the
linked image byte-identical. `ps2asm.h` also contains emitted EE extension
macros; see its section below.

## Where the names and layouts come from

Three sources, in order of authority:

1. **The SLUS_209.11 IDB** (instance 13339), read-only. 9470 named functions and
   1341 named struct types carried over from SMT3HD symbol recovery. Authoritative
   for names, and its sizes and offsets win over the other two.
2. **[sdfhook](https://github.com/tge-was-taken/sdfhook)** (tge-was-taken), a native
   renderer for SDF, the engine SMT3 runs on. `src/libsdf.h` there pins about 130
   PS2 layouts with static size and offset assertions, taken from the same IDB and
   held honest by the renderer reproducing emulated frames. Primary source for the
   sdf types.
3. **[smt3-reconstructed-sources](https://github.com/smt-ret)** (smt-ret), a
   reconstruction of the SMT3 source from SMT3 HD's il2cpp metadata. Its headers
   supply the original file grouping, field names and enum member names, which the
   IDB does not carry. Its *layouts* are a C#/IL transpilation artifact and are not
   usable as-is: see the next section.

Every size and offset asserted below was re-derived from the retail code
(allocation sizes, load and store offsets, array strides) rather than copied.
`tools/header_layout.c` carries one compile-time assertion per claim and
`python3 tools/test_headers.py` compiles it with the real `cc1`; see the end of
this document.

## What the HD reconstruction gets wrong, and why it matters

The transpiler rewrote the PS2 types, so an HD header is a hint about names and not
evidence about layout. The failure modes found here:

- **A fixed char array became a pointer.** `scrHeader_t.MagicCode` is `char[4]`
  inline, compared against `"FLW0"` as one word by the file parser; HD has it as a
  `System_Char_array *`. `scrProcTable_t.Name` and `scrLabelTable_t.Name` are
  `u8[24]` inline, which is why the proc table's stride is 32 and not 12.
- **A union became a struct.** `scrCode_u` and `scrStackData_u` are 4-byte unions;
  HD lays their members out in sequence for 16 bytes each. The stack arrays stride
  by 4 and the bytecode by 4.
- **An inline struct became a pointer.** `dds3PoseSet_t.workTM` is an inline
  `sdfMatrix4_t` at offset 0, loaded as four quadwords; HD has it as a pointer.
- **Remaster-only fields.** `dds3Basic_t.gameObject` and `sdfTexHandle_t.hashname`
  and `image` exist only in the Unity build. Dropping them is what takes those
  structs from 72 to 68 and from 72 to 64 bytes.
- **Fields the PS2 build does not have.** `dds3ProcessID_t` has no `id` or
  `func_ans`, and its `starttimer`/`shutdowntimer` are `s16`, not `s32`.
- **Field order differs.** `scrProcessWork_s` puts the table pointers *after* the
  stack arrays on PS2; HD puts them first.

The IDB's own struct bodies have the mirror-image defect in places: a bulk import
expanded some pointer-to-struct fields inline, which is why the IDB reports
`scrProcessWork_s` as 316 bytes (really 244) and `dds3DrawEnv_t` as 7136 (its
fields are all pointers). Each struct below was checked against code for that
reason.

## Verified sizes and offsets

`sizeof` for every struct declared, with the retail evidence. "alloc" means the
allocator call that creates it; offsets listed are the ones a load or store in the
code puts beyond doubt.

### font.h and interface.h

These shared layouts are adapted from the matched DDS engine C and checked
against SLUS_209.11, not copied from remaster layouts:

| struct | size | retail evidence |
|---|---|---|
| `fmFontNode_t` | 32 | `fmInitFontMemControl` allocates and links nodes with a `0x20` stride |
| `frFontRecord_t` | 12 | `frInitFontTable` requests `0xC` bytes per atlas record; refs at +4 and atlas node at +8 |
| `frTextHandle_t` | 44 | `frInitFontTable` creates the `0x2C` child pool; `frInitTextHandle` and `frSetUserImage` establish its fields |
| `frFontHandle_t` | 48 | `frInitFontTable` creates the `0x30` parent pool, unlike DDS1's `0x44` pool |
| `itfPanel_t` | 52 | `itfPnlRegist2` allocates `0x34`; RGBA occupies +`0x20`..+`0x2C` and kind is the byte at +`0x30` |
| `itfPanelVertex_t` | 8 | `CreateVertexPlain` writes four XY pairs in eight-byte steps |

Font bank, atlas, packet and message-stream types used only by `frFont.c` remain
unit-local. Message-manager work also remains local: native allocation is
`0x1D4`, selection options start at +`0x5C`, the two panel pointers are at
+`0x9C`/+`0xA0`, variables at +`0xC4`, and cursor state at +`0x1C4`.
`itfMesMngInitializeManager` initializes 128 link records, rather than DDS1's 64.

### kernel.h

| struct | size | evidence |
|---|---|---|
| `dds3ProcessID_t` | 80 | `dds3StartProcess` calls `sdfChipAllocDirect(0x50)` |

All sixteen fields confirmed. `dds3StartProcess` stores `name[0]`/`name[23]`,
`namehash` +24, `flag` +28, `priority` +32, `starttime` +36, `runtime` +40,
`starttimer` +44 and `shutdowntimer` +46 with `sh` (hence `s16`, not HD's `s32`),
then +48, +52, +56 and the five links at +60..+76. `RunProcess` *calls* the word at
+48 and stores the result back, so that field is `processfunc`; `KillProcess` calls
+52, so that one is `shutdownfunc`; `dds3GetProcessData` returns +56, `workdata`.
The IDB names +48 `id` and +52 `processfunc`, one slot off, and that is corrected
here.

`enum dds3kernelpriority` is cross-checked against all 113 `dds3StartProcess` call
sites: 35 distinct values match the HD member the caller's name implies
(`DDS3_PRIORITY_GLOBALWORK` 1 from `dds3GlobalWorkInit`, `DDS3_PRIORITY_FILE_MNG`
900 from `fileSysInit`, `DDS3_PRIORITY_CALC_CAMP` 1010 from `cmpProcessStart`,
`DDS3_PRIORITY_DRAW_PB` 11005 from the `pbInit*Draw` set, and so on) with no
conflict. `enum dds3_ot`, `enum dds3_model_ot` and `enum dds3_admini_list` are
carried from HD unchecked; the headers say so.

`RunProcess` also exposes the pad snapshots kept beside the small-data init
flag: two ports, two sticks per port and two axes per stick occupy eight
bytes at `0x0054E8F8` (previous) and `0x0054E908` (task-visible). Four status
bytes follow each axis block at `0x0054E900` and `0x0054E910`. Its clear and
copy loops prove these extents. Their typed externs remain local to
`dds3KernelCore.c`; the manual symbol block supplies the native boundaries.

### basic.h

| struct | size | evidence |
|---|---|---|
| `dds3Basic_t` | 68 | `dds3AllocBasic` calls `sdfChipAllocDirect(68)` |
| `dds3PoseSet_t` | 208 | `dds3InitPoseSet` calls `sdfChipAllocDirect(208)` |
| `dds3BasicArrayHandle_t` | 16 | `dds3GetBasicArrayHandle` calls `sdfChipAllocDirect(0x10)` |
| `dds3PathBasicControl_t` | 36 | `dds3AllocPathBasicWorkData` calls `sdfChipAllocDirect(36)` |
| `dds3BasicFunction_t` | 24 | **HD only.** See below. |
| `dds3BasicArray_t` | 8 | `dds3AddWorld` strides the array it builds by 8 |
| `dds3WorldBasic_t` | 40 | `worldconstructor` calls `sdfChipAllocDirect(0x28)` |

`dds3Basic_t`: `name` +8, `type` +12 (the basic type in the top byte), `workdata`
+24, `poseset` +28, `next` +32 and `prev` +36 (from `dds3DeleteBasic`'s unlink),
`color` +52 (set to `0x80808080`). `staticdata` +20 and the three work links
+40..+48 are named from HD; nothing in the functions read touches them, so only
their positions are pinned, by the surrounding fields and the 68-byte size.

`dds3PoseSet_t`: `dds3GetTMPoseSet` loads and stores `0/0x10/0x20/0x30` as four
quadwords, so `workTM` is an inline 64-byte matrix at 0, and it calls
`dds3MakeMatrix(+96, +80, +64)`, giving `scl` +96, `rot` +80, `pos` +64.
`dds3InitPoseSet` sets `flag` +192, `r` +196 and `frameFall` +200. The four
`work*` vectors and `plane` are HD names at HD positions; the size forces them
into 112..191.

`dds3BasicArrayHandle_t`: the three `s16` indices start at -1 (+0/+2/+4),
`count` is the `u16` at +6 that `dds3GetBasicArrayCount` returns, `next` +8 and
`prev` +12 are zeroed at creation.

`dds3WorldBasic_t` is confirmed field by field by one function: `worldconstructor`
allocates 40 bytes, stores them into the world basic's `workdata` +0x18, and
initialises every member, as words at 0, 4, 8, 0xC, 0x10, 0x14, 0x20 and 0x24 and
as halfwords at 0x18 (0), 0x1A (-1), 0x1C (-1) and 0x1E (0). Four more functions
agree independently: `dds3AddScene` and `dds3DeleteScene` on `firstscene` +0 and
`lastscene` +4, `dds3SetActiveScene`/`dds3GetActiveScene` on `activescene` +8,
`dds3SetControlScene`/`dds3GetControlScene` on `controlscene` +0xC,
`dds3DeleteAllBasicArray` on `firstempty` +0x1A, `lastempty` +0x1C and
`emptycount` +0x1E, and `dds3GetBasicArrayHandle` on `firstheader` +0x20 and
`lastheader` +0x24, which it links a new handle onto through the handle's own
`next` +8 and `prev` +0xC.

`basicarray` +0x14 is a **pointer**, which the IDB gets wrong: its
`dds3WorldBasic_t` declares an inline `dds3BasicArray_t[10000]` there and so puts
`basicarraycnt` at +0x13894. `worldconstructor` stores one word at +0x14 and
`dds3SetBasicArray`, `dds3GetBasicArray` and `dds3DeleteAllBasicArray` all load
that word and add `8 * index` to it. `dds3BasicArray_t`'s own 8-byte stride and
its `next` +4 and `prev` +6 come from `dds3AddWorld`, which threads the free
chain through them and writes -1 into `prev` for slot 0 and into `next` for the
last slot.

`dds3BasicFunction_t`: `constructor` +0 (called by `dds3AllocBasic`, returning 1 on
success), `destructor` +4 (`dds3DeleteBasic`), `mainprocuder` +8 and
`drawprocuder` +12 (`dds3CallBasicMainProc`, `dds3CallBasicDrawProc`). The table is
reached through `dds3BasicFuncTablePtr[type]`, a pointer array, so nothing fixes
its stride: `reserve1`, `reserve2` and the 24-byte size are HD's and
**unconfirmed**.

### sdf_resource.h

One layer above `sdf_types.h` and on the same principle: the engine resource
records a packet builder needs, without the queue, file and motion owners that
come with `libsdf.h`. It holds `sdfMemHandle_t`, `sdfGsMemSlice_t`,
`sdfTexHandle_t`, `sdfMat_t`, `sdfDrawTag_t` and `TMX_HEADER_t`. `libsdf.h`
includes it, so each of these has exactly one definition in the tree; before
this existed, those six were copied into seventeen units by hand, and the
copies had drifted apart in both names and signedness.

Two declarations here are the local units' rather than the IDB's, because the
retail code decides and the IDB does not:

- `sdfDrawTag_t`'s `head`, `tail`, `z` and the three reference slots are `u32`,
  not typed pointers and an `f32`. `sdfDrawTagInit` writes `z = 0xFFFF` as an
  integer, which an `f32` member does not reproduce, and nothing in the tree
  reads them as pointers.
- `sdfGsMemSlice_t`'s `pixelFormat` is `s32`: `sdfSetLoadImage` loads it with
  `lw`. See [IDB corrections](../docs/idb-corrections.md).

`sdfMat_t`'s two uvw slots are `void *` on purpose. `libsdf.h` and the IDB
spell the pointee `struct sdfMatUvwParm_t`; `sdf3DPrim.c` and `effPCPMisc.c`
spell the same 24 bytes a union of three views. Nothing dereferences them
through a material, so the shared record stays agnostic and the two readings
can be reconciled separately.

### libsdf.h

The vector, matrix, DMA and GIF packet definitions live in the common-only
`sdf_types.h`, included by `libsdf.h`. Packet constructors can therefore
reuse their canonical layouts without importing the engine's unrelated owner
types. The packet header exposes the DMA tag's two 64-bit hardware words as
`dmaWords.tag` and `dmaWords.vif`, alongside the decoded `dmatag` fields.
The parallel-light packet exposes its three normal vectors as
`normalComponents[3][4]` for indexed access.

| struct | size | evidence |
|---|---|---|
| `sdfVector2_t` / `sdfVector3_t` / `sdfVector4_t` | 8 / 12 / 16 | sdfhook; quadword use throughout |
| `sdfMatrix4_t` | 64 | four quadwords, `dds3GetTMPoseSet` |
| `sdfColor_t` / `sdfColor32_t` | 16 / 4 | sdfhook |
| `sdfDmaTag_t` | 16 | PS2 DMAC tag; `sdfDrawTagLink` writes `id` +3 and `addr` +4 |
| `sdfGifTag_t` | 16 | sdfhook |
| `sdfGrTagStdReg_t` | 16 | sdfhook |
| `sdfGrPacketHeader_t` | 32 | sdfhook; `giftag` +16 |
| `sdfChipCB_tag` | 12 | `sdfChipAllocDirect` indexes `sdfChipCB` with `(i*2+i)*4` |
| `sdfChipPageInfo_tag` | 24 | `sdfChipInit` divides by `0x1018` (4096 + 24) and strides by 24 |
| `sdfMemHandle_t` | 16 | `sdfMemInit` calls `sdfChipAllocDirect(16)` |
| `sdfMemHandleCB_t` | 40 | `sdfMemCB` is 0x577F18 and the next global 0x577F40 |
| `sdfGsMemSlice_t` | 28 | `sdfGsMemInit` calls `sdfChipAllocDirect(28)` |
| `sdfTexHandle_t` | 64 | `sdfTexNew` calls `sdfChipAllocZeroDirect(64)` |
| `sdfTexPacket_t` | 64 | `sdfTexMakeGIFPacketSub` calls `sdfChipAllocDirect(64)` |
| `sdfDrawTag_t` | 32 | `sdfDrawTagInit` writes eight words, 0..0x1C |
| `sdfPreDrawHook_t` | 16 | sdfhook |
| `sdfPreDrawTag_t` | 16 | `sdfPreDrawTagInit` writes four words |
| `sdfModelPacketHeader_t` | 16 | `sdfModelNewPacketHeader` calls `sdfChipAllocDirect(0x10)` |
| `sdfMatUvwParm_t` | 24 | `sdfMatAllocTexUvwParm` calls `sdfChipAllocDirect(0x18)` |
| `sdfMat_t` | 72 | `sdfMatNew` calls `sdfChipAllocZeroDirect(0x48)` |
| `sdfMatPacket_t` | 160 | `sdfMatNew` calls `sdfChipAllocDirect(0xA0)` twice |
| `sdf3DLight_t` | 64 | four quadwords; consistent with `dds3ModelHandle_t`'s inline copy |
| `sdfItemArray_t` | 16 | `sdfItemArrayNew` calls `sdfChipAllocDirect(16)` |
| `sdfGrOt_t` | 32 | `sdfGrOtInit` strides the table it initialises by 0x20 |

`sdfMemHandle_t`: `ptr` +8 (`sdfMemGetPtr`), `stat` +12 as `u16` (`sdfMemGetType`
uses `lhu`), `mlock` +14 as `u16` (`sdfMemLock` increments it with `lhu`/`sh`).
A block's size is the next handle's `ptr` minus its own, which `sdfMemGetSize`
computes, so `prev` +0 and `next` +4 are fixed too.

`sdfMemHandleCB_t` is confirmed field by field by `sdfMemInit`, which writes both
embedded guard handles (+0/4/8/0xC/0xE and +0x10/0x14/0x18/0x1C/0x1E, each with
`stat` 2 and `mlock` -1), `allocPtr` +0x20 and `allocSize` +0x24, and hands the
first real handle `&sdfMemCB.tailGuard` as its `next`. The guards are inline
here; the HD build holds pointers to them. `enum sdf_mem_stat` is from
`sdfMemAllocDirect`, which takes the first handle whose `stat` is 0, stops at the
one whose `stat` is 2, and stamps the one it took 1, and from
`sdfMemFreeDirect`, which writes 0 back.

`sdfTexHandle_t`: `prev`/`next` from `sdfTexLink`; `width` +12 and `height` +14 as
`s16`, `pixelGsHandle` +16, `clutGsHandle` +20, `numOfCLUT` +24, `CLUTFormat` +25,
`pixelFormat` +26, `numOfMipmap` +27 and `clutColor` +56 all from `sdfTexNew`'s
stores at the matching widths; `packet` +40 from `sdfTexGetPacket`, `numOfCLUT`
again from `sdfTexGetCLUTCount`'s `lbu`. Six fields in the middle are IDB names at
IDB positions with no load or store found for them, and are marked `unconfirmed`
in the header; the 64-byte size bounds them.

`sdfTexSetMonoClut` pins the last three. It reads `clutData` +48 as the source,
divides `clutDataSize` +52 by 4 or 2 depending on `CLUTFormat` +25 to get the
entry count, then loads `monoClutData` +60 as a word, allocates it with
`sdfAllocAuto` if it is null, stores it back, and fills it with one `sb` per
entry. So `monoClutData` is a `u8 *` to a lazily built intensity table, not the
`u8` the IDB has; the struct is 64 bytes either way, since the pointer exactly
replaces the old field plus its three bytes of tail padding.

`sdfMat_t`: `duty` +6, `packet[2]` +8, `ambient` +16, `diffuse` +20,
`multiTexDiffuse` +24, `fdBias` +64 and `fdSoften` +68 from `sdfMatNew`;
`uvwParm` +56 and `multiTexUvwParm` +60 from the four pointers `sdfMatFree`
releases (+8, +12, +56, +60). The struct ends at 72.

`sdfItemArray_t` and `sdfGrOt_t` are here rather than in `model.h` because the
model, motion and billboard paths all three end in them. Their fields are
covered under `model.h` below.

`sdfDrawTag_t.z` is the one field type here taken from the IDB rather than from the
code: `sdfDrawTagInit` sets it to `0xFFFF` with an integer `sw`, which reads more
like a maximum sort key than a float. sdfhook also calls it `f32`, so `f32` stands,
flagged in the header.

#### Disagreements with sdfhook

Two, both resolved in favour of the retail code:

- `sdfMat_t`: sdfhook has a trailing `reserved[88]` and asserts 160 bytes.
  `sdfMatNew` allocates `0x48` for it, and `sdfMatFree` releases nothing past +60,
  so the struct is 72 and the `reserved` array is not there. Every sdfhook field
  offset from +0 to +68 agrees with this header.
- `sdfMatPacket_t`: sdfhook asserts 128 bytes, ending at `texTm[6]`.
  `sdfMatNew` allocates `0xA0` for each of the two packets and writes VIF codes at
  +0x98 and +0x9C, past 128. The packet is 160; the extra 32 bytes are an unnamed
  `reserved` tail here, and sdfhook's named fields are unchanged.

Neither would show up in sdfhook's frame comparisons: it only reads a material and
its packet, never sizes or frees them.

### scr.h

| struct | size | evidence |
|---|---|---|
| `scrHeader_t` | 32 | the type table begins at file offset 32 |
| `scrTypeTable_t` | 16 | `fldFlowParse` strides the table by 16 |
| `scrProcTable_t` | 32 | `scrGetProcedureCodeAddress` strides by 32 |
| `scrLabelTable_t` | 32 | `scrGetLabelCodeAddress` strides by 32 |
| `scrCode_u` | 4 | `scrTraceCode` indexes `Code` by `CP * 4` |
| `scrStackData_u` | 4 | `scrGetIntPara` indexes `StackData` by `4 * i` |
| `scrCommandTable_t` | 8 | `CodeFunc_Comm` strides `scrCommandTable` by 8 |
| `scrProcessWork_s` | 244 | `scrPrintProcedureStart` calls `sdfChipAllocDirect(0xF4)` |

`scrProcessWork_s` is confirmed field by field. `scrPrintProcedureStart` is the
real constructor despite its name: it allocates 244 bytes, copies the procedure
name into `Name[24]` at 0, seeds `CP` +24 from the proc table entry's
`CodeAddress`, zeroes `SP` +28, clears `StackType[28]` from +32 and
`StackData[28]` from +60 in steps of 4, then stores `Header` +172, `TypeTable`
+176, `ProcTable` +180, `LabelTable` +184, `Code` +188, `MessageData` +192,
`StringData` +196, `StartProc` +200, `MesHandle` +204 and zeroes +208 through
+236. `LocalInt` +220 and `LocalFloat` +224 are allocated as
`LocalIntCount * 4` and `LocalFloatCount * 4`. `data` +240 is the only field it
leaves alone, and `scrSetUserData` writes it.

The accessors agree independently: `scrGetCodePointer` +24, `scrGetMessageHandle`
+204, `scrGetScriptTimer` +208, `scrGetCommandTimer` +212, `scrGetData` +240,
`scrLoadScriptFile` +216, `scrLinkHandle` +232/+236, `scrStartScriptProcess`
+200/+228. `scrSetIntReturnValue` writes `StackData[27]` (+168) and tags
`StackType[27]` (+59), which is how both arrays are known to be 28 long.

The PS2 struct has no `ScriptTimerF` or `CommandTimerF`; HD's two extra floats are
what put its field order eight bytes out.

`scrHeader_t`: `MagicCode` +8 is the four bytes `fldFlowParse` compares against
`0x30574C46` ("FLW0"), `TypeTableCount` +16 is its loop bound, and
`LocalIntCount` +20 and `LocalFloatCount` +22 are the two `lh` reads
`scrPrintProcedureStart` sizes the local arrays from, so both are signed: the
`blez` guard in front of each allocation only comes out of an `s16`. The remaining fields are HD
names in HD order, bracketed by those three anchors and the 32-byte size.

`scrCommandFunc_t` takes no arguments and returns `s32`: `CodeFunc_Comm` calls the
entry's `comm` without touching `$a0`, and `scrCommand_BIT_ON` reads its argument
with `scrGetIntPara` and returns 1.

### newdata.h

`dds3GlobalWork_t` is a partial view of `DDS3_GBWK`, shared by the interpreter
and gameplay data helpers, not a claim about the allocation's complete size.
`fldMainLoadStart` reads the signed transition halfword at +0xE before
choosing between the return sequence and initial field loading. This fills
part of the initial padding without moving the variable banks.
`PopInt`, `PopFloat` and the global-variable opcodes address the integer bank
at +0x40 and float bank at +0x440. `dds3GlobalWorkClearFlags` clears both banks
in one forward pass. `datAddMaka` and `datCheckMaka` access the signed currency
balance at +0xA48. `fclClearTerminalState` clears the encounter step remainder
at +0x1CF4 and the halfword encounter gauge at +0x1CF8. `slSetTmnlID` writes
the two terminal-preview halfwords at +0x11A78/+0x11A7A. Event flags and moon
state occupy the previously opaque +0x840..+0xA47 region (described below).
These are one shared view, not parallel save-work prefixes; other gaps remain
opaque.

The upstream field helpers extend that same view with `maFlags` +0xA4C,
sixteen 0xEC-byte digging-party records at +0xA50, 48 field-flag blocks at
+0x6658, and treasure-open bits at +0x11A58/+0x11A80. Each field-flag row is
0x1E bytes. `datUnitVisual_t` combines the packet helpers' scalar prefix with
the action helpers' eight 0x1C-byte part records at +0x32 and native 0x310
stride. `datUnitWork_t` retains the verified byte/halfword name and skill
storage: `nowcommand` is at +0x64, not the later offset implied by unused
expanded donor fields. Inherited-skill helpers read and write 24 halfwords at
+0x86 through `keisyoskill`, rather than appending another skill array after
the already complete 0xEC record. That field and the `keiattr` byte array at
+0xB6 are named from the IDB, whose field order for the rest of the struct
does not describe this ELF; see [IDB corrections](../docs/idb-corrections.md).

### model.h

The node tree a loaded model file becomes, plus the shape headers its draw
packets are built from.

| struct | size | evidence |
|---|---|---|
| `sdfModelBounds_t` | 24 | `sdfModelCheckBounds` reads `min[3]` at 0 and `max[3]` at 12 |
| `sdfModelOt_t` | 16 | `sdfModelOtInit` writes four pointers, 0..0xC |
| `sdfModelNodeList_t` | 16 | `sdfItemArrayNew` calls `sdfChipAllocDirect(16)` |
| `sdfModelTexList_t` | 16 | same; `sdfTexListNew` passes `elemSize` 4 |
| `sdfMatList_t` | 16 | same; `sdfMatListNew` passes `elemSize` 4 |
| `sdfModelMorphList_t` | 16 | same; `sdfModelMakeMorphList` passes `elemSize` 16 |
| `sdfModelMorphParm_t` | 8 | `sdfModelMakeMorphList` zeroes +0 and +4 per target |
| `sdfModelMorph_t` | 16 | its inner loop runs twice in steps of 8, per 16-byte element |
| `sdfModelRootNode_t` | 152 | `sdfModelRootNew` calls `sdfChipAllocZeroDirect(0x98)` |
| `sdfModelNode_t` | 256 | `sdfModelNodeNew` calls `sdfChipAllocZeroDirect(0x100)` |
| `sdfModelNodeDef_t` | 80 | `sdfModelMakeRoot` strides the def array by 0x50 |
| `sdfModelDef_t` | 16 | `sdfModelMakeRoot` reads +0, +4, +8 and +0xC and nothing beyond |
| `sdfModelShapeEntry_t` | 8 | `sdfModelLinkShapedDrawPacket` indexes `list + 8 + i*8` |
| `sdfModelShapeList_t` | 16 | one entry past the 8-byte header |
| `sdfShapeHeader_t` | 8 | `sdfModelGetDrawPacketSize` reads the count at 0 and strides the pointers by 4 from +4 |
| `sdfShapePacket_t` | 12 | `sdfModelSetNormalDrawPacket` reads +0, +4 and +8 |
| `sdfShapeShapedPacket_t` | 16 | `sdfModelLinkShapedDrawPacket` reads +4, +8 and the pointers from +0xC |
| `sdfModelDrawParmPacket_t` | 48 | `sdfModelLinkOt` writes every word of it inside a 64-byte buffer |
| `sdfModelNodeDrawPacket_t` | 64 | `sdfModelLinkOt` calls `sdfGrAllocPacketBuff(0x40)` |
| `sdfModelNormalDrawPacket_t` | 32 | `sdfModelSetNormalDrawPacket` writes two 16-byte DMA tags |

`sdfItemArray_t` is the one list type the engine actually has;
`sdfItemArrayNew(count, elemSize, growStep)` allocates the 16-byte header and
`sdfItemArrayGrow` pins every field: `memHandle` +0, `capacity` +6 (`lhu`),
`elemSize` +8 (`lh`), `growStep` +10 (`lh`), `items` +12. `count` +4 is the `sh`
`sdfModelNodeAdd` and `sdfModelMakeMorphList` write. The five per-element lists
repeat those six fields so `items` carries its type, as the engine's own headers
do.

`sdfModelRootNode_t`: `nodeList` +0 and `headNode` +4 from `sdfModelRootNew` and
`sdfModelNodeUnlink`; `texList` +8 and `matList` +0xC and `extraData` +0x90 from
`sdfModelMakeRoot`; `morphList` +0x10 from `sdfModelMakeMorphList`; `motion`
+0x14 from `sdfMotionCtrlrNew`, which chains the new controller onto whatever
was there; `flags` +0x19 as `lbu`/`sb` and `drawAttr` +0x1A as `lhu` and `color`
+0x1C from `sdfModelLinkOt`; `tm` +0x20 as four `sq`, `rotate` +0x60 and `scale`
+0x70 as one `sqc2` each, all from `sdfModelRootNew`; `light` +0x80, `fog` +0x84,
`faceDivSoften` +0x88, `faceDivBias` +0x8C and `zBias` +0x94 from
`sdfModelLinkOt`.

`sdfModelNode_t`: `prev` +0 and `next` +4 are fixed by `sdfModelNodeSetParent`,
which inserts a node between `children` and `children->+0` and leaves the
sibling chain in file order when walked through +4; `parent` +8, `children` +0xC
and `root` +0x10 from the same function and `sdfModelNodeUnlink`; `flags` +0x14
(`lhu`, tested against 0x11) and `drawAttr` +0x16 and `color` +0x1C from
`sdfModelLinkOt`; `id` +0x18, `bounds` +0x34 and `shape` +0x38 from
`sdfModelNodeBaseSet`; `drawPacket[2]` +0x28 from `sdfModelPacketFree`, which
walks two chains four bytes apart, and from `sdfModelNewPacketHeader`;
`tmPacket` +0x30 from `sdfModelAllocTmPacket`, which allocates 0x100 for it, and
from `sdfModelEvalNode`, which indexes it by `frameId * 128`; `rotate` +0x50 (a
quaternion, from `FromEuler`), `pos` +0x60 and `scale` +0x70 from
`sdfModelNodeBaseSet`; `localNoScaledTm` +0x80 and `tm` +0xC0 from
`sdfModelEvalNode`, which multiplies the first three rows of one by the scale
and the parent matrix and stores four rows into the other.

`sdfModelNodeDef_t`: `id` +8, `rotate` +0x10 (three floats fed to `FromEuler`),
`pos` +0x20, `scale` +0x30 and `bounds` +0x40 from `sdfModelNodeBaseSet`;
`parentIndex` +0xC from `sdfModelMakeRoot`, which indexes `nodeList->items` by
it and treats negative as no parent; `shape[3]` +0x44 from `sdfModelNodeSet`,
which walks three pointers from there for format 0 and treats the first as a
null-terminated pointer array for format 1.

The three shape header structs are this repo's names: neither source names
them, and the HD metadata's `sdfShapeClass` folds the list header and one
packet into a single 32-byte struct that matches neither. The field names are
`sdfShapeClass`'s. `sdfShapePacket_t` is byte-identical to sdfhook's
`sdfModelTempPacket_t`, which is the same type reached through the root's
temporary packet list.

The DDS3 model layer now shares its owner/resource layouts between
`model/mdlManager.c` and the PIB effect interpreter in `code_001DEA58.c`:
`mdlObj_t`, `mdlResrc_t`, `mdlResrcSlot_t`, `mdlDevSlot_t`, `mdlDevList_t`
and `mdlEffectResrc_t`. `mdlCreateEffectCtrlr` allocates a controller, stores
the old owner head at +0, the kind at +4 and motion slot at +6, and links it
through `mdlObj_t.resourceItems` +0x14. Its payload is a union: anchored
billboards/effects keep the instance, part slot, MPOS record and offset scale;
IPU attachments keep the owner, stream object, texture and deferred-start
parameters. `mdlPibGetChunk` reads the group's PIB pointer at +0xA4;
`mdlGetEffectResrc` reads the part-list pointer at +0xA8. That part list uses
the existing `sdfItemArray_t` header, not a texture-specific list.
`mdlResrc_t.defaultScale` at +0xAC is initialized to 1 by resource
registration and copied into the new model's three scale components by
`mdlLoad`; it is not an unidentified resource word.

The event controls share `newdata.h`'s `dds3GlobalWork_t` prefix:
`evtBitAllOff` clears the event-word bank at +0x840, and the moon controls
access flags at +0xA40, the sixteen-phase counter at +0xA41 and its float
timer at +0xA44. No layout beyond these accessed fields is claimed.

The event units reuse those model layouts. Event motion reads the owner's SDK root
at +0x18, first controller at +0x1C and controller slots at +0x20; the existing
`sdfModelRootNode_t` and `sdfMotionCtrlr_t` describe those pointees.

### event.h

`evtCreateUnitHandle` allocates and clears **0x148 bytes** for `evtUnit_t`.
The paired DDS work is larger: SMT3 has no word before `pathHandle` +0x9C,
no gap before `motionSetup` +0xBE, and no DDS shadow/reference-angle or
vertical-movement tail. Native setters and command handlers fix `flags`
+0xA4, `motionState` +0xA8, `slotFlags` +0xCC, `tableValues` +0xD8,
colour duration +0x138 and light-target transition counters +0x144/+0x146.
The model owner, basic node and motion controllers reuse their existing
headers. `basic.h`'s `dds3UnitObjectWork_t` describes the event-unit pointer at
payload +8, control pointer +0xC, linked-light ID +0x10 and ordering table +0x14.

`evtWorldSourceTransformPrefix_t` is the position/quaternion prefix of the
scene's type-17 source records, shared by stage placement, event motion and
script effect placement. `basic.h` supplies the complete, 0xAC-byte
`dds3ObjectBaseControl_t`, including its control-basic array at +0x10 and mover
in slot 1. Stage, script and field consumers use that same layout rather than
independent prefix types. Path consumers likewise share the 36-byte
`dds3PathBasicControl_t` and its typed channel-key union; `mode` is at +0 and
`pSplineHandle` at +0x10. Their public declarations live beside the types. The
serialized scene texture-offset table remains an opaque shared pointer type in
`libsdf.h`; these ports only forward it.

The upstream native path implementation supplies the canonical transform
channel: quaternion +0, XYZ position +0x10 and XYZ scale +0x1C, in 0x28-byte
samples. `dds3PathKeyframes_t.data.transform` and the cursor's `transformKeys`
name that channel; the cursor and channel types are shared with the retained
port consumers rather than declared again in the upstream unit.

### Shared DDS port layouts

`effect.h` owns the native blur payloads shared by the renderer, event setup
and polygon-movie blending: `effBlurQuad_t` (0x28), scale/scatter parameter
blocks (0x2C), their slot/allocation owners, the texture-backed template body
(0x2C), resource-rectangle parameters (0x24) and solid-rectangle parameters
(0x18). These sizes follow the native copy counts and allocation arithmetic.
Packed RGBA and byte-channel access share one word; position pairs and 2x2
corners retain the native indexing used by the movie blender.

The same header owns the effect RNG's four words, retained slot-array header
`effArrHdr_t`, cubic XYZ segment `effCubicBezierSlot_t` (0x38), and seven-point
`effBezierCurve_t` (0x60). The latter is the curve inside a moving ribbon
slot, not the ribbon's larger 0x80-byte `effBezierSlot_t`. Particle dispatch
and thunder effects share `parCell_t`, `parSystem_t` and `sdfVector3_t`; their
allocation and material pointers use the existing SDF types.

`billboard.h` owns the 0x1C-byte render pair and 0x64-byte `billObj_t`, shared
by the billboard manager and particles. The pair begins at +0x34 and
`animationActive` at +0x50. Material, texture and heap layouts in the thunder
and scatter paths reuse `sdfMat_t`, `sdfMatPacket_t`, `sdfTexHandle_t` and
`sdfMemHandle_t`, rather than retaining donor-only copies.

`nb.h` owns the shared `nbFormation_t`, `nbMainProcessData_t` and
`nbKoukaProcessData_t` layouts. Main work includes the action-stack count at
+0x40, camera process at +0xFC, encounter process at +0x114 and event-pack/memory
pointers at +0x138/+0x13C. These remain accessed fields, not a complete
main-work size claim. The formation's motion-specific unions describe actual
reuse of the loading/target words for damage and shake state.

`libsdf.h` owns the shared eight-byte thread registration node `sdfThCB_t`,
sixteen-byte SIF command and 0x40-byte native texture-file header
`TMX_HEADER_t`. These use the recovered names recorded in
[type recovery](../docs/type-recovery.md); particle axes likewise reuse
`sdfVector3_t` rather than a RenderWare-named duplicate. Texture clamp mode is
the byte at +0x1B and resource key the word at +0x1C. PAC packet words formerly
called padding are named `kind` (+2) and `tag` (+8) for the field/model
consumers that interpret them. The shared `sdfFileCmd_t` prefix combines the
queue's path pointer at +0x10 with the native device helpers' status byte at
+0x16, file descriptor at +0x28 and error word at +0x2C. This is an accessed
prefix, not a complete file-command allocation-size claim.
It remains distinct from the complete 0x40-byte `sdfFileCmdHandle_t` owner:
a matching prefix is not evidence that the two allocation sizes are equal.

`field.h` owns the 0x0C-byte filename entry and 0x24-byte file-resource row
shared by the resolver and field loader. Its single `fldAreaWork_t` view of
`fldGlobalBase` combines the area/floor (+4/+8), consumed flags (+0x14), mode
(+0x18), requested scene name (+0x20), resource selection (+0x84..+0x8C),
sequence code (+0xA0), reuse flag (+0xA8), light-spell index (+0xB8) and XYZ
position (+0x100..+0x108).
Model and field loading share `sdfPacState_t`/`sdfPacWork_t` rather than
declaring private copies; the state remains the native 0x38 bytes that
`sdfPacInit` clears.

The emitted macros in `ps2asm.h` are canonical across these ports. In
particular, particle code reuses the vf12 load, vf10/vf12 add, secondary-bank
load and unclobbered vf10 store. The battle camera reuses
`SDF_VU0_COPYMATRIX_A_TO_B`. Memory-operand and memory-clobber variants are
intentionally distinct compiler contracts, even when their EE opcode is the
same; changing those contracts can change surrounding C code generation.
Upstream's specialized unit-local VU0 sequences retain their exact operand
constraints and clobbers during header reconciliation.

### motion.h

| struct | size | evidence |
|---|---|---|
| `sdfMotionCtrlr_t` | 52 | `sdfMotionCtrlrNew` calls `sdfChipAllocZeroDirect(0x34)` |
| `sdfMotionNode_t` | 12 | `sdfMotionNodeBaseInit` writes +0 and +4; `sdfMotGetKey` reads +8 |
| `sdfMotionNodeVTBL_t` | 20 | five slots called at +0, +4, +8, +0xC and +0x10 |
| `sdfMotionNodeDef_t` | 8 | `sdfMotionCtrlrNew` strides the def array by 8 |
| `sdfMotionNodeList_t` | 16 | `sdfItemArrayNew`, `elemSize` 4 |
| `sdfMotionPlayBook_t` | 16 | the node defs start at +8, so one def past the header |
| `sdfMotionCutDef_t` | 16 | the chunk stream starts at +4 |
| `sdfMotNodeModelBase_t` | 16 | `sdfMotGenModelInit` writes the model node at +0xC |
| `sdfMotNodeMatBase_t` | 16 | `sdfMotGenMatInit` writes the material at +0xC |
| `sdfMotNodeModelVec_t` | 32 | `sdfMotGenModelLinerPos` calls `sdfChipAllocDirect(0x20)` |
| `sdfMotGetKeyIf_t` | 12 | `sdfMotGetKey` writes +0, +4 and +8 |

`sdfMotionCtrlr_t`: `next` +0 and `rootNode` +4 and `playbook` +8 and `nodeList`
+0x10 and `speed` +0x20 from `sdfMotionCtrlrNew`; `blendLength` +0x14,
`blendPreLength` +0x18, `frame` +0x1C, `cutNumber` +0x2C, `frameLength` +0x2E,
`stat` +0x30 and `playType` +0x32 from `sdfMotionChangeBlend`;
`statBeforePause` +0x31 from `sdfMotionPause`, which saves `stat` into it;
`speed` +0x20 again from `mdlSetMotionSpeedEx`; `userId` +0x28 and `userId2`
+0x2A from `mdlCreateMotion`. `playbookMemHandle` +0x0C and `userData` +0x24 are
HD names at HD positions with no access found; the 52-byte size pins them.

`sdfMotionPlayBook_t`: `numOfNode` +2 is the `lhu` `sdfMotionCtrlrNew` sizes the
node list from, and the node defs it then walks start at +8, so `nodeDefList` is
inline, not a pointer. `cutList` +4 is indexed by cut number with a stride of 4
and the result dereferenced, so it is an array of pointers.

`sdfMotionCutChunk_t`: `size` +0 is what `sdfMotionChangeBlend` advances the
per-node chunk pointer by; `numOfKey` +4 and `keyElemSize` +6 and the inline
`keyFrameList` at +8 are what `sdfMotGetKey` binary-searches and then indexes,
taking the key data to start after the frame list rounded up to a word.

The channel-specific payloads used only by `sdfMotion.c` stay local there:
visibility and material-value nodes allocate 20 bytes, UVW nodes 36 bytes,
and morph nodes 32 bytes. All begin with the existing typed node/target
prefix; their captured blend state starts at +0x10. UVW capture copies only
the five meaningful floats (20 bytes), not the material parameter's padding.
Morph capture preserves both 8-byte target/weight pairs.

The packet decoder's +0x24 pointer in `libsdf.h` has both a typed `decomp`
view and a byte-buffer `decompBuffer` view. The latter preserves the native
aliasing relationship with the texture output cursor when installing a
decoder; neither the 56-byte state layout nor ownership changes.

### Unit-local stream and packet views

The movie-stream records stay in `sdfMovie.c`: `movieOpenStream` clears the
complete 0xB0-byte owner, with its inline 0x8C-byte IPU task at +0x24. Its
linear work allocation is 0x14 bytes; PAC work is 0x78 bytes and owns separate
block-mask and ring-buffer allocations. `movieParseHeader` retains the
GCC nested `movieMemcpy` helper found in the DDS1 twin: the retail helper
saves the static-chain argument at sp+0, even though it only calls `memcpy`.

Development-console DMA state stays in `sdfDevCons.c`: a 32-byte alternating
buffer record, a 96-byte formatted-packet setup header, and 40-byte glyph
register tuples. These are actual GS 64-bit register payloads. The UV table
contains packed 32-bit coordinate pairs; its existing public getter exposes
the same data as halfwords. `sdfDevConsDrawAll` and its DDS1 twin reserve the
same 0x1040-byte stack allocation around the central 0x1000-byte DMA region.
Formatted printing consumes a separate sixteen-byte control record: signed
X/Y/Z words followed by packed RGBA. Its local typed view describes that
record without changing the public entry point's `u32 *` parameter.

Other single-unit views remain next to their users: the file manager's
shared request prefix and differently typed callback/queue links; the
memory-card control stride; the texture-upload request; and the model
manager's directory entries, 20-byte PIB records and inline PAC headers.
They do not introduce alternate definitions of the shared model, material
or motion owners.


### billboard.h

| struct | size | evidence |
|---|---|---|
| `UV_t` | 4 | `sdfBillBoardMake1` copies four of them in 4-byte steps |
| `VTX_t` | 8 | the same call reads eight floats as four pairs |
| `Sprite3D_t` | 72 | `drawSprite3D` reads +0, +0x10, +0x20, +0x40 and +0x44 |
| `sdfBillBoardBatch_t` | 1024 | `BillboardLoadTex` allocates 1100 and puts the resource at +1024 |
| `dds3BillResource_tag` | 76 | the remaining 76 bytes of that allocation |
| `sdfBillBoardTMPacket_t` | 56 | `drawSprite3D` calls `sdfGrAllocPacketBuff(0x38)` |
| `sdfBillBoard1Packet_t` | 128 | `sdfBillBoardGetPacketSize(n)` is `64n + 64` |
| `sdfBillBoardVc1Packet_t` | 144 | `sdfBillBoardVcGetPacketSize(n)` is `(76n + 75) & ~15` |
| `sdfMultiBillBoard1Packet_t` | 144 | `sdfMultiBillBoardGetPacketSize(n)` is `(84n + 75) & ~15` |
| `sdfMultiBillBoardVc1Packet_t` | 176 | `sdfMultiBillBoardVcGetPacketSize(n)` is `(108n + 75) & ~15` |

All four packet builders write every field of their packet, so every offset
above comes from a store: `sdfBillBoardMakeTMPacket`, `sdfBillBoardMake1`,
`sdfBillBoardVcMake1`, `sdfMultiBillBoardMake1` and
`sdfMultiBillBoardVcMake1`. Nothing in these four structs is unconfirmed.

`dds3BillResource_tag` is confirmed field by field twice over.
`BillboardLoadTex` writes `hTex` +0, `type` +4, `count` +8, `uv[4]` +0xC, `cx`
+0x1C, `cy` +0x20, `width` +0x24, `height` +0x28, `pBillTag[4]` +0x2C..0x3B,
`current_mode` +0x3C, `hMem` +0x40, `batch` +0x44 and `next` +0x48 (to itself).
`dds3BillboardLinkOt` reads `batch` +0x44, indexes `pBillTag` by
`current_mode`, flushes all four tags and walks `next`.

`sdfBillBoardBatch_t`: `dds3BillboardLinkOt` hands
`sdfBillBoardMakeN(batch, batch+240, batch+300, batch+540, batch->count)`, which
is `centre[15]` at 0, `color[15]` at 0xF0, `uv[15][4]` at 0x12C,
`offset[15][4]` at 0x21C and `count` at 0x3FC, ending the 1024 bytes exactly.

The HD metadata keeps `dds3BillResource_tag` in `effect.h` and `Sprite3D_t` in
`field.h`; they are together here because one allocation holds both halves of
the first and `drawSprite3D` is the only thing that reads the second. The
`sdfModel*` and `sdfMotion*` types live in `model.h` and `motion.h` rather than
in HD's single `libsdf.h`, since that file is 1936 lines of everything.

#### Disagreements with sdfhook

Five, all resolved in favour of the retail code.

- `sdfModelRootNode_t`: sdfhook asserts 168 bytes and adds `modelobj` +0x98,
  `oData` +0x9C, `effpos_type` +0xA0 and `reversePB` +0xA4 after `zBias`.
  `sdfModelRootNew` allocates `0x98`, so the struct ends at `zBias` and those
  four fields are the HD build's. Every sdfhook offset from +0 to +0x94 agrees
  with this header.
- `sdfModelNodeDef_t.bounds`: sdfhook has `f32 bounds` at +0x40.
  `sdfModelNodeBaseSet` copies it with `lw`/`sw` into `sdfModelNode_t` +0x34,
  and `sdfModelLinkOt` passes that straight to `sdfModelCheckBounds`, which
  dereferences it as six floats. It is a `sdfModelBounds_t *`. sdfhook has the
  node's copy right and the def's wrong, which is consistent: it only ever
  reads the node.
- `sdfShape_t`: sdfhook asserts 152 bytes with `vertexList`, `normalList`,
  `uvList[2]` and `faceNormalList` as inline vectors rather than pointers, which
  is the IDB's bulk-import defect this document already describes for
  `scrProcessWork_s`. It is not ported, and not because of that: no `sdfShape*`
  function exists in the binary, no IDB prototype names the type, and its
  fields have no cross-references at all. The runtime reads geometry through
  `sdfShapeHeader_t` and the three packet structs above; `sdfShape_t` is the
  authoring-side mesh container and does not appear in this build.
- `sdfModelShapeList_t.next`/`count`: sdfhook names them as a list link and a
  count, which is right about the layout but understates them.
  `sdfModelLinkShapedDrawPacket` turns the pair into one more DMA REF, `count`
  as its qwc and `next` as its address, after the per-target entries. The names
  are kept; the comment says what they do.
- `sdfModelNodeDrawPacket_t.tmRef`: sdfhook types it as its own
  `sdfModelTmRefPacket_t` alias of `sdfDmaTag_t`. It is spelled `sdfDmaTag_t`
  here; nothing about the layout differs.

Where sdfhook has no body at all, the HD metadata was used and then checked:
`sdfMotionCtrlr_t`'s HD field order gives exactly the 52 bytes
`sdfMotionCtrlrNew` allocates, and every field but two was found in code.

#### Disagreements with the HD reconstruction

Worth recording because they are new failure modes beyond the ones listed at
the top of this document:

- **A list became a different list.** `sdfModelNodeList_t`,
  `sdfMatList_t`, `sdfModelTexList_t` and `sdfMotionNodeList_t` all have
  `items` as a pointer to the element in HD; `elemSize` is 4 at every creation
  site, so they are arrays of pointers. `sdfModelMorphList_t` gains a bogus
  inline `items2` in HD, taking it to 20 bytes; it is a plain 16-byte
  `sdfItemArray_t`.
- **An inline array became a pointer, again.** `sdfMotionPlayBook_t.nodeDefList`
  and `sdfMotionCutChunk_t.keyFrameList` are both inline in the file format.
  HD also inserts a 4-byte `pad` into `sdfMotionCutChunk_t` that is not there.
- **A struct grew the whole file header.** HD's `sdfModelDef_t` has seven fields
  and the IDB reports 104 bytes. `sdfModelMakeRoot` reads four words and
  nothing else, so it is 16 bytes: the node-def block, the material-def block,
  the morph count and `extraData`.
- **An array became a scalar.** HD's `sdfModelNode_t.drawPacket` is one pointer.
  `sdfModelPacketFree` walks two chains four bytes apart and
  `sdfModelNewPacketHeader` links into `node + aId*4 + 0x28`, so there are two,
  one per frame buffer.
- **A pointer array became an inline array.** HD's `sdfModelMorph_t` is
  `sdfModelMorphParm_t *target[2]`, 8 bytes. `sdfModelMakeMorphList` writes two
  8-byte pairs per 16-byte element, so the targets are inline.

### effBattle.h

The BED bank, cut and eight-byte aim selector layouts shared by the battle
and event-effect units. `effBTLCreate` allocates `0xC44` bytes: two `0x618`-byte
banks, eight retained bytes, the allocation handle, active-cut count and list
head. `effBTLCutCreate` allocates and clears `0x120` bytes. Its frame counter
is at `+0x08`, part handles start at `+0x20`, callbacks are at `+0x110` and
`+0x114`, user data at `+0x118`, and the next-cut link at `+0x11C`.
These are SMT3 offsets, not DDS's eight-byte-larger cut layout.
`effSolidRectParams_t` is also shared here: packed color, blend control and
four corner coordinates form the `0x18`-byte filter record consumed by both
the event controls and the battle field-filter draw path.

`effect/effBattleMisc.c` keeps a local formation-extent view with radius and
height at `+0x50`/`+0x54` and uniform scale at `+0x5C`; no full formation size
is asserted. Despite its recovered name, `effBTLPartGetScalePtrSub` returns
an integer frame duration from `D_00317740` (0, 5, 15 or 30), not a pointer.
Point-light and model-color callers compare and subtract these durations.

Event-only blur, smoke-particle and Bezier-emitter owners remain local to
`effect/effEvent.c`. Their parameter prefixes are copied independently of
the retained texture/allocation handles; the packed BED record copy preserves
unaligned `0x30`-byte source and destination records. The unmatched batched
smoke renderer candidate is retained in `nonmatching/effEVECombParSmokeDraw.c`:
its residual starts at ordering-table address commoning, not particle math.
The native renderer also proves `sdfGrOt_t.regFunc` takes the ordering-table
slot and a draw-tag pointer; `libsdf.h` now declares that callback signature.

### draw.h

The GS side: the frame buffers, the three-slot draw queue, the draw environment,
and the A+D register packets the `sdfPrim` and `sdf2D` builders file into an
ordering table. Every one of these is a packet whose first 32 bytes are
`sdfGrPacketHeader_t` (`sdf_types.h`, also included by `libsdf.h`), written by
`sdfGrSetRegsPacketHeader`: the DMA tag's `qwc` is the register count plus one,
its `dat1` is a VIF DIRECT of
the same length, and the GIF tag's NLOOP is the register count with REGS 0xE,
A+D. That one function is what makes each packet's size `32 + 16 * regs`.

`sdfGrTagStdReg_t` is the native pair of `u64 data` and `u64 reg` fields.
The clear packet alone exposes its RGBAQ low word as `clearColor`:
`dds3InitRenderDrawEnv` at `0x0010493C` replaces that word (draw-tag offset
`0x120`), retaining Q and the register address. This partial view belongs to
the clear packet, not to every A+D register write. Keeping the shared register
record scalar also preserves upstream's matched packet constructors.

| struct | size | evidence |
|---|---|---|
| `sdfGrScreenDef_t` | 12 | `sdfGrSetFrameSetMode` indexes `sdfGrFrameModes` by `mode * 12` |
| `sdfGrFrameSet_t` | 20 | fields through `zbuffHandle` +0x10; `sdfGrFreeFrameSet` frees the last one |
| `sdfGrDrawState_t` | 16 | `sdfGrReqDraw` and `sdfDrawSync` index `sdfGrDrawState` with `sll 4` |
| `sdfGrDrawDefParmPacket_t` | 64 | `sdfGrSetDrawDefParmPacket` writes four A+D pairs |
| `sdfGrDrawFramePacket_t` | 64 | `sdfGrSetDrawFramePacket` writes four |
| `sdfGrClearPacket_t` | 80 | `sdfGrSetClearPacket` writes five |
| `sdfGrDrawEnv_t` | 368 | `sdfGrSetDrawEnv` passes 21 registers, and writes to +0x168 |
| `sdf2DTexPacket_t` | 80 | `sdf2DGetTexPacketSize` returns 0x50 |
| `sdfPrimTransElem_t` | 32 | `sdfPrimSetOpaqueElem` writes two A+D pairs |
| `sdfPrimTransPacket_t` | 64 | `sdfPrimSetOpaquePacket` writes a two-register header at +0 and the elem at +0x20 |
| `sdfPrimTexflushElem_t` | 16 | `sdfPrimSetTexflushElem` writes one pair |
| `sdfPrimTexflushPacket_t` | 48 | `sdfPrimSetTexflushPacket`, one register |
| `sdfPrimBitBltElem_t` | 64 | `sdfPrimSetBitBltElem` writes four pairs |
| `sdfPrimBitBltPacket_t` | 96 | `sdfPrimSetBitBltPacket`, four registers |
| `sdfModelDrawEnvSet_t` | 96 | `sdfModelSetDrawEnv` strides the four sets by 0x60 |
| `sdfModelDrawEnvTerm_t` | 48 | the same function's tail, a draw tag then a DMA tag at +0x20 |
| `sdfModelDrawEnv_t` | 432 | four sets then the terminator at +0x180 |

`sdfGrFrameSet_t` is confirmed field by field three times over.
`sdfGrSetFrameSetMode` writes `w` +0, `h` +2 and `buffH` +4 as `sh` and
`pixelFormat` +6 and `zbuffFormat` +7 as `sb`, and clears +8, +0xC and +0x10.
`sdfGrMakeFrameSet` allocates the two frame buffers at `w` by `buffH` in
`pixelFormat` into +8 and +0xC and the depth buffer in `zbuffFormat` into +0x10,
and `sdfGrFreeFrameSet` frees exactly those three. `sdfGrSetDrawEnv` indexes
`frameHandle` by `buffId` with a stride of 4, which is what makes it an array of
two rather than two fields.

`sdfGrDrawState_t`: `tag` +0 and `preDrawHead` +4 from `sdfGrReqDraw`, which
zeroes both and then stores the submitted draw tag and the head of the predraw
chain; `sdfGrDrawFrame` walks that chain through `sdfPreDrawHook_t.next` +0 and
calls `predrawFunc` +4, which is how the second field is known to be a hook and
not a tag. `mode` +8, `buffId` +9, `frameId` +0xA and `halfOffset` +0xB are all
`lbu`/`sb`: `sdfGrReqDraw` writes `sdfFrameId` into +9, `sdfGrDrawFrame` writes
`sdfGrDrawFrameId` into +0xA and the field parity into +0xB, and
`sdfDeliverDrawEnd` is handed +8, +0xA and +0xB in that order. `sdfDrawSync`
reads +8 and compares it against 2, so it is a state and not a flag. +0xC is
never read; it is named `pad`.

The four register packets are confirmed register by register, because each
builder writes both halves of every A+D pair and the register address names the
field: `sdfGrSetDrawDefParmPacket` writes PRMODECONT 0x1A, COLCLAMP 0x46, DTHE
0x45 and TEXA 0x3B at +0, +0x10, +0x20 and +0x30; `sdfGrSetDrawFramePacket`
writes FRAME, ZBUF, XYOFFSET and SCISSOR, choosing the context-2 address of each
when its `aContext` argument is set; `sdfGrSetClearPacket` writes TEST, PRIM,
RGBAQ and two XYZ2; `sdf2DMakeTexPacket` writes TEX1, TEX0 and CLAMP, taking
each address from the caller's base so the same packet serves either context.
`sdfGrSetDrawEnv` then places `defParm` +0x20, `frame1` +0x60, `frame2` +0xA0,
`clear` +0xE0 and the four loose registers at +0x130, +0x140, +0x150 and +0x160.

`sdfPrimTransElem_t` has eight builders, one per transparency layer per context,
and all eight write TEST at +0 and ALPHA at +0x10. `enum sdf_gs_alpha` and
`enum sdf_gs_test` are the values they write: `SDF_GS_ALPHA_BLEND` 0x44 with
`SDF_GS_TEST_ALPHAGEQ` 0x717FB from `sdfPrimSetOpaqueElem` and
`sdfPrimSetSemiTransElem` (identical bodies but for the register addresses),
`SDF_GS_ALPHA_ADD` 0x48 and `SDF_GS_ALPHA_SUBTRACT` 0x42 both with
`SDF_GS_TEST_NOZWRITE` 0x71801 from the `AddTrans` and `SubTrans` setters, and
`SDF_GS_TEST_DRAWENV` 0x517FB from `sdfGrSetDrawEnv`. `enum sdf_gs_reg` lists
only the register addresses one of the functions read in this pass actually
writes.

`sdfModelDrawEnv_t` is `sdfModelSetDrawEnv` end to end: it calls
`sdfPrimSetOpaquePacket`, `sdfPrimSetSemiTransPacket`, `sdfPrimSetAddTransPacket`
and `sdfPrimSetSubTransPacket` on +0x20, +0x80, +0xE0 and +0x140, so the sets are
96 bytes apart with the packet at +0x20 of each; it then runs
`sdfDrawTagInit`/`sdfDrawTagLink` on each set's own +0, stamps a VIF FLUSH into
each packet's `dat0` at set+0x28, and finally initialises a fifth draw tag at
+0x180 whose one packet is the DMA tag at +0x1A0. `sdfModelDrawEnvLinkOt` files
set *i* through `ot[i]->regFunc` +0x10 and the terminator through `ot[3]`'s.

`sdfGrScreenDef_t` is the one struct here with an unconfirmed field.
`sdfGrSetFrameSetMode` reads +0, +2 and +4 as `lh` and +6 and +8 as `lhu`, so the
table entry's `pixelFormat` and `zbuffFormat` are `u16` where the frame set's are
`u8`; the 12-byte stride leaves a halfword at +0xA that nothing in `sdfGraph.c`
reads. It is HD's `fieldFrameMode`, marked unconfirmed.

`sdfModelDrawEnv*` sit in `draw.h` rather than `model.h`, against the `sdfModel*`
naming, because they are nothing but `sdfDrawTag_t` and `sdfPrimTransPacket_t`
and `model.h` does not include this header. `sdfGrScreenDef_t` is HD's name for
the mode table entry; the IDB has no type for it.

### draw3d.h

The camera, light, fog and transform packets VU1 shades with, and the
generated-geometry path that turns a mesh into GS primitives on VU0.
`libsdf.h` declares `sdf3DLightParallelPacket_t` and `sdf3DFogPacket_t` as opaque
so `sdfDrawTag_t` can point at them. The light packet's sole definition is in
`sdf_lighting.h`, included here and directly by its constructor; the fog and
other packet definitions remain in `draw3d.h`.

The game's `dds3LightParam` state in `dds3KernelDraw.c` is not an unbounded
pointer array: its four parallel-light pointers are followed by an ambient
vector at `+0x10` and `sdf3DFogParam_t` at `+0x20`. The unit-local
`dds3LightState_t` names these regions; `dds3FogRange_t` holds the four float
blend endpoints without a color word. The light and ambient packed-color
setters remain `INCLUDE_ASM`: their scalar C drafts lose the native frame
and packing stores, and written-but-unused quadword carriers are not retained
merely to reproduce those instructions.

| struct | size | evidence |
|---|---|---|
| `sdf3DScreenParam_t` | 160 | `sdf3DSetCamera` copies 160 bytes into `sdf3DScreenParam` |
| `sdf3DViewportPacket_t` | 208 | `sdf3DMakeViewportPacket` sets `qwc` 12 and writes to +0xCC |
| `sdf3DTmPacketData_t` | 128 | `sdf3DPrimSetTmPacket` writes +0 through +0x7C |
| `sdf3DLightParallelPacket_t` | 224 | `sdf3DMakeLightParallelPacket` sets `qwc` 13 |
| `sdf3DFogParam_t` | 20 | fields through `color` +0x10; see below |
| `sdf3DFogPacket_t` | 80 | `dds3DrawFirstProcess` calls `sdfGrAllocPacketBuff(0x50)` |
| `sdf3DInitPacket_t` | 32 | `sdf3DLinkInitPacket` writes two DMA tags |
| `sdf3DPrimPolyParm_t` | 44 | `sdf3DPrimMakePoly` reads +0 through +0x28 and nothing beyond |
| `sdfCorePacketHeader_t` | 16 | the prim header sits at +0x10 |
| `sdfCorePrimHeader_t` | 8 | `corePktInitCb` reads four halfwords and takes the payload from +8 |
| `sdfCorePacket_t` | 24 | `sdfCoreDrawScanOneSkinNodes` reads the bone count at +0x18 |
| `corePktCb_t` | **unproven** | offsets only; see below |

`sdf3DScreenParam_t` is confirmed field by field by `sdf3DMakeScreenParam`
alone. It reads `flags` +0 (testing bit 1 for the half-height Y scale and bit 2
for deriving `projDist` from `fovy`), `aspect` +4, `projDist` +8, `fovy` +0xC,
`ox` +0x10, `oy` +0x14, `width` +0x18, `height` +0x1C, `zMin` +0x20, `zMax`
+0x24, `nearClip` +0x28 and `farClip` +0x2C, stores the projection as four
`sqc2` at +0x30, and writes `scale` +0x70..0x7C and `trans` +0x80..0x8C one float
at a time. `cameraPos` +0x90 is `sdf3DMakeViewportPacket`'s, which inverts the
camera matrix and stores the result into both the packet and the parameter. The
160-byte size is `sdf3DSetCamera`'s copy into the global `sdf3DScreenParam`: it
runs from the parameter to parameter+160 in 32-byte steps, and the global at
0x529670 ends exactly where `sdf3DScreenTm` begins at 0x529710. Two names in the
IDB's data, `sdfDrawFaceListParam` at 0x5296E0 and `sdfDrawFaceListParam2` at
0x5296F0, are that global's own `scale` and `trans` quadwords labelled
separately, not neighbouring globals.

`sdf3DLightParallelPacket_t` is confirmed field by field by
`sdf3DMakeLightParallelPacket`, which opens with `sh 13` at +0 (13 quadwords of
payload after a 16-byte header, which is the 224 bytes exactly) and the two VIF
codes at +8 and +0xC. Its loop runs three times writing one float into each of
+0x10, +0x20 and +0x30 and advancing by 4, so `normal[3]` holds the three light
directions as a matrix's *columns*; the same loop writes `color[i]` as a
quadword at +0x50 + 0x10i, and `colorBase` +0x40 is a `lq`/`sq` copy of
`color[0]`. The ambient argument lands in both +0x80 and +0x90, which is
`color[3]` and `color[4]`, and the lighting matrix is four `sqc2` at +0xA0.

`sdf3DFogPacket_t` is confirmed field by field by `sdf3DMakeFogPacket`: the DMA
tag 0x20000004 at +0, the VIF codes at +8, `fogParm[4]` at +0x10 as four
`swc1`, the GIF tag at +0x20 with REGS 0xE, the A+D pair at +0x30 whose register
is 0x3D, GS FOGCOL, `mscal` +0x40 and the three zeroed words to +0x4C. The size
is also the literal its only caller allocates. `sdfCoreDrawSetFog` reads
`fogParm` back as one `lq` from +0x10, which pins it again.

`sdf3DFogParam_t` is the argument of that function, read at +0, +4, +8, +0xC and
+0x10. The names are HD's and the arithmetic agrees with them: `fogParm[0]` is
+4 and `fogParm[1]` is +0, which is the minimum then the maximum, and the depth
terms divide by `+0xC - +8`, which is far minus near. No allocation of one was
found, so the 20-byte size is only its own fields.

`sdf3DTmPacketData_t` closes the `void *tmPacket` `model.h` left open.
`sdf3DPrimSetTmPacket` stores the world matrix as four `sqc2` at +0..0x30, the
three rows it derives for normals at +0x40, +0x50 and +0x60, and the four
command words at +0x70 (ITOP 0x04000010, MSCAL 0x14000014, then two zeroes), so
one packet is 128 bytes and `sdfModelNode_t.tmPacket`'s 256 is two of them, one
per frame buffer, exactly as the `frameId * 128` index said.

`sdfCorePacket_t` is the type `sdfModelLinkOt`'s type 3 path reaches through
`sdfCorePacketGen`. That function reads `packetType` +0 as `lbu` and switches on
7 and 8, `matIndex` +6 as `lhu` and indexes the root's material list by it,
`extData[0]` +8 as a word and splits it into a shape count and a morph index,
and hands `packet + 0x10` to `corePktInitCb`. `corePktInitCb` reads four
halfwords from there into the draw state, as `faceCount`, `pointCount`,
`attrBits` masked by the draw's own mask (keeping the unmasked copy as
`orgAttrBits`) and `packetBits`, and takes the mesh body as `primHeader + 8`.
`sdfCoreDrawScanOneSkinNodes` confirms the same two offsets from the other end:
it tests `packetBits` at packet+0x16 for 0x800 and then reads the bone count at
packet+0x18 and the bone ids from +0x1A. So the header is 16, the prim header 8
and the payload starts at 24. `mpgType` +1, `priority` +2 and `size` +4 have no
reader in any of the functions read, and are marked unconfirmed; they are
`sdfShapePacket_t`'s first eight bytes, where the equivalents *are* confirmed.

`sdf3DPrimPolyParm_t` is confirmed field by field by `sdf3DPrimMakePoly`:
`faceCount` +0 and `pointCount` +2 as `lh`, `attr` +4 as `lhu` or'd with 0x68,
`attrInhibit` +6 as `lhu` then inverted, `color` +8, and then one `lw` per array
at +0xC, +0x10, +0x14, +0x18, +0x1C and +0x20, each non-null one adding its own
PRIM bit and its own UNPACK; `allocator` +0x24, called in place of
`sdfGrAllocPacketBuff` when set, and `zBias` +0x28 as `lwc1`. Nothing is read
past +0x28, so the struct is 44.

`corePktCb_t` is the exception in this document: **its offsets are proven and
its size is not**. `sdfCorePacketGen` is the only function that declares one, on
its own stack, so there is no allocation and no stride. Every offset from
`tmSave` +0 to `matPacket2` +0xB4 is a load or a store: `corePktInitCb` writes
+0x40, +0x42, +0x44, +0x48, +0x4C, +0x80 and +0x90; `corePktSetupPointBuff`
writes +0x64 through +0x74 and puts `&cb->extPointTop` into +0x7C;
`corePktOpenAndEmit` writes the opened packet buffer at +0x80, the closing tag's
address at +0x84 and the VU1 address at +0x88 and +0x8C, which `corePktClosePacket`
reads back and compares to decide whether anything was emitted;
`corePktClipFace` and `corePktBuildFaceList` write +0x50 and +0x52 and read
+0x94 and +0xAC;
the emit routines read +0x54, +0x58, +0x5C, +0x60, +0x78 and +0xA4;
`sdfCorePacketGen` writes +0x9C, +0xA0, +0xA4 and +0xA8 as it lays out the
arrays, +0xAC as `+1.0` or `-1.0` from its `reverse` argument, and +0xB0 and
+0xB4 as copies of `attrBits` and `matPacketPtr`. `tmSave` is an inline
`sdfMatrix4_t`: `sdfCorePacketGen` stores four quadwords into +0..0x30 and
`corePktEmitTexTris`, `corePktBuildFaceList` and `corePktRelightBackFaces` `lq`
them back. The header therefore declares the struct through +0xB4, 184 bytes,
and `tools/header_layout.c` carries offsets for it and no size.

`vertexPtr` +0x98 is the one named field with no access found: in the paths read
the vertex base stays in a register and is passed as an argument. The three
pointer and two count slots at +0x64..+0x74 are named from HD; the code shows
`corePktSetupPointBuff` filling them from the vertex count, but not which is
which.

`enum sdf_core_attr` and `enum sdf_core_packet_bit` are the bit tests in
`sdfCorePacketGen`, `corePktEmitAllPasses` and `corePktOpenAndEmit`: 0x10
textured, 0x800 skinned and 0x1000 blend-shaped in the geometry word, 0x1000 the
multi-texture pass and 0x4000 the two-sided second batch in the attribute word.
The names are sdfhook's, which took them from the same branches.

#### sdfGsState_t is not a game type

It is sdfhook's own helper: a C++ struct with default member values and a
`constexpr sdfGetGsStateForOt` that returns one per ordering table. No such
struct exists in the IDB and nothing in the binary lays those three values out
side by side. The engine's representation is the A+D register pair in
`sdfPrimTransElem_t`, so what is ported here is the pair of enums in `draw.h`
instead. sdfhook's *values* are right: its `{0x48, 0x71801}` for table 2 and
`{0x42, 0x71801}` for table 3, with `{0x44, 0x717FB}` for 0 and 1, are exactly
what `sdfModelSetDrawEnv`'s four setters write, in that order.

#### Disagreements with sdfhook

Three, all resolved in favour of the retail code.

- `corePktCb_t`: sdfhook asserts 320 bytes. 320 is `sdfCorePacketGen`'s whole
  stack frame, which also holds that function's own spill slots: it saves `$ra`
  at frame+0x128 and two FPRs at +0x130 and +0x138, which cannot be inside a
  struct it passes by pointer. No `corePkt*` callee touches anything above
  +0xB4. So 320 is not a size claim about the struct and is not asserted here;
  every sdfhook offset from +0 to +0xB4 agrees with this header.
- `corePktCb_t.head`: sdfhook has `u8 head[64]`. The first 64 bytes are a
  matrix, written as four `sqc2` and read back as `lq` by three different
  routines, so it is declared `sdfMatrix4_t tmSave`, which is also HD's name for
  it. Nothing about the layout differs.
- `sdfGrDrawState_t.field_C`: sdfhook names it `field_C`, which is honest and
  matches the code. It is `pad` here on the same evidence: nothing reads it.

Everything else checked in this pass agrees with sdfhook exactly, including the
two sizes it could only have got from the same `qwc` fields (`sdf3DFogPacket_t`
80 and `sdf3DLightParallelPacket_t` 224, both of which its own header records
having corrected once).

#### Disagreements with the HD reconstruction

Two more instances of failure modes this document already lists, plus one new
one:

- **An inline struct became a pointer, again.** HD's `sdfCorePacketCb_t.tmSave`
  is a `sdfMatrix4_t *`. It is the inline 64-byte matrix above, which is what
  puts every HD field from `faceCount` on 0x3C below its real offset.
- **An inline array became a pointer, again.** HD's `sdf3DFogPacket_t` has
  `dmatag`, `giftag` and `fogColor` as pointers and so comes out at 28 bytes
  instead of 80. All three are inline tags the DMAC and GIF read in place.
- **New: a halfword pair became one field and vice versa.** HD's
  `sdfGrScreenDef_t` gives `pixelFormat` and `zbuffFormat` as `u16` and
  `sdfGrFrameSet_t` as `u8`. Both are right, and they are different types for
  the same thing: `sdfGrSetFrameSetMode` reads the table's with `lhu` and writes
  the frame set's with `sb`. A type copied from the wrong one of the two would
  move every pointer in the frame set.

## fpu.h: shared scalar FPU helpers

`fsqrtf` and `frsqrtf` follow DDS's documented scalar FPU convention. They
emit the bare EE `sqrt.s` and numerator-over-square-root `rsqrt.s`, without
the errno-checking library path or gcc's extra hazard nops. The evidence for
the compiler distinction is DDS `docs/idioms.md`, "Square roots: sqrt.s";
it is not a claim that every SMT3 square-root site has been audited.
New callers share these helpers rather than spelling scalar FPU asm locally.
`ffabsf` supplies the documented inline `abs.s` boundary when a unit requires
it. In `sdf3DMakeLightParallelPacket`, builtin `fabsf` schedules the axis load
ahead of the threshold literal; the shared helper reproduces retail ordering.
DDS `docs/idioms.md`, "Absolute values: ffabsf versus builtin fabsf", records
the paired convention and its kernel exception. Select it from local evidence
rather than replacing unrelated absolute-value calls.

## ps2asm.h: the EE extension macros

`ps2asm.h` is the one header the build includes. cc1 2.96 never emits COP2 macro
mode (the `v*` ops, `qmfc2`/`qmtc2`, `cfc2`/`ctc2`, `lqc2`/`sqc2`) or the MMI
128-bit integer ops (`lq`/`sq`, `pextlw`, `pcpyld`, `ppacb`, `paddw`, `mult1`,
`div1`, `mfhi1`/`mflo1`), so a function that uses them cannot be written in plain
C. 1045 of the stubbed functions contain at least one such instruction.

### Where the macro names come from

`SDF_VU0_*` is the engine's own name for the operation, not a reconstruction.
The names and their register convention come from the HD build, where the same
helpers survive as functions.

The spelling is one underscore, not two. The HD reconstructed sources write
`SDF_VU0__LOADVECTOR`, but that is their transpiler rendering the `$$` that
separates a class from its method: the HD metadata has the helper both as
`libsdf_H.SDF_VU0$$LOADVECTOR`, where `SDF_VU0` became a class, and inlined into
other classes as `bilDebug$$SDF_VU0_LOADVECTOR` and
`bilManager$$SDF_VU0_LOADVECTOR`, where the whole name is the method and the
single underscore shows through. Nineteen helpers appear in that second form and
every one of them has one underscore.

What the bodies establish: `SDF_VU0_LOADVECTOR` writes the first working
vector, `SDF_VU0_ADDVECTOR` does `A = A + B`, `SDF_VU0_INNERPRODUCT` returns
`Dot(A, B)` and `SDF_VU0_GETVECTOR_Y` returns `vct_A.y`. That fixes A, B and C
onto `vf10`, `vf11` and `vf12`, which is the convention the retail inlines
already follow. 123 macros carry these names, covering about 3,100 call sites.

The other prefixes are this repository's own, for operations the engine library
does not name:

| prefix | what it covers |
| --- | --- |
| `SDF_VU0_` | the engine's VU0 helper library, named as the engine names it |
| `VU0_` | VU0 work the library has no entry for: the matrix banks beyond A/B, the clip-segment and quaternion pieces |
| `EE_` | scalar and pipeline-1 EE instructions (`EE_MULT1`, `EE_LOAD_UNALIGNED_WORD`, the barriers) |
| none | an operation the engine has its own function name for: `sdfSqrt`, `sdfFabs` |
| `MMI_` | the 128-bit integer ops used on their own |
| `PS2_` | plain quadword memory moves |

A `VU0_` name is therefore a signal that the operation was reconstructed here
and may yet turn out to have an engine name.

The library is larger than the call sites show. `libsdf.c` in the HD sources
declares 133 of these helpers, and the matrix bank is addressed by row:
`LOAD_ROW0` through `_ROW3` load `vf28`-`vf31`, with `STORE_ROW`, `SET_ROW`
(row from the working vector) and `GET_ROW` beside them. Any macro naming one
of those registers is one of these, so it carries the engine name. The HD bodies for the matrix side are stubs, so the names there were settled by
cross-reference instead: for a function with a body in both trees, the engine
helper the HD body calls at the position our body calls a macro is that macro's
name. That is what `TRANSLATE`, `SCALE_XYZ`, `SCALE_XYZ_REV`,
`STOREIDENTITYVECTOR` and `SETSCALE1VECTOR` rest on, each aligned 1:1 in the
same function.

The cross-reference also shows where retail called one helper and we had
written several. Three moves through `vf2` are `SWAPVECTOR_A_C`, and four
per-row `vmove`s are `COPYMATRIX_A_TO_B`; folding the nine groups back into one
statement each left the image byte-identical, which is the proof that the
engine's single macro is what retail compiled.

A variant keeps the engine name. Where two macros hold the same instructions
and differ only in the operand section, the engine name carries both, with a
suffix for what differs: `_BARRIER` adds the memory clobber, `_NOMEM` drops it,
`_MEMOPS` declares the memory operands instead of clobbering, `_EXTENDED` is an
empty operand list, `_V0` routes a scalar through `$2`. A repo-local prefix on
one of those would claim an operation the engine does not have.

### The rule the macros follow

No macro parameter reaches the assembler. Not an opcode, not a suffix, not a
register name, not an axis letter. Those are literal in the macro body, always.
Parameters are only C expressions bound through operand constraints (`"r"`,
`"f"`, `"m"`) and substituted as `%0`, `%1`.

That is why there are many near-duplicate macros (`SDF_VU0_SCALEVECTOR`
beside `SDF_VU0_SCALEVECTOR_B`, `SDF_VU0_LOADVECTOR` beside
`SDF_VU0_LOADVECTOR_B`) instead of one parameterised form: a register passed as
a token is still an assembly literal in C clothing, and it costs a preprocessor
paste table to translate. 21 macros did take a register or axis token;
converting their 229 call sites to the fixed-register names removed 21
definitions and needed only 14 new ones, because 36 of the 50 register tuples in
use already had an identical fixed macro.

Two further things are not interchangeable, and both are easy to get wrong.

A **basic** asm (no colons) and an **extended** asm with an empty operand list
(`: :`) are scheduled differently by cc1 2.96. `SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED`
and `SDF_VU0_SUBVECTOR_EXTENDED` exist only because `mdlSetBlur` needs the
extended spelling; substituting the plain form moves the float constants it
materialises and breaks the match. A macro named `_EXTENDED` means exactly this.

### Name it for the function, not the instruction

`EE_SQRT_F32` and `EE_ABS_F32` were named for the instruction they emit. The
engine has its own names: the HD reconstruction's `symbols.txt` places `sdfSqrt`
and `sdfFabs` in `sdfMath.c`, with bodies that are just `sqrtf` and `fabsf`, and
Nocturne inlines them. They are now `sdfSqrt(x)` and `sdfFabs(x)`, value forms,
so a call site reads `dist = sdfSqrt(dx * dx + dz * dz);` the way the original
source did rather than passing an out parameter.

That also removed a macro. `EE_SQRT_F32` was `volatile` and `EE_SQRT_F32_SCHED`
was the plain form added because the volatile one pinned the schedule in
`code_00116AD0`. A statement expression is not volatile, so one `sdfSqrt` covers
both, and all 21 call sites across 9 units still match.

Check the HD sources before inventing a macro name, and check them by
instruction, not by name. `EE_SQRT_F32` does not resemble `sdfSqrt`: finding it
meant knowing that `sqrt.s` is what the one-line body `return sqrtf(aValue);`
compiles to. A name-similarity scan reports the eleven `SDF_VU0_COPYMATRIX_*`
macros against the `CopyMatrixAtoB` wrappers in `sdfVu0.c`, which are the
functions that *call* the macro rather than its own name, and misses the sqrt
case entirely. So the check is manual: take the instruction the macro emits and
grep the HD `src/` tree for the libc call it implements.

| instruction | HD body | engine name |
| --- | --- | --- |
| `sqrt.s` | `return sqrtf(aValue);` | `sdfSqrt`, `sdfMath.c` |
| `abs.s` | `return fabsf(c);` | `sdfFabs`, `sdfMath.c` |
| `div.s` after `sqrt.s` | `return aBase / sqrtf(aValue);` | `sdfDivSqrt`, `sdfMath.c` |

This does not apply to the `SDF_VU0_` family: eighteen of those macro names are
listed in `symbols.txt` as functions outright, because the Unity port compiled
the macro bodies as functions, so the repo's spelling is already the engine's.
Nor to `EE_SYNC*`, the MMI ops or the pipeline-1 divide helpers, which are
instructions and not inlined engine functions. The HD sources add nothing for
type names either: they resolve none of the 83 invented type names and none of
the 567 the IDB never typed, because the IDB's recovery already subsumes them.

**The operand section is part of the macro**, and so is `volatile`. Two macros
that assemble the same instruction but declare different clobbers, different
constraints, or drop `volatile` produce different register allocation or let cc1
move the block. Where two macros differ only in this way they get different
names rather than one name and an `#undef`:

| name | what differs |
| --- | --- |
| `SDF_VU0_LOADVECTOR_BARRIER` | the load clobbers memory; `SDF_VU0_LOADVECTOR` does not |
| `SDF_VU0_SCALEVECTOR_V0`, `SDF_VU0_SCALEVECTOR_B_V0`, `SDF_VU0_SETVECTOR_Y_V0` | the scalar reaches vf2.x through `$2`, not a register cc1 picks |
| `SDF_VU0_STOREMATRIX_NOMEM`, `SDF_VU0_STOREVECTOR_NOMEM`, `SDF_VU0_STOREVECTOR_B_NOMEM` | the same stores without the `memory` clobber |
| `SDF_VU0_COPYVECTOR_MEMOPS`, `SDF_VU0_LOADMATRIX_B_MEMOPS` | the memory is a declared `"m"` operand rather than a clobber |
| `SDF_VU0_COPYVECTOR_F32` | four aligned float elements are explicit read/write operands; unrelated memory is not clobbered |
| `SDF_VU0_STOREVECTOR_F32`, `SDF_VU0_LOADVECTOR_B_F32` | explicit volatile four-float scratch operands, with the address also in a GPR |
| `SDF_VU0_STORE_ROW0`, `_VF29`, `_VF30` | these clobber memory; the `_NOMEM` forms beside them do not |
| `SDF_VU0_COPYVECTOR_A_TO_B_EXTENDED`, `SDF_VU0_SUBVECTOR_EXTENDED` | an empty operand list rather than a basic asm |

The parallel-light packet uses an aligned volatile axis scratch consumed by
VU0 loads. This follows the matched rotation/Euler/quaternion scratch convention
in DDS `docs/idioms.md`: component writes remain ordered before `lqc2`. Its
identity path uses `SDF_MMI_STORE_UNIT_MATRIX`; a plain C scalar initialization
emits 728 bytes for the function instead of retail's 696, while the shared
VF0/MMI lane-shuffle operation reproduces the native sequence.

Every operation is defined once in `ps2asm.h`, and units reuse those macros. The five that used to stay local for a differing operand
section are now named for what differs -- `SDF_VU0_SCALEVECTOR_V0`,
`SDF_VU0_SCALEVECTOR_B_V0` and `SDF_VU0_SETVECTOR_Y_V0` carry the scalar through `$2`
rather than a register cc1 picks -- so nothing needs an `#undef` any more.

The engine's own convention makes that practical. The VU0 inlines in the retail
code always use the same registers for the same roles, so the macros fix them:

| register | role |
| --- | --- |
| `vf10` | the working vector, in and out of every routine |
| `vf11` | the second operand |
| `vf12` | the spare a caller parks a vector in across a call |
| `vf2` | broadcast and dot-product scratch |
| `vf28`-`vf31` | the matrix bank |
| `vf0` | the hardwired `(0,0,0,1)` |
| `Q`, `ACC` | divide/sqrt result and the multiply accumulator |
| `$vi18`, `$vi22` | clip flags, `Q` status read back by `cfc2` |
| `$2`, `$3` | GPR scratch inside the colour macros, declared as clobbers |

Of the 1045 blocked functions, 773 use only instructions these macros emit and
728 of those also stay inside this register convention. That bounds what the
header can reach; it does not promise a match, since each function still has to
come out of cc1 in retail's order.

### What ee-gcc 2.96 does with them

Measured against this `cc1` and Sony's `ee-as -G8`, not assumed:

- `__asm__ volatile` with no output operands is never deleted or moved across
  another `volatile` asm, so a chain of them keeps its written order. Blocks with
  only outputs are reordered freely, which is why every macro is `volatile`.
- `"=r"` and `"r"` on a `mode(TI)` value work: cc1 allocates one GPR and `ee-as`
  takes the 128-bit form of the instruction. No `__int128` arithmetic is needed,
  and none is available.
- cc1 does not know an asm block touches memory. Every macro that loads or
  stores through a pointer clobbers `"memory"`, without which loads get hoisted
  out of a loop that contains the block.
- `ee-as` fills branch delay slots, and will happily move a COP2 op into one.
  Every multi-instruction macro is bracketed with `.set noreorder` /
  `.set reorder`.
- Register choice inside a macro is not free. `MMI_RGBA_UNPACK` and
  `SDF_VU0_dds3ColorFtoI` name `$2` and `$3` literally because an `"=&r"`
  temporary gets `$3`/`$4` here and retail uses `$2`/`$3`.
  The same holds for the two scales of the colour pack: `MMI_RGBA_PACK_255`
  multiplies by 255 through `$2`, and `MMI_RGBA_PACK_128` by 128 through `$3`.
  Which one a function wants is visible in the retail word loaded by the
  `lui`: `0x437F` is 255, `0x4300` is 128. The 128 form must leave `$3` out of
  its clobber list, or the result moves to `$4`: `dds3SetAmbientColor` and
  `dds3SetLightColor` both need the undeclared form, and the latter also needs
  one result local per pack rather than one reused across both.
- The compiler still schedules the code around a block. The order of the C
  statements next to one decides which `$f` register a float lands in (first
  written takes the lowest free one) and which address computation is emitted
  first. Matching a VU0 routine is mostly getting that order right, not the asm.
- `SDF_VU0_COPYMATRIX_A_TO_B` preserves `vf28`–`vf31` in `vf24`–`vf27`.
  `fldPoint3To2` and `fldPoint3To2H` use this after `MultMatrixRev`, before
  projecting their input point. The four `vmove.xyzw` operations are visible at
  `0x00128DAC` and `0x00128E84` in SLUS_209.11.
- Address CSE can be an obstacle: gcc may keep a frame address in `$16`
  across calls where retail recomputes it. `effPCPBlurScreenOffset` now
  matches with ordinary scalar locals around its VU pipeline and the shared
  `fpu.h` square-root helper, without per-site scalar asm.

### What is proven

Ten functions in three units are byte-identical through these macros:
`effScreenOffset`, `effColorBlend`, `effMagaCylinderCalcColor` and
`effLinePointLength` in `effect/effMath.c`;
`Eff_ModelTrackPolyGetMarkPosition` and `dds3HermitianTrackPolyGetVertex` in
`effect/effModelTrackPoly.c`; `effDebugPointDraw`, `effDebugPointDraw2`,
`effDebugLineDraw` and `effDebugLineDraw2` in `code_001AC0A8.c`.

Those exercise `SDF_VU0_COPYVECTOR`, `SDF_VU0_LOADVECTOR`,
`SDF_VU0_STOREVECTOR`,
`SDF_VU0_STOREVECTOR_MEMOPS`, `SDF_VU0_LOADVECTOR_B`, `SDF_VU0_LOADMATRIX`,
`SDF_VU0_COPYVECTOR_A_TO_B`, `SDF_VU0_ADDVECTOR`, `SDF_VU0_SUBVECTOR`,
`SDF_VU0_MULVECTOR`, `SDF_VU0_SCALEVECTOR`, `SDF_VU0_SCALEVECTOR_B`,
`SDF_VU0_APPLYMATRIX_XYZ1`, `SDF_VU0_PERSVECTOR`, `SDF_VU0_INNERPRODUCT`,
`SDF_VU0_GETLENGTH`, `MMI_RGBA_UNPACK` and `SDF_VU0_dds3ColorFtoI`.

The rest assemble and have been read against the retail instructions they are
named for, but no function has matched through them yet: the MMI lane ops, the
pipeline-1 family (`EE_MULT1`, `EE_DIV1`, `EE_DIVU1`, `EE_REM1`),
`SDF_VU0_OUTERPRODUCT`, `SDF_VU0_NORMALIZE`, the `vf12` copies, the clip-flag
macros, `SDF_VU0_LOADVECTOR_UNALIGN`, `MMI_STORE_VEC3`, `MMI_PLZCW` and the
barriers.
Treat an unproven macro as a starting point, and check the emitted `.s`
(`SMT3_KEEP_S=/tmp/x.s tools/cc.sh ...`) rather than trusting the name.

## Unit-local SDF file, sound and IPU views

`src/smt3/code_002BD340.c` keeps its file-command, sound-command and IPU
records local: no other C unit currently shares their complete layouts.
The file handle is the 0x40-byte allocation initialized by
`sdfFileCmdNewHandle`; its operation/state bytes are at +0x15/+0x16 and
its callback/context words at +0x38/+0x3C. `sdfIpuCb_t` is the 0x8C-byte
decoder record cleared by `sdfIpuInitCb`, including two decoded buffers
at +0x20, two transfer-packet buffers at +0x28 and saved DMA state at +0x68.
The active-buffer getters use the completion byte at +0x1A and the
submission index at +0x19, rather than an unrelated MIDI-table view.

The sound work view accounts for SMT3's seven track slots and opaque words
at +0x1D8/+0x1DC; its two 24-byte MIDI state groups begin at +0x560 with
a 0x20-byte stride. These offsets differ from the DDS reference layouts.
`p2sd_2C1F50` returns the 0x10-byte channel rows initialized by
`sdfSndVoiceMapInit` at `sdfSndMap + 0x10`, not an eight-byte volume view.
The IPU output-mode byte describes RGBA32, RGBA16 or dithered RGBA16;
`sdfIpuDecode` turns those selectors into BDEC's OFM/DTE bits using the
table at 0x00529CE8. Existing allocator, item-array, texture and GS-memory
owners are reused from `libsdf.h`; concrete vector operations come from
`ps2asm.h`.

## What was deliberately left out

The HD set is 272 headers, most of it per-effect particle state
(`effPCPFlashPolySphere2Exp_tag` and 120 similar), which is wide, shallow and of no
use until someone decompiles that effect. Also left behind:

- The complete HD `effect.h` (3673 lines), `newbattle.h`, `field.h`,
  `facility.h`, and the rest of the gameplay headers. The small recovered
  `event.h`, `effect.h`, `field.h` and `nb.h` are native consumed layouts,
  not imports of those HD gameplay headers.
- The body of a generated mesh: `corePktCb_t.dataPtr` points at a VIF stream
  whose UNPACKs `sdfCorePacketGen` walks with pointer arithmetic and never a
  struct, so there is nothing to declare but the counts and flags in
  `sdfCorePrimHeader_t`, which `draw3d.h` has. The batch header sdfhook calls
  `sdfCoreBatchHeader_t` is that same four-halfword header read at a different
  address, not a second type.
- `corePktCb_t`'s size. See `draw3d.h` above: the offsets are proven and the
  size needs a second declaration site, which does not exist in this binary.
- The rest of sdfhook's draw-environment family: `sdfGrStdDrawEnv_t` (544),
  `sdfGrStdDrawEnvExt_t`, `sdfGrAlphaMaskPacket_t`, `sdfGrFramePacket_t`,
  `sdfGrDrawFrame_t` and `frDrawEnvPkt_t`. All are compositions of the four
  register packets `draw.h` now has, over `sdfGrSetStdDrawEnv`,
  `sdfGrStdDrawEnvPredraw` and `sdfGrSetFramePacket`, which were not read in
  this pass. The same goes for the per-primitive `sdfPrim*Elem_t` family
  (`Sprite`, `F3`, `F4`, `G3`, `G4`, `Ft3`, `Ft4`, `Gt3`, `Gt4`, `Rect`,
  `Line`, `BoxLine`): one `sdfPrimSet*Elem` and one `sdfPrimLink*` each, 24
  functions, mechanical but not yet done.
- The GIF PACKED register structs (`sdfGifRgbaq_t`, `sdfGifUv_t`,
  `sdfGifXyz2_t`, `sdfGrSpritePrim_t`) and the register bitfield helpers
  sdfhook carries. `draw.h` has the register *addresses* as `enum sdf_gs_reg`
  and the four ALPHA and TEST words the engine actually writes; the field
  decompositions are hardware documentation, not recovered layout.
- `sdfLoadImagePacket_t` (176) and `sdfStoreImagePacket_t`, over `sdfSetLoadImage`
  and `sdfSetStoreImage`. Both open with the `sdfPrimBitBltElem_t` that
  `draw.h` proves, so they are the cheapest thing left on this list.
- `dds3DrawEnv_t`: 15 pointer fields in HD, expanded inline in the IDB to 7136
  bytes. Neither reading is checked against code, so it is not here. sdfhook
  has it inline at every member, which is now the more likely reading given
  `sdfGrFrameSet_t` and `sdfMemHandleCB_t` both turned out that way, but it
  still needs `dds3InitDrawEnvSub` read end to end.
- `types.h`: `common.h` already supplies the primitives, and the sdf math types
  live in `libsdf.h` where the engine keeps them. HD's `UNKNOWN_ARRAY_SIZE` and
  `UNKNOWN_DEFINE_VALUE` placeholders are not carried over.
- HD's `dds3ModelHandle_t` and `dds3ModelResrc_t` names and expanded layouts
  are not imported. The native owner/resource layer is represented by
  `mdlObj_t` and `mdlResrc_t` in `include/model.h` instead.

## Checking these headers

Size and offset assertions live in `tools/header_layout.c`, rather than in
the headers consumed by built units. It includes every header and makes one
claim per documented number with the array-sized-by-condition trick, since
ee-gcc 2.96 has no `_Static_assert`:

```c
#define ASSERT_AT(line, cond) typedef char static_assert_##line[(cond) ? 1 : -1]
#define SZ(line, type, bytes) ASSERT_AT(line, sizeof(type) == (bytes))
#define OFF(line, type, field, at) \
    ASSERT_AT(line, ((char *)&((type *)0)->field - (char *)(type *)0) == (at))
```

`python3 tools/test_headers.py` compiles it with the project's own `cc1` and
`ee-as`, then compiles it again with one size deliberately wrong to prove the
claims are not passing vacuously. It prints the current claim count on success.
A wrong number gives `size of array 'static_assert_mNN' is negative`.

To compile one header on its own, which is also worth doing when adding a type:

```sh
printf '#include "model.h"\nconst int ok = 1;\n' > /tmp/only_model.c
tools/cc.sh -Wall /tmp/only_model.c -o /tmp/only_model.o
```

### Name entry work (`nme.h`)

`nmeWork_t` shares the existing `Nme_Init` work view with `Nme_Draw`.
The initializer and cleanup establish the allocation handle at `0x14`, target
at `0x18`, cursor at `0x30D4`, input position at `0x5150` and texture handles
at `0x51B4`. The input cursor is at `0x1058`, confirmed by `nmeSyncCursor` calling
`cmpSetCursorRow`; `nmeInitNameCursor` initializes another at `0x2094`. The animation helper
reads and writes its signed timing value at `0x51C4`. Draw callbacks use
`nmeUpdateWork`, distinct from the persistent `nmeProcessId` pointer. Unrecovered spans remain padding. `tools/header_layout.c` checks
these offsets with the EE compiler.

The allocator clears the three signed input-row flags at `0x514D`. Cursor
initialization clears the alternate-cursor flag at `0x514C`; input dispatch
uses it to select the alternate movement handler. Input helpers index three
`0x20`-byte rows of two-byte character codes at `0x5152`, ending before the
texture handles. The shared layout checks these boundaries as well.

`nmeInitNameBuffer` initializes the mode cursor at `0x1C` and clears the
primary movement-handler flag at `0x30D0`. Input dispatch tests that flag
before calling `nmeMoveCursor`; `0x514C` selects `nmeMoveCursor2` instead.
