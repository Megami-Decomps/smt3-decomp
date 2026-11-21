# Fusion menu labels and process name

The five native menu labels and the combine process name occupy six
eight-byte slots from 0x54FAD8 through 0x54FB07. They remain writable
character arrays, consistent with their existing declarations. Menu
tables use the label addresses, and fusion startup/exit pass the process
name directly to the process API.

The one-byte process state still precedes the text at 0x54FAD4. Its
original three zero padding bytes are kept by the generated small-data
pad before the next eight-byte-aligned unit. The remaining fusion small data starts at
0x54FB08. No function body or access width changes. The stock objects
keep their original small-data relocation kinds; moved objects can now
be addressed by their names, while references remaining in the original
unit rebase their section-local addends. Both placements resolve to
identical retail addresses.
