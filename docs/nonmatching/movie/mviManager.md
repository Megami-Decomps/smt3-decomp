# Non-matching drafts: mviManager

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## movieViewerStep (mviManager, 0x4C)

6 of 14 words match, 56 emitted bytes against retail's 80. Walks the movie
viewer's chip list forward by the list's own count and returns the node it
lands on, or NULL if the chain runs out first.

The entry sequence and the single shared exit are settled: the head load into
`$3`, the count `lh` in the `beqz` delay slot, `blez` on the count, the
peeled first `chip = *chip`, and `jr $31` / `daddu $2, $3, $0` all match.
What is left is the loop's closing branch. Retail ends the loop with
`bgtzl $2` and the next `lw $3, 0($3)` in its annulled delay slot, which is
also why retail's loop carries five `nop`s and one trailing `nop` that the
draft does not: the pad is the R5900 short-loop fix the assembler applies to
a loop closed by a branch-likely, so getting the `bgtzl` back should bring
the padding with it. Every spelling that writes the preheader advance
explicitly has it folded away again: gcc threads
`n--; if (n > 0) { chip = *chip; goto loop; }` into an unconditional `b` back
to the entry `blez`, which reuses the guard as the loop test and leaves one
`lw` instead of two.

Ruled out: a test-at-top `while (n > 0)` with the null test as a `break` (8
of 13 words match); the same with `--n`; a `for` with the count read in the
init (2 of 13); `chip != NULL && n > 0` as the guard and a `do`/`while` on
`--n > 0` (6 of 15); `while (1)` with two `goto`s out (6 of 15); the null
test as a `while (chip != NULL)` condition with the advance in the `for`
increment (6 of 15); an early `return NULL` per exit instead of the shared
one, which runs past the next function; and both `goto` forms of the
rotated loop, with the bottom test written `if (n > 0)` and `if (--n <= 0)`,
which are the draft at 6 of 14.
