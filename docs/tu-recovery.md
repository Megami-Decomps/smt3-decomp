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

The tier lists below were the plan. They have since been worked through: the
split now has 183 named `c` subsegments and 9 `code_<vram>` fillers, up from 74
and 24. The state of each filler, and why it is still one, is in "What is left"
at the end of this page. The tier lists are kept because the evidence rows in
them are what each cut was judged against.

### Tier 1: applied

All seven are in the split. `kernel/dds3KernelMain`, `kernel/dds3SequenceList`,
`script/scrParameterCommand`, `effect/effPacket`, `effect/effPCPBezier`,
`newbattle/nbCameraHelper` and `sdf/sdfMat` now carry their recovered names.

Three more boundaries that already existed as filler edges have been named the
same way, which needs no cut and no data split:

| Was | Now | Functions | Evidence |
|---|---|---|---|
| `code_0024C470` | `newbattle/nbStageProcess` | 71 | HD run plus the duplicate `BTL_DEF_LIGHT` literal |
| `code_0025D690` | `newbattle/nbTarSelProcess` | 2 | HD run; a same-unit group starts here and the previous one ends |
| `code_00128848` | `field/fldDraw` | 143 | HD run of 51 named functions, and the edge was already applied |

`tools/compare_boundaries.py` scores all three as `supported`.

### Applied: code_002AC138 split into its five files

The first container taken apart rather than renamed. Its 108 functions are
five HD files with nothing left over, so each cut's end is the next cut's start
and the last one ends where the container already did. All five relink
byte-identical and `check_unit` reports no `CONTEXT` line in any of them.

| Unit | Start | Functions | `compare_boundaries` | Evidence |
|---|---|---|---|---|
| `sdf/sdfGsMemIO` | `0x002AC138` | 6 | `name+start+end` | HD run; the six are the `sdfSetStoreImage`/`sdfLinkLoadImage` family |
| `sdf/sdfDelayedResrc` | `0x002AC760` | 7 | `name+start` | HD run of 6; the seventh, `sdfDelayedResrcCheckFunc`, is unnamed by HD but carries the family prefix |
| `sdf/sdfPacketBuff` | `0x002ACAB0` | 6 | `name+start+end` | HD run; all six are `sdfGr*PacketBuff*` |
| `sdf/sdfDraw` | `0x002ACB90` | 40 | `none` | HD's run starts at `0x002ACBF0`, two functions later; see below |
| `sdf/sdfPrim` | `0x002AE090` | 49 | `name+start+end` | HD run, and the IDB switches from `sdfGr*`/`sdfDraw*` to `sdfPrim*` here |

`sdfDraw` is the one cut placed off the HD run start, and it scores `none`
because the function it starts at has no HD witness at all. The HD run is a
lower bound: `sdfDrawTagInit` and `sdfDrawTagLink` sit in the two-function gap
above it, and HD names `sdfDrawTagLink2`, `sdfDrawTagLinkRef` and
`sdfDrawTagLinkCall` as `sdfDraw.c`. A file boundary between `sdfDrawTagLink`
and `sdfDrawTagLink2` is not credible, and no same-unit group or duplicate
literal falls anywhere in that range, so the binary permits either placement
and only the naming decides. This is the same correction already recorded for
`sdf/sdfMovie` under "What to change in the applied set".

79 of the 102 `c` subsegments are now named, leaving 23 `code_<vram>` fillers.

Two things this container established for the cuts that follow. A gap between
two HD runs is the boundary's uncertainty window, and every gap function here
carried its neighbours' prefix, so the convention of giving the gap to the
earlier file held each time. And `split_unit.py` carries named types but not
anonymous ones: both halves of the first cut lost the file's GS register enum
and failed to compile until each got back the one its code names.

### Tier 2: new cut, two independent sources

A cut is not a rename. These rows give a unit's **start**; the split gives the
tail everything up to the next existing boundary, so a cut applied on its own
labels whatever follows with the new name too. Each of these therefore needs its
*end* established as well, from the next HD source run, before the name is
honest -- and then `.rodata`, `.lit4` and `.sdata` have to split with it. None of
that is true of the tier 1 renames above, which is why those went in first.

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
| `0x002AE090` | `sdfPrim.c` | 49 | applied, with the rest of `code_002AC138` |
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

## What is left

183 of the 192 `c` subsegments are named. Every cut relinked byte-identically
and `check_unit` reports no `CONTEXT` line in any unit.

| Verdict against the binary-only rules | Units |
|---|---|
| `supported`, a rule the binary proves | 12 |
| `echo`, the HD map agreeing with itself | 102 |
| `silent`, no evidence row at the address | 69 |
| `contradicted` | 0 |

