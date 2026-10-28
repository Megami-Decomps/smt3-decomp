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

## datDeadNakama (newdata/datCalc, 0xC4)

28 of 48 words differ and the draft is one word short. The loop body, the
`fldAnahoriAkuma_t` view at `DDS3_GBWK + ofs + 0xA50` and the `u32` offset
local that keeps `ofs + 0xA50` in a saved register are all right. Two things
are not. Retail copies the record address with `daddu $5, $3, $0` in the
`flag & 1` branch's delay slot so `$3` can be reused, and it forms the second
read of the same flag as `(DDS3_GBWK + 4) + ofs` with displacement 0 where
the draft forms `(DDS3_GBWK + ofs) + 4`. The two are the same instruction
count, so this is an association ee-gcc canonicalises.

Ruled out for the association: explicit parentheses around
`((u8 *)DDS3_GBWK + 4)`; the same through a `datUnitWork_t *` cast; the
index-first cast `(u8 *)aofs + ((u32)DDS3_GBWK + 4)`; an address derived from
`&DDS3_GBWK->akuma[0].flag`; and `s32` instead of `u32` for the offset local.
Ruled out for the copy: a named `fldAnahoriAkuma_t *` local for the record,
with the second access written through it, through the global, and through the
`+ 4` form.

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

## datInitPlayer (newdata/datCalc, 0x12C)

7 of 75 words differ, exact size. The `memset`, the 0xEC struct assignment
from `D_003493C0` with its runtime 8-byte-alignment dispatch, the item clear
loop and the tail call to `evtBitOn` are all right. What is left is a
one-register rotation over the two scalar stores: retail puts the constant 1
in `$4` and the loop index in `$5`, skipping `$2`, while the draft uses `$2`
and `$4`. Retail also emits `sb 0x1914` before `sw 0x1924` and the draft emits
them the other way round, and no source order changes that.

Ruled out: both orders of the two stores, written through the `u8 *` local as
a subscript and as a cast; both declaration orders of the pointer and index
locals; and four `memset` declarations (the `s32` return meant to reserve
`$2`, `void *`, unprototyped, and none at all, leaving gcc's builtin). All
sixteen combinations produce byte-identical output.
