# Read-only linker-fill report cases

These four cases add comparison references, not C source, game data or
production link inputs. Each existing production/SKIP payload matches
the retail read-only bytes. The following input's alignment produces
a separately identified zero-fill tail.

| Existing owner | C window (ROM, half-open) | Linker fill | Following owner | Alignment |
| --- | --- | ---: | --- | ---: |
| pb/pbDraw | 0x436CD8..0x436D4D (117 bytes) | 3 | pb/pbMain | 16 |
| newbattle/nbCameraProcess | 0x4355A8..0x4355E7 (63 bytes) | 9 | newbattle/nbCameraStartEnd | 16 |
| pb/pbGameDraw | 0x437040..0x4371C3 (387 bytes) | 5 | pb/pbRecord | 8 |
| sdf/sdfHdd | 0x44A6C8..0x44A833 (363 bytes) | 5 | sdf/sdfDev9 | 8 |

The live linker selectors and map prove neighboring ownership and exact
placement. The receipt pins the actual production and SKIP objects,
symbols, relocations and complete linked image; no outgoing read-only
relocation or incoming relocation into the fill is admitted for these
four cases. Reassembly preserves the original reference symbols,
relocations and all peer sections, with only the proved terminal
extent separated into the comparison-only linker-fill section.

The last stock read-only label is followed by .lit4 for pbDraw and
nbCameraProcess, EOF for pbGameDraw, and .sdata for sdfHdd. These
expectations are checked per case; they are not interchangeable.
Only the 930 existing C bytes become newly credited; all 22 linker
bytes remain visible and unmatched, with no change to total data.


## Puzzle Boy records and billboard manager

| Existing owner | C window (ROM, half-open) | Linker fill | Following owner | Alignment |
| --- | --- | ---: | --- | ---: |
| pb/pbRecord | 0x4371C8..0x437397 (463 bytes) | 1 | pb/pbHelp | 8 |
| effect/billManager | 0x433C28..0x433D49 (289 bytes) | 7 | effect/parManager | 16 |

These existing C sections have no outgoing read-only relocations. The
same receipt and reference-reassembly rules above apply; the last
read-only label must be followed by EOF for each of these two owners.
Their exact stock assembler spellings contain escapes, while their
literal byte lengths and linked comparisons use the actual native
string bytes. Thus 752 additional existing C bytes are compared, and
the eight zero linker-fill bytes remain separate and unmatched.


## Needle model renderer

The existing Needle renderer owns 79 exact C read-only bytes at ROM
0x434C68..0x434CB7. The following nine bytes stay unmatched as alignment
before the 16-aligned effPCPMisc read-only owner. The terminal diagnostic
has one newline byte with the exact stock assembler spelling. The
subsequent .lit4 section is unchanged.

## Kernel startup and Rag initialization

The Kernel Core, Create Data, and Rag initialization owners emit 322,
311 and 203 exact C bytes, respectively. The next linked sections add
six, one and five bytes of zero fill, which all stay unmatched. Each
case requires its original .sdata after the terminal .rodata label.
The Core diagnostic matches the exact stock newline spelling. The
reported increase for these cases is 836 existing C bytes only.

## Scene, Pipe effect, and font test

Scene Basic, Pipe Effect, and the font test emit 142, 127, and 81
exact C bytes. Their successors add two, one, and seven unmatched zero
bytes. The Scene and font test cases require EOF after the terminal
label; Pipe Effect requires its original .sdata section. The Scene
diagnostic uses the exact stock newline escape.


## Shared reference verification

The comparison references now share one complete linked-input receipt and
incoming-reference scan. Each owner still has its own assembly, native
window, and byte/relocation checks. The batch publishes its completion
marker last so that Ninja reruns an interrupted publication. Original
production commands and artifacts do not depend on this batch.

With the same 16 existing reference outputs removed and all production
inputs already built, the old and batched outputs are byte-identical.
In a warm-build measurement on the same checkout, reference verification
went from 46.0s to 28.1s with four Ninja workers and from 191.7s to
29.8s with one. These are reference-step timings, not full build times.

## Three named non-contributing linker selectors

The common command, interface, and field title cases contain 299, 189
and 174 byte-exact bytes of existing C. Their successor alignments add
five, three and two bytes respectively, all left unmatched. Each case
names its sole intervening linked object and requires a current receipt,
no read-only section or relocation section in that object, and exactly
one unchanged selector-only map line with no contribution, even zero.
All other owners keep the same strict adjacency checks.

## Sound, Viewer menu and battle effect jump tables

| Existing owner | C bytes | Unmatched link fill | Pinned local text relocations |
| --- | ---: | ---: | ---: |
| sdf/sdfSound | 2612 | 4 | 137 |
| event/evtEventViewerMenu | 2290 | 6 | 132 |
| effect/effBattle | 599 | 9 | 12 |

Each owner permits only its audited ordered R_MIPS_32 sites and targets
in its exact, receipt-pinned live text window. Production, SKIP and
stock must all project to their corresponding exact linked bytes.
The EE objects must have their exact empty RELA companion; the GNU
references must not. No imported symbol, other relocation payload, or
destination in stock-only text padding is allowed. Reassembly preserves peer bytes and relocations,
and the existing terminal literal and whole-link incoming-target checks
still apply. Only 5501 existing C bytes gain comparison credit; the 19
alignment bytes remain unmatched.

## Graphics memory and battle sound terminal jump tables

| Existing owner | C bytes | Unmatched link fill | Pinned owner text relocations |
| --- | ---: | ---: | ---: |
| sdf/sdfGsMemory | 716 | 4 | 177 |
| newbattle/nbKoukaProcess | 1340 | 4 | 248 |

Both named stock references end in a 59-word jump table plus one
non-relocated alignment word. Every native table source address,
linked target, literal annotation, and directed local text relocation
must match exactly. Only the single stock zero directive is split
out of the table, leaving all real table words and relocation
records untouched. The stock label extent shrinks from 240 to
236 bytes; .sdata for GsMemory and .lit4 for KoukaProcess remain
untouched, and the eight link alignment bytes stay unmatched.


## One linked symbol scan per comparison batch

The comparator snapshots the freshly pinned linked ELF's relevant symbols
once, preserving their order, and performs the same range checks for each
owner. The stock and candidate input objects are still checked independently.
The snapshot is not persisted or reused between batches.

On the same warm build of 24 reference objects with four Ninja workers,
the two runs averaged 32.8s before and 18.6s after sharing the
linked symbol scan. This is reference regeneration time, not full build time.
All reference outputs remained byte-identical.
