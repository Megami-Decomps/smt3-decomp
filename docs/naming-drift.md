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

1. The four single-field header renames, about 10 sites. Free, and proves the
   no-codegen claim.
2. The ten unreferenced enum renames. Two agreeing sources, zero risk.
3. Ten single-file congruent member types, about 460 sites, each self-contained.
4. The narrow shared types (`sdfMemHandle_t`, `sdfGsMemSlice_t`, `sdfThCB_t`,
   `sdfGrTagStdReg_t`, `sdfDrawTag_t`, `sdfGrOt_t`, `sdfPreDrawTag_t`,
   `sdfTexHandle_t`, `sdfMat_t`), about 450 sites. This unlocks the header merge.
5. The enums used in live switches, then the wide records
   (`sdfFileCmdHandle_t`, `sdfIpuDecodeCb_t`, `movieStream_t`,
   `sdfMotionCtrlr_t`, `sdfModelRootNode_t`).
6. The layout-shifted packets, per type, each checked against IDB offsets rather
   than member index.
7. The magic constants, one literal at a time with a build check each.

Record the three meaning corrections when they are applied: the repo's reading
was replaced, which is a different claim from a rename.
