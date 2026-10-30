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

Matches all 104 instruction bytes with `switch (mode)` and the established
single-nop `IPU_POLL_BARRIER`. Case 0 polls the signed control word until clear
and returns zero; case 1 returns its unsigned busy bit; other modes return the
initialized zero without a hardware read.

The switch puts both case bodies after the zero/one comparison chain, unlike
the former nested `if`, whose mode-one body fell through inline. Actual compiler
expansion branches to the two case labels and the default return join; delay-slot
reorganization retains that layout, fills the first branch's slot with the
zero initialization and the second with the result copy, and adds three nops
after the explicit poll barrier. Ordinary and `-da` builds have identical full
`.text` and normalized assembly. The former four-nop macro overcounted the
wait; changing only that macro fixed the padding but not the dispatch layout.

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
