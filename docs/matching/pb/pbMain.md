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

## pb_draw_bg_Process (pb/pbMain, 0x26C)

155 of 155 words match at 620 bytes using ordinary C. A native state switch
improves the initial 139/155-word draft to 147/155; placing ones-digit arithmetic
at its real draw consumer and reading the map directly completes the match.
Unused locals are removed. No register constraint, ABI view, type change,
extra consumer or compiler flag is needed. This establishes an exact ordinary
source shape, not historical spelling or an unmeasured optimizer mechanism.

State is unsigned 16-bit promoted to signed 32-bit, preserving all 65,536
encodings. State 1 clears the full flags word then draws; state 5 returns -1
without drawing; all other states draw and return zero. The native workdata
dereference precedes its null check; no new null-safety guarantee is claimed.
The process callback type, 16-byte allocation and constructor remain unchanged.

Render/test/alpha calls preserve native order, including the short-circuited
message-process and flag checks, conditional lower overlay, upper overlay,
ones and optional tens. Real provider signatures are unchanged. Providers may
invoke callbacks: map and signed stage are freshly reloaded after the ones
render before computing tens, rather than cached across that call.

Stage retains its full signed-16-bit domain. Promoted stage+1 ranges from
-32767 through 32768; signed division and remainder by ten preserve native
truncation toward zero, including negative values. Ones coordinates use the
signed remainder; tens draw only when the signed quotient is positive. The
intermediates fit signed 32-bit throughout this domain, without a fabricated
nonnegative or game-normal stage bound. Existing initialized process/map,
texture and provider validity requirements remain unchanged; anomalous digit
coordinates are not asserted to be safe texture inputs.

Independent complete-owner/data review gives 24 match, 0 differ and proves
all bytes outside the target replacement unchanged.
Full retail checksum, 149 guarded owners / 577 drafts, seven IRX owners and
26 tooling files / 427 tests pass. Actual published-checkout verification proves
all 780 tracked files, 23 owner dependencies, report/provenance equality and
no-op builds.
Source commit `23812ac2f851a52fd96f4fb89a7a9563810cc845` passed exact-source hosted CI
[41000591538](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41000591538),
including build and deploy.
