# Matching notes: newbattle/nbMakePacket

## Flow1_11 (newbattle/NbNegoFlowOne, 0xC8)

50 of 50 words match in ordinary C at the native 200 bytes. The earlier 192-byte statement
predated the current owner context and was not an exact native-size result.

Case zero sets the 30-frame wait, then breaks to the shared zero return; the
default also breaks. In case one, the stage-major boss-message arm returns one
immediately. The enemy-ID and ordinary message arms retain the common case-one
return. No declarations, accesses, provider signatures or callback ABI changed.

Qualified full-owner ee-gcc 2.96 dumps show the effect: `25.sched2` still has
three message calls; `27.jump2` redirects the enemy-ID arm to the stage-major
call (call insn 62, new label 241), while retaining the separate ordinary-message
call (174). `29.dbr` leaves the shared call slot empty and fills the backward
branch with the message-number subtraction. Returning one separately from all
three arms instead merges all three calls and does not match. The original
source spelling remains unknown.

Plain and dumped complete owner assembly and literal entire object ELF agree
for the guarded source, the exposed old draft, and the candidate, in production,
`SKIP_ASM`, and `SKIP_ASM` plus `NON_MATCHING` modes. The only normalization is
the exact observed equal-length random source path. Complete allocated section
contents match the guarded owner, and independent complete-owner
checks report 40 match, 0 differ. The callback returns zero outside phase one
and one after each message, as consumed by `nbNegoFlowOne` through
`D_004EF4B0`.

## nbClearActionFlags (newbattle/nbMakePacket, 0xE0)

18 of 56 words match, at the exact 224-byte size and with the same instruction
multiset.

The structure is settled: a `mask = ~0x800;` local above the first loop so the
constant is materialised before the counter, `for (i = 3; i >= 0; i--)` over
four formations clearing `stat`, then a second loop that remembers the last
index whose `stat & 1` was set and, at the first formation without it, copies
that formation's `orgpos` and `orgrot` with four `SDF_VU0_COPYVECTOR`s,
adds `form[src].r + 100.0f + 50.0f` to `orgpos.x`, sets `stat |= 0x800` and
returns. Both float constants are `lui`/`mtc1` pairs, not `.lit4` entries.

What is left is which call-clobbered register each of the four locals gets:
retail has `mask` in `$6`, the counter in `$5` and the pointer in `$3`, and
ee-gcc permutes all three, which rewrites every word that names them. There
are no calls in the function, so this is local-alloc ordering; four
declaration orders leave it unchanged. This is the ruled-out
register-permutation class, kept only so the next reader does not redo the
decode.

## Flow10_21 (newbattle/nbMakePacket, 0x98)

11 of 37 words differ, one word short of retail's 152.

Both `nbAddNegoIdKouho` ids, the three-way `ework->id` chain over `0x5C`,
`0x3D` and `0x61` (plain `beq`s, since the values are not contiguous), the
`evtBitCheck(8) == 0` polarity and the `(u32)dds3GetRandIntA(0, 10) < 5`
unsigned test are all confirmed; the first 26 words are identical. The whole
condition has to be one `||` of the two disjuncts assigning
`nextkouho[1]`, with `nextkouho[0]` in the `else`: an early `return 0` after
the first assignment duplicates the tail and runs eight bytes over.

What is left is the rand test's polarity in the emitted code. Retail puts the
`sltiu` result in `$3` and branches `bnez $3` to the `nextkouho[1]` block,
which it places last; ee-gcc puts it in `$2` and emits `beql $2, $0`, keeping
`nextkouho[1]` as the fall-through, which shifts the two branch offsets. The
`>= 5` spelling with the arms swapped and an explicit `bit` local for the
`evtBitCheck` result are both much worse (10 of 38 over size, and 24 of 38).

## Flow10_10 (newbattle/nbMakePacket, 0x90)

15 of 35 words differ, one word short of retail's 144.

