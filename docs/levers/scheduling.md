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
