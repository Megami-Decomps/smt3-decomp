# Matching by analogy: tools/find_similar.py

Every function already in C is a solved (assembly, C) pair. When an unmatched
function's instruction sequence is close to a matched one, that match's C body
is a template and the work drops to substituting constants, offsets and
callees. `tools/find_similar.py` runs that search over the whole image, and
also groups the unmatched functions among themselves, which is the per-unit
sibling search generalised across unit boundaries.

This page is what it measured. The short version: the technique is sharp but
small. It is worth running at the start of a round because it costs half a
minute and sometimes hands over a 460-byte function for a three-line edit, not
because it will clear the tail.

## Using it

    python3 tools/find_similar.py                      # ranked worklist
    python3 tools/find_similar.py --for NAME --body    # templates for one function, plus the C
    python3 tools/find_similar.py --clusters           # unmatched functions sharing a shape
    python3 tools/find_similar.py --stats              # the per-tier counts below
    python3 tools/find_similar.py --unit field/fldDraw

The worklist's `sub` column is the number of instructions the template does
*not* already supply. Read that, not the ratio. `--for NAME` prints those
substitutions one per line, so a three-substitution row says which three
constants to change before you open the assembly at all.

Both sides of every comparison come from one `objdump` of the retail `.text`,
cached under `build/find_similar/`, so a function written in C and one still in
assembly normalise identically. The cache is keyed on the retail image and the
`src/` timestamps and rebuilds itself; `--force` forces it.

### The four tiers

| tier | keeps | finds |
|---|---|---|
| `exact` | mnemonic, registers, immediates | the substitution list |
| `noimm` | mnemonic, registers | siblings differing only in constants |
| `noreg` | mnemonic, immediates | siblings whose allocation differs |
| `mnem` | mnemonic | the default for neighbour search |

A `jal` target is wildcarded at every tier, because two siblings call different
functions; the callee names are compared separately and reported as
substitutions. Branch and intra-`.text` jump targets become offsets relative to
the instruction, so the same control flow at two addresses compares equal.

## What it measured

Measured 2026-10-27, after the six conversions below had taken the cheapest
rows, with 2130 functions and 989,396 bytes still assembly. Rerun the tool
rather than quoting these.

Identical-sequence clustering, over all 9694 functions:

| tier | unmatched with an identical matched twin | with unmatched siblings only |
|---|---|---|
| `exact` | 8 (124B) | 56 (2600B) |
| `noimm` | 21 (1012B) | 249 (16096B) |
| `noreg` | 10 (180B) | 58 (2736B) |
| `mnem` | 31 (1280B) | 246 (16092B) |

So **exact sibling identity is nearly spent**: 31 functions and 1280 bytes,
0.13% of the remaining code. The 246-function sibling column is 162 sixteen-byte
SDK syscall stubs plus 84 real functions; the tool drops anything under eight
instructions, which leaves 34 clusters holding 83 unmatched functions, 7596
bytes of which follow from converting the first of each. Six of those clusters
cross a unit boundary, which a per-unit search cannot see: `nbE905_Model2Start`
against `nbE907_Model2Start` sits one unit apart.

Approximate neighbours are the larger pool, and the useful measure is
substitutions, not ratio:

| template within | functions | bytes | share of the remaining bytes |
|---|---|---|---|
| 1 substitution | 2 | 132 | 0.01% |
| 3 substitutions | 13 | 700 | 0.07% |
| 8 substitutions | 52 | 3072 | 0.31% |
| 20 substitutions | 151 | 10368 | 1.05% |
| 40 substitutions | 373 | 40600 | 4.10% |

**The sequence ratio is a bad proxy for the work left.** By ratio band, against
how many instructions the best template still does not supply:

| ratio | functions | median substitutions | median share of the function |
|---|---|---|---|
| 0.95 - 1.00 | 15 | 6 | 22% |
| 0.90 - 0.95 | 23 | 11 | 44% |
| 0.80 - 0.90 | 73 | 19 | 64% |
| 0.70 - 0.80 | 89 | 29 | 90% |
| 0.60 - 0.70 | 185 | 41 | 105% |
| below 0.60 | 1527 | 129 | 135% |

