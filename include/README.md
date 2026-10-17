# Headers

Reconstructed types for the SLUS_209.11 decompilation. `common.h`, `include_asm.h`
and `macro.inc` belong to the build; the rest are recovered game types.

Nothing in `src/` includes these yet, so adding or editing one cannot change the
build. They are here so a unit that includes `model.h` or `scr.h` finds the types
the original source would have given it.

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
3. **[smt3hd-reconstructed-sources](https://github.com/smt-ret)** (smt-ret), a
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

### basic.h

| struct | size | evidence |
|---|---|---|
| `dds3Basic_t` | 68 | `dds3AllocBasic` calls `sdfChipAllocDirect(68)` |
| `dds3PoseSet_t` | 208 | `dds3InitPoseSet` calls `sdfChipAllocDirect(208)` |
| `dds3BasicArrayHandle_t` | 16 | `dds3GetBasicArrayHandle` calls `sdfChipAllocDirect(0x10)` |
| `dds3PathBasicControl_t` | 36 | `dds3AllocPathBasicWorkData` calls `sdfChipAllocDirect(36)` |
| `dds3BasicFunction_t` | 24 | **HD only.** See below. |

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

`dds3BasicFunction_t`: `constructor` +0 (called by `dds3AllocBasic`, returning 1 on
success), `destructor` +4 (`dds3DeleteBasic`), `mainprocuder` +8 and
`drawprocuder` +12 (`dds3CallBasicMainProc`, `dds3CallBasicDrawProc`). The table is
reached through `dds3BasicFuncTablePtr[type]`, a pointer array, so nothing fixes
its stride: `reserve1`, `reserve2` and the 24-byte size are HD's and
**unconfirmed**.

### libsdf.h

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

`sdfTexHandle_t`: `prev`/`next` from `sdfTexLink`; `width` +12 and `height` +14 as
`s16`, `pixelGsHandle` +16, `clutGsHandle` +20, `numOfCLUT` +24, `CLUTFormat` +25,
`pixelFormat` +26, `numOfMipmap` +27 and `clutColor` +56 all from `sdfTexNew`'s
stores at the matching widths; `packet` +40 from `sdfTexGetPacket`, `numOfCLUT`
again from `sdfTexGetCLUTCount`'s `lbu`. Nine fields in the middle are IDB names at
IDB positions with no load or store found for them, and are marked `unconfirmed`
in the header; the 64-byte size bounds them.

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
`scrPrintProcedureStart` sizes the local arrays from. The remaining fields are HD
names in HD order, bracketed by those three anchors and the 32-byte size.

`scrCommandFunc_t` takes no arguments and returns `s32`: `CodeFunc_Comm` calls the
entry's `comm` without touching `$a0`, and `scrCommand_BIT_ON` reads its argument
with `scrGetIntPara` and returns 1.

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

## What was deliberately left out

The HD set is 272 headers, most of it per-effect particle state
(`effPCPFlashPolySphere2Exp_tag` and 120 similar), which is wide, shallow and of no
use until someone decompiles that effect. Also left behind:

- `effect.h` (3673 lines), `newbattle.h`, `field.h`, `facility.h`, `event.h`,
  `camp.h`, `newdata.h` and the rest of the gameplay headers. Each needs the same
  per-struct verification as the above and none of it is foundational.
- The 3D and core-packet half of sdfhook: `sdf3DScreenParam_t`,
  `sdf3DLightParallelPacket_t`, `sdf3DFogPacket_t`, `sdf3DPrimPolyParm_t`,
  `sdfCorePacket_t`, `sdfGsState_t`, `corePktCb_t`, `sdfGrDrawEnv_t` and the
  2D primitive packets. `sdfDrawTag_t` already points at the first two as
  opaque types, and `sdfModelLinkOt`'s type 3 path goes through
  `sdfCorePacketGen`, so that is the natural next block.
- `sdf3DTmPacketData_t`: `sdfModelNode_t.tmPacket` is 256 bytes from
  `sdfModelAllocTmPacket`, indexed by `frameId * 128`, which is all this pass
  proved. The field layout of one 128-byte packet is `sdf3DPrimSetTmPacket`'s
  and was not recovered, so the member stays `void *`.
- The generated-geometry shape packet types, `sdfShapePacket_t.packetType` 4
  through 8. `sdfModelLinkDrawPacket` only forwards them to `sdfCorePacketGen`,
  so their bodies belong with the core-packet block above.
- `sdfGrFrameSet_t`, `sdfGrDrawState_t` and the rest of sdfhook's frame and
  draw-environment types. `sdfGrOt_t` is here because the model and billboard
  paths both end in its `regFunc`; the others do not.
- `dds3DrawEnv_t`: 15 pointer fields in HD, expanded inline in the IDB to 7136
  bytes. Neither reading is checked against code, so it is not here.
- `types.h`: `common.h` already supplies the primitives, and the sdf math types
  live in `libsdf.h` where the engine keeps them. HD's `UNKNOWN_ARRAY_SIZE` and
  `UNKNOWN_DEFINE_VALUE` placeholders are not carried over.
- `dds3ModelHandle_t`, `dds3ModelResrc_t` and the rest of HD's `model.h`: the
  `dds3Model*` layer above `sdfModelRootNode_t`. Used in 79 IDB prototypes and
  wanted, but no allocation site was found to fix `dds3ModelHandle_t`'s size in
  either pass. `include/model.h` holds the `sdfModel*` types instead.

## Checking these headers

The size and offset assertions are not in the headers, since nothing in the build
includes them yet and a header should not carry a test. They live in
`tools/header_layout.c`, which includes every header and makes one claim per
number above with the array-sized-by-condition trick, since ee-gcc 2.96 has no
`_Static_assert`:

```c
#define ASSERT_AT(line, cond) typedef char static_assert_##line[(cond) ? 1 : -1]
#define SZ(line, type, bytes) ASSERT_AT(line, sizeof(type) == (bytes))
#define OFF(line, type, field, at) \
    ASSERT_AT(line, ((char *)&((type *)0)->field - (char *)(type *)0) == (at))
```

`python3 tools/test_headers.py` compiles it with the project's own `cc1` and
`ee-as`, then compiles it again with one size deliberately wrong to prove the
claims are not passing vacuously. It prints the claim count on success, 381 as
of this pass. A wrong number gives `size of array 'static_assert_mNN' is
negative`.

To compile one header on its own, which is also worth doing when adding a type:

```sh
printf '#include "model.h"\nconst int ok = 1;\n' > /tmp/only_model.c
tools/cc.sh -Wall /tmp/only_model.c -o /tmp/only_model.o
```
