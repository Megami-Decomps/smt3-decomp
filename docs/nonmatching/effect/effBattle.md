# Non-matching drafts: effect/effBattle

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## effBTLFieldColorON (effect/effBattle, 0x3C4)

Thirteen of 241 words differ, exact size, and the instruction stream is
otherwise identical: the same eight saved registers in the same slots, the
same `c.eq.s` guard against a zero materialised with `mtc1 $0`, the same six
unsigned-to-float conversion blocks, the same `0x3C000000` scale and
`437F0000` divide, and the same two `lqc2`/`vmul`/`sqc2` triples.

Three things remain, in two groups.

Seven of the thirteen are the `printf` argument copies, rotated by one. Retail
emits `daddu $5..$10` and then `daddu $4, $17, $0` for the format string; the
reconstruction emits `$4` first and the rest after. Everything else about the
call matches, including the format string living in `$17` across all six
`sdfFloatToULong` calls, which only happens when the format is a named local
assigned before the calls: with the literal at the call site gcc builds the
address in `$4` right before `jal printf` and the function goes to 205 of 238
differing words. Placement of that assignment was swept across twelve points
(function top, after each of the three colour reads, after each VU0 macro,
after each of the first five argument calls) and `declFirst`/`declLast` for the
local; everything from "after the `sqc2`" through "before the fourth argument"
gives the same 13, and moving it later than the fifth argument loses 40 or more.
Ruled out for the rotation itself: `const char *`, `s32` with a cast at the
call, `&D_00532FE8[0]`, an unprototyped `printf`, and an initializer at the
declaration (70 words).

Four are the `addu` that forms the table row: retail has
`addu rd, <scaled index>, <base>` at all four sites and the reconstruction has
the operands the other way. The documented remedy,
`(T *)((u8 *)(i * sizeof(T)) + (u32)base)`, is **wrong here**: applied to the
three `work.r/g/b` reads it goes to 221 differing words, and applied to every
site to 230, because the cast form also stops gcc sharing the `id * 0x14`
computation that retail does compute twice (once as `$17` for the first block
and once into `$16` for the argument block). All fifteen non-empty subsets of
the four access groups were measured; only the single `->word` store takes the
cast without loss, and it changes nothing.

The last two are a scheduling swap: retail puts `daddu $22, $2, $0` before
`lwc1 $f12, 0xC($29)` and the reconstruction the other way round.

What the draft does pin down, and is worth keeping even though the function is
not matched: `D_00317750` is a table of 20-byte rows,
`{ f32 a; f32 r; f32 g; f32 b; u32 word; }`, indexed by the effect id, and the
`a` word is the enable test. `D_00569970` and the stack colour locals are read
and written at indices 1, 2 and 3 only, so the colour type carries its alpha
first. `D_0031CA90` must be declared `extern u32 D_0031CA90[]`: as a scalar it
is reached through `$gp` where retail uses `lui`/`%lo`, which costs one word
and misaligns the whole tail. The three `D_00569970` stores are written
ascending in the source and come out `[1]`, `[3]`, `[2]`, which is worth eight
words.

## effBTLCutDraw (effect/effBattle, 0x420)

Eighty-eight of 263 words, one word short of retail's 264, and the jump table
has one of six entries wrong. The draft is kept for the decode rather than for
its distance: the body is semantically complete and everything awkward about
the function is pinned down.

What the draft establishes. The three rodata quadwords the unit already carries
are quaternions fed to `ToMatrix`: `D_00532EA0` is the one `Mult` combines with
the formation's `rot` quadword at +0x30 to build the base matrix, and
`D_00532EB0` and `D_00532EC0` are swapped between the two rotation matrices
depending on whether `effBTLSkillGetInvokeParty` returns 0x100. The frame holds
one `sdfVector4_t` at 0 and five `sdfMatrix4_t` at 0x10, 0x50, 0x90, 0xD0 and
0x110, in that declaration order; the one at 0xD0 is the output of
`effBTLPartGetRotateMatrixPtr` and the one at 0x110 the `MultMatrixNoArg`
result for the mark case. The quake block passes `list->shakeFade` as the
duration and `list->shake * 10` as the amplitude, clamped by a `shake < 0x1F`
test whose failure path prints `D_00532ED0` with the shake value (the argument
reaches `$5` from the `beql`'s annulled delay slot) and passes 0x12C instead.
The `switch` is over `aimParams.scaleKind` with cases 0 and 4 sharing the base
matrix, 1 and 2 taking the two rotation matrices, 3 empty and 5 doing the mark
multiply. `effBTLCutSetMax` takes the cut kind as a `u8`, which is what gives
retail's `lbu 0x14` of a `u16` field.

The residual is register allocation. Retail saves ten registers
(`$16`-`$23`, `$30`, `$31`) and the draft eight, and the two it does not spend
are the ones retail keeps for the loop: `$16` holds `i * 4` across the switch
and `$17` first the address of `cut->parameterWorks[i]` and then the 0xD0
matrix. Writing those as explicit locals, `s32 ofs = i * 4;` with a
`dds3EffectPart_t **slot` pointer and the documented
`(T *)((u8 *)ofs + (u32)base)` cast inside the switch arms, is **worse**: 149
of 260 words and three of six jump-table entries, because it also stops gcc
recomputing the row the way retail does. The plain `cut->parameterWorks[i]`
form is the better of the two measured.

Not yet swept: the order of the three leading loads (`currentFrame`,
`cut->list`, `cut->formation`), the placement of the `flags` and `count` reads
relative to the `effBTLCutGetAllPartColor` call, and the arm order inside the
`switch`, which is what the wrong jump-table entry is most likely to be.
