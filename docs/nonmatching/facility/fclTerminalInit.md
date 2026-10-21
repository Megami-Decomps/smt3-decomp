# Non-matching drafts: fclTerminalInit

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## fclTerminalLoadTextures (fclTerminalInit, 0x70)

16 of 27 words match, 108 emitted bytes against retail's 112. Loads the
terminal's one texture from the name table at `D_00330808` and prints
`D_0053D5E8`.

Everything through the store matches: the three saved registers, the counter
in `$16`, the texture slot pointer walking `$17` by 4, the table base in
`$18`, the indexed `sll`/`addu`/`lw` of the name and the `sw` of the handle.
The one missing word is the counter's own normalization. Retail writes
`addiu $3, $16, 1`, `sll $3, $3, 24`, `sra $16, $3, 24`, so the counter is a
signed byte; a `u8 i` gives `andi $16, $3, 0xFF`, one word short.

The two halves pull against each other, which is the finding worth keeping: a
**signed** byte counter is a biv ee-gcc's strength reduction recognises, so it
also reduces `D_00330808[i]` into a second walking pointer and the whole body
goes to 8 of 27 words match with the registers rotated. A `u8` counter is set
by an `and`, which is not a biv form, so the table stays indexed exactly as
retail has it. Every signed spelling tried reduces the table and every
unsigned one gets the `andi`.

Ruled out, all 19 of 27 words differing with the table reduced: `s8` and plain
`char` counters with the bound written `i < 1U`, `i < 1`, `i == 0` and
`(u8)i < 1U`; the name read as `*(char **)((u8 *)D_00330808 + i * 4)`, as
`D_00330808[(u32)i]` and through a `char **name = D_00330808;` local; the slot
advance written `tex++` as its own statement, in the `for` increment, and as
`*tex++ =`; and an `extern s32 fclLoadTexture(char *name);` prototype above the
function. Also ruled out: both sides indexed (`tex[i] = ...D_00330808[i]`),
which spends a fourth saved register on the shared `i * 4`; and
`D_00330808[(u8)i]` with a signed counter, which is 10 of 28 words match and
one word long.

## fclTerminalInitMenu (fclTerminalInit, 0x7C)

27 of 31 words match, at the exact 124-byte size, with zero instruction
differences. Picks the terminal menu's text table, with the second group of
four entries when the current event is 0x296, hands it to the cursor window,
and sets the command bit and the work's enable byte.

The four differing words are the two registers of the `fclTerminalCommandCount`
read-modify-write: retail holds the `$28`-relative address in `$2` and the byte
in `$3`, this holds the address in `$3` and the byte in `$2`. All four remaining
words are a register name.

Ruled out: writing the work's `+0x25` byte before the command bit, which costs
four more words; the command bit spelled `x = x | 2` instead of `x |= 2`; a `u8
*` work parameter with `work[0x25]`; an `s8 *` local for the work byte, which
adds 12 bytes; and testing `!= 0x296` with the arms exchanged, which reaches
only 24 matching.
