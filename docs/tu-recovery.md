# Translation-unit recovery

`SLUS_209.11` has no `__FILE__` strings, so its source-file names have to come
from another build. They come from
[dds-decomp](https://github.com/Megami-Decomps/dds-decomp), which projected the
names of a Nocturne Dec 26 2002 debug build (`SLPM_652.42`, 172 `__FILE__`
paths) onto DDS1 and DDS2 and recorded the result as named `c` subsegments in
`config/dds1/SLUS_209.74.yaml`. The names are dds-decomp's work and its
`docs/tu-names.md` is the account of how they were earned; this page describes
carrying them back to the build they started from, and what a second,
independent pass over this binary says about the result.

Because those `__FILE__` paths are Nocturne's own, a dds-decomp unit name is a
statement about this game's source tree, not about DDS's. That matters below.

## Sources

| Build | ELF | Date | Use |
|---|---|---|---|
| Nocturne (USA) | `SLUS_209.11` | Sep 3 2004 | the target; 9,547 single-chunk functions in the IDB, 9,698 function starts in the split |
| DDS1 (USA) | `SLUS_209.74` | 2004 | 9,554 single-chunk functions; carries dds-decomp's 65 named units |
| SMT3 HD reconstructed sources | il2cpp metadata | - | 11,595 function-to-`.c` rows |

Both IDBs are read-only here. The IDAPython payload reads function bounds and
bytes and nothing else.

The three evidence orders used throughout: the IDB and the retail bytes decide;
`a checkout of smt3-reconstructed-sources` is often right but lossy, a
hint rather than proof; a DDS1-carried name is a claim about the name, and only
a lower bound on the range.

## Three independent sources, and what each can say

| Source | File | Can say | Cannot say |
|---|---|---|---|
| DDS1 pairing | `config/smt3/tu_candidates.tsv` | a file of this name exists and covers at least `[start, end]` | where the file begins or ends |
| HD source map | the HD reconstruction's `symbols.txt` | which `.c` each named function came from, hence a run boundary address | nothing about functions it does not name, and its file names are the HD remaster's, not Nocturne's |
| This binary | `config/smt3/tu_evidence.tsv` | that a boundary cannot be inside a proven same-unit group, and that one must exist inside a duplicate-literal window | no exact boundary address on its own: no duplicate-literal window here narrows to a single position |

DDS1 and the HD map share no inputs, so where they agree there are two
witnesses. The binary-only rules share no inputs with either.

## Method

`tools/pair_dds1.py` produces `config/smt3/tu_candidates.tsv`:

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

`tools/find_boundaries.py` produces `config/smt3/tu_evidence.tsv` from the split
asm, the retail ELF and the HD source map, without reading the IDB. Its header
lists its rules and the rules that produced nothing.

`tools/compare_boundaries.py` scores the applied subsegments against both and
fills columns 14-22 of `tu_candidates.tsv`:

```sh
DDS_DECOMP=../dds-decomp python3 tools/pair_dds1.py
python3 tools/find_boundaries.py smt3
python3 tools/compare_boundaries.py smt3 --write-candidates
```

`--write-candidates` rewrites only columns 14-22 and is idempotent. The
`action` and `action_note` values are review decisions held in the tool, not
derived, and the evidence for each is in this page.

## What the pairing produced

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
monotone cut threw out. All three are placed below from the HD map instead.

## Applied

60 units are `c` subsegments in `config/smt3/SLUS_209.11.yaml`, with the unowned
stretches between them left as `code_<vram>` units, for 92 code units in total.
The build was verified byte-identical after the boundaries landed.

Of the 62 candidates, 53 were applied with the candidate span inside the
subsegment, 6 were superseded (narrowed or moved off the candidate span), and 3
were not applied. The 60th unit, `newdata/datCalc` at
`0x00242A88-0x002460B0`, has no candidate row: it was placed from the HD map
alone, where `datCalc.c`'s run starts and ends exactly there.

Relinking byte-identically is necessary and says nothing about file boundaries:
any cut between two functions relinks, because the sections are concatenated in
the same order either way. The verdicts below are the part that carries
evidence. They are recorded per candidate in `config/smt3/tu_candidates.tsv`
(`disposition`, `support`, `hd_fit`, `witnesses`, `action`).

## The comparison

### Where the applied starts came from

56 of the 60 applied starts sit exactly on an HD source-run boundary. That is
how they were chosen, which makes the `hd-source` rows in `tu_evidence.tsv`
useless as confirmation of them: both read the same map. The columns therefore
separate `echo` (only `hd-source` or `prefix` rows at the address) from
`supported` (a binary-only rule: `dup-string`, `core-edge` or `dup-func`).

### Binary-only verdict, 60 applied starts

| Verdict | Count | Reading |
|---|---|---|
| supported | 1 | `event/evtPolygonMovie` at `0x001F0170`, where a same-unit group begins |
| echo | 36 | the HD map talking twice |
| silent | 22 | no row of any kind at the address |
| contradicted | 1 | `file/fileManager`, at its end, not its start |

One of the 92 `c` subsegment edges cuts a proven same-unit group:
`0x00297F68` falls inside `0x00297990-0x00298050`, where `frAllocNameData` and
`nmeLoadFont` both name the literal `/font/name_ent.fnt`. That is a real error
and the only one the binary can prove.

The hard test is weak but not vacuous: 370 of 9,698 positions are inside a
group, so 92 random edges would cut 3.5 of them. Getting 1 is better than
chance and not significantly so.

The in-binary rules cannot do better than this at pinning a start. No
duplicate-literal window in this ELF narrows to a single position, and a
same-unit group edge marks a place a boundary is *allowed*, not one where it
must be. Treat `silent` as "this source has nothing to say here", not as doubt.

### Two-source verdict, 60 applied starts

The useful agreement is between the DDS1 name and the HD address, which share
no inputs.

| Fit | Count |
|---|---|
| HD run starts here and its file name matches the unit name | 54 (14 of them also end where the unit ends) |
| HD name is a suffix of the unit name (`EventPack.c` / `evtEventPack`) | 1 |
| HD run starts here, HD file name disagrees (`sdfMc.c` / `mc/mcManager`) | 1 |
| HD names nothing at the start function, DDS1 is the only witness | 4 |

So 55 of 60 applied starts have two independent witnesses on both the name and
the address. The four single-witness ones are `script/scrScriptProcess`
(`0x00109C58`), `effect/effMagatuhi` (`0x001A51E0`), `sdf/sdfMovie`
(`0x002C53C8`) and `sdf/sdfPacDecode` (`0x002C6030`). Reading their
neighbourhoods in the IDB, three sit on a clean name transition that
`find_boundaries.py`'s prefix rule missed for mechanical reasons
(`fldFlowParse` -> `scrLoadScriptFile`, `effSwordTrackReset` ->
`effMagaCylinderAllocPart`, `sdfMovieIsFinished` -> `sdfPacInit`), and one is
wrong: see `sdf/sdfMovie` below.

### A cross-check of the HD runs themselves

11 of the 303 merged HD source runs start on one of the 107 same-unit group edge
positions, against 3.3 expected by chance over 9,698 positions. Separately, 8 of
the 9 duplicate-literal windows that hold no subsegment edge contain an HD run
boundary (the ninth is newlib's `_dtoa_r`/`vfprintf` pair). The HD map's run
boundaries are therefore real file boundaries far more often than not, even
though the map is lossy and its file *names* are the remaster's.

## The six disputed cases

| Case | Resolution | What decided it |
|---|---|---|
| `field/fldFileResolver`, candidate `0x00125D40-0x0013C3D8`, 197 functions, 8 HD files | keep as applied, `0x00125D40-0x00128848` | HD's `fldFileResolver.c` run is `0x00125D40-0x00128840` and `fldDraw.c` starts at `0x00128848`, which the IDB confirms is a function start (`fldFileAddrResetMode` -> `fldDrawInit`). The candidate end was an outlier at the far side of seven other files. The applied narrowing is correct; the remaining 7 files are in `code_00128848` and are listed as additions below. |
| `mc/mcManager` versus HD `sdfMc.c`, `0x0029B0A8-0x0029B780` | keep the boundary and the name | The IDB shows 34 consecutive functions, all `sdfMc*`, bounded by `nmeUpdate` before and `slKillLoadProcess` after, with no strings of their own, so the extent is not in doubt. The name is: `mc/mcManager.c` is a path out of Nocturne's own Dec 2002 debug build, while `sdfMc.c` is the HD remaster's file. The HD symbol names in this span are themselves HD recovery output, so they cannot adjudicate the file name. Nocturne's own build wins. |
| `event/evtPolygonMovie` versus `event/evtPicture`, split point | keep as applied, split at `0x001F3A70` | HD gives `evtPolygonMovie.c` `0x001F0170-0x001F3998` and `evtPicture.c` `0x001F3A70-0x001F44B0`, and the IDB confirms `evtStartPolygonMovie2` -> `evtPicturePriClear` there. The candidate `event/evtPicture` start at `0x001F4340` sits inside HD's `evtPicture.c` run, so the two sources never actually disagreed: the DDS1 range was a lower bound. `0x001F0170` is also the one applied start with binary-only support. |
| `effect/effPCPThunder` versus HD `effPCPBezier.c` | keep as applied, `0x001762F8-0x0017AC10`; `code_0017AC10` is `effect/effPCPBezier` | HD's `effPCPThunder.c` run ends at `0x0017AA90` and `effPCPBezier.c` runs `0x0017AC10-0x0017CE28`; the IDB confirms `effPCP_ThunderPolyListDraw` -> `effPCPBezierBillCreate`. The candidate end `0x0017D0A0` is itself the first function of HD's `effPCPFlash.c` and the applied `effect/effPCPFlash` start, so that one DDS1 label was a mispair. |
| `script/scrCommonCommand` versus HD `scrParameterCommand.c` | keep as applied, `0x0010B858-0x0010CF38`; reject dds-decomp's widened end; `code_0010CF38` is `script/scrParameterCommand` | HD gives `scrCommonCommand.c` 67 functions ending `0x0010CF10` and `scrParameterCommand.c` 28 functions from `0x0010CF38` (`scrCommand_FILTER_ALLOFF` -> `prm_ITEM_PLUS` in the IDB), and the name prefix changes `scr` -> `prm` at the same address. The candidate end `0x0010D030` is inside the `prm_*` run, so the Persona 4 transfer that widened this unit for DDS does not transfer to Nocturne. |
| `effect/effEvent` versus HD `effDebug.c`, 34 `Eve_*` names | keep the start at `0x001AD230`, narrow the end to `0x001B0858` | The `Eve_*` names are not a conflict: HD attributes `Eve_BlurON` onward to `effEvent.c`, so both sources agree the file starts at `0x001AD230`, where the IDB shows `effDebugWindowDraw` -> `Eve_BlurON`. The candidate start `0x001AC0A8` is in Bezier and `effDebug*` code and was a bad DDS1 label; the applier was right to move it. The end is wrong, though: `0x001B0858-0x001B12D0` is one continuous `dds3*Spline*` run in the IDB, and HD names `SolveTridiagonal` at `0x001B0EF8` `effSpline.c`, so the unit currently swallows six spline functions. |

The four candidates with no HD name anywhere in their span stay single-witness:
`misc/l2d`, `basic/dds3ModelRangeBasic`, `sdf/sdfThread`, `sdf/sdfMovie`. The
first three were not applied, and the reasons are in the TSV's `action_note`.

## What to change in the applied set

| Unit | Change | Evidence |
|---|---|---|
| `file/fileManager` | move the end from `0x00297F68` to `0x00297928` | The current end cuts a proven same-unit group. HD's `file_Manager.c` run ends at `fileSysInit` (`0x00297890`, ending `0x00297928`) and the IDB's next function is `nmeCharToIndex`. The single HD `Nme_Init.c` hit at `0x00297F68` that the end was taken from is one named function inside the name-entry object, not its start. |
| `effect/effEvent` | narrow the end from `0x001B0EF8` to `0x001B0858` | The spline run above. |
| `sdf/sdfMovie` | move the start from `0x002C53C8` to `0x002C5080` | The `movie*` run is `0x002C5080-0x002C5F38` in the IDB (`sdfIpuClose` -> `movie_2C5080`), and the applied start is five functions into it. The candidate start was a lower bound and was used as if it were the boundary. |
| `basic/dds3CameraObjectBasic` | widen the start from `0x00112500` to `0x001123D0` | DDS1 labels `0x001123D0`, `0x00112410` and `0x001124F8` for this unit; HD names none of the three, so its run simply begins at the next named function, `MakeCamera`. With HD silent, the DDS1 label is the only witness and it says the file starts at or before `0x001123D0`. Lower confidence than the three above: the alternative is `0x00112370`. |

Nothing else in the applied set is contradicted by any source.

## Boundaries worth adding next

The code between the applied units is still in `code_<vram>` fillers, and
244 HD source-run boundaries fall inside them. Ranked by how many independent
sources back each one.

### Tier 1: rename only, no new cut

These filler subsegments already exist, already relink, and hold exactly one HD
source file. Only the name is missing. The name comes from the HD map except
where a dds-decomp unit name exists, which is noted.

| Subsegment | Proposed unit | Functions | Evidence |
|---|---|---|---|
| `code_00108AC8` `0x00108AC8-0x00109C58` | `kernel/dds3SequenceList` | 139, 129 HD-named | HD `dds3SequenceList.c` run, and a same-unit group starts at `0x00108AC8` (`CallDebug`..`ExitDebug`): two sources, the strongest unapplied boundary in the image |
| `code_0017AC10` `0x0017AC10-0x0017D0A0` | `effect/effPCPBezier` | 13 | HD run; resolves the Thunder/Bezier dispute; both edges are already applied boundaries |
| `code_002B2A60` `0x002B2A60-0x002B3B20` | `sdf/sdfMat` | 44, 43 HD-named | HD run fills the whole filler |
| `code_0016A018` `0x0016A018-0x0016A9C0` | `effect/effPacket` | 7 | HD run fills the filler, and the name prefix changes `dds3` -> `eff` at the same function |
| `code_001001D8` `0x001001D8-0x001005E8` | `kernel/dds3KernelMain` | 15, 14 HD-named | HD run fills the filler; the unit starts where crt0 ends, at `dds3FirstInit` |
| `code_0010CF38` `0x0010CF38-0x0010DCB8` | `script/scrParameterCommand` | 28 of 32 | HD run plus the `scr` -> `prm` prefix change; the last 4 functions are HD `dds3DbgBasic.c`, so a second cut at `0x0010D730` finishes it |
| `code_001AAE28` `0x001AAE28-0x001ABA68` | `newbattle/nbCameraHelper` | 16, 12 HD-named | HD run, but HD also has a second `nbCameraHelper.c` run at `0x001C55F8`, so one of the two is the map being lossy. Apply last of this tier. |

### Tier 2: new cut, two independent sources

An HD source-run boundary that also carries a binary-only row. `0x00108AC8` is
in tier 1 because its cut is already applied.

| Address | HD source | Run | Binary-only evidence |
|---|---|---|---|
| `0x0025E8E0` | `nbKoukaProcess.c` | 58 | two same-unit groups meet here: `0x0025D690-0x0025E870` ends and `0x0025E8E0-0x00261490` begins |
| `0x00261500` | `nbMakePacket.c` | 78 | after the 58-function group above |
| `0x001D5980` | `pbGame.c` | 62 | after the group `0x001D4D40-0x001D58E0`, and it is the one boundary that satisfies both open `DebugOwner` and `puzzleBoy-root` duplicate-literal windows |
| `0x0025D690` | `nbTarSelProcess.c` | 5 then 16 | a group starts here and the previous one ends |
| `0x00123C10` | `fldProcess.c` | 24 | a group starts here; satisfies the open `fldProcSequence` window |
| `0x0028C9F0` | `nbEncount.c` | 5 | group edge; satisfies the open `/sound/MIDI%04X.SMG` window |
| `0x0014F148` | `fldTitle.c` | 8 | a group starts here and a duplicate literal brackets it |
| `0x0024C470` | `nbStageProcess.c` | 16 | duplicate `BTL_DEF_LIGHT` |
| `0x001EB208` | `evtDebug.c` | 5 | duplicate `/event/e%03d/e%03d/scr/e%03d.bf` |
| `0x0022B6E0` | `mviManager.c` | 4 | after the group `0x0022B5E8-0x0022B6B8` |
| `0x002C39C8` | `sdfAddrResolve.c` | 1 | after the group `0x002C3730-0x002C3968` |
| `0x001CAD00` | `frTestFont.c` | 1 | duplicate `test_font` |

### Tier 3: new cut, HD alone, biggest first

One witness, but each converts a large run of filler into a named unit. The
whole list is reproducible with `tools/compare_boundaries.py`; these are the
runs over 40 functions.

| Address | HD source | Run | Filler |
|---|---|---|---|
| `0x0016FC90` | `effBattleSystem.c` | 255 | `code_0016F490` |
| `0x001BA828` | `nbCameraBoss.c` | 126 | `code_001B0EF8` |
| `0x00216618` | `fclEncyc.c` | 119 | `code_001FAE80` |
| `0x001518D0` | `slMain.c` | 102 | `code_00144798` |
| `0x0024F478` | `nbActionProcess.c` | 96 | `code_002460B0` |
| `0x00157F18` | `fldCommand.c` | 91 | `code_00144798` |
| `0x0026EFA0` | `NbNegoFlowTwo.c` | 76 | `code_002460B0` |
| `0x00287D78` | `nbAiScript.c` | 72 | `code_002460B0` |
| `0x00279690` | `nbPanelProcess.c` | 71 | `code_002460B0` |
| `0x002654B8` | `nbCalc.c` | 65 | `code_002460B0` |
| `0x00121530` | `fldMain.c` | 53 | `code_00116AD0` |
| `0x002853D8` | `nbAi.c` | 51 | `code_002460B0` |
| `0x00128848` | `fldDraw.c` | 51 | `code_00128848`, already an edge |
| `0x00118CA0` | `fldTest.c` | 50 | `code_00116AD0` |
| `0x002AE090` | `sdfPrim.c` | 49 | `code_002AC138` |
| `0x0026A8A0` | `nbNegoProcess.c` | 45 | `code_002460B0` |
| `0x00246998` | `nbMainProcess.c` | 45 | `code_002460B0` |
| `0x0029FBF8` | `pipeObject.c` | 42 | `code_0029B780` |
| `0x001B3590` | `nbCameraSkill.c` | 41 | `code_001B0EF8` |

Four of these recover units dds-decomp dropped as likely DDS mispairs:
`effBattleSystem.c` at `0x0016FC90`, `nbAiScript.c` at `0x00287D78`,
`nbMisc.c` at `0x00258048` and `nbSound.c` at `0x0028D090`. The name was always
Nocturne's; only the DDS projection was in doubt.

### Smaller additions worth recording

| Address | Unit | Evidence |
|---|---|---|
| `0x001AC508` | `effect/effDebug` | eight consecutive `effDebug*` functions in the IDB, three of them HD `effDebug.c`, ending where the applied `effect/effEvent` begins. Splits `code_001AC0A8`. |
| `0x001B0858` | `effect/effSpline` | the `dds3*Spline*` run; the other half of the `effect/effEvent` end fix |
| `0x00112A30` | `basic/dds3UnitObjectBasic` | already an edge (`code_00112A30`); HD places the file exactly there, and it is one of the DDS1 units the pairing could not reach, so the two sources agree it exists. A second cut at `0x00113300` gives `basic/dds3MatterObjectBasic`. |
| `0x00115D50` | `basic/dds3ModelBasic` | the other unreachable DDS1 unit, placed by HD |
| `0x00115F68` | `basic/dds3ModelRangeBasic` | one function; HD corrects the DDS1 start of `0x00115F28`, which is `dds3ModelBasicFreeModel` |
| `0x002A8598` | `sdf/sdfThread` | the thread block (`sdfSystemThreadProc` .. `sdfSetMainThreadPriority`) begins here, right after HD's `sdfSystem.c` run; the DDS1 label at `0x002A87F0` sits inside it |
| `0x00102FB8` | unknown | a same-unit group of `fclEncyc*` functions inside `kernel/dds3AdminiProcess`, where HD claims `dds3AdminiProcess.c`. One of the two is wrong; worth a look before trusting that unit's interior. |

## Limits

- Zero pairing contradictions is weak evidence, not strong. The monotone cut
  removes order violations before the ranges are formed, so only the 107 anchors
  it dropped could contradict anything, and the 7 of those with a usable unit
  name all land outside every span.
- A candidate range is a lower bound on the file and an upper bound on nothing.
  Two of the four applied-set errors above come from using a candidate `start`
  as if it were a boundary.
- Only 32% of functions paired. ee-gcc 2.96 output is stable, but SMT3 is a 2004
  build of a 2002 engine and DDS1 forked it, so most functions differ somewhere.
  The unpaired remainder carries no evidence either way.
- 24 SMT3 and 64 DDS1 functions are split into chunks in their IDBs and never
  pair. That is an IDB property, not a property of the binaries.
- The HD check is independent of the DDS1 pairing but is itself a recovery: its
  addresses are HD addresses, the join here is on function name, and 353 HD
  names are shared with another file. Shared names are dropped rather than
  guessed. Its file *names* are the remaster's, which is why `sdfMc.c` loses to
  `mc/mcManager.c`.
- The binary-only rules can refute a boundary and bracket one, never pin one.
  Expect them to stay mostly silent as units are added.
- dds-decomp's own pipeline projected the debug build's labels onto
  `SLUS_209.11` as an intermediate hop before reaching DDS. That table is not
  checked in and needs romwright plus the prototype ELF to rebuild. Having it
  would replace the DDS1 round trip here with the direct projection.

## Before a candidate becomes a subsegment

A reviewer must confirm, per range:

1. The range starts at a function whose predecessor belongs to something else,
   and the function after `end` does too, checked in the IDB rather than from
   the TSV. The TSV reports `end` as the last labelled function's start address,
   never a file boundary.
2. The start is not inside a same-unit group in `config/smt3/tu_evidence.tsv`.
   `tools/compare_boundaries.py` reports every subsegment edge that cuts one.
3. `.rodata`, `.lit4` and `.sdata` split along with it. dds-decomp's
   `tools/split_rodata.py`, `include_rodata.py` and `include_sdata.py` are
   vendored here for that, and a unit whose data cannot be split cleanly is not
   ready.
4. The unit relinks byte-identically. That is a necessary check and not an
   argument for the boundary: any cut relinks.
