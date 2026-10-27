# A second label in the split assembly: which kind matters


A `.s` file holding a second **code** symbol (`glabel` or `alabel`) is a
boundary problem, not a matching one: writing C for the named function drops
the other's bytes, so leave it and record it. A `.s` holding a second **data**
symbol (`dlabel`) is ordinary. The data does disappear when the C replaces the
function, and `check_unit` only says `NOASM`, so add an
`INCLUDE_RODATA(const s32, "<unit>", D_XXXXXXXX);` in address order and run
`configure.py --force-split` again. A `jtbl_` label needs nothing: the C
`switch` emits its own table.

`check_unit` will not tell you which kind you have. Grep the `.s` for a second
label before concluding anything.

An `alabel` is the exception worth checking first. When the second label is an
`alabel` (spimdisasm's auto label, no symbol behind it) and `symbol_addrs.txt`
gives the *named* function a size covering both blocks, there is one symbol and
two code blocks, so two C functions in the unit reproduce it: write the named
one first, declare the second above it under its `func_<vram>` name and call it.
gcc lays them out in source order, the tail call comes out as the `j` retail
has, and the extra global symbol changes no bytes. `sdf/sdfMisc`'s
`sdfItemArrayGrow` is 0x148 with `alabel func_002BFF98` at 0x80 into it, and
both blocks are exact that way. Only a second `glabel`, or an `alabel` whose
bytes the named symbol's size does not cover, is a real boundary problem.
