# Reading progress

Nothing is published anywhere: no decomp.dev page, no badge, no tracker. This
page records how progress is measured and what the local reports measured last
time they were run.

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

All three work. `ninja report` runs `objdiff-cli report generate` (3.8.1) three
times and writes:

| File | Project | Units | Scope |
|---|---|---|---|
| `report.json` | `objdiff.json` | 102 | whole binary, all versions |
| `build/smt3/report.all.json` | `build/smt3/objdiff.json` | 102 | whole binary, by category |
| `build/smt3/report.json` | `build/smt3/progress/objdiff.json` | 92 | game-code units only |

With one version key the first two hold the same numbers. `configure.py`
assigns each unit a category from its path (`game`, `sdk`, `handwritten`,
`vu1`, `data`) and omits categories that no unit is in: objdiff measures an
empty category as 100% matched, which reads as a finished component rather than
an absent one. No SDK unit has been identified yet, so there is no `sdk`
category and the SDK's code sits inside the `game` denominator; every
percentage below is therefore a whole-binary figure, not game-code coverage.

For a single unit in the GUI or one-shot, use the root config, the only one
whose relative paths resolve from the directory `ninja` runs in:

```sh
tools/bin/objdiff-cli diff -p . -u smt3/newdata/datCalc            # interactive
tools/bin/objdiff-cli diff -p . -u smt3/newdata/datCalc datGetParam
```

### What a per-unit diff proves

`src/smt3/newdata/datCalc.c` checked end to end on 2026-10-17: its nine C
functions each diff at 100% and its 73 `INCLUDE_ASM` entries at 0%, because
`-DSKIP_ASM` leaves them out of the base object entirely, so there is nothing
for objdiff to credit. The same nine, and only those nine, are what
`tools/check_unit.py src/smt3/newdata/datCalc.c` compares against the retail
ELF at their real addresses (`9 match, 0 differ`).

What that 100% does not prove is production equality. The base object is the
unit compiled without its asm fallbacks, which is not the object the build
links: function order, literal pools and relocations all differ, and datCalc's
`.rodata` diffs at 0% in objdiff while the linked image is byte-identical to
retail. Objdiff answers "does this C emit the retail instructions for this
function in isolation". Only the checksum build answers "do the retail bytes at
this address come from C".

## Measured on 2026-10-17

At `5facfbf7916836613449c922e26a4ec412207b32`, with the retail checksum
verified in the same run.

| Measure | Game code (92 units) | Whole binary (102 units) |
|---|---|---|
| Code bytes matched | 1,704 / 1,976,292 (0.086%) | 1,704 / 2,036,072 (0.084%) |
| Functions matched | 119 / 9,694 (1.23%) | 119 / 9,699 (1.23%) |

`python tools/progress.py smt3` reports the same 119 functions and 1,704 bytes
from the source inventory alone, which is the cross-check that matters: the
report credits nothing that is still an `INCLUDE_ASM` line.

The 119 are not 119 reconstructions. 110 of them are empty-bodied functions
that compile to `jr $ra; nop`, 8 bytes each, 880 bytes in total. The remaining
nine are the reconstructed functions in `src/smt3/newdata/datCalc.c`, 824
bytes. Substantive C is therefore 824 bytes of 1,976,292 (0.042%) and 9
functions of 9,694 (0.093%); the function percentage is the one that flatters
the work most, since an empty function and a 120-byte one count the same.

No function anywhere is partially credited. A function left as `INCLUDE_ASM` is
absent from the base object, so objdiff scores it 0 rather than part marks:
every one of the 119 is at 100%, and `matched_code_percent` equals
`fuzzy_match_percent` for that reason.

Excluded from the game-code denominator, and matched nowhere:

| Category | Units | Code bytes | Matched |
|---|---|---|---|
| `handwritten` (`asm/smt3/hand/crt0`) | 1 | 468 | 0 |
| `vu1` (`.vutext`, `.vudata`) | 2 | 59,312 | 0 |
| `data` (splat `.data`/`.rodata`/`.bss` segments) | 7 | 0 | n/a |

## Percentages in these reports that are not measurements

A unit with no base object has nothing to compare, and objdiff scores an empty
comparison as 100%. The `data` and `handwritten` rows print percentages of that
kind (`matched_code_percent: 100` on a unit with no code, `matched_data_percent:
100` on a unit with no data). They are artifacts of an absent object, not
progress, and no number from them is quoted above. Data is not compared at all:
all 2,648,896 bytes of split data sit outside the game report, and the 147,080
bytes of data in the game report belong to the C units and match 0%.

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
