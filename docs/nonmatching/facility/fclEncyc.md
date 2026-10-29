# Non-matching drafts: facility/fclEncyc

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

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

## drawRegistSprites (facility/fclEncyc, 0x26C)

The draft emits the native 620 bytes; 153 of 155 words match. It draws five
fixed horizontal strips, binds the first texture for the registration labels,
submits three additional sprites with that texture, then binds the second
texture for the final sprite and registers the draw tag. The five 8-byte
left/right pairs occupy the complete 40-byte table at D_00332340; the preceding
16-byte record is the strip color. Both remain assembly-owned.

Only +0xA0/+0xA4 differ: C loads the first texture handle before completing the
shared color address; native completes the color address first. Qualified
plain/hooked/unhooked scheduling traces agree. In sched2, the address completion
(UID163) and texture load (UID175) both have priority 19 and register weight 1.
The load has twelve dependents versus seven for the address and is chosen
first. Sched1 had selected the address first via register weight 0 versus 1,
but reload removes its dead-pseudo advantage.

Naming the color pointer or capturing the first texture handle does not change
the output. Grouping the two corners and UV record into one primitive object
is also inert. The retained draft uses the two ordinary local records and the
existing drawing-provider contracts. No synthetic dependencies, register
constraints or compiler options are introduced.

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
