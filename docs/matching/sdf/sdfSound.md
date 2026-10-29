# Matching notes: sdf/sdfSound

## sdfSndVoiceMapInit (sdf/sdfSound, 180 bytes)

39 of 45 words match at the exact 180-byte size, and the instruction multiset
is retail's. Both loops are exact: the ascending 16-channel loop whose counter
doubles as the stored `index` and whose `slti` against 0x10 sits between the
`index` and `unk1` stores, and the seven-slot countdown from 6 with `bgez` and
an ascending `p2sdLoadEntry_t *`.

The difference is the order of the three stores in the block after
`sdfSndIopAlloc`. Retail emits `opaque1D0`, `opaque1D4`, then
`channels[0].iopWorkAddress`, with the slot pointer's `addiu $2, $3, 0x190`
last; the draft emits the allocation store first and the slot pointer second,
and the `-1` the slot loop stores lands in a different register as a result.
The three stores are to disjoint offsets of one base, so sched is free to
order them and source order does not decide it.

Ruled out:

- All six orders of the three statements. Three of them (any order ending in
  the allocation store that does not also start with `opaque1D0`) reach 40 of
  45; the natural `1D0`, `1D4`, allocation order kept here is 39.
- Storing the allocation result directly from the call instead of through a
  local, which costs 16 words, and doing the same with no local at all.
- The `extern u32 sdfSndMap[]` view for either side of the split, to give the
  two groups different alias sets: `sdfSndMap[6]` for the allocation is 8
  words out and `sdfSndMap[0x74]`/`[0x75]` for the zeroes emits an extra word.
- A `sndMergedWork_t *` local for the whole function, which loses the first
  loop as well.
- Declaring the function `s32` and falling off the end, to keep `$2` live past
  the store. No effect; the dead note stays on the store.
- A `sndChannel_t *` local for the allocation store (17 words out: it survives
  into the first loop and renames its registers).

### The scheduling trace, and what it asks for

    tools/cc1probe.sh <candidate> --as-unit src/smt3/sdf/sdfSound.c --version smt3 \
        --func sdfSndVoiceMapInit --sched --no-dumps --out <dir>     # verify: OK

sched1 reaches the post-call block with four ready insns: the three stores and
the `6` the slot loop needs. All three stores have priority 2, because the
`addiu $2, $3, 0x190` that follows them is an anti-dependence on the base
register, so no store is ordered against another: one base register plus
disjoint constant offsets is provably non-overlapping, whatever the field
spelling. The tie goes to `INSN_REG_WEIGHT`, and the allocation store is the
only one that frees a register:

    uid179 = (set (mem:SI (plus (reg 139) 24)) (reg:SI $2))   regw=-1, DEAD $2
    uid169 = (set (mem:SI (plus (reg 139) 464)) 0)            regw=0
    uid174 = (set (mem:SI (plus (reg 139) 468)) 0)            regw=0
    rank A=uid169 B=uid179 ... => decided@reg_weight (smaller reg_weight wins) -> B first

`reg_weight` is `sets - dead notes`, so the allocation store scores -1 for
killing the call's result and sorts ahead of both zero stores regardless of
source order. That is why all six statement orders leave it first, and why the
`-1` the slot loop needs lands in `$3` (the base dies earlier) instead of
retail's `$5`.

Retail's order needs either the allocation's value live past its store, which
no reading of the function provides, or a memory dependence between the stores,
which one base register forbids. A second base register would create one, but
retail forms all three addresses off the single `addiu $3, $16, %lo(sdfSndMap)`.
