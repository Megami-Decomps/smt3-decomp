# Non-matching drafts: newdata/datCalc

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

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

## datPartyDamagePipe (newdata/datCalc, 0x114)

54 of 68 words differ, one word short, and every instruction retail emits is
present: the guard, the `fldAnahoriAkuma_t` view at `0xA50 + i * 0xEC`, the
`div`/`div1` pair on the two pipes, the `mult`/`mult1` pair, and all three
cmov clamps. The whole allocation is rotated by one. Retail puts the two rate
parameters in `$21` and `$20` and hoists the loop-invariant constants in the
order 100, 10, 1; the draft puts the parameters in `$22` and `$21` and hoists
10, 100, 1.

Ruled out: both declaration orders of the offset and index locals crossed with
three ways of initialising the offset (a statement above the loop, and either
order inside the `for` init), crossed with both orders of the two terms of the
damage sum. All twelve produce byte-identical output, so ee-gcc canonicalises
the sum and the constant-hoisting order is not reachable from the expression.
