# Non-matching drafts: kernel/dds3KernelCore

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## RunningProcess (kernel/dds3KernelCore, 0x124)

33 of 73 words match, at the exact 292-byte size. Positionally 40 of 73
differ; aligned as a sequence, 32 do, because the first third is shifted one
word by a missing loop-alignment `nop`.

The scheduler walks the running and ending lists together, runs whichever
process has the lower `priority`, and resumes from the process's `prev`
link because `RunProcess` returning 0 means `EndProcess` has already unlinked
it. Retail has four copies of that body: two in the merged loop and one in
each single-list tail loop. The draft reproduces the block layout, all four
`beql` prev tests, the `sltu` comparison, every branch-likely form and the
exact size.

What is left is one register pair. Retail holds `run` in `$16`, the `prev`
temp in `$17` and `end` in `$18`; the draft gives `prev` `$16`, `run` `$17`
and `end` `$18`. `prev` wins `$16` because global allocation ranks allocnos
by references per unit of live length, and the one `prev` pseudo is live only
across the four calls while `run` is live almost everywhere. Nothing in the
source moves that ranking: declaration order is inert (all six orders of the
three locals measured, identical output), and splitting `prev` into `runprev`
and `endprev` makes it worse, 43 of 73, by also costing the `daddu $4`
placement.

The second residual is the `nop` at `+0x1C` that aligns retail's loop top to
eight. ee-gcc emits that alignment only for a loop the front end marked with
a loop note, and the `goto loop;` the draft needs to stop the loop being
rotated gets no such note.

Ruled out:

- `while (run != NULL && end != NULL)`, and `while (1)` or `for (;;)` with
  `if (run == NULL) break; if (end == NULL) break;`. All four spell the same
  thing and all four let `expand_end_loop` move the exit tests to the bottom
  of the loop, which costs the entry `b` and the preheader split: 52 to 64 of
  71 or 73 words. Only an explicit label with `goto loop;` keeps retail's
  test-at-top layout.
- The tail loops inlined inside the two guards rather than written after the
  merged loop. That duplicates the epilogue and runs past the next function.
- `else if (prev == NULL) { p = head; } else { p = prev->next; }`. The
  inverted spelling, `else if (prev != NULL) { p = prev->next; } else
  { p = head; }`, is what gives all four copies retail's `beql` plus the
  annulled head reload; the `== NULL` form inverts three of the four to
  `bnel` and is two words shorter.
- `if (end->priority < run->priority) { end } else { run }`. Retail's
  `sltu` operand order and its `bnez` to the end arm come from the opposite
  spelling, `if (run->priority <= end->priority) { run } else { end }`.
