# The Japanese debug build as a TU boundary source

An assessment of `SLPM_652.42`, the Dec 26 2002 Japanese debug build, as evidence for
translation unit recovery in `SLUS_209.11`.

Every number here was measured on this ELF unless it is marked DDS, which means it
comes from `docs/tu-recovery.md` quoting dds-decomp's work on DDS1/2 rather than from
Nocturne. Both sides' function bounds come from their IDA databases, addressed by
module name (`SLUS_209.11` and `SLPM_652.42`); section headers and bytes come from the
two ELFs on disk. Neither database was written to.

Scripts and derived data are in the session scratchpad at
`/tmp/claude-1000/-home-wsl-projects-smt3-decomp/0ce7bc11-57f8-40e0-9fcc-1bf574b87d94/scratchpad`.
They are prototypes, read-only with respect to both builds, and nothing in the repo
was changed except this file.

## 1. What the build is

| | debug | retail |
|---|---|---|
| file | `SLPM_652.42` | `SLUS_209.11` |
| size | 4,568,088 | 4,551,696 |
| sha1 | `69d03ff738ef1f42322a2b34ca4c968b22f0bba4` | `5facfbf7916836613449c922e26a4ec412207b32` |
| entry | 0x00100008 | 0x00100008 |
| sections | 28 | 28 |
| `.text` size | 0x1E8AFC | 0x1E6E44 |
| `.rodata` delta | +30,272 bytes | |
| date string | `Dec 26 2002` / `15:24:27` | `Sep  3 2004` |

One ELF, no overlay code. The 28 sections have the same names in the same order as
retail, including the same eight `.DVP.overlay*` sections, which in both builds are
tables rather than alternate code. `.text` is 4,280 bytes larger, +0.21%.

It is fully stripped. `.mdebug.eabi64` exists but is zero bytes. There is no
`.symtab`, no `.strtab`, no `.debug_*`, no `.stab`. IDA's auto-analysis named exactly
one of 8,813 recognised functions (`start`). Retail is stripped the same way, so on
the symbol axis the debug build adds nothing: the project's names still have to come
from the IDB's HD recovery.

What it does add is `__FILE__` strings and assert text, which retail compiled out.
Retail contains zero `.c` paths and zero assert format strings. 13,047 string
literals are shared between the two builds.

## 2. The strings, by class and by use

| class | count | what it is good for |
|---|---|---|
| `__FILE__` paths | 172 distinct | high: names a TU and, through its references, locates it |
| assert expressions | 654 strings, 544 distinct identifiers | medium: field, global and constant names, some array bounds |
| line numbers | present, paired with every path | low here: no source to check against |
| `__DATE__` / `__TIME__` | 1 pair | identity only |
| `__func__` literals | 0 | none |
| printf trace strings | present but not separable from retail's 13,047 shared strings | low |

The 172 distinct paths match the count `docs/tu-recovery.md` cites from dds-decomp
exactly. The reference count does not: 1,498 xrefs here against 1,478 DDS. The 20
extra are consistent with Nocturne having slightly more assert sites than the DDS1/2
builds dds-decomp measured, and with the +4,280 bytes of `.text`. Treat 172 as
confirmed for Nocturne and 1,478 as a DDS figure that does not apply here.

Three path shapes:

| shape | count | example |
|---|---|---|
| `../<dir>/src/<file>.c` | 144 | `../kernel/src/dds3KernelInit.c` |
| `../<dir>/<file>.c` | 2 | `../file/fileManager.c`, `../mc/mcManager.c` |
| bare `<file>.c` | 26 | `sdfMc.c` |

This settles the yaml unit naming convention. The `<dir>/<file>` form used in
`config/smt3/SLUS_209.11.yaml` is the original source tree layout, confirmed for 146
of the 172. The 26 bare names are all `sdf*.c`: the sdf engine was built from a
directory the compiler saw as `.`, so those units have no directory evidence at all
and the `sdf/` prefix the yaml gives them is a project convention, not a recovered
fact. All 11 directory mismatches between the yaml and the paths are this sdf case,
except `newbattle/nbCameraHelper`, which the debug build spells
`../effect/src/nbCameraHelper.c`, with `../effect/src/nbCameraBoss.c` beside it.

Of the 69 applied unit names, 67 appear in the 172 as real Nocturne source files.
`kernel/dds3KernelMain` and `script/scrParameterCommand` do not, which means they were
either inlined away, assert-free, or wrongly named. 105 of the 172 paths are not yet
used as unit names.