The `nbAddNegoIdKouho(n, 0x6004A)` call, the three stores (`workid[6] =
0x60009`, `cnt6_36 = 0`, `cnt6_37 = 0`) and both message arms are right, and so
is the single shared `nbDispNegoMessage` call fed by `mes` and `type` locals
set in each arm: writing the call out in both arms instead makes ee-gcc
duplicate the `jal` where retail shares it, which is worse.

What is left is that retail duplicates the fourth argument's `daddu $7, $0, $0`
into the `else` arm as well as setting it before the dispatch, and materialises
`0xD9` ahead of the three stores rather than inside the arm. Assigning the
`else` arm's values as defaults before the test instead reaches only 23 of 32
words, because the `flowmsg` load then happens before the stores.

## nbCheckBattleSkillError1 (newbattle/nbSkillError, 0x48)

1 of 18 words differ, at the exact 72-byte size. The nested signed range checks
accept skills 0x31 and 0x32; a zero `nbFindStockSkillError()` result then returns
2, and every other path returns 0.

The remaining word is the lower-bound gate at +0x14, before the helper call:
retail uses `bnez $2, +9` (`0x14400009`) and C emits `bnel $2, $0, +9`
(`0x54400009`). Both test the same `nskill < 0x31` boolean and target. Retail
executes the delay-slot zero of `$2` on the call path; the likely branch annuls
it there, and the helper overwrites that value. The residual is branch scheduling,
not a reversed range condition.

`nbFindStockSkillError` is already ordinary C in `newbattle/nbEventProcess.c`
and matches its full 88-byte extent. It keeps separate overlapping unit/status
views, an index and a byte offset. The old advice to decompile that helper is
obsolete: it is compiled in a separate unit, and requalifying the current caller
still leaves this one word.

An initialized common result emits 56 bytes; early-return guard clauses emit
64. A single-pass validation block with common failure breaks reaches 72 bytes
but differs in five words, including the likely branch. Those forms do not close
the existing source. Keep the typed range reconstruction; further changes need
evidence about the caller's scheduling or original compilation context.

## nbSetBadCurseTimer (newbattle/nbEncProcess, 0x2C)

2 of 11 words differ, at the exact 44-byte size.

The reads, the increment, both stores and the `gp`-relative write-back are all
identical. Retail computes the row once, `addu $7, $2, $3` with the index
first, and reaches the `step` store through a plain copy of it,
`daddu $2, $7, $0`; ee-gcc computes the row twice instead, once in each operand
order, so the two words are that second `addu` and the missing copy.

Ruled out: the cast form `(u8 *)(i * 8) + (u32)nbBadCurseTable` for one, the
other or both accesses (one address and no copy, four bytes short); a
`&nbBadCurseTable[i]` pointer local; two pointer locals with an explicit copy
between them; `*(s32 *)p` for the `step` store; `nbBadCurseTimer++` in place of
the index local; and both store orders, which only swap the registers the
timer and the table base get.

A current `nbEncProcess` trace with `--cflag -DNON_MATCHING --cse --alloc
--no-dumps --func nbSetBadCurseTimer` passes hooked/unhooked/plain output
parity (`verify: OK`). CSE1 reverses the operands of the first row address
UID21 to `index + base`. For the second member it first forms `base + 4`
(UID34), then `index + (base + 4)` (UID38); CSE2 retains both address
expressions. Local allocation gives the first address quantity register 2
and the second register 7. There is no shared row-address pseudo whose copy
allocation could preserve: the distinction already exists before allocation.
Assigning the actual two-member, eight-byte record as an aggregate instead
introduces a stack temporary and emits 68 bytes, so it does not reproduce
the native shared address and copy. No additional source variant is retained.

## Flow5_6, Flow5_18 (newbattle/nbMakePacket, 0xD8 each)

23 of 52 words match each, two words short of retail's 216.

