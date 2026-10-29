# Current action status classification

`newbattle/nbActionProcess::SetNowActionStat` at `0x00252FB8` is 752 bytes.
This is a native SMT3 reconstruction, not a byte-identical DDS transfer. Names
and field meanings were cross-checked against
[smt3-reconstructed-sources](https://github.com/smt-ret/smt3-reconstructed-sources),
revision `cdcd3e5874d72e63afae6e771e3b2d8f85cf66b1`,
`src/nbActionProcess.c::SetNowActionStat`. That HD/transpiler body is semantic
reference only: its flattened pointer-to-pointer argument and bitmask tests
are not copied. The native caller, records and dispatch tables govern this C.

## Native contracts

The existing `nbActionProcessData_t` directly owns the unsigned status word
at +4 and signed party index at +0x30. The native caller `SetAction_SKILL` at
`0x00255890–0x002558A0` passes that record, the command and the skill number
in a0/a1/a2. This function makes no provider calls. A party index of four or
more returns without any table access or modification; negative indices
retain the native signed-comparison behavior.

The existing `datSkillTable.normal` records are 60 bytes: `koukatype` is u8
at +2, `hptype`/`mptype` are u16 at +0x16/+0x1A, `badtype` is u8 at +0x24 and
`program` is u32 at +0x30. These are canonical `include/newdata.h` fields;
no local view, alias or replacement record is introduced. The caller supplies
normal skill indices from the game data, with the native zero/default and
command-six skill 255 cases. Neither retail nor this reconstruction adds a
new bounds check: valid indexing of the existing `normal[288]` array remains
a caller/data precondition.

Each HP/MP loop performs both existing field reads in order. Types
1, 3, 4, 6, 8, 10, 12, 13 and 14 set the relevant attack bit; types
2, 5, 7, 9, 11 and 15 set 0x40000. Out-of-range types do nothing. The native
14-entry tables are at `0x00543920`, `0x00543960` and `0x005439A0`; their
first type is respectively 1, 1 and 2. Each has eight bytes of zero alignment
padding. The switches generate these tables; there are no copied pointer
tables, labels-as-values or hand-authored instruction words.

Command zero sets 0x10000. For command one, effect kind zero selects the
0x10000 HP/MP classification; kind one selects 0x20000 and also sets it for
bad types one or three. The unconditional HP/MP classification sets 0x40000
for every command. Command one/kind one additionally sets 0x80000; command
five sets 0x100000. Finally, command six or command one with program two or
three sets 0x200000. Every update is OR-only and preserves other status bits.

## Final dispatch shape

The final non-skill-command arm precedes the skill-program arm, just as in
retail at `0x0025324C`. The two program cases naturally produce unsigned
upper-four and lower-two tests, then join the command-six status update.
Writing the terminal command dispatch as a switch instead emits an extra
local return, changing five branch/delay-slot words while retaining the
752-byte size. This is a control-flow distinction, not a register constraint
or a fake dependency. Ordinary RTL for the final source shows one return
in `27.jump2`; the normal native branch structure survives delay-slot filling.
The earlier explicit no-op program cases zero/one were removed rather than
preserving artificial cases; only actual updating program cases remain.

## Qualification

Source intake baseline: public `8d8c77514e25b620767d834d47914f4004be41a3`.
The patch replaces only this function's ASM fallback and leaves all unrelated
functions and drafts, including `SetAllPacket`, unchanged.

- Complete owner: **79 matches, 0 differences**; all five retained drafts
  compile.
- All three 14-entry jump tables resolve exactly, including their alignment
  bytes; the linked 64-byte spans were also compared directly with retail.
- Full retail image SHA-1: `5facfbf7916836613449c922e26a4ec412207b32`.
- Plain and `-da` whole-owner assembly agree after normalizing only the
  randomized first `.file` line: SHA-256
  `469ad45add2b8f7694987f7e4f7071a83627192bc9429eff302034d0e752aac7`.

The target-only RTL is an ordinary compiler-output receipt, not a hooked
GDB/cc1probe trace. No new helper, inline assembly, symbol alias, shared
header, compiler option or special allocation premise is involved.
