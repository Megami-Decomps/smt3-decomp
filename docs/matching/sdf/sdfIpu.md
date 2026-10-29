# Matching notes: sdf/sdfIpu

## sdfIpuDecode (sdf/sdfIpu, 0x1D0)

22 of 115 words match, at 460 emitted bytes against retail's 464.

The draft has retail's instruction mix almost exactly: the DMAC-enable save and
restore around `SDF_TO_IPU_CHCR = 1`, the two `sceIpuSync(0, 0)` calls, the
`cb->dma.output*` block with the `movn` rounding for `cb->buffSize / 16`, the
`cb->unk65` branch, and the final `SDF_IPU_CMD` word built from
`D_00529CE8[cb->rgbMode * 2]` shifted to bit 27 and `[... + 1]` to bit 26. What
does not reproduce is where `addiu $18, $17, 0x68` lands. Retail schedules
`&cb->dma` into the prologue, between `sd $18` and `sd $16`, and fills the
`bnez` delay slot with the `daddu $4, $0, $0` that the first `sceIpuSync` needs;
ee-gcc puts the address in the delay slot instead and materialises the zero
later, so the whole body sits one word earlier than retail and every later word
reads as a difference.

Retail also lays out the `if (!cb->ipuCycleInitialized)` body with a trailing
`b` whose delay slot duplicates the join's `lbu $9, 0x18($17)`; the draft falls
through with no branch, which is two words shorter there.

Ruled out: `&cb->dma` written inline at both call sites (476 bytes, gcc
rematerialises the address and the duplicate costs three more words); a `dma`
local used for the `cb->dma.output*` stores as well (the stores then come out
`$18`-relative at offsets 0x10/0x18, where retail uses `$17` at 0x78/0x7C/0x80);
and the `cb->unk65` test written `== 0` with the branches swapped, which gets
the taken-path layout retail has but costs two more differing words overall.
