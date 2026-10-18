# Shin Megami Tensei III: Nocturne

[![Build Status]][actions]
[![smt3-code]][smt3-progress] [![smt3-functions]][smt3-progress]

[Build Status]: https://github.com/tge-was-taken/smt3-decomp/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/tge-was-taken/smt3-decomp/actions/workflows/build.yml
[smt3-code]: https://decomp.dev/tge-was-taken/smt3-decomp/smt3.svg?mode=shield&category=game&measure=matched_code_percent&label=code%20bytes
[smt3-functions]: https://decomp.dev/tge-was-taken/smt3-decomp/smt3.svg?mode=shield&category=game&measure=matched_functions_percent&label=functions
[smt3-progress]: https://decomp.dev/tge-was-taken/smt3-decomp/smt3?category=game

A work-in-progress **matching decompilation** of *Shin Megami Tensei III:
Nocturne* for PlayStation 2. The goal is to understand and document how the
game works by reconstructing readable source code from its retail binary.
Matching builds verify the reconstruction against the original executable.

> [!IMPORTANT]
> **This is a binary reconstruction and analysis project for understanding and
> documenting how the game works. You cannot use this repository to play the
> game; it is not a port.**
>
> Building requires your own lawfully obtained copy of the game. Required
> files are extracted locally from your copy. Disc images and retail
> executables are not included.

## Status and versions

Unfinished functions use assembly extracted from the original executable, so a
matching build does not mean the decompilation is complete. Nearly all of the
code is still assembly; names, types and source organization are being
recovered.

| Version | Game | Serial | ELF SHA-1 |
|---|---|---|---|
| `smt3` | Shin Megami Tensei III: Nocturne (USA) | `SLUS_209.11` | `5facfbf7916836613449c922e26a4ec412207b32` |

The ELF is 4,551,696 bytes, entry `0x00100008`, `gp` `0x00554170`, built
`Sep  3 2004`. It is symbol-stripped but keeps section headers, so the build
also has to reproduce `.vutext`, `.vudata`, `.ctors`, `.dtors`, `.eh_frame`,
the `.DVP.*` VU overlay tables and the section-header region byte for byte.

The build is verified: `ninja` relinks the split objects and `objcopy`s the
result into a file byte-identical to the retail executable, reproduced from a
clean tree. `.text` is split into 96 translation units, 69 of them under their
recovered names. 8,320 of the 9,694 functions are still `INCLUDE_ASM`
fallbacks; 1,374 are in C, which is 160,696 of 1,976,292 code bytes, verified
against the linked image rather than against objdiff's base objects. 129 of
those 1,374 are the empty functions splat emitted directly.

The badges above track matched game and engine code bytes and function
counts. Assembly fallbacks earn zero credit, and SDK/runtime code, handwritten
assembly and VU1 microcode sit outside those totals. They read from
[decomp.dev](https://decomp.dev/tge-was-taken/smt3-decomp/smt3?category=game),
which ingests the report CI uploads on every push to `main`, so they go live
with the first run that has the retail executable available. See
[Reading progress](docs/progress.md) for how the numbers are measured.

## Quickstart

Requirements:

- Linux x86-64 or WSL2, with support for running 32-bit i386 programs.
- Python 3.10+, `ninja`, `cpp`, and `git`.
- Your own disc image of the game, or the `SLUS_209.11` executable extracted
  from it.

```sh
python -m pip install -r requirements.txt
python tools/download_tools.py

# Copy your disc image into the repository root or orig/, then:
python tools/extract.py        # or place the ELF at orig/smt3/SLUS_209.11 yourself
python configure.py
ninja
```

`ninja` builds the executable and checks it against the retail SHA-1 above. A
checksum mismatch fails the build.

See [Building](docs/building.md) for build targets, generated files and
checksum troubleshooting. The [toolchain notes](docs/toolchain.md) explain the
original compiler, assembler and pinned 32-bit runtime.

## Documentation and contributing

Start with the [contribution guide](docs/CONTRIBUTING.md). Pick an
`INCLUDE_ASM` function, replace it with readable C, check the whole unit, and
verify that `ninja` still produces a matching executable.

The [documentation index](docs/README.md) lists every page. Nocturne is the
engine *Digital Devil Saga 1 and 2* were built on, so the compiler evidence
and source idioms collected in
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp) apply here, and its
`docs/idioms.md` is the reference for which source shapes produce which retail
code until this repo has its own.

## Acknowledgements

- [dds-decomp](https://github.com/Megami-Decomps/dds-decomp) and its author
  **redthing1**. This repository vendors dds-decomp's machinery (the splat
  configuration flow, `configure.py`, the tooling under `tools/` and the
  documentation structure) and adapts it for `SLUS_209.11`. The DDS1/DDS2
  decompilation that work supports is of the same engine as SMT3, so its
  toolchain findings, matching rules and confirmed source idioms carry over
  directly. dds-decomp ships no license file; it is used here with
  attribution.
- [smt3hd-reconstructed-sources](https://github.com/smt-ret/smt3hd-reconstructed-sources)
  for the SMT3 HD derived names, reconstructed headers and source file names,
  and [smt3](https://github.com/smt-ret/smt3) for the existing SMT3
  decompilation effort and the naming and type conventions this repo follows.
- [splat](https://github.com/ethteck/splat),
  [spimdisasm](https://github.com/Decompollaborate/spimdisasm),
  [m2c](https://github.com/matt-kempster/m2c) and
  [objdiff](https://github.com/encounter/objdiff).
- [decomp.me](https://decomp.me) for the ee-gcc 2.96 toolchain packaging.
- The Persona 3/4 decompilation projects, for SDK layout and naming
  conventions.