Assert strings come in two formats, `assertion fail : %s:%d:[%s]` and
`ASSERT: %s:%d:%s`, both file, line, expression. The 544 distinct identifiers in the
expressions name struct fields, globals and constant families; 36 already match a
retail symbol. Some carry hard numbers, for example `header->LocalIntCount < 256` and
`header->LocalFloatCount < 128`. This is type and bounds evidence, not boundary
evidence.

## 3. How the paths attribute code

1,073 distinct debug functions reference at least one path. No function references
two different paths, which is what makes the references usable: a referencing function
is unambiguously inside that file.

Reference density per file:

| references | files | share |
|---|---|---|
| >= 1 | 172 | 100% |
| >= 2 | 124 | 72% |
| >= 3 | 100 | 58% |
| >= 5 | 70 | 41% |
| >= 10 | 31 | 18% |
| >= 20 | 8 | 5% |

Median 4, max 86. So the paths attribute 1,073 of 8,813 debug functions, 12%. They do
not attribute whole files: for the median file, 4 scattered functions out of a run
that may be 20 or more.

File ordering in `.text` is clean. 170 of 171 consecutive file spans are disjoint,
meaning the lowest reference of file N+1 lies above the highest reference of file N.
The single exception is `itfMesManager.c`, and it is caused by one outlier reference
at 0x002160B8 out of 28. This is the key structural fact: the compiler emitted the
TUs in link order and the references sample that order faithfully, so the gaps between
consecutive files bracket the boundaries. 11 of the 170 debug-space gaps have zero
width, meaning the boundary is pinned exactly in debug space; the median gap is 1,686
bytes.

`.rodata` is partitioned in the same order. Of 14,706 file pairs, 12 are inverted
(0.08%) against 7,353 expected if the orders were independent; the longest increasing
subsequence covers 171 of 172 files and 159 sit at identical rank. So a future
`.rodata` split can reuse the same TU order.

## 4. Projection to retail: measured

### 4.1 Function bounds

Both sides use exact IDA bounds, taken with `tools/pair_dds1.py`'s own rule of
skipping functions split into several chunks, since a split function has no single
extent to hash.

| | debug | retail |
|---|---|---|
| functions IDA recognises | 8,813 | 9,571 |
| single-chunk | 8,735 | 9,547 |
| split, skipped | 78 | 24 |
| single-chunk inside `.text` | 8,735 | 9,547 |

An earlier pass, run while both databases were down, recovered debug bounds by
scanning for `jr $ra` plus delay slot and unioning the following positions with all
`jal` targets. The exact bounds show that scanner was weaker than its own validation
suggested, and the error was not where it was expected to be:

| subset | scanner exact | 
|---|---|
| all 8,735 IDA functions | 78.9% |
| the 1,066 attributed, assert-carrying functions | 90.9% |
| the other 7,669 | 77.3% |

The 90.9% figure reported earlier was measured only against the attributed functions,
because those were the only bounds IDA had produced before the databases died. They
are the easiest subset, not the hardest: assert code makes a function longer and gives
it extra `jal` edges, so its extent is more clearly marked. The true scanner accuracy
is 78.9%, so every figure derived from it was a looser floor than stated. Start
addresses alone were 92.6% correct, and that was near-identical across the two subsets
(93.0% against 92.6%), which is why the bracketing survived the bad bounds.

### 4.2 The hash pairing rate

Using the `tools/pair_dds1.py` scheme unchanged (sha1 over instruction words with
`j`/`jal` targets masked to the opcode and immediate-form opcodes masked to the upper
halfword, paired on size and hash with a 16-byte floor, unique hashes as anchors, a
longest-increasing-run monotone cut, then ambiguity resolution inside anchor gaps):

| | debug to retail | scanner floor | DDS1 baseline (DDS) |
|---|---|---|---|
| functions, debug | 8,735 | 9,042 | |
| functions, retail | 9,547 | 9,447 | |
| unique-hash anchors | 4,581 | 3,841 | 2,508 |
| anchors after monotone cut | 4,579 | 3,840 | 2,401 |
| anchors dropped | 2 (0.04%) | 1 | 107 (4.3%) |
| one-to-one pairs | 5,889 | 4,723 | 3,040 |
| ambiguous | 627 | 831 | 1,211 |
| share of debug functions paired | 67.4% | 52.2% | |
| share of retail functions paired | 61.7% | 50.0% | 32% |

Size-equality hashing does not break. It works far better here than on the DDS1 pair,
61.7% of retail against 32%, and the monotone cut discards 2 anchors out of 4,581
against DDS1's 107 out of 2,508, so the two builds agree about code order almost
exactly. Code ordering matches retail. None of the fallbacks (call-graph structure
matching, string-reference matching, relative ordering plus data-reference
fingerprints) are needed.

