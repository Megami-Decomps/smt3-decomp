# Matching notes: effect/effBlur_Filter

## dds3EffectFilterDrawTexQuad (effect/effBlur_Filter, 0x1B8)

Twenty-two of a hundred and ten words differ at the exact retail size. The
first eighty-eight are exact: the whole draw tag, the blend-state packet, the
texture link and the tail through `sdfGrOtTable[57]`. Everything that is left
sits in the tri-strip fill, and it is one mechanism.

The source colour is four `u8` bytes, and a `char`-typed access has alias set
0 in ee-gcc 2.96, so it conflicts with every store through the packet pointer.
That totally orders the four `lbu`/`sw` pairs against the sixteen texture and
ADC stores, and the scheduler then has nothing to put in each `lbu`'s load
latency: the draft emits `lbu`, `sw`, `lbu`, `sw` back to back and parks the
`swc1` run on one side of them. Retail interleaves one coordinate store into
every load's latency and reuses `$3` for all four bytes, which is exactly what
the same body produces when the four reads are `s32` fields instead: replacing
`source->color[i]` with `source->bounds.left` and its neighbours (wrong
instruction, right dependence graph) reproduces retail's schedule and register
choice instruction for instruction. So the open question is a byte load whose
alias set conflicts with the `s32` packet stores but not with the `f32` ones.

Ruled out: `u32 r : 8` bitfields in a unit-local struct, which do give `lbu`
and do break the false dependence, but go too far, since a bitfield read
conflicts with neither the integer nor the float stores and cc1 hoists all
four loads into four separate registers ahead of every store (54 of 110);
reading the bytes into four `s32` locals before the stores; and the plain
`u8` read with every block order (all 24 permutations of the colour,
coordinate, texture-coordinate and ADC blocks, crossed with four
texture-coordinate groupings and four z/ADC groupings, 64 of them measured).
The best of those is this draft's order, texture coordinates first with the
`s` values grouped ahead of the `t` values, at 22; the natural colour-first
order is 35 and the worst is 44. Writing the whole block out in retail's own
emission order, which is what the alias model says the source must have been
if the read really is `char`-typed, gives 31 and an implausible source, which
is itself evidence against that model.
