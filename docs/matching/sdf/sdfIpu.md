# Matching notes: sdf/sdfIpu

## sdfIpuDecode (sdf/sdfIpu, 0x1D0)

18 of 114 words match, at 456 emitted bytes against retail's 464, first
differing at +0x10.

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

### Measured: the prologue address is already right; the delay slot is not

Taking `&cb->dma` through a pointer local declared before the `if` puts the
address in the entry block, where retail has it. `tools/cc1probe.sh
scratch/sdfIpu_try.c --as-unit src/smt3/sdf/sdfIpu.c --version smt3 --func
sdfIpuDecode --sched --no-dumps` (verify: OK) shows sched2 issuing the entry
block as `addiu sp` / `sd $17` / `move $17,$4` / `sd $18` / `addiu $18,$17,104`
/ `sd $16` / `sd $31` / `lbu $2,14($17)` / `bnez`, which is retail's order
word for word. `--reorg` (verify: OK) then shows
`fill_simple_delay_slots(non_jumps_p=0)` accepting `uid11` (`$18 = $17 + 104`)
for the `bnez` slot: the backward scan skips the `lbu` and the two `sd`s and
takes the address.

Retail's slot instead holds `daddu $4, $0, $0`. The simple pass always prefers
the nearest eligible preceding insn, so retail had a `$4 = 0` standing between
`addiu $18` and the test, inside the entry block. That also explains retail's
register choice in the if-body: with `$4` live from the entry block the
constants land in `$5`, `$6` and `$8` (and `$5` is reused for the `1` stored to
`cb->ipuCycleInitialized` and then for `0x10000`), while ee-gcc puts `0x10000`
in `$4` because the body's own `sceIpuSync` argument dies before it. The
argument setup for a call inside the if-body is emitted in the body block and
ee-gcc does not move a hard-register set across the branch, so no ordinary
spelling of these calls supplies the entry-block zero.

Also ruled out: writing `bufferIndex = cb->bufferIndex` in both arms of an
if/else to reproduce retail's duplicated `lbu $9, 0x18($17)` and the `b` that
precedes it. The compile collapses the two reads back into one block (456
bytes, unchanged), so retail's duplicate is not a source duplicate.

Taking the zero through a local declared before the `if`
(`s32 syncArg = 0; ... sceIpuSync(syncArg, 0);`) does not supply it either:
cprop folds the local back into the argument move at the call, and the output
is byte-identical to the plain spelling. A zero in `$4` that survives into the
entry block therefore needs a value that is not a propagable constant, which
this call does not have.
