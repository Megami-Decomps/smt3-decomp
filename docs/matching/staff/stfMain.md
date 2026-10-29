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

303 of 321 words match, at retail's size.

The two things that got it there, from 87 matching words, are both layout. The
`x < -0xFF` fallback must be the `else` of `if (x >= -0xFF)`, so retail's
forward `bnez` puts it out of line. And `width = 0x100` has to precede
`fade = 0`, because retail's `bnez` delay slot holds `daddu $5, $0, $0`, which
serves as both `fade` and the `y` argument of the fallback's `DrwSprite2D`;
with `fade = 0` first the draft emits that zero twice and is 14 words worse.

What is left is one register-allocation tie, repeated. For
`cover = unk_0100 * 3; if (cover > 0x200) cover = 0x200;` retail ties the cmov
and the `addu` of the `* 3` to the value's register and emits
`movz $2, $5, $3`; the draft ties both to the constant's and emits
`movn $4, $2, $3`. That swaps `$2`/`$4`/`$5` through the clamp, the `* 3`, and
the later `slti $2, $18, 0x100`, and leaves the two `addiu`s of `step`'s 1 and
2 in the other order in the entry block's delay slot. Ruled out: `>= 0x201`
for the clamp, both ternary spellings, loading `unk_0100` into the same local
first, every permutation of the local declarations, and splitting the two uses
of `alpha` (which costs 16 words).
