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

## Share the interpolation delta across template fields

`effPCPFLEACHBlurDraw` uses one `delta` for the five successive end-minus-start
interpolations: area, rotation, scale and the two center coordinates. Each
value is consumed immediately by its field's interpolation, and the local
retains the same meaning throughout. Independent expression temporaries leave
35 of 131 words different at the same 524-byte size.

Qualified `cc1probe.sh --alloc` traces of pinned cc1
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`
verify hooked/unhooked and dump/plain output parity for both forms. In the
exact form, RTL UIDs 134, 202, 216, 232 and 250 assign the five differences
to pseudo 90. Its combined ten references, live length 21 and zero crossed
calls produce one global allocation; conflicts with `$f0` through `$f7` and
`$f12` leave `$f8`. This reproduces all five native `sub.s $f8` operations
and the surrounding scheduling without register constraints. The complete
owner checks 385 match / 0 differ.

The local parameter's rectangle coordinates are filled by the draw provider
from its area and center before rendering. The native caller leaves those
four words unset; only the consumed area, color, blend, rotation, scale and
center fields are constructed here. The parameter setter copies the actual
0x2C-byte partial-blur body, preserving the owned texture tail.

## Group construction and outline indices

`pbAddGroupEntry` in `pb/pbGame` matches all 1,028 bytes. Its three map
passes and final group-outline pass use one unsigned `index` counter. Each
loop initializes the counter independently; it always counts entries.

Paired `cc1probe --alloc --no-dumps` runs pass hooked/unhooked and plain-output
equality with the pinned cc1 identified above. The shared index is pseudo 86,
with 49 references, live length 432 and nine crossed calls, allocated to $20.
A control changing only the final loop to a separate `groupIndex` leaves
pseudo 86 in $20 with 42 references, live length 410 and eight crossed calls;
the new pseudo 91 has seven references, live length 22 and one crossed call,
and receives $17. The control retains the native size but differs in the
three final-loop counter words at +0x3AC, +0x3C0 and +0x3C8.

Reproduce with `tools/cc1probe.sh <owner.c> --as-unit src/smt3/pb/pbGame.c
--func pbAddGroupEntry --alloc --no-dumps --out <output>`. For the control,
declare `u32 groupIndex` and use it only in the final outline loop. No value
is carried between loops. The block kind in the insertion pass has its own
local; sharing it with the creation pass is unnecessary.

The nested upper/lower kind checks retain the native separate comparisons.
The two creation-kind arms each initialize eight colors. The complete
256-byte group owns its block memory handle at +0xA0; its block records are
144 bytes. Construction and freeing use this same group type.

## Advance the live confirmation row

`mc/slMain::slQueryYesNoDraw` matches all 2,012 bytes. After drawing the two
highlight halves, advance the live Y by one for the two text labels. Both
values are row coordinates. A separate `fontY = y + 1` control emits 2,004
bytes, with 48 of 501 emitted words different and eight native bytes absent.

Paired pinned-cc1 `--alloc --no-dumps` traces pass hooked/unhooked and plain
output parity. The exact Y is pseudo 88 in $21, with 11 references, live
length 242 and eight crossed calls. In the split control, the base Y is
pseudo 89 in $21, with nine references, live length 238 and eight crossed
calls. The control computes the label arguments from the base plus 1 and
13; the exact form retains the Y update in each drawing branch.

Initialize the alternate highlight row only in the branch that uses it, and
initialize all three blink colors inside their selected phase. Keep message
tests independent, with fresh global reads after drawing callbacks. The
message-6 row choice uses an ordinary equality `if`/`else`; its arm order
reproduces the two constant loads before the conditional move.

Reproduce with `tools/cc1probe.sh <owner.c> --as-unit src/smt3/mc/slMain.c
--func slQueryYesNoDraw --alloc --no-dumps --out <output>`. The control adds
`s32 fontY`, assigns it from `y + 1` instead of incrementing `y`, and uses
that local in the final two label calls.

## Retain bigint limb and bit counts

`sdk/libc::_lshift` matches all 344 bytes with two live counts used across
disjoint phases. `count` first grows the limb capacity, then counts the
zero-filled low limbs. `bitCount` first holds the allocation exponent, then
the complementary carry shift. Every assignment contributes to the actual
allocation or output; neither value is carried between phases.

Pinned cc1 `d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`
allocation traces qualify the source and each control with hooked/unhooked
and plain-output parity. The deciding pass is global allocation. The exact
source's destination pseudo 89 has 20 references and live length 36, allocated
to $7; bit-count pseudo 94 has nine references and live length 21, allocated
to $5. Input, end and carry use $4, $6 and $3. With both counts split by
phase, destination pseudo 89 instead receives $5 and right-shift pseudo 97
receives $7, leaving 13 of 86 words different at the same native size.

Two independent controls establish that both count lifetimes matter. Keeping
only the bit-count reuse and splitting limb capacity from the zero-loop index
leaves 14 differing words. Keeping only the limb-count reuse and splitting
the allocation exponent from the carry shift leaves ten. Each control keeps
the 344-byte extent and first differs at +0x7C, where the destination pointer
is assigned after allocation. Their qualified allocation logs show changed
pseudo lifetimes and hard-register assignments; the source with both counts
shared restores the native pair without register constraints or extra work.

Reproduce each trace with `tools/cc1probe.sh <owner.c> --as-unit
src/smt3/sdk/libc.c --func _lshift --alloc --no-dumps --out <output>`. For
the first control, split `count` into `maxwds` and `zeroCount`; for the second,
split `bitCount` into `k` and `rightShift`. Compute the complementary shift
only in the nonzero-shift branch. The complete libc owner remains exact.

## Retain bigint subtraction values

`sdk/libc::__mdiff` matches all 400 bytes. Its result pointer first serves as
the operand-swap temporary, then receives the allocated difference. Both
subtraction loops use the same signed low-half result and unsigned limb
intermediate. The first loop subtracts both operands; the remainder loop
propagates the borrow through the larger operand. Each intermediate is
consumed by the real low/high stores and the next borrow.

Paired pinned-cc1 allocation traces use `--as-unit src/smt3/sdk/libc.c
--func __mdiff --alloc --no-dumps`; all pass hooked/unhooked and plain-output
parity. A further allocation trace with RTL dumps also passes dumped/plain
and hooked/unhooked parity. The compiler is the pinned 2.96 binary identified
in the preceding bigint-shift section.

In `rtl.13.life`, result pseudo 88 receives the swap input at UID 76 and
the allocator result at UIDs 53/118; global allocation assigns it $11. Low-half
pseudo 97 receives the two loops' sums at UIDs 179/237 and supplies halfword
stores 195/248; it receives $3. Limb pseudo 98 transforms the first input
word through high-half subtraction/addition at UIDs 189/192 and receives
the remainder high sum at UID 245. It supplies high-half stores 198/251
and receives $5. These are live arithmetic values, not register ties.

Independent single-property controls retain the 400-byte extent but differ:
a separate swap pointer changes two words, first at +0x60; separate low-half
locals for the two loops change eleven, first at +0xB8; splitting the limb
intermediate at the remainder loop changes three, first at +0x128. The
qualified allocator controls and the RTL definitions show the shared-pseudo
property that restores the native allocation without claiming original
source spelling. Reproduce the exact trace with `tools/cc1probe.sh <owner.c>
--as-unit src/smt3/sdk/libc.c --func __mdiff --alloc --out <output>`.

Keep the borrow live between the low and high half of each word. The equal
case writes `wds = 1` and `x[0] = 0`; the allocator already initializes the
sign. Both loops write the two halfwords through one advancing output-word
cursor. The complete libc owner remains exact.

## Command-entry scratch across skill and stock scans

This completes the guarded SetCommList recorded in commit `8df31643`. Its
matched skill and stock phases documented the same live-key reuse; the exact
source also keeps each player/demon escape gate in its own branch before
GCC merges the common tail. The old nonmatching entry is retired.

`newbattle/nbCommSelProcess::SetCommList` matches all 1,640 bytes. Its signed
`entry` scalar holds a skill ID in the skill pass and a stock ID in the later
summon pass. The loops define each value before every use; no value flows
between the passes, the address never escapes, and both values have real
array-index and command-list consumers. No extra operation or lifetime bridge
is needed. Independent review confirms the disjoint phases and truthful type.

Paired ordinary and stock-RTL compilations use the unmodified pinned ee-gcc
2.96, SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Both the shared-entry source and a control with a separate final-loop
`stockindex` pass full-owner ordinary/`-da` assembly equality (ignoring only
`.file`), allocated-section equality and raw-relocation-section equality.
This is stock compiler RTL evidence, not debugger instrumentation.

Initial RTL assigns the skill ID to pseudo 93 at UIDs 226/253 and the stock
ID to the same pseudo at UID 1274. In `20.greg`, the shared pseudo conflicts
with hard registers 2, 3, 4 and 5 and is allocated to $16. The separate-index
control retains the skill pseudo 93 in $16 but introduces stock pseudo 473;
its conflicts include 2, 3 and 4, not 5, so it receives $5. That moves the
stock-record offset and other final-pass values up through the argument
registers: the persistent-work base changes from pseudo 572 in $6 to pseudo
573 in $7. The control keeps the native size but differs in 24 words.
The shared pseudo's actual allocation and merged conflict set explain the
required ownership; matching does not establish the original local spelling.

Reproduce each form with `SMT3_AS_UNIT=src/smt3/newbattle/nbCommSelProcess.c`
and canonical `tools/cc.sh`, once normally and once with only
`-da -dumpbase <out>/rtl` added. Preserve assembly with `SMT3_KEEP_S` and
compare the complete owner, allocated sections and relocations before using
the dumps. The control declares `s32 stockindex` at the start of the final
stock loop and replaces only that loop's uses of `entry`.

The signed skill-type dispatch explicitly ignores -1 and passive type 1;
retail's 512 attribute rows contain only -1, 0, 1 and 2. Active type-0 IDs
fit the 288-entry normal table and type-2 IDs begin at 0x180. Player and demon
branches independently append and gate the escape command; GCC merges their
common call tail while preserving count publication before the pass command.
The input party argument is forwarded to `nbGetUnitWorkPointer`; the later
skill-cost call deliberately reloads `s->my->formindex`.

The 160-slot inventory scan is native behavior, not proof that the separate
item table has 160 rows: its declared native extent remains unchanged. The
stock scan reads byte IDs at save-work +0x1914, the count at +0x1924 and
0xEC-byte unit records at +0xA54, independently corroborated by `datAddDevil`.

## Snapshot the camera span before the angle call

The guarded `bCameraShot_AttackInvokeTargetPartyNear_Set` now reproduces the
native 776-byte size and 188 of 194 words. Keep the physical radius and the
later combined span as separate locals, and compute `span = length + distance`
before the second atan call. A single reused radius/span local emits identical
bytes; it is not required and is absent from the preferred source.

A qualified final-source `--alloc --sched --no-dumps` probe uses cc1 SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`
and reports `verify: OK`, with hooked/unhooked output equality and parity with
plain compilation. UID286 computes radius in pseudo95; UID360 computes span
in pseudo96, and UID391 scales that span. These distinct pseudos both receive
`$f20` because their live ranges do not overlap. Distance pseudo100 receives
`$f21`, and the earlier vector-length pseudo97 receives `$f22`. These are used
geometry values, with no extra lifetime or dependency introduced.

