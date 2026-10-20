# sdk/libdma

## sceDmaPutEnv

21 of 118 words differ, and the draft emits 476 bytes against retail's 472.

Everything up to the last store matches: the four range checks and their
return codes, the `D_CTRL` composition through the three byte tables, and the
five register writes in retail's order (`e000`, `e020`, `e030`, `e050`,
`e040`). The entry reads of all five registers are kept, and `env->rbsr` has
to be loaded before the stores while `env->rbor` is loaded just before its
own, which is what retail does.

The remaining difference is one instruction at the end. Retail uses `$v0` as
the scratch register of the 20-byte unaligned struct copy into `D_00598940`,
so `move $v0, zero` can only be emitted after the copy and the delay-slot
filler takes it:

    sw   $a1, 0x10($a2)
    jr   $ra
    move $v0, $zero

The draft allocates `$v1` to the copy instead, which leaves `$v0` free, so the
return value is materialised before the copy and the delay slot takes a `nop`.
That is register allocation, not statement order.

Ruled out, all emitting the same 476 bytes:

- computing each register value immediately before its store instead of
  computing `pcr`, `sqwc` and `rbsr` up front;
- holding the destination in a local `sceDmaEnv *` before the copy;
- keeping `rbor` in its entry local rather than assigning `env->rbor` inline
  (this is the 21-word form; the inline form differs in 22).
