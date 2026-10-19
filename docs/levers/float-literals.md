# Float literals


ee-gcc truncates a decimal literal toward zero, so `-946.42f` lands one ULP
away from retail and `-946.41993f` is exact. Derive the shortest decimal that
round-trips to the retail word rather than transcribing the rounded value.

`check_unit.py` says so outright when it hits one: a differing word whose
retail side is a `gp`-relative `lwc1` is reported as `D_00XXXXXX is a .lit4
pool constant: write the float literal`. Take it at its word and inline the
value rather than declaring the symbol `extern f32`; `dds3RunQuake`'s
`D_0054C188` is `0.01f`.

To tell whether a `gp`-addressed `D_00XXXXXX` is a pool entry, subtract the
flat-image delta `0xff000` from its vram and look the file offset up against
the `.lit4` rows in `config/smt3/SLUS_209.11.yaml`, whose addresses are file
offsets. `D_0054C2B4` and `D_0054C2B8`, read by `fldCamCalcSlopeOffset`, land
at `0x44D2B4` and `0x44D2B8` inside `field/fldDraw`'s pool and hold `0.001f`
and `-0.001f`. The pool is not deduplicated: the same constant appears at
several offsets, so only the order of use matters.

A float literal passed to an asm `"r"` operand folds to one `lui`, because
cc1's `movsf` can load a constant straight into a GPR and `regclass` prefers
`GENERAL_REGS` when the operand's only other reference is that `"r"` use.
Where retail instead shows `lui` + `mtc1` + `mfc1`, the constant is sitting in
an FP register and cc1 is moving it out, which also means those three
instructions are ordinary insns the scheduler may interleave with the
surrounding code. `PS2_FLOAT_BITS(out, f)` emits the same three but as a
volatile asm, so they become a scheduling barrier: use it only where retail
keeps them together. `dds3SceneLightUpdateFirst` needs the schedulable form
four times and has not been written yet; adding a dummy `"f"` input beside the
`"r"` one does flip the class, but cc1 then commons the two constant loads in
the same block.

`ee-as` owns the `.lit4` pool, not `cc1`. `configure.py` rewrites every
`lwc1 %gp_rel(D_...)` in the split asm into `li.s`, so literal values are
readable in `build/eeasm/` and the pool is rebuilt per object in emission
order. The order in which the C uses a constant has to match retail.
- Where retail loads every field of one struct before storing any of them into
  another, and the two pointed-to types are different alias sets so the
  scheduler is free to interleave, read the fields into locals of the source
  field's own declared width. `s16` and `u16` locals also keep retail's `lh`
  against `lhu` per field, which a direct `dst->f = src->f` copy does not:
  assigning `s16` to `s16` lets cc1 use `lhu`.
  Confirmed a third time, and this time the barrier buys a *load*:
  `dds3ComparePosPoseSet2` in `basic/dds3Basic` compares `fabs(length) - right->r`
  against zero, and with `fabsf` the radius `lwc1` lands after the `abs.s` and the
  zero and the radius take each other's float registers, 5 of 22 words under eleven
  spellings (the comparison reversed, a zero local, a radius local or none, the
  `abs` as its own statement, `else`, a `goto`, a ternary). `EE_ABS_F32` schedules
  the load into the `mtc1`-to-`abs.s` latency the way retail does and is exact. Its
  matching sibling `dds3ComparePosPoseSet`, which adds two radii, needs `fabsf`, so
  the two live side by side in one unit.
