# Matching notes: nme/Nme_Calc

## nmeDrawCursorAt (nme/Nme_Calc, 0x2B8)

62 of 174 words match at the native 696-byte size. The older guarded body
emitted 764 bytes, including six volatile word temporaries absent from native.
Two ordinary three-byte copies now initialize the character buffer and signed
skip buffer. Only the first two character bytes change during drawing; the
third retains the copied terminator. External template declarations leave
their total extent unspecified rather than deriving it from the copied prefix.
The templates remain assembly-owned.

Native initializes the step to 22 and changes it to 23 for mode 2. Expressing
that default and override directly recovers the native conditional move.
The prefix through both template copies, argument saves and row setup matches
up to +0xBC. Broad loop register allocation and scheduling differences remain,
including row-Y storage, offset accumulation and draw-X update timing.

Element initializers remove the volatile traffic but do not recover the native
copy sequence. A separately accumulated row Y and explicit working coordinate
phases do not improve the retained body. Qualified normal, dumped and
instrumented compilation agree on assembly and all intermediate representations.
Native/provider review confirms signed offsets and skip values, row and column
indexing, callback-visible work reads, drawing arguments and final row reset.
Assembly remains active; no exact-match credit is claimed.

## nmeDrawNameString (nme/Nme_Calc, 0x1E4)

The native call to `nmeDrawSymbolRow` at 0x00298D9C explicitly passes a zero
third word. The analogous `nmeDrawNameText` call to `nmeDrawCharRow` at
0x002989B4 does the same. Both helpers ignore that argument. Their declarations
now retain an unused word formal and both callers pass zero; no artificial
use is added, and both 196-byte helper bodies remain exact.

The matching implementation copies the three-byte template prefix and keeps
the sprite positions separate from the later glyph positions. The glyph
loop reloads the current work pointer and signed row count after the font
calls. It keeps the third template byte unchanged while replacing the first
two bytes for each glyph.

The localized compiler constraints keep the actual incoming coordinates and
running pixel positions live over the calls. The native glyph phase separately
starts at row index zero and uses zero font priority; the tied row index
and 18-cell stride keep the compiler from using the row register to
initialize priority. All of those values are consumed by the renderer.
The function matches all 484 native bytes. The other 13 owner functions
remain exact.

## nmeDrawNameText (nme/Nme_Calc, 0x3E4)

The entire 996-byte function matches, together with all 14 previously
matching functions in the unit. The two sprite strips each draw eight
cells. The two glyph loops gate on signed row counts and replace only the
first two bytes of the three-byte font template. They reload the current
work object after each font callback; the refreshed count decides whether
the next glyph is drawn. The first row uses its own initial work snapshot
and adopts the next snapshot only when continuing. This recovers the
native branch and delay slots without an empty latch constraint.

The localized constraints act on the actual coordinates, sprite table,
row index, 21-cell stride, depth and font priority at their real drawing
phases. They keep the native values in the observed register lifetimes and
do not introduce extra callbacks, data reads, or writes. The first row
retains its unscaled coordinates until after the count check. The native
template is a three-byte prefix, not a claim about the full external
object's size.

## Name glyph offsets

The glyph builder uses adjacent signed bytes as the X and Y offsets for
each grid cell. The 96 pairs end at the first independent card message;
the previously attached card text is not part of the offset table.
Both name-entry tables retain their retail writable section despite the
read-only C views.
