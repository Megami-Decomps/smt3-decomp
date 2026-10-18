# The Japanese debug build as a TU boundary source

An assessment of `SLPM_652.42`, the Dec 26 2002 Japanese debug build, as evidence for
translation unit recovery in `SLUS_209.11`.

Every number here was measured on this ELF unless it is marked DDS, which means it
comes from `docs/tu-recovery.md` quoting dds-decomp's work on DDS1/2 rather than from
Nocturne. The retail side was measured from `orig/smt3/SLUS_209.11` and from the
function bounds in `config/smt3/symbol_addrs.txt`.

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
except one real correction: `newbattle/nbCameraHelper` is `../effect/src/nbCameraHelper.c`,
and `nbCameraBoss` is in `effect/` too.

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

Median 4, max 86. So the paths attribute 1,073 of 9,042 debug functions, 12%. They do
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

### 4.1 Function bounds on a stripped ELF

With the IDBs gone, debug function bounds were recovered by scanning for `jr $ra`
plus delay slot and taking the union of all following positions with all `jal`
targets. Validated against the 1,073 bounds IDA had produced before the databases
died: 90.9% exact. Three narrower variants scored worse (80.8%, 65.3%, 65.3%). The
scanner finds 9,042 functions against IDA's 8,813, so it over-splits slightly, which
makes every pairing rate below a floor rather than an estimate.

### 4.2 The hash pairing rate

Using the `tools/pair_dds1.py` scheme unchanged (sha1 over instruction words with
`j`/`jal` targets masked to the opcode and immediate-form opcodes masked to the upper
halfword, paired on size and hash with a 16-byte floor, unique hashes as anchors, a
longest-increasing-run monotone cut, then ambiguity resolution inside anchor gaps):

| | debug to retail | DDS1 baseline (DDS) |
|---|---|---|
| functions, side A | 9,042 | |
| functions, side B | 9,447 | |
| unique-hash anchors | 3,841 | 2,508 |
| anchors after monotone cut | 3,840 | 2,401 |
| anchors dropped | 1 (0.03%) | 107 (4.3%) |
| one-to-one pairs | 4,723 | 3,040 |
| ambiguous | 831 | 1,211 |
| share of functions paired | 50.0% of retail, 52.2% of debug | 32% |

Size-equality hashing does not break. It works better here than on the DDS1 pair:
50% against 32%, and the monotone cut discards 1 anchor instead of 107, so the two
builds are in near-perfect agreement about code order. Code ordering matches retail.
None of the fallbacks (call-graph matching, string-reference matching, ordering plus
data-reference fingerprints) are needed.

### 4.3 The confound, and the method that works

Naive projection, following a path's referencing functions through the hash pairs to
their retail twins, recovered 1 of 1,073. The reason is structural: a function is
attributed precisely because it contains assert code that retail compiled out, so its
instruction stream differs. Attributed functions pair at 0.1%; everything else pairs
at 58.7%.

The fix is to stop following labels and interpolate positions through the monotone
anchors instead. Each file's debug-space gap is mapped to a retail interval by the
nearest anchors on either side, which brackets the boundary without needing the
attributed functions themselves to pair.

| | result |
|---|---|
| files bracketed | 171 of 171 |
| median bracket width | 20 retail functions, 3,448 bytes |
| brackets <= 5 functions | 13 (8%) |
| brackets <= 10 functions | 40 (23%) |

A bracket alone is not a boundary. It becomes one when a second source puts a
candidate inside it.

### 4.4 Intersecting with the HD map

Joining the 11,595-function HD map by name to retail addresses reproduces 303 HD
source-run boundaries, as `docs/tu-recovery.md` reports.

| | observed | expected by chance |
|---|---|---|
| brackets holding >= 1 HD boundary | 157 of 171 | 88.8 (sd 5.6) |
| brackets holding exactly 1 HD boundary | 92 | |
| brackets holding exactly 1 whose HD name matches the next file | 139 | 1.07 (sd 1.01) |
| brackets holding none | 14 | |

