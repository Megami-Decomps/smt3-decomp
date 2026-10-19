# Duplicated tails and post-reload cross-jumping


gcc 2.96 runs cross-jumping after reload, in `jump2`, so it compares
instructions by their **allocated hard registers**. Two consequences, both
measured on `dds3ParticleColorAnim`:

- Where retail computes a value in both arms of an `if` but converts or
  stores it once at the join, the source most likely duplicates the whole
  tail in both arms and gcc merged the common suffix. Writing one shared tail
  after the `if` instead loses the duplicated constant materialization that
  the merge leaves behind: a `lui`/`ori` pair scheduled earlier in each arm
  than the merged suffix stays in both arms, which is exactly what retail
  shows. The draft went from 45 of 83 words to 64 on that change alone.
- The merge stops as soon as one register differs, which is why retail can
  keep two of three float-to-int conversions inside the arms and share only
  the third. Do not read that asymmetry as asymmetric source.

Beware the inverse trap: making the two arms' last instructions *identical*
invites a merge retail does not have. Two arms that each end
`alpha = product; alpha &= 0xFF000000;` get the `and` cross-jumped into the
join and come out two words short, where the same two statements with the
mask folded into the first expression in one arm only do not.

Also: `objdump` collapses a run of identical instructions to `...`, so a pair
of `nop`s that ee-gcc emitted before a `div.s` can look absent when you
compare a disassembly by hand. Pass `-z`.