### 4.3 The confound, and the method that works

Naive projection, following a path's referencing functions through the hash pairs to
their retail twins, recovers **0 of 1,066**. With exact bounds the effect is total,
not merely strong:

| subset | paired |
|---|---|
| attributed, assert-carrying | 0 of 1,066 (0.0%) |
| everything else | 5,889 of 7,669 (76.8%) |

This is structural rather than a measurement artifact. A function is attributed
precisely because it contains assert code that retail compiled out, so its instruction
stream cannot match. The earlier 0.1% against 58.7% was scanner noise in both
directions; the real figure is zero. Label-following projection is not unreliable
here, it is impossible, and no amount of tuning recovers it.

The fix is to stop following labels and interpolate positions through the monotone
anchors instead. Each file's debug-space gap is mapped to a retail interval by the
nearest anchors on either side, which brackets the boundary without needing the
attributed functions themselves to pair.

| | exact | scanner floor |
|---|---|---|
| files bracketed | 171 of 171 | 171 of 171 |
| median bracket width | 18 retail functions, 3,336 bytes | 20, 3,448 |
| brackets <= 3 functions | 4 (2%) | |
| brackets <= 5 functions | 16 (9%) | 13 (8%) |
| brackets <= 10 functions | 50 (29%) | 40 (23%) |
| brackets <= 20 functions | 96 (56%) | |

A bracket runs from the last anchor below the file's highest reference to the first
anchor at or above the next file's lowest reference. That upper anchor can itself be
the first function of the next file, so the bracket is inclusive of its upper endpoint.

A bracket alone is not a boundary. It becomes one when a second source puts a
candidate inside it.

### 4.4 Intersecting with the HD map

Joining the 11,595-function HD map by name to retail addresses reproduces 303 HD
source-run boundaries, as `docs/tu-recovery.md` reports.

| | observed | expected by chance |
|---|---|---|
| brackets holding >= 1 HD boundary | 155 of 171 | 84.4 (sd 5.2) |
| brackets holding exactly 1 HD boundary | 97 | |
| brackets holding exactly 1 whose HD name matches the next file | 138 | 1.03 (sd 1.00) |
| brackets holding none | 16 | |

The chance baselines were computed by Monte Carlo, 2,000 trials each: the first by
scattering 303 boundaries uniformly over the 9,547 retail functions keeping the
bracket sizes, the second by shuffling the HD file names across the 303 boundaries.
155 against 84.4 says the brackets are real; 138 against 1.03 says the name agreement
between a Japanese 2002 debug build and a reconstructed remaster source map is not a
coincidence. These 138 are two-witness boundaries. The brackets cover 6,356 of the
9,547 retail functions, 66.6%, which is what the first baseline controls for.

Note what each witness contributes. The address comes from the HD map. The debug build
contributes the bracket that admits it, the file's own Nocturne name, the file order,
and the file end. For the 16 brackets with no HD boundary, the debug build is the only
witness and the boundary stays a bracket.

### 4.5 Against the applied config

Of the 69 applied named starts, 59 reproduce as pinned boundaries with 0 name
conflicts. 10 do not pin, and 69 of the 138 pins are cuts the config does not have.

The 10 that do not pin are `kernel/dds3KernelMain`, `kernel/dds3KernelCore`,
`script/scrScriptProcess`, `script/scrParameterCommand`, `basic/dds3CameraObjectBasic`,
`effect/effMagatuhi`, `newbattle/nbCameraHelper`, `mc/mcManager`, `sdf/sdfMovie` and
`sdf/sdfPacDecode`. Not pinning is not disagreement. 6 of the 10 sit inside their own
start bracket and simply have no single name-matching HD boundary to pin against,
which is expected for the units the HD map never named. The other 4 have no bracket at
all: `kernel/dds3KernelCore` is the first file in `.text` so has nothing below it,
`kernel/dds3KernelMain` and `script/scrParameterCommand` are not among the 172 paths,
and `newbattle/nbCameraHelper` is keyed under `effect/` by its path string. Not one of
the 10 is contradicted by a bracket.

This is a weaker check than it looks: 56 of the 60 applied units took their address
from the HD map in the first place, so the address axis is partly circular. The
non-circular part is that the debug build independently brackets those addresses and
agrees, and that it supplies the name.

## 5. What this settles

Resolved:

- `mc/mcManager` versus HD `sdfMc.c`. `../mc/mcManager.c` is a real Nocturne source
  file. `sdfMc.c` also exists as a separate bare path. The name dispute goes to
  `mc/mcManager`; HD renamed it.
