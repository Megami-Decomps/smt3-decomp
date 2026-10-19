# Non-matching drafts: script/scrParameterCommand

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## DispBaseData (script/scrParameterCommand, 764 bytes)

One hundred and seventy of 190 words differ and the body is one word short,
but the only structural difference is which of two column coordinates spills.
The function prints five labels down one column and three float triples down a
second column seven cells to the right, so it holds ten values across its
twenty calls and only nine callee-saved registers are available. Retail spills
the label column to `0x10($29)` and reloads it into `$4` four times, keeping
the value column in `$21`; every source shape tried here keeps the label
column in a saved register and spills the value column instead, which is one
reload fewer and is the missing word. Everything else lines up: the frame size,
the nine saved registers, the `$16` reuse for the model pointer and the row
coordinate, the reused `format` local that retail holds in `$17` across the
`sdfFloatToULong` calls, the three reused result locals, the eight-byte
outgoing slot for the fourth vararg of the rotation line, and the `2` and `8`
return values.

Retail's value column is `sll rd, x3, 2`, then `sll rd, rd, 4` and
`addiu rd, rd, 0x7540` much later, so its source computes it in more than one
statement from the shared `x * 3`; the extra references are what should raise
its allocation priority above the label column's five. Every spelling tried
collapses back to the shared `x * 3 << 6`, because cse folds `(x * 12) << 4`
into the `x * 192` the label column already computed, and the folded form has
too few references to win the register.

Ruled out, all leaving the same word count or worse:

- `((x + 7) * 3 << 6) + 0x7000`, `((x + 7) * 12 << 4) + 0x7000` and
  `(x * 3 << 6) + 0x7540` for the value column, each as a local and as the
  expression written at its three call sites.
- The same for the label column, as a local and inline at all five sites, and
  both declaration orders of the two locals.
- Four multi-statement forms of the value column with the second statement
  placed after the `poseset` null check: `x * 12` then `<<= 4` and `+= 0x7540`;
  `x * 3` then `<<= 2`, `<<= 4`, `+= 0x7540`; `x * 12` then `* 16 + 0x7540`;
  and `x * 12` then `*= 16` and `+= 0x7540`. All four fold to the shared shift
  and come out two words short.
- `(x + 7) * 12` then `(valueX << 4) + 0x7000`, `x + 7` then
  `(valueX * 3 << 6) + 0x7000`, and `(x + 7) * 3` then `(valueX << 6) + 0x7000`
  keep both columns in registers and run two or three words long.
- Reassigning the `x` parameter for either column instead of using a local.
