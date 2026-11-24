# Matching notes: newbattle/nbMainProcess

## nbMainProcess1 (newbattle/nbMainProcess, 0x420)

The complete dispatcher matches all 264 words at 0x00248E38 and all 16
native jump-table entries at 0x005434D0. Its initial new-phase check, phase
dispatch, successful phase advance, and final debug handling use the
existing work-record fields and calls.

The EVENT press transition and CHKEND's no-end-type transition share a
single call site with different real phase arguments. The successful
transitions share CHKTURN's advance flag without merging the separate ATO
calls. The CHKEND action-stack path still calls CHKTURN; its non-stack
path reads the actual unsigned end type. An ACTION end type advances
without calling the phase setter again. The RENZOKU exit calls
`nbCountActiveUnits(void)` using its existing definition's actual
no-argument interface.

The EVENT-only ATO call names the existing `nbSetNextPhase` symbol
through a conservative `void(s32)` declaration. Its argument, order and
callee do not change. This withholds the local definition only at that
call site; CHKEND's ATO call stays ordinary. The earlier 1,056-byte
control with CHKEND's ATO call opaque missed only +0x260: `beq` in place of
retail's `beql`, with the same target and `next = 1` delay slot. In parity-
checked diagnostics the exact candidate's pre-assembler delay-branch
output carries the required annul flag; a visible CHKEND call's RTL retains
the ordinary provider metadata that the opaque call lacks. A both-visible
control also merged the ATO call tails and changed the function extent. The
native branch delays the update
to the taken path; the non-taken path makes the ATO call and then sets
the same flag before the common phase tail. This is a matching choice,
not a claim about the original source declaration.

## DebugDisp2 (newbattle/nbMainProcess, 0x3DC)

247 of 247 words match at the native 988-byte extent. The formation row
advances through sixteen entries. The separate real offsets retain the
native first-status, active-entry attribute and filter reads; each row
starts from the current formation pointer. The fallback palette entry
is initialized once before the countdown. Its input-only localized
operand keeps the initializer in the native preheader. The value is
used for the actual glyph attribute, not for a dummy dependency.

The order row reads twelve signed entries in order. It reloads the
selected index for each glyph, then reads the optional event process after
both rows. The packet is linked in native call order and handed to the
then-current registration callback. The reconstruction keeps the
original EE address widths and real provider interfaces.

Five diagnostic strings move from the retired assembly body into constant
arrays at the original offsets and extents. Their bytes and padding, and
the full native .rodata section, are unchanged.
