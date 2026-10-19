# Naming drift

[Type recovery](type-recovery.md) replaced 64 invented struct *type* names with
the recovered ones. This is the audit of everything else a name can be: struct
and union members, enums and their members, `#define`s, functions and globals.

A type can carry the right name over invented field names, so a correct type
list is not evidence that the members are right. The three audits behind this
page matched the tree against the IDB mechanically, the same way
[type recovery](type-recovery.md) describes, rather than by reading names.

## The rule this page is built on

Three cases, kept apart. Blurring them is what produced the drift in the first
place.

- **(a) A source proves a different name.** The IDB layout, a `dds3_*.bt`
  template, or the retail code itself. This is drift; fix it.
- **(b) The repo breaks a stated convention but no canonical name exists.** A
  style fix at most. Never invent a replacement name to satisfy a rule.
- **(c) No evidence either way.** Leave it. An `unkNN` that nothing explains is
  more honest than a confident guess.

Renaming a member, enum member or `#define` changes no codegen, so each batch is
verifiable by rebuilding to `5facfbf7916836613449c922e26a4ec412207b32`.
Replacing a *literal* with a named constant can change codegen if the value or
type differs, so those need a build check one at a time.

## What is not drift

Two findings are worth stating up front, because both look like large problems
and neither is one.

**Function and global names do not drift.** Every name the repo emits, 9,700
`FUNC` symbols covering C definitions, `INCLUDE_ASM` entries and splat symbols,
was compared against the IDB name at the same address. 9,469 are byte-identical.
The remaining 231 are `func_<addr>`/`jtbl_<addr>` placeholders at addresses where
the IDB has only `sub_*`, `nullsub_*`, `loc_*` or no function at all. No
placeholder sits where the IDB has a real name, and no prototype disagrees with
its definition. Of 5,817 `OBJECT` symbols, no `D_<addr>` sits at an address the
IDB names; the 283 differences are splat's `jtbl_` against IDA's `jpt_` for the
same jump tables. `config/smt3/symbol_addrs.txt` is current.

