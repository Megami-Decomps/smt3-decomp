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
