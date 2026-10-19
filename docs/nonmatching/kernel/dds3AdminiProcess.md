# Non-matching drafts: kernel/dds3AdminiProcess

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## fclEncycFindFreeSlot (kernel/dds3AdminiProcess, 0x1C4)

111 of 113 words match, at the exact 452-byte size, with zero instruction
differences. Registers, the name copy and the list splice are all exact. The
two differing words are the `task` and `update` zeroing stores swapped:
retail writes `sw $0, 0($6)` (offset 0x24) second and `sw $0, 0($3)` (0x28)
fourth, the draft the other way round. Both address temps are computed by the
same two `addu` in the same order and get the same registers, so this is the
scheduler's tie-break between two `sw zero` whose addresses become ready
together, not anything the field order states. `fclEncycFreeEntries` in the
same unit stops on the identical pair, which is evidence the two share one
cause.

The copy loop needed `while ((table[slot].name[i] = name[i]) != '\0')`: the
assignment as the condition is what gives retail's one `lbu`, `sll 24`,
branch, store-in-delay. A separate `char c` local gives `lb` plus a bare
`beqz` and loses two instructions, a `u8 c` local gives `lbu` plus `beqz`, and
two subscript reads give two loads.

Ruled out: all 5040 orderings of the eight field assignments with `next` last,
swept mechanically; the best is the order in the draft, ascending field
offset, and nothing reaches zero. `task`/`update` swapped alone moves the
`+4`/`+8` base temps instead and costs four words. Moving `previous` out of
seventh place costs 38 to 41 words. The remaining 35280 orderings with `next`
not last were not swept.

## fclEncycFreeEntries (kernel/dds3AdminiProcess, 0x1E8)

120 of 122 words match at the exact 488-byte size. The two differing words are
the `task` and `update` zeroing stores swapped, the same residual
`fclEncycFindFreeSlot` stops on.

What took this from 54 differing words to 2 was writing `kind > 0 && kind < 3`
as nested `if`s. Both spellings give retail's `blezl` then `slti`/`beqz`, so
the branches are no evidence either way, but with the `&&` the register holding
`%lo(fclEncycTable)` and the one holding `slot * 0x44` come out exchanged and
the swap propagates through every address temp and every store in the unlink.
Reading `kind` into a local first changes nothing, and so does writing
`name[0] = '\0'` before `kind = 0`.

The rest is settled: `dds3ExistProcess(table[slot].task) == 1` guarding
`dds3KillProcess(table[fclEncycFree].task, 1)` (retail really reads
`fclEncycFree` there, not `slot`), the eight-field clear in ascending offset
order, the two-sided unlink, `previous`/`next` to -1,
`fclEncycFree = fclEncycHead` when it pointed at this slot, and
`fclEncycCount--`. A `previous` local read before the first unlink arm is
required; testing `table[slot].previous` in the `if` runs the function over
its size.