**About 2,800 function names break the convention in
[Contributing](CONTRIBUTING.md#naming-and-type-conventions), and every one is the
original IDB name** (`CheckAction_AI`, `Eve_BlurON`, `camp_update_dh_Process`,
`SdfIopInit`). Nothing here should be renamed to satisfy the rule. The rule
governs names a contributor invents, not recovered ones.

## Member name drift

Eight source files include only `common.h` and `ps2asm.h` and define the sdf
engine types locally: `code_002AC138.c`, `code_002B5CD0.c`, `code_002BD340.c`,
`code_002C6F38.c` and four `effPCP*.c`. (Type recovery recorded three of these;
the four `effPCP*` files and `code_002BD340.c` were missed.) For every one,
`include/libsdf.h`, `draw.h` or `draw3d.h` already agrees with the IDB
member-for-member, so the local copies are provably the drifted ones.

22 types are affected. The widest are `sdfFileCmdHandle_t` (17 members,
~193 sites), `sdfIpuDecodeCb_t` (40 of 43 members, ~170 sites), `movieStream_t`
(~104), `sdfMotionCtrlr_t` (16 members, ~55) and `sdfModelRootNode_t` (17
members, ~38). The narrow ones are the ones to do first: `sdfMemHandle_t`
(`address`→`ptr`, `state`→`stat`, `referenceCount`→`mlock`), `sdfGsMemSlice_t`
(`word`→`addr`), `sdfDrawTag_t` (`first`/`last`→`head`/`tail`) and `sdfGrOt_t`
(`first`/`last`→`tagHead`/`tagTail`).

The prize is not the names. Merging the eight local copies onto the shared
headers deletes about 1,400 lines of duplicated type definitions, and the
renames are what make the merge possible.

### Four single-field drifts in the shared headers

The cheapest fixes anywhere in this audit, and they prove the no-codegen claim
on the first rebuild.

| File | Type | Repo → canonical | Sites |
|---|---|---|---|
| `include/libsdf.h` | `sdfMatPacket_t` | `multiTexTm` → `mtexTexTm` | 1 |
| `include/libsdf.h` | `sdfDelayedResrcCB_t` | `freeFunc` → `freeFuncPtr` | 0 |
| `include/libsdf.h` | `sdfThCB_t` | `threadId` → `thId` | 9 |
| `include/draw.h` | `sdfPrimTexflushPacket_t` | `elem` → `texflush` | 0 |

### Two traps

**`sdf3DFogParam_t` in `code_002B5CD0.c` names +0 and +4 `fogNear` and
`fogFar`, while the canonical `fogNear` and `fogFar` are at +8 and +12.** A
positional rename silently swaps two float pairs.

**`dds3Polygon_t` in `polyManager.c` is shifted by one slot against the IDB
offsets.** The member names are drifted, but the mapping has to come from
offsets, not member index. Same for the GS primitive packets in
`code_002AC138.c`: the repo holds DMA and GIF tags inline where the IDB points
at them, and compresses several canonical registers into one array member, so a
positional rename would be wrong. That is about 1,500 of the 4,065 counted
sites, and each type needs checking individually.

### Where the repo is better than the IDB

`corePktCb_t`'s repo names beat the IDB's `unk120` placeholders. So do
`sdfGrDrawState_t`'s `pad` against IDA's `field_C`, and `datUnitVisual_t`'s
members against `reserve1..3`. An IDA placeholder is not a recovered name; do
not "correct" these backwards.

## Enum and define drift

Every enum in the tree whose members satisfy the uppercase-tag-prefix rule came
from the IDB, and every invented one breaks it. The convention violation and the
invention are the same set, which makes the rule a usable filter.

The IDB carries 341 named enums, and `dds3_model.bt` independently reproduces
about ten of them with identical spellings and values, so the model-layer
findings have two agreeing sources.

Ten high-confidence renames, all unreferenced or referenced only by name:
`sdf_core_attr`→`SDF_ATTRBIT_e`, `sdf_core_packet_bit`→`SDF_PACKETBIT_e`,
`sdf_model_root_flag`→`SDF_MODEL_FLAG_e`,
`sdf_model_node_flag`→`SDF_MODEL_NODE_FLAG_e`,
`sdf_shape_packet_type`→`PACKET_TYPE_e`,
`sdf_model_packet_type`→`MODEL_PACKET_HEADER_TYPE_e`,
`MdlViewerRecordKind`→`PIB_CHUNK_TYPE`, `MdlResourceKind`→`dds3ModelEffectType`,
`sdf_image_set_free_mode` and `sdfImageOwnership`→`SdfImageSetMem`,
`dds3_billboard_draw_mode`→`BILL_DRAW`.

The file-I/O enums (`sdf_dev_operation`→`SdfFileIOCmd`,
`SDF_DEV_EVENT_*`→`SdfFileIOCmdClbk`) and the pac decoder trio
(→`sdfDecomp_enum{,2,3}`) are exact value-for-value matches with every name
different. Type recovery mapped the owning type (`DevState`→`sdfFileCmdHandle_t`)
and missed its enums.

### Three corrections of meaning, not spelling

These change what the code means, so they matter more than the renames.

- **`sdf_core_attr` has bits 0 and 1 swapped.** The repo says 0x1 is the refmap
  pass and 0x2 specular; the IDB and `dds3_model.bt` both say the opposite. The
  enum has no users, so nothing in the repo corroborates its version.
- **`sdf_pac_command` member 1 is `MEM` in the repo and `EFFECT` canonically.**
  `mdlManager.c:1440` and `:1456` settle it for canonical: `command == 1` leads
  to the `PIB0`, `P2A` and `D3P` chunks.
- **`sdf_dev_state` reads 7 as active and 9 as inactive** where canonical has
  suspend and error. Canonical is a coherent state machine; the repo's is not.

One finding is deliberately not settled: `EFFBTL_PART_TYPE_BTL` 0xFFFD against
the IDB's `DDS3EFFPART_TYPE::BED`. The repo's branch calls `effBTLCreate`, so it
needs the dispatch read rather than a rename.

### Duplication

Worse here than in the type layer. `EFF_WORLD_KIND_*` is copy-pasted into three
files, the GS register list exists three times, the MC poll result and motion
state enums each exist twice under different names, and track topology exists
twice **with conflicting values**. There are 7 names for `0x80808080`, 4 for π
and 3 float spellings of 2π.

**The three 2π spellings are not one constant, and merging them breaks the
image.** `6.2831853f` is `0x40C90FDB`; `6.2831850051879883f` and `6.2831852f`
are both `0x40C90FDA`. Retail carries both values, one ULP apart, so only the
second pair can share a name. The four π spellings (`3.14159265f` and friends)
do all round to `0x40490FDB` and are genuinely one constant. Check the bit
pattern, not the digits, before merging any float constant.

### Magic constants worth naming

The `PIB0`, `P2A` and `D3P` fourCCs sit bare in `mdlManager.c`; GS register
addresses are re-spelled as `ITF_TEST_FIRST_CONTEXT` and similar in
`code_001C9530.c`; DMA tag IDs are restated as pre-shifted literals in
`code_002AC138.c`. These are the only items in this page that can change
codegen, so each needs its own build check.

### Left alone

All GS, DMA and PSM hardware enums: the IDB has no GS register enum at all.
Also `sdf_mem_stat`, `sdf_motion_stat`, `sdf_3d_screen_flag`, the pac flag
enums, the `effPCP*` enums, `include/ps2asm.h` (its macros wrap single
instructions where the SDK has differently-shaped inline functions, and the
header documents that choice) and `include/macro.inc` (standard splat
conventions).

## Loose ends in the tree

- Six invented function names are declared and never defined or called, in the
  context preambles of `code_002B5CD0.c`, `code_002AC138.c` and
  `code_002C6F38.c`: `sdfQuatToAxisAngle`, `sdfQuatFromAxisAngle`,
  `sdfPadRequestMode`, `sdfDevResizeBufferedRequest`,
  `btlInitializeActorSlotOrder`, `sdfSoundInitFormattedAndAppendNode`. None
  exists in the IDB. Delete them; there is no address to resolve them against.
- 11 contributor-written `static inline` helpers use prefix+CamelCase where the
  convention wants TitleCase. These are the only genuinely fixable convention
  violations, because they have no IDB counterpart. Two also spell the module
  prefix `effPcp` against 1,690 uses of `effPCP`.
- 11 parameters break the `a`-prefix pattern, all in `sdfPacDecode.c`
  (`state`/`work` where the IDB says `cb`/`resrc`) and `code_002BD340.c`
  (`angle`, `blendAmount` where the IDB says `aAngle`, `aRatio`). The split
  itself is not drift: every `src/smt3/sdf/` file uses the prefix and no other
  file does, mirroring the IDB, where 75% of `sdf*` prototype parameters are
  `a`-prefixed against 1.8% elsewhere.
- `tools/ida_symbols.py` drops 40 named globals because it filters on
  `has_user_name`, false for IDA's `aFooBar` string names. Most are IDA
  propagating a parameter name onto a global (`b`, `c`, `size`, `pos`), but nine
  read like original identifiers worth curating: `FoceScript`,
  `hDistortionBlur`, `hCAMPBadEffect`, `pointResName`, `MidiNo`, `shutdownfunc`,
  `mccb`, `hEffPart`, `hPartialMultiBlur`.

## An IDB correction

`include/kernel.h`'s `dds3ProcessID_t` disagrees with the IDB at +48 and +52,
and **the repo is right**. `RunProcess` (0x00100A48) calls through +48 and
stores a non-zero return back into it; `KillProcess` (0x00100D70) calls through
+52; `dds3StartProcess` takes both callbacks as separate parameters. The IDB
names those slots `id` and `processfunc`; they are `processfunc` and
`shutdownfunc`. Both structs are 80 bytes, so this is a naming error, not a
layout difference, and it changes no row of `symbol_addrs.txt`. Under the
CLAUDE.md exception the fix belongs in the IDB. See
[IDB corrections](idb-corrections.md).

## Order of work

Member renames total about 4,065 sites across 134 (type, variant) sets, counted
by `->field` and `.field` occurrences, which over-counts common names like
`.next` and `.state`. Treat it as an upper bound.

Steps 1 and 2 are done, along with the loose ends and the three meaning
corrections; see [What has been applied](#what-has-been-applied). What is left:

1. Done. Every type defined both in a shared header and in a `.c` file now has
   one spelling, bar `datUnitVisual_t`: `nbMakePacket.c` holds a truncated view
   of it that stops after `motion`, which the header merge should replace with
   the `include/newdata.h` type rather than a rename.
2. The narrow shared types: done except `sdfMat_t`, whose only local copy is
   in `sdf3DPrim.c`. `sdfThCB_t`, `sdfMemHandle_t`, `sdfGsMemSlice_t`,
   `sdfDrawTag_t`, `sdfGrOt_t`, `sdfPreDrawTag_t`, `sdfTexHandle_t` and
   `TMX_HEADER_t` now match the shared headers member for member, which is what
   the header merge was waiting on. `sdfGrTagStdReg_t` needed nothing: its only
   definition is in `include/sdf_types.h` and already agrees.
3. The enums used in live switches, then the wide records
   (`sdfFileCmdHandle_t`, `sdfIpuDecodeCb_t`, `movieStream_t`,
   `sdfMotionCtrlr_t`, `sdfModelRootNode_t`).
4. The layout-shifted packets, per type, each checked against IDB offsets rather
   than member index.
5. The remaining magic constants, one literal at a time with a build check
   each. The `PIB0`, `P2A` and `D3P` fourCCs in `mdlManager.c` are still bare.
6. Offsets off a `u8 *` where a struct member exists. `sdfBillBoard.c`'s 133
   sites are done; `code_001FAE80.c` (44) and `nbMakePacket.c` (13) are the
   next largest, and each needs the owning type recovered first rather than a
   field name invented for it.

## What has been applied

Every batch below rebuilt to `5facfbf7916836613449c922e26a4ec412207b32`.

- **The four single-field header renames.** `sdfMatPacket_t.multiTexTm` →
  `mtexTexTm`, `sdfDelayedResrcCB_t.freeFunc` → `freeFuncPtr`,
  `sdfThCB_t.threadId` → `thId`, `sdfPrimTexflushPacket_t.elem` → `texflush`.
- **The ten enum renames**, each taken from the IDB member-for-member, with the
  members the repo was missing added: `SDF_ATTRBIT_e`, `SDF_PACKETBIT_e`,
  `SDF_MODEL_FLAG_e`, `SDF_MODEL_NODE_FLAG_e`, `PACKET_TYPE_e`,
  `MODEL_PACKET_HEADER_TYPE_e`, `PIB_CHUNK_TYPE`, `dds3ModelEffectType`,
  `SdfImageSetMem`, `BILL_DRAW`.
- **The file-I/O and pac enums**: `sdf_dev_operation` → `SdfFileIOCmd`,
  `sdf_dev_state` → `SdfFileIOCmdStat`, the `SDF_DEV_EVENT_*` defines in
  `sdfTmx.c` → the `SdfFileIOCmdClbk` enum, `sdf_pac_command` →
  `DDS3_FILE_TYPE`, and the decoder trio → `sdfDecomp_enum{,2,3}`. The two
  masks the IDB has no member for keep repo names under their own anonymous
  enum.
- **The static inline helpers** now follow the TitleCase-no-prefix rule for
  private functions, which also removes the two `effPcp` spellings. `fsqrtf`,
  `frsqrtf` and `ffabsf` are left alone: those spell the standard math names,
  not a module prefix.
- **`sdfPacDecode.c`'s parameter drift.** Seven functions declared the
  parameter `aState` and then defined it as `state`; the file is otherwise
  `aState`/`aWork` throughout, so the stragglers were brought in line. The IDB
  spells these `cb` and `resrc`, against the repo's `sdfPacState_t` and
  `sdfPacWork_t`; that rename belongs with the type rename, not before it.
- **Eight IDB-named globals** that `tools/ida_symbols.py` drops, curated into
  `symbol_addrs.txt` above the generated block: `mccb`, `MidiNo`,
  `pointResName`, `hEffPart`, `hDistortionBlur`, `hPartialMultiBlur`,
  `hCAMPBadEffect`, `shutdownfunc`. 190 `D_<addr>` references became names,
  `mccb` alone covering 147 of them. The cause is the exporter's
  `has_user_name` filter: IDA set these names itself, so the flag is false even
  though they are not dummy names. Loosening the filter is not the fix, because
  most of the 40 it drops are IDA propagating a parameter name (`b`, `c`,
  `size`, `pos`) onto a global. `FoceScript` is the ninth candidate and was
  rejected: the bytes at `0x00531F28` are the string `"elvstart_label"`.
- **The explicit asm in `code_002C7CB0.c`.** The two `__asm__("")` sibcall
  barriers in `registerAtExit` and `libc_2D25B0` became the dead store to a
  parameter that [matching levers](matching-levers.md) prescribes, and the four
  `nop`s standing in for the gap before the handwritten syscall stubs became
  `.space 16`, which is what those 16 bytes are in retail. Neither is a lever
  the image depends on any more.
- **The `fptodp` equate**, which five units each spelled out with its own
  comment, is now one `EQUATE_FPTODP()` in `include/ps2asm.h`. It cannot be
  shared through `.globl`: the equate aliases an extern, so every unit that
  passes a float to a varargs call needs its own.
- **The narrow shared types.** The local copies of `sdfMemHandle_t` (8
  files), `sdfGsMemSlice_t` (12), `sdfDrawTag_t` (6), `sdfGrOt_t` (4),
  `sdfPreDrawTag_t` (3), `sdfTexHandle_t` (5) and `TMX_HEADER_t` (5) now carry
  the shared headers' names, which were already the IDB's. Only names moved:
  every declared type was left as the matching build had it, which is what
  caught the `pixelFormat` signedness. The member accesses were found by
  renaming the struct body and letting ee-gcc name the lines, then resolving
  each line's object through its nearest declaration, because `first`, `last`
  and `unkC` mean different members on `sdfDrawTag_t`, `sdfGrOt_t` and
  `sdfPreDrawTag_t`. `TMX_HEADER_t` keeps `resourceKey` and
  `battleTextureSlot` at +0x1C and +0x20: the IDB calls that span `userArea`
  and `comment`, and these are what this game puts there.

- **The duplicated local type copies.** Found mechanically rather than from
  this page's file list, by comparing every type defined both in a shared
  header and in a `.c` file: `sdfTexHandle_t`, `sdfMat_t` and `TMX_HEADER_t`
  in the four `effPCP*` units, `sdfPrim.c`'s three packet members
  (`registers`/`transfer`/`textureFlush` → `elem`/`bitblt`/`texflush`),
  `sdfDelayedResrc.c`, `sdf3D.c`'s `sdf3DFogParam_t`, `sdfMovie.c`'s
  `SoundFormat`, `MovLinearStream` and `MovPacStream`, `billManager.c`'s
  `BillChildPayload`, and `effPCPBoss.c`'s two boss-param records. Where the
  IDB has a name it won (`effPCPBossBUNSINParam_t` is `pStartForm`/`pEndForm`,
  `movieStream_t`'s +0x10 is `file`); where it has only placeholders the repo's
  own names were kept, per the rule at the top of this page.

  `include/libsdf.h` turned out to hold the last drifted `TMX_HEADER_t`, and
  `sdf3D.c` the last drifted `sdf3DFogParam_t`, so the shared header is not
  automatically the canonical side. Check which copy the IDB agrees with.

- **The billboard packet writes.** All 133 `*(u32 *)(packet + N)` stores in
  `sdfBillBoard.c` now go through the `sdfBillBoard*Packet_t` types that
  `include/billboard.h` already carried; the offsets agreed field for field.
  `sdfBillBoardMakeN` and the other `*MakeN` builders keep their `packet[i]`
  word walk, which is a variable-length packet rather than a record.

### The rename that still compiles and is still wrong

Three of these types had a member name that appears in **both** the old and the
new list, at different positions. `MovPacStream` had `scratch` at index 3 going
in and at index 17 coming out; `sdf3DFogParam_t` had `fogNear` and `fogFar` at
+0/+4 going in and at +8/+12 coming out. Renaming the struct body and then
fixing whatever the compiler complains about does not work on these: the old
spelling is still a valid member, so `->scratch` keeps compiling and silently
moves to a different field. `movieOpenStream` came back 30 of 130 words out
that way.

Both were redone as a single simultaneous pass over the accesses, resolving each
object to its declared type before rewriting, so no name is rewritten twice.
Before renaming any struct body positionally, intersect the two member lists; if
the intersection is non-empty, the compiler is not going to find the mistakes.

A matching build is the only check that catches this, which is why each of these
went through `tools/check_unit.py` per unit and not just the final link.

### The three meaning corrections, applied

- **`SDF_ATTRBIT_e` bits 0 and 1 were swapped.** 0x1 is specular and 0x2 the
  refmap pass, per the IDB and `dds3_model.bt`, not the reverse.
- **`DDS3_FILE_TYPE` member 1 is `EFFECT`, not `MEM`.** `mdlManager.c:1437`
  switches the chunk tag on `0x413250`, `0x503344` and `0x555049`, the P2A, D3P
  and IPU chunks, which is where `command == 1` leads.
- **`SdfFileIOCmdStat` 7 and 9 are suspend and error**, not active and
  inactive, and the IDB's 0, 1 and 10 (init, dormant, cancel) were missing.

### Two claims on this page that the tree had already outrun

- The six invented function names said to be declared and never defined are
  gone; nothing in the tree declares them.
- `sdf_model_root_flag` was listed among the unreferenced enums. It is not:
  `sdfModel.c` and `fldAutoMap.c` use four of its members. That turned out to
  help rather than hurt, because `sdfModel.c:600` sets bit 4 inside
  `sdfModelMakeEarly`, which is what settles the IDB's `EARLY_SETUP` against
  the repo's invented `TEMPPACKET`. `SDF_MODEL_NODE_FLAG_MOTION_HIDE` is
  corroborated the same way: its only writers are the two
  `sdfMotCLBK_modelVisible*` callbacks.