Both are the same body with a different `nbAddNegoIdKouho` id (`0x50009` and
`0x50015`): a `for (i = 0; i < 6; i++)` over `D_003E3158[n->innen_id].phase[i]`
that stops at the first zero `item`, stores it into `n->getitem` and passes it
with the neighbouring `cnt` to `datAddItem`. ee-gcc peels the first test into
the preheader exactly as retail does, hoists `&D_003E3158[0].phase[0]` as the
one loop invariant, and re-reads `n->innen_id` at the loop bottom.

The `cnt` argument has to be written
`((nbInnenPhase_t *)&D_003E3158[n->innen_id].phase[i])->cnt`. Written as the
plain `.cnt` field the `+2` folds into the hoisted base, so gcc keeps a second
loop invariant at `symbol + 0xA`, spends a fourth saved register on it and
grows the frame by 16 (9 of 54 words). The cast keeps the association
`(base + index * 4) + 2`, which is retail's `lhu $5, 2(reg)`.

The two remaining words are both register copies ee-gcc coalesces away:
retail computes the element address into `$3`, copies it to `$2`
(`daddu $2, $3, $0`) for the `cnt` load, and at the loop bottom loads
`n->innen_id` into `$2` and copies it to `$4`. The draft loads straight into
the register it uses in both places. That is the ruled-out
register-allocation class, and every word that differs after `+0x3C` is one of
those two copies or an offset shifted by them.

Ruled out: an `item` local for the value stored and passed; `n->getitem` read
back as the first argument; an `nbInnenPhase_t *` element pointer assigned in
the loop, which lets gcc strength-reduce the whole address and comes out 180
bytes against retail's 216; a `u16 *` cast plus `+2` arithmetic; a `while`
loop with the `i >= 6` break at the bottom; and an `id` local for
`n->innen_id`.

Confirmed on the way: `nbInnenRec_t`'s `phase[13]` entries are
`{u16 item; u16 cnt;}`, not `{u16 item; u16 pad;}`: retail passes the second
halfword as `datAddItem`'s count argument.

## Flow6_37 (newbattle/NbNegoFlowTwo, 0x10C)

67 of 67 words match at the native 268-byte size. The former `nbMakePacket`
owner label is historical; the configured owner is `NbNegoFlowTwo`.

The ordinary draft already recovered the two candidate-registration calls,
four-way enemy-ID test, early event/type/count guard, level snapshots and
nested event/level test. It matched 58/67 words. The six previously tested
statement orders remain negative controls (9, 10, 10, 10, 12, 12 differences);
combining the nested event/level test into `&&` with `else` regressed to 22.

The remaining nine words are local register allocation: the first choice
load/store uses native `$4`, while the compare constant 3 uses `$3` and the
player-work pointer uses `$2`. An ordinary scoped `u32` choice retains the
58/67 baseline. Binding only that first actual choice load/store to `$4`
fixes the six early words (64/67); independently binding only the final
post-event choice load/store fixes the three late words (61/67). Both
separate scopes are required for 67/67. No pointer or compare constant is
bound, and no empty operand template is needed.

Both choice fields are `u32`; each initialized local is consumed by the
actual `nextnegoid` store. Qualified compiler snapshots show the initial
choice dying at that store, before the next `evtBitCheck` argument setup,
and the final choice freshly loaded after the callback and dying at its
store. No fixed-register value survives a call. Level reads remain zero-
extended `u16` snapshots, and the signed `plevel - 17` comparison is safe
for their entire range. Callback reloads, native access widths, branch paths
and early returns are preserved. These bindings are target-specific compiler
constraints, not evidence of the original source spelling.

Whole-owner verification reports 72 matching C functions, no differences.
Plain versus diagnostic builds agree on all four allocated sections,
451 non-file symbols, 358 ordered relocations and normalized assembly;
all non-text allocated sections, symbols and relocations also remain
identical to baseline. `-Wuninitialized` is silent and preserves output.
Strict implicit-declaration checking still fails on the unchanged unrelated
`Flow6_22` call to `datCheckMaka`, in baseline and candidate alike; no global
strict-warning-clean claim is made. Full retail checksum, byte comparison,
repository regressions and fresh guarded-owner checks pass.

