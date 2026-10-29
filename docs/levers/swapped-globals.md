# Two global addresses in callee-saved registers can come out swapped


A function that calls through two different global state pointers often keeps
both addresses in callee-saved registers, rematerializing the `lui %hi` in
each block and spending an `addiu %lo` per call. Which symbol gets which
register is gcc's global-allocation priority tie-break
(`floor_log2(n_refs) * freq / live_length`, then allocno number, which is
pseudo creation order), and no source change found so far reaches it.

Traced in `effect/parManager`: the two addresses are not pseudos from
expansion but copies created by gcse PRE, which rematerializes
`(high (symbol_ref))` per block. `--alloc` shows both allocnos tying on
priority (`pri=8/8`, live 41728 and 35328) so the colour order falls to the
allocno number, which is the pseudo number, which is the order gcse created
the copies: its expression hash table (211 buckets) holds
`high(dds3EffectBase)` at index 13 and `high(ParRand)` at index 34, so
`dds3EffectBase` gets the lower pseudo and `$20`. Both inputs to that index,
the `symbol_ref` hash and the bucket count derived from the function's set
count, are fixed once the rest of the code matches.

Both large functions in `effect/parManager` stop on exactly this: with
everything else identical, `dds3Particle_SparkBuffInit` is 490 of 512 words
and `dds3Particle_SparkOffsetBuffInit` 462 of 484, at their exact sizes, with
retail holding `dds3EffectBase` in `$21` and `ParRand` in `$20` and the
drafts the other way round. The two pseudos have the same reference counts and
the same first-reference order in both compiles, so the difference is below
the source. Measured and rejected: `&ParRand` for `ParRand`; reordering the
two tail statements that are each symbol's only tail use; all five non-retail
orderings of the `switch` cases that hold the other uses; three forms of
sharing or splitting the float local that sits in the same arms; swapping the
two `extern` declarations, which does not move the gcse hash buckets.

Treat a diff that is only this swap as finished work: write the draft, say so,
and spend the budget elsewhere. It is the same class of residual as the spill
slot rotation in `fldSkyDrawFilter`.
