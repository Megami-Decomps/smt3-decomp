# Non-matching drafts: interface/lstListWindow

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## lstDrawListWindow (interface/lstListWindow, 0x84)

26 of 33 words match, at the exact 132-byte size.

Every instruction is retail's. Retail schedules the `sra $16, $16, 24` that
finishes the `s8` parameter's sign-extension before the first `jal`, which
leaves `daddu $5, $17, $0` as the last insn before the second and third calls
and so as their delay slots. The draft sinks the `sra` past the first call, it
takes the second call's delay slot, and `move $5` moves up between the `lui`
and `ori` of the sprite id; three swapped pairs, same size.

Pinned down on the way:

- `lstDrawUpDownCursor` is called with two arguments here. Retail fills the
  `bnez` delay slot with `daddu $5, $17, $0`, which is dead on the taken path,
  so the second argument exists and is the ot number. Its definition takes the
  extra parameter and still matches.
- `fclInitPanelPos` takes `u16` width and height, not `s16`: the call site
  masks with `andi`, it does not sign-extend.
- `fclInitPanel` takes nine arguments, the ninth on the stack, and both the
  eighth and ninth are `s8`.

Ruled out for the sign-extension: an `s32` parameter with an `(s8)` cast in the
body, an `s8` or `s32` local assigned from the parameter before the calls, a
boolean local, and testing the parameter directly at the `if`.
