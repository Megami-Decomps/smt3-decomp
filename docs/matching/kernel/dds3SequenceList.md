# Matching notes: kernel/dds3SequenceList

## CallNewBattleTest (kernel/dds3SequenceList, 0x4C)

Packed battle request contract audited 2026-11-06.

`CallNewBattleTest` at `0x00109018` receives a pointer to a two-word sequence
request. Its call to `nbCallNewBattle` at `0x002468F8` passes the two loaded
words, not the request address: retail loads argument two from payload `+4`
at `0x0010902C`, then argument one from payload `+0` at `0x00109040`.
The null-payload and restored-sequence paths keep their existing behavior.

The second word is a packed encounter value. The provider masks its low
halfword and extracts the next and highest bytes without dereferencing it.
`nbEncountCalc` constructs the value from condition, selected row and formation
ID; `TestProcess6` passes it directly. `CallBattleSub` and `CallBattleSub2`
likewise place integer `encno` in the second word of their eight-byte request
for sequence `0x13`. The sequence payload pointer is a different value.

The caller declaration now agrees with the existing `nbInit.c` provider and
`nbTest.c` declaration, `(s32, s32)`, and passes `data[1]` directly. Native bit
operations do not distinguish signed from unsigned source spelling, so this
repair preserves the canonical provider's signed type. It removes an
incorrect pointer interpretation, without changing the provider, record
layouts, owner boundaries, compiler options or retail instructions.

Both complete source versions produce identical whole assembly and ELF
objects in production, `SKIP_ASM` and retained-C modes, including all data,
symbols and relocations. Fresh plain/dump and wrapper/direct comparisons pass.
The sequence owner remains 139 match, 0 differ; `nbInit` remains 3/0 and
`nbTest` 14/0. Strict retained-source diagnostics pass for all three owners.

This is a source-contract repair, not new matching credit. The retained
`nbCallNewBattle` draft remains nonmatching; its separate formation-address
and scheduling residuals are not resolved by this declaration correction.
