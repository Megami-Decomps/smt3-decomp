# Type recovery

[Names](names.md) covers where function and unit names come from, and
[Naming drift](naming-drift.md) audits every other kind of name. This covers
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
| The HD reconstruction, exported by `tools/hd_types.py` to `config/smt3/hd_types.txt` and `hd_enums.txt` | member names and their order for 1042 types, and 329 enums with their values; its sizes are advisory, see below |
| The IDB's own struct members, exported by `tools/ida_members.py` to `config/smt3/ida_members.txt` | 9623 named members for 1341 types, with offsets that can be trusted, since the layouts are applied at PS2 addresses |
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

## The HD corpus, and the check that keeps it in use

Invented names kept coming back because nothing looked for the recovered ones and
nothing failed when a new one appeared. Two corpora and one check close that.

`tools/hd_types.py` reads the HD reconstruction and writes two files, checked in for
the same reason `config/smt3/ida_types.txt` is, that the HD tree is not part of this
repo:

| File | What it holds |
|---|---|
| `config/smt3/hd_types.txt` | 1042 struct and union layouts, 10579 members |
| `config/smt3/hd_enums.txt` | 329 enums, 2857 members, each with its value |

**Member names and their order are what these are good for. The sizes are
advisory.** The remaster rewrote inline arrays and embedded structs as pointers, so
`u8 Name[24]` reaches the corpus as `u8 *Name`. Against the 146 sizes
`tools/header_layout.c` proves, the corpus disagreed on 45 before those errors were
corrected upstream and 22 after. Never take an offset from it; take the names.

Three tools read the corpora:

- `tools/hd_names.py <name>` is the lookup to reach for before inventing a type
  name. `-m TYPE` lists a type's HD members, `-v VALUE` finds enum members holding a
  value, `--gaps` reports which HD enums the tree has adopted.
- `tools/hd_members.py` proposes names for `unk_` members, and only where the answer
  is forced. See below.
- `tools/check_drift.py` fails the build on new drift, and runs in CI with the rest
  of `tools/test_*.py`.

### Forced, not guessed

`hd_members.py` aligns our members against the HD ones on the names both already
agree on, and treats those as anchors. Between two anchors, if our run of members is
the same length as the HD run, the pairing is forced: in `nbCamera_AllTable_t`,
`radius` anchors `unk00` and `unk04` onto `distance` and `up` with nothing else they
could be. An unequal gap is reported instead, which is most of them: 18 applied
against 66 left ambiguous.

Two guards came out of getting it wrong. A proposal read off one copy of a type must
not be written to a copy of a different shape, because
`nbCamera_AllTable_t` is three members in `nbCameraCommand.c` and two in
`nbCameraSkill.c`, where one `u8[8]` spans the first two and `distance` would have
been a lie. And where another struct in the same file declares the same member name,
the uses are left alone deliberately: an access through the renamed type stops
compiling while one through the other type still does, so the compiler attributes
each site. That found nine in `effPCPBoss.c`.

A member whose HD name is `pad`, `resv` or `dummy` is left as `unk_`, since trading
one empty name for another gains nothing.

### The IDB is the better corpus for members

The HD corpus can only order members, since its sizes are advisory. The IDB's
layouts are applied at real PS2 addresses, so its offsets can be used as keys:
`sdfMatPacket_t` is 0xa0 there and in `tools/header_layout.c`, where HD says 0x78.

`tools/ida_members.py` exports 1341 types and 9623 members, every one of them
named, to `config/smt3/ida_members.txt`. `tools/ida_fill.py` renames an `unk_`
member only when our computed offset **and** extent both equal an IDB member's, so a
`u8[8]` covering two IDB fields is refused rather than given the first one's name.

Our own `unk<NN>` names check the layout model before it is trusted: NN is the offset
the name claims, so a struct where any member disagrees with the computed offset is
skipped whole rather than half understood.

Three refusals are built in, each from getting it wrong first:

- A name already in the struct cannot be reused. `tagpipeParts` has `mdl` at offset
  6 where the IDB has it at 5, and taking the IDB's name for the member at 5 would
  declare `mdl` twice.
- A struct defined in a header is skipped. Its uses are spread across the tree and an
  offset name like `unk04` belongs to dozens of other structs, so a tree-wide rename
  would hit the wrong ones. These are reported instead.
- Where another struct in the same file declares the same member name, the uses are
  left for the compiler. An access through the renamed type stops compiling while one
  through the other type still does, which attributes each site exactly.

