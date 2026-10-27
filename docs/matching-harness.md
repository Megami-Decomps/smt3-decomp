# Matching by analogy: what the numbers say

`tools/find_similar.py` answers one question: is some function already written
in C close enough to an unmatched one that its body is a template? Every
matched function is a solved (assembly, C) pair, so where the shapes agree the
work drops to substituting constants, offsets and callees.

This page is the measurement, including the parts that do not pay. Rerun the
tool rather than quoting the counts; they fall as work lands.

    python3 tools/find_similar.py                      # ranked worklist
    python3 tools/find_similar.py --for NAME [--body]   # templates for one function
    python3 tools/find_similar.py --clusters            # unmatched functions of the same shape
    python3 tools/find_similar.py --stats               # the per-tier counts below

## Method

Both sides come from one `objdump -d -z` of the retail `.text`, cached under
`build/find_similar/`. That is the point: a function written in C and one still
in assembly normalise through the same printer, so they compare without an
asymmetry. The inventory is the one `tools/progress.py` counts, 9694 functions,
and the tool refuses to run if the disassembly does not cover it instruction
for instruction.

Four tiers, coarsening in this order:

| tier | keeps |
|---|---|
| `exact` | mnemonic, registers, immediates |
| `noimm` | mnemonic, registers |
| `noreg` | mnemonic, immediates |
| `mnem` | mnemonic |

A `jal` target is wildcarded at every tier, because two siblings call different
functions; the callee names are resolved against the inventory and compared
separately. Branch and intra-`.text` jump targets become offsets relative to
the instruction, which is control-flow shape rather than an address.

`-z` is not optional. Without it objdump prints `...` for a run of zero words
and silently drops the nops inside 1609 of this image's functions, which
corrupts every sequence that contains one.

## Identical shapes are nearly spent

Measured 2026-10-27, 2131 functions still assembly:

| tier | unmatched with an identical matched twin | with unmatched siblings only |
|---|---|---|
| `exact` | 8 (124B) | 56 (2600B) |
| `noimm` | 22 (1388B) | 249 (16096B) |
| `noreg` | 10 (180B) | 58 (2736B) |
| `mnem` | 32 (1656B) | 246 (16092B) |

So exact-sequence matching against the 7563 solved functions reaches 32
functions and 1656 bytes, under 0.2% of the 989,772 bytes left. **The
"match every remaining function by analogy from a matched one" idea does not
scale.** That is the headline, and it is measured.

The tiers rank as expected except `noreg`, which is useless: dropping register
numbers while keeping immediates finds fewer twins than keeping both. Register
allocation is what varies between siblings, so a tier that discards it and
keeps the constants discards the signal and keeps the noise. Use `mnem` or
`noimm`.

The sibling column is the per-unit cluster search generalised. Filtered to
functions of at least eight instructions, so the 162 identical 16-byte SDK
syscall stubs drop out, it is 34 clusters holding 83 unmatched functions, of
which 7596 bytes follow from converting the first of each. Six clusters cross
a unit boundary, which a per-unit search cannot see: `nbE905_Model2Start`
against `nbE907_Model2Start` is one unit apart, and `nbPnlSprite` took its
template from `field/fldDraw` while sitting in `newbattle`.

## Approximate similarity: the ratio is a bad signal, the substitution count is good

Widening from identity to a sequence ratio finds more candidates, and most of
them are worthless. For every unmatched function, the closest matched function
by `mnem` ratio (size band plus name family, see below), against how many
instructions the template still does not supply:

| ratio | functions | bytes | median substitutions | median share of the function |
|---|---|---|---|---|
| 0.95 - 1.00 | 15 | 2156 | 6 | 22% |
| 0.90 - 0.95 | 23 | 4232 | 11 | 44% |
| 0.80 - 0.90 | 73 | 12228 | 19 | 64% |
| 0.70 - 0.80 | 89 | 17136 | 29 | 90% |
| 0.60 - 0.70 | 185 | 38184 | 41 | 105% |
| below 0.60 | 1527 | 863004 | 129 | 135% |

At a ratio of 0.8 the median template differs in two thirds of its
instructions. That is a rewrite with a misleading head start, not a template.
A plausible-looking row such as `cmpSetAfterCrossFunc` against `effBTLCreate`
at 0.818 pairs a battle-effect constructor with a cross-fade setter: the same
instruction mix, unrelated work.

