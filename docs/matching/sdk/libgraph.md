# Matching notes: sdk/libgraph

## sceGsResetPath (sdk/libgraph, 0x68)

**Assembly remains production.** The truthful C draft emits 100 bytes against
retail's 104; the earlier match claim is withdrawn.

`EE_FBRST_SET` reads COP2 control register 28 into its scratch, ORs the reset
bits and writes the result back. Its scratch is an output, not an input:
the previous input-only constraint hid an actual register write from the
compiler. The macro now uses `"=r"`, and its caller does not initialize the
output. A paired `"=&r"` control also emits 100 bytes. Keeping a read-write
operand merely to retain an otherwise unused zero setup has no justified
input consumer and was rejected. No invalid contract is retained for credit.

The pointer-valued volatile register accesses, actual two-quadword VIF packet,
register operations and return are retained in the guarded reference. The
native fallback preserves all 104 bytes, including its dead zero setup;
recovering that extra instruction needs an independently justified source
premise. The SDK's 2.10 assembler selection remains in effect, but is not by
itself evidence that an input-only overwritten operand is sound.

## sceGsSyncPath (0x314)

The C matches: 788 bytes plus retail's four bytes of alignment fill, every
word equal, under the SDK toolchain's 2.10 assembler. It is still `INCLUDE_ASM`
for one packaging reason, and the body is kept beside it ready to promote.

The ten register-dump format strings have no per-symbol rodata piece. While the
function was assembly they rode in its own `.s`; once it is C, splat leaves
them in the unit's whole-rodata file, which nothing includes, and the link ends
176 bytes short in rodata. Writing them as C literals instead loses retail's
eight-byte string alignment and shortens the image by 16 bytes. Giving those
ten strings a piece, the way `D_0054A428` and the other four messages have one,
is all that is left.

The body itself needed: the counter in the unused timeout parameter's register,
five spin loops with their rotated do-while shape and top tests, a
post-increment counter test against 0x1000000, two COP2 control-register-29
reads around the VU1 loop, the eleven-call diagnostic block written out at each
of the five timeout sites so cross-jumping merges the tails, a literal zero
return on the blocking path with the result local confined to the non-blocking
section, and five single-statement ifs that `-O2` turns into four conditional
moves.

## sceGsSetDefDispEnv (0x374)

892 bytes against retail's 888, with the first 64 words exact. The assembler
pin does not touch this one: it has no unfilled delay slot. Matched: the
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

**Still assembly in production.** 1668 bytes against retail's 1680 under the
pinned assembler (1672 before it), with the first difference at +0x3C, where
retail has its fifth prologue zero and this shape sinks that one
initialization past the register saves. Everything the
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
