# Toolchain

A matching decompilation reconstructs readable C that reproduces the original
machine code byte for byte when compiled with the original compiler and flags.
The recovered source describes the implementation; it is not the original
source text.

| Code | Toolchain |
|---|---|
| Atlus game and engine code | ee-gcc 2.96 (`2.96-ee-001003-1`) at `-O2`, assembled by Sony's ee-as with `-G8` |
| Sony SDK 2.5.x libraries, newlib, libgcc | prebuilt archives, identified by signature matching |
| `.vutext`, `.vudata` | VU1 microcode and its data, kept as binary |

## Evidence from this ELF

Measured on `orig/smt3/SLUS_209.11` (`.text`, 498,577 instructions):

| Check | Result | Reading |
|---|---|---|
| `move` encoding | 51,258 as `daddu`, 0 as `addu` | ee-as, not modern GNU as |
| Callee saves | 26,294 `sd` (7,166 of them `$ra`), 0 `sq`, 0 `sw $ra` | gcc, not MWCC, which saves with `sq` |
| `.mdebug.eabi64` | present (empty, at file offset `0x452b84`) | Sony EE EABI64 assembler output |
| Build date string | `Sep  3 2004` | build identity |

The original assembler matters as much as the compiler: modern GNU as encodes
`move` differently and inserts FPU hazard `nop`s, so C is assembled with ee-as.

## Carried over from DDS

SMT3 is the engine *Digital Devil Saga 1 and 2* were built on, with the same
compiler and assembler, so the toolchain work in
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp) applies here. The
following has **not** been re-measured on `SLUS_209.11` and is carried over:

- The compiler-identification bake-off: of 231 random DDS functions compiled
  straight from m2c output, 94 match under ee-gcc 2.96, against 0 to 26 for the
  other candidate compilers.
- The exact compiler package identity `2.96-ee-001003-1` and the `-O2 -G8`
  flag pair. The instruction-encoding evidence above is consistent with it but
  does not pin the package version on its own.
- The confirmed source idioms (`docs/idioms.md` in dds-decomp) and the
  compiler decision atlas.
- Per-file flag exceptions. DDS has a handful of files built without
  sibling-call optimisation, recorded with their evidence in its
  `config/<v>/cflags.txt`. No such file has been identified in SMT3 yet; this
  repo builds every unit with the same options until one is found with
  evidence.

## Pinned compiler runtime

The 2000-era compiler binaries are i386 ELF. `tools/download_tools.py` fetches
the exact 32-bit glibc they are run under (Fedora `glibc-2.43-8` i686), because
ee-gcc 2.96's output can depend on the C library's heap layout: a different
libc can compile some functions differently. This is why the build needs a host
that can run 32-bit i386 programs.

## VU and overlay sections

`.vutext` (`0x002e6e50`, 0xe7b0 bytes) holds two VIF packets, the VU0 and VU1
microcode and the data they are uploaded with. No C can replace it, but it is
not a blob either: all 2,061 instructions are assembled from the listings in
`src/smt3/sdf/vu/`, and only the VIF headers, the debug font texture and the
unpacked data come from the extracted asset. See [VU microcode](vu/README.md).

The microcode was built with dvp-as, Sony's VU assembler, which the
`.DVP.ovlytab`, `.DVP.ovlystrtab` and `.DVP.overlay.*` sections after `.bss`
record one entry per MPG upload for. Those tables are reproduced as data: their
names carry generated ids that no source recovers. The listings are written in
that assembler's syntax, and `tools/download_tools.py` fetches a 2003 dvp-as to
check them against; the build itself assembles them with `tools/vuas.py`, so it
needs no VU toolchain. `.vudata` stays binary.

See [Building](building.md) for setup and build targets, and the
[matching rules](CONTRIBUTING.md#matching-rules) for source requirements.
