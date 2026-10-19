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
