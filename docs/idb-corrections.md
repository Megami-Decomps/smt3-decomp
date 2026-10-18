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

After each correction: re-run `tools/ida_symbols.py --instance SLUS_209.11`, re-run
`configure.py`, build, and confirm the image still hashes to
`5facfbf7916836613449c922e26a4ec412207b32`.

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

`sdfMatPacket_t`'s two transform blocks are inline, not pointers: `sdfMatSetUvwParm`
(0x002B3620) writes six floats at the pointer it is handed, `sdfMatUpdatePacket` passes
packet+0x68 and the multi-texture path passes packet+0x80.

These five changed no row of `symbol_addrs.txt`: every global using them is a 4-byte pointer,
and `tools/ida_symbols.py` emits only splat's own type names (`func`, `asciz`, `u8`, `u16`,
`u32`) plus `size:`. A project type name in a `type:` field makes splat exit 2.

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

Entry 0 of `modelFileKindTable` is `{0x0032A858, 2}` and the table runs seven entries to
0x0032C458, where `mdlResrcMajorList` starts. `modelFileTable` (0x0032A820) has the same
`{entries, count}` shape.

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
