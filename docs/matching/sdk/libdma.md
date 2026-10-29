# sdk/libdma

## sceDmaPutEnv

15 of 118 words differ, at retail's 472 bytes.

Everything up to the register writes matches: the four range checks and their
return codes, the `D_CTRL` composition through the three byte tables, and the
five register writes in retail's order (`e000`, `e020`, `e030`, `e050`,
`e040`). The entry reads of all five registers are kept.

The earlier draft loaded `env->rbor` between the `D_SQWC` and `D_RBOR` writes
and came out 476 bytes with a `nop` in the `jr` delay slot. Loading it with
the other field values, before the writes, closes that: the `env->rbor` load
then fills the scheduler's slot that the draft gave to `move $v0, zero`, so
the copy of `*env` into `D_00598940` can use `$v0` as its scratch and the
return value sinks into the delay slot, exactly as retail does. The struct
copy and the epilogue now match word for word. Computing `sqwc` before `pcr`
is worth three further words (12 instead of 15), which is a probe result, not
evidence about the original statement order, so the recorded draft keeps the
register order.

What remains is one scheduling decision in the register-write block. Retail
issues five field loads before the `D_CTRL` write (`pcr_hi`, `pcr_lo`,
`sqwc_hi`, `rbsr`, `sqwc_lo`) and issues the `env->rbor` load late, in the
slot just before the `D_SQWC` write, which keeps only one extra value live
across the writes: `pcr` and `rbor` then share `$a2`, and `rbsr` gets `$t0`.
The draft issues the `rbor` load with that early group, so six values are live
there, `rbor` takes `$t0` and `rbsr` is pushed to `$t3`. The differing
words in that block, and the differing entry-read destinations whose dead
reads reuse the same pseudos, all follow from that one placement.

The placement is not reachable from statement order. A sweep of every position
of the four value computations in the five gaps between the register writes,
every emission order within a gap, each value inline at its own write instead
of in a local, and both operand orders of the `pcr` and `sqwc` compositions
(about 700 compiles) bottoms out at 12 words. Moving the `rbor` load to
any gap after the `D_CTRL` write makes the writes themselves issue early
(gap 1 puts the `D_CTRL` write in slot 5, gap 2 in slot 7, against retail's
slot 11), because the load that follows a volatile write cannot be hoisted
above it and so raises that write's priority; moving it to a gap at or after
the `D_SQWC` write reproduces the 476-byte draft. Carrying `rbor` in the `pcr`
local, which pins the load after the `D_PCR` write by anti-dependency, gives
the same 476 bytes.

Also ruled out, each worse than 12 words:

- computing each register value immediately before its store instead of
  computing them up front;
- holding the destination in a local `sceDmaEnv *` before the copy, which
  materialises `&D_00598940` at entry and moves the whole function;
- writing `D_RBOR` before `D_SQWC` in source, which confirms the volatile
  writes keep source order and so fixes the write sequence.

The assembler pin that closed `sceGsResetPath` and `sceGsSyncPath` does not
move this one: 472 bytes and the same 15 words either way, because its only
delay slot is one gcc fills itself.

An independent description of the function confirms the draft's shape: five
locals each initialized from one volatile register read, of which only the
`D_CTRL` one is read again, so the other four reads are dead values that only
survive because the reads are volatile; four independent range guards whose
early return is the fall-through arm; each guarded byte loaded a second time
for its table index, and the release-cycle byte a third time; the four
remaining register values built before the writes, in register order; and one
whole-struct assignment into the cache, which is a file-local static in the
original where this unit has to name the existing `D_00598940` symbol. It
confirms the byte fields select D_CTRL's STS, STD and MFD channel numbers and
the release cycle, which is how they are now named, and the flags this unit is
already built with. None of it accounts for the load placement, so the
residual stands.
