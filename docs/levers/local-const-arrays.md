# Function-local constant arrays


A block-scope initializer such as `s32 list[3] = {1, 11, 14};` goes to
`.rodata` at the function's position in the C file, not where splat put the
`INCLUDE_RODATA` line for it. Move the `INCLUDE_RODATA` lines for every
rodata object that precedes it above the function, or the relink reorders the
pool. `check_unit.py` masks local rodata relocations and still says the
function matches; only the `ninja` checksum catches this.

A `char names[7][0x20] = {...}` whose initializer is too wide to fold expands
to a runtime alignment test (`andi reg, 7`) in front of two copy loops, an
`ldl`/`ldr` one and an `ld`/`sd` one. That is just the block move for a
4-aligned `char` destination, so it reproduces on its own.

The registers that copy picks are set by **declaration order**, because an
initialised local's initializer is emitted where it is declared. A local array
with an initializer declared first takes `$a0` as the copy destination, which
clobbers the incoming argument and forces every use of it to a callee-saved
register. Declaring the pointer the function needs from its argument *before*
the array (`pbBlockDB_t *db = h->data.blockDB;` ahead of the `names`
initializer) lets retail's `lw $21, 0xC($4)` happen in the prologue and leaves
the copy using `$3` and `$4`. In `pbDumpBlockData` that one reordering went
from 48 of 115 words to 99.

A 40-byte copy of a pointer table out of `.rodata` is the same shape without
the alignment test: `memcpy(local, D_XXXXXXXX, sizeof(local))` over a
`char *local[10]` gives retail's six `ldl`/`ldr` plus `sdl`/`sdr` pairs
directly, and keeps the `INCLUDE_RODATA` line, which an initializer here would
have to replace. `nbDrawMakaNum` needs that form.