The first-found scan has two real arms: initialize the minimum and clear the
first flag, or replace only a strictly lower minimum. Publish the midpoint's
camera-Y field before the sphere-center call and reload it afterward. Finally,
write the last vector length directly to the camera distance field before
normalizing direction. These source boundaries recover native operations that
the earlier shared-tail/local-output form omitted.

Six words remain: the two atan-result quantities receive `$f1` rather than
native `$f2`. Their moves, divisions and span multiply account for every miss;
the allocation trace does not show a real constraint excluding `$f1`. Do not
invent a live value or fixed register to create one. For a reproducible probe,
use an otherwise unchanged full-owner copy with only this draft promoted, then
run `tools/cc1probe.sh /tmp/near.c --as-unit src/smt3/newbattle/nbCameraSkill.c --func bCameraShot_AttackInvokeTargetPartyNear_Set --alloc --sched --no-dumps --out /tmp/near-alloc` and require `verify: OK`.


## Stream flags followed by callback status

`sdk/libc::fflush` matches all 276 bytes using one signed `status` scratch.
It first holds the stream's signed flags for the write-enabled test and buffer
space selection. Each write callback then replaces it with the signed byte
count used by the error test, remaining count and cursor advance. The initial
flags value never crosses that callback; failure reloads the current flags
before setting the error bit. Independent native review confirms both phases
and the unchanged callback-visible state.