That recovered `fclPanel_t` as X, Y, Z, W, H, Col, Spd, Timer, Timer2, Flag and
TexHandle, and `dds3GlobalWork_t`'s `unk1CFA` as `heartsequip`. The last one
corroborated itself: `header_layout.c` already asserted a member at 0x1CFA, so the
rename had to satisfy an assertion written before it.

### Layout alone cannot recover an invented type name

`SdfPool` and `EffRecordPool` have nothing to look up, so layout is the only way in,
and it does not work. `tools/ida_rename.py` matches a struct's total size and every
member's offset and extent against the IDB. Of 258 invented types:

- 165 have no IDB layout of that shape at all,
- 84 match several, or are too plain to tell apart, a struct of four 4-byte words
  being shared by hundreds,
- and every unique match read as a coincidence between unrelated things:
  `MdlObjectAttachment` onto `fclribbon_elem_t`, `EffOwnerBillPayloadView` onto
  `sdfSortOtNode_tag`. Requiring the module to agree removed the worst of them and
  still left `SdfVuColorTransform` onto `sdfCorePacketFaceDivParm_t`.

So the tool reports leads and deliberately cannot apply them. A shape is not an
identity. What does settle one of these is the code's own use of the type, which the
next section reaches through the HD function signatures.

Their capitalization is not the problem to fix either. `SdfPool` is spelled that way
**because** it was written here rather than recovered, which is the signal this
document opens with. Renaming it to `sdfPool_t` would make an invented layout look
like a recovered one and lose that.

### What the HD bodies give that its headers do not

The HD `src/` tree is il2cpp output, so its locals are `v5` and its types carry
`_array` and `_o` suffixes, and its member accesses are often `v3[2]` on a `u64 *`.
None of that is usable. Its **function signatures** are, and they are what settles an
invented type name when layout cannot.

Our function and HD's share a name, so the types in HD's signature are the types ours
handles. `tools/hd_signatures.py` votes a candidate across the whole family of
functions that use one of our types. Two filters are what make the vote mean
anything:

- A parameter name is not a type. Counting every identifier in a signature let
  `hData` out-vote every real type in the file.
- A candidate only counts for a function whose own name carries its stem.
  `effPCPMisc.c` holds dozens of effects, so without this the types of every effect
  in the file vote for each other: `effPCPBurstWork` drew 24 votes for
  `effPCPEyeThunder_tag` purely by sharing a file with it.

Then `--check-layout` corroborates each vote against the IDB layout, and only a vote
the layout agrees with exactly is worth applying. Of 40 votes, 4 agreed in both
directions and were applied:

| ours | the IDB's | why it holds |
|---|---|---|
| `EffPCPBurstWork` | `effPCPEyeThunder_tag` | 36 bytes, every offset equal, `length` already agreeing by name, and our `EffThunderFragmentWork *fragmentWork` sits where the IDB has `effPCP_ThunderPoly_t *pThunderPoly`, the type the vote separately proposed for that second struct |
| `EffPCPAnalyzeWork` | `effPCPAnalyzeBED_tag` | 16 bytes, `scale` agreeing, both pointers corresponding |
| `EffPCPGrowWork` | `effPCPThunderSphereExp_tag` | `fadeIn`/`fadeOut` against `fade_in`/`fade_out` |
| `EffUnitObject` | `dds3P2APolygon_tag` | |

The other 36 votes were withheld, and sizing the tree's own types showed why: 28 of
them disagree on size outright. The vote alone produces false candidates freely; it
is the second direction that makes it evidence.

Recovering a type's name then licenses its members. `ida_fill.py --exact` takes every
member name, not only the `unk_` ones, where our struct's every offset and extent
equals the IDB's: 80 members across 25 structs.

### The rewrite rule that broke the build

A member use is only ever `.name` or `->name`. Rewriting a bare identifier instead
meant that renaming a member called `billboard` renamed every billboard in the file,
`#include "billboard.h"` included, and `billManager.c` stopped parsing. It had looked
survivable while only `unk00`-style names were being renamed, because those are not
words. Both fillers now rewrite only member-access positions.

What is left over are lines assigning between two structs that share a member name,
where one side is the renamed type and the other kept its own spelling:
`work->life = params->duration`. The compiler flags those, and the accessed
variable's type says which side to put back.

### The same position of the same function

This is the method that moved the invented names, and it was available all along. I
rejected positional matching early because arity and pointer depth differ between our
signatures and HD's, when the fix was to gate on arity rather than abandon it: 5463
of our functions share a name **and** a parameter count with HD's, and for those,
parameter i of ours is parameter i of theirs. The type HD names there is the type ours
handles, which is correspondence rather than the stem heuristic the family vote needed.

