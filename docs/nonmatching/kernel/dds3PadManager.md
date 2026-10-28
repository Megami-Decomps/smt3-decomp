# Non-matching drafts: kernel/dds3PadManager

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3GetVibrationLevel (kernel/dds3PadManager, 0x74)

27 of 29 words match, at the exact 116-byte size. The ramp itself is exact:
the large motor's current level steps toward `dds3PadVibrationMode[1]` by
0x20 per call, snapping to the target once the gap is 0x20 or less, with the
equal case storing the target as well (the three stores retail crossjumps
into two branch-likely delay slots and one fallthrough `sb`). The downward
step is written `+ 0xE0`, not `- 0x20`: retail's `addiu $3, $3, 0xE0` is the
wrapped byte constant, and the signed spelling costs that word.

What is left is the two words of the `printf` tail. Retail loads the level
with `lbu $5, %gp_rel(D_00551B91)($28)` and so schedules the format string's
`lui` first; the draft still has the address of `dds3PadVibrationState[1]` in
`$5` from the ramp and loads `lbu $5, 0($5)`, which takes the earlier slot
and pushes the `lui` behind it. The addressing choice is a register
allocation artifact, not a value: the pointer is live across the whole ramp
in both.

Ruled out:

- Reading the level into a local, casting both arguments to `s32`, and
  `*(volatile u8 *)&dds3PadVibrationState[1]` at the call. All three keep the
  `0($5)` form, so the reuse is gcc's address selection and not value CSE.
- A `u8 *level = &dds3PadVibrationState[1];` local used for every compare and
  store, with the array spelling kept only at the `printf`. Identical output.
- `s32 target` instead of `u8 target` (2 of 29) and hoisting the format
  string into a `const char *` local (4 of 29). Both much worse.
- An unprototyped `extern int printf();`. No change.
