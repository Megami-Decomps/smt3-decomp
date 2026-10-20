# Matching the SDK and C runtime

`.text` ends with 126,120 bytes of code that is not Atlus source: the Sony EE
libraries, newlib, and libgcc. It runs from `0x002C81A8` (`sceGsResetGraph`) to
the start of `.vutext` at `0x002E6E50`, holds 669 functions, and today sits
inside the single unit `code_002C7CB0` as `INCLUDE_ASM` fallbacks.

[Reading progress](progress.md) counts this region in the `game` denominator
because no unit had been identified as SDK. It is not game code, and this page
is the account of what it is, function by function.

## Where the names and the boundaries come from

`config/smt3/sdk_objects.tsv` maps each function to the archive object it was
compiled into. `tools/sdk_objects.py` writes it from installed SCE SDK trees:

```sh
python tools/sdk_objects.py smt3 --sdk .../sce/296 --sdk .../sce/204 --sdk .../sce/303
```

The archives are evidence, never a build input. Nothing from them is linked,
and no Sony source text or symbol is copied into this tree. Two rules produce a
row, and each row says which one did:

| Evidence | Rule | Strength |
|---|---|---|
| `bytes` | Same length, and the same instruction words once `j`/`jal` targets and the low half of every immediate form are cleared. Same normalisation as `tools/pair_dds1.py`. | identifies the object the code was compiled into |
| `name` | The recovered name is a function name in exactly one archive member. | says which object defines that name in the SDKs on hand, not that these bytes came from it |

`libg.a` and `libidt.a` are copies of `libc.a` and `libmx.a` of `libm.a`, so a
name found only there is reported against the original. The soft-float multilib
of `libgcc` is left out: the game links hard-float, and the two define the same
names.

Of the 126,120 bytes, 50,080 (39.7%) are identified by bytes, 65,508 (51.9%) by
name alone, and 10,532 (8.4%) not yet at all. 116 distinct archive members are
named, so the region is around 116 translation units rather than one.

## Two different provenances, measured separately

The three SDK trees on hand are 2.0.4, the ee-gcc 2.96 toolchain package, and
3.0.3. Byte-exact coverage of this region, per tree, says the C runtime and the
Sony libraries came from different places:

| Tree | Byte-exact | Where it lands |
|---|---|---|
| ee-gcc `2.96-ee-001003-1` package | 23,640 bytes | `libc.a` 12,240, `libgcc.a` 8,992, `libm.a` 2,368 |
| SDK 2.0.4 | 38,132 bytes | `libkernl.a` 16,536, `libcdvd.a` 4,376, `libpad.a` 2,400, `libdma.a` 1,232, `libipu.a` 1,008, `libgraph.a` 984, `libmc.a` 480 |
| SDK 3.0.3 | 16,036 bytes | the same Sony libraries, less of each |

The C runtime is the 2.96 package's own: `_malloc_r`, `_dtoa_r`, `qsort`,
`_strtol_r` and `vfprintf` are word-for-word that `libc.a` and differ from both
2.0.4's and 3.0.3's by hundreds of words. That is the compiler package this
repo already pins for game code, so the runtime needs no version guess.

The Sony libraries are closest to 2.0.4 and identical to none of the three,
which fits a 2.5 or 2.6 release that is not on hand. Each library has to be
matched against the retail bytes, not against an archive.

## The Sony libraries were built by a different compiler

They were not compiled by the 2.96 the rest of the build uses. They were
compiled by **ee-gcc 2.9-991111-01** at `-O2 -G0 -fno-common`, and that is
measured, not inferred:

- SDK 2.0.4's `libgraph.a` member `graph020.o` holds `sceGsPutIMR` and is byte
  identical to the retail function. Only the two unrelocated `jal` targets
  differ, so retail's libgraph is that build of that object.
- 2.96 cannot produce it. Given the obvious C body, 2.96 and 2.95.3 both emit a
  32-byte frame with the three saved registers 8 bytes apart. Retail uses a
  48-byte frame with them 16 bytes apart, which is what 2.9 emits: it reserves
  16 bytes per saved register and reports them as `regs= 6/0`.