## Flow6_43 (newbattle/nbMakePacket, 0x10C)

45 of 68 words match, one word over retail's 268.

The two-case `switch` on `nownegophase`, both `nbDispNegoMessage` message
numbers (`0x4E`, `0x61`, `0x62`), the two `nbAddNegoIdKouho` ids and the
`n->data->enemypcnt == 1 && n->cnt6_36 >= 4 && (u32)dds3GetRandIntA(0, 0xA) < 6`
chain are all confirmed, as is the `enemypcnt` test reusing the phase
register that `case 1` already knows holds 1.

The extra word is the `flowmsg[ekucyo]` load in the `0x62` arm. Retail emits
that load three times, twice annulled into the two `bnel` delay slots and once
in the plain `beqz`'s slot, and the `0x62` block then reads `$2` straight from
whichever predecessor's slot set it; ee-gcc keeps all three slot copies and
re-emits the load at the top of the block as well. That is the delay-slot
filler's redundant-insn elimination, not a source shape, and it shifts every
branch offset after it.

Ruled out: nested `if`s in place of the `&&` chain (identical), and the two
`nbDispNegoMessage` calls shared through a `mes` local, which hoists the
argument setup above the test.

## Flow7_20 (newbattle/NbNegoFlowQuestion, 0x130)

76 of 76 words match at the native 304-byte size. See the current
[negotiation-question note](NbNegoFlowQuestion.md) for the qualified live
complement constraints, semantic review and closed ordinary controls.

## Flow10_1 (newbattle/nbMakePacket, 0x100)

15 of 63 words differ, one word short of retail's 64.

Three arms each need their own `return 0`, and the last test is written
inverted (`if (datGetRace(n->ework) != 0x21) return 0;`) to get retail's
`bne` with the zero in its delay slot. The `dds3GetRandIntA(0, 0xA)`
comparison is unsigned, as the `sltiu` shows.

What is left is where the `nextnegoid` store goes. Retail keeps the first
arm's and the ternary's values in `$3` with `daddu $2, $0, $0` scheduled
in front of each store, so neither store cross-jumps with the last arm's;
ee-gcc puts every value in `$2` and merges the first arm's store into the
shared tail. Retail's first arm also returns the zero that
`evtBitCheck(8) == 0` left in `$2` through an annulled `bnel` slot rather
than materialising it.

Ruled out: the ternary as an `if`/`else` pair of stores; no early return
in the first two arms (33 of 60 differ); and the ternary without an early
return.

## Flow9_16 (newbattle/nbMakePacket, 0x168)

45 of 89 words differ, one word short of retail's 90.