In the 0.75 to 0.85 band the median template differs in 26 instructions and not
one of those 60 functions is within three substitutions. That is a rewrite with
a misleading head start. So the worklist floors at ratio 0.90, where 37
functions and 6012 bytes sit, and ranks on retail bytes per substitution. A
floor of 0.75 shows 136 functions and 23,552 bytes, which is worth quoting only
to show how fast the quality falls off.

Name families are a separate signal worth keeping. A size band cannot see the
variant that grew: `effBTLModelLook2` is 272 bytes against `effBTLModelLook`'s
172, so the sequence search never offers it and scores an unrelated function
0.379 while the real template sits at 0.739. Measured over the remaining set,
81 functions have a shared-prefix candidate at ratio 0.6 or better and 38 of
those score higher than the size-banded search does. Family membership is only
a candidate generator, since `effBTLModelHide` shares the same stem and is a
different function; the results are ranked by sequence ratio and the junk sinks.

## Why it pays despite the small numbers

Three of the six functions converted here were **parked `NON_MATCHING` drafts
whose write-ups had diagnosed the cause wrongly**, and in each case a matched
sibling settled it in one reading.

`nbCameraBoss_BSSAMA_Command_Set`, 460B, was parked at 25 of 115 words with the
conclusion that ee-gcc CSEs an address retail recomputes and that "nothing in
the source appears to break it". Its sibling `nbCameraBoss_BSARI_Command_Set`,
already matched, 390 lines further down the same file, has an identical
mnemonic sequence and differs in three words, all `.lit4` loads. What the draft
had wrong was the spelling of the camera pointer: the sibling passes
`pNbCamera` where the draft wrote `&pNbCamera->target`, stores through
`SDF_VU0_STOREVECTOR_NOMEM`, and takes the length into `pNbCamera->distance`
directly instead of through a local. Copying the sibling and changing 950.0f to
550.0f matched on the first compile.

`Chk_ENHOJOMAX` and `Chk_FRHOJOMAX`, 220B each, were parked at 54 of 55 words
each, blaming a branch-annul decision and ruling out three source shapes. At
the exact tier both are **identical** to the matched `Chk_ENHOJO` and
`Chk_FRHOJO` except for one callee name, which is
[callee visibility](levers/callee-visibility.md) and nothing else. Writing that
callee, `ChkAiHojoMax`, in C closed both without touching them.

The lesson is about where to spend a draft's budget. **A draft one or two words
out, in a unit that holds a matched sibling of the same shape, is almost never
a codegen mystery.** Diff it against the sibling at the exact tier first: the
answer is a constant, an argument spelling, or the callee.

## The six converted, as end-to-end validation

1940 bytes, every one picked off this tool's output.

- **`nbCameraBoss_BSSAMA_Command_Set`** 460B, from
  `nbCameraBoss_BSARI_Command_Set`, same unit, three substitutions.
- **`ChkAiHojoMax`** 404B, from `ChkAiHojo`, same unit. The sequence search
  missed it, best 0.400, because the `Max` variant is 404B against 336B; the
  name family found it. The case ladder is the same, testing the counter
  against `nbGetHojoCounterMax`/`Min` where `ChkAiHojo` tests its sign.
- **`Chk_ENHOJOMAX`** and **`Chk_FRHOJOMAX`** 220B each, identical to
  `Chk_ENHOJO` and `Chk_FRHOJO` but for the callee.
- **`nbPnl_rsetAlpha`** 260B, from `eglLinkAlpha` in `kernel/dds3KernelDraw`,
  three substitutions: its own jump table, and `gnbPnlOtOfs` where the template
  reads `egl_OT_Offset`. Cross-unit and cross-module.
- **`nbPnlSprite`** 376B, from `DrwSprite2D` in `field/fldDraw`, one
  substitution, `gnbPnlOtOfs` for `rsetFrameMaskOtOffset`. It also needed
  `fldDraw`'s two-corner sprite vertex struct, copied under `newbattle` names
  on the pattern `fldSpriteCorner_t` and `dds3EffectSpriteCorner_t` already
  set; if the IDB carries a name for that layout, take that one instead.

