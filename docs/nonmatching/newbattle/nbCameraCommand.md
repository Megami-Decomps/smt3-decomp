# Non-matching drafts: newbattle/nbCameraCommand

## nbCameraState_CommandTargetSel_Init (newbattle/nbCameraCommand, 0x11C)

The draft has the exact 284-byte size; the summon/party dispatch branches
still differ. Retail scans target formation flags, then selects summon, all
parties, the invoking party, or the fixed-position sequence. The fixed-position
arm calls ground collision before the KORONZ check, and every arm reaches a
second common ground-collision tail. Both calls are retained in the draft.
The unresolved question is the ordinary dispatch shape and value lifetimes
that preserve retail's branch layout without changing either collision call.

The target-selection field at main-process offset 0x80 is a scalar word:
this routine reads it with `lw`, and SetTarget_nbActionProcess writes it with
`sw`. Its shared declaration and those writers now use `s32` directly.
