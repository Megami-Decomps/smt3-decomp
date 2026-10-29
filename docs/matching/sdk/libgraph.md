# Matching notes: sdk/libgraph

## sceGsResetPath (0x68)

108 bytes against retail's 104. The two VIF1 register writes, the store sync,
the four-instruction VU1 reset through COP2 control register 28 and both
quadword pushes of the initialization packet at `D_00529F30` match word for
word, as does the position where retail materializes the GIF_CTRL address.

The one open word is the ending. Retail writes GIF_CTRL in the `jr` delay
slot, and ee-gcc 2.9 never moves a volatile access into a delay slot, so that
store cannot be volatile; but a plain store to the constant address becomes a
two-instruction form that is itself ineligible for the slot, and ee-as fills
delay slots with `nop` rather than swapping, so the assembler will not rescue
it either. Taking the address from a pointer local that is initialized in its
declaration gives both the register-held address and the filled slot, at
exactly 104 bytes, but the materialization then sits 12 words early, before
the asm block, because a volatile asm is a scheduling barrier the insn cannot
cross. The draft keeps the direct volatile writes.

## sceGsSyncPath (0x314)

828 bytes against retail's 792, the 36 bytes being nine unfilled delay slots.
The body is otherwise reproduced: the mode test with the return-address save
in its delay slot, the counter in the unused timeout parameter's register, the
five spin loops in order with their rotated do-while shape and their top
tests, the post-increment counter test against 0x1000000, the two COP2
control-register-29 reads of the VU1 loop, the shared eleven-call diagnostic
block that cross-jumping merges out of the five written-out copies, the
literal zero return, and the five conditional moves that accumulate the
non-blocking mask.

What is left is the ten register reads of the diagnostic block. Retail reads
each one in its `scePrintf` delay slot, from an address held in a register as
a `lui`/`ori` pair with zero displacement. That address shape is what this
compiler emits only for a volatile access, and it will not put a volatile
access in a delay slot; a plain read is eligible but collapses the address
into a single two-instruction macro load, which is ineligible in turn. A
pointer local does not help: the constant folds back into the load unless a
volatile asm sits between the assignment and the use, and there is none here.
Ten reads, nine `nop`s, because the last call's slot takes its string
address.

## sceGsSetDefDispEnv (0x374)

892 bytes against retail's 888, with the first 64 words exact. Matched: the
`sceGsGetGParam` call kept external, the `mode == 2 || mode == 3` range test,
the `checkModelVersion` path, the `_GetGsDxDyOffset` call with the four frame
addresses formed by OR-ing into the stack pointer, the zeroing block with its
descending stores and the mode halfword reloaded into both predecessors by
delay-slot filling, `pmode`, the three `smode2` stores that cross-jumping
merges into one, and `dispfb`.

Two findings closed most of it. The display-mode field is read unsigned for
the range test and sign-extended (`sll` 16, `sra` 16) for the dispatch and the
diagnostic argument, so it is not cached in a local; reading `param->mode` at
each use reproduces retail's reloads exactly. And `smode2` is written in each
arm of the nested if rather than selected into a local, which is what keeps
the three constants in branch delay slots: selecting a value gives a
conditional move instead.

The residual is in the NTSC and PAL display term. Retail adds the horizontal
constant to the frame word with a 64-bit `daddiu` and only then the
offset-times-magnification product, where plain C keeps that sum 32-bit.
Giving the term a 64-bit operand does produce the `daddiu`, but gcc then folds
the constant into the product instead and seven words are lost elsewhere, so
the draft keeps the 32-bit form. Retail also decrements the magnification in
the `mflo` register after both multiplies, where this shape needs one register
copy first, and that one extra instruction shifts the remainder of the
function.

The vertical terms are written offset-plus-frame-word-plus-constant and the
two multiplies in the order the horizontal term needs; both were read off
retail's operand order rather than guessed.

## sceGsExecStoreImage (sdk/libgraph, 0x68C)