`tools/check_unit.py` ends in `0 differ` on `newbattle/nbCameraBoss` (100
match), `newbattle/nbAi` (42 match) and `newbattle/nbPanelProcess` (53 match),
and `ninja` reaches `check config/smt3/checksum.sha1`. Two of the six are
unreachable from the per-unit sequence search the method started as: one came
only from the name family, one only from a cross-unit pairing.

## What did not pay

- **Bulk conversion from matched templates.** The expectation behind this work
  was that the 7558 matched functions would serve a large share of the tail.
  They do not. Over all 1928 remaining functions of eight instructions or more,
  the best matched neighbour has a median mnemonic-sequence ratio of 0.454, and
  37 reach 0.90. At 0.60 the pool is 363 functions and 6.4% of the remaining
  bytes, but at that distance the template is a structural lead, not a
  substitution, and reading it is no cheaper than reading the assembly.
- **Sibling clusters with no matched member.** `ragBuildList2` and
  `ragBuildList3` have an identical mnemonic sequence and both carry a draft 22
  of 93 words out; the nearest matched function scores 0.486. The cluster says
  they are stuck for the same reason, which stops two people paying for one
  investigation, but it solves neither. 34 of the 42 clusters with two or more
  unmatched members have no matched member at all.
- **Control-flow-shape-only matching.** Not built. The `noreg` tier is already
  a coarser view than `noimm` and finds strictly fewer twins, 10 against 21,
  which says register identity carries more signal here than immediate
  identity. A basic-block-graph tier would be coarser still, and the
  substitutions table says coarse matches are not worth reading.

## One trap in building anything like this

`objdump -d` prints `...` for a run of zero words and silently drops them.
Without `-z` that corrupts the instruction sequence of 1609 of this image's
functions, and the corruption is invisible: the sequences still compare, just
wrongly. `corpus()` therefore asserts that every function's token count equals
the instruction count the split assembly reports, and raises if it does not.
`tools/test_find_similar.py` pins that invariant.

The same class of error is the one `docs/nonmatching.md` warns about from the
other direction: count instructions, never the `/* ... */` comment lines, since
a `.s` file's `.rodata` jump table shares that comment format.

## Where the work actually goes

Measured over 432 agent transcripts of this project's own decompilation rounds,
24,224 shell calls in total:

| what | calls |
|---|---|
| `grep` | 9732 |
| `sed -n` | 4651 |
| `check_unit.py` | 6028 |
| `configure.py` | 1965 |
| `git` | 1846 |
| `ninja` | 1654 |
| `revert_differing.py` | 227 |
| `decompile.py` (m2c drafts) | 38 |
| `dds_reuse.py` | 26 |
| `perm_unit.py` (permuter) | 19 |

Two things follow, and they are the ones worth acting on.

**Reading is the cost, not compiling.** 14,383 of the 24,224 calls, 59%, are
`grep` and `sed -n`: finding the lever, the draft entry, the sibling, the
assembly. Anything that turns a grep hunt into one answer is worth more than
another lever page. That is the case for `--for NAME --body`, which replaces a
grep for the sibling, a `sed -n` into its source, and a read of the assembly.

**The compile-check loop is brute-forced.** 169 transcripts call
`check_unit.py`, a median of 37 times each and up to 87, and of the results
3226 reported a differ against 1940 reporting `0 differ`. Nearly two thirds of
compile checks are misses. The output says which words differ but not which
compiler decision produced them, so the next attempt is a guess from the lever
list. Making `check_unit.py` name the responsible decision, even coarsely
(annul bit, register allocation, literal pool slot, frame offset, instruction
scheduling), would attack the largest cost after reading. That is judgement,
not measurement.

The permuter and the m2c draft path are, by these counts, unused: 19 and 38
calls out of 24,224. Importing more search machinery is not the gap.
