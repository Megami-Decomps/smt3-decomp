# pipe/pipeObject

## objOption

78 of 152 instruction words match at the native 608-byte size. The frame
and actual local extents match: one 32-byte object and a 16-byte way table.
The default build retains assembly.

The guarded reconstruction corrects two errors in the older draft: option
100 consumes all sixteen table bytes, with a byte counter from 15 through 0,
and reloads the parent ID and action for every construction. The exact
`objMake` and `objCopy2` definitions establish both call interfaces.

Status zero adds 128 to a 32-bit threshold, without byte wrapping. The signed
status switch preserves all three random masks and reaches native size.
An earlier unsigned-status variant emitted 600 bytes; a combined widened-
selector variant emitted 584 bytes. Neither is retained. The table cursor
is initialized with its copied buffer before dispatch. A separate loop
counter, a consumed-selector counter and explicit table-byte snapshot compile
identically. No source constraints, padding or extra observable accesses are
used.

Remaining questions are the initial table-copy temporary registers and byte
extension timing, the status-threshold copy and case-tail layout, and the
sixteen-iteration loop's sentinel hoisting and field-load scheduling. The
ordinary decrement-to-option-boundary loop already matches. Resolve those
specific differences before retrying declaration or loop-form changes.
