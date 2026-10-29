# Matching notes: effect/effBattle

## effBTLCutDraw (effect/effBattle, 0x420)

Eighty-eight of 263 words differ, one word short of retail's 264, and the jump table
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
