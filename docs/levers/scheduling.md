# Store scheduling

### A final member store ends the object pointer's live range

`pipeLoadEffect` in `pipe/pipeEffect` initializes a free effect slot after
loading its handle. Ordinary stores in source order `pos2[0]`, `d3p`,
`pos2[1]`, `offset`, `count` produce retail's order `pos2[0]`, `count`,
`d3p`, `pos2[1]`, `offset`.

The two source forms were qualified with:

```
tools/cc1probe.sh <candidate.c> --as-unit src/smt3/pipe/pipeEffect.c \
    --func pipeLoadEffect --sched --no-dumps --out /tmp/effect-slot
```

Both traces returned `verify: OK`, with hooked, unhooked and plain assembly
identical. With `offset` written last, sched1 sees uid78's store as killing
both its value and the slot pointer: register weight -2 rather than -1.
After the first coordinate store, `rank_for_schedule` therefore selects it
ahead of count uid69, handle uid72 and second-coordinate uid75. Sched2 keeps
that order through equal-priority original-order comparisons. Writing `count`
last gives its actual initialization the final pointer use and corresponding
priority; the full function then matches all 160 bytes.

This is ordinary member initialization, with every stored value consumed by
the effect's lifetime. No artificial use, dependency or volatile access is
needed. Keep the resource-table base separately from the advancing slot
pointer, and retain the native byte-sized predecrement free-slot scan.

### Initialize the counter before the offset snapshot

In `pipeGetObjData` (`pipe/pipeEffect`), the mode-6 loop needs `i = 4`
before `offset = offsetCount + 4`, followed by `for (; i != 0xFF; i--)`.
Putting the counter assignment in the `for` initializer permutes nine
preheader instructions. The earlier assignment closes all nine while the
other two arms remain exact.

### Initialize the cursor after clearing its temporary buffer

`pipeEffDraw2Doffset` clears a four-float position buffer before assigning
its effect cursor from the input parameter. That source order retains the
native argument copy through `v0`, its delayed move to the saved cursor,
and the loop alignment. Initializing the cursor before `memset` removes
eight bytes. Separate local snapshots of the effect's X and Y coordinates
also preserve both loads before writing the temporary `tagPipeObject`.
The translated position buffer and object occupy 0x10 and 0x20 bytes;
no unused frame padding is needed.

### Final uses can reverse independent initialization stores

`nbCameraState_Start_Animation` initializes two real Bezier segments. Assign
`step` to element 0 then element 1, and clear `t` in element 1 then element 0.
This source order emits the native reverse order for both pairs and closes
seven of 161 words at the unchanged 644-byte size.

Paired qualified `cc1probe.sh --sched --alloc --no-dumps` runs verify
hooked/unhooked output and plain compilation parity. In the exact form,
sched1 UID 28 stores the shared float pseudo 91 to element 1's `step` with
a DEAD note, giving register weight -1; UID 21's element-0 store has weight
0. Equal priorities therefore put UID 28 first. UID 53 clears element 0's
`t` and ends the shared address pseudo 89, so it likewise precedes UID 48's
element-1 clear. Original-order tie breaks place the intervening timing and
state stores. The control reverses which stores end those live ranges and
leaves the seven initialization words different. No extra stores, fixed
registers or artificial dependencies are needed; the complete owner matches
11 functions and its data.

Reproduce with `tools/cc1probe.sh <owner.c> --as-unit
src/smt3/newbattle/nbCameraStartEnd.c --func nbCameraState_Start_Animation
--sched --alloc --no-dumps --out <output>`. The control moves element 1's
`step` assignment before the timing globals and element 0's assignment after
the state store, then clears element 1's `t` last.
