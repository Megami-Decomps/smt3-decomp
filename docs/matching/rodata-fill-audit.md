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
