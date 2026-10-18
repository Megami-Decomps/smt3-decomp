# Non-matching drafts

Reconstructions that are close but not exact, kept in the source behind
`#ifdef NON_MATCHING` so the next person starts from the draft instead of
from scratch. The build never defines `NON_MATCHING`, takes the assembly
fallback, and stays byte-identical; the function still counts as unmatched.

The source carries one line saying how close the draft is and what differs.
This page carries the rest: what the remaining difference actually is, and
which source shapes have already been ruled out. Read the entry before
picking a draft up, or you will repeat the search.

A draft is not evidence of anything. It is not compiled by the build, so
`python3 tools/check_nonmatching.py` compiles every draft and checks that each
has an entry here.

## fldSkyDrawFilter (field/fldDraw, 0xAD8)

682 of 694 words, at the exact 2776-byte size. Transliterated from
dds-decomp's `fldDrawSkyGradient`, the same function at the same size, which
is the one dds pairing that has held exactly.

The remaining twelve words are three reload spill slots rotated. Retail
numbers them `0x118` `fldSkyDataTable`, `0x11C` `row*8`, `0x120` `row*32`;
the draft gives `row*8`, `row*32`, `fldSkyDataTable`. Every instruction is
otherwise identical, so it is purely which pseudo-register gets which slot.
`-fno-gcse` moves 653 of the 682 words and `-fno-strength-reduce` changes
nothing, which puts the mechanism in GCSE and reload pseudo numbering rather
than anything the source can state. Someone attacking this needs to change
what order the pseudos are created in, not what the source says.

Ruled out: a `fldSkyData_t *` local at four scopes, which gcc folds back to
the global with byte-identical output; the flattened colour array; pointer
and `(p + i)->` subscript forms; `<<` for `*`; a `mode` local for the switch;
an if/else chain for the switch; `u32` loop indices; a row-base `s32 *` with
`cur[column+9]` and `cur[column+10]`; alpha extracted before rgb; hoisted `x`
and `y`; and nine declaration-order permutations.

Confirmed on the way: `fldSkyDataTable`'s row stride is
`{s32 mode; s32 colors[8][9];}`, 292 bytes; `rsetOtOfs` takes `0x39` here
where DDS1 passes `0x44`; the sway offset global is `fldRunDanteGunFlag[14]`,
not `[16]`; and the clamp list is three alphas with `a3` unclamped.

## fldStatusSet (field/fldDraw, 0xF8)

36 of 62 words, at the exact 248-byte size.

Retail carries two induction variables where the draft has one: an integer
offset added to `DDS3_GBWK` serving `hp` and the first `badstatus` read, and
a pointer biased at the `badstatus` field serving the flag at `-0xE`. The
body never uses the loop index.

Ruled out: the typed `akuma[i]` form throughout; a `fldAnahoriAkuma_t *`
local at three scopes; all eight cast orders of `(u8 *)base + ofs` against
`(u8 *)ofs + (u32)base`, which gcc folds together here; a `u16 *` walking the
`badstatus` field with the flag at `p[-0xE]`; and an explicit second pointer
induction.

This draft proved `fldAnahoriAkuma_t.badstatus` at `+0x12`: retail tests that
`u16` against 0x40, 0x100, 0x20 and 0x8000, and `datCheckDying` is called on
`&akuma[0].flag`, which makes the record `datUnitWork_t` shifted by four and
puts `badstatus` at `0x0E + 4`.

## nbCameraBoss_BSKAGU_EP_IT_Set (code_001B0858, 0x144)

19 of 79 words before the first difference. The four-case switch and all four
camera store lists are exact; only the selector that feeds the switch differs.

Retail computes the selector in two arms that both end with the same
`movz $4, $3, $2`, so the arms cross-jump onto one shared copy of it: each arm
sets `$3` to the value for an even draw (3 or 2) and `$4` to the value for an
odd one (0 or 1), then falls into the shared `movz`. ee-gcc here emits a
`movn` inside each arm instead, with the result register preset to the even
value and the odd value moved in. Because the moved operand differs between
the arms (`$0` against `$17`), the two `movn`s are not identical and do not
cross-jump, so the arm bodies stay separate and the allocation rotates from
there on. The needed form is an `if_then_else` whose RTL condition is `EQ`,
which gcc's folding turns back into `NE` for every source spelling tried.

Ruled out: `(x & 1) == 0 ? 3 : 0`, `!(x & 1) ? 3 : 0`, `((x & 1) ^ 1) ? 3 : 0`,
an explicit `if`/`else` per arm, a `u32` temp holding the masked draw, and
initialising the selector to 0 before the `if`.

## nbCameraBoss_BSSAMA_Command_Set (code_001B0858, 0x1CC)

25 of 115 words before the first difference, 8 bytes short of the retail size.
The instruction list is otherwise the same work in the same order.

Retail forms the address of the local `center` vector twice, `addiu $2, $29,
0x20` for `SDF_VU0_STOREVECTOR` and `addiu $3, $29, 0x20` again for the
`SDF_VU0_LOADVECTOR` after `effBTLModelGetHeight`. ee-gcc here CSEs the two
into one pseudo that is live across that call, so it lands in `$16` and the
two `addiu`s collapse to one. That costs a callee-saved register and rotates
the float registers in the `pBase->h * pBase->scl * 0.35f` multiply just
above. Both `addiu`s are `$29`-relative, so the equivalence class survives
the call in cse.c and nothing in the source appears to break it.

Ruled out: a local for the height instead of negating the call result in
place, both multiply operand orders, `+=` against an explicit
`target.y = target.y + ...`, reordered local declarations, and the
`_MEMOPS` VU0 variants (which address the frame directly and drop the
`addiu` entirely).

## nbCameraBoss_Sukady_DAIJIS_Set (code_001B0858, 0x134)

4 of 77 words, at the exact 308-byte size and with the same instruction list.

In case 0 the camera's `direction.x` comes from a `lui`/`%lo` global rather
than a `.lit4` literal. Retail schedules that `lwc1` fifth among the six value
loads, after the four `.lit4` loads for `direction.y`, `direction.z`,
`target.x` and `target.y`; this emits it second, right after `direction.y`, so
four loads rotate. The `lui` is already in the `beqz` delay slot in both, and
the store order and every other case are exact, which leaves only where the
scheduler places that one load.

Ruled out: declaring the symbol `extern f32 D_0054F3A0[]`, `extern f32
D_0054F3A0[4]` and `extern sdfVector4_t D_0054F3A0`, read as `[0]`, `*p` and
`.x` respectively.
