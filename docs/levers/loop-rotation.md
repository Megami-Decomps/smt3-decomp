# A loop whose exit test is written first gets rotated; plain `goto`s do not


`for (;;) { if (flag != 0) break; <body> }` is not laid out the way it reads.
ee-gcc moves the test block to the end of the loop as the latch and jumps to it
from the preheader, so the body comes out first and the test last. Retail keeps
the test at the head of its block with the body falling through behind it.
Writing the whole loop as labels and `goto`s, with no loop construct at all,
keeps the source order: `dds3KernelMain` has a rotated loop whose backedge runs
from the frame-end test to the bookkeeping block, and
`goto top; frame: ... goto runcount; pause: ... runcount: ... top: if (flag)
goto done; ... if (f()) goto frame; done:` reproduces the layout exactly, where
the same structure inside a `for (;;)` with `break` is 64 of 71 words out. A
`do`/`while` or a `for` with `goto done` in place of the `break` both work as
well, so it is the `break` reaching the loop's own exit machinery that rotates
it.

## `i < K` reverses the loop, `i != K` does not

A counted loop whose bound is a literal gets reversed into a countdown when the
counter is dead outside the loop and every use is strength-reduced away:
`for (k = 0; k < 16; k++)` comes out as `li $10, 15` / `addiu $10, $10, -1`.
Writing the same bound as `k != 0x10` blocks `check_dbra_loop` and keeps
retail's ascending counter compared against a register holding the bound.
`sdfPadWork`'s inner loop over the 16 button masks is 78 of 89 words out with
`<` and matches with `!=`; its outer loop over the two pads already used `!=`
and never reversed.

`newbattle/nbCommSelProcess::DispPanelLabel` is another counted-loop case:
its plain `for (i = 0; i < count; i++)` supplies the native single entry guard.
An extra `if (count > 0)` around that loop survives lowering and adds eight
bytes. Initialize the count and rank when entering the scan phase, after the
animated margin calculation. A qualified `cc1probe --reorg --no-dumps` trace
selects the shared count clear (uid65) for the type branch's delay slot
(uid46), reproducing the native prefix. Plain and hooked output agree; the
complete owner matches with the 788-byte function and its six-entry switch.
