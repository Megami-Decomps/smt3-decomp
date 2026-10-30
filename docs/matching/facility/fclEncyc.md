# Matching notes: facility/fclEncyc

## ApplyChangeListIndex (facility/fclEncyc, 0x128)

The draft emits the native 296 bytes, with 56 of 74 words matching. It scans
signed 16-bit record indices, clamps the selected list position to the visible
range, updates the byte-sized drawing and logical shifts, then invokes the
independent edge-correction helper for every found entry. When the list is
shorter than the visible range, the upper clamp is zero.

The remaining eighteen words begin at +0x44: comparison operand order, timing
of the list-count and visible-count loads, signed versus unsigned comparison
selection, and byte-store order. Native reloads Shift after writing Index;
the draft retains it. The cursor, geometry and line-step pointers address real
subobjects of the existing list window. No layout or extent changes are used.

A more complicated visible-end formula emitted 320 bytes. The direct clamp
recovers the native size. Capturing both count bytes early or chaining the
shift stores retains 296 bytes but worsens overlap to 51 or 46 matching words;
those variants are not retained.

## DebugProcMenu_fclEncyc (facility/fclEncyc, 0x1A8)

The draft emits the native 424 bytes; 103 of 106 words match. It wraps the
three menu choices on horizontal input, returns one on cancellation, selects
the debug state on confirmation, then draws the three labels and selection
marker. The label table occupies sixteen bytes: three text pointers and a
null trailing entry. Its storage remains assembly-owned.

The allocator requests twelve bytes for the debug dispatcher work. A dedicated
work type now describes those bytes, with state and selection halfwords,
instead of borrowing the larger ribbon work type. The dispatcher and existing
debug callbacks use the same complete twelve-byte object.

Only +0x11C through +0x124 differ. Native prepares X before the color's low
half and zero attribute; C prepares the color, attribute and X in that order.
Qualified plain/hooked/unhooked scheduling traces agree: sched1 ties their
priority and register weight, but X has two dependents versus three for the
other argument setups. Sched2 retains that ordering through its original-order
tie break. These are independent constant arguments to the same print call.

Capturing the returned packet or the label pointer in a local does not change
the result. The retained source uses the direct print/link expression. Provider
coordinate and color parameters follow the unsigned 32-bit C definition.

## DrawRegistListwinElem (facility/fclEncyc, 0x18C)

The draft emits the native 396 bytes with 83 of 99 instruction words matching.
It draws seven registration rows. The existing work record's `fonts[7]` starts
at +0x11E0; the request at +0x11DC precedes that array and is not an eighth row.
Each iteration reads the current texture handle for the background, then reads
the font request after that draw callback. A non-null request receives two
foreground sprites and a font draw. The tag is finally submitted through the
ordering-table callback. The work view is captured before tag allocation.

The font provider's native sign extension establishes its `s8` mode parameter;
its existing font-unit declaration and native return paths establish
`func_001C7E38(frFontHandle_t *, s8, u32) -> s32`. The owner declarations now
agree with that contract, preserving all 105 normal C matches.

Sixteen words remain different at +0x20, +0x24, +0x28, +0x2C, +0x30, +0x34,
+0x38, +0x3C, +0x48, +0x60, +0x68, +0x70, +0x74, +0x8C, +0xCC and +0x10C.
The unresolved question is the native saved-register allocation and address /
sprite-argument scheduling; the operations and frame size agree. A separate
font-array cursor regressed to 71 of 99 words matching and was discarded.
A default scratch promotion qualified the complete owner at 105 match / 1
differ with this target as the sole difference. The retail build retains its
assembly fallback.

## ApplyEdgeListCursor (facility/fclEncyc, 0xA8)

42 of 42 words match at 168 bytes in ordinary C. The complete owner checks
110 match, 0 differ, including owned data. Source commit: `f173c1fadb534e63fd19fc84463ac9e4af0a46cf`.
Exact-source [CI run 40976126797](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40976126797)
passed build and deployment.

Keep explicit pointers to the existing cursor, geometry and vertical-step
subobjects. Snapshot the actual shift and maximum-shift bytes. At the upper
edge the observed shift is zero; `Shift = 1`, the index decrement, then
`DrawShift = shift + 1` express the actual transition to row one. Both one
values have real consumers. This source ordering produces the native store
ordering without a constraint, dummy value, artificial branch or new type.
At the lower edge, assign the truncated new row to the byte-sized `Shift`
then copy that stored value to `DrawShift` before calculating geometry.

Signed index comparisons remain distinct from the native unsigned-halfword
observation used for index updates. The geometry calculation preserves the
existing vertical-step multiplication and default Y addition. This does not
invent range guarantees for arbitrary list contents or alter the existing
valid-list contract. Independent review establishes StepY 28 and DefY 11
for the initialized Encyc windows; even row 253 gives Y 7095, safely within
signed 32-bit range. This is evidence for that UI domain, not a whole-program
proof against later arbitrary memory mutation or an arbitrary-raw-state
signed-overflow guarantee.

The inherited draft emits 160 bytes. Explicit real subobjects restore the
native size with 28 of 42 words matching. Store ordering reaches 33; using
`shift + 1` reaches 37. A separate unsigned index snapshot emits 176 bytes,
while treating the index update as only a signed observation misses native's
independent halfword load. Moving the index decrement between the real shift
and draw-shift publications closes the final two store differences. These
are measured source/output comparisons, not an attribution to an unqualified
internal compiler pass or a claim about historical source spelling.