The twelve supported starts are the only ones the binary itself backs:
`kernel/dds3SequenceList`, `field/fldTitle` (the one `strong` row, carrying both
a core edge and a duplicate literal), `field/fldSnow`, `interface/frTestFont`,
`pb/pbMenu`, `pb/pbGame`, `event/evtPolygonMovie`, `newbattle/nbStageProcess`,
`newbattle/nbTarSelProcess`, `newbattle/nbKoukaProcess`,
`newbattle/nbMakePacket` and `sdf/sdfAddrResolve`.

### How the cuts were placed

An HD run start is a lower bound, not the boundary. Where the IDB's name family
changed before the run began, the cut moved up to the family's first function;
24 of the cuts are placed that way, and `field/fldSnow` is the case where the
binary's own core-edge rule independently landed on the address the names had
already chosen. Where a gap's functions carried the preceding file's prefix,
they stayed with it.

One cut was placed wrong and the tooling caught it. `pb/pbMenu` started inside
the 15-function group at `0x001D4D40-0x001D58E0`, which the binary proves is one
unit, so `compare_boundaries` scored it `contradicted`; moving the start to the
group's first function made it `supported`.

### What `split_unit.py` drops

It carries the declarations and named types a part uses. Five other things it
does not, each of which broke a build rather than the checksum:

- anonymous enums, which have no name to match on. Both halves of the first cut
  lost the file's GS register enum.
- a multi-declarator `extern` carrying a section attribute
  (`extern u16 A[], B[4] __attribute__((section(".data")));`).
- `__asm__("fptodp = sdfFloatToULong")`. Without it every float reaching a
  variadic call fails to link, and the error names `fptodp` rather than the
  file that lost the alias.
- a `PS2_SYMBOL_AT` binding, and the type it names.
- `.lit4` and `.sdata` ownership. The row stays with whichever part inherited
  the container name, and the linker then discards the section for every other
  part that emits one. Measure each object (`objdump -h`) and split the row in
  link order: the sizes must total the row's span exactly, which is what shows
  the division is the one retail had. This happened in six of the containers.

### The nine remaining fillers

| Filler | Functions | Why |
|---|---|---|
| `code_00116AD0` | 192 | being decompiled in parallel; its five files are listed below |
| `code_001FAE80` | 1398 | being decompiled in parallel; 35 HD runs cover 36 percent of it, with a 299-function tail |
| `code_002C7CB0` | 682 | 8 percent HD coverage, and windows of 71, 116 and 322 functions |
| `code_0029B780` | 26 | the head above `dds3ConfigMain`'s run, with no HD row of its own |
| `code_001AC0A8` | 16 | its one HD row names `effDebug.c`, which also appears at `0x00117CC8`; one of the two is a mis-attribution |
| `code_001C55F8` | 13 | HD says `nbCameraHelper.c`, but that unit is applied at `0x001AAE28` |
| `code_002C0BA8` | 3 | HD says `dds3KernelCore.c`, applied at `0x001015E8` |
| `code_00115D38` | 1 | HD says `dds3PathBasic.c`, applied at `0x00116058` |
| `code_002A99B0` | - | no HD row falls inside it at all |

The four small ones share one shape: HD attributes them to a file that is
already applied somewhere else, and a `.c` cannot hold two non-contiguous
ranges. Each needs that contradiction settled, not another cut. Two more names
collided the same way and were left unused inside containers that were
otherwise split: `nbCameraCommand.c` at `0x00175320` and, in `code_002C7CB0`,
`pbMCard.c` and `tgsMain.c`, the latter claimed twice over.

One case points the other way and is worth checking: `nbLoadStageSky`, the last
function of `newbattle/nbResultProcess`, carries the prefix of
`newbattle/nbStageProcess`, which begins immediately after it.

### `code_00116AD0` is five original files, not one

The HD reconstruction splits this unit's 80 remaining functions cleanly by
prefix with no interleaving, so naming the file after any one of them would be
wrong for most of it:

| Original file | Functions | Prefixes |
|---|---|---|
| `fldTest.c` | 26 | `fldRun*`, `fldSdw*`, `fldElv*`, `FroFtoI`, `ooyTestDrawProc` |
| `fldMain.c` | 17 | `fldSet*`, `fldAnahori*`, `fldWave*`, `fldLightMa*`, `fldOtInit`, `fldReadyScene` |
| `fldProcess.c` | 10 | `Proc*`, `fldProc*` |
| `fldGlobal.c` | 6 or more | `fldGb*`, the flag accessors, `fldAllTblLoad` |
| `dds3GlobalWork.c` | 4 | `dds3GlobalWork*`, `NewGame2*` |

