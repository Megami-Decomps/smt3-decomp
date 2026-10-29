# Two cross-block locals whose registers come out swapped


Where a value is tested in one block and updated in another, read **both** of
the values the second block needs at the second block's label, and test the
global directly at the first. `dds3KernelMain` tests `dds3PauseSign` and then
decrements `dds3PauseTime`; with `sign = dds3PauseSign;` at the test and
`time = dds3PauseTime;` at the `goto`, the two land in `$2` and `$3` the wrong
way round and the `movz`, both stores and the two delay-slot loads differ, 10 of
72 words. Reading `dds3PauseSign != 0` as the global and assigning both locals
at the `pause:` label is exact. Declaration order, the store order, `u32` for
the counter, `>= 1` for `> 0`, `time = time - 1`, and swapping which global is
read first in the test block all leave the pair swapped, so move the reads
rather than permuting around them.

## Keep an interpolated color in its existing local

`cmpDrawPanelEffRing` first blends `color8` and `color4` with neutral after
phase 128, then interpolates between them. Update the first color local with
that final blend. A separate final-color local leaves 26 of 94 words different
at the same 376-byte size; updating `colorA` matches all 94 words. Both values
are live packed colors, and the same local describes the evolving result.

Qualified `cc1probe --alloc` runs for the two complete owner variants passed
hooked/unhooked assembly and RTL equality, and dumps/plain assembly equality
(`verify: OK`). In `17.sched`, UID 26 loads color8 into pseudo 91. The final
blend is call UID 140; UID 142 assigns its result to pseudo 93 in the separate
form, but to pseudo 91 in the evolving-color form. Global allocation raises
pseudo 91 from four references, live length 26 and priority 3076 to eight
references, live length 58 and priority 4137. Its priority crosses the frame
pseudo 86 (3125). The separate form assigns frame/state/color8 to
`$18/$19/$20`; the evolving-color form assigns state/color8/frame to
`$18/$19/$20`, as retail does. This is the allocator property behind the
ordinary color update, not evidence for unrelated temporary reuse.

Reproduce with `tools/cc1probe.sh <owner.c> --as-unit
src/smt3/code_0022C578.c --func cmpDrawPanelEffRing --alloc --out <output>`.
The comparison variant declares a separate `u32 color`, assigns the final
blend to it and uses it for the corner colors; keep every other statement
unchanged.

The corner array is passed to the first box renderer. Initialize all four
corners again before the second call, and reload its geometry from the state
after the first call. Omitting the two neutral-corner stores shortens the
function by eight bytes. The native square-root operation is
`fsqrtf(50.0f) * frame * (1.0f / 256.0f)`, followed by integer squaring for the box offset.

## Normalize the live command-list coordinate

`DispCommandList2` matches all 648 bytes with the incoming `ox` updated to
the draw position in its layout branch. Both values are X coordinates; the
normalized coordinate is then passed to each renderer and the selected-row
help callback. A separate `drawX` local leaves 28 of 162 words different at
the same size when every other statement is unchanged.

Paired full-owner `cc1probe --alloc` runs with pinned cc1
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`
pass hooked/unhooked output equality and dumps/plain equality (`verify: OK`).
In `17.sched`, UID 8 loads the incoming X into pseudo 86; UIDs 51 and 63
update it by the two layout offsets. The exact variant retains that pseudo
in $21 with 15 references, live length 119 and six crossed calls. The split
control keeps incoming pseudo 86 in $6 (three references, live length 11,
no crossed call) and puts normalized pseudo 90 in $21 (12 references, live
length 108, six crossed calls). Keeping the input and normalized coordinate
in one live value recovers native entry and argument setup.

The mutable row Y is initialized from the incoming Y before layout selection.
After `GetCommListCnt`, snapshot that row Y as the help baseline. The count
and baseline are genuine live values, stored at native frame offsets +8 and
+0xC. Start the row index at `drawlisty`, and reload it for the loop limit
after callbacks; `drawcursor` instead selects the highlighted and help row.
Reload `commcnt`, disable state and the post-render help selection at their
native boundaries. Advance row Y only for an entry within `commcnt`.

Reproduce with `tools/cc1probe.sh <owner.c> --as-unit
src/smt3/newbattle/nbCommSelProcess.c --func DispCommandList2 --alloc
--out <output>`. The control declares `s32 drawX`, assigns it from `ox` plus
the layout offset, and passes it in the five drawing/help call sites.

The four `DispComm1` renderers consume six arguments: process, halfword entry
ID, X, Y, depth and packed color. Their sole native caller supplies those six;
the unused trailing `listidx` and `col2` declarations are removed. All four
renderer bodies remain byte-exact.
