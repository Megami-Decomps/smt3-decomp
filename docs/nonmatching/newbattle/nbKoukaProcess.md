# Non-matching drafts: newbattle/nbKoukaProcess

## NumEffectClip (newbattle/nbKoukaProcess, 0x40C)

The draft emits 1044 bytes against the native 1036, with 62 of the 259 native
instruction words matching and eight excess bytes. A scratch promotion leaves
the target and its data ownership unmatched; the normal owner retains all 55
C matches through the assembly fallback.

The input and output position travel in `vf10`. The initial projected point is
accepted for signed pixel X in [13,500) and Y in [75,324), and returns the
preserved incoming vector. The fallback starts with the packet formation's
sphere center. Skill IDs 0x107, 0x108, 0x10B or 0x10D with invoke-model ID 0xC0
raise its Y by `h * scl * 0.35f`. Skill 0x9E instead checks targets for model ID
0x158. Keep fresh `packet->form` reads after provider callbacks.

The other path projects model marks 10 and 0, using the sphere center when a
mark query fails. Convert successful screen arrays in place with signed shifts
4 and 3. Failed mark projections become pixel sentinels (512,448), with metric
zero. The two rounded screen-distance results occupy a real two-element array;
this reproduces the native 192-byte frame and its +0x70/+0x74 stores. Using two
register scalars instead emits a 176-byte frame.

Choose mark 10 only when its screen metric is smaller than mark 0's and its
pixel position is valid. Otherwise choose a valid mark 0. If neither qualifies,
compare the 3D distance from mark 0 to the saved sphere center: ordered distance
below 1000 lowers center Y by `h * scl * 0.35f`; all other results choose mark
10. The six diagnostics have separate padded data symbols. Keep their full
retail extents when promoting; external references alone leave the promoted
unit's rodata layout wrong. No string bytes are moved by the guarded draft.

Native also reads the initial screen words after a failed projection, although
`nbClipEffectNum` can return without writing them. The draft preserves that
uninitialized-stack behavior rather than silently initializing the array.
The remaining code-generation questions are saved-register roles and branch /
epilogue structure. A common outer guard improves overlap over an early-return
form, but does not close the function. No padding or artificial uses are added.


## RunNumEffectKouka (newbattle/nbKoukaProcess, 0x144)

In the 320-byte emitted prefix, 57 of 80 words match, against 324 native bytes
(81 words). Four bytes remain uncovered by positional comparison; this is not
a one-instruction-only residual. Initial constant/party access scheduling, party
address registers and the final output/call setup still differ. The complete
default owner retains all 55 C matches; assembly remains active.

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
