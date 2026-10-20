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

So the original wrote the pad explicitly. Reproducing it needs the house
inline-asm idiom:

    #define IPU_HAZARD_WAIT() __asm__ volatile ("nop\nnop\nnop\nnop")

and the loop as a `do`/`while` over a local, because the value has to be read
before the pad and tested after it. This is also why the three unmatched
functions here looked unreachable before: the shortfall was always a multiple
of four instructions.

## sceIpuSync

100 bytes against retail's 104, with the hazard pad in place and the arms in
retail's order (`if (mode != 0) { if (mode == 1) ... } else { <wait> }`, which
is what puts the `ret = 0` initialisation in the first branch's delay slot).
The last instruction is block placement: retail branches to the `mode == 1`
body out of line with `beq`, where the draft falls into it and branches around
with `bne`.

## sceIpuStopDMA and sceIpuRestartDMA

Both carry the same hazard pads, and adding them gets the instruction count
right but 8 and 12 bytes long respectively: holding the polled word in a local
costs a register copy retail does not make. Retail tests the loaded register
directly, so the pad has to sit between the load and its use without a C
variable in between — which the `do`/`while` form cannot express. These need
either a different macro shape or the loop written as one asm block.
