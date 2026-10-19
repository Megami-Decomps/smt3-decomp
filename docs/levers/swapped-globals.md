# Two global addresses in callee-saved registers can come out swapped


A function that calls through two different global state pointers often keeps
both addresses in callee-saved registers, rematerializing the `lui %hi` in
each block and spending an `addiu %lo` per call. Which symbol gets which
register is gcc's global-allocation priority tie-break
(`floor_log2(n_refs) * freq / live_length`, then allocno number, which is
pseudo creation order), and no source change found so far reaches it.

Both large functions in `effect/parManager` stop on exactly this: with
everything else identical, `dds3Particle_SparkBuffInit` is 490 of 512 words
and `dds3Particle_SparkOffsetBuffInit` 462 of 484, at their exact sizes, with
retail holding `dds3EffectBase` in `$21` and `ParRand` in `$20` and the
drafts the other way round. The two pseudos have the same reference counts and
the same first-reference order in both compiles, so the difference is below
the source. Measured and rejected: `&ParRand` for `ParRand`; reordering the
two tail statements that are each symbol's only tail use; all five non-retail
orderings of the `switch` cases that hold the other uses; three forms of
sharing or splitting the float local that sits in the same arms.

Treat a diff that is only this swap as finished work: write the draft, say so,
and spend the budget elsewhere. It is the same class of residual as the spill
slot rotation in `fldSkyDrawFilter`.
