# Translation-unit recovery

`SLUS_209.11` has no `__FILE__` strings, so its source-file names have to come
from another build. They come from
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp), which projected the
names of a Nocturne Dec 26 2002 debug build (`SLPM_652.42`, 172 `__FILE__`
paths) onto DDS1 and DDS2 and recorded the result as named `c` subsegments in
`config/dds1/SLUS_209.74.yaml`. The names are dds-decomp's work and its
`docs/tu-names.md` is the account of how they were earned; this page only
describes carrying them back to the build they started from.

`tools/pair_dds1.py` produces `config/smt3/tu_candidates.tsv`. Nothing in it is
a splat subsegment. `config/smt3/SLUS_209.11.yaml` still has one
`code_001001D8` unit and keeps it until a reviewer confirms a candidate.

## Sources

| Build | ELF | Date | Use |
|---|---|---|---|
| Nocturne (USA) | `SLUS_209.11` | Sep 3 2004 | the target; 9,547 single-chunk functions in the IDB |
| DDS1 (USA) | `SLUS_209.74` | 2004 | 9,554 single-chunk functions; carries dds-decomp's 65 named units |
| SMT3 HD reconstructed sources | il2cpp metadata | - | 11,595 function-to-`.c` rows, used only as a check |

Both IDBs are read-only here. The IDAPython payload reads function bounds and
bytes and nothing else.

## Method

1. Each function in each IDB becomes `(address, size, hash, name)`. The hash is
   over the function's instruction words with `j`/`jal` targets and the low 16
   bits of every immediate-form opcode cleared, so two differently linked builds
   of the same code hash the same. Split-chunk functions are skipped.
2. Functions pair on `(size, hash)` with a 16-byte floor, dds-decomp's. A key
   unique on both sides is an anchor; the anchors are cut to their longest run
   increasing on both sides. A key held by several functions per side pairs only
   when the counts match and each pair falls in the same gap between anchors.
3. Each paired DDS1 function takes the unit name of the `c` subsegment it sits
   in. `game/code_<vram>` placeholders and the four units dds-decomp dropped as
   likely mispairs (`newbattle/nbSound`, `newbattle/nbMisc`,
   `newbattle/nbAiScript`, `effect/effBattleSystem`) carry nothing. The label
   moves to the SMT3 partner.
4. A unit's candidate range runs from its lowest to its highest labelled SMT3
   function. Contradictions are counted from the anchors dropped in step 2 as
   well, because a monotone pairing cannot contradict a span by construction.
5. Every span's SMT3 function names are looked up in the HD source map.
   Separators and case are ignored, so `sdf_PacDecode.c` and `sdfPacDecode`
   agree; a name that is only a suffix of the other (`EventPack.c` for
   `evtEventPack`) reads as `near`.

```sh
DDS_DECOMP=../dds-decomp python3 tools/pair_dds1.py
```

## What it produced

| Measure | Count |
|---|---|
| SLUS_209.11 functions / SLUS_209.74 functions | 9,547 / 9,554 |
| unique-hash anchors, before and after the monotone cut | 2,508 -> 2,401 |
| paired one-to-one | 3,040 |
| dropped as ambiguous | 1,211 |
| unit labels carried to SMT3 | 1,220 |
| units reached, of dds-decomp's 65 | 62 |
| candidate ranges | 62 |
| rejected for contradictions | 0 |

The 62 ranges span 0x65480 bytes of SMT3 `.text` start to start, hold 2,091
functions, and do not overlap. HD source names cover 59 of them: 51 agree, 1 is
`near`, 5 are mixed, 1 disagrees, 4 have no HD name at all.

`basic/dds3ModelBasic`, `basic/dds3UnitObjectBasic` and `newdata/datCalc` reached
no SMT3 function. The first two are one-function units in DDS1; `newdata/datCalc`
has 103 DDS1 functions, yet the only ones that reached SMT3 were 6 anchors the
monotone cut threw out; dds-decomp lists it among the blocks DDS relinked.

## Limits

- Zero contradictions is weak evidence, not strong. The monotone cut removes
  order violations before the ranges are formed, so only the 107 anchors it
  dropped could contradict anything, and the 7 of those with a usable unit name
  all land outside every span. Treat the contradiction column as a sanity check
  that fired correctly, not as a confirmation.