Address order suggests the boundaries run `fldTest.c`, `fldGlobal.c`,
`fldMain.c`, `fldProcess.c`, with a `dds3GlobalWork.c` tail. This is
`tools/find_tus.py` boundary work rather than a rename, and no reconstruction
header declares the membership, so the per-file prototype blocks are the only
oracle. Prefix agreement is weaker evidence than the two-source cuts above:
one engine module's functions can be spread across several files.

## The five fillers that are left

Four of the ten were named from the HD map, and `code_002C7CB0` is gone: a
parallel pass split its Sony SDK region into one unit per archive
(`docs/sdk-matching.md`), and its first 10 functions are the `sdfSortOt` family
HD names, so they became `sdf/sdfSortOt`. The table in "The nine remaining
fillers" above is superseded by this one.

| Filler | Functions | Why it has no name |
|---|---|---|
| `code_0022C578` | 421 | The camp module, `cmp*` 275 and `camp*` 106. No source names it: the HD map has only the `fclPanel` mis-attribution, and no DDS1 candidate range covers it. |
| `code_0029B780` | 25 | 21 of 25 are `dds3*` and the last is `dds3AllocConfigWork`, immediately before `kernel/dds3ConfigMain`. It looks like that unit's head, which would be a boundary move rather than a name, and no HD row falls inside it. |
| `code_001C55F8` | 13 | All 13 are named `nbCameraHelper*` and HD agrees, but `newbattle/nbCameraHelper` is applied at `0x001AAE28` and a `.c` cannot hold two ranges. HD's `nbCameraHelper.c` has 30 functions spanning both, among them the `dds3EffectFileListWindow*` family that fills the applied unit. The name fits this range better than the one that holds it, so settling it means renaming an applied unit, on evidence this does not supply. |
| `code_002C0BA8` | 3 | `dds3GetRandA`, `dds3GetRandFloatA`, `dds3GetRandIntA`, immediately before `sdf/sdfRand`. HD puts them in `dds3KernelCore.c`, which is applied at `0x001015E8` and 1.7 MB away, so that is a mis-attribution of the SDK kind. With HD discredited here, nothing else names them; merging them into `sdfRand` would cross a `dds3`/`sdf` prefix change. |
| `code_00115D38` | 1 | `dds3PathBasicFreeControl`. HD says `dds3PathBasic.c`, applied at `0x00116058`, and `basic/dds3ModelBasic` sits between the two, so one of the three placements is wrong. One function is not enough to say which. |

The shape worth noticing is that four of the six are the same shape: HD names a
file that is already applied elsewhere. Two of those, `dds3KernelCore` here and
the six in `code_002C7CB0`, are settled as il2cpp naming a caller's file for a
library function. The other two, `nbCameraHelper` and `dds3PathBasic`, would
need an applied unit renamed, which is a separate piece of evidence work.

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

## The remaining homes, and what blocks them

288 HD source files have code in this ELF. 175 already carry their name as a
unit and 5 more are the same file spelled differently, which the normalising
comparison had to learn: HD's `file_Manager` is `file/fileManager`, `sdf_PacDecode`
is `sdf/sdfPacDecode`, `ScrTraceCode` is `script/scrTraceCode`, and so on. That
leaves **108 HD files with no unit of their own**.

Two of those are not missing homes either, by this page's own rule. HD's
`EventPack` and `sdfMc` are the only HD files in `event/evtEventPack` and
`mc/mcManager` respectively, so each is Nocturne's own name for the HD file
rather than a second file hiding inside one; `sdfMc` losing to `mc/mcManager`
is the precedent set above.

The rest sit in six containers:

| Host | Homes | `NON_MATCHING` drafts | State |
|---|---|---|---|
| `newbattle/nbMakePacket` | 47 | 48 | done |
| `field/fldDraw` | 9 | 12 | done |
| `code_00116AD0` | 8 | 6 | done |
| `newbattle/nbStageProcess` | 4 | - | done |
| `script/scrParameterCommand` | 1 | - | done |
| `code_001FAE80` | 35 | 43 | done |
| `code_002C7CB0` | 2 | 0 | not homes: SDK mis-attributions, see below |

All 108 are resolved: 104 placed, and 4 shown not to be homes at all, the two
Sony SDK pairs below plus `EventPack` and `sdfMc`. Each container relinked
byte-identically and `check_unit` reports no `CONTEXT` line in any part.

