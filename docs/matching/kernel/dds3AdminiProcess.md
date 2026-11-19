# Matching notes: kernel/dds3AdminiProcess

The five registry routines below now match exactly after distinguishing the
actual no-argument cleanup callback from the scheduler update callback.
The row keeps the native layout and unchanged four-byte callback storage.
The registration wrappers retain their existing exported types and convert
the cleanup pointer only when storing it. Native registrants pass
`fclEncycDebugShutdown` and `pbDebugShutdown` for cleanup. No consumer,
required data representation, callback invocation, or field-store
statement was added or changed. The former common function-pointer type
recreated the shared scheduling differences; the corrected representation
matches without constraints.

## fclEncycFindFreeSlot (kernel/dds3AdminiProcess, 0x1C4)

**Matched: all 113 words / 452 bytes.** In the historical draft, the
two differing words were the `task` and `update` zeroing stores swapped:
retail writes `sw $0, 0($6)` (offset 0x24) second and `sw $0, 0($3)` (0x28)
fourth, the draft the other way round. Both address temps are computed by the
same two `addu` in the same order and get the same registers, so this is the
scheduler's tie-break between two `sw zero` whose addresses become ready
together, not anything the field order states. `fclEncycFreeEntries` in the
same unit stops on the identical pair, which is evidence the two share one
cause.

A current-unit `cc1probe --sched --alloc --no-dumps` run, with
`-DNON_MATCHING` and function filtering, passes hooked/unhooked output parity
(`verify: OK`). In sched2, task store UID245 and update store UID266 are
both ready with priority 1, register weight -1 and cost 2. The dependent-count
comparison chooses update first: two dependents versus task's one. The cause
of that dependency-graph asymmetry remains unresolved; it is not evidence for
adding an artificial source dependency. A single block clear of the six
contiguous runtime fields instead emits 428 bytes and changes the frame,
so it does not close this residual.

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

**Matched: all 122 words / 488 bytes.** The historical draft swapped two
zeroing stores, as with `fclEncycFindFreeSlot`.

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

## fclEncycConfigureMode3, fclEncycAllocEntry and fclEncycConfigureMode2 (kernel/dds3AdminiProcess, 0x94, 0xA8, 0xAC)

**Matched: all 37, 42 and 43 words / 148, 168 and 172 bytes.**
The three slot-registration wrappers differ in the `kind` constant and
which callback payload they store. Their historical drafts shared the same
scheduling defect: two address temps for the row are exchanged, which swaps the order of the
`destroy` and `userValue` stores. In `fclEncycAllocEntry` and `fclEncycConfigureMode2` the
three incoming function-pointer arguments are also rotated between `$16`,
`$17` and `$18`: retail takes them in parameter order, the drafts one place
around.

Store order is the lever that got them here and does not finish them.
Ascending field offset, `kind`, `task`, `update`, `destroy`, `setState`,
`enabled`, `userValue`, is what the drafts use; the obvious order (`kind` then
the argument store, then the zeroes) is 23 of 37 and 29 of 42 words.
`fclEncycAllocEntry` was swept exhaustively over all 5040 orderings of its
seven stores and caps at 11; `fclEncycConfigureMode3` was swept over 5040 and caps at 7.

Ruled out: splitting the declaration from the `fclEncycFindFreeSlot` call, a
`ret` local with `if (slot >= 0)` in place of the early `return 0`, a dead
self-store to raise a reference count, and parking
`fclEncycFindFreeSlot`'s draft so the callee is only an `extern`.
