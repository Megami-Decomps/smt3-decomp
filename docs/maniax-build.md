# The Japanese builds as name donors and version targets

A survey of `SLPM_654.62` (Nocturne Maniax) and `SLPM_666.81` (Maniax Chronicle
Edition) against `SLUS_209.11`, the USA retail build this project decompiles, and the
method used to carry the USA IDB's recovered names onto both.

Provenance: **ELF** means measured on the files on disk, **IDB** means read out of an
IDA database addressed by module name, **doc** means carried from
`docs/debug-build-assessment.md` without re-measuring. The USA IDB was not written to.
The two Japanese IDBs were, and that is the point of the exercise.

## 1. The four builds

| | JP debug (doc) | Maniax | USA retail | Chronicle |
|---|---|---|---|---|
| file | `SLPM_652.42` | `SLPM_654.62` | `SLUS_209.11` | `SLPM_666.81` |
| date string | `Dec 26 2002` | `Oct 16 2003` | `Sep  3 2004` | `Mar 14 2007` |
| size | 4,568,088 | 4,543,376 | 4,551,696 | 4,544,016 |
| sha1 | `69d03ff7...` | `a2dee659...` | `5facfbf7...` | `ee4da2cc...` |
| `.text` size | 1,999,100 | 1,991,876 | 1,994,308 | 1,992,388 |
| sections | 28 | 28 | 28 | 28 |
| entry | 0x00100008 | 0x00100008 | 0x00100008 | 0x00100008 |

All four are stripped the same way: `.mdebug.eabi64` present but zero bytes, no
`.symtab`, no `.debug_*`. Both retail Japanese builds compiled out asserts exactly as
USA did, with zero `.c` paths and zero assert strings (ELF). Neither is a second
`SLPM_652.42`, and neither reduces the value of finding more debug builds.

Both Japanese builds identify as SCE PS2 SDK 2.5.0 through library stamps that are
byte-identical to USA's (`PsIIlibkernl2500` and siblings, `IOPRP250.IMG`). No compiler
version string exists in any of them, so the ee-gcc 2.96 attribution is neither
confirmed nor refuted here.

Chronicle carries the latest date string of the four, yet its code sits closest to
Maniax, not to USA. The pairing in section 5 measures this: Maniax to Chronicle needs
41 delta runs where USA to Chronicle needs 180. Chronicle continued the Japanese line
rather than descending from the localised build.

## 2. The flat-image property holds in all three

Every file-backed section in all three builds has the same vram minus file offset
delta, `0xff000`, one distinct value each, and in every case the section header table
ends exactly at the file size (ELF):

| | Maniax | USA | Chronicle |
|---|---|---|---|
| distinct deltas | `0xff000` | `0xff000` | `0xff000` |
| `e_shoff` | 0x454F30 | 0x456FB0 | 0x4551B0 |
| shtab end == file size | 0x455390 | 0x457410 | 0x455610 |

Rebuilding each file from its ELF header, program headers, file-backed sections at
their recorded offsets, zeros elsewhere, and the section header table reproduces its
sha1 exactly. So either Japanese build could become a second version target the way
`SLUS_209.11` is one, needing splat coverage of `[0x0, 0x455390)` for Maniax or
`[0x0, 0x455610)` for Chronicle.

## 3. Sections that are byte-identical across all three

Not merely the same size (ELF):

| section | size | note |
|---|---|---|
| `.vutext` | 59,312 | VU1 microcode, every byte, Oct 2003 through Mar 2007 |
| `.vudata` | 768 | |
| `.lit4` | 9,812 | float pool, same bytes in the same order |
| `.ctors`, `.dtors`, `.eh_frame` | 8, 8, 4 | |
| all eight `.DVP.overlay..*` | 1,416 + 2,048 x 6 + 736 | payloads, and the same decorated names with the same embedded hashes |
| `.DVP.ovlystrtab`, `.shstrtab` | 344, 509 | |

`.lit4` being identical byte for byte means the float literal pool is emitted from the
same sources in the same translation unit order in every build, which makes the
Japanese builds a second witness for `.lit4` ownership work.

