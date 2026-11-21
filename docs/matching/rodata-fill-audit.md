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