**Still assembly in production.** 1672 bytes against retail's 1680, with the
first difference at +0x3C, where retail has its fifth prologue zero and this
shape sinks that one initialization past the register saves. Everything the
structure decides now agrees instruction for instruction: the seven saved
registers and the two parameter copies, one load of each 64-bit record word
with the width, height and pixel-store mode extracted from the loaded
registers in that order, the 59-entry jump table with its five bodies in text
order, the uncached `trxreg` patch, all five spin loops, the merged timeout
blocks, both FIFO loops and the success tail. What remains is instruction
order inside the case bodies and the cleanup block, plus three delay slots
this compiler leaves empty for a volatile access (rule 6 in
[sdk-matching](../../sdk-matching.md)). Measured as 142 differing lines of a
418 against 420 instruction sequence, down from 214 before the last fix; the
byte-level word count stays high only because the one missing instruction at
+0x3C shifts the rest.

Four changes took it there, from a draft that was 1656 bytes and diverged at
+0xC:

The DMA quadword count is masked with the unsigned `0xFFFFFFF8`, not with
`~7`. The unsigned constant has to be materialized with `lui` plus `ori`
where `~7` is one `addiu`, and the 16-, 8- and 4-bit bodies then share that
register for their align-up masks, which is five instructions and most of the
length gap.

The shared cleanup is written out in full at each of the four timeout sites
rather than reached by a `goto`. Cross-jumping then merges the `scePrintf`
call itself, giving retail's three call sites instead of four, exactly as it
does for the eleven-call block of `sceGsSyncPath`.

The `trxreg` patch re-extracts the width from the record word instead of
reusing the width local. Local CSE cannot share the earlier extraction across
the block boundary, so retail has a second mask-and-sign-extend there.

Each case body computes the byte remainder first, then the quadword
remainder, then the DMA count. That is the opposite of the earlier draft's
order and it halved the instruction-order difference.

An independent description of the function confirms the rest of the shape:
one spin counter shared by every timeout and never reset, five counters
zeroed before any branch with the padded height set only inside the switch,
the saved interrupt mask living in a caller-saved temporary across the
success path (so no call may be introduced between its assignment and its
last use), both interrupt-mask stubs typed 64-bit, and the destination
indexed by the sum of the two counters in the quadword loop while the byte
loop re-derives its address each iteration.

The existing `sceGsSetDefStoreImage` constructor and SDF capture callers
establish the two-argument interface and 112-byte, seven-quadword command
packet. Width and height are unsigned 12-bit values converted to signed
32-bit locals. The single format switch handles 32-, 24-, 16-, 8- and 4-bit
pixels, divides payload into DMA blocks of eight quadwords, remaining
quadwords and a byte tail, and rounds height only when that byte tail exists.
It writes the adjusted TRXREG through the native uncached address alias.

All polling stages share one unsigned postfix timeout count. The initial
and command DMA waits, GS FINISH wait, payload DMA wait, and FIFO-read waits
retain their individual conditions. FIFO data is available when the VIF1
fullness mask is nonzero. Register accesses preserve their native 32-, 64-
and 128-bit widths.

Ordinary `u128` C reproduces the aligned FIFO load/store pairs. The same plain
16-byte local supplies the partial-byte copy and receives discarded excess
quadwords; the compiler retains its stack store in the discard loop. There
is no volatile local, copy intrinsic, load/store assembly or dummy dependency.
The final FIFO command is an external reference to its existing native data;
only its first quadword is read.

The saved interrupt mask is the return from
`GsPutIMR(GsGetIMR() | 0x200)`, not the earlier getter result. Success clears
VIF1 status and GS BUSDIR, restores that mask, acknowledges FINISH, then
writes the final FIFO command. Timeouts deliberately differ: the early DMA
exit only prints; the FINISH exit prints and writes the command; later DMA
or FIFO exits print and reset CSR, BUSDIR, GIF control and VIF1 FBRST.
None of those error paths restores the mask. Combining them into a uniform
cleanup would change native behavior.

A follow-up needs evidence for the native packet/count lifetimes and error
block layout. Shared format setup followed by a second rounding switch was
rejected: native has one switch with branch-local calculations. Native
entry initialization improves code correspondence, but does not close the
remaining register allocation. Do not add volatile scratch storage or
instruction shims to force the already reproduced quadword operations.
