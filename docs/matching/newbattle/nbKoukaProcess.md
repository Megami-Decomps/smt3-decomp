# Matching notes: newbattle/nbKoukaProcess

## RunNumEffectKouka (newbattle/nbKoukaProcess, 0x144)

81 of 81 words match in `3e13612e`: 324 bytes plus four bytes of alignment
fill, complete owner 57 match / 0 differ with data checks. The actual published
commit rebuilt the exact retail image; a second `ninja` reported no work, with
a clean tracked tree and validated object and link inputs.

The ordinary reconstruction matched 73/81 words. Its remaining eight
words were initial constant/party-access scheduling: the compiler delayed the
forward Z store into the inactive-guard branch delay slot. One local volatile
empty `"m"` input operand on initialized `forward.z`, immediately after loading
`k->data`, retains the native schedule without adding instructions. Its real
consumer is the later VU load for `dds3RotationArc`; the operand is not a general
memory barrier. This is an SMT3 reconstruction aid, not original-source evidence.
Qualified stock diagnostic dumps preserved production code/data output.

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

A ROT/HOMEROT switch preserves the native dispatch. Existing NOMEM quaternion
store macros retain the native final output-address rematerialization, and the
party-index pointer expression retains its native operand order. Removing only
the empty operand from the final owner returns the 73/81-word ordinary result;
placing it before the `k->data` load gives 79/81, exchanging that load and the Z
store. Tied memory/float constraints regress and are not retained. The final
source has no register binding, synthetic arithmetic, new call or runtime memory
access. All 26 regression scripts, normal-draft and IRX checks passed before
publication.
