# Non-matching drafts: nme/Nme_Calc

Conventions: [../../nonmatching.md](../../nonmatching.md).

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
