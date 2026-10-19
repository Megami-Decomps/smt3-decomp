# Non-matching drafts: effect/effBlur_Filter

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## dds3EffectBlur_AlphaMultiParmSet (effect/effBlur_Filter, 0xB4)

Twenty-six of forty-five words differ at the exact retail size, and the whole
difference is adjacent pairwise transposition: every instruction retail emits
is present, in the same registers (`$2` for both x values, `$6` for the first
y and `$3` for the second), but in thirteen places retail issues the filler
store from the texture-coordinate or ADC group one slot before the dependent
load or shift and the draft issues it one slot after. Nineteen words,
including the whole `r`/`g`/`b` head and the last thirteen instructions, are
exact.

The vertex layout is settled and is not where the remaining words are: nine
A+D registers, `RGBAQ` once as four words at 0, then `ST` at `0x10` and
`XYZ2` at `0x20` per vertex on a `0x20` stride, with `s`/`t` as `f32`, `z` as
`s32` and `adc` as a bare `s16`. The four corners are read in the order
`corners[0][0]`, `corners[0][1]`, `corners[1][1]`, `corners[1][0]` and each
scaled value is stored into two vertices, which needs the four `s32` locals
the draft has: written as the expression at each of the eight stores, cc1
reloads the corner per store (228 bytes against retail's 180), since a store
through the packet pointer is not in the same alias set as the corner load but
cc1 still will not share the subexpression across it.

Ruled out, all at 26 or worse: all six orders of the three store blocks
(coordinates, texture coordinates, z and ADC) crossed with three groupings of
the z and ADC stores (interleaved per vertex, all z then all ADC, all ADC then
all z) and four positions for the alpha store, 72 variants, every one of which
the scheduler re-sorts to the same 26; all 360 per-vertex groupings that write
one vertex's six fields together before the next vertex's (36 at best); the
alpha byte through a `u8` local (26); the alpha store written first, last, or
after the coordinate block (36, 36, 37); and reversing the `r`, `g`, `b` run
(28). The `sw $0` against `swc1 $f0` split for the zero and one texture
coordinates already matches, so the `f32` typing of `s` and `t` is confirmed
rather than open.

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

## dds3EffectRippleBlurDraw (effect/effBlur_Filter, 0x1E8)

Eighteen of a hundred and twenty-two words differ at the exact retail size,
and a hundred and four match. The residual is one instruction's worth of phase
in the loop guard: retail fills `blez`'s delay slot with
`lw $17, 0x34($19)`, the `slot = work->slots` load from inside the guarded
block, while the draft fills it with `daddu $4, $21, $0`, the argument setup
for the trailing `dds3EffectBlurDrawFrameMaskReset` tail call, which cc1 has
already hoisted above the branch. Everything after it is the same
instructions one slot out of step, plus the register rotation that costs
(`$6`, `$3`, `$2` in retail against `$2`, `$3`, `$6` here).

Three things in the draft were measured and are worth keeping rather than
rediscovering. The weight `1.0f - slot->phase` has to be written out at both
of its use sites, not held in an `f32` local: the local is live across the
`sdfSin` call and takes a second callee-saved float register, which grows the
frame by 16, and the expression twice gives retail's `sub.s $f3` plus
`mov.s $f2, $f3`. The colour read-modify-write needs the converted alpha in
its own `u32` local (`level = (u32)(...); slot->quad.color = (... & 0xFFFFFF)
| (level << 24);`), which took the function from 43 differing words to 19 by
keeping cc1 from hoisting the `lw` of the old colour above the
float-to-unsigned select; the one-expression form, the reversed `or` operands
and a `* 0x1000000` in place of the shift are 43, 21 and 43. And the count
belongs in the guard as `remaining = work->params.count; if (remaining > 0)`
with `while (--remaining != 0)`, worth one word over the four other spellings.

Ruled out on the delay slot, all at 19 or worse: seven loop spellings (`do`
with a post-decrement, with a pre-decrement, a `for` inside the guard, a bare
`for`, a `for` with the bound test `> 0`, a `while`, and the slot pointer
initialised in the `for` init, the last four at 97 because they drop the
explicit guard and lose the whole shape); separate `count` and `remaining`
locals in both orders; and an early `return` in the skip arm with its own copy
of the trailing call, which duplicates it and is 97.

## dds3EffectDistortionBlurDraw (effect/effBlur_Filter, 0x208)

Eight of a hundred and thirty words differ **inside the full unit**, which is
the number that matters; compiled alone the function is eight bytes shorter
and 60 of 128 words differ, and `check_unit` reports both. The eight are the same
loop-guard delay slot as `dds3EffectRippleBlurDraw` above: retail's `blez`
carries `lw $17, 0x34($19)` and the draft's carries the hoisted
`daddu $4, $21, $0` for the trailing tail call. Read that entry for what was
ruled out; the same eight loop spellings were measured here and none moves it.

Two source facts got this from 119 of 130 to eight and are the reason to keep
the draft. The two double constants have to be declared `extern f64 D_00533E30;`
as plain scalars, not as the incomplete arrays the addressing row in
[matching levers](matching-levers.md) recommends for forcing absolute
addressing: with `extern f64 D_00533E30[]` cc1 treats the array base as loop
invariant, hoists both symbols' addresses into `$22` and `$23` and grows the
frame by 16, where retail materialises `lui %hi` plus `ld %lo` fresh at each
use. The scalars keep the `INCLUDE_RODATA` lines and the pool order untouched.
And the delay guard belongs the other way round, `if (delay == 0) { <the whole
body> } else { slot->delay = delay - 1; }`: the single-statement arm in the
`else` is what gives retail's `bnel` with the `addiu $2, $2, -1` in the
annulled slot, where the natural polarity emits a `beqz` and a `b` over the
long arm.