Qualified `--cse --alloc --reorg --no-dumps` probes preserve plain, hooked and
unhooked output parity with the pinned ee-gcc 2.96. UID92 reads the halfword;
UIDs94/96 sign-extend it into pseudo88. UID159 defines that same pseudo from
callback result `$2`. Its eleven references and eleven-instruction live length
cross no calls: each callback result is a new definition. Global allocation
has a `$2` conflict and a `$3` preference, assigning pseudo88 to `$3`.

A control with separate `flags` and callback `status` locals emits 272 bytes,
50 of 68 emitted words matching. Callback pseudo88 then has eight references,
live length four, a `$2` copy preference and no `$2` conflict, so it remains
in `$2`; flags pseudo89 receives `$3`. The native callback-result move and
related scheduling disappear. This is live scratch reuse with real consumers,
not an extra dependency or forced copy.

The attachment/initialization checks also retain the single-pass phase already
qualified for `__swsetup` in [control flow](control-flow.md). Flattening only
that phase keeps 276 bytes but differs in 20 of 69 words, first at +0x4C.
The byte-exact source retains both properties. Reproduce with
`tools/cc1probe.sh src/smt3/sdk/libc.c --func fflush --cse --alloc --reorg --no-dumps`.
The complete libc owner, including the other stdio helpers, remains exact.
