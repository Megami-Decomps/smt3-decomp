# A block-local whose live range reaches the end of the block costs a register


Local allocation is per basic block and runs before global allocation, so one
extra value live across a block pushes every function-wide pseudo up one hard
register. `evtPictureProcess1` interpolates a position with
`s32 cnt = work->pos_cnt - 1;` and stores it back to `work->pos_cnt` as the
last statement of the block: that keeps seven integer temps live at once,
local allocation reaches `$8`, and the `work` pointer lands in retail's `$9`
with the texture behind it in `$10`. Writing `work->pos_cnt--;` instead, or
storing `cnt` back first or in the middle, leaves six temps live, gives `work`
`$8`, and shifts the whole function's register names by one, which is 119 of
140 differing words against 103 for the long-lived spelling. Where a draft
differs from retail by a uniform `+1` or `-1` on the register numbers, look for
a block temp that retail keeps alive and the draft does not, rather than at the
function's own locals.

## An unused aggregate local still owns its frame slot

cc1 gives every aggregate local a stack slot at expand time whether or not the
function reads it, so a frame larger than the locals you can account for means
there are more declared. `nbCameraBoss_Beru_HAENO1_Set` reserves 0x60 of locals
and touches only 0x40 of it: the slots at 0x40 and 0x50 belong to two more
`sdfVector4_t` declarations the retail code never stores to, and the function
does not match until they are declared. The HD reconstruction of the same
function uses both, so they were live in the original source.

Scalars do not behave this way; only things that must live in memory do.
