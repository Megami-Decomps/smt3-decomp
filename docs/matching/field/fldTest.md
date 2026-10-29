# Field test and chase matching

## fldRunDantePatr (field/fldTest, 0x2D0)

Matched in `b62c6009`: all 180 of 180 words match in the 720-byte production
body, and the assembly fallback is removed. The existing patrol body and
callback reloads are unchanged except for the short diagnostic's native symbol
reference.

`D_0054EAD0[]` retains the existing seven-byte `"Re1KO\n"` string and its
one-byte pad through `INCLUDE_SDATA`, preserving the complete 140-byte,
four-aligned owner. Emitting that literal instead raises alignment to eight
and fails retail despite a clean unit report. A truthful `[7]` extern keeps
data exact but emits a nonmatching 724-byte body; no enlarged bound is used.
The ten-byte `"ReCalcOK\n"` literal remains C at `0x00531308` in its 16-byte slot.

The complete owner checks 38 match, 0 differ; the entire retail image matches
SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`. Stock plain/`-da` output parity
also passes. `fldRunEffSet` and the remaining guarded drafts are preserved.

### Historical residual and controls

The earlier `376abcae` draft matched 178 of 180 words at the native 720-byte
size, with assembly active in that context.
The two differences at +0x1C0 and +0x1C4 exchange the speed load and the
stack distance load used to compute the route timer.

The routine converts Dante and player positions to map coordinates, finds a
16-byte cell record, and uses its route offset/count at +8/+0xC. The found
cell pointer is stored at Dante +0x28, separately from the route data pointer
at +0x2C. A route with two or more points probes the next point against enemy
collision; a blocked probe returns to the first point. Successful shortcuts
advance the waypoint and set the shortcut flag. Patrol movement uses
turn-toward parameters 15 and 1 (divisor 15, minimum step 1), updates position,
and enables the patrol BGM state.

A qualified `cc1probe.sh --sched --no-dumps` trace of the isolated C body in
this unit has equal plain, hooked and unhooked outputs. In sched1, distance
load UID546 and speed load UID548 both have priority 14, register weight 1
and cost 2; their original order breaks the tie (LUID105 before106). In
sched2 the distance load has three dependents versus two for speed, retaining
that order. The native order is speed then distance. These observations
identify the residual without establishing the original source spelling.

The blocked-probe branch precedes the shortcut branch, retaining the native
branch and common completion rather than a shorter annulled branch. Advancing
the waypoint before setting the shortcut flag preserves the native store
ordering. A floating frame value followed by the integer timer assignment
keeps the native FPU registers; an explicit speed snapshot adds register
allocation differences. A one-pass timer block does not improve the two-word
residual and is not retained. No artificial dependency or compiler flag is
used. The map-cell fields and target-coordinate/pointer view are corroborated
by `fldFindMapCell` and `fldRunAiCng`.
