# A value held in a callee-saved float register shares a local with the tail


A local that is set and used inside one arm gets a call-clobbered register,
because its allocno crosses no call; one given its own name per arm is
block-local and lands in `$f0`. Where retail holds such a value in `$f20` or
another callee-saved register, the source is reusing **one** local that is
also live across a call somewhere else in the function. In both Spark
`BuffInit` functions the per-arm jitter scale is the same local as the tail's
`head.speedJitter`/`head.spinJitter`, and only that sharing puts it in `$f20`;
its own local gives `$f5`, three per-arm locals give `$f0`, and either one
costs the three `mul.s` registers and the schedule around them.
- `SDF_VU0_SCALEVECTOR` against `SDF_VU0_SCALEVECTOR_BITS` with the float
  expression passed straight to the `"r"` operand is a scheduling lever, not
  only a spelling. The plain macro's `mfc1` sits inside the volatile asm, so the
  scratch is a barrier and two successive scales reuse one register; the `_BITS`
  form lets cc1 emit the `mfc1` as an ordinary insn, the two scratch live ranges
  overlap in `sched1` and take different registers, which is retail's `$2` then
  `$3`. `lightmainprocedure` lost four differing words on that pair, and
  `MMI_RGBA_PACK_UNIT(out, 128.0f)` and `MMI_RGBA_UNPACK(src, 0.0078125f)` are
  the same thing for the pack and the unpack: the `lui`/`mtc1`/`mfc1` triple
  retail shows for the scale is cc1 moving an FP constant into a GPR, with
  unrelated stores scheduled between the `mtc1` and the `mfc1`.
- A word the **next** block needs, read right after a volatile asm and before
  the stores of the previous block's result, belongs in its own local.
  `lightmainprocedure` reads `data->BaseAmbientRGB` into `nextColor`
  immediately after the first colour pack and stores it into the carrier below
  the two stores of the packed light colour: that is exact, where the field read
  written at its use site is 13 of 130 words out and the six orders of the three
  statements around it range from 9 to 17.
