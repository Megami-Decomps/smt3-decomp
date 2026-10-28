# Non-matching drafts: facility/fclRagDraw

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## ragDraw (facility/fclRagDraw, 0x120)

69 of 72 words match, at the exact 288-byte size.

Residual 2: the three remaining words differ only by a register number. Retail
reuses `$4`, which held the work pointer and is dead by then, for the sequence
byte; the draft takes `$3`, which held the flag byte and is equally dead. The
`lb`, the `sltiu` bound check and the `sll` into the jump table follow from it.

Ruled out: a local for the sequence, a local for the flag byte, reading the
sequence through a cast, and `else if` in place of the early return. Nesting the
switch inside `if (flags & 2)` rather than returning early on its complement is
needed and is in the draft: it is worth two of the five words the flat form
leaves.
