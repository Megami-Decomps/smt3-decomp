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
## func_00213190 (code_001FAE80, 0x50)

8 of 18 words, 72 against retail's 80 bytes. A `qsort` comparator over the
`u16` inherited-skill list: zeros sort last, otherwise ascending.

Retail copies the first argument into `$3` and reloads `*a` in the tail where
the draft reuses the halfword already loaded for the zero test, so the
pointer copy and the reload are the two missing words and every later
register is shifted by one. The two compares, the `movz` and the whole middle
block are already identical. GCSE sees the second load as available, and no
source shape tried stops it; the retail source must have made the two reads
different RTL, or the function was compiled with the pointer live for another
reason.

Ruled out: a `const u16 *` local at two scopes and declared both before and
after the `bv` local; `bv` as `u16`, `u32` and `s32`; the dereference written
as `*pa`, `pa[0]` and `*(const u16 *)a`; `u16 *` parameters instead of
`const void *` (the `qsort` call site fixes the signature); and nesting the
tail inside an `else` so the two compares sit in one block.

## func_002429C0 (code_001FAE80, 0x54)

19 of 21 words, at the exact 84-byte size. Sums `attr[].capacity` over the
leader's inherited-skill list.

The two differing words are one constant split inside loop strength
reduction. Retail starts the induction variable at `0xA80` and leaves `0xE`
in the load displacement; every shape tried starts it at `0x30` and puts
`0xA5E` in the displacement. Same sum, same instruction count, same
registers.

Ruled out: four struct bases for the skill block, `(cmbUnitSkills_t *)
cmbGlobalWork->party` with the array at `+0x3A`, a block struct at
`cmbGlobalWork + 0xA80` with the array at `+0xE`, the same reached as
`&party[0].unit.namecode[4]`, and the count read through one base with the
array read through the other; four declared sizes for the skill array, which
change nothing; `sum +=` against `sum = sum +`; and an explicit
`if (count == 0) return 0;` guard, which makes gcc hoist the bound and
produce a downcounter instead of re-reading it.

## rstDrawStatusRow (code_001FAE80, 0x5C)

21 of 23 words, at the exact 92-byte size. A pure call wrapper around
`cmpDrawStatusComEx`.

The two differing words are the `sra` halves of the two `s8` arguments that
travel on the stack, emitted in the opposite order: retail sign-extends the
second parameter and then the third, this emits the third and then the
second. Both `sll` halves, both stack stores, the eight register arguments
and the frame are identical, so nothing but the schedule is left.

Ruled out: `s8` parameters against `s32` parameters with `(s8)` casts at the
call; `(s32)` casts on the two stack arguments; `s32` locals copied from the
parameters and passed instead; an `s8` local for the first parameter; a local
for the colour constant; a `rstDrawWork_t *` local for the global; and
renaming the parameters, which changes nothing because the `sll` order
already matches.

## campDhGetEntryRatio (code_001FAE80, 0x60)

11 of 24 words, at the exact 96-byte size. Decrements a delay, then either
rolls a new ratio or advances the current one.

Retail copies the work pointer out of `$4` into `$3` as its first
instruction and then uses `$3` and `$5` throughout; this keeps it in `$4` and
uses `$3` for the decremented value, so every differing word is that one
rename. The structure, the `blez`/`bnez` arms, the float add and the shared
epilogue already match, and `entry = NULL;` after the call is what stops
ee-gcc sibcalling `campDhRollRatio` and gets the size right. Nothing in the
source reaches the pointer's register: it is not live across the call, so
gcc has no reason to copy it and every shape tried declines to.

Ruled out: `s32` and `void` return types; a `campDhEntry_t *` local for the
parameter, declared with and without a separate assignment; assigning NULL
to the local against assigning it to the parameter; an `if`/`else if`/`else`
chain instead of early returns; and an `s32 next` local holding each loaded
value instead of `--` on the field.

## cmpFindKugatatiMsgEntry (code_001FAE80, 0x60)

12 of 24 words, at the exact 96-byte size. Scans a devil's 24 level-up
records for kind 5 and returns that record's value, or -1.

Every instruction matches; the twelve differing words are two register pairs
exchanged. Retail loads the id into `$5` and reuses the parameter register
`$4` for the loop index; this loads the id into `$2` and puts the index in
`$5`, which shifts the multiply chain's temporaries with it. Nothing in the
source reaches that choice: the parameter is dead after the single `lhu`
either way.

The record walk itself is settled. The induction pointer is based one record
early and the fields are read at `entry[1]`, which is what puts 0x6 and 0x7
in the load displacements against a pointer stepping by 4.

Ruled out: a `u16` and a `u32` local for the id, declared before and after
the entry pointer; `unit->id * 0x66` against `0x66 * unit->id`; biasing the
base by 4 and reading `entry->kind` with no `[1]`; `entry[i + 1]` indexing
instead of a walking pointer; and a typed `datLevelUpTable` array with an
`asm` alias, which costs one more word.