A named unit can hide a container. `newbattle/nbMakePacket` holds 48 HD files
in strictly ascending order, among them `nbE901` through `nbE915` and sixteen
`NbNegoFlow*` files, and keeps only 77 functions of its own. Nothing flagged it,
because the audit above only ever looked at the `code_<vram>` fillers. The runs
are the cleanest in the tree: 0 to 4 unnamed functions between consecutive runs.

### The tool was taught instead

All five things "What `split_unit.py` drops" lists are now carried, and each was
found by a sweep failing to build rather than by reading the list:

- **A conditional region is one block.** `cparse.blocks` emitted every top-level
  `#` line separately, so a `NON_MATCHING` draft became three items and the
  declaration-copying rule duplicated the `#else` into every part: 16 of 48
  parts came out with bare `#else` lines and no `#ifdef`. A
  `#if`/`#else`/`#endif` region now moves whole, addressed by the function
  inside it, comment included.
- **An attributed or aliased declarator.** `declared_name` split at the first
  `(`, so `extern s32 fldGb __attribute__((section(".data")));` declared
  `__attribute__` and `extern T *cmbGlobalWork asm("DDS3_GBWK");` declared
  `asm`. Both suffixes are stripped with matched parens now.
- **An anonymous enum**, which introduces no name at all. `declared_names`
  returns an enum's member names, so the part using `FIELD_ROOM_PLANE_COUNT`
  gets the definition.
- **A multi-declarator extern**, found by any declarator rather than only the
  last, from the same `declared_names`.
- **A bare `__asm__("x = y")` alias**, which declares nothing and now reaches
  every part like an orphan type.

`.lit4`/`.sdata` ownership remains the one item on that list the tool does not
compute; `split_rodata.py` does, and the measurement below is how to check it.

### `code_001FAE80`, and the three things its last 40 bytes taught

Its 34 cuts apply and every part compiles. Two things were settled along the
way and one is left.

**The `fclPanel` contradiction is settled.** HD names `fclPanel.c` twice, at
`0x2155B8` and again for one function at `0x234010`. The second is the
mis-attribution: `0x234010` sits inside a 420-function run of `cmp*` and `camp*`
names, the camp UI, nowhere near `fclPanel.c`'s own run.

**The container does not end with a named run.** `MovieViewer`'s HD run stops at
`0x22C048`, but `movieViewerSetFlags` through `movieViewerDebugDraw` follow it
and carry the prefix, so the file runs to `0x22C578` where `rst*` and then the
`cmp*` block begin. Everything from there to the container end has no HD
attribution at all, so it is cut off as its own filler rather than left to
inflate `MovieViewer`. A sweep must not assume the last run reaches the
container's end.

**A 16-byte alignment rule decides which object owns a string.** The tail
emitted 32 bytes of `.rodata` too many. `/camp/tmx/camp_01.tmx` at `0x0053F478`
is not 16-aligned, and the tail has a jump table, so its object begins
16-aligned and cannot start there: by the rule `split_rodata.py` already
applies, that string is the previous object's last symbol and the tail
references it as an extern. Emitting it as a C literal instead cost 24 bytes
for the string and 8 more for the padding gcc then inserted to realign the
jump table. The fix is in the C, not the yaml; moving the row instead gets the
size right and the contents wrong.

**A declaration has to precede its use, and a stale object hides that.** The
first attempt put that extern after the function using it, and the build kept
passing on a cached `.o` while the real compile had failed. `rm` the object and
build it alone when a diagnosis stops making sense.

**The last two bytes were a callee-visibility miss, and it moved a boundary.**
`lstPageUp` and `lstPageDown` each emitted one `bnel` where retail has `bne`.
`docs/levers/callee-visibility.md` names the cause: the branch depends on
whether the function the arm calls is a C definition in the same unit. They
call `lstMoveCursor` at `0x00213518`, which is one of the 353 HD names shared
between two files (`lstListWindow.c` and `fclShare.c`) and so dropped from the
map, which left it in `fclDraw`. The IDB settles it: `lstInitListWindow`,
`lstMoveCursor`, `lstMoveCursor2`, `lstDrawUpDownCursor`, `lstDrawListWindow`,
`lstDrawListWindowCursor`, `lstPageUp`, `lstPageDown` are one `lst*` family
from `0x002133F8` to `0x002138C0`, so `lstListWindow.c` starts at `0x002133F8`
and HD's `0x002136A8` was the lower bound this page keeps warning about. That
page's closing note holds: a callee-visibility result is a boundary hint.

### `code_002C7CB0` holds no homes at all