- A range is a lower bound on the file and an upper bound on nothing. dds-decomp
  says the same of its own ranges. A unit really ends at an original file
  boundary, which neither build records.
- A span can be too wide where the pairing put an outlier at one end.
  `hd_files` counts the distinct HD source files inside a span; above 1 the
  range swallows code from a neighbour. `field/fldFileResolver` is the worst,
  8 HD files across its 197 functions.
- Only 32% of functions paired. ee-gcc 2.96 output is stable, but SMT3 is a 2004
  build of a 2002 engine and DDS1 forked it, so most functions differ somewhere.
  The unpaired remainder carries no evidence either way.
- 24 SMT3 and 64 DDS1 functions are split into chunks in their IDBs and never
  pair. That is an IDB property, not a property of the binaries.
- The HD check is independent of the DDS1 pairing but is itself a recovery: its
  addresses are HD addresses, and the join here is on function name, which 353
  HD names share with another file. Shared names are dropped rather than guessed.

## Before a candidate becomes a subsegment

A reviewer must confirm, per range:

1. The range starts at a function whose predecessor belongs to something else,
   and the function after `end` does too. The TSV reports `end` as the last
   labelled function's start address, never a file boundary.
2. `.rodata`, `.lit4` and `.sdata` split along with it. dds-decomp's
   `tools/split_rodata.py`, `include_rodata.py` and `include_sdata.py` are
   vendored here for that, and a unit whose data cannot be split cleanly is not
   ready.
3. The unit relinks byte-identically. `configure.py` and the SHA-1 check are the
   only real test; a name that looks right and breaks the checksum is wrong.
4. `hd_files` is 1, or the disagreement is explained. The five mixed rows and
   the one disagreement below are each a real question, not noise.

| Row | Question |
|---|---|
| `field/fldFileResolver` 0x00125D40-0x0013C3D8 | 197 functions, 8 HD files (`fldDraw`, `fldPlayer`, `fldSky`, `fldCamera`, ...), only 34 naming `fldFileResolver`. The range needs splitting before it means anything. |
| `mc/mcManager` 0x0029B398-0x0029B728 | The only outright disagreement: HD calls all 7 known functions `sdfMc.c`, and every name in the span is `sdf*`. Either Nocturne's `mc/mcManager.c` became HD's `sdfMc.c`, or the pairing is wrong. |
| `effect/effPCPThunder` 0x00176560-0x0017D0A0 | 11 functions are HD `effPCPBezier.c`. Likely a neighbouring file inside the span. |
| `event/evtPolygonMovie` 0x001F1CD0-0x001F40E8 | 15 functions are HD `evtPicture.c`, and `event/evtPicture` has its own one-function candidate at 0x001F4340. The two ranges disagree about where the split is. |
| `script/scrCommonCommand` 0x0010B908-0x0010D030 | 6 functions are HD `scrParameterCommand.c`. dds-decomp already widened this unit on a Persona 4 transfer, so its end is the part to re-check. |
| `effect/effEvent` 0x001AC0A8-0x001AFFB0 | 3 functions are HD `effDebug.c` and 34 of the span's names are `Eve_*`, not `eff*`. dds-decomp notes this unit's DDS1 start was itself chosen from compile-order evidence. |
| `misc/l2d`, `basic/dds3ModelRangeBasic`, `sdf/sdfThread`, `sdf/sdfMovie` | No HD name anywhere in the span, so DDS1 is the only witness. `sdf/sdfThread` is a single function and `basic/dds3ModelRangeBasic` likewise. |

## Applied

60 of the candidate units are now `c` subsegments in
`config/smt3/SLUS_209.11.yaml`, with the unowned stretches between them left as
`code_<vram>` units, for 92 code units in total. The build was verified
byte-identical after the boundaries landed.

Two things this page does not yet carry, because the work that applied the
boundaries was interrupted before recording them:

- Per-candidate verdicts in `config/smt3/tu_candidates.tsv`. The status columns
  still describe the candidates as proposed, not as applied or rejected, so the
  file no longer says which of the 62 became a unit.
- The resolution of the six disputed cases listed above. The applied set is
  evidence that each boundary relinks byte-identically, which is not the same as
  evidence that it is an original file boundary.

`config/smt3/tu_evidence.tsv` holds boundary evidence found in this binary
alone, independent of the DDS1 pairing. Comparing the two is the next step:
a boundary both sources agree on is far stronger than either alone.
