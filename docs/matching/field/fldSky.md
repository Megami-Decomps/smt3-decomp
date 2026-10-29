# Matching notes: field/fldSky

## fldSkyDrawFilter (field/fldSky, 0xAD8)

682 of 694 words match at the native 2,776-byte size. The remaining twelve
instructions differ only in three temporary stack locations: retail places
the table-pointer spill at +0x118 and the two derived row offsets at +0x11C
and +0x120; C rotates them to +0x120, +0x118 and +0x11C. The guarded source
preserves the native algorithm while the production build retains assembly.

Adapted from Raikaru's DDS1 sky-gradient reconstruction in commit
`15fdee1692`, using the [published DDS source](https://github.com/Megami-Decomps/dds-decomp/blob/bd9c319dbe656fa4c440baa1b9984ca6df7f43e1/src/dds1/field/fldFileResolver.c#L5671).
SMT3 uses `fldAutoMapChkMode`, OT offset 0x39 and `aDrawBoxG`; its current and
previous filter indices, fade timing, integer channel extraction and alpha
scaling were checked independently against native code. The first three
corner alphas are clamped, but the fourth is not, for both filter grids.
The 292-byte record contains its mode and an 8-by-9 color grid; seven rows
of eight quads consume neighboring corners.

The native table pointer is reloaded after drawing callbacks. A function-wide
snapshot would lose that behavior and is not a replacement for the direct
global accesses. A per-cell table local leaves the same twelve differences.
Swapping row/column declaration order changes their own stack locations and
adds twelve differences without fixing the temporary slots. No frame padding,
false aggregate or compiler attribute is used. Another attempt needs evidence
for temporary-slot allocation rather than more source-order permutations.
