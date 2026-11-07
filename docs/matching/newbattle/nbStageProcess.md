# Matching notes: newbattle/nbStageProcess

## nbSetNextOrder (newbattle/nbStageProcess, 0xAC)

10 of 44 words match, 176 bytes against the native 172.

Retail recomputes `data + 8 + (s8)orderindex` with an `addu` on every
iteration and reads the order byte at `0x40` of it. The draft strength-reduces
that to an induction pointer set up in the loop preheader, which adds the
preheader block and the extra 4 bytes. Retail also holds the counter in `$4`
across the whole function, loading the initial `orderindex` into `$2` and
copying it, where the draft loads straight into `$4`.

Ruled out: `data->orderindex` used directly in the loop rather than an `s8`
local; an `s32` local, which drops the `sll 24`/`sra 24` pair retail has; and
a `do`/`while` on `data->order[data->orderindex] < 0`.

## CheckSenseiNego (newbattle/nbStageProcess, 0x130)

57 of 74 words differ, 296 bytes against the native 300.

The guards are right: the stage-major range test folds to retail's
`addiu -0xF0`/`sltiu 7`, the four `data->stat` bit tests come out in retail's
order, and the age-of-moon test is `== 8` returning 0, not `!= 8`.

The missing word is the empty delay slot after `jal evtGetAgeOfMoon`: retail
leaves a `nop` there and materialises the 8 afterwards, while the draft fills
the slot with `li $3, 8`. Retail then tests with `bnel` and annuls the
`work = a->work` load into the branch's own slot. With one word fewer the
draft's `a` lands in `$16` and `rate` in `$17`, where retail has them the
other way round, which is what most of the 57 words are.

Ruled out: `goto` to one trailing `return 0` (69 words, worse); both
polarities of the `0x2000000` test; `rate` preset to 0x64 before the test; and
an intermediate local for the `21 / enemypcnt` quotient.

## CheckAction_STKSEL (newbattle/nbActionProcess, 0xE8)

58 of 58 words match at 232 bytes. The current baseline had only the two
mask/counter setup words left; the older 49/58 result in this note was superseded. An ordinary initialized `u32 clearMask = ~8u` value, assigned
before forming the stock-form pointer, supplies the existing sixteen `stat`
clears. This deliberate reconstruction aid prevents late loop-invariant
materialization; it is not a claim about the original source. No register
binding or inline assembly is needed.

The existing default / -1 / -2 switch order and all calls, stores and return
values remain unchanged. The shift now uses unsigned `1u`, including bit 31,
and still reloads `StkSelId` after both calls rather than caching it. In normal
initialized roster state, sixteen-slot stock IDs are 0..15; selector startup
is 0, initialization writes -1, and polling returns -2, -1 or a stock ID.
Thus the reloaded value plus 16 is in 14..31. This is a state invariant, not
validation of corrupt saves or arbitrary bytes.

The empty-list edge was checked against native layout rather than assumed
unreachable: UP can leave cursor -1, and confirm reads the word immediately
before `StockIDList`, the fourth panel's MP-animation counter (0..8); cursor
0 reads a zero-initialized or previously valid table entry. Native navigation
keeps an empty list's top at 0 and cursor in {-1, 0}. This existing out-of-bounds
UI quirk is not repaired or justified as a C array access here; its native
producer result still fits the selector shift domain. Counter initialization,
positive-only decrement, reset and assignment of 8 were independently audited.

Qualification: complete owner 87/0, with the nine remaining guarded drafts
still compiling. The clear loop visits exactly sixteen `u32 stat` fields and
preserves the native mask, pointer stride and memory access order.

## CheckAction_ESCAPE (newbattle/nbActionProcess, 0xE4)

48 of 57 words match, at the exact 228-byte size. Fresh complete-owner
qualification retains the other 89 C matches.

A `switch ((u16)((u16)a->seqtime / 15))` over cases 1 and 2 with a `goto` to
one trailing `return 0` gives retail's divide trap, dispatch and both arms.

Retail uses `$5` for dispatch constant 1 and the first arm's endtype constant
3; the draft uses `$4`. The second arm's constant 3 already correctly uses
`$4`. Qualified compiler dumps establish separate real comparison consumers
and deaths before calls, not a common constant lifetime.