`.reginfo` differs in one byte in both Japanese builds, the `$gp` value: USA
0x554170, Maniax 0x552470. `.DVP.ovlytab` differs in 18 of 108 bytes against USA and 9
of 108 between Maniax and Chronicle, in every case one `.data` pointer per entry offset
by that build's `.data` base delta, so the overlay table is identical modulo
relocation. `.bss` is 319,408 bytes in both Japanese builds against USA's 319,920,
another place the Japanese line shows through.

## 4. Where `.text` diverges

Measured by masking both `.text` images into instruction-word streams with the same
masking `tools/pair_dds1.py` uses, then diffing. The masking is what makes "identical
modulo relocation" measurable: unmasked, the same two streams produce 48,090 hunks
against the masked 2,097.

Maniax against USA: **99.02% of USA's 498,577 instruction words are untouched**. The
net shift at the end of `.text` is -608 words, which is the -2,432 byte section size
difference exactly. Identical runs of 4 KB or more cover 90.69% of `.text`, the longest
single run is 451,032 bytes, and all divergence sits in **60 clusters**.

The largest clusters, with names from the USA IDB:

| USA range | hunks | what is there |
|---|---|---|
| 0x001C6098-0x001CAF20 | 375 | `fr*` font and text renderer, 130 functions |
| 0x00297928-0x0029CDE0 | 187 | `nme*` name entry, 123 functions |
| 0x00154DD4-0x00157EC0 | 210 | `slSave*` and `slLoad*` save data |
| 0x001CDBD4-0x001D0C44 | 80 | `itfMesMng*` message manager |
| 0x0025B5AC-0x0025BF08 | 88 | battle command display, `DispComm1*`, `DispHelp1` |
| 0x00254598-0x00255FF4 | 103 | `SetAction_READY`, `SetAction_SKILL` |
| 0x00244530-0x002452B8 | 79 | `dat*` party and player parameter access |

The divergence is the text and territory layer, not game content. At whole-function
granularity only two USA functions have no aligned Japanese counterpart at all,
`slCopyNonZero` (64 bytes) and `frGetHelpPos` (304 bytes); every other hunk is an edit
inside a function present in both. The largest Japanese-only run is 1,224 bytes inside
`nmeDrawTabSprite`, where a kana entry grid costs more code than a Latin one.

USA retail is the localised Maniax rather than an edition missing Maniax content. Both
builds carry the added content: USA has `fldRunDanteClear` and
`effPCPDANTEShowTimeTable` (IDB) and the strings `Labyrinth of Amala` and `First
Kalpa` through `Fifth Kalpa`, Maniax has `DANTE_UNIT` and `?DanteStanceSet( %d )`, and
both carry the same save migration strings. On the data side a 647,040-byte run from
`datDevilTable` to `datExplainTable` is byte-identical, so the demon stat tables are
the same bytes; the one large `.data` cluster that differs is the `itfMesEmbedded_*`
message table, English against Japanese.

Chronicle's own additions are thinner still, and they are not in the engine. It keeps
every Dante identifier Maniax has (`DANTE_UNIT`, `eff_DANTE_thunder`, `field/dante.PB`),
adds `field/raidou_run.PB`, and drops only `?DanteStanceSet( %d )`. The Raidou
substitution was done at the asset layer on top of the existing Dante code, which is
why `.text` moves by 512 bytes.

## 5. Carrying the USA names across

`tools/pair_build.py` pairs functions between any two of these builds through their
IDA databases. It masks the fields that change purely because the image was relinked,
the 26-bit target of `j` and `jal` and the immediate halfword of the relocatable
immediate forms, and leaves branch offsets alone because they are PC-relative and
therefore real signal. Functions whose masked hash is unique on both sides become
anchors; the leftovers are aligned in address order within a band, then weak pairs are
promoted on callee agreement. Confidence tiers follow `docs/dds-port.md`.

