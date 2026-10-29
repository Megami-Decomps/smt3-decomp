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