The `SetTimeList` argument setup is an independently unresolved residual:
retail puts `daddu $4, $16, $0` in the call delay slot, whereas the draft moves
that real argument earlier and puts the final constant argument in the slot.
A scoped binding of only the first arm's actual `u32` comparison constant to
`$5` leaves this setup unchanged, swaps the data/endtype load registers, and
worsens the result to 47/57 at 228 bytes. An ordinary `u16` stage local with
`if`/`else if` dispatch yields 14/55 at 220 bytes. Both controls retain the
other 89 C matches. Plain versus diagnostic compiler outputs agree on
allocated sections, non-file symbols, relocations and normalized assembly for the
baseline and both controls; neither control is a publishable match.

Historical negatives remain: early `return 0` per arm instead of the `goto`;
no `default` arm; an `s32`, `u16` and `u32` local for the switch value; and
swapping the two case bodies' source order.

## nbDrawKoukaSprite (newbattle/nbStageProcess, 0x108)

24 of 66 words differ, at the exact 264-byte size and with the same instruction
multiset.

The digit loop, the `n == 0` fixup and the draw loop are all exact. What is
left is the three-way test on `align`: retail fills the `beqz`, `bgtz` and
`beq` delay slots with `sll $2, $16, 1` (the `n * 2` the two bodies share) and
a `-1` materialisation, and places the `n * 96` block ahead of the `n * 192`
block. The draft hoists the draw loop's `n--` into those slots instead, which
forces `bgtzl` and `beql` where retail has the plain branches, and lays the
`n * 192` block first.

The test chain itself is right as nested `if`s: `if (align != 0) { if (align <=
0) { if (align == -1) ... } } else { x -= n * 96; }` gives retail's `beqz`,
`bgtz`, `beq` with the two bodies cross-jumped onto one `subu`.

Ruled out: a `switch (align)` over 0 and -1, which ee-gcc 2.96 leaves as an
ascending compare list (two case nodes are below the balancing threshold) and
tests -1 first; the same switch with an empty `case 1:` or `case 2:` added to
raise the node count, both of which run over size; `default: break;`; the
`else if (align <= 0)` spelling of the same chain; `align <= 0 && align == -1`
fused into one condition; and four spellings of the draw loop (`for (n = n - 1;
...)`, `for (n--; ...)`, `while (--n >= 0)`, a separate index local, and an
`if (n > 0)` wrapping a `do`/`while`), none of which keep the decrement out of
the delay slots.