The pairing is only credible if the address delta is piecewise constant, stepping at
insertions and deletions, so the tool writes the delta runs and they should be checked.
USA to Maniax produced 129 runs over 9,433 pairs with zero non-monotone adjacent pairs.

Three independent checks agreed:

- Against 757 anchors derived from shared strings, switch tables and `.rodata` blobs
  with no reference to code shape: **744 of 744 co-decided pairs agree, zero
  disagreements** at every tier combination.
- Against an independent byte search for USA function bodies sitting in Japanese code
  that IDA had not marked as a function: **248 of 248 agree**.
- Transitively, USA to Chronicle directly against USA to Maniax composed with Maniax to
  Chronicle: **8,896 of 8,896 agree**.

What was applied, high tier only, placeholder donor names and the entire low tier
excluded:

| | functions | named | share |
|---|---|---|---|
| `SLPM_654.62` before | 9,037 | 2 | |
| `SLPM_654.62` after | 9,508 | 9,080 | 95.5% |
| `SLPM_666.81` before | 9,039 | 1 | |
| `SLPM_666.81` after | 9,252 | 8,839 | 95.5% |

The function counts rose because both Japanese databases were missing function starts,
not because the builds differ. IDA's auto-analysis creates a function at every direct
call target, so what it misses is reached only indirectly. Of the 511 USA functions
found in unmarked Japanese code, exactly 2 are `jal` targets against a 58.8% baseline,
and 509 of 511 carry a name recovered from the HD symbols, so they entered the USA IDB
through that recovery rather than through analysis. 471 starts were defined in Maniax
and 213 in Chronicle. One, Maniax 0x0024EFA8, was 5,388 bytes of code IDA had marked
as data despite a `jal` to it from 0x00254B78.

Do not reach these by re-running auto-analysis on the segment. It already failed to
find them, and it risks touching `.vutext`, which is VU1 microcode and must never be
disassembled as MIPS.

## 6. The HD source tree

The HD remaster's reconstructed sources are not a Maniax snapshot. `itfMesMngGetVarLocalize`
is in HD's `itfMesManager.c` with a real body beside its non-localised sibling, and in
USA at 0x001CE538, but its whole 172-byte body is a USA-only run with no aligned Maniax
counterpart. A localisation accessor present in the HD tree and in USA but absent from
Maniax cannot come from an Oct 2003 Japanese tree. HD's `Nme_Draw.c` name entry is kana
only (`nmeDrawInputListHira`, `Kan`, `CapKan`, no Latin list), and the USA `nme*`
cluster is the worst-covered region in the binary at 27 of 123 functions named in the
HD map against a 71.5% baseline over the whole image. The HD tree sits downstream of
both PS2 builds.

## 7. What these builds are worth

Real:

- Byte-identical `.vutext`, `.vudata` and `.lit4` validate the VU tooling against two
  further independent builds, and give a second and third witness for `.lit4` order.
- Two more byte-exact flat-image targets are mechanically available.
- The 60 clusters are a map of which functions are territory-dependent. When an `fr*`,
  `nme*`, `itfMesMng*` or `sl*` function will not match, that map says whether the
  Japanese build can help.
- A named Japanese IDB is a second reading of any function whose USA form is unclear,
  at 99.02% agreement for Maniax.

Not:

- No symbols of their own: 2 and 1 named functions before this work. Every name in them
  came from the USA IDB and is therefore no better than its source.
- No translation unit boundaries at all, 0 `__FILE__` paths and 0 asserts.
- No new content.

## 8. Corrections this work produced

- The `.data` size delta between Maniax and USA is 7,552 bytes (0x1D80), not 8,064.
- The 534-function gap between the USA and Maniax IDBs was never a difference between
  the builds. It was IDA auto-analysis against years of curation, and it closes exactly:
  248 unmarked-region matches plus a 286 net unpaired difference.
- 43 name collisions remain unadjudicated, where the code-shape pairing and the
  independent byte search disagree about which Japanese function should take a USA
  name. 41 are sixteen-byte newlib and RFU stubs whose bodies are too short to be
  unique. They are deliberately left unnamed.
