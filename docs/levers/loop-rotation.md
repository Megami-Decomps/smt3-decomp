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
