# Matching notes: newbattle/nbKoukaProcess

## RunNumEffectKouka (newbattle/nbKoukaProcess, 0x144)

In the 320-byte emitted prefix, 57 of 80 words match, against 324 native bytes
(81 words). Four bytes remain uncovered by positional comparison; this is not
a one-instruction-only residual. Initial constant/party access scheduling, party
address registers and the final output/call setup still differ. The complete
default owner retains all 56 C matches; assembly remains active.

The source zeroes a vector at stack+0 and sets its Z to -1 before selecting the
packet's party. An inactive party returns. The signed formation index selects
a 0xF0-byte record. ROT sets bit0x20 in `stat` at +0x78 and copies the packet's
quadword vector to stack+0x10. HOMEROT clears that same status bit, loads orgrot,
builds the matrix and transforms the VF0 vector with Z set to +1. Do not update
the neighboring `flag` word at +0x7C; the older off-build draft used that wrong
member. Other packet types still reach the common arc path with the destination
stack slot untouched, matching native behavior.

The shared path loads the -Z source and destination into VF10/VF11, calls
`dds3RotationArc`, stores the result and calls `evtSetUnitRot1`. Keep the real
no-argument VU helper declarations and the rotation provider's three-argument
void signature. Load `form->hunit` and the packet's signed parameter after the
arc call. The existing quadword-copy and identity-vector macros describe the
native operations; an aggregate C copy emits a different transfer.

A ROT/HOMEROT switch recovers the native three-way dispatch, improving the
corrected macro-only form from 308 to 320 bytes. A memory-input VU load confirms
the initial Z value is consumed. It was already present in the plain-load
object: the scheduling mismatch is not dead-store removal. A volatile input
vector regresses to 328 bytes and is not retained.
