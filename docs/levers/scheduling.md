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

### Reload work, then capture the call argument before the store

`pb/pbDraw::pbInitMapBaseDraw` matches all 196 bytes with two ordinary
post-clear snapshots. Keep the allocation in a `void *` local for the 12-byte
`memset`, then reload the actual typed work pointer through `panel->data.mat`.
Capture `panel->id` before writing `mapWork->value = sw`, and pass that saved
ID to `dds3SetProcessData`. The work value is at offset 4 in the existing
three-word work record. Both snapshots preserve the native reads after the
clear and before the value store.

The previous direct member-store/call form emitted the native size with three
words in the wrong order. A qualified `cc1probe --sched --reorg --no-dumps`
run on the complete promoted unit preserves hooked/unhooked output and plain
compilation parity. Work reload UID 160 takes `$3`, process-ID load UID 163
takes `$4`, and UID 179 copies the panel to `$5`. Reorg accepts value-store
UID 166 as the delay instruction for call UID 180, producing the native
`sw $18, 4($3)` there. The complete owner and its data match.
No volatile access, extra store or register constraint is needed.

Retain the full `D_00535CE8` process-name symbol through `INCLUDE_RODATA`.
Retail owns 24 bytes there: the 16-byte name and two aligned empty strings
occupying eight further zero bytes. Emitting only the name as a C string
literal leaves those bytes without an owner.

### A gp load always issues before the compare constant it is tested against

`field/fldTitle::fldTitle_Process` stops at 755 of 757 words, at the native
3028 bytes, on the two words at +0x4C: retail materialises the dispatch
constant (`addiu $4, $0, 1`) before loading `D_0054EF8C`, and every source
shape emits the load first.

A qualified `tools/cc1probe.sh <candidate.c> --as-unit src/smt3/field/fldTitle.c
--version smt3 --func fldTitle_Process --sched --no-dumps` run (`verify: OK`,
hooked, unhooked and plain assembly identical) gives the reason. In both sched1
and sched2 the load and the constant are the last two insns of the entry block
and both feed the `beq`. The load's result cost is 2, so its priority is 3,
against the constant's 2, and the load is ready in the call's own issue cycle.
`rank_for_schedule` therefore never reaches the register-weight, dependents or
original-order tiers that a source change could move. A priority tie would not
help either: the index load has the smaller luid, so the original-order tier
would pick it again.

Measured and rejected: a local snapshot assigned immediately before the
`switch` (identical output), a local initialised above the three `rset` calls
(26 words worse, the snapshot goes callee-saved), and a `(u32)` index (loses
the compare tree). The constant is already shared with case 0's
`D_0054EF8C = 1` store in both compiles, so the residual is the ready-list
pick alone.