- 2.9 has no sibling-call optimisation. Retail's `sceGsGetIMR` keeps a frame
  and calls `GsGetIMR` with `jal`; 2.96 turns the same C into a `j` sibcall.
- The flags come from SDK 3.0.3's own library makefile
  (`ee/src/lib/dma/Makefile`): `CFLAGS = -c -Wa,-al -fno-common -O2 -G0`, no
  `-g`. The ICO decompilation records the same `-G 0` without `-g` for the Sony
  libraries, against `-g -G 8 -fno-common` for game code.

`tools/download_tools.py --compiler ee-gcc2.9-991111-01` fetches that compiler
from the decomp.me archive, and its `cc1` output is identical to the 2.9 `cc1`
shipped in SDK 2.0.4, so nothing from the SDK has to be run.

Four functions are already solved against retail under it, and are the test
cases for wiring the second compiler in: `sceGsGetIMR` and `sceGsPutIMR`
(straight wrappers over the `GsGetIMR`/`GsPutIMR` syscall stubs),
`sceDmaGetChan` (a bounds-checked load from `sceDmaChanRegs`) and
`sceDmaPutStallAddr` (read `0x1000E060`, write it, return the old value,
through a non-volatile pointer so the store falls into the delay slot). None of
them can be committed as C while every unit is compiled with 2.96.

## What is in the region

| Library | Funcs | Bytes | Members | Note |
|---|---|---|---|---|
| `libkernl.a` | 313 | 37,012 | 21 | threads, intc/dmac, sif cmd and rpc, tty, file I/O, loadfile, TLB, exec |
| `libc.a` | 85 | 32,640 | 46 | newlib: stdio, malloc, dtoa, qsort, strtol, locale, signal |
| `libgcc` | 31 | 10,032 | 14 | fp-bit double emulation, dwarf2 frame unwinding, `__main` |
| `libm.a` | 14 | 7,624 | 14 | fdlibm single-precision: sin, cos, tan, acos, atan2, sqrt, rem_pio2 |
| `libcdvd.a` | 30 | 6,976 | 5 | `cdvd000.o` and four numbered siblings |
| `libmc.a` | 28 | 6,248 | 1 | one object |
| `libpad.a` | 30 | 5,016 | 1 | one object |
| `libgraph.a` | 13 | 4,744 | 10 | `graph001.o` through `graph029.o` |
| `libdma.a` | 19 | 2,120 | 1 | one object |
| `libmrpc.a` | 7 | 1,832 | 1 | the multi-threaded RPC variant |
| `libipu.a` | 4 | 1,344 | 2 | |
| unidentified | 66 | 10,532 | | mostly placeholder-named functions inside a known library |

Sony numbered the members of `libcdvd.a` and `libgraph.a`, so those names say
which object without saying what it was called in source.

### 3,508 bytes that were never C

Twelve newlib functions are hand-written R5900 assembly in newlib itself, which
the archive's debug paths name: `memchr`, `memcmp`, `memcpy`, `memmove`,
`memset`, `strcat`, `strchr`, `strcmp`, `strcpy`, `strlen`, `strncmp`,
`strncpy`, all from `libc/machine/r5900/*.S`. They belong with
`asm/smt3/hand/crt0` in the handwritten category, not in a C denominator.

## Free software, and everything else

The split that decides how each unit is worked is not Sony library against
Atlus code. It is whether the source is public:

**newlib, fdlibm and libgcc (50,296 bytes).** Free software, and the exact
build is pinned: the ee-gcc 2.96 package's archives. A matching unit is upstream
source compiled with the pinned `cc1` and the flags Sony built the archive with.
The archive objects are on disk, so the loop does not even need the retail image
to iterate: compile, compare to the member, then confirm against retail. The
debug paths in each member name the upstream file, down to
`src/newlib/libc/stdlib/mallocr.c` for `_malloc_r` and `src/gcc/libgcc2.c` for
the rest of libgcc.

Flags are the one unknown. ICO adds `-fno-builtin` for newlib and libgcc on top
of the Sony-library flags. Which compiler built that newlib is also still open:
it ships with the 2.96 package, so 2.96 is the first thing to try, unlike the
Sony libraries above.

