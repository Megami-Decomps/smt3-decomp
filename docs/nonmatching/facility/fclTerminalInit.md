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

## trmStartEvent (facility/fclTerminalInit, 0x84)

The mode switch emits the exact 132 bytes: 25 of 33 words match.
Mode 0 requests an event with `trmGetEventNo(1, GBWK_3)`: a negative event
returns zero; otherwise it starts the event, sets terminal mode 1 and returns
one. Modes 1 and 5 return one without mutation; every other mode returns zero.
The provider returns a signed byte from `trmGetMode`, a signed word from
`trmGetEventNo`, and no value from `evtStartEvent` or `fclSetTerminalMode`.

The eight residual words occupy three windows: `+0x24`, `+0x30` and `+0x38`
change the branch-likely choice and hoist the terminal-work load into the
mode dispatch; `+0x50` through `+0x5C` rotate that load, the event request
and the argument move around its negative-result guard; `+0x68` fills the
event-start delay slot differently. The calls and mode/event tests are
accounted for; the remaining source/control-flow premise is unresolved.

An if/else mode dispatch emitted 120 bytes; only 4 of 30 words match.
A switch returning through a shared result local emitted 152 bytes and added
a saved register. Neither is a closer reconstruction. The explicit switch
with early returns is the preferred draft; production retains assembly.
