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
