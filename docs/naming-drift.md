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

The prize is not the names. Merging the local copies onto the shared headers
deletes the duplicated type definitions, and the renames are what make the
merge possible.

**Done.** `include/sdf_resource.h` now owns the six engine resource records
(`sdfMemHandle_t`, `sdfGsMemSlice_t`, `sdfTexHandle_t`, `sdfMat_t`,
`sdfDrawTag_t`, `TMX_HEADER_t`), `libsdf.h` includes it, and the seventeen
units that carried their own copies include it instead. `nbMakePacket.c` takes
its five `datUnit*`/`datSkill*` records from `newdata.h`, and three `effPCP*`
units take `sdfVector3_t` from `sdf_types.h`. That removed 52 of the 66
duplicate definitions, 763 of 837 lines.

Fourteen definitions, 74 lines, are deliberately left:

- `sdfPrim.c`, `sdfDraw.c` and `sdfGsMemIO.c` keep nine `draw.h` packet types.
  `draw.h` cannot be included: it brings `libsdf.h`, whose `sdf3DFogPacket_t`
  and `sdf3DViewportPacket_t` conflict with these units' own readings.
- `sdf3D.c` keeps `sdf3DFogParam_t` for the same reason.
- `sdfDelayedResrc.c` keeps `sdfDelayedResrcCB_t`: `libsdf.h` types `chunk` as
  `void *` where the unit walks it, and taking the header's version left two of
  seven functions differing.
- `nbMakePacket.c` keeps `nbKoukaProcessData_t`; `nb.h` would pull `kernel.h`.
- `sdfPreDrawTag_t` was not moved at all. Its pointee is spelled
  `SdfPacketPatchLink` (8 bytes) in three units and `sdfPreDrawHook_t` in
  `libsdf.h`, which the IDB confirms is 16; that is a truncated reading to
  recover, not a name to merge.

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

### A drifted copy can hide behind a header the unit cannot include

`effPCPMisc.c`'s local `sdfMatPacket_t` named twelve of its scalars
`pad00`, `unk04`, `unk08`, `specular`, `unk14`, `pad18`, `unk1C`,
`alphaState`, `multiTexMethod`, `y` and `x`, against the IDB's `vifcode0`,
`ambient`, `diffuse`, `specularColor`, `refmapColor`, `vifcode1`, `constant`,
`mtexAlpha`, `mtexAlgo`, `fdBias` and `fdSoften`, all at the same offsets.
The duplicate-type scan never flagged it, because the scan only compares copies
of a type a unit could already see, and this one differs from `libsdf.h`'s in
granularity as well as names: three of its members are composites covering
pad+vifcode2, the two three-register texture blocks and the two transforms.
The names are now the IDB's and the composites are kept, each commented with
the canonical members it spans.

