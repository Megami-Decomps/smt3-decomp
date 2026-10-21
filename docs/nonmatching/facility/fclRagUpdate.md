# Non-matching drafts: fclRagUpdate

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## ragCheckHeartsChoice (fclRagUpdate, 0x80)

22 of 29 words match, 116 emitted bytes against retail's 128. Returns a hearts
choice: a table entry for index 1, the rag work's `+0x54` halfword for 2 and 3,
and 0 otherwise.

The body is settled. `ragGetHeartsFlag`'s result has to land in `$4`, the
`index == 1` test has to be written as `if (index != 1) { ... }` with the table
arm after it so the `beq` reaches the table arm as retail's forward branch
does, and the two range tests have to be **nested** `if (index > 0) { if (index
< 4) ... } return 0;` rather than two sibling early returns: as siblings
ee-gcc folds them into an `addiu -1`/`sltiu 3` range test and loses retail's
`blez` and `slti 4` pair, which is 8 of 30 words match.

The seven remaining words are the exit layout. Retail restores `$16` twice,
once in the delay slot of the `b` that leaves the table arm and once at the
shared `L830`, and lets `jump2` merge only from the `ld $31` on; this draft
gets one `ld $16` and three words fewer, so every branch displacement in the
function is off by three.

Ruled out: `index == 1` with a `goto` to a trailing table arm (8 of 30 match);
a `u16 ret` local assigned in each arm with one trailing `return` (8 of 30);
the same with explicit `goto done;` out of each arm (15 of 29); and
`(flag * 7) * 2` in place of `flag * 14` at the subscript.