- All four single-witness units are real files: `misc/l2d`, `basic/dds3ModelRangeBasic`,
  `sdf/sdfThread`, `sdf/sdfMovie`.
- `sdf/sdfMovie` start. The config already carries 0x002C5080, the value
  `docs/tu-recovery.md` proposed. Its start bracket is [0x002C2B38, 0x002C5170], which
  admits 0x002C5080 and excludes the superseded 0x002C53C8. A single-witness start
  becomes a two-witness one.
- `basic/dds3CameraObjectBasic` start, settled at the applied 0x00112370. Its start
  bracket is [0x00112280, 0x001123D0], 4 retail functions wide, which excludes the
  superseded 0x00112500 and leaves 0x00112370 and 0x001123D0 as the candidates. The
  code picks between them: `func_00112370` calls `dds3InitPoseSet`, allocates 0x90
  bytes and stores the result at `basic->workdata`, and 0x90 is exactly the unit's
  `dds3CameraObjectBasicWorkData_t` (a 64-byte matrix, four 16-byte vectors, a pointer
  and three words). `func_001123D0` is its destructor, calling `dds3FreePoseSet` and
  freeing the same allocation, so it cannot be the first function of the file. The
  function before, `dds3ObjectBaseSetControlBasicSvs`, ends exactly at 0x00112370.
- `evtPolygonMovie` to `evtPicture` pins at exactly the doc's 0x001F3A70.
- `effPCPThunder` to `effPCPBezier` pins at 0x0017AC10; `effPCPBezier` to
  `effPCPFlash` at 0x0017D0A0.
- `newdata/datCalc` pins at 0x00242A88, matching the doc's HD-only placement.

Open, and reported rather than resolved:

- `basic/dds3ModelRangeBasic`, resolved by counting functions between two anchors
  instead of by the bracket. The start is 0x00115EE8, two functions below the
  0x00115F68 `docs/tu-recovery.md` proposed, which the bracket already excluded. See
  below.
- `fldDraw.c`, `scrParameterCommand.c` and `effDebug.c` are absent from the 172, so
  the debug build cannot speak to any cut that rests on them.
- 7 of the 172 are named nowhere in the HD map: `kernel/dds3TexViewer`, `misc/l2d`,
  `effect/parTool`, `event/evtEventPack`, `facility/fclCombineCalc`, `mc/mcManager`,
  `sdf/sdfMovie`.

## 6. IDB improvement, separate from TU boundaries

Each of these needs the proof CLAUDE.md requires, which is retail code that forces the
conclusion, not a debug-build string that suggests it. Listed in descending order of
how close the evidence already is.

1. **Array bounds in assert expressions.** `header->LocalIntCount < 256` and
   `header->LocalFloatCount < 128` give struct array sizes directly. Proof needed:
   the retail code at the corresponding site indexes the member with that stride or
   bound. Several of the 544 identifiers are of this kind.
2. **Field names on structs the IDB has typed but not named.** The expressions name
   fields through `->`. The hash pairs cannot be used to find the retail twin of an
   assert site, since attributed functions pair at 0%; locate it through the bracket
   and the surrounding call graph instead. Proof needed: the offset the retail code
   loads matches the field position implied by the expression.
3. **Globals.** 36 of the 544 identifiers already match an existing retail symbol,
   which is a cross-check on those names rather than new information. The remainder
   are candidates that need an allocation site, an init function or a caller in retail
   before they can be applied.
4. **Function names.** Nothing. There are no `__func__` literals and no `.h` paths, so
   no function name string of any kind survives.

## 7. Plan, ranked by value to effort

Nothing here is a correction to an applied boundary. The two changes
`docs/tu-recovery.md` proposed, `sdf/sdfMovie` to 0x002C5080 and
`basic/dds3CameraObjectBasic` to 0x00112370, are already in the config; the debug
build confirms both and upgrades them from one witness to two. Everything below is
new ground.

1. **Add the 69 new two-witness cuts.** These come with a Nocturne file name, so they
   both split units and name them, which is the single largest increase in named
   coverage available. Cost: a day, since each cut needs a build to confirm the image
   still hashes to `5facfbf7916836613449c922e26a4ec412207b32`. Risk: low per cut,
   moderate in aggregate, so stage it in groups and build between groups.
2. **Split the span the yaml calls `newbattle/nbCameraHelper`.** The directory is
   wrong, but renaming it to `effect/nbCameraHelper` would be wrong twice over: the
   span 0x001AAE28 to 0x001B0858 is 22KB holding at least eight unrelated files, from
   `dds3EffectFileList*` at the start through `effDebug*`, the `Eve_*` and blur entry
   points, `effEveBED*`, `effEVECombParSmoke*` and `effAPPoint4*`, and no
   `nbCameraHelper*` function is in it at all. Those sit in the next unit, around
   `nbCameraClearState` at 0x001B1490. The HD map put the name on the wrong chunk, so
   this span is a target for the two-witness cuts above, not a rename. Until it is
   cut, the name is a label for the chunk and nothing more.
