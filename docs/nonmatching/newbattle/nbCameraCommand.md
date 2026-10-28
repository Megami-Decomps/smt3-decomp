# Non-matching drafts: newbattle/nbCameraCommand

## nbCameraState_CommandRoot_Init (newbattle/nbCameraCommand, 0xD0)

The draft is 4 bytes over retail. The branch after the player-formation
request flag load leaves a nop where retail copies the retained main-process
pointer into the first argument register. The calls and the final order-index
store follow retail. The unresolved question is which ordinary local camera
and process-pointer lifetimes preserve that delay-slot copy.

## nbCameraState_CommandRoot_Calc (newbattle/nbCameraCommand, 0x80)

1 of 32 words differ: the saved-order equality branch becomes `beql` instead
of retail `beq` while Root_Init is an assembly fallback. Exposing Root_Init's
C draft changes the callee visibility and removes this residual, but Root_Init
still differs, so both remain fallbacks. Preserve the shared unit context when
investigating the branch; a single-function match is insufficient.

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
