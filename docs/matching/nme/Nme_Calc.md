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

## nmeDrawNameText (nme/Nme_Calc, 0x3E4)

18 of 249 words match in the existing guarded draft. It emits 940 bytes
against native 996, with 18 matching words in the 235-word overlap and
56 native bytes uncovered. The following alternatives are unpublished
diagnostic controls, not retained source. Replacing its three volatile word intermediates with an ordinary three-byte prefix copy emits 908
bytes, with 7 matching words in 227 and 88 bytes uncovered. The copy reproduces
the native three signed byte loads followed by three byte stores and the 128-byte
frame, but worsens overall positional matching. Independent working X coordinates
for the two sprite and two glyph loops also emit 908 bytes, with 6 matching words.
These controls do not justify a source promotion or matching-progress claim.

Unhooked ordinary and dumped compilations agree on assembly, allocated sections,
symbols and ordered relocations for all three controls. The expanded RTL already
represents the row coordinates as scaled x/y plus constant offsets. CSE retains
shared scaled values across drawing phases; the loop pass already strength-reduces
the indexed X formulas. Combine marks the original unscaled x/y values dead.
Native instead retains unscaled coordinates and separately initializes the glyph
phase after its count gate, with a saved stride and zero-priority value. Explicit
induction locals alone do not supply those lifetime properties. No register grid,
false volatility or unrelated scalar reuse is justified by this evidence.

The native template contract is only a three-byte prefix (30 30 00), not a proven
external object extent. The cursor snapshot, both eight-iteration sprite loops,
conditional mode queries, callback-visible work/texture/count reloads, signed glyph
counts, font call ordering and final row reset are represented in the ordinary
control. Known callers use x 42/244 and y 84/100/150. No arbitrary-coordinate or
out-of-range input-array behavior is inferred. The original guarded body and
assembly fallback remain unchanged pending stronger source-lifetime evidence.

A subsequent localized diagnostic placed one empty read/write operand constraint
on original x/y at the real first-glyph phase, after its signed positive-count
gate. An ordinary if/do-while control emits 916 bytes with 7 matching words and 80
uncovered bytes. The constrained control emits 940 bytes with 8 matching words
and 56 uncovered bytes. Both retain 13 other exact active owner functions, and
ordinary/dumped full-owner output parity passes. The constrained outputs feed
actual post-gate scaling in combine insns 784/874 through constraint insn 370,
confirming that value identity blocks reuse of the earlier scaled coordinates.
It still carries pre-scaled values through the sprites alongside originals and
does not recover native stride/priority phases. This is another negative
control, not a retained constraint or improvement over the original baseline.
