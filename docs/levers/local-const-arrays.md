# Function-local constant arrays


A block-scope initializer such as `s32 list[3] = {1, 11, 14};` goes to
`.rodata` at the function's position in the C file, not where splat put the
`INCLUDE_RODATA` line for it. Move the `INCLUDE_RODATA` lines for every
rodata object that precedes it above the function, or the relink reorders the
pool. `check_unit.py` masks local rodata relocations and still says the
function matches; only the `ninja` checksum catches this.
