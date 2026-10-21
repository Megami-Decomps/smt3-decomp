# Non-matching drafts: rstupdate

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## rstBuildDestroyList (rstupdate, 0x64)

1 of 24 words matches positionally, 23 of 24 with the register names ignored;
96 emitted bytes against retail's 100. Compacts the destroy unit's inherited
skill list, moving every non-zero entry to the front and storing the new count
back over `skillcnt`.

Every instruction is retail's, in retail's order: the `lhu` of `skillcnt` in
the preheader with its `beqz` skip, the source pointer walking from
`unit + 0x3A`, the destination pointer biased to `unit + 0x38` and advanced by
2 *before* both stores, the `sh $0` then `sh value` pair, the `sll 24`/`sra 24`
on the `s8` count, the reload of `skillcnt` inside the taken arm, and the
`slt`/`bnez` at the bottom. The destination's bias is what
`unit->skill[count - 1] = skill;` after `count++;` gives; the natural
`unit->skill[count] = skill; count++;` bases it at `unit + 0x3A` instead and is
no closer.

The one missing word is a `daddu $3, $2, $0` copy of the loaded skill. Retail
loads the halfword into `$2`, copies it into `$3` (where the loop bound is dead
at that point) and reuses `$2` for `count + 1`; the draft computes `count + 1`
in the `beqz` delay slot, which keeps `$2` busy, loads the skill into `$3`
directly and needs no copy. That one value shifts every register in the
function up by one, which is why the positional count looks so much worse than
the body is.

Ruled out: `skill` as a block-scope and a function-scope local, and as `u16`
and `s32`; `count` as `s8` and `s32` (the `s32` form is 22 words, two short);
`count++` written before the stores, between them and after them;
`unit->skill[count]` with a post-increment; a `u16 *dst = &unit->skillcnt;`
local advanced in the arm; and a `while` loop with `i++` hoisted above the test,
which runs one word long.
