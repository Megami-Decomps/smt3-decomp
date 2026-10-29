# sdk/libipu

## The IPU hazard wait

Every `IPU_CTRL` poll in this unit pads **four slots between the load and the
branch that uses it**, and no plain C loop emits them. Retail's
`sceIpuSync` wait is

    lw   $v0, 0($v1)
    nop
    nop
    nop
    nop
    bltz $v0, ...

while `sceIpuStopDMA`, which masks before testing, pads only three because the
`andi` fills the fourth slot:

    lw   $v0, 0($a3)
    andi $v0, $v0, 0xF0
    nop
    nop
    nop
    bnez $v0, ...

Under the configured SDK compiler, `ee-gcc 2.9-991111-01`, a single
`__asm__ volatile ("nop")` barrier makes the compiler add the remaining
padding: two nops after the masked read, or three after an unmasked signed
read. The assembled loops above are reproduced by `IPU_POLL_BARRIER`.
Spelling all three/four nops inside one asm statement overcounts the wait,
because the compiler still inserts that padding. This is a hardware-read
barrier, not a general instruction-scheduling lever.

`sceIpuStopDMA` matches all 228 instruction bytes with that barrier and the
nine-word DMA snapshot already used by the movie decoder. It saves the final
IPU control word, but the restart routine does not write that word back.

## sceIpuSync

100 bytes against retail's 104, with the hazard pad in place and the arms in
retail's order (`if (mode != 0) { if (mode == 1) ... } else { <wait> }`, which
is what puts the `ret = 0` initialisation in the first branch's delay slot).
The last instruction is block placement: retail branches to the `mode == 1`
body out of line with `beq`, where the draft falls into it and branches around
with `bne`.

## sceIpuRestartDMA

74 of 84 words match at the native 336-byte size under the configured 2.9
compiler. The ten differences are in the prefix registers holding the saved
bit pointer, its two buffered-qword fields, and the saved input address/count.
Both waits and all DMA restore operations match after that prefix.

The buffered input count is `((bp >> 8) & 15) + ((bp >> 16) & 3)`; it is
added to saved QWC and subtracted, times 16, from saved MADR. The output
channel restores only when both its address and count are nonzero. The input
channel has the same independent gate after both IPU waits and the bit-position
command. Field arithmetic is unsigned, and the IPU control polls are signed.

Using the record fields directly instead of a bit-pointer snapshot, and
separate named buffered-count fields, leave the same ten-word result. Do not
repeat those controls or change the snapshot contract to force allocation.