Same residual as Flow1_29: retail materialises `return 0` in both of the
dispatch delay slots and keeps three exit labels, because `case 0` reaches
the epilogue with `$2` already zero from `datCheckItemFull`'s test.
ee-gcc annuls each arm's first instruction into those slots instead
(`beqzl`/`beql` for retail's `beqz`/`beq`), which leaves the `b` to take
`ld $16` and shifts everything after the dispatch by one word.

Ruled out: all six orders of the three case arms.

## Flow6_65 (newbattle/nbMakePacket, 0x190)

79 of 100 words differ, four words long.

The `datGetRace(n->ework) != 0x13` arm is written first so the `== 0x13`
arm is the branched-to one, which is retail's polarity. Two residuals
remain and neither moved under the source forms tried.

ee-gcc if-converts the `== 0x13` arm's `aisyo` ladder into two `movz`,
where retail keeps it branchy and cross-jumps its `aisyo == 2` test with
the other arm's. And the allocator gives `ritu` `$17` and the
`datGetParam` result `$18`, which is retail's pair swapped; `ritu` is
already assigned in front of the leading call, so it does get a
callee-saved register, just the wrong one.

Ruled out: `ppar`/`epar` declared before `ritu`, declared as block locals
inside the `if`, and declared in the other order; and the second arm
written as a braced `else` block rather than a flattened `else if`. The
documented rule that declaration order is inert for register allocation
holds here.

## nbCollectBadCurseUnits (newbattle/nbEncProcess, 0xA4)

23 of 41 words match at the exact 164-byte size; first difference is at
entry. The guarded body scans the 16 formation records for status bit 4,
collects signed-byte indices, chooses one with `dds3GetRandIntA`, then
reloads the action's formation pointer and sets status bit 8.

The native stack frame is 0x30 bytes; the typed 16-entry candidate list
emits a 0x20-byte frame. An earlier `s8 list[32]` draft reproduced the
frame but had no independent evidence for that capacity. The observed
scan establishes 16 entries, so the preferred body retains that bound
rather than enlarging it to obtain stack space. The remaining differences
are frame/save offsets and allocation of the scan and selected-index
registers. A source-level reason for the extra native stack space remains
unresolved. The earlier integer pointer arithmetic and explicit byte
offset have been replaced with indexing through the existing formation
type; no new layout or field extent is introduced.

## ChkCost (newbattle/nbAi, 0xC8)

50 of 50 words match in ordinary C. Command 0 checks skill 0; commands 1 and 5
check the unsigned current skill. Other signed command values return zero
without calling the cost provider. After a nonzero cost result, the work pointer
is reloaded before the unit ID and unsigned AI level are read. Levels 1 and 2
reject the action; level 0 and levels above 2 do not.

The level test is a grouped `switch` with cases 0, 1 and 2 assigning
`ret = ailevel != 0`. This reflects the same real level cases used by adjacent
`ChkClose`. The former `if (ailevel < 3)` draft omitted retail's `xori ..., 0`
and changed scheduling. The switch produces that instruction without identity
arithmetic, dummy code or constraints. Qualified compiler snapshots first show
the XOR after conditional-move conversion (`15.ce`), absent through combine;
this locates the transformation boundary, not the original source spelling or
an independently hooked compiler decision.

Independent review verifies signed command/form-index loads, unsigned skill,
unit ID and level loads, the 164-byte AI record stride, the exact cost-provider
signature, post-call observations and the caller's rejection test. Complete
owner checking gives 48 match, 0 differ. All allocated production code and data
bytes equal the assembly-backed baseline. Nine complete-owner plain/dump pairs
cover guarded, former draft and final source in production, SKIP_ASM and
retained modes; full assembly and literal entire ELF objects agree after only
the exact randomized source pathname replacement. Required retained strict
compilation passes. Optional strict production/SKIP_ASM checks still report
inherited missing declarations for `nbPickAiAction` and `SetKoudou`; this change
introduces no new diagnostic.

## nbSetActionCommand (newbattle/nbMakePacket, 0x100)

38 of 62 words match, two words short of retail's 256.

Every instruction is retail's except one. Retail keeps `command * 784 + 0x10`
as a single index and adds it to two symbol bases, `datModelParamTable` and
`datModelParamTable + 8`, so the pair of `f32` reads are both at offset 0 of a
computed address. ee-gcc reassociates the same expression the other way,
folding the `+0x10` into the symbol (`datModelParamTable + 0x10`) and dropping
retail's `addiu $2, $2, 0x10`. That one missing instruction also removes
retail's loop-alignment `nop`, and every word from the first guard branch on is
the same instruction with its offset shifted by two.

The rest is confirmed: the 10-iteration `stat &= ~0x800` sweep over
`form[4..13]`, the `data->unk_84` and `nbEncountFormTable[encno].unk_22` guards
in that order with the table byte loaded before both, and the second loop
written as `for (i = 4; i < cnt + 4; i++)`, which is what gives retail's hoisted
`slt $2, $6, $2` guard against `cnt + 4` and the count-down trip register.

Ruled out for the fold: an `ofs = command * 784 + 0x10` local, which stops
ee-gcc hoisting the addresses out of the loop at all (45 words differ);
`&datModelParamTable[...]` array syntax; `(u32)` and `(s32)` casts on the
symbol; writing the second read as `+ 8 + command * 784 + 0x10`; the constant
before the product; and the word-indexed forms
`((f32 *)datModelParamTable)[command * 196 + 4]` and
`(command * 196 + 4) * 4`. All seven give the identical 24-word diff, so the
fold is not reachable from the address expression.

## nbPickAiAction (newbattle/nbMakePacket, 0x118)

68 of 70 words match, at the exact 280-byte size.

Both differing words are `daddu rD, $0, $0`: retail zeroes the loop index
first and puts `acc = 0` in the `blez $16` zero-trip guard's delay slot, and
ee-gcc zeroes `acc` first and puts the index in that slot. The instruction
sequence is otherwise retail's.

Two things were pinned getting here. The weighted pick has to test before it
accumulates, `if (r < acc + row[list[i]].ritu) { return list[i]; }` then
`acc += row[list[i]].ritu;`: ee-gcc then CSEs the sum into a temp and leaves
retail's `addu $3, $6, $3` plus `daddu $6, $3, $0` copy, where
`acc += ...; if (r < acc)` adds in place and is 23 words out. And the row base
is `D_00398A08 + work->id * 164 + tbl * 40` over 8-byte entries, so the
`datDevilAITbl_t` added for it is `{ u16 ritu; u16 skill; u32 target; }`, the
same record the HD sources carry.

The shared `datDevilAIData_t` now describes the complete 164-byte row:
a script ID and AI level, three twelve-byte condition records, three forty-byte
action tables, and the death-script/event pair. `D_00398A08` is the interior
action-table base at row +0x28, not a separate array of four action tables.
The earlier thirteen-condition overlay is removed. These type corrections
preserve the existing matched owner functions; they do not resolve this
function's two-zero scheduling residual.

Ruled out for the two zeros: `i = 0;` before the `dds3GetRandIntA` call, after
it, and in a `for (i = 0, acc = 0; ...)` or `for (acc = 0, i = 0; ...)` init;
a `while` with the increment written out; and an explicit `s32 *` cursor over
`list` in place of the subscript, which costs two more words.

A qualified compiler-dump pass localizes the remaining pair. Plain compilation
and `-da -fsched-verbose=5` produce identical allocated sections, non-file
symbols, ordered relocations and normalized assembly (4 sections, 300 symbols,
144 relocations). In `combine`, the original index-zero assignment is deleted;
the loop-entry comparison becomes a replacement index-zero assignment as its
test folds to `count <= 0`. The accumulator-zero assignment survives. Thus
source initialization order alone does not determine the surviving pair's
order. `sched2` selects accumulator zero, random-result copy, index zero, then
the branch; delay-slot reorganization puts index zero in the `blez` slot.
These are observed compiler stages, not a runtime trace of the scheduler's
comparator tie-break or a claim about original source spelling.

A separate ordinary second-loop index changes the frontier to 66/70 words at
280 bytes: the zero sequence is correct, but four loop-body words swap the
accumulator/index register roles. Binding that initialized, genuinely consumed
index to native `$7`, with a read-only operand constraint alongside the random
result, emits 284 bytes and matches 44/70 native-prefix words. Removing the
read-only operand while retaining the binding also emits 284 bytes and matches
44/70; neither closes the loop-entry/control-flow residual. The index lifetime
crosses no call. All 47 matched peers remain exact in these checks.

The production draft remains the 68/70 ordinary form. The scoped-index and
constraint controls are not retained, and no exact matching credit is added.
Further work needs a different evidence-backed premise rather than more
initialization-order or register permutations.
