# Native SDK data

These objects use the same names and extents as
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp), at SLUS_209.11's
own addresses. The curated entries in `config/smt3/symbol_addrs.txt` survive
IDA exports; `name_sources.txt` records their reader and layout evidence.

| Object | Address | Bytes |
|---|---:|---:|
| ipuDefaultQuantizerMatrices | 0x0052BAA0 | 0x50 |
| ipuDefaultVqClut | 0x0052BAF0 | 0x20 |
| _impure_ptr | 0x0052BE28 | 0x4 |
| __malloc_av_ | 0x0052BE30 | 0x408 |
| __malloc_trim_threshold | 0x0052C238 | 0x8 |
| __malloc_top_pad | 0x0052C240 | 0x8 |
| __malloc_sbrk_base | 0x0052C248 | 0x4 |
| __malloc_max_sbrked_mem | 0x0052C250 | 0x8 |
| __malloc_max_total_mem | 0x0052C258 | 0x8 |
| __malloc_current_mallinfo | 0x0052C260 | 0x28 |
| sbrkBreak | 0x0052C2A8 | 0x4 |
| _ctype_ | 0x0054B550 | 0x101 |

The corresponding DDS1 and Nocturne readers agree instruction-for-instruction
after relocating calls and absolute data references: `sceIpuInit` (0x238
bytes), `_malloc_r` (0x728), `_malloc_trim_r` (0x16C), and `sbrk` (0xAC).
Branch displacements, scalar constants, registers, and structure offsets agree.

`sceIpuInit` loads five quantizer quadwords and two CLUT quadwords. Both
initial tables are byte-identical to DDS1. The allocator's 0x408-byte bin
array and following state have the same relative layout; its initial links
agree after expressing pointers relative to the array. The following scalar
state bytes agree directly. `_impure_ptr` points to the native reentrancy
record at 0x0052BB38, whose initial random state at +0xA8 is 1. `sbrk` reads
and writes `sbrkBreak` with interrupts disabled; its initial break is
0x005A0330. `_ctype_` is the unique byte-identical 257-byte table, including
the EOF entry, used by `_strtol_r` and `mdlManager`'s character conversion.

## ATRAC3

The Nocturne USA EE executable does **not** contain DDS's linked libat3dec
decoder. All 22 DDS1 `at3dec*` routines were searched, at every four-byte
offset across Nocturne's entire `.text`, using the relocation/immediate masks
described by `tools/pair_dds1.py`. None matches, including
`at3decDecodeFrame`, `at3decDecodeChannels`, and `at3decInitDefaults`. This
search includes unnamed code. Nocturne also lacks the `libat3dec.c` assertion
string present in DDS1 and DDS2.

This establishes absence of the DDS decoder build, rather than a general
claim about every possible audio implementation. No ATRAC3 routine or table
names are assigned to unrelated Nocturne data.
