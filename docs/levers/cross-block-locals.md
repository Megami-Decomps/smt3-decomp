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
