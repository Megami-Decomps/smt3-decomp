# nbSetMaeFlag

`nbSetMaeFlag` (`0x0028D9E0`, 880 bytes) selects battle MIDI state and starts
loading the main MIDI resource when it is not already loaded.

## Source structure

The native prologue copies three constant tables to automatic storage:
seven signed words at stack offset zero, eight at `+0x20`, and eight at
`+0x40`. The four bytes after the first table are alignment, not another
entry. Three ordinary initialized arrays reproduce these copies and their
read-only data. Direct `nbResultCount % 7` / `% 8` subscripts preserve the
native scheduling; separate early remainder temporaries do not.

The counter is initialized to zero. Its sole identified writer is
`nbIncResultCounter`, which increments the signed word. There is no overflow
cap. The source retains signed remainder and the native table bounds; it
does not invent defined values for negative indices reached after signed
counter overflow. The prior draft's leading zero padding and merged table
were not the native automatic objects.

All fifteen selector cases retain their main/sub MIDI, saved BGM and fade
state assignments. The nonzero flag early return, selector-provider choice,
load-done reset, out-of-range selector behavior and load-check/file-load
branches remain unchanged. Provider signatures agree with `nbEncount.c`.

## Verification

The complete `nbSound.c` owner reports **26 match, 0 differ**, including the
880-byte function and its replacement constant data. No new inline assembly,
register constraint, compiler flag or scheduling-only dependency is used.

A host state/call harness passed 7,888 scenarios under AddressSanitizer and
UndefinedBehaviorSanitizer: selectors 0 through 16, all 56 combined remainder
classes plus `INT_MAX-1` and `INT_MAX`, both flag states, both provider paths,
and both load-check outcomes. Negative-count behavior is not claimed as a
defined C input domain. Leak detection was disabled because the execution
host does not support LeakSanitizer under its tracing setup.
