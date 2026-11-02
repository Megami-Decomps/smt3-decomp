# Matching notes: fclTerminalInit

## fclTerminalLoadTextures (fclTerminalInit, 0x70)

28 of 28 words match at the native 112-byte size. A signed-byte counter plus
one localized `FAKEMATCH_ASM_LIVE(i)` immediately before its real table-index
consumer preserves the indexed filename load and signed counter normalization.
The constraint emits no instruction and is a target-specific reconstruction
aid, not a claim about original source spelling. The counter is not bound to
a fixed register and no artificial call-crossing lifetime is introduced.

The counter starts at zero, loads the sole filename, increments to one and
exits. The slot pointer advances by four after the handle store. The native
call, word store, signed-byte update and final diagnostic tail call are
preserved. No volatile memory access, provider/type change, dummy computation,
or additional executed memory access is introduced.

Fresh complete-owner checks give 31 match / 0 differ, including owned data;
no guarded function remains. Plain and `-da` builds have identical allocated
sections, relocation sections and assembly after removing only the generated
source-path directive. Paired ablations: unsigned counter without the constraint
is 16/27 at 108 bytes; signed counter without it is 8/27 at 108 bytes, with
filename indexing strength-reduced to a second walking pointer.

### Historical ordinary-source residual (superseded)

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