The chance baselines were computed by Monte Carlo, 2,000 trials each: the first by
scattering 303 boundaries uniformly over the 9,473 retail functions keeping the
bracket sizes, the second by shuffling the HD file names across the 303 boundaries.
157 against 88.8 says the brackets are real; 139 against 1.07 says the name agreement
between a Japanese 2002 debug build and a reconstructed remaster source map is not a
coincidence. These 139 are two-witness boundaries.

Note what each witness contributes. The address comes from the HD map. The debug build
contributes the bracket that admits it, the file's own Nocturne name, the file order,
and the file end. For the 14 brackets with no HD boundary, the debug build is the only
witness and the boundary stays a bracket.

### 4.5 Against the applied config

Of the 69 applied named starts, 59 reproduce as pinned boundaries with 0 name
conflicts. 10 do not pin, and 70 of the 139 pins are cuts the config does not have.

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
- `sdf/sdfMovie` start. The bracket confirms the doc's proposed 0x002C5080 and
  excludes the applied 0x002C53C8.
- `basic/dds3CameraObjectBasic` start. The bracket excludes the applied 0x00112500 and
  confirms the doc's widening, to within 6 retail functions.
- `evtPolygonMovie` to `evtPicture` pins at exactly the doc's 0x001F3A70.
- `effPCPThunder` to `effPCPBezier` pins at 0x0017AC10; `effPCPBezier` to
  `effPCPFlash` at 0x0017D0A0.
- `newdata/datCalc` pins at 0x00242A88, matching the doc's HD-only placement.

Open, and reported rather than resolved:

- `basic/dds3ModelRangeBasic`. Its start bracket ends at 0x00115F28, below the doc's
  proposed 0x00115F68. Two sources disagree by a small amount; do not apply either
  until the gap is explained.
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
   fields through `->`. Proof needed: pair the assert site with its retail twin
   through the hash pairs, then confirm the offset the retail code loads matches the
   field position implied by the expression order.
3. **Globals.** 36 of the 544 identifiers already match an existing retail symbol,
   which is a cross-check on those names rather than new information. The remainder
   are candidates that need an allocation site, an init function or a caller in retail
   before they can be applied.
4. **Function names.** Nothing. There are no `__func__` literals and no `.h` paths, so
   no function name string of any kind survives.

## 7. Plan, ranked by value to effort

1. **Apply the two corrections the debug build settles on its own.** `sdf/sdfMovie`
   start to 0x002C5080 and `basic/dds3CameraObjectBasic` start per the doc's widening.
   Both are splat config edits with two witnesses each. Cost: an hour plus a build.
   Risk: low, the per-unit diff plus the image hash catch any error.
2. **Add the 70 new two-witness cuts.** These come with a Nocturne file name, so they
   both split units and name them, which is the single largest increase in named
   coverage available. Cost: a day, since each cut needs a build to confirm the image
   still hashes to `5facfbf7916836613449c922e26a4ec412207b32`. Risk: low per cut,
   moderate in aggregate, so stage it in groups and build between groups.
3. **Fix `newbattle/nbCameraHelper` to `effect/nbCameraHelper`, and check
   `nbCameraBoss`.** A rename, no boundary change, directly contradicted by the path
   string. Cost: minutes. Risk: none.

Then, in order: resolve the 10 applied starts that did not pin and the
`dds3ModelRangeBasic` tension; work the 14 single-witness brackets down by narrowing
them with data references; use the `.rodata` ordering result when `.rodata` is split;
mine the assert expressions for bounds and field names under the proof rule in
section 6.

Nothing above requires the retail IDB to be written to, except the `nbCameraHelper`
rename and any name or type applied from section 6. Those follow the documented
procedure: correct in the IDB, re-run `tools/ida_symbols.py`, verify the image still
hashes to `5facfbf7916836613449c922e26a4ec412207b32`.

## 8. Caveats

- Hex-Rays availability on the debug IDB was never tested. Both IDA instances died
  mid-investigation and everything after that point was measured from the ELFs on
  disk. Unverified.
- MCP ports are assigned in open order and move between sessions, so the debug IDB
  has no fixed number. Address both databases by module name, `SLUS_209.11` and
  `SLPM_652.42`.
- Debug function bounds are from a 90.9%-exact scanner, not from IDA. Pairing rates
  are floors.
- The pinned boundary addresses come from the HD map. The debug build's independent
  contributions are the bracket, the file name, the file order and the file end.
