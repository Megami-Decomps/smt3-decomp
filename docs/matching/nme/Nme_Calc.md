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

With that call contract restored, the retained name-string draft emits
492 bytes against native 484, with 3 words matching in the native overlap.
A three-byte copy in place of the volatile intermediates emits 460 bytes,
with 5 matching words. Separating the working pixel X coordinate reaches
14 matching words at that same size. Explicitly reusing the Y and cursor
locals for spacing and priority emits 476 bytes with 8 matching words;
the compiler still folds their constants rather than retaining the native
saved registers. These controls do not qualify a body promotion.

The remaining question is the real source lifetime of unscaled Y, character
spacing and zero priority. No register binding, volatile constraint or frame
padding is justified by these controls. The existing assembly fallback and
guarded body remain preferred pending stronger source evidence.
