# Shin Megami Tensei III: Nocturne

[![Build Status]][actions]
[![smt3-code]][smt3-progress] [![smt3-functions]][smt3-progress]

[Build Status]: https://github.com/Megami-Decomps/smt3-decomp/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/Megami-Decomps/smt3-decomp/actions/workflows/build.yml
[smt3-code]: https://img.shields.io/endpoint?url=https%3A%2F%2Fmegami-decomps.github.io%2Fsmt3-decomp%2Fbadges%2Fcode.json
[smt3-functions]: https://img.shields.io/endpoint?url=https%3A%2F%2Fmegami-decomps.github.io%2Fsmt3-decomp%2Fbadges%2Ffunctions.json
[smt3-progress]: https://megami-decomps.github.io/smt3-decomp/

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
matching build does not mean the decompilation is complete. The remaining code
uses assembly fallbacks while its source, names and types are recovered.

| Version | Game | Serial | ELF SHA-1 |
|---|---|---|---|
| `smt3` | Shin Megami Tensei III: Nocturne (USA) | `SLUS_209.11` | `5facfbf7916836613449c922e26a4ec412207b32` |

The ELF is 4,551,696 bytes, entry `0x00100008`, `gp` `0x00554170`, built
`Sep  3 2004`. It is symbol-stripped but keeps section headers, so the build
also has to reproduce `.vutext`, `.vudata`, `.ctors`, `.dtors`, `.eh_frame`,
the `.DVP.*` VU overlay tables and the section-header region byte for byte.

The build is verified: `ninja` relinks the split objects and `objcopy`s the
result into a file byte-identical to the retail executable. The configured
C units comprise 293 game/engine units and 12 SDK/runtime units. The VU1
and VU0 microcode is assembled from the annotated listings in
`src/smt3/sdf/vu/`: all 2,061 instructions, 16,488 bytes, are reproduced
(see [VU microcode](docs/vu/README.md)).

Measured on 2026-10-28, the linked-image report verifies 985,208 of 1,851,268
game-code bytes (53.22%) and 7,456 of 9,022 game functions (82.64%) from C.
The remaining 1,566 game functions use assembly, including 261 with retained
non-matching drafts. SDK/runtime reconstruction is a separate category.
Function coverage runs ahead of byte coverage because small functions count
as much as large ones; neither drafts nor assembly fallbacks earn C credit.

The badges above track matched game and engine code bytes and function
counts. Assembly fallbacks earn zero credit, and SDK/runtime code, handwritten
assembly and VU1 microcode sit outside those totals. They read from the
[progress site](https://megami-decomps.github.io/smt3-decomp/), which CI
publishes to GitHub Pages on every push to `main` that has the retail
executable available. See [Reading progress](docs/progress.md) for how the
numbers are measured.

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
- [smt3-reconstructed-sources](https://github.com/smt-ret/smt3-reconstructed-sources)
  for the SMT3 HD derived names, reconstructed headers and source file names,
  and all existing SMT3
  decompilation efforts and the naming and type conventions this repo follows.
- [splat](https://github.com/ethteck/splat),
  [spimdisasm](https://github.com/Decompollaborate/spimdisasm),
  [m2c](https://github.com/matt-kempster/m2c) and
  [objdiff](https://github.com/encounter/objdiff).
- [decomp.me](https://decomp.me) for the ee-gcc 2.96 toolchain packaging.
- The Persona 3/4 decompilation projects, for SDK layout and naming
  conventions.
