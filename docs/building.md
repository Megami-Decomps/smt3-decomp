# Building

Run the commands in the [quickstart](../README.md#quickstart) from the
repository root.

## Requirements

- Linux x86-64 or WSL2. The host must be able to run 32-bit i386 programs:
  the original compiler and assembler are i386 binaries, run under the pinned
  glibc that `tools/download_tools.py` fetches (see
  [Toolchain](toolchain.md#pinned-compiler-runtime)).
- Python 3.10+, `ninja`, `cpp`, `git`.
- `python -m pip install -r requirements.txt` (splat64, spimdisasm,
  rabbitizer, pyelftools, pycdlib, pyyaml, ninja, ninja_syntax, and m2c as an
  optional helper).
- `python tools/download_tools.py` for ee-gcc 2.96, ee-as, the decompals
  MIPS/PS2 binutils, objdiff-cli, dvp-as and the 32-bit glibc. Everything it writes
  lands under `tools/` and is ignored by git.
- Your own copy of the game. The retail executable is never distributed here:
  `orig/` is git-ignored except for its placeholder, and no disc image, ELF or
  extracted asset is committed.

Put the executable at `orig/smt3/SLUS_209.11`, either by extracting it from
your own disc image or by copying it from a mounted disc.

## Retail executable

```sh
python configure.py
ninja          # same as ninja smt3: build and verify
```

`configure.py` splits the executable with splat per `config/smt3/` and writes
`build.ninja` and `objdiff.json`. `ninja` compiles the C units, assembles the
remaining retail assembly, links, and checks the result against the retail
SHA-1.

The checksum step runs `sha1sum --quiet -c config/smt3/checksum.sha1`, which
`configure.py` writes from the recorded retail SHA-1 and points at the linked
image `build/smt3/SLUS_209.11`. It is silent on success. A mismatch prints

```
build/smt3/SLUS_209.11: FAILED
```

and the build fails.

## Checksum troubleshooting

The checksum covers the whole file, so a failure is not necessarily in the
function you just wrote.

1. Check the unit first:
   `python tools/check_unit.py src/smt3/<dir>/<unit>.c -v`. A clean unit that
   still breaks the link points at data, not codegen.
2. Re-run `python configure.py` after any change to a `config/smt3/` file or
   after renaming a symbol. Splat output and the ninja graph are generated, and
   a stale split silently links old assembly.
   When replacing assembly functions that own literals with C, use
   `python configure.py --force-split` to migrate their data ownership too.
   Run `ninja` to refresh the generated `build/eeasm/` includes before
   checking the unit if the resplit introduced a new standalone data file.
3. If the unit is clean and configure is fresh, suspect data placement. A new C
   definition can emit `.rodata`, `.lit4` or `.sdata` that the retail layout
   accounts for somewhere else, which shifts everything after it. The
   `INCLUDE_RODATA` and `INCLUDE_SDATA` lines in the unit exist to hold that
   data; do not delete or reorder them by hand.
   An `extern` reference does not preserve data that an old `INCLUDE_ASM`
   brought in with its function. After porting a function with embedded strings
   or constants that must remain external, run
   `python configure.py --force-split`: the split postprocessors regenerate
   the corresponding data imports around the C functions. Plain `configure.py`
   can reuse its cached split when only C source changed.
4. `.sbss` and `.bss` are aligned to 128 bytes, as in Sony's `app.cmd`. A
   wrongly sized `.bss` object moves every later object and fails the
   checksum rather than any single function check.
5. `ninja -t clean` and a fresh `configure.py` rule out stale objects. If the
   first build out of the box fails, the input ELF is wrong: verify its SHA-1
   against the [status table](../README.md#status-and-versions) before
   debugging anything else.

Remember that the first configure produces a matching build with no C at all.
A checksum pass is a statement about the build, not about progress; see
[Reading progress](progress.md).

### Compiler helper names in ports

A unit can match instruction-by-instruction yet fail to link if the compiler
emits a runtime helper under a name absent from the symbol map. The native
float conversions are now named `fptodp`, `dptofp` and `dptoul`, matching
EE libgcc's GOFAST names. No assembler equates are needed for these names.
Preserve each existing C declaration's native argument/return ABI when
renaming a helper; changing its types is a separate correction. The full
`ninja` link and checksum remain required after a clean unit check.

## Generated files

None of these are committed. `.gitignore` covers them.

| Path | Written by |
|---|---|
| `asm/smt3/` | splat, through `configure.py` |
| `build.ninja`, `.ninja_*` | `configure.py` |
| `objdiff.json` | `configure.py` |
| `build/smt3/` (objects, linked ELF, map) | `ninja` |
| `build/smt3/vu/*.bin` | `tools/vuas.py`, from the listings in `src/smt3/sdf/vu/` |
| `build/smt3/SLUS_209.11.ld`, `config/smt3/undefined_*_auto.txt` | splat |
| `config/smt3/checksum.sha1` | `configure.py` |
| `tools/bin/`, `tools/compilers/`, `tools/glibc32/` | `tools/download_tools.py` |
| `orig/smt3/SLUS_209.11` | you, from your own copy |

## Project structure

```text
config/versions.json        serial, SHA-1, gp and disc-image glob of each version
config/smt3/SLUS_209.11.yaml splat config
config/smt3/symbol_addrs.txt names and addresses
config/smt3/cflags.txt      per-unit compiler options, with evidence
src/smt3/<dir>/<unit>.c C units; INCLUDE_ASM marks functions not decompiled yet
include/                    common.h, include_asm.h and shared headers
tools/                      build, checking, splitting and analysis tools
docs/                       this documentation
asm/ build/ orig/           generated or extracted locally (git-ignored)
```

`.text` is in the `main` splat segment, split into named C units and
`code_<vram>` placeholders. Preserve the configured boundaries; see
[TU recovery](tu-recovery.md).

## Comparison tools

```sh
ninja objdiff                             # comparison objects for objdiff
ninja report                              # objdiff reports, see progress.md
python3 ../asm-differ/diff.py -mwo NAME   # reads diff_settings.py
```

The unit checker pairs MIPS HI16/LO16 addends in ELF relocation-table order,
not instruction order. ee-as can place a loop-backedge `lui` after its
`addiu` in `.text`, but put both initial and backedge HI16 records before the
shared LO16 record. `dds3InitRenderDrawEnv` is one such native case.

`permuter_settings.toml` configures decomp-permuter against the same compiler
and assembler wrappers.

See the [toolchain notes](toolchain.md) for compiler and assembler evidence,
and the [documentation index](README.md) for the full reference list.