3. **Apply the method of section 9 to the other 15 single-witness brackets.** The
   `dds3ModelRangeBasic` case shows that a bracket with an anchor just inside it
   settles to an exact address, which the bracket alone does not.

Then, in order: work the 16 single-witness brackets down by narrowing them with data
references; use the `.rodata` ordering result when `.rodata` is split;
mine the assert expressions for bounds and field names under the proof rule in
section 6.

Nothing above requires the retail IDB to be written to, except any name or type
applied from section 6. Those follow the documented
procedure: correct in the IDB, re-run `tools/ida_symbols.py`, verify the image still
hashes to `5facfbf7916836613449c922e26a4ec412207b32`.

## 8. Caveats

- MCP ports are assigned in open order and move between sessions, so the debug IDB
  has no fixed number. Address both databases by module name, `SLUS_209.11` and
  `SLPM_652.42`.
- Function bounds on both sides are now exact IDA bounds, so the pairing rates are
  measurements and not floors. The 78 debug and 24 retail functions IDA splits into
  several chunks are excluded throughout, since a split function has no single extent
  to hash, and a boundary falling inside one of them would not be bracketed.
- The pinned boundary addresses come from the HD map. The debug build's independent
  contributions are the bracket, the file name, the file order and the file end. No
  pinned address is independent evidence for itself.
- The 138 two-witness pins are consistent, not proven. Consistency with a bracket and
  a name is weaker than a relinked byte-identical image, which remains the only test
  that settles a boundary.
- Hex-Rays on the debug IDB works, and IDA's 8,813 functions there match the earlier
  scanner count. Decompiler output has not been used for any claim in this document.

## 9. Counting between anchors: three boundaries applied

A bracket says where a file starts to within a few functions. Where an anchor falls
just inside the bracket, the gap between two anchors can be counted on both sides and
the boundary comes out exact. The three files after `dds3ModelBasic.c` are the worked
case, and the first of them is the `dds3ModelRangeBasic` tension section 5 left open.

Three hash anchors fix the frame. Debug 0x0011A7B8 is retail 0x00115E88
`dds3ModelBasicSetMotion`, debug 0x0011A878 is retail 0x00115F28
`dds3ModelBasicFreeModel`, and debug 0x0011AA68 is retail 0x00116058
`dds3PathEvalSpline`, each byte-identical at 92, 64 and 356 bytes. Between them the
function counts agree exactly:

| debug run | file | functions | retail run |
|---|---|---|---|
| 0x0011A818 to 0x0011A99C | `dds3ModelRangeBasic.c` | 5 and 5 | 0x00115EE8 to 0x00115FFC |
| 0x0011A9A0 to 0x0011AA64 | `dds3EventBasic.c` | 2 and 2 | 0x00116000 to 0x00116057 |
| 0x0011AA68 on | `dds3PathBasic.c` | | 0x00116058 on |

The runs come from the assert paths: `dds3ModelRangeBasic.c` is referenced from
0x0011A818, 0x0011A8B8 and 0x0011A940, `dds3EventBasic.c` from 0x0011A9A0 and
0x0011AA20, `dds3PathBasic.c` first from 0x0011ABD0. The 0x0011A878 anchor sits second
of five on both sides, so the correspondence is pinned in the middle as well as at
both ends.

So `dds3ModelRangeBasic.c` starts at 0x00115EE8, not the proposed 0x00115F68, and
`dds3PathBasic.c` starts at 0x00116058, not the applied 0x001161C0. All three cuts are
in the config and the image still hashes to
`5facfbf7916836613449c922e26a4ec412207b32`.

The HD names in the region are what misled the earlier reading. `dds3ModelRangeBasic.c`
holds functions the HD map calls `dds3AllocModelBasicWorkData`,
`dds3ModelBasicFreeModel`, `dds3SetPathBasicSpline` and `dds3SearchSceneBasicWorkData`,
so a name's prefix is not evidence of which file it is in. Those names are untouched:
the debug build locates the file boundary, not the function names.

What stays open in the region: the remaining `code_00115D38` chunk starts at
0x00115D38 with `dds3PathBasicFreeControl` and runs to 0x00115EE8, which the debug
build attributes to `dds3ModelBasic.c`. There is no anchor below it, so its start is
still only bracketed and it keeps the `code_` name.
