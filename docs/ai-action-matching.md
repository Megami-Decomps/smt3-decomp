# AI action selection

`nbRunAiSub` matches all 372 instruction bytes. It clears the five action
check bytes, picks and logs each candidate, and retries rejected candidates.
Cost failure skips the close check. A separate rejection state survives the
close check; only accepted candidates read the target entry and call
`nbSetAiTarget`. Selection publication follows the loop: a successful action
uses `nbGetSelectBit`, while exhaustion clears selection and sets command 3
and index 0. Work fields remain fresh across callbacks.

The table contains 164-byte unit rows, four 40-byte action tables and eight-byte
entries. Combine the unit, table and action byte offsets before adding the
base, then read the typed entry's target word. Its high and low halves are
the target provider's second and third arguments. Naming the table and action
offsets before the rejection test retains their native registers and schedule.

The older short-circuit draft emitted 356 bytes with 6 matching words. Explicit
rejection state and the shared loop exit recover the native size and 85 of
93 matching words. Combined byte offsets recover 89; the separate offset
locals close the final four differences. A normal for loop retains the same
match as a condition containing the pick/log sequence. Qualified allocation
instrumentation preserves normal, dumped and instrumented assembly and all
intermediate representations. The complete owner and its data remain exact.