RunSake findings now live with its actual owner in [nbUnitProcess](nbUnitProcess.md#runsake-newbattlenbunitprocess-0x104).

## CheckAction_TARSEL (newbattle/nbActionProcess, 0x1CC)

Exact: 115 of 115 words, 460 bytes. The dispatch, callback-visible reads,
error handling and sixteen formation-status clears retain their native behavior.

The former draft differed in four register operands: the error code `4` used
in a comparison after `nbDispSkillErrorMessage`, and the sequence `1` stored
after `nbSetCommSelStat`. An ordinary block-local `nextSequence = 1` restores
the latter. The error code needs a localized `FAKEMATCH_ASM_REG` binding to
`$2`, initialized after the message callback and consumed by the comparison
before any further call. It is a compiler-allocation reconstruction aid, not
evidence of original source spelling. No empty operand constraint is needed.

Ablation in the complete current owner: ordinary named locals match 113/115;
only the error binding matches 115/115; only the sequence binding remains
113/115; both bindings also match 115/115. The redundant sequence binding and
both diagnostic empty operands were removed. No ABI, load/store width, callback
order, return value or extra runtime operation changes.

The existing source shape remains important: `nowcommand != 1` encloses the
`== 5` arm with `no = -1` above it, so retail's `== 1` arm stays last; the
sixteen `form[i].stat &= ~8` clears use the plain ascending loop.

## SetDamagePacket (newbattle/nbActionProcess, 0x1E0)

Exact: 120 of 120 words, 480 bytes, ordinary C. A block-local `s32 moji = 1`
immediately before `nbMakeDamagePacket`, used for its real eleventh argument,
restores the native constant schedule and register. It is initialized after
`CheckTimeListDead` and consumed by the stack-argument store before the packet
call. No extra load, store, call, conversion or ABI change is introduced.

The old literal form was 116/120. A diagnostic `$3` binding with an empty
operand constraint reached 120/120; removing the operand still matched, and
removing the binding also matched. The final source retains only the ordinary
block-local constant. Earlier named-local experiments recorded here did not
establish this exact scoped source in the current complete owner context.

Both kuisibari guards remain intact, including separate `GetTimeListHp` calls
for comparison and subtraction, the skill `else if`, and the dead arm before
the live arm. Native callback reloads, stack-argument spacing, and the final
`SetTimeList` sibling call remain unchanged.

## StageLoad (newbattle/nbStageProcess, 0x1B4)

109 of 109 words match at the native 436-byte size. The complete owner checks
16 match / 0 differ, including owned data. The existing ordinary reconstruction
is promoted with one target-specific VU0 handoff adjustment; no register binding,
empty operand, new macro, layout, ABI or scalar computation is added.

`FromEuler` produces the actual quaternion in VU0. The existing
`SDF_VU0_STOREVECTOR_NOMEM(&q)` writes its four lanes to the real stack object
immediately before `dds3SetRotPoseSet(camera, &q)`. That external provider uses an EE `lq`/`sq` copy (not a VU0 copy)
to read all four lanes into the pose rotation. `FromEuler` produces all four
lanes, and the store precedes this consumer without an intervening clobber.
There are no later C reads of `q` elements. The actual distinct frame slots
are position at 0x00, rotation at 0x10 and quaternion at 0x20; this confirms
the emitted alignment without claiming the type itself requires 16-byte alignment.
The bare-address macro declares no memory output; this local memory audit and
exact output are essential to the target-specific qualification. This is a
qualified bare-address hardware handoff, not a general memory-barrier guarantee
or evidence of original source spelling. The earlier `memset` and `q.w` store,
all callbacks, state observations and native vector accesses remain intact.

The paired memory-clobbered store emits 436 bytes with 97/109 words matching.
The explicit full-vector output `SDF_VU0_STOREVECTOR_F32(&q)` also gives
97/109 at 436 bytes, with the same twelve residuals; the modeled-output
alternative therefore does not remove this target-specific dependency.
The bare-address form retains the native store address and separately materialized
call argument, resolving the two saved-register choices and all twelve residual
words. Whole-owner plain and diagnostic builds have identical allocated and
relocation sections and normalized assembly. Independent native/provider and
handoff review plus final retail and current regression gates are required
before publication.

### Historical residual and rejected controls

97 of 109 words match, at the exact 436-byte size and with zero instruction
differences in the multiset.

The shape is settled. The three stack vectors are `pos`, `rot` and `q`
declared in that order, which puts them at the frame's 0x00, 0x10 and 0x20;
both constant vectors arrive by `memcpy` from `extern char D_00543668[]` and
`D_00543678[]`, whose alignment of 1 is what gives retail's unaligned
`ldl`/`ldr` and `sdl`/`sdr` block moves rather than an `lq`/`sq` pair; `q` is
zeroed by a real `jal memset` and then gets `q.w = 1.0f`; and the dispatch is
`s32 state = nbStageState.unk00; switch (state)` with `case 0:` and a shared
`case 1: case 2:`, the `s32` local being what stops gcc dropping the `bltz`
low-bound test that an `unsigned char` index would make dead.

What is left is that `&rot` and `&q` swap between `$18` and `$19`. Retail
holds `&rot` in `$18` with three references of its own (the four `sdl`/`sdr`
words and the `lqc2`) and `&q` in `$19` with two (the `memset` argument and
the `sqc2`), rematerialising the `dds3SetRotPoseSet` argument as
`addiu $5, $29, 0x20`. Every source shape tried lets cse share that argument
with the `&q` pseudo, which raises `&q`'s reference count to the same
`floor_log2` bucket as `&rot`'s and flips the two registers.

Also measured: `-0.34906585f` and `0.69813165f` are the spellings that give
retail's `0xbeb2b8c2` and `0x3f32b8c1`, since cc1 truncates rather than rounds
and the nearest-decimal forms land one ulp low.

Ruled out: `SDF_VU0_STOREVECTOR_MEMOPS(q)`, which does assign the two
registers retail's way but spells the store `sqc2 $vf10, 0x20($29)` and still
copies the call argument from `$19` (two words); `SDF_VU0_LOADVECTOR_MEMOPS`;
`(u8 *)` casts on either macro operand; `&q.x`, a `(u128 *)` cast and a
`(void *)` cast at the call; a `sdfVector4_t *qp` local used for the memset,
the store, or the call; an `f32 q[4]`; all 24 orders of the four
initialisation statements; all six declaration orders of the three vectors
(which only move the frame slots); moving either initialisation into the
`case 0` arm; and `void *`, `u128 *`, `f32 *` and absent prototypes for
`dds3SetRotPoseSet`.
