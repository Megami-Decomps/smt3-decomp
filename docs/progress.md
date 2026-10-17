# Reading progress

There are no published progress numbers for this repo yet, and no decomp.dev
page. This page records how progress is measured, so the first numbers mean
something when they appear.

## What counts

A function counts only when the normal build proves that C supplies the exact
retail bytes for it. Two things that look like progress do not count:

- **A matching build.** The ELF is byte-identical from the first configure,
  because every function that has no C is assembled from the retail code. A
  passing checksum says the build is faithful, not that anything is
  decompiled. An all-assembly build earns zero credit.
- **Removing an `INCLUDE_ASM` line.** `python tools/progress.py` inventories
  functions and retail code bytes no longer named by an `INCLUDE_ASM`
  directive. That is source coverage. It does not compile or compare anything,
  so it is an upper bound on what is actually matched.

Unfinished functions stay in the denominator. Function counts and code bytes
are reported separately: a short and a large function each count once in the
first, and by size in the second.

## Scope

The reconstruction target is Atlus game and engine EE code. These are tracked
separately, not folded into the main denominator, since no C can replace them:

| Excluded from the main denominator | Why |
|---|---|
| Sony SDK 2.5.x, newlib, libgcc | prebuilt archives, not Atlus source |
| `asm/smt3/hand/crt0` | handwritten `_start` and stubs |
| other handwritten assembly (VU0 loops) | never was C |
| `.vutext`/`.vudata` VU1 microcode | not EE code |
| `.DVP.*` overlay tables | generated data |

The split between game code and SDK code depends on signature identification
of the SDK archives, which has not been done for this binary yet. Until it is,
any percentage over the whole `.text` understates the game-code share, and
should be labelled as whole-binary rather than game-code progress.

## Reports

```sh
python tools/progress.py smt3   # source coverage
ninja objdiff                   # build the comparison objects
ninja report                    # objdiff reports
```

`configure.py` writes `objdiff.json`, so objdiff can compare a unit against the
retail code locally. `ninja report` runs `objdiff-cli report generate` and
writes `report.json` (whole binary), `build/smt3/report.all.json` (the same
scope with categories) and `build/smt3/report.json` (game-code units only).
`configure.py` assigns each unit a category from its path (`game`, `sdk`,
`handwritten`, `vu1`). Today only `asm/smt3/hand/crt0` is `handwritten` and the
VU1 sections are `vu1`; no SDK unit has been identified, so the SDK's code sits
inside the `game` denominator. A percentage from these reports is therefore a
whole-binary figure, not game-code coverage, until the SDK archives are matched
by signature and split out.

Objdiff bases are compiled separately with `-DSKIP_ASM`, which changes compiler
context: ee-gcc 2.96 can then select different instructions, and relocatable
objects can encode the same final address or literal differently. A function
can therefore be exact in the real build and imperfect in objdiff. The
authoritative per-function check is `tools/check_unit.py` plus the retail
checksum build, as in [Contributing](CONTRIBUTING.md#3-verify).

dds-decomp solves that discrepancy with a production-proof reporter
(`tools/production_report.py`) that binds compile and link receipts to the
linked ELF and gives C credit only for proven production bytes. It is not
vendored here, so this repo's reports are raw objdiff measures, and nothing is
published anywhere.
