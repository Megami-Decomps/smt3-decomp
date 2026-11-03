# Matching notes: staff/stfMain

## stfMain2 (staff/stfMain, 0x2D4)

145 of 181 words match, at retail's exact size of 724 bytes.

Three things got the draft there. The centred row writes the same `sprintf` and
`stfDrawText` pair in both arms of the `text1 == NULL` test, because retail
emits that pair twice; one pair before the test is 24 words short. The centred
arm then ends in `continue`, which is what lets cross-jumping merge its
`jal stfDrawStaffText` with the second row's. And the state dispatch is one
`if (state != 0) { guard } else { fade; return 0; }` with the scroll update
after it, which is the only arrangement that puts the fade block between the
guard and the scroll update the way retail lays it out.

What is left is one guard. Retail tests the range as `bltz` plus a separate
`slti 3` with `beql`, two branches into the trailing `return 0`. Every spelling
that keeps the operands equal, `state < 0 || state >= 3` and
`state >= 0 && state < 3` with an empty arm, folds in `fold_truthop` to the one
`sltiu $2, $2, 3` the draft emits, and the branch comes out as `bnel` past the
return instead of `beql` into it. Spellings that block the fold by reading
`oStaff_UI->reserved00E4` again for one side do keep two branches, but run a
word over and drop to 106 matching words. Ruled out: `state > 2` for the upper
bound (identical output), swapping the two operands, and a single trailing
`return 0` with the scroll update nested inside the guard (99 words).

The remaining 36 words are that branch plus two register-naming knock-ons it
drags along: `subu $3, $6, $3` for `0x100 - w / 2` comes out as
`subu $6, $6, $3`, and the `$2`/`$3` pair in the two divisions is swapped. None
of those three moved under any source spelling of the expressions themselves
(a named local for the coordinate, `w = w / 2` first, and reordering the `w`
and `y` declarations all give byte-identical output), so they follow the guard.

## ObjDispOff (staff/stfMain, 0x504)

311 of 321 words match at the native 1284-byte size in the guarded draft.
The target remains assembly-backed; this is not exact-match credit.

The earlier 303-word draft established the following ordinary source layout.

The two things that got it there, from 87 matching words, are both layout. The
`x < -0xFF` fallback must be the `else` of `if (x >= -0xFF)`, so retail's
forward `bnez` puts it out of line. And `width = 0x100` has to precede
`fade = 0`, because retail's `bnez` delay slot holds `daddu $5, $0, $0`, which
serves as both `fade` and the `y` argument of the fallback's `DrwSprite2D`;
with `fade = 0` first the draft emits that zero twice and is 14 words worse.

The earlier 303-word draft emits `movn $4, $2, $3` for the cover clamp,
where retail keeps the result with its input in `movz $2, $5, $3`. The old
explanation attributed the later register and constant-order differences to
this same clamp. Qualified controls now separate those effects. Ruled out: `>= 0x201`
for the clamp, both ternary spellings, loading `unk_0100` into the same local
first, every permutation of the local declarations, and splitting the two uses
of `alpha` (which costs 16 words).


### Qualified clamp dependency and bounded controls

The plain and diagnostic compiler builds agree on six allocated sections,
109 non-file symbols, 86 ordered relocations and normalized assembly. In the
old draft, if-conversion defines the clamp result in a fresh pseudo from the
product and the constant. The subsequent **regmove** dump explicitly records
that it fixes the constant operand to match the destination, rewrites the
result to the constant's pseudo and updates the following comparison. Thus
the `movn` direction is already determined before register allocation; it is
not merely an allocator tie inferred from final disassembly.

A localized binding of the actual cover input/result to native register `$2`
keeps that value together through regmove and gives 310 of 321 matching words.
It is initialized by the real product and dies at the first clamp-result test,
before subsequent calls. No empty operand or fabricated consumer is needed.
The binding is a reconstruction aid, not evidence of original source spelling.
Adding a read/write empty operand before the clamp is worse at 301/321 and
is not retained. Removing the binding restores 303/321.

Separately, the actual fade numerator is assigned to `fade` and then divided
by two, retaining that real scalar across the two phases. This gives the
current 311/321. It restores the native numerator register but does not repair
all instruction scheduling. Explicit unsigned wrapping for products, affected
coordinate sums/subtractions and counters, and unsigned alpha packing,
preserve that output without relying on signed overflow or signed left shifts.
Conversions back to `s32` use the pinned EE compiler's low-word interpretation,
not a portable-C out-of-range conversion guarantee. Other subtractions and
divisions remain within their branch-proven ranges.

The ten remaining words belong to four distinct phases: the two entry step
constants; two words in the opposite-direction slide product; the width-range
predicate and branch; and four instructions scheduling the fade division and
remaining-width calculation. They are not all knock-on effects of the first
clamp. Native-register controls for the actual step-one value, slide product
and fade numerator give 304/321, 309/321 and 310/321 respectively when based on
the isolated 310-word clamp control; none is retained. Reversing the ordinary
step initializer/condition on the old baseline gives 302/321.

All measured controls preserve the five existing C functions and owned data.
The hardened draft also compiles without uninitialized-value or implicit-
declaration diagnostics and with identical output. Diagnostic flags are not
production options. The remaining work requires evidence for those separate
value lifetimes and scheduling dependencies, not more clamp spelling variants.