`tools/hd_positions.py` reads it off. Most proposals are self-evidencing, since the
function names the type: `effPCPRingPolyExp2Create`'s return and
`effPCPRingPolyExp2Init`'s first parameter both name `effPCPRingPolyExp2_tag`.

Three refusals sit on top of it, and each came from being wrong first.

**Size, as a one-way test.** A position says which type a function handles; it cannot
say our struct is that same record. 31 proposals have our struct larger than the IDB's
type and are refused, which is as likely to mean our shape is wrong as the name is.

**Structure, because size is too strict on its own.** `PcpScatterInstanceA` is 0x18c
against `effPCPWhirlwindPoly_tag`'s 0x7c purely because the IDB holds that type's
param by pointer and its part array inline where ours is the reverse. Both carry the
same seven members in the same roles, and our `dds3WhirlwindPolyList_tag` member
already agrees with the IDB's by type. So an equal member count sharing a member type
counts as well as a size that fits, which recovered seven names the size test alone
withheld.

**Containment, which shape cannot replace.** `effPCPWwSpherePolyPart_tag` and
`effPCPHamaCardSpherePart_tag` are both 32 bytes, so a layout match between them is a
coin flip, and an earlier unit-scoped match lost it. A work struct holds its own
family's part record, and ours held the other one's in both directions at once:
`PcpScatterSpinWork` and `PcpScatterPlainInstance` were reverted on that basis.
`containment_ok()` now refuses the case by the part type where our particle is already
named, and by the part's size where it is not, since
`PcpScatterPlainInstance` holds a 40-byte record where `effPCPWwSpherePoly_tag`
embeds a 32-byte part.

The three methods, the signature family vote, the unit-scoped layout and the
positional match, now disagree with each other where they are wrong instead of
agreeing silently, which is the only reason that pair was caught.

### Where the rename scope has to stop

A member use is only ever `.name` or `->name`, and even that is not enough. The check
for whether another type shares a member name has to span the whole corpus, not the
file: renaming `P2A_TEXANIM_tag`'s `height` to `sy` rewrote `tex->height` on an
`sdfTexHandle_t` because that type is declared in a header and the file-local check
could not see it.

What is left after that is lines touching two structs at once, where one side is the
renamed type and the other kept its own spelling:

    work->node->drawKind = src->blend
    spark->up_s = work->head.verticalSpeed
    sprite->radian = work->params.angleStep

The compiler attributes every one of them, so the loop is: rename, build, and read the
accessed variable's type at each site it names. Checking `ninja`'s exit status rather
than grepping its output for one message is part of that; grepping for "no member
named" hid a whole class of "undeclared" errors and reported a clean build twice.

### Scope is what makes a layout mean something

Tree-wide layout matching fails, and the reason is scope rather than method. 280 of
our 300 units carry an HD source file's name, and one file mentions only a few dozen
types, so `tools/hd_unit_types.py` matches a struct's layout against that pool
instead of all 1341. Twelve types came back that way, the strongest being
`EffPCPBeamParams` onto `effPCPRingPolyExpParam_t`: 17 members, every offset and
extent equal, and the meanings agreeing throughout (`startRadius`/`radius_s`,
`fadeInFrames`/`fade_in`, `firstColor`/`color_e_in`). `SoundFormat` turned out to be
`sdfIpuInitParm_t`, its `loopMode` agreeing by name, the old name a misreading of a
video-decode parameter block.

Two refusals are built in. A target two of our types both claim is dropped, since
they cannot both be right and renaming both would declare one name twice. And
widening the match is counterproductive: accepting our struct as a prefix of a longer
IDB type takes the forced set from twelve to zero, because the evidence is uniqueness
inside the unit and a looser test makes every match ambiguous. `--prefix` keeps that
result reproducible.

This method and the signature vote barely overlap, which is useful rather than
disappointing: the signatures find work structs, since those are what functions take,
and the unit scope finds the param and part structs beside them.

### Naming a type is what opens up its members

The two halves of the drift are one problem. A struct called `SdfPool` gives nothing
to key a lookup on, so its members cannot be looked up either, and 1754 of the
remaining members sit behind 440 such names. Conversely, the moment the twelve types
above were named, `ida_fill.py --exact` could take 102 of their members.

So the order of work is: name the type, then fill it. `ida_fill.py --why` counts what
blocks the rest.

### Enums need the same corroboration, and mostly fail it

`tools/hd_enum_sites.py` looks for the evidence a bare literal needs: a switch's case
labels are a set of values from one enum, so a set fitting exactly one enum says
which. On values alone this is worthless, exactly as it was for type names. Five
switches fit one enum each and all five were coincidences, `switch (button)` sitting
perfectly inside `DDS3_PATHBASIC_FLAG` because any four-value set sits inside dozens
of enums.

