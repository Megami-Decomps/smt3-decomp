# sdk/libipu

## The IPU hazard wait

Every `IPU_CTRL` poll in this unit pads **four slots between the load and the
branch that uses it**, and cc1 emits them for a plain C loop on its own.
Retail's `sceIpuSync` wait is

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

This padding is the compiler's, not the source's. Both `ee-gcc
2.9-991111-01` and 2.96 fill the load-use slots after a volatile read whose
branch consumes it, with the same count whether or not the loop body holds a
`__asm__ volatile ("nop")`: the hand-written nop only takes one of the slots
cc1 would have filled anyway. The unit carried an `IPU_POLL_BARRIER` macro
for this; removing it from all four polls leaves every function in the unit
byte-exact, so the barrier is gone. Spelling the nops out by hand still
overcounts the wait, because cc1 pads after them.

A nop in the source is load-bearing only where the loop reads no hardware at
all, as in `sdfSndTransToIOP`: there the countdown has no side effect and cc1
deletes the loop. `EE_SPIN_NOP` in `include/ps2asm.h` is that case.

`sceIpuStopDMA` matches all 228 instruction bytes with the plain C loop and
the nine-word DMA snapshot already used by the movie decoder. It saves the final
IPU control word, but the restart routine does not write that word back.

## sceIpuSync

Matches all 104 instruction bytes with `switch (mode)` and a plain C poll
loop. Case 0 polls the signed control word until clear
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

Matches all 336 instruction bytes under the configured 2.9 compiler. The
saved input-QWC snapshot is a real initialized local bound to `$4` while the
initial count/address expressions are formed. Its last use precedes the first
call; no caller-saved value is retained across `ipuWriteFromIpuChcr`. The bit
pointer and buffered count remain ordinary C locals. The low seven-bit command
is extracted before forming the buffered count; spelling the independent
unsigned high-field term first reproduces the native sum's operand ordering.

The buffered input count is `((bp >> 16) & 3) + ((bp >> 8) & 15)`; it is
added to saved QWC and subtracted, times 16, from saved MADR. Both fields are
bounded, and all address/count arithmetic is unsigned. The output channel
restores only when both its address and count are nonzero; the input channel
has its independent gate after both signed IPU waits and the bit-position
command. The established hardware poll barriers and all MMIO operations are
unchanged. The snapshot's saved IPU-control word is not restored.

Removing only the input-QWC register binding leaves 80 of 84 words at the
same 336-byte size: the prefix's input-count/address registers differ. The
original guarded source matched 74 of 84 words. Binding the bit pointer or
buffered count as well is unnecessary and was removed. Ordinary and `-da`
builds have identical full `.text`, relocation records, and normalized assembly.
The full enabled owner has six matching functions and no differences.

## sceIpuInit (sdk/libipu, 0x238)

All 142 words match at the native 568-byte extent under the configured 2.9
compiler. The complete seven-function owner, including padding, reproduces
all 1552 retail bytes after its fifteen relocations are resolved.

The routine writes 1 to the input DMA channel control register, resets the
IPU and clears its command, transfers the quantizer tables, issues the two quantizer commands,
transfers the CLUT, issues the CLUT and final mode commands, then resets and
clears the command again. All eight busy waits read the signed 32-bit control
word until its high bit clears. The compiler supplies each poll's four hazard
slots; no manual nop or new instruction-emitting helper is used.

There are ten volatile 128-bit FIFO writes. Eight use quantizer chunks
0, 1, 2, 3, 4, 4, 4, 4; the last two use CLUT chunks 0 and 1. In particular,
native performs four separate loads of the final quantizer chunk. The external
80-byte and 32-byte tables are represented directly as five and two u128
chunks. Both actual storage ranges are 16-byte aligned. The source reads
through those declared types, without casting byte/halfword arrays to wider
lvalues, inventing bounds, making source tables volatile, or changing their
existing data ownership. Assembly byte/halfword presentation metadata is not
a conflicting C declaration.

One existing empty live-value operand constrains the initialized FIFO pointer
after the first store and before the other seven quantizer transfers. Its
actual value remains 0x10007010, and it has real store consumers until the last
quantizer write. No call crosses that lifetime. This is a conservative compiler
visibility aid, not a memory barrier or recovered original source spelling.
The first store retains its ordinary scheduling; the later pointer opacity
prevents the compiler from discarding native source reloads.

Removing only this operand lets cse2 replace three repeated source loads with
copies of the first loaded quadword, while retaining the FIFO writes. Qualified
complete compiler output establishes that dependency. The ordinary ablation
is not retained, and no false alias promise or table volatility is used to
repair it.

Independent review verifies the hardware addresses, volatile access widths,
poll ordering, table bounds, real pointer lifetime, caller and provider ABI.
The corrected direct-chunk source and minimal ablation pass strict three-mode
plain/dump complete assembly and ELF parity, normalizing only randomized source
pathnames. Both complete-owner checkers report 7 match, 0 differ. All six
previous functions and all resolved relocation targets remain exact; this
owner introduces no non-text allocated data. Full retail and regression gates
accompany the landing. Existing SDK archive and DDS correspondence are prior
provenance, not a new archive comparison in this pass.