So the worklist floors at 0.90 and ranks by **retail bytes per substitution**,
not by ratio, and prints the substitution count instead of the score. 38
functions and 6388 bytes clear that floor. Dropping the floor to 0.75 shows
137 functions and 23,928 bytes, which is the number to quote only if someone
wants to see how fast the quality falls off.

The substitution count is also the whole payload: `--for NAME` prints the
exact-tier differences and the callee substitutions, which is the list of
edits, and `--body` prints the template's C with its file and line.

## A name family beats a size band when the variant grew

The sequence search only looks within 15% of the target's instruction count,
which is what makes it fast, and that band hides the variant that got longer.
`effBTLModelLook2` is 272 bytes against `effBTLModelLook`'s 172, so the
sequence search scores it 0.379 against an unrelated function while the real
template sits at 0.739. A shared name prefix of eight characters or more finds
it regardless of size.

Measured: 38 unmatched functions have a name-family relative scoring 0.6 or
better, 10,212 bytes, and 15 of those the size-banded search scores lower.
It is only a candidate generator, not a claim of relatedness, since
`effBTLModelHide` shares the same stem and is a different function. Rank what
comes back by sequence ratio and read the top of it.

## Validated end to end

Six functions, 1940 bytes, every one picked off this tool's output. Three had
been parked as `NON_MATCHING` drafts whose write-ups named the wrong cause.

- **`nbCameraBoss_BSSAMA_Command_Set`**, 460B, from `nbCameraBoss_BSARI_Command_Set`
  in the same unit at `mnem` 1.000, three exact-tier differences, all of them
  `.lit4` slots. The parked draft was 25 of 115 words in and blamed a CSE of a
  frame address that "nothing in the source appears to break"; the template
  shows the real difference is `SDF_VU0_STOREVECTOR_NOMEM` against
  `SDF_VU0_STOREVECTOR` plus passing `pNbCamera` where the draft passed
  `&pNbCamera->target`. Matched on the first compile.
- **`ChkAiHojoMax`**, 404B, from `ChkAiHojo` in the same unit. The sequence
  search missed it (best 0.400, the `Max` variant is 404B against 336B); the
  name family found it. The case ladder is the same, testing the counter
  against `nbGetHojoCounterMax`/`Min` where `ChkAiHojo` tests its sign.
- **`Chk_ENHOJOMAX`** and **`Chk_FRHOJOMAX`**, 220B each, identical to
  `Chk_ENHOJO` and `Chk_FRHOJO` at the `exact` tier with a single differing
  callee. Both were parked at 54 of 55 words with a `bnel` for retail's `bne`,
  and both closed by themselves once `ChkAiHojoMax` became C, exactly as
  [callee visibility](levers/callee-visibility.md) predicts. Worth noting for
  the ranking: a one-word annul miss whose callee is still `INCLUDE_ASM` is
  not 54/55 of the way done, it is a dependency, and this tool's cluster output
  is where that callee's own template turns up.
- **`nbPnl_rsetAlpha`**, 260B, from `eglLinkAlpha` in `kernel/dds3KernelDraw`,
  three exact-tier differences: its own jump table, and `gnbPnlOtOfs` where the
  template reads `egl_OT_Offset`. A cross-unit, cross-module template.
- **`nbPnlSprite`**, 376B, from `DrwSprite2D` in `field/fldDraw`, one
  substitution: `gnbPnlOtOfs` for `rsetFrameMaskOtOffset`. It also needed
  `fldDraw`'s two-corner sprite vertex struct, copied under `newbattle` names
  on the pattern `fldSpriteCorner_t` and `dds3EffectSpriteCorner_t` already
  set; if the IDB carries a name for that layout, take that one instead.

`python3 tools/check_unit.py` ends in `0 differ` on all three units, and
`ninja` reaches `check config/smt3/checksum.sha1`.

One of the six, `ChkAiHojoMax`, came only from the name family, and one,
`nbPnlSprite`, only from a cross-unit pairing. Neither is reachable from the
per-unit sequence search the method started as.

## What this does not do

- It cannot find a template for a function that has none, which is 1527 of the
  1929 remaining functions over eight instructions and 87% of the bytes left.
  Those need the ordinary method.
- A cluster whose members are all unmatched tells you the members are stuck
  identically, which saves one of two investigations but solves neither.
  `ragBuildList2` and `ragBuildList3` are byte-shape identical, both parked at
  22 of 93 words, and the nearest matched function scores 0.486.
- The `jal` wildcard means a sibling pair that differs only in a callee reads
  as identical. That is usually what you want and it is why the callee list is
  printed separately, but a pair reported as identical can still be two
  different functions over the same helper.
