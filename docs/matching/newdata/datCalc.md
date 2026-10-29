# Matching notes: newdata/datCalc

## datGetPlayerParam (newdata/datCalc, 0x50)

7 of 20 words differ, exact size. The structure is right and the remaining
difference is which register holds the hearts bonus. Retail puts it in `$2`:
the `beqz` that skips `cmbGetHeartsParam` carries `daddu $2, $0, $0` in its
delay slot and falls into a single `subu $2, $16, $2`, so the zero and the
call result share the return register and the `$gp`-relative `DDS3_GBWK` load
has to avoid `$2` and lands in `$3`. Every shape tried either puts the bonus
in `$6` with a copy after the call (12 of 20) or duplicates the `subu` and the
epilogue into both arms (88 bytes against retail's 80).

Ruled out: `bonus = 0` before the `if`; `if`/`else` in both polarities; a
ternary at the return and in a local; a `goto` into a shared tail from either
arm; duplicating `return param - bonus` in both arms to invite cross-jumping;
`hearts` as `u8`, as `s8` and as an `s32` with the cast at the read; and all
36 combinations of declaration order and initialisation order of the three
locals.