Requiring the selector's own name to share a word with the enum's cut it to three,
and reading those three settled them:

- `nbAi.c`'s `SetGroupTarget2` switches on `code` through 0..10 against `TAR_CODE`'s
  eleven members, in the `newdata.h` that file already includes. Code 0 is
  `TAR_CODE_AI`, handled alone, while the ten explicit criteria share a body.
  Adopted, and its members were already prefixed so nothing collided.
- `dds3ObjectBase.c` switches on `mode` with six cases against `POLY_MODE`'s six, and
  the bodies set `animeSpeed` from `animeMode`. It is an animation mode; `POLY_MODE`
  is `OPAQUE`/`SEMITRANS`/`ADDTRANS`. Rejected. "mode" is too generic a word to
  corroborate anything.
- `evtEventViewer.c` switches on `viewer->fileMode` against `EVT_VIEWER_MENU`. Case 5
  and case 6 corroborate exactly, frame totals against `FRAMELIST` and a frame change
  against `FRAMECHANGE`, but cases 3 and 4 raise a "FRAME SET.OK?" confirm that
  `FILENAME` and `OBJLIST` do not explain. Left as a lead: a wrong label here would
  be the same drift as an invented name.

Of the 329 HD enums, 27 are fully declared here and 297 not at all, and that is
mostly not neglect. The reconstruction left most members unprefixed, so adopting them
wholesale would put `ALL`, `IT`, `INIT`, `MIDDLE`, `START` and `END` into the global
namespace; 25 of the 297 hold names the tree already uses for something else.

### Merge duplicates before anything else

Some invented types are local copies of a type this tree already names correctly, and
`tools/merge_duplicates.py` settles those with no corpus at all: identical member
names **and types** in the same order. It runs first, because it is the only method
here that cannot be fooled by a shape.

That ordering matters. Both the unit-scoped layout and the HD-header neighbour pool
proposed `dds3MultiBillResource_tag` for `BillRenderPair`, agreeing with each other
and both wrong: it is an exact duplicate of `include/billboard.h`'s
`billRenderPair_t`. Two layout methods agreeing is not corroboration when they read
the same shape. The same mistake, made by hand, had renamed
`SdfGsDrawDefaultsRegisters` to `sdfGrRefGifPacketHeader_t` when it duplicates
`sdfGrDrawDefParmPacket_t`; the use sites said so, assigning `SDF_GS_PRMODECONT` to a
member the rename had called `dmatag`.

Two refinements it needed. Member **types** as well as names, since
`PcpScatterPlainInstance` and `effPCPWhirlwindPoly_tag` share every member name and
differ in their param types. And an offset-named member compared by its slot alone,
so two copies that left different offsets unnamed still match, which is the common
case. It cascades: merging an inner duplicate is what lets the outer one match.

### Struct locality, and where it survives

Related types sit together in the original headers, a param beside its work type
beside its parts, so a type's neighbours are a tight candidate pool.

HD's own `src/*.c` keeps none of it: those files sort their typedefs alphabetically,
and declaration order agrees with ours in only 10 of 124 shared units. The
`include/` headers do keep it, `effect.h` running
`effPCPThunderSpherePolyParam_t`, `effPCP_ThunderPolyParam_t` and
`effPCP_ThunderRadiatePolyParam_t` together, so `hd_types.py` records each type's
index in its header and `tools/hd_neighbours.py` pools the neighbours of the types a
unit already names.

It recovered nothing on its own. Its value was negative evidence: the one case it
spoke on, it agreed with another layout method and both were wrong.

### What the IDB's enums give that HD's do not

The enum half looked closed because HD's members are mostly unprefixed: adopting them
would put `ALL`, `IT`, `INIT`, `MIDDLE`, `START` and `END` into the global namespace,
and 25 of its 297 undeclared enums hold names the tree already uses. The IDB's 341
enums are the applied ones and carry their prefix, and across all 2938 members only
36 would collide. `ida_members.py` exports them.

What still does not work is matching an enum by its values. A sequential 0..N set
sits inside every enum of that length, so our `NB_PHASE` "matches" `AM_MK` and our
`TAR_CODE` matches `BE_DATATYPE`, each with not one member name in common. Nine of
our fourteen enums draw a same-length false match. What settles an enum is reading
the code: `TAR_CODE` by `SetGroupTarget2` handling code 0 alone while the ten
criteria share a body, `EVT_VIEWER_MENU` by `EnterMain` switching on the menu the
viewer is in, and `POLY_MODE` rejected because the bodies set `animeSpeed`.

