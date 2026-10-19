# Two cross-block locals whose registers come out swapped


Where a value is tested in one block and updated in another, read **both** of
the values the second block needs at the second block's label, and test the
global directly at the first. `dds3KernelMain` tests `dds3PauseSign` and then
decrements `dds3PauseTime`; with `sign = dds3PauseSign;` at the test and
`time = dds3PauseTime;` at the `goto`, the two land in `$2` and `$3` the wrong
way round and the `movz`, both stores and the two delay-slot loads differ, 10 of
72 words. Reading `dds3PauseSign != 0` as the global and assigning both locals
at the `pause:` label is exact. Declaration order, the store order, `u32` for
the counter, `>= 1` for `> 0`, `time = time - 1`, and swapping which global is
read first in the test block all leave the pair swapped, so move the reads
rather than permuting around them.
