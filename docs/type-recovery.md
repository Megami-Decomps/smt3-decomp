# Type recovery

[Names](names.md) covers where function and unit names come from. This covers
struct, union and enum types, which have a different problem: early
decompilation work invented types instead of using the recovered ones, and an
invented type encodes wrong field offsets and sizes into the source where an
invented function name only misleads a reader.

The tell is capitalization. Engine types are lowerCamel with a module prefix
and a `_t` suffix (`sdfMat_t`, `dds3ModelHandle_t`). A type in this repo spelled
`SdfAsset` or `Dds3ModelPart` was written by a contributor, not recovered.
Capitalization is a signal, not proof: some invented types are spelled
correctly, and a type can carry the right name over a guessed layout.

## Sources

Same order of authority as [IDB corrections](idb-corrections.md).

| Source | What it gives |
|---|---|
| The `SLUS_209.11` IDB | structs already applied at PS2 addresses with layouts and field names, reachable through function prototypes |
| SMT3 HD symbol recovery, which the IDB carries | most type and enum names, including PS2-specific ones such as much of the sdf rendering layer |
| [010 Editor templates](https://github.com/tge-was-taken/010-Editor-Templates/tree/master/templates), `dds3_*.bt` | on-disk layouts for assets: models, textures, motions, packs |

## Matching a repo type to an IDB type

Reading names and guessing is how the invented types got here. Match
mechanically instead:

1. Extract every function prototype in the tree and pull the IDB prototype for
   each function that resolves. 2,860 prototypes exist; 2,734 resolve.
2. Vote per parameter slot: each `(repo type, IDB type)` pair in the same
   position of the same function is one vote. A type used in many prototypes
   accumulates a strong majority for its counterpart.
3. Cross-check the winner by compiling the repo struct with the project's own
   ee-gcc and comparing `sizeof` against the IDB layout.

A vote with no size agreement is not a match. The vote is also uninformative
wherever the IDB prototype has `void *` in the slot, which is why the `effPCP*`
types below stay unresolved.

### Two systematic offsets

Sizes do not compare directly, because the IDB layouts come from the HD build:

- **HD pointerization.** `sdfPrim*Elem_t` and `sdfPrim*Packet_t` hold
  `sdfGifTag_t *` and `sdfDmaTag_t *` where PS2 has the tag inline, so the PS2
  struct is the IDB size plus 8, or plus 16 with two tags. This accounts for
  every apparent mismatch in the GS primitive family.
- **HD-grown tails**, already recorded in [IDB corrections](idb-corrections.md):
  `sdfTexHandle_t` gains `hashname` and `image`, `TMX_HEADER_t` ends in a
  variable-length `userArea` and `comment`.

Subtract both before calling a size a mismatch.

## Renamed

64 types, each with a prototype majority and an agreeing size. Grouped by
subsystem; sizes are PS2 sizes after the adjustments above.

| Repo | Canonical | Evidence |
|---|---|---|
| `SdfListHead` | `sdfDrawTag_t` | 77 of 79 votes, 32 = 32 |
| `SdfLinkedPacketList` | `sdfPreDrawTag_t` | 10 of 10, 16 = 16 |
| `SdfPoolNode` | `sdfGrOt_t` | 8 of 9, 32 = 32 |
| `SdfPendingRequest`, `SdfPendingNode` | `sdfDelayedResrcCB_t`, `sdfDelayedResrcChunk_t` | unanimous, 8 = 8 |
| `SdfDmaNode` | `sdfGrRefPacketHeader_t` | 4 of 4, 32 = 32 |
| `SdfDescriptorPacket`, `SdfResourcePacket` | `sdfLoadImagePacket_t`, `sdfStoreImagePacket_t` | unanimous, 176 and 240 exact |
| `SdfImageTransfer{Packet,Registers}` | `sdfPrimBitBlt{Packet,Elem}_t` | unanimous, 96 and 64 exact |
| `SdfGsBlend{Packet,Registers}` | `sdfPrimTrans{Packet,Elem}_t` | 16 of 16, 64 and 32 exact |
| `SdfGsCenteredBoundsRegisters` | `sdfGrClearPacket_t` | 80 = 80 |
| `SdfGsContext{Packet,Registers}` | `sdfGrDrawFrame_t`, `sdfGrDrawFramePacket_t` | 96 and 64 exact |
| `SdfGsTextureFlushPacket` | `sdfPrimTexflushPacket_t` | 48 = 8 plus two inline tags |
| `SdfGsClosedRectangle{Payload,Packet}` | `sdfPrimBoxLine{Elem,Packet}_t` | 72 = 72 |
| `SdfGs{Flat,Gouraud,Textured,GouraudTextured}{Triangle,Quad,Pair}{Payload,Packet}`, 18 types | `sdfPrim{F3,F4,G3,G4,Ft3,Ft4,Gt3,Gt4,Sprite}{Elem,Packet}_t` | member for member once the inline giftag is accounted for |
| `SdfGsRegisterWrite` | `sdfGrTagStdReg_t` | 16 = 16 |
| `SdfGsPacketHeader` | `sdfGrPacketHeader_t` | 32 = `sdfDmaTag_t` plus `sdfGifTag_t` |
| `SdfTex` | `sdfTexHandle_t` | 9 of 9, 64 = the `libsdf.h` body |
| `SdfTexBuf`, `SdfTexResource` | `sdfTexPacket_t`, `sdfGsMemSlice_t` | 10 of 10, 64 and 28 exact |
| `SdfMemBlock` | `sdfMemHandle_t` | 11 of 11, 16 = 16 |
| `SdfThreadNode` | `sdfThCB_t` | 8 = 8 |
| `SdfTextureFileHeader` | `TMX_HEADER_t` | `sdfTmxSetHeader` (0x002C77A0) does `memset(h, 0, 0x40)` then writes `fileType` and `formatId` |
| `SdfGraphObj` | `sdfGrFrameSet_t` | 4 of 4, 20 = 20 |
| `SdfKeyTreeNode` | `sdfSortOtNode_tag` | 10 of 10, 16 = 16 |
| `SdfPacketBuilder` | `sdfGrFramePacket_t` | 96 = 96 |
| `SdfStreamFrameNode` | `sdfIpuDecodeCb_t` | 140 = 140 |
| `DevState` | `sdfFileCmdHandle_t` | 4 of 4, 64 = 64, field for field against [IDB corrections](idb-corrections.md) |
| `SdfAsset`, `SdfAssetEntry`, `SdfSubParam` | `sdfMat_t`, `sdfMatPacket_t`, `sdfMatUvwParm_t` | 13 of 13, 72, 160 and 24 exact, field for field |
| `SdfModel` | `sdfModelRootNode_t` | 5 of 5, first 0x98 field for field |
| `SdfProjectionRecord` | `sdf3DScreenParam_t` | 4 of 4, 160 = 160 |
| `ConsFrustumPacket`, `ConsMatrixPacket` | `sdf3DFogPacket_t`, `sdf3DViewportPacket_t` | 80 and 208 exact |
| `KwlnDrawVectorParams` | `sdf3DFogParam_t` | 20 = 20 |
| `VuLightingPacket` | `sdf3DTmPacketData_t` | 128 = 128 |
| `Motion` | `sdfMotionCtrlr_t` | 52 = 52 |
| `RwV3d`, `RwMatrix` | `sdfVector3_t`, `sdfMatrix4_t` | 12 and 64; RenderWare names in an sdf engine |
| `MovObj` | `movieStream_t` | 10 of 19, 176 = 176 |
| `PcpScatterPool` | `dds3ScatterPolyList_tag` | 5 of 5 |
| `EffParamWork` | `effPartHandle_t` | 7 of 7, 8 = `{u16 type; void *pHandle;}` |

## Layout corrections

- `sdfTexHandle_t` and `sdfGsMemSlice_t` had **`next` before `prev`**. The IDB,
  `include/libsdf.h` and the decompiled `src/smt3/code_002AA350.c`, which walks
  the GS slice list through `slice->prev->next`, all put `prev` at +0. Swapped
  in all seven copies.
- `sdfTexHandle_t+8` was `SdfTexRef *reference` pointing at an invented 8-byte
  `{SdfTex *cloneSource; s32 refCount;}`. It is `sdfTexHandle_t *origin`. The
  dead `SdfTexRef` is gone from all seven files.
- Two local duplicates that collided with `libsdf.h` once renamed are deleted:
  `sdfMemHandle_t` in `dds3EffectObjectBasic.c` and `sdfTexHandle_t` in
  `billManager.c`. Neither accessed a member.

## Examined and left alone

A vote without an agreeing size is not evidence. None of these were renamed,
and none should be renamed to a guess.

| Type | Why |
|---|---|
| `SdfPool`, 28 bytes | votes `sdfGrOt_t`, 32 bytes; trailing `keyTree[1]` against `data` plus pad is unresolved |
| `SdfKeyPoolNode`, `SdfKeyTreeItem`, 16 bytes | vote `sdfDrawTag_t`, 32 bytes |
| `SdfPacDecoder`, 20 bytes | votes `sdfDecompCb_t`, 32 bytes |
| `PacState`, 56 bytes | votes `sdf_PacDecode2_sdfPac2DecodeCb_t`, 64 bytes |
| `SdfPacketChain`, 8 bytes | `sdfPreDrawChain_t` is 16; the pointers match, the tail does not |
| `SdfPacketPatchLink`, `SdfPatchableResourcePacket`, `SdfGraphCopyPacket` | vote `sdfPreDrawHook_t` and `sdfGrStdDrawEnv_t`; 8, 256 and 112 against 16 and 544 |
| `SdfGsTwoVertex{Payload,Packet}` | the vote splits evenly between `sdfPrimRectElem_t` and `sdfPrimLineElem_t`, which have identical shape |
| `SdfMotionIndexedBinding`, `SdfMotionBindingDispatch` | 16 and 24 against `sdfMotNodeMatBase_t` at 20 and `sdfMotionNodeVTBL_t` at 20 with five slots to the repo's six |
| `BillObj`, 100 bytes | votes `billHandle_t`, 48 bytes; almost certainly the owner struct, not the handle |
| `SoundMixer`, 3140 bytes | votes `effBTL_t`, 16 bytes, from a pointer parameter rather than the type |
| `ParSystem`, `PcpScatterDraw`, `PcpScatterRes` | `dds3TrackPoly_t`, `dds3WhirlwindPolyList_tag` and `dds3ScatterPolyListTexResource_tag` carry no layout in the IDB |
| `SdfCamera`, `DevRequest`, `SdfDrawNode`, `SdfChunkHeader`, `VuWork`, `IpuDmaState`, `Pac*`, `Mov{Linear,Pac}Stream`, `SoundFormat`, `McPollResult`, `EvtUnit`, `NB_FILEHANDLE_T` | no counterpart found, or no evidence either way |

### The effPCP block

Roughly 250 `Eff*`, `PcpFlash*`, `PcpScatter*`, `Bill*` and `Mdl*` work and
params types in `effPCP{Misc,Flash,Scatter,Thunder,Boss}.c` are untouched. The
IDB prototype for their functions has `void *` in the matching slot, so the
vote carries no information. They are almost all invented and the canonical
`effPCP*_tag` and `effPCP*Param_t` names exist in the IDB, but pairing them
needs per-function decompilation rather than a prototype diff. This is the
largest remaining block.

## Open discrepancies

Found while matching, not acted on. None is a case of the retail code
contradicting an IDB name: where the IDB and this repo disagreed, the IDB was
right.

1. **`sdfModelRootNode_t`'s tail contradicts [IDB corrections](idb-corrections.md).**
   The repo body is 156 bytes, with `lodIndex` at +0x98 and `pad99[3]` after it.
   That doc proves the allocation is 152, from `sdfModelRootNew` (0x002B0540)
   calling `sdfChipAllocZeroDirect(152)`, which puts `lodIndex` past the end of
   the object. One of the two is wrong, so the "native model-viewer and
   `MODEL_LOD_CHG` setters use byte 0x98" claim on that field needs rechecking.
2. **`MdlCtx` appears to drop a field.** It matches `dds3ModelHandle_t` through
   +0x30, `stat`, `next`, `prev`, `resrc`, `motionMajorNum`, `motionMinorNum`,
   `effects`, `modelRoot`, `motionCtrlr`, `motions[4]`, `blur`. The IDB then has
   `dds3Basic_t *Owner` at +0x34 and `radius` at +0x38, where the repo has
   `objectRadius` at +0x34.
3. **`VuWork` is a prefix of `corePktCb_t`.** 19 of 19 votes and the first 0xB0
   match field for field, `head[64]` and `matrix[4][4]`, `faceCount` at +0x40,
   `matPacketPtr` at +0x54. The repo struct stops at 0xB0 and `corePktCb_t` is
   320 bytes, so the repo model is incomplete rather than wrong. Not renamed.
4. **`sdfModelRootNode_t`'s five list pointers are one repo type.** The IDB
   distinguishes `nodeList`, `headNode`, `texList`, `matList` and `morphList` as
   `sdfModelNodeList_t *`, `sdfModelNode_t *`, `sdfModelTexList_t *`,
   `sdfMatList_t *` and `sdfModelMorphList_t *`. The repo types three of them as
   `DevRequest *`, an unrelated invented name.
5. **`BillChildPayload` has one list too many for the native resource.**
   Its five `pendingLists` slots move the allocation, batch and next pointers
   to +0x44/+0x48/+0x4C. The shared `dds3BillResource_tag` has four slots and
   those pointers at +0x40/+0x44/+0x48. Retail `BillboardLoadTex` at 0x0015B1B0
   allocates 0x44C bytes and puts the resource after the 0x400-byte batch,
   leaving 0x4C bytes, then stores exactly those shared-header offsets.
   `dds3MultiBillboardPktLink` at 0x0015BF20 also loads both batch pointers
   from +0x44. These two ports remain `INCLUDE_ASM` while the upstream local
   layout is preserved; a second four-list local type is not introduced.

## Local duplicates

`code_002AC138.c`, `code_002B5CD0.c` and `code_002C6F38.c` retain local
`sdfDrawTag_t`, `sdfTexHandle_t`, `sdfMat_t`, `sdfGrOt_t` and other engine-owner
layouts. Their field names differ from `include/libsdf.h` (`first` and `last`
against `head` and `tail`, `address` against `ptr`, `word` against `addr`).
Merging those owners onto the shared headers needs field renames at about
500 call sites and is independent of a type rename.

The GS foundations are now consolidated too: the three units use
`sdf_types.h`'s sole `sdfGrTagStdReg_t` and `sdfGrPacketHeader_t` definitions.
Register writes use `data`/`reg`; headers use `dmatag`/`giftag`. The header's
`dmaWords.tag` and `dmaWords.vif` views describe the same hardware words as
`dmatag`'s decoded fields, without adding bytes or alternate local layouts.
The shared register record keeps upstream's two scalar `u64` fields. Only
`sdfGrClearPacket_t` overlays its RGBAQ slot with a `u32 clearColor` view for
the native partial-word update; a union in every register's data field changes
the upstream constructors' aliasing and scheduling.

The math and DMA foundations are different: `sdf_types.h` holds their sole
shared definitions, and `sdf_lighting.h` holds the canonical parallel-light
packet. `code_002B5CD0.c` includes that leaf header rather than retaining its
invented lighting record, RenderWare matrix or vector copies. The shared
packet exposes its matrix as `sdfMatrix4_t` and its indexed lighting columns
as `normalComponents`, without changing the vector type or alignment.

## Verifying

A type rename changes no codegen, so the image must still hash to
`5facfbf7916836613449c922e26a4ec412207b32` afterwards. A layout change can
change codegen: if the image stops matching, either the new layout is wrong or
the C was written against the invented one. Both need the function read, not a
revert and a guess.
