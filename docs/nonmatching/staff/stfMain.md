# Non-matching drafts: staff/stfMain

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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
