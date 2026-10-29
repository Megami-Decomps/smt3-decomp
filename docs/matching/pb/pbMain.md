# Matching notes: pb/pbMain

## pbEditInitProc (pb/pbMain, 0x148)

79 of 82 words match at the exact 328-byte size. The constructor table uses
the editor root's process-link prefix and returns a panel handle; its zero
stub follows the same parent/handle signature as the game and help constructors.
The dispatcher preserves the seven-entry jump table, previous/next/direct
editor transitions, both halt paths, conditional return to the administrative
sequence, and process-name/state diagnostics. Native strings and jump-table
entries were checked with the reconstruction.

The remaining three words follow `p2sdStopBGM`: the map-work pointer is loaded
into `$3`, its byte at `+0x35C` into `$2`, and the branch reads `$2`; retail
uses `$2` for the pointer and `$3` for the byte and branch. The actual sound
provider returns a signed command status, so the declaration remains `s32`.
An incorrect `void` declaration reproduces all 328 bytes but is not a qualified
match. Explicitly discarding the result and naming the map/flag locals leave
the same three differences. An early return for a zero flag instead merges
the two administrative-call tails and emits only 304 bytes. The accurate API
and ordinary conditional call are the preferred draft; production keeps assembly.
