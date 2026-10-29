# Name-entry cursor dispatch matching

`nmeMoveCursor` and `nmeMoveCursor2` match 516 and 372 game EE instruction
bytes. The first dispatcher handles character editing and can return one for
confirmation; the alternate dispatcher changes cursor focus. Both retain the
native button priority, callback-visible work reloads and common pad/sound tail.

Two recovered call contracts close the former broad residuals:

- Both native dispatchers pass the signed mode value in the word argument to
  `nmeSyncCursor`. The helper ignores that incoming value and derives its row
  from the primary cursor. Its existing 52-byte body remains exact with an
  unused `s32 mode` formal. No artificial use of that argument is added, and
  the caller's byte sign extension does not establish a byte formal.
- `cmpResetCursorPos` accepts the word-valued row and narrows when storing to
  the signed-halfword cursor field. Its 20-byte native body is just that store
  and three byte clears. A halfword destination does not establish a halfword
  formal. The two movement callers pass `nmeGetRowCount(mode)-1` directly,
  without an intermediate halfword normalization. The definition and both
  name-entry declarations now consistently describe that word boundary.

The row-count getter remains `s32`. Its bounded 0/15/16 results do not establish
an original byte return type. The existing halfword reset declaration inserted
two conversion instructions in each movement caller; the corrected word
contract removes them while preserving the provider's exact destination writes.
The sync parameter alone left the first dispatcher 524/516 bytes; correcting
the reset boundary makes both dispatchers exact with no other body changes.

Complete owners including data: name drawing 20 match/0 differ, name
initialization 20/0 and the reset-provider owner 369/0. Existing provider bodies
receive no additional matching credit. Independent native/provider review
covers both dispatchers and the stored-field contracts.
