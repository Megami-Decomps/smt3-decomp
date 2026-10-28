# Non-matching drafts: field/fldFileResolver

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## fldFileResolverFL1 (field/fldFileResolver, 0x240)

116 of 144 words match, at the exact 576-byte size.

Everything structural reproduces: the four name-table words copied into
`fldResolverFL1Data`, the `area == 0x18 && evtBitCheck(0x4F)` guard, the
'b'-before-the-dot rewrite over `j < 16 && s[j] != 0`, the `D_0055DA98` and
`D_0055DAA8` clear loop, and the `area < 0x1F4` load loop whose
`fldMakeFilePath` reads `fldGlobalBase.area` and `fldGlobalBase.floor + 1`
rather than anything out of the info header, which is what retail's `beql`
delay slot proves (on the fall-through `$5` is still `&fldGlobalBase`).

Two things remain. Retail tests `s != NULL` with `beql` and fills its delay
slot with a duplicated `i--`, having already put `extra = 0` in the `memset`
call's delay slot; ee-gcc emits `beqz` with `extra = 0` in its delay slot and
the decrement once, so `extra` lands in a caller-saved register instead of
retail's `$16`. Everything else differing is a register permutation that
follows from that: `$19`/`$20` for the `fldGlobalBase` high half, and
`$9`/`$10` against `$10`/`$16` for `extra` and the two character constants.

Ruled out: `extra = 0` written before the `memset` (gets retail's `beql` and
the callee-saved `extra`, but hoists `strcpy`'s first argument out of the loop
and comes out 8 bytes long); `s` fetched before the `memset`, and `extra = 0`
after `s`, both of which also overrun; `if (s == NULL) continue;` instead of
the positive test, and an inner `continue` for the empty-string case, the
first of which changes nothing and the second overruns. The alias-set lever
is what got the four name stores batched the way retail has them:
`fldResolverFL1Data` has to be an `extern u32 []` with casts at the
assignments, because a `char *[]` destination shares alias set with the
`char *name[4]` source and ee-gcc then serialises load and store in pairs
(that spelling is 40 words differing instead of 28).
