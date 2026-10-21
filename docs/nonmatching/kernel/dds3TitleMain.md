# Non-matching drafts: dds3TitleMain

Conventions and the reason these are kept: [../../nonmatching.md](../../nonmatching.md).

## mselToggleFlag (dds3TitleMain, 0x6C)

13 of 27 words differ, at the exact 108-byte size. Flips a `u8` toggle, then steps
the low byte of a packed `s32` by +0x10 or -0x10 depending on the third
argument and clamps it to 0..0x80.

The structure is settled: the `bnez` on the narrowed `s8` argument with the
toggle store in its delay slot, the duplicated `lw` in both arms, the
duplicated `addiu -0x100` mask and the final `and`/`or`/`sw`. Retail carries
two plain register copies, `daddu $4, $2, $0` in each arm (the packed value
the tail re-reads) and `daddu $3, $2, $0` at the top of the clamp (the clamped
value). Writing the arms as `level = *value; v = level; level = (v & 0xFF) +
0x10;` reproduces the first pair, which is what took this from 20 of 25 words
and two words short to the exact size. The clamp copy needs the same
treatment and the two will not coexist: adding a third local for the clamped
value produces `daddu $3, $2, $0` and loses both arm copies instead, in all
six declaration orders of the three locals.

Ruled out: `*value` written at every use; an `s32 v` local assigned once above
the `if`, which hoists the load out of both arms; a separate `raw` temp in the
arms rather than reusing `level`; `if (dir != 0)` rather than
`if (dir == 0)`, which inverts the first branch; the clamp as
`if (level <= 0) ... else if (level > 0x80)`, which emits `bgtz` where retail
has `blez`; and `~0xFF` against `-0x100` against `0xFFFFFF00` for the mask,
all of which give the same `addiu $2, $0, -0x100`.