It surfaced only from trying to `#include "nb.h"` in that unit, which reported
three redefinitions. That is worth remembering as a probe: a failed include
names the types a unit duplicates at a different granularity than the scan can
see. The include itself was backed out, because `sdfTexPacket_t` and
`sdfGrOt_t` there are coarser readings that are not obviously worse (the local
`sdfGrOt_t` pins `callbackFunc`'s signature where `libsdf.h` has `void *`).

Backing it out leaves two casts in `effPCPMisc.c` that cannot become member
reads: `effBTLSkillGetTargetFormation` returns `nbFormation_t *` per the IDB,
but the unit holds it as `u8 *`. They now say which field they land on
(`+0x60` is `oneshotr`, `+0x78` is `stat`), which is the most the unit can
carry until it can see `nb.h`.

### The global work record, and what a cast is doing that a member is not

`include/newdata.h`'s `dds3GlobalWork_t` is a partial view of the save work.
Five of its members disagreed with the IDB and are now its names: `eventFlags`
-> `EventBit`, `encounterRemainder` -> `encountkyori`, `encounterGauge` ->
`encountCount`, `unk1CFB` -> `hearts`, `unk1D7B` -> `heartscnt`, plus `unk34`
-> `memHandle` (name only: the IDB types it `sdfMemHandle_t *` and the code
only moves the word). Its 0x3E4-byte padding run at 0x1910 is split on the
IDB's names for the span, which exposes `stocklist`, `boss_press`, `negoques`
and `kotowari`.

That let three `nbMakePacket.c` casts become member reads (`DDS3_GBWK->maka`
four times, `DDS3_GBWK->encountCount`). Three more were tried and reverted,
and the reasons are the useful part:

- `(u8 *)(q + (s32)DDS3_GBWK + 0x1CE8)` is not `&DDS3_GBWK->negoques[q]`.
  Retail adds the index to the base as an integer; the array form reorders the
  add against the store and moves the result register. `Flow2_4` and `Flow7_3`
  went 4 and 2 words out.
- `fclClearTerminalState`'s `u8 *gb` local is load-bearing. It caches the base
  so ee-gcc emits one `$gp` load for both stores; reaching through `DDS3_GBWK->`
  per statement reloads `$gp` and swaps the two stores.

So an offset cast is not automatically worse than a member access. Where it
encodes an integer add or a cached base that retail's codegen depends on, it is
the honest spelling, and the comment should say which field it lands on.

### INCLUDE_SDATA lines cannot carry a comment

`tools/include_sdata.py` owns those lines: its `LINE` regex matches
`INCLUDE_SDATA(...);` through the newline and re-places the directives on every
`configure.py` run. A trailing comment stops the regex matching, so the tool
decides the line is missing and adds a second one, and ninja then reports
*multiple rules generate .../D_0054E9D0.s*. Moving the comment to the line
above does not work either: the directives get re-placed and the comments are
left behind, stacked above the first one.

So the six `.sdata` strings in `dds3ObjectBase.c` stay undocumented in the
source. They are the character model names `hij_b`, `isam_f`, `aradia`,
`yuuko_p`, `chiaki` and `tiaki_a`, recorded here instead. Anything that wants
to annotate generated directive lines has to teach the generator first.

### String placeholders: the rule, and what still blocks 61 of them

A format string or process name referenced as `extern char D_<addr>[]` plus an
`INCLUDE_RODATA` line is worth becoming the literal it spells. ee-gcc pools each
literal back to the address the split had given it, so the image does not move.
`tools/inline_strings.py` finds and converts them.

The counts on this page were wrong for two sweeps, both times because the scan
was wrong rather than the tree. It read only `asm/smt3/data/*.s`, and most
strings live in the per-symbol and per-unit files beside them, so "262" was
really 1023 references. 128 are now literals across 32 units; 61 are left in 15
files.

Two things block the rest, and both are reported rather than guessed:

1. **The rodata is shared with a function still in assembly.**
   `tools/check_unit.py` says so outright: *SHARED rodata of
   `sdfDumpDmaChain` (retail 0x00549B70) is also used by asm
   `sdfGrFlushOtChain`: keep the extern `D_` symbol until they are C.* While
   any `INCLUDE_ASM` sibling references the string, the symbol has to stay.
   This is the dominant cause, and it clears itself as those units finish.
2. **The string is not in `.rodata`.** `.sdata` starts at vram `0x54E800`
   (`config/smt3/SLUS_209.11.yaml`). gcc emits a literal into `.rodata`, so
   inlining one that lives in `.sdata` moves its bytes between sections;
   `check_unit` reports *this section has no accepted per-unit source owner*.
   An `INCLUDE_SDATA` line is the obvious signal but not a reliable one: an
   `.sdata` string can simply be an extern into a blob another unit owns, so
   the test is the address, not the directive.

A third thing blocks two files, and it is the one `check_unit.py` cannot see:
**the padding**. Retail pads `D_0052DD00` ("cdrom0:\\SLPS_999.99;1", which
really does have a backslash byte) to 24 bytes; gcc emits 22 and aligns to its
own taste. `dds3AdminiProcess.c` lost 16 bytes that way and
`evtPolygonMovie.c` gained 8, and because those nearly cancelled, every unit
passed `check_unit.py` and the image still came out 8 bytes short with 112,514
bytes differing. Masking a unit's local rodata relocations is exactly what
hides a size change. **Run the full relink, every time.** The same padding is
why `evtStage.c` spells its format string `const char D_0053A7F8[0x10] =
"%sf%03d_%03d";` rather than inlining it: the explicit size is the padding.

The procedure is per string, not per unit. Converting a whole file and reverting
it on failure was the earlier advice and it throws away most of the yield, since
only some of a unit's strings share rodata with an asm sibling: going one at a
time turned 8 whole-file failures into 10 partial wins, 31 strings. Convert,
run `tools/check_unit.py`, revert that one string unless the match count is
unchanged, and relink at the end.

### Three ways a string scan lies

Each of these cost a wrong count or a broken file, and each is now a test in
`tools/test_inline_strings.py`.

- **A leading `.asciz` does not make the symbol a string.** A record that opens
  with a name field disassembles the same way: `dds3UnitPartTable_t` starts with
  `.asciz "hijri"` and is 360 bytes of table. Inlining 14 of those turned
  `hijriParts = D_0052F850;` into `hijriParts = "hijri";`. A symbol qualifies
  only if one `.asciz` is its whole body, between `dlabel` and `enddlabel`.
- **That structural test is still not enough.** `effEvent.c`'s `D_00320598` is
  an `effBlurScatterParams_t` whose first bytes are `3C 00` and whose rest is
  zero, so the splitter emits it as a single `.asciz "<"`. Nothing in the
  assembly distinguishes it. The tree's own declaration does, so the rule is:
  only a `char` object is a string.
- **Printable bytes are not text.** `evtStage.c`'s `D_0032C5F0` disassembles as
  `.asciz "333?333?333?"`, which is `33 33 33 3F` repeated, three `0.7f` floats.
  Requiring a NUL terminator rejects it; it is in fact `sdf3DLight_t light[3]`.

### Inlining an sdata string: do not

`effBattle.c`, `evtPolygonMovie.c` and `nbKoukaProcess.c` referenced 24 of
their own format strings and process names as `extern char D_<addr>[]` plus an
`INCLUDE_RODATA` line. Those are now the literals they spell
(`printf("effBTL:Set Stat[%X]\n", ...)`), and the image is unchanged: ee-gcc
pools each literal back to the address the split had given it.

One in the same set must stay as it was. `D_00551138` ("%s\n") is defined
through `INCLUDE_SDATA`, and a C literal goes to `.rodata`, so inlining it
moves the bytes out of `.sdata` and the link shifts. The rule is per symbol:
`INCLUDE_RODATA` converts, `INCLUDE_SDATA` does not.

### What the IDB will not name

Of 78 `D_<addr>` references in the four units added since the last sweep, 52
stay. The IDB has a name for all of them, and every one is a dummy
(`unk_32C840`, `xmmword_31B8A0`, `flt_317750`, `stru_31CA90`) — no better than
the address. Three are real, unique IDB names, and only one was adopted:

| Address | IDB name | Taken? |
|---|---|---|
| 0x00551D88 | `abgr` | yes, and it says what the word is |
| 0x005699B0 | `resource` | no: `effBattle.c` already has three locals by that name |
| 0x0031B8C0 | `state` | no: eleven locals, and the object is a per-kind camera lookup table |

`frFont.c`'s thirteen `0xF2xx` message markup codes are left bare on purpose.
They sit in a live switch and would read far better named, but the IDB has no
enum for them and this page's own rule forbids inventing one to satisfy that.

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

## Checking this periodically

The audits behind this page were one-off scans. They went stale within two
sweeps, because every newly decompiled unit arrives with its own type names and
its own local copies of shared records. The check is now a tool:

```sh
python3 tools/ida_types.py    # IDB types -> config/smt3/ida_types.txt (needs IDA open)
python3 tools/check_names.py  # compare the tree against that file
```

`ida_types.txt` is tracked, so `check_names.py` runs without IDA. Re-export it
after any IDB correction, the same as `symbol_addrs.txt`. The report is a
report, not a test: it always prints findings and always exits 0, because most
of what it lists is legitimate and only a layout check can tell which is which.

It sorts every typedef in the tree into six buckets. The counts below are from
the run at 74.3% of functions in C.

| Bucket | Count | What to do |
|---|---|---|
| matches an IDB name exactly | 238 | nothing |
| matches but for the suffix | 18 | nothing: `_t` for IDA's `_tag` is the convention |
| external name | 8 | nothing: newlib and the Sony SDK own these spellings |
| close to an IDB name | 97 | a lead; confirm the layout, then rename |
| off-convention, no counterpart | 83 | invented; recover the type, do not rename blind |
| conforms, IDB never typed it | 545 | nothing to check it against |

The two middle buckets are the queue. The last one is the honest residue: the
IDB recovered 1778 names and the tree has 989 typedefs, and the overlap is
smaller than either, so most names cannot be checked at all.

### Why the convention is a weak filter

The convention catches a name spelled `SdfPacket` or `BillOut`. It cannot catch
one spelled `dds3ModelBasicWork_t`, which conforms perfectly and is still not
what the IDB calls that record. 83 names break the convention; 97 more conform
and disagree with the IDB anyway. Running the stem comparison matters more than
reading the suffixes.

### Three systematic drifts, one of them not a drift

**The dds3 basic work records, fixed.** The tree dropped words the IDB keeps:
`dds3ObjectBaseControl_t` is `dds3ObjectBaseControlWorkData_t`, and the seven
per-object records are `dds3<Kind>BasicWorkData_t`, not `dds3<Kind>Work_t`.
Sizes and offsets corroborate each one, and the control record is 0xAC in both.
134 occurrences across 22 files, no codegen change.

What makes these names canonical is not the layout match but the recovered
*function* names, which the repo already relies on and which were not touched:
`dds3AllocModelBasicWorkData`, `dds3ObjectBaseInitControlWorkData`,
`dds3ObjectBaseFreeControlWorkData`, `dds3GetBaseControlWorkData`,
`dds3SearchSceneBasicWorkData` and the four
`dds3<Kind>ObjectBasicGetControlWorkData` accessors. 17 functions carry the
word, every one is in `symbol_addrs.txt`, and the field they fill is
`dds3Basic_t.workdata`, the IDB's own member name. `dds3ModelRangeBasic.c:31`
closes it: the function named `dds3AllocModelBasicWorkData` allocates
`sizeof(dds3ModelBasicWorkData_t)` into `basic->workdata`. The IDB carries no
`*BasicWork_t` spelling for any of the eight, so the tree's shorter names were
not a competing recovered reading. No retail string spells `WorkData`, so this
rests on recovered names rather than on the binary's own text.

Worth noting what the rename exposed rather than fixed: the tree's
`dds3UnitObjectWork_t` is 0x20 where the IDB's is 0x24, so the tree's view is
one member short of `IsBattle` at +0x20. The three offsets the two readings
share do agree (+0xC, +0x10, +0x14), which is what justified the rename. The
truncation is left alone, because widening a struct can change codegen wherever
something takes its size.

Only two of the eight can be asserted in `tools/header_layout.c`: the other six
are defined in the `.c` file that uses them, so the header test cannot see them.
Their IDB sizes are 0x1C model, 0x10 mover, 0x10 area, 0x10 event object, 0x30
matter object, 0x24 model range. Moving them into `include/basic.h` would both
remove a duplicate definition and let the size claims be checked on every build,
which makes them a good first candidate for the next header merge.

**`camp*` should be `cmp*`.** `campInfoWork_t` is the IDB's `cmpInfoWork_t`
exactly, and the IDB uses `cmp` for all 39 types in that module. The other four
(`campWork_t`, `campDhEntry_t`, `campDhAnimWork_t`, `campStatusPanel_t`) need
per-type evidence before moving, since a module-wide prefix change is not
itself proof about any one record.

**`dds3SpPoly*` is not a drift, and the report's suggestion is wrong.**
`check_names.py` offers `dds3SpPolyRing_t` -> `dds3SpPoly_Ring_t3` and two
siblings. Do not take them. The IDB types `dds3SpPoly_Ring_t3` as a 0x44 record
whose first member is a *pointer* to a 0xC4 `dds3SpPoly_Basic_t`. The retail
code says otherwise: `dds3SpPoly_RingCreate` makes one allocation of
`sizeof(dds3SpPolyRing_t)` plus the trailing per-entry records, copies the
template head to offset 0, copies the parameters to `&band->spawnDelayStep`,
and sets `records = band + 1`. The head is inline and the block is contiguous.
It also writes `templateSize = sizeof(dds3SpPolyHead_t)`, which is 0xC0 in the
tree against the IDB's 0xC4, so the four bytes at +0xC0 that the IDB calls
`pRollRing` are the ring's own first parameter.

That is the general shape of the risk. A close name is evidence about naming,
never about layout, and the IDB's struct can describe a different build of the
same engine. Confirm against an allocation site before any rename that the
report only guessed at.

### What the report cannot see

Duplicate definitions are counted separately, because the same name in twelve
files is not a naming problem. That count moves the other way as the tree
grows: it was 14 after the header merge and is 85 now, the widest being
`pbMyCharacter_t` and `pbMatWork_t` at 12 copies each.
