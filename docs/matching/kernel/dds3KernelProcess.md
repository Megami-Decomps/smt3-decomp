# Matching notes: kernel/dds3KernelProcess

## dds3DrawFirstProcess (kernel/dds3KernelProcess, 0x58C)

**Reference implementation, not a match candidate.** Assembly remains production.

The guarded C records the render-texture, secondary-view and scene passes using
`dds3DrawBank_t`, the existing light/fog records and the VU0 vector/matrix macros.
It preserves the second render-texture query after the lighting callback, the
600-unit target construction and quake offset, the camera packet handoff, each
fog packet's lifetime, and the final default-light/default-fog installation.

The model ordering-table slots are initialized in native order: 0, field tables,
7, 8, 1 through 5, 9 and 10. The three Z-clear tags reach ordering tables 69,
80 and 82 through their freshly loaded callbacks. Frame selection, provider
signatures, draw-bank offsets and the two aligned VU-only local vectors were
checked against the native body and the matched providers.

The emitted address calculations and register lifetimes differ substantially.
Separating the three real packet-pair lifetimes restores the native frame size,
but does not reproduce the body. A matching follow-up needs evidence for the
native phase-local bank/address lifetimes; this source is retained to explain
the rendering flow rather than to suggest that the remaining difference is small.