**The Sony libraries (65,292 bytes).** No public source. SDK 3.0.3 ships
sources for `libdma` and `libgraph` only (`ee/src/lib/dma/libdma.c`,
`ee/src/lib/graph/graphdev.c`), and those are confidential Sony text: they may
be read to settle what a function means, and nothing may be copied from them,
not source lines and not symbol names. Work on these units is clean room: the
implementer works from the retail disassembly and public API documentation, and
whoever has read the SDK source does not write the unit.

Public references that are not SDK text: the SDK's own public headers, the
ps2sdk reimplementation (`ps2dev/ps2sdk`), and the naming already recovered in
the IDB.

## Other projects that have matched this code

| Project | Compiler | What it has | Usable here |
|---|---|---|---|
| [nathanialf/ico](https://github.com/nathanialf/ico) | ee-gcc 2.9-991111-01 | MIT licensed. Reproduces the whole `sce/` tree plus newlib, libm, libgcc and `crt0.s`, and records per-library build flags. Fully matching on three targets. | the flag evidence above; its SDK objects are reproduced from assembly, not C |
| [OpenRAC/rac5-sm-decomp](https://github.com/OpenRAC/rac5-sm-decomp) | pinned Sony compiler, version not stated | Nine SDK bodies matched in C, including `sceGsResetGraph`, `sceDmaSend`, `sceDmaReset`, `sceDmaPause`, `InitAlarm` | no license file and the upstream provenance of the bodies is not established, so do not copy from it |
| [denzi-gh/crashwoc-decomp-ps2](https://github.com/denzi-gh/crashwoc-decomp-ps2) | ee-gcc | `sdk/sce/graphdev`, `sdk/gcc/libgcc2` and newlib `mallocr` decompiled, `sceGsResetGraph` at 97% | same questions to settle before any reuse |
| [entriphy/kl2_lv_decomp](https://github.com/entriphy/kl2_lv_decomp) | ee-gcc 2.9-991111-01 | the nearest compiler to ours among active PS2 projects | comparison, not code |
| [oohweee/sh2-proto-decomp](https://github.com/oohweee/sh2-proto-decomp) | PS2 era | SDK headers in tree | reference |

None of them removes work here: the only one that is both licensed for reuse
and complete reproduces the SDK as assembly, which this repo already does.

## Plan

1. **Teach the build a second compiler.** Every Sony library unit needs
   ee-gcc 2.9-991111-01 with `-O2 -G0 -fno-common`, so `configure.py` has to
   select a compiler per unit the way `config/smt3/cflags.txt` already selects
   flags. Nothing in the region can be matched before this lands. The four
   functions above are the test cases; `tools/check_funcs.py` compares named
   functions in the linked image against their own retail spans, which is what
   a changed function length needs.
2. **Split the region into units.** One unit per identified archive member,
   under `src/smt3/sdk/<library>/<member>`, with unattributed runs in their own
   address-named units. `configure.py` already gives any path under `sdk/` the
   `sdk` progress category, so the game-code denominator stops counting this
   code as soon as the split lands. The relinked image has to stay byte
   identical.
3. **newlib, fdlibm and libgcc.** Settle the build flags against one small
   member, then work outward. Highest byte yield and no recovery involved.
4. **`libdma` and `libgraph`.** Clean room, with the semantics settled from the
   3.0.3 sources by someone other than the implementer.
5. **The rest of the Sony libraries.** Clean room from the disassembly, public
   headers and ps2sdk. `libkernl` is the largest at 37,012 bytes and most of it
   is thin syscall and sif wrappers.

## What the region says about the recovered names

Several names in this region are the IDB's placeholders or are wrong, and the
archives prove it. `initAtExit`, `runAtExitList` and `registerAtExit` sit in
libgcc's `__main.o`; `sdfFloatToULong`, `sdfDoubleToULong`, `sdfUnpackFloat`,
`sdfPackFloat`, `sdfUnpackDouble` and `sdfPackQuad` sit between libgcc's
`fp-bit` objects and are soft-float helpers, not `sdf` engine code. Correcting
them follows the rule in `CLAUDE.md`: fix the IDB, re-export, rebuild. Record
each in [IDB corrections](idb-corrections.md) when it is done.
