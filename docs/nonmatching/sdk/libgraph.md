# Non-matching drafts: sdk/libgraph

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## sceGsExecStoreImage (sdk/libgraph, 0x68C)

**Reference implementation, not a match candidate.** Assembly remains production.

The C emits 1664 of 1676 bytes. In that emitted prefix, 392 of 416 words differ;
24 words match and 12 bytes remain uncovered. The first difference is at +0xC.
The 59-entry format jump table has 57 differing destinations. This is a broad
register-allocation and scheduling residual, not an almost-exact routine.
The complete default owner remains nine exact functions; the guarded owner
adds only this instruction/table residual.

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
