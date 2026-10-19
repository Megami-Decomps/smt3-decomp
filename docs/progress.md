# Reading progress

This page records how progress is measured and what the local reports
measured last time they were run. CI publishes the same reconciled numbers to
the [progress site](#progress-site).

## What counts

A function counts only when the normal build proves that C supplies the exact
retail bytes for it. Three things that look like progress do not count:

- **A matching build.** The ELF is byte-identical from the first configure,
  because every function that has no C is assembled from the retail code. A
  passing checksum says the build is faithful, not that anything is
  decompiled. An all-assembly build earns zero credit.
- **Removing an `INCLUDE_ASM` line.** `python tools/progress.py` inventories
  functions and retail code bytes no longer named by an `INCLUDE_ASM`
  directive. That is source coverage. It does not compile or compare anything,
  so it is an upper bound on what is actually matched.
- **A non-matching draft.** A reconstruction kept behind `#ifdef NON_MATCHING`
  is never compiled by the build, which takes the assembly fallback instead.
  Drafts are counted on their own, against what is still assembled rather than
  against the game, and are never added to matched functions or bytes. See
  [Non-matching drafts](nonmatching.md).

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
ninja report                    # reconciled reports
```

All three work. `ninja report` runs `objdiff-cli report generate` (3.8.1) three
times, then reconciles each result with `tools/production_report.py`:

| File | Project | Units | Scope |
|---|---|---|---|
| `report.json` | `objdiff.json` | 102 | whole binary, all versions |
| `build/smt3/report.all.json` | `build/smt3/objdiff.json` | 102 | whole binary, by category |
| `build/smt3/report.json` | `build/smt3/progress/objdiff.json` | 92 | game-code units only |

Each of the three is written three times over: `<file>.raw` is what objdiff
measured, `<file>` is the same report with code credit replaced by what the
reconciler can prove, and `<file>.proof.json` is the per-function evidence
behind that. The published numbers are the reconciled ones.

### What the reconciler proves

`tools/production_report.py` credits a function only when four independent
pieces of evidence agree, and it does not fall back to anything weaker when one
is missing:

- **Compiler provenance.** The `.ent`/`.end` pair for the function appears in
  the cc1 assembly of the C unit, outside any `#APP` block, so the instructions
  came from the compiler and not from an `INCLUDE_ASM` fallback or inline asm.
- **An unambiguous production object.** The object the build actually links
  (not the `-DSKIP_ASM` objdiff base) owns exactly one span for that function.
- **A link owner.** The link map places that object's `.text` input section at
  an address, and the linked ELF has a symbol of that name, address and size
  inside it.
- **Exact bytes.** The linked bytes over the function's span equal the retail
  bytes at the same file offset.

The owning symbols have to tile the retail span. The one gap allowed is a
trailing alignment fill, where a C body is one instruction shorter than the
retail function and `.text` alignment supplies the last word; the exact-bytes
check still covers it.

The `cc` and `ld` rules write `<out>.proof.json` receipts of SHA-256 hashes for
their inputs, and the reconciler re-hashes the sources, flags and configs those
receipts name. A source edited after the build, or a receipt that no longer
matches, fails the report instead of being reported as progress. A function
whose body is mostly inline asm (at least 8 instructions and more than half the
body) is refused as well, since the compiler is then only the frame around
handwritten code. Any gap in the chain exits non-zero with
`production report proof failed: ...`.

With one version key the first two reports hold the same numbers. `configure.py`
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

## Measured on 2026-10-18

At `5facfbf7916836613449c922e26a4ec412207b32`, with the retail checksum
verified in the same run.

| Measure | Game code (98 units) | Whole binary (108 units) |
|---|---|---|
| Code bytes matched | 593,740 / 1,976,292 (30.043%) | 593,740 / 2,036,072 (29.161%) |
| Functions matched | 5,363 / 9,694 (55.32%) | 5,363 / 9,699 (55.29%) |

`python tools/progress.py smt3` reports the same 5,363 functions and 593,740
bytes from the source inventory alone, which is the cross-check that matters:
the reconciler credits nothing that is still an `INCLUDE_ASM` line, and the
source inventory credits nothing the link does not supply from C. 4,331
`INCLUDE_ASM` lines remain in `src/smt3`. Its second line counts the
non-matching drafts among those, which earn nothing in either measure.

Raw objdiff, before reconciliation, reports 5,153 functions and 512,244 bytes
for the same tree (`build/smt3/report.json.raw`). The 210-function, 81,496-byte gap
is in the other direction from what the caveats below might suggest: those
functions are exact in the linked image and imperfect in the `-DSKIP_ASM` base
object, so raw objdiff understates them. Reconciliation moves credit both ways
and only on evidence from the real link.

588 of the 5,363 are two-instruction functions, 8 bytes each, 4,704 bytes in
total: empty bodies that compile to `jr $ra; nop` and returns of a single
value. The function percentage is the measure that flatters the work most,
since an 8-byte function and a 120-byte one count the same; code bytes are the
honest headline.

No function anywhere is partially credited. A function left as `INCLUDE_ASM` is
absent from the base object and has no C provider in the link, so it scores 0
rather than part marks.

Excluded from the game-code denominator, and matched nowhere:

| Category | Units | Code bytes | Matched |
|---|---|---|---|
| `handwritten` (`asm/smt3/hand/crt0`) | 1 | 468 | 0 |
| `vu1` (`.vutext`, `.vudata`) | 2 | 59,312 | 0 |
| `data` (splat `.data`/`.rodata`/`.bss` segments) | 7 | 0 | n/a |

## Progress site

`tools/progress_site.py` renders `build/smt3/report.json` (the headline and
units) and `build/smt3/report.all.json` (the categories) as a static site:

```sh
ninja report
python tools/progress_site.py smt3          # writes build/site/
python -m http.server -d build/site         # preview
```

| File | Contents |
|---|---|
| `index.html` | headline, draft counter, history chart, categories, every unit with its functions |
| `history.json` | one entry per published commit: matched and total code bytes and functions |
| `report.json` | the game-scope report the page was rendered from |
| `badges/code.json`, `badges/functions.json` | [shields.io endpoint](https://shields.io/badges/endpoint-badge) badges the README uses |

On a push to `main` with the retail executable available, the `Build`
workflow fetches the `history.json` the site currently serves, renders the
site with the new commit added, and deploys it to GitHub Pages from a separate
job that holds the only `pages: write` token. Only reconciled credit is
published, never `.raw` objdiff numbers. Pushes to `main` run one at a time so
each publish extends the history the previous one served; a fetch that fails
with anything but 404 stops the publish instead of restarting the history.

One-time setup in the repository settings: **Pages → Build and deployment →
Source: GitHub Actions**, plus the `ORIG_REPO` and `ORIG_DEPLOY_KEY` secrets
the build already needs. The site is then at
<https://megami-decomps.github.io/smt3-decomp/>.

## Percentages in these reports that are not measurements

A unit with no base object has nothing to compare, and objdiff scores an empty
comparison as 100%. The `data` and `handwritten` rows of a `.raw` report print
percentages of that kind (`matched_code_percent: 100` on a unit with no code,
`matched_data_percent: 100` on a unit with no data). They are artifacts of an
absent object, not progress. The reconciler recomputes code credit and the
aggregates over it, so those code figures read 0 in the reconciled report, but
it leaves data measures alone and the empty-comparison data percentages
survive. Data is not compared at all: all 2,648,896 bytes of split data sit
outside the game report, and the 147,080 bytes of data in the game report
belong to the C units and match 0%.

Objdiff bases are compiled separately with `-DSKIP_ASM`, which changes compiler
context: ee-gcc 2.96 can then select different instructions, and relocatable
objects can encode the same final address or literal differently. A function
can therefore be exact in the real build and imperfect in objdiff, or exact in
objdiff and not in the linked image. That is the discrepancy the reconciler
removes: a credited function in a reconciled report is one whose retail bytes
the linked build demonstrably takes from C, which is the same question
`tools/check_unit.py` and the retail checksum answer per unit, as in
[Contributing](CONTRIBUTING.md#3-verify).

What is still not a measurement, after reconciliation: the denominator is the
whole binary rather than game code, because no SDK unit has been identified
yet; data is never credited; and a function counts once whatever its size, so
the function percentage flatters small bodies. The reconciler proves ownership
of bytes, not that a reconstruction is readable or correct beyond matching.
These numbers come from a local `ninja report`; the
[progress site](#progress-site) tracks them per commit.