### Commit each verified batch before starting the next

Nine renames were applied, built and checksummed, then lost: they were still
uncommitted when a later bad batch had to come out with
`git checkout -- src/ include/`, which took them too. Two uncommitted batches at once
is how that happens.

### What is left, and what blocks it

`tools/ida_fill.py --why` counts the remaining `unk_` members by what stops each one,
and the answer reframes the two halves of this problem as one:

| structs | members | blocked by |
|---|---|---|
| 440 | 1754 | no IDB type of that name |
| 162 | 1174 | IDB has no usable name at that exact offset and extent |
| 73 | 369 | our struct is larger than the IDB's |
| 51 | 293 | our offsets disagree with our own `unk` names |
| 21 | 97 | a member type we cannot size |

The largest bucket is the invented-type problem wearing another hat. A struct called
`SdfPool` gives nothing to key on, so its members cannot be looked up either:
recovering one type's identity is what unlocks its members, and 1754 of the remaining
members sit behind 440 such names. That is why the type names matter beyond style,
and why they have to be settled from the code one at a time.

The second bucket is different and is real work rather than a dead end: the type is
known and the offsets line up, but our member is coarser than the IDB's, a
`u8 unk_15[3]` standing where the IDB has three named fields. Splitting one is a
layout change rather than a rename, so it can change codegen and has to be verified
against the image rather than assumed.

The fourth bucket is worth auditing before trusting: 51 structs name members after
offsets that are not where this project's own layout computation puts them. Either
the name is stale or the struct has a packing rule not modelled here.

### What the baseline is for

`config/smt3/drift_baseline.txt` has two halves, checked in opposite directions.

`type` and `member` rows list the invented names that existed when the check landed,
258 types and 618 members. A name **not** in the baseline fails, so the tree as it
stands passes and only new drift breaks the build. The half only ever shrinks: fix
one, then `--update`.

`enum` rows list the 481 HD enum members the tree has adopted. One of these going
**missing** fails, so a constant that has been named cannot quietly go back to a
literal. Adoption counts either spelling: the reconstruction strips an enum's prefix
off its members, and the tree re-adds it, so `NB_PHASE`'s `NON` is declared here as
`NB_PHASE_NON` because a bare `NON` would collide.

### Where the enums stand

Of the 329 HD enums, 27 are fully declared here, 5 in part, and 297 not at all. That
is mostly not neglect. The reconstruction left its enum members unprefixed, so
adopting them wholesale would put `ALL`, `IT`, `INIT`, `MIDDLE`, `START` and `END`
into the global namespace: of the 297 undeclared, 25 hold names the tree already
uses for something else, `SDF_PADMAP` alone holding 12. The five partial ones are the
real backlog and `hd_names.py --gaps` lists them.

Seven enums are marked `merged` in the corpus, meaning their values repeat because
the reconstruction ran several enums together. `SDF_PADMAP` holds three, so its value
4 is both `SDF_PADMAP_L` and `L` and nothing picks between them. A name out of a
merged enum is a lead, not an answer, and `-v` says so when it offers one.

### Errors corrected in the HD tree

The structural errors this project can prove were patched upstream rather than worked
around, since every later sweep reads the same corpus. Each fix rests on a size
assertion in `tools/header_layout.c`, which compiles and links to a byte-identical
image:

- `sdfGrTagStdReg_t` carried a spurious leading `void *tex`, putting `data` at 8
  where it is proven to sit at 0 in a 16-byte struct. Eleven packet types embed it,
  so this one error moved all of them.
- 24 embedded structs written as pointers, nearly all GS and GIF register writes
  inside hardware packets, where a pointer would make the entry 4 bytes instead of 16
  and the hardware would read the wrong words.
- 12 lost array extents, including `scrProcTable_t.Name` and
  `scrLabelTable_t.Name` (`u8 *Name` for a proven `u8 Name[24]`) and
  `scrProcessWork_s.StackData` (28 entries).

Agreement with the proven layouts went from 47 of 146 sizes to 70, and from 241 of
602 offsets to 293. Both mechanical classes are now empty.

What was deliberately left: 22 types whose size still disagrees through a difference
in member count alone. Some are the remaster's own additions, which are not errors to
remove, such as `dds3Basic_t`'s trailing `void *gameObject`. The rest need individual
evidence rather than a rule.

## Verifying

A type rename changes no codegen, so the image must still hash to
`5facfbf7916836613449c922e26a4ec412207b32` afterwards. A layout change can
change codegen: if the image stops matching, either the new layout is wrong or
the C was written against the invented one. Both need the function read, not a
revert and a guess.