Its six HD attributions were read as contradictions, `pbMCard` and `tgsMain`
each claimed twice and `pbMenu` already applied at `0x001D4D40`. Resolving what
is actually at those addresses settles all of them at once:

| HD file | Address | The function there |
|---|---|---|
| `pbMCard.c` | `0x002CDC90` | `sceMcInit` |
| `fclShare.c` | `0x002D43A4` | `memcpy` |
| `tgsMain.c` | `0x002DFD70` | `sceOpen` |
| `pbMenu.c` | `0x002E0FD8` | `sceMkdir` |
| `tgsMain.c` | `0x002E1ED0` | `sceSync` |
| `pbMCard.c` | `0x002E3A60` | `sceSifLoadModule` |

Every one is Sony SDK or libc. The container's name families say the same from
the other side: of 679 functions it is `sce*`, `__ieee754_*`, `__kernel_*`,
`libc*` and `RFU*` almost throughout. These are HD mis-attributions of the kind
il2cpp metadata produces, naming the caller's file for a library function, and
CLAUDE.md's rule applies: the Sony SDK libraries are prebuilt archives and stay
assembly. `pbMCard`, `pbMenu`, `tgsMain` and `fclShare` keep the homes they
already have, and this container is not a container.

### Two more that are not homes either

Its seven runs name `pbMCard` and `tgsMain` twice each at non-contiguous
addresses, and `pbMenu`, which is applied at `0x001D4D40`. A `.c` cannot hold
two non-contiguous ranges, so this is the contradiction shape already described
for the four small fillers: it needs settling rather than cutting.

### What a clean sweep costs

Two things that are not obvious from the tool's own docstring, both found by
doing it:

- **Each cut needs its end, and the tail can expose an older error.** Cutting
  `script/scrParameterCommand` at `dds3DbgBasic.c`'s run gave the new unit a
  fourth function, `dds3AllocBasic`, which calls `dds3DeleteBasic`, the first
  function of `basic/dds3Basic`: one file split across two units. Moving that
  unit's start back absorbed it, so the cut left no new filler.
- **`split_rodata.py --write` regenerates a whole section.** On a tree whose
  rows have been hand-corrected it replaces them with its own inference; doing
  that for one cut rewrote rows in `kernel/`, `field/` and `effect/` and came
  out 8 bytes short. Compute with it, then splice only the rows falling inside
  the container's own span.
- **Measure the sections when the image changes length.** For each row, compare
  its span with what the owning object emits (`objdump -h`). A part emitting
  more than its span is the whole story: that is how `MovieViewer`'s 32 bytes
  were found, after `.lit4` and `.sdata` both came back with zero overflow.


### Effect resource directory providers belong with the browser

The `newbattle/nbCameraHelper` lower bound at `0x001AAE28` separated
`dds3EffectFileListCreate` from its immediately preceding directory providers:
`dds3EffectDirOpen` (`0x001AAD00`, 76 bytes), `dds3EffectDirClose`
(`0x001AAD50`, 44 bytes), and `dds3EffectDirRead` (`0x001AAD80`, 164 bytes).
Move the real definitions, in native order, into the browser owner and start
its text at `0x001AAD00`. This does not rename the still-provisional owner.

Three pieces of evidence agree:

- The native directory-provider triplet directly precedes the browser that
  consumes it, with only normal function alignment between them. The
  corresponding DDS1 source (`game/code_0018CAC8.c`, from dds-decomp/redthing1)
  retains precisely this provider/caller order in one translation unit.
- `dds3EffectDirOpen` owns `"pfs0:%s"` at `0x0054F260`, the final eight
  bytes of the old owner's small-data range. It immediately precedes the
  browser's `0x0054F268` string. Moving the matching small-data boundary
  from `0x0054F268` to `0x0054F260` preserves the native text/data order.
- The unchanged 1,080-byte `dds3EffectFileListCreate` draft differed only at
  `+0x3F8`: `bgtzl` instead of retail's `bgtz`, with the same flags load in
  the delay slot. Making the authentic preceding `dds3EffectDirClose`
  definition visible closes this one word. The visibility probe is evidence
  for this independent ownership correction, not permission to use a stub
  or a `nothrow` annotation.

Ordinary `-da` dumps were qualified against the plain compiler output (only
cc.sh's randomized `.file` path differs). In both visibility cases, pass
`28.mach` retains loop branch uid179 targeting label182, probability6001,
followed by the close call uid934. Pass `29.dbr` uses flags-load uid187 as
the same donor; only the opaque case marks branch179 `/u` (annulled). The
visible close call carries `REG_EH_REGION 0`. No source-level loop change,
call hint, compiler option or provider duplication is needed.
