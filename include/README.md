# Headers

Reconstructed types for the SLUS_209.11 decompilation. `common.h`, `include_asm.h`
and `macro.inc` belong to the build; the rest are recovered game types.

Nothing in `src/` includes these yet, so adding or editing one cannot change the
build. They are here so a unit that includes `scr.h` or `basic.h` finds the types
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
(allocation sizes, load and store offsets, array strides) rather than copied. The
compile-time checks live outside the repo, in the scratch file described at the end.

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

## What was deliberately left out

The HD set is 272 headers, most of it per-effect particle state
(`effPCPFlashPolySphere2Exp_tag` and 120 similar), which is wide, shallow and of no
use until someone decompiles that effect. Also left behind:

- `effect.h` (3673 lines), `newbattle.h`, `field.h`, `facility.h`, `event.h`,
  `camp.h`, `newdata.h` and the rest of the gameplay headers. Each needs the same
  per-struct verification as the above and none of it is foundational.
- The remainder of HD's `libsdf.h` and the bulk of sdfhook's 130 structs: the
  model, shape, motion, 3D-packet and billboard types. They are verified in
  sdfhook and worth porting next; they were cut here to keep the first pass to
  types that can each be justified from the retail code in this document.
- `dds3DrawEnv_t`: 15 pointer fields in HD, expanded inline in the IDB to 7136
  bytes. Neither reading is checked against code, so it is not here.
- `types.h`: `common.h` already supplies the primitives, and the sdf math types
  live in `libsdf.h` where the engine keeps them. HD's `UNKNOWN_ARRAY_SIZE` and
  `UNKNOWN_DEFINE_VALUE` placeholders are not carried over.
- `model.h`: `dds3ModelHandle_t` is used in 79 IDB prototypes and is wanted, but
  no allocation site was found to fix its size in this pass.

## Checking these headers

The size and offset assertions are not in the headers, since nothing in the build
includes them yet and a header should not carry a test. They live in a throwaway
unit; recreate it as `check.c` with every header included and one assertion per
claim above, using the array-sized-by-condition trick, since ee-gcc 2.96 has no
`_Static_assert`:

```c
#define ASSERT_AT(line, cond) typedef char static_assert_##line[(cond) ? 1 : -1]
#define SZ(line, type, bytes) ASSERT_AT(line, sizeof(type) == (bytes))
#define OFF(line, type, field, at) \
    ASSERT_AT(line, ((char *)&((type *)0)->field - (char *)(type *)0) == (at))
```

and compile it with the project's own compiler and flags:

```sh
tools/cc.sh -Wall /path/to/check.c -o /path/to/check.o
```

A wrong number gives `size of array 'static_assert_sNN' is negative`.
