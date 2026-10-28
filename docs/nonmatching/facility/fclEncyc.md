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
