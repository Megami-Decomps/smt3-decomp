# Non-matching drafts: nme/Nme_Init

Conventions: [../../nonmatching.md](../../nonmatching.md).

## nmeStartProcesses (nme/Nme_Init, 0x11C)

69 of 71 words match, at the exact 284-byte size. Allocates name-entry work
unless it is already active, starts the calculation, draw and update processes,
saves and clears the kernel debug sign, and begins a 30-frame fade. Event bit
`0x12` selects black when set and white otherwise.

Only the first two white-color byte stores differ: retail stores offsets 2, 0,
then 1, while this draft stores 0, 2, then 1. The black stores and every other
instruction match. The chained white assignment expresses equal RGB values;
the separate zero stores retain the native black-path ordering.

Bounded checks ruled out reversed and permuted three-field chains, separate
stores in native order, paired assignments before or after the third store,
a named color pointer, and a saved event result. These either retain two or
more differing stores or change register allocation and grow to 292 bytes.
The unresolved question is the source shape that gives the native white-store
schedule without changing the surrounding allocation and control flow.
