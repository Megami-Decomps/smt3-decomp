# Matching levers

What to change in the source when a function is close but not exact. Every
item here came from a function that matched afterwards, or from one that was
reverted after the lever was ruled out. [Contributing](CONTRIBUTING.md) covers
the workflow; this is the list to work through before reaching for a
permuter.

Read the retail assembly first and ask which of these it is. Guessing at
source shapes at random costs more than the reading does.

Three references in [dds-decomp](https://github.com/Megami-Decomps/dds-decomp)
go further than this page, since it is the same engine and compiler:
`docs/idioms.md` is 14,000 lines of source shapes confirmed to produce
specific ee-gcc 2.96 output, `docs/compiler-decision-atlas.md` locates which
pass is responsible for a mismatch, and `docs/matching-investigation.md` is
the method for finding the missing source fact. Check each claim here, since
it was measured on DDS and not on this ELF.

## Control flow

| Retail shows | Change |
|---|---|
| the opposite arm falls through (`bnez` where you emit `beqz`) | invert the `if` condition |
| one exit, default set before the branch | replace an early `return 0` with `s32 ret = f(); if (...) ret = g(); return ret;` |
| `jal` and a full epilogue | assign the call's result to a variable instead of `return f();`, which ee-gcc 2.96 sibcalls |
| `sltiu i, N` counting up | make the loop index `u32`, or subscript with it, instead of post-incrementing a pointer. A `for` whose index is unused in the body becomes a downcounter |
| `blez n` then a `bnez` downcounter | a `for` to a variable bound whose index the body does not use. With a constant bound the same shape counts `n-1` down to 0 and tests `bgez` instead |
| no zero-trip guard at all, and an up-counter kept as `addiu i, i, 1` plus `slt` against the bound | a `do`/`while`, not a `for`. The guard is only half of it: given a `for` whose index the body does not use, gcc also flips the counter to `addiu i, i, -1`, so the `blez` and the counter direction go together. Both `dds3Particle_SparkBuffInit` and `dds3Particle_SparkOffsetBuffInit` turned on this one change, 164 differing words down to 48 |
| `slt` then `sltiu` | write `if (a < b) return 0; return 1;` rather than `return a >= b` |
| a `break` where the hit arm of a search loop ends the function | write `return` instead. `evtPicturePriAdd` searches eight slots for a free one, copies the row in and stops: with `break` it is 36 of 48 words out and with `return` it is exact, because the `break` adds a `b` out of the loop and loses the `bnel` that carries the index increment |
| `sltu rd, $0, x` then a chain of `ori` plus `movn` into `$2` | a bit-flag accumulator written as parallel `if` statements with the **first** one spelled `stat = 1`, not as an initialiser. `s32 stat = 0; if (a) stat = 1; if (b) stat |= 2; if (c) stat |= 4;` is exact, where `s32 stat = a != 0;` puts the `ori` result in `$2` and emits `movz` against retail's `movn`, 11 of 15 words in `evtPictureStat`. The ternary spelling, the bare conditions, `u32`, and the declaration split from the assignment all give the `movz` form |
| explicit branches where you emit `andi` or `sltiu` | gcc folds `(x & 1) ? 1 : 0` and `f() == 0`. Usually unreachable from C |
| the same field stores duplicated per `switch` case | write the full store list in each case and let gcc merge the tail. Hoisting the values into locals does not match |
| every arm reaching one shared epilogue at the end | use an `else if` chain. An early `return` per arm gives each arm its own epilogue copy |
| a jump table | write a `switch`. The jtbl entry order gives case to label, and the label order inside the body gives the source order of the cases, which is often not ascending |
| two or three equality tests on one value, bodies placed after the chain | a `switch`, not an `if`/`else if` chain, which inverts the first branch |
| separate `beqz` and `slti` | separate `if` statements. `x != 0 && x < C` is folded into one range test, an `addiu -1` and an `sltiu` |
| two `andi`s and two branches on one field | nested `if`s. `(x & 1) && (x & 2)` is folded to `(x & 3) == 3` |
| a three-case `switch` testing something other than the middle case first | an explicit `goto` dispatch. gcc 2.96's `balance_case_nodes` splits exactly three nodes at the middle one, so a `switch` over 1, 2 and 3 always tests 2 first |
| `j callee` with the full epilogue in front | make the call the last unconditional statement: invert a trailing `if` into an early `return` so the call is not inside it |
| `jal` and a full epilogue where the call is the last statement | a dead store after it, `cursor = NULL;`. It reads oddly but is an idiom this tree already carries: `mymdlColorSet` and its neighbour in `code_0029B780` end the same way, from separate earlier work |
| the body of a later case falling through after the compare chain | keep the dispatch ascending but place the bodies in source order. The case written first is the fallthrough, so cases written 3, 2, default gives a `beq 2`, `bne 3` chain |
| separate `slti`s rather than a subtract and one unsigned compare | a `switch` with several `case` labels, not an `&&`/`||` range chain, which folds |
| three `beql`/`beqz` annul bits and a `%hi` materialised in the wrong delay slots, on a function whose paths all `return 1` or `return 0` | a `goto` to one trailing `return 0` placed after the last `return 1`. `RetryMapLoad` went from 6 of 53 words to exact on that alone: the separate `$2 = 0` block is what the delay-slot filler then annuls into each test, and it also moved the duplicated half of the `lui`/`addiu %lo` pair from the `addiu` to the `lui`. This is the row above read on a function with two exit constants rather than one |
| two arms whose index and bit expressions differ only by a constant offset, each recomputing one of the pair while the other is hoisted above the branch | give the pair named locals and assign both in each arm: `idx = (id - 0x100) >> 3; bit = (id - 0x100) & 7;`. gcc then hoists one computation of each pair into the entry block and leaves the other in the arm, which is retail's four-instruction split. `fldGetTakaraOpen` went from 24 of 24 words to exact; the same expressions written inline at the subscript hoist all four and run one instruction long |
| two explicit branches selecting 0 and 1 from a call's result, where you emit one `sltiu` | a one-case `switch` over the call, `switch (f()) { case 0: return 1; }`, or a `goto` to a trailing `return 0;`. Both reach it; every `if` spelling of `f() != 0`, an `s8`/`s16`/`u8` local for the result and an explicit `else` all fold to `sltiu $2, $2, 1`. `cmpItemIsStockFull` went from 7 of 14 words to exact. The same trick does not work on a *mask* test: see `cmpItemHasFlag` in the drafts |
| a clamp whose bound is both a compare operand and the stored value, compared with `sltu` against a register | a named local for the bound. As a literal the compare becomes an `sltiu` immediate against `bound + 1` and the stored constant is materialised separately. `rstUpdateEvoBgFade` needed `u32 max = 0x32;` for the second of its two tests, with the first left as the literal |
| one guard whose failure block sits at the end of the function | an `if`/`else`, not an early `return` inside the guard. The early return places the failure block first and reaches it with the inverted branch; the `else` form puts it last, which is what retail's `bne` to the tail does. `rstUpdateEvoBgFade` turned on that plus the bound local |
| explicit branches where two early `return 0`s in the same block fold to `slt` plus `sltiu` | a `goto` to one trailing `return 0;`, which stops the if-conversion. `nbSkillIsInRange`'s two `switch` arms each lose a branch as `if (skill < 0x24) { return 0; } return 1;` and come out exact as `goto zero;` |
| a `switch` arm's guard failure branching to the same block as the `default`, and a value you expected in `$2` or `$3` sitting one register higher | drop the last arm's own `return 0` and let the arm fall into the `default:` label, writing its tests as `if (...) { callee(x); break; }`. The separate `return 0` makes its zero conflict with the switch value: `nbCameraBoss_Cut1_Run` moved `skill` from `$4` to `$3` and went 59 to 33 differing words on that alone, and `nbCameraBoss_Cut2_Run` 89 to 92 of a longer body. A nested `switch` over the same values frees the register too but runs three words over |
| `jal callee` then a `b` to one shared epilogue, with nothing writing `$2` in the arm | let the arms only call and the function fall off the end of the `switch`, so the callee's value is what reaches `$2`. `return callee(x);` sibcalls every arm and a `ret` variable spends `$3` and a `daddu` per arm. The returns that do exist (`return 0`, `return 1`) still give the epilogue its `use` of `$2`. `nbCameraBoss_Cut1_Init` and the four `Cut*` dispatchers in `newbattle/nbCameraBoss` are all this shape |
| a guard whose failure returns a constant, where the constant lands in a second register and the branch is `bne` rather than `bnel` | write the guard positively, `if (skill == n) { callee(x); break; } return 0;`, not inverted with an early `return 0`. The inverted form keeps the compare constant and the return constant live together; `nbCameraBoss_Cut1_Init` went from 2 differing words to exact on that one arm |
| a `switch` with fewer than ten `case` nodes still reaching a jump table | add the missing low `case` label with an empty body. `case_values_threshold` is not the whole story: `nbMaeCheckDone`'s six-entry table needs an explicit empty `case 0:` alongside 1 to 5, and without it ee-gcc builds a five-node compare tree instead |
| each arm of an `if`/`else` recomputing the same shift or scale that your C shares at the merge, and the taken-equal arm reusing the compare's constant as its return value | write the tail out in each arm with its own `return`, so `if (f() == 1) { store; return 1; }` lets gcc reuse the `1` already in `$2` and cross-jump only the store's tail. `NegoDebug` went from 21 of 46 words to exact on that plus a `< 0` arm written as `if (x < 0) { body; return 1; } return 0;` |
| a run of sibling `if`s after one guard, where your C emits the guard as an early `return` | nest every later `if` inside the guard. `SyuGo2`'s three frame tests under one `if (e->frame >= 0xB)` swapped `e` and the loop counter back into retail's `$16` and `$17`; declaration order, the loop form and an inverted last test all leave the pair swapped |
| a call's pointer argument computed before an inner call whose result is the next argument | give it its own local: `form = &e->data->form[list[i] + 4];` ahead of `f(0, 0, g(...), 2, form)`. Written inline at the call gcc orders the inner call first and spends one saved register fewer. `PartsEffect2` turned on that alone |
| the loop body once, with the exit test at the bottom and the first load in the preheader | a `do`/`while`, not a `for`. A `for` whose body carries a `break` gets the whole body peeled ahead of the test, which is 14 words in `SetAction_NEGO` |
| a packet-list walk whose backedge is `bnez next` with `daddu p, next` in the delay slot | invert the per-node test and repeat the advance: `next = p->next; if (p->f != x) { p = next; continue; } g(p); p = next;`. The positive form `if (p->f == x) { g(p); } p = next;` tests `p` after the copy instead and runs two words long. `nbDelAllUniqueIDKoukaPacket`, `nbDelAllKoukaPacketType` and `nbDelUnEventKoukaPacket` all turned on it, and in `nbDelUnEventKoukaPacket` the skip arm needs `p = next; continue;` inside the `switch` as well, or the `jal`'s delay slot swallows the advance |
| two `switch` arms reaching one shared `jal` with the argument loaded in each | a `goto` from one arm into the other arm's `if` body, label and all. Four spellings that look equivalent (`break`, an early `return` per arm, an `if`/`else` chain, a result local plus one trailing call) all leave the tail duplicated, four words long. `RunSummonKouka` turned on `goto add;` jumping into the other arm's guarded block |
| two `jal`s that converge on one epilogue where every spelling of the chain sibcalls both with `j` | a `goto` per arm to one trailing empty label: `if (!(t < 7)) { if (t < 9) { goto seta; } return; } first(); goto done; seta: second(); done: ;`. `RunUnitColKouka` sat at 10 of 57 words with `if`/`else if`, early returns, an `s32` return type, and a dead store to each of six locals; the two `goto`s took it to exact. Note the inner test has to be the positive `t < 9` with the `goto` inside it, since `if (t >= 9) return; goto seta;` emits the branch-likely the other way round |
| a loop arm that deletes the current node, reached by a long forward branch from the middle of the body | write it last in the loop body behind a forward `goto`: `if (p->framelength < 0) { goto del; } ... continue; del: f(p); p = next;`. Inline (`if (<0) { f(p); p = next; continue; }`) it costs two extra branches, and nesting the rest inside `if (framelength >= 0)` rearranges both jump tables. `nbKoukaProcess1`, 304 words, closed on that one move |
| a read-modify-write of a field whose value the C keeps live from the test above it | write the RMW through the outer pointer (`p->form->anime &= ~0x80;`) while a local holds the same pointer for the calls. With the local on both sides gcc keeps one load in a register across the guarded block; the outer spelling restores retail's second load and its branch-likely delay-slot copy. `RunDamageMotionKouka` went from 94 of 204 differing words to 16 on that one expression |
| one guarded call compiled as `jal` plus a full epilogue, with no `switch` in sight | a dead store to a local holding the tested value: `s32 type = p->type; if (type == 0x23) { f(...); type = 0; }`. The `SetNextSeq` row below is the same trick inside a `switch` arm; it works on a plain `if` too. `RunHelpKouka`, `RunHitStopKouka`, `RunSkillColorKouka`, `RunBossEffectKouka`, `RunFadeKouka`, `RunHitEffectKouka` and `RunMotionKouka` all needed exactly this, and it also fixes the `bne`/`bnel` on the guard in the same move |
| a counter loop with one exit, the count returned through `daddu $2, $5` | `cnt = 0;` before a `while`, not `if (p == NULL) return 0;` ahead of a `do`/`while`. The early return materialises a second zero and runs two words long. `nbGetUniqueIdKoukaPacketCnt` and `nbCountKoukaByType` both turned on it |
| three `slti`s on one value where your C folds them to an `addiu`/`sltiu` range test | nest them, `if (x >= A) { if (x < B) { if (x >= C) { ... } } }`, with a `goto` into the innermost body for the singleton case. Written as a run of separate `if`s each ending in `continue` they fold anyway, which is the opposite of what the "separate `if` statements" row above predicts: what keeps them apart is the nesting, not the statement split. `nbCheckSePacket` went from 10 of 48 differing words to exact, and the nesting also fixed which parameter got `$16` |
| an extra range test (`slti`, `bnez` to the default) in front of a two-case compare chain | an extra `case` with an empty body below the lowest real case value. `nbStageProcess1`'s `switch` over 2 and 3 needed a `case 1: break;` to make gcc emit the bound test; an `s32` or `u32` local for the index does not reach it |
| `bgez` on a sign test where you emit `bgezl` with the epilogue's `ld` duplicated into its delay slot | spell the test as the mask, `if ((stat & 0x80000000) != 0)`, not `if (stat < 0)`. Both are one instruction, but the relational form lets ee-gcc fill the delay slot from the branch target and pay for it with a second epilogue copy. `CheckSenseiOrBackAttack` went from 8 differing words to exact |
| separate `beq`s on contiguous values inside an `||` chain, where a `switch` would test the middle value first | distribute the shared antecedent instead of nesting the disjunction: `(a == K && f == 8) || (a == K && f == 9) || (a == K && f == 0xA)` keeps three plain `beq`s where `a == K && (f == 8 || f == 9 || f == 0xA)` folds the inner chain to `addiu -8`, `sltiu 3`. gcc still tests the outer value once. `fldOtInit` and `ProcAmCheck` both turned on this |
| a long flat chain of early `return 0`s where the exits reach a shared block that sets `$2` itself, and the address temps use `$2` and `$3` where you emit `$3` and `$4` | convert only the *last* few early returns to `goto ret0` with one trailing `ret0: return 0;`, and sweep how many. All of them at once is worse: `ProcCampCheck` wants the last two of eight and `ProcAmCheck` the last nine of twelve, and either one converted wholesale loses more than it gains. What the conversion stops is gcc keeping one hoisted zero live in `$2` across the rest of the chain |
| a `switch` whose arms each need their own temporaries | declare them in the arm's own block. At function scope the arms share one pair of allocnos spanning the whole switch; one function went from 45 of 370 words to exact on that alone |
| an in-place base adjustment folded into the first use | write it as its own statement, `x = (s32)(...); x -= 2;` rather than `x = (s32)(...) - 2;`, or the unadjusted value stays live and cc1 folds the offset into the first use |
| a register copy of a field you already loaded, feeding one of two roles | write the field expression again at the second use site rather than passing the local. cse turns the second load into a copy, and the two pseudos then take the two hard registers the uses prefer. `effPCPDANTE_Sprite_Draw` went from 10 of 60 words to exact on `uv->uw` written at the call where a local was already holding it |
| explicit branches selecting one of two or three constants, the value left in a saved register across calls | a `switch` over the tested values, not an `if`/`else if` chain, which ee-gcc if-converts into a `movn` and drops the saved register. `Flow6_14` and `Flow11_9` in `nbMakePacket` each turned on that alone |
| `jal` and a fallthrough for one `switch` arm while the other arms sibcall with `j` | a dead store after the call in that arm only, `seq = 0;` on the switch index. Giving the callee an `s32` return type with no `return` does not reach it, since the other arms sibcall the same way. `SetNextSeq` went from 27 of 30 differing words to exact |
| a `switch` tree split one node later than your case set produces | an extra `case` with an empty body, above every other case value, so the node collapses into the default and only the node count changes. gcc's `balance_case_nodes` splits at the `(n + ranges + 1) / 2`th node, so six singleton cases split at the third and seven at the fourth. `Flow1_9`'s six status bits needed a seventh case above `0x200` to make the tree start at `8` rather than `4` |
| two early `return <same constant>`s | write them as separate `if`s, which gets the returns cross-jumped and the constant hoisted into `$2` above the first branch. `&&`, `||` and a `goto` all cost four bytes and a register copy |
| a clamp that leaves the bound in the value's own register | assign the bound rather than returning it: `if (2.0f < s) { s = 2.0f; } else if (s < 0.8f) { s = 0.8f; } return s;`. `return 2.0f;` puts the constant straight in `$f0`, costing four bytes and rotating every fp register |
| a `switch` whose arms share one exit | set a result local before the dispatch, `break` in each arm, and one trailing `return`. One function went from 21 of 54 words and 12 bytes long to exact |
| `c.lt.s a, b` then `bc1t` on a float test you wrote as `a >= b` | negate the `<` instead: `if (!(a < b))`. ee-gcc turns `a >= b` into `c.le.s b, a` plus `bc1f`, which is the same test in the other encoding and two words off |
| an `ldl`/`ldr` and `sdl`/`sdr` run | one struct assignment of a 4-aligned type, the trailing word included. Splitting that word off as its own field is what breaks it; `effPCPCrackCreate`'s 0x8C copy is why it carries an 8-byte-alignment runtime dispatch |
| `sw $0` to a float field, not clustered with the integer zero stores | write it `0.0f` into an `f32` field. A float-typed zero store sits in the float store chain; one function went from 15 differing words to 2 on that alone |
| every compared field loaded before the first branch | read them all into locals at the top. One function went from 65 of 69 differing words to 42 |
| an `lq` and `sq` pair for a struct copy | `SDF_VU0_COPYVECTOR(dst, src)`. `QWORD_ALIGNED` is empty in this build, so a plain `sdfVector4_t` assignment gives `ldl`/`ldr`/`sdl`/`sdr`, and a `u128` assignment gives `sq` with a displacement where retail biases the base and uses `0(reg)`. Took one function from 300 bytes to 220 |
| two `slti`s and a `bltz`, the last test if-converted to an `sltiu` | a `switch` whose cases are one singleton and one small range, `{0}` and `{1,2}` |
| `bnel` against a materialised `-1` in a table scan | `for (i = n; i != -1; i--)`, not `i >= 0`, which gives `bgez` instead |
| which of two cases gcc tests first | make the switch index a `u32` local; `s32` adds an `slti` and reshapes the tree over the same case set |
| `beql` on two successive early exits, each with the same `addiu $2, 1` in its delay slot | fuse the two tests into one `||`. As separate `if`s gcc puts the constant in the first delay slot unconditionally and the second exit then reuses it, which costs the branch-likely |
| one `jal` at the end reached from four or five blocks, each setting only the argument | an early `return` per block with its own call. gcc cross-jumps the identical tails into the one call; a `ret` variable instead spends a saved register and misses by the whole body |
| a store in a `jal`'s delay slot | write the store before the call. After the call gcc cannot move it up and leaves a `nop`, which is one word and one branch displacement off |
| one shared tail after a `switch`, with the call's argument loaded in each predecessor | write the tail out in full in every arm and let gcc cross-jump. A shared block placed after the `switch` pulls the argument load inside it and puts the remaining arm on the wrong side of it |
| the same call duplicated in two arms, each with its own `return` | the opposite of the row above: an early `return` per arm lets gcc cross-jump the two calls into one, so write an `if`/`else` with a single trailing `return`. `CheckAction_AISCR` has two `jal SetNextActionSeq` in retail and one with a `return 1` in each arm |
| a `beq` on one value, then an `slti` that selects a constant, then a second `beq` | a `switch` over three values, not an `&&` chain of inequalities. gcc splits the three case nodes at the middle one, then folds the two leaves into one compare against a constant chosen by the `slti`. The chain emits three plain `beq`s instead |
| a flat `beq` chain over four or more contiguous `case` values | make the switch index a `u32` local. With `s32` ee-gcc splits the balanced tree with an `sltiu` range test before the lowest compare; unsigned makes the left subtree bounded below, so `index < 1` folds to the `beqz` retail emits |
| a call's constant arguments set up above the `if` that guards the call | invert the test into an early `return`, so the call is no longer inside a nested block |
| more instructions than the C emits, with a 0 or 1 materialised per exit | nest the body inside the guard and give the function one trailing `return`, instead of a chain of early returns. A trailing `if (n <= 0) return 0; return 1;` folds to `slt $0, $2`, and separate `return` constants get cross-jumped into one epilogue; the nested form keeps both. Two functions in `code_001FAE80` went from 0 of 17 and 1 of 17 words to exact on that change alone |
| a branch straight to the arm you expected to fall through | write that arm last. gcc jumps forward to the block written after the test and falls through to the one written before it, so `if (c) return X; return Y;` and `if (!c) return Y; return X;` differ in which of X and Y the branch reaches |
| `bnel` on the second of two zero tests, with the first store of the taken block duplicated into the annulled delay slot | invert the pair: `if (a == 0 && b == 0) { flag = 0; } else { store a; store b; flag = 1; }`. The `||` spelling, `if (a != 0 || b != 0) { ... } else { flag = 0; }`, cross-jumps the two true paths into one block and comes out two words short with plain `beq`. `BlurFrameFunc` in `event/evtPolygonMovie` went from 9 of 44 words to exact on that inversion alone, and the store order inside the arm has no effect either way |
| one call block reached from five different condition chains, each predecessor only setting up the arguments | write the call **once** after the chain and reach it with `goto` labels: every guard becomes `if (...) { goto next; }` and `next:` sits just above the loop's trailing statement. Writing the call out in each arm and trusting cross-jumping does not merge them: `FrameRun` emitted five copies of the `nowcamera[kind]` load and was 97 of 132 words out, and the single `goto` funnel took it to 4 words. The last four needed one arm's successful test spelled as its own `goto run;` with a `goto next;` after it, since `if (!(now < bound)) { goto next; }` branches to the call and falls through to the skip, which is the mirror of what retail does |
| a guard whose zero default is materialised in the *entry* block, above an unrelated earlier test | initialise the local at its declaration, `char *target = NULL;`, and write the guard as a bare `if (index >= 0) { target = ...; }` with no `else`. The `if (index < 0) { target = NULL; } else { target = ...; }` form emits `bgez` where retail has `bltz` and keeps the zero inside the arm. `BEDFrameFunc` turned on that alone |
| a byte loaded into `$3` and tested with a plain `beq`, where the return value is already hoisted into `$2` | an early `return` before the call: `if (x == 0) { return i; } f(); return i;`. The trailing `if (x != 0) { f(); } return i;` form coalesces the return value into the tested byte's register and emits `beql` on `$2`. `AddEffectTableMG12` sat at 2 of 85 words on that one statement |
| a value whose register retail gives to a coordinate while a flag parameter sits one register higher | give the second, unrelated role its own local instead of reusing the first one. `effDebugDrawRectLine` updates `left`/`top` in place and then scales them into the second corner, which is the natural reading of retail's `$17`/`$18`; written that way the flag takes `$17` and `left`/`top` shift up, 27 of 85 words out. Separate `x1`/`y1` locals shorten `left`/`top`'s live ranges enough to rotate the allocation back and took it to 13 |
| a sibling loop after an inner loop, whose downcounter sits in the *inner* loop's index register | reuse the inner loop's index variable as the second loop's counter. In `func_001AA5A0` retail's second loop counts down in `$7`, the first loop's `k`; a fresh local and the middle loop's `j` both give the middle loop's doubled induction variable instead, and `j` alone costs nine more words. That plus the store order took the function from 12 differing words to exact |
| a call's argument setup duplicated in both arms of a loop guard, where your C computes it once above the branch | drop the explicit `if (count > 0)` and let the `for` emit its own guard. With the `if` written out gcc hoists the shared address and the call's pointer argument above the branch and runs one word long; without it the preheader and the skip arm each get their own copy, which is what retail has. `dds3MultiBillboardPktLink` went from 84 of 88 words differing to exact on dropping that one `if`, and six loop-body forms left it unchanged |
| three constants selected into one struct field, with a single store the arms branch to | assign the field itself in all three arms and let gcc cross-jump the stores into one. A local for the value is if-converted into a `movz` pair whichever way the chain is written, `goto`s included, and a `switch` over the masked value gets the size right but reshapes the compare chain. `dds3Billboard_P2ADraw` went from 23 of 174 words to 1 on the three direct stores |
| one shared exit with the return constant set in each predecessor's delay slot, and one arm falling into it | a `goto` to a single trailing `return 0`. Nesting the bodies inside their guards, an `&&` chain and an `else if` chain all reach within two words but keep one exit too few; the explicit label is what lets gcc put a `$2 = 0` in a delay slot on one path and fall into another on the next. `nbCheckModelPackLoaded` went 13 of 52 differing to exact |
| the `default` arm's body as the fall-through after the compare chain, with two `case` bodies behind forward branches | write `default:` **first** in the source, before the `case` labels. This is the row above read on a default arm: gcc lays the arms out in source order, so `default`, `case 1`, `case 2` is what puts the default body where retail has it and lets it share the tail with the last case. `ResetWindowPara` was 31 of 49 words out with `default` written last and exact with it written first, over all six arm orders |
| a `switch` arm that only advances a counter and tests it, where your C reads the field twice | `p->field++; if (p->field >= bound)`. An `s16` local for the incremented value is the obvious shape and is four words out: it sign-extends in place and takes the bound's register. `CalcMessage` turned on that alone |
| one shared `jal` reached by a `b` from one arm and a fall-through from the other, where the argument comes from two different producers | write the whole call out in each arm and let gcc cross-jump it. A local for the produced value (`name = f(); ... g(x, y, name)`) is 11 words out on the argument-setup order alone; `itfMesMngRequestMessage` went to exact with the two `MessageRequestEx` calls written in full inside the `if` and the `else` |
| the **same** tail block appearing once per branch of an `if`/`else`, byte-identical apart from the branch displacements | two changes that do nothing on their own and everything together. Cross-jumping runs in `jump2`, after register allocation, so two identical tails merge unless a register differs. First, read the dispatched field directly in both the `switch` head and the following `== N` test (`switch (control->animeMode)` and `if (control->animeMode == 6)`), not through a local: gcse then hoists one load into the branch block and leaves a `daddu rd, rs, $0` copy in one arm, so the two tails test different registers. A local for the value, two locals, a local copied from a local, function-scope locals and six declaration orders all coalesce to one register and change nothing. Second, repeat the call that *follows* the `if`/`else` inside each arm instead of writing it once after it, which is what stops gcc merging the arms from the end inward. `dds3ObjectBaseModelLinkOt` has three such arms; it sat at 130 of 230 differing words, reached 127 on the direct field reads alone, and went to 20 of 242 when the trailing `sdfModelEvalTm` call moved into all three |
| a `switch` `default` arm whose counter bump shares the block the other arms branch to, where your C gives the default its own copy | write the default's test with the opposite polarity, `if (c >= 0xB) { call(); } else { c++; }`. The five other arms of `Eff_ModelTrackPolyDraw` cross-jump their `c++` into one block with the natural `if (c < 0xB)` and only the default, written last, keeps a copy. 18 of 174 words out, exact once the default alone is inverted |
| two `sltiu`s against different immediates converging on one branch, where your C emits a `movz` | put the ternary on the comparison, not on the bound: `if (cond ? (c < 0x11) : (c < 0xB))`. Selecting the bound first, as a ternary or as an `if`/`else` that assigns a `limit` local, if-converts to `movz`. `Eff_ModelTrackPolyDraw`'s `case 0x12` went from 29 of 174 words to 18 on that one expression |
| a dead store of an intermediate into the array slot your C keeps in a temp | update the slot in place, `runStart[0] -= 2; ... runStart[0] += data->capacity;`, and order the statements offset, negate, add capacity, count, zero. `effSwordTrackPolygonDraw` needs that, while its sibling `dds3HermitianTrackPolyDraw` in the same unit matches with a `wrappedStartOffset` temp, so both shapes are live and the extra store in the retail text is what tells them apart |
| 26 arms, each ending in a `b` to one shared exit, with no code after the chain | a plain `if`/`else if` chain. The `goto` funnel and the duplicated-tail rows above apply only where the arms *share* a tail; here each arm carries its own whole loop and there is nothing for `jump2` to cross-jump, so the chain matched with no `goto` at all. `dds3ObjectBaseMakeUnit`'s 26 model probes are this shape |
| a `for` whose bound is loaded once into a saved register, where your C runs the whole pointer chain again each iteration | read the bound into a local first, `count = obj->inner->nodeList->count;`. Written as the loop condition the two loads and the `lh` repeat in the backedge block, cost five words, and also take the argument register the call in the body wants. `dds3ObjectBaseMakeUnit` went from 94 of 1406 words to 9 on that one local |
| a loop whose body is guarded by `bnel i, $0` and whose call arguments are all `$0` | `if (i == 0) { ... }` inside the `for`. The index is then known zero in the body, so an `i` passed as an argument materialises as `$0` rather than a copy |
| a two-sided clamp whose high arm has its own `jr $31` and a `mov.s` from the bound's register | `if (hi < x) { x = hi; } else if (x < lo) { x = lo; } return x;`. `return 2.0f;` in the high arm puts the constant straight in `$f0` and rotates every float temp, 20 of 34 words in `evtCalcEffBedScale`; an early `return x;` after the assignment does the same, and two sibling `if`s instead of the `else if` are 8 words out |

A flag mask that steps by a constant shift per iteration is a source variable,
not an expression. Retail's `sll $18, $18, 2` in the loop-closing delay slot
comes from `for (i = 0, mask = 4; i < 3; i++, mask <<= 2)`; writing
`4 << (i * 2)` keeps `i * 2` as the induction variable and emits `sllv` from a
constant held in a register, since a variable shift of a constant is not an
induction variable cc1 can strength-reduce. Where the same loop also needs the
next bit, write it as `mask << 1` at each use: retail recomputes that shift
once per use, and a named local for it computes it once.

A run of equality tests on one field whose arms are branch *targets*, with the
tests falling through to each other and the arms laid out after them in case
order, is a `switch`, not an `if`/`else if` chain. The chain puts its first arm
on the fall-through path, which is two words apart from retail and spoils the
annulled delay slot that carries the arm's first store.
`sdfMcFuncGetInfo1`'s three-way on `cb->type` needed the `switch`.

ee-gcc moves a `while`'s exit tests to the bottom of the loop and enters it
with a `b`, so a loop whose test retail keeps at the top with an unconditional
back-edge is an explicit label plus `goto`. `while (1)` or `for (;;)` with
`if (...) break;` spell the same thing and get moved the same way. The
`goto` costs the loop-alignment `nop` ee-gcc emits only for a loop the front
end marked, so expect the body to sit one word early.

## Addressing and types

- A struct pointer local is what keeps two index scalings apart. For
  `base[port].arr[slot]`, writing it out folds both scalings into one
  `(port * N + slot) * 4`, and a row pointer (`u32 *f = base[port].arr;`)
  materialises the whole address into a register and stores at offset 0.
  `sdfMcControl_t *cb = &base[port];` then `cb->arr[slot]` gives retail's
  separate `sll` per index, their sum, and the base folded into the
  reference's `%lo`. All three `sdfMcCb` slot-flag accessors turned on it.
- A `u8` field compared against a small constant folds to `sltu`, since cc1
  knows the promoted value is non-negative. Copying the field into an `s32`
  local first and testing that gives retail's `slti`, and the local is reused
  for the `+ 1` store as retail does.
- A clear loop written up-counting, whose index the body uses only as a
  subscript, is what gcc reverses and strength-reduces into a decrementing
  address plus a separate `bgez` downcounter.
  `for (i = 0; i < 0x40; i++) tab[i] = ' ';` is exact in
  `dds3PrintProcessTree`; writing the same loop down-counting defeats the
  reduction and leaves an `addu` of base and index in the body.
- The motion-speed setter is inlined in `newbattle/nbKoukaProcess`: retail writes
  `form->hunit->owner->first->speed` with no `jal`, which pins `mdlObj_t + 0x1C`
  as `first` and `sdfMotionCtrlr_t + 0x20` as `speed`. Hoist both hops into
  locals when a `switch` uses them in more than one arm: `RunMotionStopKouka`
  needs `evtUnit_t *hunit` *and* `mdlObj_t *obj` assigned before the dispatch,
  since retail loads `hunit->owner` in the dispatch's delay slot.
- Three identical call sites through **one** function-scope pointer local give
  that local nine refs and one pseudo, so the index temp feeding it takes `$2`
  and the loaded function pointer `$3`. Declaring the pointer per site, in a
  braced block, gives each site its own pseudo that ties the index into `$4` and
  matches retail's `lw $4, 0x2C($22)` / `sll` / `addu` / `lw $2, 0x10($4)`.
  `effSwordTrackDraw` has three `ot = &sdfGrOtTable[work->otIndex];
  ot->regFunc(ot, list);` sites and sat at 15 of 176 words with the shared local;
  the per-site declaration took it to exact. Six spellings of the address
  (`sdfGrOtTable + i`, a `u8 *` cast, a table pointer local, the two-step `+=`,
  the inline `sdfGrOtTable[i].regFunc(&sdfGrOtTable[i], list)`, a cast on
  `regFunc`) all leave the shared local at 15.
- In a comparison between an `f32` field and an integer cast to `f32`, gcc
  expands the cast first whatever the operand order, because it is the more
  complex operand. With a `u32` the cast is a branch (`bltz`, then the
  `srl`/`andi`/`or`/`add.s` pair), and the field load lands inside both arms of
  it. Reading the field into an `f32` local on its own statement first puts the
  load above the branch, where retail has it, and leaves the cast in `$f0` with
  the field in `$f1`. `effSwordTrackDraw` went from 162 of 175 words to 15 on
  `frame = work->params.object->first->frame;` ahead of the `if`.
- Externs of 8 bytes or less are addressed through `$gp`. Where retail uses
  `lui`/`%lo`, declare the symbol as an incomplete array (`extern s32 x[];`)
  to force absolute addressing.
  **The incomplete array goes too far for a constant read inside a loop.**
  `extern f64 D_00533E30[]` with `D_00533E30[0]` at the use makes cc1 treat
  the array base as loop invariant: it hoists the `%hi`/`%lo` pair of both
  doubles into callee-saved registers ahead of the loop and grows the frame by
  16. Retail materialises `lui %hi` plus `ld %lo` fresh at each use, which is
  what a plain scalar `extern f64 D_00533E30;` gives, since the symbol lives in
  `.rodata` and is never `$gp`-addressed anyway. That was 11 of the 119
  differing words in `dds3EffectDistortionBlurDraw`, the frame size and two
  saved registers. So reach for the array form only where the symbol really is
  small-data, and read the frame size before and after.
- `div` against `divu`, and `lb` against `lbu`, come from a cast at the use
  site, not from narrowing the field's declared type.
- A symbol that looks standalone may be one element of a table already
  declared. `check_unit.py` reporting "retail uses a different address" on a
  `%lo` that otherwise matches is the tell: `fldGetBgmTrack`'s five track
  globals are `gFldBgmTop[50]` through `[56]`, and only the `ninja` checksum
  catches it otherwise.
- A row stride that is not a power of two comes from subscripting a
  two-dimensional array. `char names[][28]` gives retail's `sll 3`, `subu`,
  `sll 2`, and `[][26]` gives `sll 1`, `addu`, `sll 2`, `addu`, `sll 1`.
- Reuse the loop index variable for a value clamped after the loops. Where
  retail's `movn` writes a saved register, a fresh local gives `movz` with the
  value already in `$2`.
- A store to a struct field aliases a global table of the same type, so
  reading `tbl[i].f` after the store forces a reload. A local holding the
  field is what matches.
- **A `u8` read is alias set 0 and serialises every store around it**, which
  is a scheduling fact before it is an addressing one. ee-gcc 2.96 gives
  character types the universal alias set, so one `source->color[0]` read
  conflicts with every store through an unrelated packet pointer, and a run of
  `lbu`/`sw` colour pairs can then no longer have anything scheduled into each
  load's latency: the pairs come out back to back with the float stores parked
  on one side of them. Retail interleaves one coordinate store into every
  load's latency and reuses one register for all four bytes, which is exactly
  what the same body emits when those four reads are `s32` fields instead, so
  the tell is reproducible: swap the byte reads for same-sized integer reads
  and see whether the schedule falls into place. A `u32 r : 8` bitfield does
  give `lbu` and does break the false dependence, but it swings past the
  target, since a bitfield read conflicts with neither the integer nor the
  float stores and cc1 then hoists all four loads into four registers ahead of
  every store. `dds3EffectFilterDrawTexQuad` sits on this at 22 of 110 words;
  see [Non-matching drafts](nonmatching.md).
- A colour read-modify-write whose new value comes out of a float-to-unsigned
  conversion needs the converted value in its own `u32` local. Written as one
  expression, `p->color = (p->color & 0xFFFFFF) | ((u32)(...) << 24)`, cc1
  hoists the `lw` of the old colour above the conversion's select and loses the
  branch-likely with it; `level = (u32)(...); p->color = (p->color & 0xFFFFFF)
  | (level << 24);` keeps the load inside the two arms where retail has it.
  `dds3EffectRippleBlurDraw` went from 43 differing words to 19 on that one
  local, where the reversed `or` operands and a `* 0x1000000` in place of the
  shift are 21 and 43.
- A float expression needed both before and after a call, where retail copies
  it with `mov.s`, is the **expression written twice**, not a named `f32`
  local. The local is live across the call, takes a second callee-saved float
  register and grows the frame by 16; written at both use sites, cse turns the
  second one into the copy. `dds3EffectRippleBlurDraw`'s `1.0f - slot->phase`
  feeds the displacement multiply and the alpha scale and needs this.
- A run of packet builders that each allocate a buffer, fill it and link it
  takes **one function-scope pointer for some of them and a block-scope
  pointer for the rest**, and which is which is worth sweeping mechanically.
  A shared local gets its own pseudo and a `daddu rd, $2, $0` copy; a
  block-scope one coalesces with `$2` and stores straight through it. Retail
  does both inside one function: `dds3EffectBlurDrawFrameMask` copies the first
  two packets into `$7` and stores the last two through `$2`, and all sixteen
  combinations of its four blocks were measured, with the right one exact and
  the next best 45 of 146 words. `dds3EffectBlur`, with three packet blocks,
  wants block scope for all three.
- The argument setup for a tail call that follows a counted loop gets hoisted
  above the loop's guard and lands in its `blez` delay slot, where retail fills
  that slot with the first load from inside the guarded block. Nothing in the
  source reaches it: eight loop spellings (`do`/`while` with the decrement
  before or inside the test, a `for` inside the guard, a bare `for`, a `for`
  with a `> 0` bound, a `while`, the induction pointer initialised in the `for`
  init, and separate bound and counter locals in both orders), and an early
  `return` in the skip arm carrying its own copy of the call, all leave it or
  make it far worse. `dds3EffectRippleBlurDraw` and
  `dds3EffectDistortionBlurDraw` are both capped by exactly this, at 18 of 122
  and 8 of 130 words.
- Where retail **reloads** a `void *` handle after a store made through a
  pointer derived from it, cast the store's lvalue to `void **`. ee-gcc 2.96
  gives each pointed-to type its own alias set, so a store to a
  `struct sdf3DLightParallelPacket_t *` field does not kill an available load
  of a `void *` field and cc1 keeps the handle in a register across it, one
  word short and in the wrong register. `*(void **)&obj->inner->light = light;`
  puts the store in the same alias set as the load and restores the reload.
  That was the last 6 words of `dds3ObjectBaseModelLinkOt`. The cast at the
  dereference is what matters, not the stored value's declared type: a `u32`
  local and a `u32 *` store do not reach it, while `(void **)` on the field's
  address, a `(void **)` subscript of the base and a `(void **)` cast of a
  `u8 *` sum are all exact.
- `for (i = 0, ofs = 0; ...)` against `ofs = 0;` above the loop decides which
  of the two variables is zeroed first.
- A row offset of `n - 1` folds into the load displacement on a two
  dimensional array but not on a flat index: `tbl[max - 1][r]` with
  `extern s32 tbl[][10]` gives retail's `lw -0x28(reg)`, where the same
  arithmetic as a single index keeps an `addiu -10`.
- Two `if`/`else` arms that read-modify-write the same kind of address get
  cross-jumped into one `ld`/`or`/`sd` tail, where retail has the tail twice.
  Put the byte offset of one arm in a `u32` local, `u32 ofs = 0x17F10 +
  (floor * 32 + room) * 8;`, and dereference `(u8 *)GLOBAL + ofs`. That is
  what changes gcc's association of the three-term sum from
  `(index + base) + const` to `(index + const) + base`, so the two arms end up
  with the pointer in different registers and the tail is no longer
  cross-jumpable. Parenthesising the constant with the index in the expression
  does not work: gcc reassociates every form tried.
  `fldAutoMapOpenBlock` went from 137 of 168 words to exact, and
  `fldAutoMapOpenBlock2` from 28 of 40, with the local in the `>= 30` arm
  only.
- A store through a cast pointer may alias a `gp`-addressed pointer global, so
  gcc reloads the global inside the inner loop. Cache it in a local `u8 *`
  first, once per loop where retail reloads it once per loop. That took
  `fldAutoMapClear` from 35 differing words to 6.
- An inner loop that walks up (`addiu ptr, 8`) while its counter walks down
  comes from `*p++` on a row pointer. An address derived from the index
  instead, `base + (i * 32 + j) * 8`, makes gcc reverse the loop, so the
  pointer descends from the last element and the row constant is the last
  element's offset.
- Where retail folds a constant offset into a symbol's `%hi`/`%lo` pair and
  then uses small displacements, the source holds a pointer to the sub-object:
  `p = (T *)&dds3LightParam2[0x20];`. Indexing the array at each use leaves the
  `0x20` in the store displacements instead.
- Storing `0.0f` to several float fields gives `sw $0` per store. Where retail
  materialises the zero once with `mtc1 $0` and stores it with `swc1`, which is
  one word more in total, assign it to an `f32` local first and store the local.
  Copying one field into the others works as well.
- An `s16` table field used once at `* 16` and once at `* 8` comes out as a
  second `lhu` plus `sll 16`, `sra 13` for the narrower scale. Read the field
  into an `s32` local and scale the local, which is what gives retail's single
  `lh` and two `sll`s.
- `s16 *t = table[index];` as an initializer at the declaration and
  `t = table[index];` as a statement after the function's leading calls differ
  by one saved register and by whether `index * 16` or the whole pointer is the
  value kept across the calls.
- Do not cache a value a call consumes in each arm. Written at the call site,
  `effColorBlend(work->color & 0xFFFFFF, work->color, blend)` is exact; the
  same value in a local spends a saved register and copies it into `$5`. That
  took one function from 42 differing words to 0.
- An uninitialised local passed as an argument is a real retail shape:
  `Create(work, NULL)` with `work` not yet assigned is how one register serves
  as both the argument and the result.
- A `Create` that reads its parameter block at offset 0 needs the parameter to
  have its own type, not the work type. Where the parameter store is not the
  first store, read it into a local first so the `lw` lands ahead of the
  stores.
- A local caching a `gp`-addressed global pointer (`evtViewer_t *viewer =
  viewerinfo;`) held across the whole body costs one pseudo and shifts every
  later allocation. Where retail re-reads the global, write `viewerinfo->`
  at every use and let gcc keep the one load: stores through the pointer
  invalidate the cached load by themselves, so retail's reload pattern falls
  out. Four functions in `event/evtEventViewer` turned on that alone, one of
  them from 58 differing words to exact.
- A loop bound used once belongs in the `for` condition, not in a named local.
  `for (i = 0; i < a->x + a->y; i++)` gives retail's `addu` into `$2` and a
  copy into the compare register; `bound = a->x + a->y;` coalesces the add
  into the compare register and loses two words.
- Where retail has `movn rd, rs, rs`, the cmov whose value and condition are
  the same register, write an explicit `if (x != 0) y = x; else y = <default>;`.
  The `y = <default>; if (x != 0) y = x;` form reads better but gcc sinks the
  default into the test and emits a branch pair; no ternary spelling of it
  reaches the cmov either.
- A string the retail pool keeps next to padding or an empty string is safer
  as `INCLUDE_RODATA` plus `extern char D_XXXXXXXX[];` than as a literal in
  the body. `include_rodata.py` picks up any rodata symbol named this way,
  including one splat had folded into a nonmatching function's own asm file.
- Two `addiu` of the same frame address where the C uses `&local` twice means
  the store before it is `SDF_VU0_STOREVECTOR_NOMEM`, not
  `SDF_VU0_STOREVECTOR`. The `"memory"` clobber lets cc1 keep the address
  pseudo across the asm, so a following `SDF_VU0_LOADVECTOR(&local)` reuses
  the register; without it cc1 rematerialises `$29 + offset` for every asm
  operand, which is what the `nbCameraShot_*_Set` family does throughout.
  `nbCameraShot_InvokeTargetNear_Set` went from 73 differing words to 4 on
  that one macro, and its neighbour from 82 to 11.
- An accumulate into a saved fp register (`add.s $f20, $f20, $f1`) needs
  `x += ...; x *= ...;` as separate statements. `x = (x + a) * b` puts the sum
  in the addend's register and adds a `mov.s` to the variable's register at
  the end.
- The alignment of a struct assignment comes from the assigned type, not from
  the object it sits in, so one set of bytes can need two struct types. In
  `slSaveData_writeFile` the save header inside the global work copies its
  20-byte block as `ldl`/`ldr` plus a plain `lw` for the trailing word
  (alignment 4, from a `struct { s32 w[5]; }` member), while the same 20 bytes
  in the on-card record copy as `ldl`/`ldr` plus `lwl`/`lwr` (alignment 1,
  from a `struct { s8 ch[20]; }` member). One struct for both gets one of the
  two copies wrong whichever way it is declared.
- A `$gp` reload landing in `$3` where retail uses `$2`, immediately after a
  call whose result is ignored, means that callee is `void` in retail's
  compile: a non-void declaration keeps the result pseudo alive and gcc skips
  `$2` for the reload. If turning the callee `void` then makes its own body
  sibcall its last call, block that with a dead store to the parameter rather
  than the `s32`-with-no-`return` form, which is what forced the callers to
  skip `$2` in the first place. `fclDestroyRag` and `fclDestroyRecover` went
  from two and four differing words to exact when `fclScriptForcedEnd` and
  `fclRecoverFreeTextures` became `void` with a `script = NULL;` dead store.
- A constant offset that retail leaves in the load displacement while
  hoisting only the bare symbol comes from a pointer local:
  `info = &tbl[i]; info->field`. Written as `tbl[i].field` gcc hoists
  `tbl + offsetof(field)` out of the loop and loads at `0(reg)`.
- Where retail shifts a run of parameters in argument order but stores the
  shifted values in some other order, the shifts are their own statements:
  `u0 <<= 4; v0 <<= 4; u1 <<= 4; v1 <<= 4;` above the stores. Written inline
  at the stores the shifts follow the store order instead. `fclSetSprite`
  came down to that plus the store list in retail's emitted order.
- Where retail recomputes the same `addiu rd, $29, N` or `addiu rd, base, N`
  for every VU0 access to one local, and the C holds it in a saved register
  instead, use `SDF_VU0_STOREVECTOR_NOMEM` for the store. The `"memory"`
  clobber on the plain store is what lets gcse share the address expression
  across the next call, and dropping it makes gcc rematerialise it the way
  retail does. Volatile asms keep their order either way, so a following
  `SDF_VU0_COPYVECTOR` or `_LOADVECTOR` of the same local still reads it.
  `nbCameraBoss_Sp_JAREIH_Run` lost a saved register and
  `nbCameraBoss_BS{KAGU,LUSI}_Command_Calc` each lost an `addiu` on that one
  change. Only use it where the C does not read the stored vector back by
  field, since gcc is then free to treat the local as unwritten.
- Two successive divides into the same destination come from reusing one
  float local, not from two named locals. `nbCameraShot_BossEX02_Set` writes
  `d = half / angle; pCamera->distance = d; d = f(party) / angle;`, which
  keeps both quotients in `$f0`; a second local moves the first one into a
  callee-saved register and rotates six words.
- Writing the derived value at its uses, `(gSlWindowCnt - 2) * 0xE9`, lets
  gcc distribute the multiply (`gSlWindowCnt * 0xE9 - 466`), two words more
  than retail's subtract and multiply. A local blocks that.
- The register the reused local gets follows its neighbours. gcc's
  `expand_preferences` unions the hard-register preferences of any two global
  pseudos joined by a single set whose source dies in it, so a counter
  multiplied into a value that is copied into an argument register is pulled
  into that argument register itself. Keeping the value that reaches the call
  out of the chain (here the scaled alpha is its own expression at the call,
  and the unscaled one shares the offset local that retail copies into `$4`)
  is what leaves the counter in `$4`.
- Where retail folds a float constant's register into the result of the
  expression that consumes it (`lwc1 $f20, 3.14f`, then
  `mul.s $f20, $f0, $f20`, then `div.s $f20, $f20, $f1`), the source assigns
  the constant to the destination variable first and then rewrites it in
  separate statements: `angle = 3.14f; angle = -x * angle;
  angle = angle / 180.0f;`. Any one-expression spelling gives the constant a
  temp and the quotient a different saved register.
- A second loop-invariant float constant used across the same calls needs its
  own named local, assigned right after the first one, or it takes the lower
  saved register. `calcCamNormalNPC` went from 16 differing words to 6 on
  `dist = 200.0f;` in place of the literal at its four use sites, and to exact
  once the angle was written as the three statements above.

- A jump table needs enough `case` **nodes**, but the threshold is not 10: an
  eight-entry table and a six-entry one both emit tables here, so the earlier
  claim of `case_values_threshold == 10` is wrong as stated. The actionable
  mechanism is that the node count is taken **before** empty arms are merged.
  Where retail has a table and the reconstruction has a compare tree, give
  each extra arm a dead store to a local nothing reads after the switch,
  `case 8: seq = 0; break;`. Flow analysis then deletes every store and those
  entries land on the shared exit. One function went from 59 of 79 words
  differing to 1 on that alone, where a live assignment leaves the block in
  and runs 16 bytes over. Confirmed again at seven entries and at six, and the
  node count is still not the whole story: `basic/dds3ObjectBase` has four
  `switch`es over the same `u32` field with the same case values, and the three
  in `dds3ObjectBaseModelLinkOt` (cases 0 to 6 twice, 0 to 5 once) emit tables
  while `dds3ObjectBaseMatterAnimeSpeed`'s cases 0 to 6, with two stores in
  every arm and no empty arm at all, emits a compare tree and matches that way.
  So read which one retail has rather than predicting it from the arm count.
- `extern f32 fabsf(f32);` and an ordinary call compile to `abs.s` directly,
  verified against this toolchain. `EE_ABS_F32` is what fails to match where
  retail has a plain `abs.s`. **But the choice is also a scheduling lever, and
  it runs the other way in a VU0 body**: where retail materialises the other
  operand's constant *after* the `abs.s`, `fabsf` lets sched1 hoist the
  `li.s` into the `mtc1`-to-`abs.s` latency and `EE_ABS_F32` does not, because
  its inline asm is a barrier. `dds3BillboardGetDirectionScale`'s
  `1.0f - fabs(dot)` sat at 3 of 26 words with the call and every placement of
  the constant (its own local before or after the `abs`, a reused local, a
  result local) and went exact with `EE_ABS_F32(dot, dot)`. Confirmed again
  outside a VU0 body, on a run of magnitude tests: `evtCommand_MODEL_LIGHT_DIR`
  compares `fabs` of three vector components against one constant, and with
  `fabsf` the constant and the retained third component take each other's
  float registers and the constant's `lwc1` lands after the component copy,
  8 of 53 words out under every shape tried (an `&&` chain, nested `if`s,
  `goto`s, a named bound local at four positions, an `f32` local per
  component, `sdfVector4_t` for the array, and `_MEMOPS` for the `lqc2`).
  `EE_ABS_F32` into **one reused** magnitude local, with the three tests
  nested, is exact: the barrier is what forces the constant to be materialised
  before the copy.
- The same thing happens by accident when a `void` callee defined later in the
  unit has no prototype above the caller: the implicit declaration returns
  `int`, so the load after the `jal` lands in `$3` where retail uses `$2`.
  Adding the `void` prototype above the caller is the fix. `SetMakaPacket`
  came down to that one word, on `nbSetJyokyoMes`.
- Where retail computes a table row as `addu rd, index, base` *into the
  register the index already occupies*, the index has to reach the add as a
  pointer: `(T *)((u8 *)(i * sizeof(T)) + (u32)base)`. Written `base[i]` gcc
  emits `addu rd, base, index` into a fresh temp, which costs the operand
  order and the register. `nbRunKoukaType1` and `nbRunKoukaType2` each went
  from 20 differing words to exact on that one cast.
- Stack vectors a function stores with `sqc2` and later copies with `lq`/`sq`
  have to be local *aggregates*, not scalar `u128`s, when retail's frame layout
  follows the declaration order. gcc gives an aggregate its slot at
  `expand_decl`, in declaration order, but gives a scalar its slot only when its
  address is first taken, so a run of `u128` locals comes out in first-use order
  instead. `effect/effPCPBoss` declares
  `union effPCPBossVector_t { u128 q; f32 f[4]; }` for this: the `u128` member
  supplies the 16-byte alignment `sqc2` needs and the union makes it an
  aggregate. `effPCPBossDengekiDraw`'s ten vectors land at `0x30` through `0xC0`
  only in that form.
- A `SDF_VU0_STOREVECTOR` to a local that a later `SDF_VU0_COPYVECTOR` reads as
  its source has to be `SDF_VU0_STOREVECTOR_NOMEM`. The plain form's `"memory"`
  clobber lets gcse share the `addiu rd, $29, N` between the store and the copy,
  which is one instruction fewer than retail and parks the address in a saved
  register across the intervening call. That took `effPCPBossDengekiDraw` from
  182 differing words to 16. It is needed even where the C reads the stored
  vector back by field, against the caution on the macro itself.
- The `effEveBEDCutSetParams` placement block in the draw routines is written
  zeros first: `payload[3..6] = 0.0f`, then `payload[7]`, `[8]`, `[9]`, `[10]`,
  then `payload[0..2]` from the position, then `color`. Every order that follows
  the field numbers leaves the last `swc1` on the wrong side of the three
  `sw $0`, and no position for `payload[8]` alone fixes it:
  `effPCPBossDengekiDraw` and `effPCPBossKAGUThunderboltDrawSub` both sat at 4
  and 8 differing words until the zeros moved to the front, then went exact.
- An interpolation factor that retail builds with a bare `lui`/`ori` pair is a
  `u32` local holding the binary32 bits, assigned just before the call that
  precedes the loop, and passed to `SDF_VU0_INTERPOLATE`. As a literal inside
  the loop body gcc hoists the materialisation into the preheader and rotates
  every saved-register assignment; `PS2_FLOAT_BITS` is wrong here because it
  adds the `mtc1`/`mfc1` pair retail does not have.
- A local that gets spilled competes for slot order with the others. Where
  retail gives the lower spill slot to a value read in the prologue, declare the
  loop-local one (a `found` pointer, say) after every other local;
  `effPCPBossMETATOBeamDraw` swaps its two spill slots otherwise.
- `Hatudo` and `nbE915_Test` call `nbGetEventForm` with no argument, which
  only an unprototyped declaration produces, and that function is defined
  with a prototype in the same unit. An unprototyped `extern` does work for
  callers placed above the definition, which matters if the unit is split.
- Where retail updates a base coordinate in place, `addiu $19, $19, -2`, and
  you fold the constant into every use instead, write the adjustment as its
  own statement on the same variable: `x = (s32)(...); x -= 2;`, not
  `x = (s32)(...) - 2;`. In the one-expression form the unadjusted value stays
  live and cc1 folds the offset into the first use, so the base is
  materialised and bypassed at once. That fixed the whole prologue of
  `fldVLDraw`.
- Locals that each `switch` arm assigns and uses for itself belong inside the
  arm's own block, not at function scope. At function scope the arms share one
  pair of allocnos whose live range spans the switch, which rotates them
  against whatever else is live across it: `fldVLDraw`'s seven sprite arms put
  the alpha value in the register retail gives the coordinates and were 45 of
  370 words out, and went exact with `s32 sx; s32 sy;` declared in each arm.
- A block-scope aggregate initializer is not the only way to reach a rodata
  table the unit already carries. Declare the splat symbol with the real type
  and assign it, `axis = D_00531148;` with `extern sdfVector4_t D_00531148;`,
  which gives retail's `ldl`/`ldr` plus `sdl`/`sdr` copy from `%hi`/`%lo` of
  the symbol and leaves the `INCLUDE_RODATA` line and the pool order alone.
  An initializer at the declaration instead emits the copy in the prologue,
  before anything else in the body, and moves the data into the compiler's own
  rodata, which is what the function-local-array section below is about.
- Where retail forms a symbol's address and then adds a constant in a separate
  `addiu`, put the bare symbol in a pointer local inside the block that uses
  it and add the offset at the use: `char *roof = D_002FBD40;` then
  `roof + 0x30`. Written `&D_002FBD40[0x30]` gcc folds the offset into the
  `%lo` and is one word short. `fldElvGetFloorDigits` went from 19 differing
  words to 8 on that alone.
- A quotient and a remainder of the same division belong in one reused local,
  not two. `digit = n / 10; ... digit = n % 10;` gives retail's `mflo`/`mfhi`
  register pair and keeps the second divide-by-zero trap at the `%`
  statement's position; two named locals rotate four registers, and writing
  either expression twice adds a third trap.
- A `sw $0` that sits in the wrong place *among* a run of other `sw $0` stores
  is the same tell read the other way: the field is `f32`, not `s32`. In
  `fldClearEveRngTable` the zero store to `fldRoomState_t + 0x114` came out
  four words early, ahead of the two integer zeros next to it, and no ordering
  of the thirteen statements in that tail moved it; declaring the field `f32`
  and writing `0.0f` put it exactly where retail has it and took the function
  from five differing words to exact.
- An `andi 0xffff` after the `andi` of a bit test comes from reading a `u16`
  struct field directly in the condition. Read it into an `s32` local first
  and the truncation goes away; `SetBattleFlag` turned on that alone. Retail
  does emit the pair where the field really is tested in place
  (`nbGetUnitWorkPointer` and `CheckErrorActionStack` both have it), so the
  tell is which side has the extra word, not the shape.
- Two bits cleared in one mask come out as one `addiu`/`and`; retail's two
  `addiu`/`and` pairs with a single store come from two separate statements,
  `x &= ~0x10; x &= ~0x80;`. gcc merges the dead store but not the masks. One
  function in `newbattle/nbMainProcess` turned on that alone.
- Two reads of the same pointer field in one function, where retail puts them
  in different registers, need two distinct locals. Reassigning one local for
  the second read keeps retail's reload but not its register.
- A placeholder field declared `u8 unk_NN[4]` in a shared header decays to its
  own address, so `if (data->unk118 == 0)` compiles to a compare against a
  constant and silently tests nothing. Read it through `*(s32 *)data->unk118`
  until the field has a type.
- Where retail loads two function pointers out of the same kind of struct and
  calls each, into the *same* register, put the pointer in one local and reuse
  it: `reg = ot->regFunc; reg(ot, tag); ot = ...; reg = ot->regFunc;
  reg(ot, tag2);`. Calling `ot->regFunc(ot, tag)` directly puts the second
  load in `$3`, since `$2` is still the first call's result register.
  `nbUnitDrawProcess1` went from two differing words to exact. This is the
  reused-destination form of the plain-copy rule above.
- A callee defined in the same unit can take a parameter it never reads, and
  still match: `nbGetUnitMarkPos` has to be declared with three parameters
  because `nbUnitDrawProcess1` passes a draw tag it ignores. An unused
  parameter costs no instructions, so adding one is free on the callee and is
  what lets the caller compile at all.
- Which of `beq`/`beql`, `bne`/`bnel` and `bgez`/`bgezl` ee-gcc picks is the
  delay-slot filler deciding whether the slot's instruction is safe on the
  untaken path, and it is not reachable from the branch's own source shape.
  Three functions here (`nbRunAi`, `nbE901Process_1`, `nbE905_End`) sit one
  word from exact on that bit alone after every spelling of the branch,
  the loop and the callee's declaration. Restructuring the block the slot
  instruction belongs to is what moves it, as in the `NegoDebug` row above;
  varying the branch does not.
- One float local reused for two unrelated values is how retail gets both into
  one register, and it is the commonest residual in the `nbCameraShot_*_Set2`
  family. Where retail's atan result and a later negated distance share `$f3`,
  or the `0.5f` and the quotient that replaces it share `$f21`, the source has
  one local doing both jobs: `scale = sdfAtan(...); radius /= scale; ...
  scale = -radius; neg.x = scale;`. Two named locals leave the second value one
  register higher and every later assignment rotates.
  `nbCameraShot_Target_Set2` sat at 2 of 178 words on exactly that and went
  exact; `nbCameraShot_All_Set2`'s accumulator, its `0.5f` and its
  `SDF_VU0_GETLENGTH` output are all one local for the same reason.
- `SDF_VU0_STOREVECTOR` against `_NOMEM` cuts both ways within one function,
  so sweep the pair mechanically rather than reasoning about it. In
  `nbCameraShot_Target_Set2` the *first* store of a stack vector has to be
  `_NOMEM` so that a later store of the same vector rematerialises
  `addiu $2, $29, N` instead of reusing the saved register the load got: the
  plain form there was 47 of 177 words out and `_NOMEM` took it to 2. In
- Where a global's value lives in one register across a whole loop and is
  reloaded only after a call, nothing else in the loop may clobber memory.
  `dds3SceneLightUpdateFirst` keeps `dds3DrawFlag` in `$7` for three
  iterations and reloads it only after `Lerp` and `ToMatrix`, so both of its
  colour unpacks are `MMI_RGBA_UNPACK_NOMEM` and both vector stores are
  `SDF_VU0_STOREVECTOR_NOMEM`; a `"memory"` clobber on any one of them adds a
  `lw` reload at every flag clear and at the loop top, and took the function
  from 145 of 383 matching words to 21. Read whether retail reloads the global
  after each asm block before choosing the macro, since that reload is the
  clobber's visible signature.
  `nbCameraShot_AttackTuibiReverse_Set` the plain form's `"memory"` clobber on
  the second of two stores let cse keep a `0.5f` in a callee-saved register
  across the intervening call, collapsing retail's two `lui`/`mtc1` pairs into
  one; `_NOMEM` on that store restored them and the function went from 48 of
  151 words to exact. Nine combinations of the two macros took a second to
  measure.
- A float constant that retail materialises with `lui`/`mtc1` *after* the
  multiply whose operand register it reuses comes from one reused float local
  holding both values: `t = p->scl; radius = width * t; t = 100.0f;
  if (t < radius) { t = t / radius; radius *= t; }`. As a literal the constant
  is its own pseudo, sched1 fills the operand load's latency with the
  `lui`/`mtc1` ahead of the multiply, and the overlapping live range then
  costs it the multiply's register. The write to the shared local is what
  pins the order. `nbCameraShot_InvokeTarget_Set2` and
  `nbCameraShot_InvokeTargetReverse_Set2` each went from 5 differing words to
  exact on that one local.
- A run of locals whose initializers are constants materialised in the prologue
  takes its registers from the *declaration* order, and that order is a real
  lever. `fldAutoMapDrawBgOfsRF` passes eight constant texture coordinates to
  `sdfPrimLinkFt4`, each in its own callee-saved register; declaring them
  `au, av, du, dv, bu, bv, cu, cv` (the fourth corner's pair moved ahead of the
  second and third) took it from 10 differing words to exact, where all 24
  orders of the pairs and all 16 u/v swaps inside them were swept to find it.
  The same permutation in its sibling `fldAutoMapDrawIconOfsRF` is *not*
  reachable this way, because there the four values are computed from a table
  mid-function rather than initialized: declaration order and assignment order
  both have no effect at all on them.
- Where a function fills several stack vectors and then transforms each through
  VU0, two orders are separate levers and both are worth sweeping: the order the
  vectors are filled in, and the order of the field assignments within each one.
  `fldAutoMapDrawBgOfsRF` and `fldAutoMapDrawIconOfsRF` both want the four
  vectors filled `v0`, `v3`, `v1`, `v2` (8 words each over the natural order),
  and both want `.z` written before `.x` in the *second* vector only and `.x`
  first in the other three (14 words). Nothing about the asm suggests either;
  24 and 16 variants are cheap to measure.
- An inner loop whose counter walks down while its pointer walks up, with the
  outer base recomputed from the index rather than strength-reduced, comes from
  a pointer local whose base is formed `(u8 *)(i * stride) + (u32)base` and whose
  constant offset is then added in a *separate statement*. `fldAutoMapClear`
  needed `u64 *row = (u64 *)((u8 *)(i * 256) + (u32)gb); row = (u64 *)((u8 *)row
  + 0xFC58);` exactly: with the constant inside the cast sum, or with the base
  written `gb + 0xFC58 + i * 256`, ee-gcc strength-reduces the outer base and
  drops the `sll` retail keeps (two words short); with the address derived from
  the index at the store it reverses the inner loop instead (two words, the
  wrong direction). Subscripting the row pointer by the constant
  (`row[0x1F8B]`) keeps the direction but spends a `lui`/`addu` pair on the
  displacement.
- Two subtractions of 1 from two different parameters, where retail evaluates
  one before a `div` and the other after the divide-by-zero trap, *and* keeps
  the first one's result out of its own parameter register. The two are coupled
  and C cannot separate them: the `beql`/`break` trap is one insn pattern
  rather than a basic-block boundary, so the scheduler emits whichever
  subtract is evaluated first in RTL ahead of the `div`, and that same first
  subtract is the one that coalesces with its parameter's register. A comma
  expression proves the rule is RTL evaluation order and not statement order.
  `fldAutoMapCloseBlock2` sits two words from exact on it; see
  [Non-matching drafts](nonmatching.md).
- A constant that retail adds to the *scaled index* rather than to the base
  (`addiu rd, index, 0xC0` and then one `addu`) comes from an integer offset
  local: `s32 ofs = index * stride + 0xC0;` and a cast pointer off the work
  base. `&work->arr[index]` associates the other way, `(work + index*stride) +
  0xC0`, and leaves the `0xC0` in the load displacements.
  `cmpStockHasRoom` went from 17 of 21 words to exact on that alone. Its work
  type is two arrays indexed by one parameter, `cmpLocalStock_t stock[2]` of
  stride 0x56 at 0 and a 0x103C-byte record at 0xC0, which is the same pair
  `cmpGetStockInfoIndex` reads.
- A constant byte offset into a global table folds into the symbol's
  `%hi`/`%lo` pair only when the source names a flat array at that address.
  `extern s8 D_00339E06[]` indexed `race * 66 + index` gives retail's
  `lb $2, %lo(D_00339E06)($1)`; the same bytes reached as
  `D_00339E00[race].field[index]` with the row struct split keep the `+6` in
  the load displacement and cost four words per access.
  `cmbGetHeartsParamByIndex` went from 12 of 22 words to 8 on that.
- A `lui`/`addu`/load triple using `$at` is `ee-as` expanding `lw rd, sym(rx)`,
  not cc1 splitting the address. The assembler uses `rd` itself as the temp
  whenever `rd != rx`, so a triple through `$at` means retail's index and load
  destination are the *same* register. Read it as a register-allocation tell,
  not an addressing one: `trmGetJumpTerminalNo` and
  `cmbGetHeartsParamByIndex` both sit on it.
- An index whose stride is 4 on what the struct declares as a byte array means
  the record is four bytes wide. Read it with a cast at the use site,
  `*(s8 *)((u8 *)WORK + sel * 4 + 0x4110)`, rather than reshaping a field other
  functions already use: `cmbGetBirthDevilPanel` was 19 of 19 words out with
  `slotIndex[sel]` and exact with the cast.
- A `u8` parameter used as a loop counter has to be copied into an `s32` local.
  Decrementing the `u8` itself emits `addiu rd, rs, 0xFF` plus `andi 0xFF` in
  every arm where retail has one `addiu -1`; the entry `andi 0xFF` the `u8`
  parameter gives is still correct. `cmbCountEvoCandidates` turned on that.
- Where retail copies a loop bound into a second register (`daddu $3, $2, $0`)
  before the loop, the source has a separate counter local initialised from the
  bound and decrements the local, leaving the bound untouched. Reusing the one
  variable gives no copy. `func_00242710` turned on that plus a `u8 *` cast for
  the `lbu` of an `s8` field.
- A `gp`-addressed work pointer that retail holds in two registers comes from a
  local assigned from the global plus the global written at the other uses, and
  which half gets the local matters: `rstUpdateEvoBgFade` wants the local for
  the first test and the store, and `startPos->` for the clamp.
- An `f32` field that retail keeps in `$f0` across a read-modify-write is
  written in place, `work->ofs -= 16.0f;`, not through a named `f32` local. The
  local takes `$f1` and leaves `$f0` to the literal, and no declaration order
  of the value, the step and the bound swaps them back.
  `MoveScreenOffset` went from 9 of 22 words to exact. A local declared inside
  each arm works too.
- `fsqrtf` from `include/fpu.h` is what produces a bare `sqrt.s $f0, $f0`, and
  ee-gcc does not fold it even on a constant: `fclRagCalcTexSize` really does
  compute `sqrtf((f32)(size * 16))` at run time, and
  `campGetCursorColorAlt` computes `(s32)fsqrtf(512.0f)` and squares it.
  A `mult`/`mult1` pair falls out of the two multiplies with no help.
- A global pointer read inside a loop beats caching it. `g_pinst->texHandle[i]
  = src[i]` gives retail's one `lw` in the preheader, the `addiu 0x38`, two
  walking pointers and an `slti` up-counter; an `s32 *dst` local for the
  destination makes gcc reverse the loop into a downcounter, and `dst[i] =
  src[i]` with the local adds a third induction variable.
- An epilogue duplicated into a branch-likely delay slot, where ee-gcc emits
  the plain branch. **Steerable when the branch is a sign test**: spell it
  `(x & 0x80000000) != 0` rather than `x < 0`, which took one function from 8
  differing words to exact. Only the non-sign-test cases have resisted. Note the narrow scope: ee-gcc 2.96 emits `bnel` from
  ordinary C readily, 42 times in `field/fldDraw` and 126 in
  `newbattle/nbMakePacket`, so a `bnel` in the retail code is not a reason to
  skip a function. Only this one use of it has resisted.
- Two arms of an `if` holding the *same* call is a real retail shape, not only a
  cross-jumping artifact. `RunBadMotKaifukuKouka` has two `jal nbSetUnitMotion`
  with identical argument setup hoisted above the branch, and an `if`/`else`
  whose arms are the same statement reproduces it; the condition has to be
  spelled `== 0` rather than inverted, which is what decides the `bnez`/`beqz`
  order. The `func_00214FD8` row below is the case where gcc collapses them, so
  measure rather than assuming either way.
- Two arms of an `if` that compute the same thing. gcc 2.96 cross-jumps them
  whatever the source does: `func_00214FD8` has the same sign test on
  `dds3PadButtonState[0x22]` in both arms, laid out differently in each, and
  every spelling (duplicated test, `>= 0` against `< 0`, the mask form, a
  ternary, an explicit `else`, and even *two different symbols* that resolve to
  the same address) collapses to one arm and 36 of retail's 56 bytes.
- A single-bit mask test ee-gcc folds. Retail's `andi 1` followed by a
  redundant `andi 0xFF` and an explicit branch is not reachable: see
  `cmpItemHasFlag` in the drafts.
- Where in a tail call's argument setup the second half of a `lui`/`ori`
  constant lands. ee-gcc's second scheduling pass moves the `ori` past the
  register restores; `cmp_23A918` and `fclDrawPanel` both sit on it and no
  spelling of the constant (a local, an unsigned literal, dropping the
  `return`) moves it.
- A negated comparison assigned to a variable needs the comparison in its own
  local first. `ok = (prev >= 0x13);` and every spelling of it (`!(prev <
  0x13)`, `== 0`, `!= 1`, `? 0 : 1`, `> 0x12`) gives `sltiu` then
  `xori rd, rs, 1`, because gcc knows the inner value is 0 or 1. Retail's
  second `sltiu rd, rs, 1` is the generic `== 0` expansion, which needs
  `t = prev < 0x13; ok = t == 0;` as two statements. That was the last word of
  `nbActionProcess1`.
- `case_values_threshold` is 10 in this build: a `switch` needs at least ten
  `case` nodes before ee-gcc emits a jump table rather than a compare tree,
  independently of the range test. Where retail has a jump table and the
  reconstruction has fewer labels, the missing ones share the `default` body.
- An address that looks like a global may be a `.lit4` pool entry holding a
  float constant, and must be written as a literal.
  `tools/check_unit.py` says so when you get it wrong. This also empties a
  branch delay slot: as an `extern` the `lwc1` gets scheduled into it, while
  the literal becomes a `li.s` macro that `ee-as` cannot place there, which
  is how retail's `nop` appears.
- Holding a struct address in a local (`nbCamera_t *p = &pNbCamera;`)
  reproduces retail keeping it in a saved register. Placement matters:
  initialising it at the declaration puts an `addiu` in the prologue and
  spends a saved register, while declaring it and assigning just before the
  first use does not. Assigning it after a leading call keeps it out of `$16`.
- The cheapest sibling-call blocker measured is `s32 dummy; s32 *p = &dummy;`,
  which costs nothing at all: the frame still reports `vars= 0` and no extra
  instruction is emitted. `volatile`, an array and a store each grow the frame
  by 16, while an old-style declaration, a varargs callee and a mismatched
  return signedness do not block it at all. It blocks **every** sibling call in
  the function, so it is the wrong tool where retail sibcalls some calls and not
  others: `dds3ObjectBaseModelLinkOt` has two `j callee` tail calls on early
  return paths and a `jal` plus epilogue on the last one, and the dummy pointer
  took it from 20 of 242 differing words to 207 of 236. A dead store after the
  one call that must not sibcall (`ot = NULL;` on an unused parameter) blocks
  only that one and took it to 6. Any dead store reaches it; three different
  variables gave the same result.
- Blocking the sibling call where retail uses `jal` and a full epilogue has
  two other forms, and which one works varies by unit. Giving a void function an
  `s32` return type with no `return` statement collapses a duplicated epilogue
  too, and took one function from ten differing words to exact. It failed for
  all seven functions tried in `code_0029B780`, where the answer was a dead
  store instead: introduce a local for one of an earlier call's constant
  arguments and assign 0 to it after the last call. Try both.
- An ignored return value still reserves `$2` across the call. When retail
  skips `$2` for the address temp after a `jal`, declare the callee `s32`
  rather than `void`.
- The `void`-callee remedy for an unused result reserving `$2` is not always
  available. In `code_001FAE80`, `fclDrawPanelSprite` is defined `s32` in the
  same unit, matches that way with `return cmpDrawSprite(...)`, and three other
  callers ignore its result, so changing it has to be measured across all of
  them at once rather than for the one caller in hand.
- An implicit declaration is itself a void against non-void trap. A callee
  defined further down the unit is implicitly `int` at the call site and
  reserves `$2`, so a forward prototype alone can fix a near miss: one added
  prototype took four functions from two words off to exact. When a `$gp`
  reload lands in the wrong register, check whether the callee is merely
  implicitly declared before concluding its return type is wrong.
- A local whose only initializer is a constant gets that constant materialised
  in the *prologue*, which costs a prologue instruction and rotates every
  saved register. Where retail sets it next to the code that first needs it,
  declare the local bare and assign it as a statement there.
  `frSetFontString` went from 72 of 115 words to exact on moving
  `glyph = NULL;` out of the declaration and down beside `i = 0;` and
  `previous = chain->lastChild;`, in that order. Declaration order is *not*
  the lever here: all 24 permutations of its four locals emit byte-identical
  code, and only the statement's position moves anything.
- An index read and then advanced reaches `addu base, i` plus `addiu i, 1`
  into one register only as a post-increment in a single expression,
  `text[(*offset)++]`. Written as two statements, the read and
  `(*offset)++`, gcc keeps the old and the new value in two registers.
  `frAnalyzeMessage` went from 20 differing words to 10 and `frAnalyzeTag`
  gained its whole prologue on that one change.
- One variable whose comparisons are signed (`slti`, `slt`) but whose byte
  extraction is `srl` needs the cast at that one use, `(u32)code >> 8`. An
  unsigned declaration fixes the shift and breaks both compares.
- A guard arm that is a single instruction belongs in the `else`, not the
  `then`: the delay-slot filler then absorbs it into the branch and the `b`
  over it disappears. `frPreCalcStrWidth` went from 32 differing words and one
  word long to 8 at the exact size by inverting
  `if (code >= 0x80) { <long> } else { index = code - 0x20; }` to
  `if (code < 0x80) { index = code - 0x20; } else { <long> }`, and
  `frCopyTagArgs` turned on the same inversion twice, once per nested test.
- Subscript the whole expression instead of caching an element address.
  `e->data->form[i].stat` matches where `form = &e->data->form[i];` then
  `form->stat` swaps the `addu` operands and shifts a register. The converse
  holds inside a loop retail re-reads: go through the outer pointer each
  iteration, since a cached local gets hoisted out and costs an instruction.
- `addu` operand order follows the cast order: `(u8 *)ofs + (u32)base` emits
  `addu rd, ofs, base`, and `(u8 *)base + ofs` emits the reverse. The same
  distinction separates pointer from integer arithmetic: `base + i` emits
  `addu base, i`, and retail's `addu i, base` needs the base cast to an
  integer first.
- A `gp`-addressed global can be cached in a local for part of a function.
  Where retail reloads it once for a loop and once for the tail, use the
  global inside the loop and a local pointer for the tail.
- `andi 0xff` after a `jal` comes from declaring the callee `s32` and casting
  at the use site, `(u8)f()`, not from giving it a `u8` return type.
- When retail biases a base once and stores at `0(reg)`, the induction
  variable is a pointer to the field, not to the struct. A struct pointer with
  a constant displacement gives `sw $0, 0xD0(reg)` instead.
- An induction pointer can be based one byte past its record: reading the
  weight at `p[-1]` and the item at `p[0]` matched where walking a
  `{rate; item}` struct lost 20 words.
- Split a packed argument into named locals in retail's computation order,
  `type = joken >> 16; para = joken & 0xFFFF;`, rather than using the
  expressions at their use sites. The parameter has to be `u32`, not an `s32`
  cast where it is read.
- A local assigned before the first call lands in a callee-saved register.
  Moving `sum = 0;` above a leading `jal` shifts every later allocation into
  place.
- A weighted-table scan compares before it accumulates: `if (r < sum + *p)
  return i; sum += *p;` gives retail's temp, compare, then copy. Accumulating
  first misses by several words.
- Pack a colour word through explicit `s32` locals assigned in `r`, `g`, `b`
  order. The same expression with the `(s32)(f * 128.0f)` conversions inline
  gives the right registers and `or` chain but a different mul and cvt
  schedule.
- A `u16` or `u8` parameter adds an `andi` on entry that retail does not
  have. Where retail copies the argument with `daddu`, declare the parameter
  `s32` and let the caller narrow: `fclEncycSearch`, `SetRecordToUnit` and
  `RequestModelLoad` each came down to that one word.
- A run of adjacent constant stores comes out right-rotated by one, so the
  source order is retail's list left-rotated: the store retail emits first is
  the one written last. `InitializeInstance` and `movieViewerFree` both
  matched on that rotation.
- Where retail adds the index to the base (`addu rd, index, base`) and leaves
  the rest of the offset in the load displacement, write
  `((u8 *)index + (u32)base)[disp]`. The plain `work->field[index]` form costs
  one register copy.
- Subscript the array with the loop index rather than walking a pointer when
  retail computes the pointer after the loop guard. A `*p++` walk makes gcc
  sink the pointer init into the guard's delay slot; `arr[i]` leaves gcc to
  create the walk itself, in retail's order.
- The EABI stack argument slot is eight bytes wide here, not four: the ninth
  and tenth arguments of a call sit at `0x0` and `0x8` of the outgoing frame.
- Pick the VU0 load by how retail forms the address:
  `SDF_VU0_LOADVECTOR(ptr)` where it computes the address into a register,
  `SDF_VU0_LOADVECTOR_MEMOPS` where it addresses the frame directly.
- `SDF_VU0_STOREVECTOR_NOMEM` is not only for globals and frame addresses
  shared across a call. It applies to a stack local inside a loop too, where
  the plain form's `"memory"` clobber makes the shared address a loop
  invariant and spends a callee-saved register on it. That took one function
  from 61 of 171 differing words to exact.
- Copy a vector quadword with `SDF_VU0_COPYVECTOR(dst, src)`, not a struct or
  `u128` assignment. The macro is one asm block, so the `lq`/`sq` pair stays
  together and the delay slot goes to the next call's argument; a `u128`
  assignment is two insns the scheduler splits, and it also hoists the `lq`
  above a store to the source that precedes it.
- A loop-invariant float constant that retail materialises into a saved fp
  register ahead of the loop index's init comes from a named local assigned
  before the loop. Written as a literal in the body gcc still hoists it, but
  into the preheader after the index init, which rotates four instructions.
- `sltiu` on a function result comes from a `u32` at the use site, either an
  unsigned return type or a cast. `effBTLModelGetGroupNumber(mask) < 2` with
  an `s32` declaration gives `slti`.
- Where retail forms a global's address twice, once into a temp for an `sqc2`
  and again as the following call's argument, use
  `SDF_VU0_STOREVECTOR_NOMEM(g)` rather than `SDF_VU0_STOREVECTOR(g)`. The
  `"memory"` clobber on the plain form does not block CSE of the address, so
  cc1 keeps one register and feeds both the store and the argument from it,
  two words short. Without the clobber it rematerialises. `nbSetFormRot` went
  from 16 differing words to exact on that one change.
- Where retail materialises `%hi`/`%lo` of `symbol + offset` fresh while the
  C holds the bare symbol's address in a register, declare the sub-object with
  `PS2_SYMBOL_AT`. In `effBTLFieldColorRun` a `SDF_VU0_COPYVECTOR` from
  `dds3LightParam.ambient` shared its base with the `dds3LightParam.sources[0]`
  load two statements earlier, so gcc kept the full address in a register,
  derived the ambient address with an `addiu +0x10`, and ran one word short.
  `PS2_SYMBOL_AT(effBTLVector4_t, effBTLSceneAmbient, dds3LightParam, 0x10)`
  gives the member its own relocation, which also lets the `%lo` of the
  pointer load fold back into the `lw`. That took the function from 13
  differing words to exact.
- A `u16` struct field used both as a loop bound test and as the loop counter's
  initial value needs **two** locals, and the bound one has to be `u16`. In
  `effBTLCutCreate` writing `if (cut->list->count != 0) { remaining =
  cut->list->count;` puts the count in `$2` and the reloaded `cut->list` in
  `$3` where retail has them the other way round; `u16 count = cut->list->count;`
  read before the pointer's other two members, with `remaining = count;` inside
  the guard, swaps the pair back and took the function from 9 differing words
  to 3.
- A `movz` selecting between two member addresses takes its two temporaries in
  the order the statement appears relative to its neighbours, not from how the
  selection is written. `effBTLCutCreate`'s `list = (kind < 2) ?
  &owner->banks[0].list : &owner->banks[1].list;` was 3 words out as the second
  statement of the function and exact once moved below the two `cutList`
  linkage stores; eleven spellings of the selection (an `if` either way round,
  both ternary polarities, an index expression, a byte-offset ternary and an
  explicit flag local) all give the same 3 or 4, so sweep the statement's
  position rather than its form.
- A varargs format string that retail keeps in a callee-saved register across
  the calls that compute the arguments is a named local assigned before them,
  and the arguments then have to be locals too. `effBTLFieldColorON` went from
  205 of 238 differing words to 41 with `format = D_00532FE8;` placed after the
  last VU0 macro and the six `sdfFloatToULong` results in their own `s32`
  locals; with the literal at the call site gcc builds the address in `$4`
  immediately before the `jal`. Anywhere from "after the last statement before
  the first argument" through "before the fourth argument" gives the same
  result; later than the fifth loses 40 words.
- The `(T *)((u8 *)(i * sizeof(T)) + (u32)base)` cast for retail's
  `addu rd, index, base` **can make things much worse**, so measure it rather
  than applying it on the operand order alone. It also stops gcc sharing the
  scaled index, so in a function that recomputes the row in two places
  (`effBTLFieldColorON` computes `id * 0x14` once for the colour reads and
  again for the printf arguments) the cast costs more than the operand order
  earns: 13 differing words became 221 with the cast on three accesses and 230
  with it on all of them. `effBTLCutDraw` shows the same thing for a loop row,
  88 of 263 words with plain subscripts against 149 of 260 with the cast.
- A dead `sw` to the frame after a colour-pack macro is a dead store to a
  stack aggregate's element, not a compiler artefact. `effBTLCutGetAllPartColor`
  ends `SDF_VU0_dds3ColorFtoI(color, 128.0f); work[0].word = color; return
  color;` with `work` a one-element 16-byte struct array, which is the same
  idiom `effColorBlend` in `effect/effMath` already carries.
- A copy of a struct's whole leading image, where retail emits a runtime
  8-byte-alignment dispatch over an `ld`/`sd` loop and an `ldl`/`ldr` loop plus
  an unconditional `ldl`/`ldr` tail, is one struct assignment of a 4-aligned
  type of exactly that size. `effBTLCreate`'s 0xC38-byte copy matched first try
  as `*(effBTLImage_t *)work = *(effBTLImage_t *)data;` with
  `typedef struct { s32 words[0xC38 / 4]; } effBTLImage_t;` declared in the
  unit, which avoids reshaping the shared `effBTL_t` header.
- Two pointer locals that walk in step through one loop, each initialised from
  a different global inside the same guard, couple their register assignment to
  their `lui` emission order and pull opposite ways. The saved registers follow
  statement order, so assigning the one retail gives the lower register first
  gets the registers right and the two `lui` the wrong way round; assigning the
  other first gets the `lui` order right and swaps the registers.
  `dds3BillboardManagerInit` sits two words from exact on that, and neither
  declaration order nor any loop-body or guard spelling separates them.
- Two pointers of the same type that a function needs again after its calls,
  where retail spills them to two adjacent 4-byte stack slots instead of using
  callee-saved registers, are **elements of a local array**, `T *batch[2]`, not
  two scalar locals. gcc gives the array a stack slot and reloads from it, and
  copies the two values into caller-saved registers for the loop, which is
  exactly retail's `sw`/`lw` pair plus two `daddu`. With two scalars it keeps
  them in `$17`/`$18` and the prologue grows. That took
  `dds3MultiBillboardPktLink` from 85 of 88 words differing to 4.
- Where retail folds a constant element offset into a symbol's `%lo` and then
  loads a member at a small displacement, `&table[n]` is not enough: both
  `&sdfGrOtTable[44]` and `sdfGrOtTable[44].regFunc` leave gcc holding the bare
  symbol and spending the `0x580` in the displacements, and a
  `(T *)((u8 *)table + 0x580)` pointer local is worse. `PS2_SYMBOL_AT(sdfGrOt_t,
  dds3MultiBillboardOt, sdfGrOtTable, 0x580)` and the member read through the
  alias is what gives retail's `addiu rd, $18, %lo(sym + 0x580)` with a shared
  `%hi` in a saved register. `dds3MultiBillboardLinkOt` went from 15 of 90
  words to exact on that one declaration.
- Two float products that share one converted operand take their registers
  from the source order, and the **second** one written is the one that reuses
  the shared operand's register. `evtDrawDeadMoonSprite` scales a width and a
  height by one integer scale factor; retail computes the height product into
  a fresh `$f15` and the width product back into `$f14`, the scale's own
  register, so the width statement is written second even though retail emits
  the height multiply first. Written the other way round the four coordinate
  conversions rotate as well, 22 of 112 words. An `f32` local for the scale and
  splitting each product from its divide both leave it unchanged.
- An integer parameter that retail converts to `f32` early and then multiplies
  by another converted integer needs its own `f32` local. Written `h * scale`
  with both `s32`, gcc emits an integer `mult` and one `cvt.s.w` of the
  product, which is two words over and shifts every saved register;
  `height = h;` ahead of `height * scale` gives retail's two `cvt.s.w` in the
  leading call's shadow and one `mul.s`. `evtDrawDeadMoonSprite` went from 56
  of 112 words and two words long to 22 at the exact size on that one local.
- A header word a function needs both before its calls and again after them
  belongs at its second use site as the field expression, not in a local. A
  local for it takes a callee-saved register and shifts every later allocation;
  `dds3BillAnimLoadP2A` wants `*(s32 *)resourceData` written again as `memcpy`'s
  length while the same read feeds the allocation size above it, and that moved
  10 of its 118 words.
- `SDF_VU0_LOADVECTOR_MEMOPS` and `SDF_VU0_STOREVECTOR_MEMOPS` take an
  **lvalue**, and an array's name is not one. `SDF_VU0_LOADVECTOR_MEMOPS(delta)`
  on `f32 delta[4]` makes cc1 write the decayed pointer to a fresh stack slot
  and then read *that slot* as the quadword, which also grows the frame by 16
  and moves every other local; `delta[0]` is the correct spelling and gives
  retail's bare `lqc2 vf10, 0($29)`. `dds3BillboardGetDirectionRotate` went from
  23 of 94 words to exact on that alone.
- The double-precision helpers are explicit calls in this engine's source, not
  compiler soft float. `sdfFloatToULong(x)` widens an `f32`, `dpdiv(a, b)` is
  the **comparison** (its result's sign is what `bgez` tests, despite the name),
  `dpsub(a, b)` subtracts, and `sdfDoubleToULong` returns the narrowed value in
  `$f0`, so declare it `f32`. A `1.0` double is retail's `ori rd, $0, 0xFFC0`
  plus `dsll32 rd, rd, 14`. `dds3BillboardTagLink` and `script/scrTraceCode`
  both use the quartet this way.


- A run of field copies between two objects of the same struct type gets its
  loads and stores regrouped by the scheduler, and the alias set is the lever,
  not the statement order. In `sdfModelCopyStat` retail copies `light`
  (a `sdf3DLightParallelPacket_t *`), `faceDivSoften`, `fog`
  (a `sdf3DFogPacket_t *`) and `faceDivBias` as two load/store pairs, while the
  plain field assignments hoist both pointer loads together and spend a second
  register: ee-gcc 2.96 gives each pointed-to type its own alias set, so a store
  to `light` does not block a load of `fog` and the scheduler interleaves them.
  Copying both through **one** set, `*(u32 *)&dst->light = *(u32 *)&src->light;`
  and the same for `fog`, restores retail's grouping and took the function from
  12 of 154 words to 4. `void **` on both reaches it too; `u32` on only one of
  them, or on all four fields including the two `f32`s, does not. The last four
  words were the fog and bias pair written above the `nodeList` reads rather
  than below them, and all 120 permutations of the eight statements in that
  tail were swept to find it: twelve of them sit at 2 words and exactly one is
  exact, so sweep rather than reason.
- The alias-set lever reaches a run of **zero stores**, not only a run of field
  copies, and the type to share is `u32` rather than a pointer type. The
  allocation initializer in `dds3AllocBasic` writes fourteen fields of a fresh
  `dds3Basic_t`; retail emits them in source order, while the plain field
  assignments come out rotated by whole pairs (gcc groups the stores by alias
  set and emits the last pair first), and no permutation of the six statements
  in the affected group reaches retail's order, all 1440 of them with the
  vtable read at either position. Writing just the two `void *` fields as
  `*(u32 *)&basic->staticdata = 0;` puts them in the same set as the two `u32`
  fields next to them and the natural source order is then exact. `void **` on
  the pointer fields instead is not equivalent and leaves six words out, so
  reach for `u32` first, and the `char *` and vtable fields between them do not
  need the cast at all.
- A table read whose index retail keeps in `$4` wants the read placed **after**
  the first store of the object it initialises, not before it.
  `dds3AllocBasic` reads `dds3BasicFuncTablePtr[type]` between
  `basic->type = type << 24;` and the rest of the initializer; read before the
  first store, the scaled index lands in `$6` and costs the `addu`'s operand
  register as well.
- A function whose early exits all return 0 does **not** want the
  `s32 ret = 0; ... return ret;` form just because retail materialises the zero
  in the prologue. `dds3AllocBasic` has `daddu $2, $0, $0` as its second
  instruction and two plain `if (...) return NULL;` guards reach it, because
  the guard's delay slot is already full; the result local instead runs eight
  bytes long and leaves the whole body shifted.
- An index into an array that is a **struct member** associates as
  `(base + index * scale) + offsetof`, which is what leaves retail's constant in
  the store displacement. A pointer local for the member
  (`f32 *w = (f32 *)(tag + 1);` then `w[k]`) associates the other way,
  `(base + offsetof) + index * scale`, and folds the constant into the base.
  `sdfModelLinkShapedDrawPacket`'s two blend weights need a unit-local
  `struct { sdfDmaTag_t dmatag; f32 weight[2]; u32 code0; u32 code1; }` with the
  running `sdfDmaTag_t *` cast to it at each access, and advancing that one
  pointer by `tag += 2` rather than keeping a second header pointer is what
  gives retail's `addiu rd, rd, 32` in place.

- A `jal fptodp` anywhere in the output means a float argument reaches a callee
  with no prototype in scope: the implicit declaration promotes it to double.
  This bites hard when a unit's helpers are **defined below** their callers, as
  splat's INCLUDE_ASM ordering usually leaves them. Seven functions in
  `event/evtPolygonMovie` were 45 or more words out until the six `evtHokan*`
  prototypes were added above the first caller, and all seven then came within
  a couple of words.
- A static out buffer used three times at different offsets does **not** want
  the pointer local the `fldElvGetFloorDigits` row above asks for. Where retail
  keeps the symbol in a saved register and adds the offsets with separate
  `addiu`s, writing `D_0056CFF0`, `&D_0056CFF0[4]` and `&D_0056CFF0[8]` at the
  three call sites is exact, while `f32 *out = D_0056CFF0;` plus `out`,
  `out + 4`, `out + 8` spends the *other* saved register on it and rotates
  `$20`/`$21`. `SLightFrameFunc` sat at 21 of 102 words on that one local, and
  no declaration or assignment order of `out` against the `hokan` local moved
  it. So sweep the local against the bare symbol rather than assuming either.
- Where a run of sibling callbacks reads a key frame and its successor, which
  arm of `now >= f->frame` is written first decides the whole body's branch
  direction. Retail falls through to the `now >= f->frame` arm, so that arm is
  the `if` and the `now < f->frame` arm is the `else`. Written the other way
  round all seven key-frame interpolators in `event/evtPolygonMovie`
  (`SFogFrameFunc`, `Blur2FrameFunc`, `MBlurFrameFunc`, `DBlurFrameFunc`,
  `RBlurFrameFunc`, `MFilterFrameFunc`, `FilterFrameFunc`) sat at 45 to 51 of
  65 to 74 words, and all seven went exact together when the arms were swapped.
- Two values computed by the same expression shape and merged at one exit take
  their registers from the order of the two assignments, and the computing arm
  and the default arm have to **agree**. `BlurFrameFunc`'s alpha and scale are
  assigned alpha-first in both the interpolating arm and the two default arms;
  alpha-first in one and scale-first in the other is 32 to 34 of 42 words out,
  and scale-first in both is 16 of 44. The test order in the following `||`
  chain and the order of the three stores have no effect at all.
- The `void`-to-`s32` return-type sibcall blocker has to be swept per function,
  not applied to a family. In `event/evtPolygonMovie` it is what
  `QuakeFrameFunc`, `BokasiFrameFunc`, `FadeFrameFunc`, `LightFrameFunc` and
  `BgmFrameFunc` need, while `SeFrameFunc`, `SkyFrameFunc`, `UnitFrameFunc` and
  `CameraFrameFunc` match as `void` and *break* when given the same `s32`
  return type, even though all nine sit in the same frame-callback table. For
  `evtRunUnitMode` the `s32` return type made it worse (26 of 52 words) and a
  dead store on the switch index in the last arm, `mode = 0;`, took it to
  exact.
- A run of stores inside a loop whose addresses are hoisted base pointers has
  **two** orders to get right, and they run opposite ways. The stores themselves
  follow the right-rotation rule (`func_001AA5A0` writes the word array, then
  `dds3PadButtonPrev`, then `dds3PadButtonState`, and retail emits Prev, State,
  word), while the order of the three hoisted `%hi`/`%lo` base pairs in the
  preheader follows the source order directly. Both fall out of one statement
  order, so sweep the permutations rather than reasoning about either rule alone:
  all six were measured and only one is exact.
- A single-statement guard arm belongs in the `else` even when the statement is a
  *store*, not just an index computation. `eff_1AA3B0`'s pad toggle written
  `if (a == 0 || b != 0) { a ^= 1; } else { b = 1; }` is 15 of 104 words out and
  one word long; inverted to `if (a != 0 && b == 0) { b = 1; } else { a ^= 1; }`
  it is exact. In the same function a cached `u8` local for the toggled global is
  far worse than reading the global at each of its three uses (85 of 100 words
  against 15 of 104), and the `lbu` retail uses for a symbol other callers read
  with `lb` comes from a `(u8)` cast at the use site, not from the declaration.
- A two-load chain `work->owner->field` whose base is also a later call's
  argument can leave the base in `$4` across the whole function, so that a path
  which bypasses the loop reaches the call with it already there and the `jal`'s
  delay slot stays a `nop`. Nothing in the source reaches this: an owner local
  (one assignment, reassigned after the loop, or mixed with the inline
  expression), every ordering of the prologue loads and every declaration order
  leave ee-gcc loading the base into `$2` and letting the delay-slot filler take
  the reload for both paths. Retail itself goes both ways on it inside one unit,
  so it is a gcse availability accident rather than a source fact: the three
  `effMaga*Draw` drafts in `effect/effMagatuhi` sit on it at 77 percent of their
  words while their sibling `effMagaSmokeConvDraw`, with the identical tail,
  matches with the load in the delay slot.
- A function whose body is more than half inline assembly is only refused by the
  production report when it carries no exemption comment. `check_unit.py`'s
  `inline_asm_share` skips any function whose definition is preceded within three
  lines by a `vu0 routine:` or `libvu0:` comment, which is why the VU0-dense
  setters in `effect/effMagatuhi` count. `func_001AA6D0` there is 223 VU0
  instructions of 394 and needs that comment to earn anything.

- A `(s16)` cast on a `u16` struct field compiles to a bare `lh`, not to `lhu`
  plus `sll`/`sra`: ee-gcc 2.96 folds the sign extension into the load.
  `GetFontHandleUnit` needed `(s16)chain->...cellHeight` where retail has
  `lh`, and the cast cost nothing. Reading it through an `s16 *` cast of the
  field's address gives the same thing, so prefer the plain cast.
- Where every retail read of a `u16` field is `lh`, retype the field `s16` in
  the header rather than casting at each use. A `> 0` guard is the cheap tell:
  on a `u16` it compiles to a plain nonzero test (`beq`/`bne`), on an `s16` to
  `blez`. `scrHeader_t.LocalIntCount` and `LocalFloatCount` were `u16` and cost
  `scrPrintProcedureStart` an `lhu` plus a `beq` per array; retyping both `s16`
  (same offset, same size) was the last change before it matched, and no other
  unit shifted.
- A `base[index].field` expression used several times keeps `index * sizeof` in
  a register and re-adds `base` at each use. Hoisting it into a pointer local
  (`row = &base[index];`) instead keeps the whole address in a callee-saved
  register and renumbers everything around it. `scrPrintProcedureStart` reads
  `proctable[procno]` four times and wanted the subscript spelled out each
  time; the `proc` local moved 123 of 154 words.
- One extra *statement* on a value raises its register-allocation priority,
  which is a cheaper lever than any declaration order. `GetFontHandleUnit`
  held four values across its loop and gcc gave them `$16` to `$19` one place
  rotated from retail; splitting `y += h * 8 + 0xA;` into
  `y += h * 8; y += 0xA;` added a reference to `y`, moved it from `$19` to
  `$16`, and took the function from 22 differing words to exact. Same
  instruction count either way. gcc's `allocno_compare` sorts on
  `floor_log2(n_refs) * freq / live_length`, so one more reference can cross a
  threshold.
- A named local for a *divisor* decides which register the constant takes, and
  its declaration position among the other locals decides the prologue order.
  `ConvertZenkakuNumber` divides by ten in a loop; `s32 radix = 10;` declared
  between the digit counter and the digit pointer is exact, where the literal
  leaves the 10 in `$4` and the loop bound's `slti` in `$6` (retail has them
  the other way round), and the same local declared first or last, or assigned
  as a statement, is 2 to 7 words out.
- One `u32` local for the switched value, **reused** across two successive
  `switch`es in the same function, is what gives both their registers.
  `CalcWindow` dispatches on `panel->flags & 0x300` and then on
  `panel->flags & 0x3000`; `mode = panel->flags & 0x300; switch (mode)` and
  `mode = panel->flags & 0x3000; switch (mode)` is exact, where the
  expressions written in the `switch` heads leave the value one register low
  and rotate the two panel pointers (16 of 81 words), and so do a `flags`
  local and a second re-read of it.
- A `switch` arm that writes one byte of a stack array in each of two paths
  wants a pointer local assigned *before* the `if`, with the stores written
  `*p = c;`. In `itfMesMngSetVar` the `p = text;` assignment above the
  `value >= 0x3E9` test is what puts `text`'s address in a saved register and
  turns both `text[0]` stores into `sb rd, 0(reg)`; with the assignment inside
  the `else` the arm addresses the frame directly and the function is 26 of 124
  words out, and the second store has to stay `text[1] = 0` (frame-addressed)
  rather than `p[1]`.
- A frame address that a VU0 store and a following call argument both use is one
  pseudo for both, and inside a loop that pseudo is loop invariant: gcc hoists
  the `addiu rd, $29, N` into the preheader, spends the last callee-saved
  register on it and spills whatever loses the race (in
  `effEveBEDCutDraw` a colour word, which also grew the frame by 16 and left it
  71 of 119 words out). Retail rematerialises the address at each use instead,
  from two short live ranges that never cross the call. Neither
  `SDF_VU0_STOREMATRIX` nor `_NOMEM` reaches that, and a
  `sdfMatrix4_t *mp = &marked;` local is folded straight back into the shared
  pseudo. `SDF_VU0_STOREMATRIX_RWADDR`, whose address operand is `"+r"` so cc1
  has to give the asm its own copy, is what reproduces it; the function went to 2
  of 119 words. `_MEMOPS` frees the register too but addresses the frame
  directly, so the four `sqc2` and everything after them shift by a word.
- A clamp whose result is stored straight into a vector field belongs in the
  field, not in an `f32` local: `if (half < radius) { offset.z = -radius; } else
  { offset.z = -half; }`. With a `depth` local gcc coalesces it with the compared
  value and negates in place (`neg.s $f2, $f2`) where retail negates into a fresh
  register, which also rotates the compared value one register down and moves the
  store past the call's argument setup. That was the last 12 words of
  `effEveBEDPartGetPositionPtr`, and no declaration order of its five `f32`
  locals moved any of them.
- A constant assigned once above an `if (a == K1 || a == K2)` whose body
  overwrites it comes out hoisted and shared. Retail's two copies, one between
  the two tests and one at the top of the body, come from writing the assignment
  in **both** arms: `if (dir == 8 || dir == 0xA) { y = -1.0f; if (...) { y =
  -radius; } } else { y = -1.0f; }`. `effEveBEDPartGetPositionPtr` went from 114
  of 138 words to 84 on that duplication alone, and the `&&` spelling of the
  inner pair changes nothing.
- Where retail materialises one zero with `mtc1 $0` and stores it with `swc1`
  twice, the source assigns an `f32` local as a *statement* positioned after the
  first non-zero store of the same vector, and stores the local into both fields.
  `offset.y = offset.x` instead gives `sw $0` plus a `lwc1` of the slot back, and
  the same function stores its other two zeros as plain `0.0f` (`sw $0`) in the
  other arm, so the two spellings really do coexist in one body. The four stores
  around it were swept: `offset.z`, the zero local, `offset.y`, `offset.x`,
  `position.y` is the only order within two words.
- A run of stores that each repeat the whole subscript needs the row to be one
  flat array, not an array of two-field structs. `effEVECombParSmokeDraw` writes
  eight texture coordinates per quad; spelled `batch.uv[n][k].u` and `.v` on an
  `UV_t uv[15][4]`, cc1 builds two base pseudos, `$29 + 0xC` for the `.u` stores
  and `$29 + 0xE` for the `.v` stores, shares neither, and materialises a whole
  address per store with a zero displacement. Declared `s16 uv[15][8]` and
  written `batch.uv[n][k]`, it builds the one `$29 + 0xC` base retail has, with
  the `0x120` to `0x12E` displacements and the seven `daddu rd, rs, $0` copies
  that give each `sh` its own register. Worth 30 words and both base registers.
  The index of such a run belongs in a local read once, `n = batch.count;`:
  written `batch.uv[batch.count][k]` the counter is re-read for each of the
  three scaled forms (`sll 2`, `sll 4`, `sll 5`) where retail reads it once and
  shifts that, and the increment after the run still re-reads the field. One
  local reused across two such batches is what gives retail's single register.
- Four corner offsets that each rotate a signed half extent come from **two**
  float locals reassigned between the pairs, not from the expressions written
  out: `x = cx - hw; y = cy - hh;` for the first corner, `x = cx + hw;` for the
  second, `y = cy + hh;` for the third and `x = cx - hw;` again for the fourth.
  `effEVECombParSmokeDraw` reuses one register for each of the pair and
  recomputes the repeated value, which is six words more than the expressions
  written at the eight stores; that one change also took the body to within six
  words of retail's size. `cx` and `cy` are both `0.0f` there and share one
  `mtc1 $0` pseudo, and assigning them as statements above the half extents is
  what puts that pseudo in `$f25` rather than behind the `0.0625f`.
- The read-write address operand generalises past the matrix store. Where
  several VU0 accesses to one frame vector sit inside a loop, the plain and
  `_NOMEM` forms share one address pseudo, which is loop invariant, gets
  hoisted into the preheader and spends a callee-saved register, so whatever
  loses the race moves out of its spill slot and the frame grows.
  `SDF_VU0_LOADVECTOR_RWADDR`, `SDF_VU0_STOREVECTOR_RWADDR` and
  `SDF_VU0_COPYVECTOR_RWSRC` take the address through a pointer local as an
  `"+r"` operand, so cc1 gives each asm its own copy and retail's separate
  `addiu rd, $29, N` per access come back. That took
  `effEVECombParSmokeDraw` from 524 of 706 words and a 16-byte larger frame to
  the exact frame size, every saved register and all thirteen spill slots.
- Two pointer pairs retail holds in adjacent four-byte slots are **not**
  necessarily a local array. gcc rounds every stack aggregate up to 16 bytes, so
  `sdfDrawTag_t *tag[2]` followed by `sdfTexHandle_t *tex[2]` lands at `0x870`
  and `0x880` with an eight-byte hole and a 16-byte larger frame, where retail
  packs all four at `0x870` to `0x87C` as ordinary reload spill slots. Read the
  gap between the slots before reaching for the `dds3MultiBillboardPktLink`
  array row above: contiguous four-byte slots mean plain locals that spill.
- Two locals that both spill take their stack slots in **declaration** order,
  and their load order in the prologue follows the statement order separately.
  In `eff_1AEC58` retail gives `0x30` to the `u8 repeat` and `0x34` to the `u32`
  particle count; declaring `repeat` first is exact and declaring it last swaps
  both slots and their four reloads (5 of 262 words). The statement order is a
  second, independent lever on the same pair: reading the particle pointer before
  the count took it from 9 of 262 to 5, and all six orders of the three leading
  reads differ. An `s32` for the `u8` field undoes the slot fix.
- The whole spill-slot sequence is readable, not just a pair, and it gives the
  declaration list back. gcc assigns frame slots in this order: addressed
  locals (an array) at `expand_decl` time, then every spilled pseudo in pseudo
  number order, which is the parameters first, then the declared scalars in
  declaration order, then the temps the statements create, then the pseudos
  the loop optimizer creates. So retail's slots name the locals in order.
  `sdfModelLinkOt`'s eight spilled locals read off its frame as `i`,
  `rootColor`, `drawAttr`, `attr`, `light`, `drew`, the shifted soften bias
  and the 64-bit draw-parameter word, with the colour sign flag and the
  hoisted `frame * 4` as the two trailing temps; declaring them in that order
  took it from 134 to 112 differing words and fixed the frame size, and the
  same list explains `sdfModelTempPacketLinkOt`'s frame, which spills three
  more of the same variables.
- A local assigned **inside** a loop and then hoisted out loses its place in
  that sequence: gcc's loop invariant motion gives the moved value a fresh
  high-numbered pseudo, so it lands after the loop-created temps and the frame
  grows. Assigning it before the loop keeps the declared slot but leaves the
  computation before the loop guard instead of in the preheader. In
  `sdfModelLinkOt` the two halves could not be had together for
  `corePktSoftenBias << 16`.
- `(flags & bit) != 0` stored as a flag folds to `srl`/`andi`. Where retail
  has `andi` then `sltu rd, $0, rs`, assign the masked value to its own local
  and test *that* local: gcc's `fold_single_bit_test` only fires when the
  comparison's operand is the `BIT_AND_EXPR` itself, so one extra local
  blocks it.
- `x < 0` stored as a 0/1 flag is **always** `srl rd, x, 31` from C. gcc
  2.96's `do_store_flag` passes `normalizep = 1` to `emit_store_flag`, which
  takes the sign-bit shortcut before trying `slt`. Seventeen spellings and
  four flag types were measured for `sdfModelLinkOt` and every one gives the
  shift. `slti rd, x, 0` in this tree only ever comes out of
  `emit_conditional_move`: `sdfChipFreeDirect`'s signed divide by 0x1000
  emits `slti`/`movn` for the rounding adjust. Treat a bare `slti r, x, 0`
  whose result is then stored as an unsolved shape, not a spelling to sweep.
- A word load whose only use is `>> 16` is narrowed to `lhu` at +2. Eight
  spellings were ruled out on `sdfModelTempPacketLinkOt`'s shape-header read,
  including a `u32` struct member, `*cursor++ >> 16` and `/ 0x10000`. Only a
  second live use of the low half keeps the `lw`/`srl` pair;
  `(w & 0xFFFF0000) >> 16` keeps it in a small function but is narrowed again
  once the value feeds an index, so the pattern in
  `sdfModelSetNormalDrawPacket` works because `qwc` is used too, not because
  of the cast.
- The three `nop`s between the `sw $0` and the `bne` of retail's four-slot
  draw-tag clear loops are ee-as's short-loop padding, not source. A plain
  `for (j = 0; j != 4; j++) tags[j] = NULL;` reproduces them word for word.

- A packet builder whose stores all go to one array element wants the element
  indexed at every store, `packet_array[frame].field = v;`, not a
  `T *p = &packet_array[frame];` local. The pointer form folds the whole body
  into displacements off one base; retail computes `(index * size + offset) +
  base` per field and spills the ones it cannot keep, which is 65 words more
  in `dds3DrawFade`. Within an A+D register entry write `.reg` before `.data`:
  that is the order the scheduler emits, and `.data` first costs a word per
  pair in both `dds3DrawFade` and `dds3InitZClearDrawTag`.
- Where retail multiplies a loop index by a struct size with an odd factor and
  a shift (`sll idx, 5` with the induction variable stepping 0xB for a 0x160
  row), index the element inside the inner loop, `a[outer][inner]`. Hoisting a
  row pointer to the outer loop (`p = a[outer];` plus `p++`) makes gcc build a
  single induction variable stepping the whole row size and costs two words.
  `dds3InitZClearDrawTag` and `dds3DrawFade` both show the factored form.
- A loop that reads three arrays, where retail walks two by pointer and
  subscripts the third by the counter, comes from subscripting *all three* by
  the counter. gcc then reduces two of them to pointers on its own and leaves
  the third indexed, which is what keeps the counter an up-counter with an
  `slti` bound. Incrementing two pointers in the source instead lets gcc
  reduce the third as well, the counter goes dead, and the loop becomes an
  `addiu -1` downcounter: `dds3RunQuake` was 126 words that way against
  retail's 129, and 13 of 129 with every subscript written out.
- An eight-byte `extern` struct copied by value is addressed `gp`-relative,
  with the `ldl`/`ldr` pair reading off `$28`. Where retail builds the address
  in a register first, declare the symbol as an incomplete array and copy its
  first element, `extern T sym[]; T local = sym[0];`. The unknown extent keeps
  it out of small data. `dds3RunQuake`'s pair of axis gains needs this.
- Two references to one symbol with different addends that retail materialises
  as two independent `lui`/`addiu` pairs are two symbols in the source. Give
  the second one a `PS2_SYMBOL_AT` name and pass that: `dds3InitDrawEnvSub`
  hands `sdf3DMakeLightParallelPacket` the light record and its ambient term,
  and sharing the `%hi` through `&dds3LightParam.ambient` runs a word short per
  call.
- Row 78's `ldl`/`ldr` plus 8-byte-alignment dispatch is reproduced exactly by
  `extern const T g; T local = g;` on a struct of plain `char` arrays, as long as
  the size is a multiple of 8: `DATA_ALIGNMENT` lifts a global aggregate to four
  bytes, so cc1 knows it can use doublewords but still has to test the source
  address's low three bits at run time. `dds3ObjectBaseMakeUnit` has fourteen
  copies of a 360-byte `{char name[20]; char parts[17][20];}` and every one
  matched first try. The locals take 16-byte-aligned frame slots in declaration
  order, so a 0x168 type gets a 0x170 stride and the declaration order is
  readable straight off the stack offsets. Declaration initializers against plain
  assignments decide only where the copies land relative to the function's
  leading calls; retail has them after `mdlLoad`, which is the assignment form.

- **Every repeated scale factor in a run of coordinate expressions is a reused
  `f32` local, not a literal at the use site**, and this is the largest single
  lever measured on a long float body. `effPCPBezierBillDraw` builds 35 bezier
  control points as `spin[k] * K1 + axis[k] * K2 + center[k]`; with the
  constants written as literals each point's factor takes a fresh float
  register, every temp in all five `switch` arms rotates, and the function sits
  at 936 of 1497 words. With `scale`, `bend` and `reach` declared once and
  **reassigned before each point**, retail's single `$f7`, `$f5` and `$f8` fall
  out and it goes to 1276. The tell for the local rather than the literal is a
  bare `mov.s $f8, $f20` where the factor is the unscaled value: that copy comes
  from `reach = radius;` and not from `axis[k] * radius` written out. Read which
  float register retail reuses across a repetitive block before writing one of
  these out.
- The operand order of a commutative `mul.s` follows the **source** order, so
  `reach * axis[k]` and `axis[k] * reach` are one word apart at every use. In
  `effPCPBezierBillDraw` that was 90 words over 95 multiplies. The order is not
  uniform within a function: the same expression wants the variable first where
  its load is scheduled before the constant's `lui`/`mtc1` and the constant
  first where a `SDF_VU0_APPLYMATRIX_XYZ` between two points puts the constant
  in a register ahead of the load, so read each block rather than applying one
  rule.
- Where several VU0 accesses to one frame vector sit in a loop *or* in a
  straight run punctuated by calls, the plain `SDF_VU0_STOREVECTOR` is what
  costs the frame. `effPCPBezierBillDraw` keeps three stack quadwords and one
  spin vector this way: with the plain form the shared `addiu rd, $29, N`
  becomes a loop invariant in a callee-saved register, one more pointer spills,
  a third quadword spill slot appears at `0x170` and the frame grows from
  `0x1F0` to `0x200`. `_NOMEM` on those four stores was worth 46 words, the
  spill slot and the whole frame, **even though the C reads the stored vectors
  back by field** immediately afterwards, against the caution on the macro.
- A second read of a field that a store to an addressable local sits between is
  a reload, and the cheap fix is to read the local back instead of the field.
  `effPCPBezierBillDraw` writes `place.payload[0..2]` from `slot->position[]`
  and then needs two of those components again for an atan2; written
  `center[0] - slot->position[0]` the store to `place` kills the load and gcc
  reloads, 15 words out, while `center[0] - place.payload[0]` is
  store-to-load forwarded into exactly the register retail uses. Reading the
  three components into `f32` locals first is the obvious alternative and is
  worse, because all three are then live at once where retail reuses one
  register for the second and the third.
- A colour word built by a float-to-unsigned conversion and stored through a
  pointer field wants **no intermediate local at all** when retail loads that
  pointer in the block head.
  `slot->fragment->color = ((u32)(fade * 128.0f) << 24) | 0x808080;` as one
  statement lets gcc hoist the `lw` of `slot->fragment` above the unrelated
  `slot->fadeScale` store into the entry block; with a `u32 color` local the
  load cannot move and the whole 19-word block shifts by one instruction.
  That was 18 words in `effPCPBezierBillDraw`, and a `u32` local for just the
  conversion (the `dds3EffectRippleBlurDraw` row above) does not reach it, so
  the two shapes are opposites and which one applies depends on whether retail
  loads the pointer early.
- A float local reused for two values with **disjoint** live ranges is how
  retail gets the long-lived one into the lowest callee-saved register, and
  splitting it is what fixes a rotation of three. In `effPCPBezierBillDraw` the
  `-height * rand()` term dies at its one use and the `SDF_VU0_GETLENGTH`
  result lives across the whole `switch`; two locals give the short one `$f20`
  on priority and let the long one reuse `$f20` after it dies, which is
  retail's assignment, while one local for both leaves `$f20`, `$f21` and
  `$f22` rotated by one. This is the inverse of the one-local rows above: read
  whether the two values overlap before choosing.
- A `.lit4` constant whose decimal is obvious needs no reverse engineering:
  write the natural literal and let ee-gcc truncate it. `0.15f` in the source
  gives retail's `1.499999911e-01` word, and the same holds for `0.3f`,
  `0.45f`, `0.6f`, `0.14f` through `0.84f` and `0.333333333f`. The float-literal
  section's warning is about transcribing a *rounded value read out of a
  disassembly*, not about ordinary decimals.

## Two local vectors: declaration order fixes the stack offsets

Local aggregates are laid out in declaration order from offset 0, each rounded
to 16 bytes, so which of two `f32 v[4]` locals lands at `0($29)` decides
whether a VU0 macro taking the address as a register operand gets a bare
`$29` or retail's `addiu $2, $29, 0x10`. One wrong order shifts the whole body
by a word: `dds3Particle_SparkBuffInit` sat at 55 matching words and 2008
bytes until `position[4]` was declared before `direction[4]`, and went to 335
matching words at the exact size on that line alone. Read the offsets in the
retail `lqc2`/`sqc2` address setups before writing the declarations.

## A value held in a callee-saved float register shares a local with the tail

A local that is set and used inside one arm gets a call-clobbered register,
because its allocno crosses no call; one given its own name per arm is
block-local and lands in `$f0`. Where retail holds such a value in `$f20` or
another callee-saved register, the source is reusing **one** local that is
also live across a call somewhere else in the function. In both Spark
`BuffInit` functions the per-arm jitter scale is the same local as the tail's
`head.speedJitter`/`head.spinJitter`, and only that sharing puts it in `$f20`;
its own local gives `$f5`, three per-arm locals give `$f0`, and either one
costs the three `mul.s` registers and the schedule around them.

## Function-local constant arrays

A block-scope initializer such as `s32 list[3] = {1, 11, 14};` goes to
`.rodata` at the function's position in the C file, not where splat put the
`INCLUDE_RODATA` line for it. Move the `INCLUDE_RODATA` lines for every
rodata object that precedes it above the function, or the relink reorders the
pool. `check_unit.py` masks local rodata relocations and still says the
function matches; only the `ninja` checksum catches this.

## A block-local whose live range reaches the end of the block costs a register

Local allocation is per basic block and runs before global allocation, so one
extra value live across a block pushes every function-wide pseudo up one hard
register. `evtPictureProcess1` interpolates a position with
`s32 cnt = work->pos_cnt - 1;` and stores it back to `work->pos_cnt` as the
last statement of the block: that keeps seven integer temps live at once,
local allocation reaches `$8`, and the `work` pointer lands in retail's `$9`
with the texture behind it in `$10`. Writing `work->pos_cnt--;` instead, or
storing `cnt` back first or in the middle, leaves six temps live, gives `work`
`$8`, and shifts the whole function's register names by one, which is 119 of
140 differing words against 103 for the long-lived spelling. Where a draft
differs from retail by a uniform `+1` or `-1` on the register numbers, look for
a block temp that retail keeps alive and the draft does not, rather than at the
function's own locals.

## Arithmetic

ee-gcc 2.96 does not turn a division by a constant into a multiply: it loads
the divisor into a register and emits `div` plus the divide-by-zero trap, the
`beql`/`break 0, 7` pair. It does synthesise constant multiplies out of
shifts and adds, so `n * 100 / 0xB8` is a shift chain followed by a real
`div`.

## Float literals

ee-gcc truncates a decimal literal toward zero, so `-946.42f` lands one ULP
away from retail and `-946.41993f` is exact. Derive the shortest decimal that
round-trips to the retail word rather than transcribing the rounded value.

`check_unit.py` says so outright when it hits one: a differing word whose
retail side is a `gp`-relative `lwc1` is reported as `D_00XXXXXX is a .lit4
pool constant: write the float literal`. Take it at its word and inline the
value rather than declaring the symbol `extern f32`; `dds3RunQuake`'s
`D_0054C188` is `0.01f`.

To tell whether a `gp`-addressed `D_00XXXXXX` is a pool entry, subtract the
flat-image delta `0xff000` from its vram and look the file offset up against
the `.lit4` rows in `config/smt3/SLUS_209.11.yaml`, whose addresses are file
offsets. `D_0054C2B4` and `D_0054C2B8`, read by `fldCamCalcSlopeOffset`, land
at `0x44D2B4` and `0x44D2B8` inside `field/fldDraw`'s pool and hold `0.001f`
and `-0.001f`. The pool is not deduplicated: the same constant appears at
several offsets, so only the order of use matters.

A float literal passed to an asm `"r"` operand folds to one `lui`, because
cc1's `movsf` can load a constant straight into a GPR and `regclass` prefers
`GENERAL_REGS` when the operand's only other reference is that `"r"` use.
Where retail instead shows `lui` + `mtc1` + `mfc1`, the constant is sitting in
an FP register and cc1 is moving it out, which also means those three
instructions are ordinary insns the scheduler may interleave with the
surrounding code. `PS2_FLOAT_BITS(out, f)` emits the same three but as a
volatile asm, so they become a scheduling barrier: use it only where retail
keeps them together. `dds3SceneLightUpdateFirst` needs the schedulable form
four times and has not been written yet; adding a dummy `"f"` input beside the
`"r"` one does flip the class, but cc1 then commons the two constant loads in
the same block.

`ee-as` owns the `.lit4` pool, not `cc1`. `configure.py` rewrites every
`lwc1 %gp_rel(D_...)` in the split asm into `li.s`, so literal values are
readable in `build/eeasm/` and the pool is rebuilt per object in emission
order. The order in which the C uses a constant has to match retail.

## Stores

Adjacent constant stores come out in reverse source order, so swapping them
is the first thing to try on a two-word diff. For a longer run the rule is a
right rotation by one, and it holds for a run of twelve: `frInitFontHandle`
writes `glyph->chainHead = glyph;` last and retail emits that store second,
so take retail's emitted list and left-rotate it by one to get the source
order. Mixed `sb` and `sw` stores rotate together. A short-circuit `||` with one
store beats an if/else chain that stores in each arm.

- Where retail's last store before a `jal` is a constant store rather than the
  argument setup, hoist the argument into a local above the stores. The store
  then falls into the call's delay slot. `nbSetBadstatMotion2` went from six
  differing words to exact on `party = &data->party[form->partyindex];` moved
  above three `sw $0` stores, and no order of the stores alone reached it.

Where a handful of constant stores differ only in order, permute them
mechanically rather than reasoning about it. A loop driving
`check_unit.py --func <name>` costs about a second per permutation; two
functions turned on the first of 24 permutations and on a rotation of three.

The sweep is worth reaching for past pure constant stores. `dds3ObjectBaseMakeUnit`'s
last eight differing words were a five-statement tail (two `sw $0` constants, a
field store, a pointer load and a float field store) whose order no reading of the
schedule predicted. All 120 permutations of the five run in five seconds and the
second one was exact: the float store `basic->poseset->r = obj->objectRadius;` has
to precede `control->modelHandle = obj;`.

- Caching a field in a local and testing the local, where retail re-reads the
  field at each test, decides which caller-saved register holds the mask
  result and therefore where the scheduler may put it.
  `dds3AdminiStartSequence` held `flag = work->flag;` and tested `flag &
  0x10000` and `flag & 8` against it; reading `work->flag` at each of the
  three tests instead took it from 5 differing words to exact, all five being
  the reload landing in `$5` rather than `$4`. `dds3AdminiNeoProcess` has the
  same shape twice and went from 20 words to 10 once both `flag & 2` tests
  read the field. Neither a second local, nor an `(x & C) != 0` spelling, nor
  a `u32`/`s32` change on the neighbours moves it. The caching local can still
  be needed for the *first* read: dropping it there costs 50 words.
- Where a block needs three constants or addresses live at once, the order of
  two adjacent *byte* stores decides whether gcc reuses one register for two
  of them. `dds3AdminiNeoProcess` writes `waittimer`, `nextsequence` and a
  `flag &= ~4` whose mask needs a register (`andi` cannot hold `~4`); with
  `waittimer = 2` written before `nextsequence` the constant dies early, gcc
  reuses `$4` for the mask and again for the `reqdata` load, and the function
  is 10 words out. Swapping the two stores keeps all three live, which is
  retail's `$4`, `$5`, `$6`, and it is exact. A `void *reqdata` local for the
  value instead loses two instructions, because gcc then loads straight into
  `$4` and drops retail's `daddu $4, $6, $0`.
- A bounded string copy that retail writes as one `lbu`, a `sll rd, rs, 24`,
  the branch and the store in its delay slot is
  `while ((dst[i] = src[i]) != '\0')`, the assignment as the loop condition.
  The `sll 24` is gcc normalising a `char` it loaded with `lbu` because the
  one value serves both the store and the test. A `char c = src[i];` local
  gives `lb` and a bare `beqz`, two instructions short; a `u8 c` local gives
  `lbu` and a bare `beqz`; two subscript reads give two loads, since each
  `char *` may alias the other. `fclEncycFindFreeSlot` needed the condition
  form for its 31-character copy, with the truncation arm written
  `if (i == 31) { dst[i] = '\0'; break; }` at the top of the body so gcc
  rotates the `i == 31` test to the loop header.
- In a function whose body is a run of stores into one table row, the store
  order decides which address-base temp gcc creates first, and that decides
  which register holds `%lo(symbol)` against the scaled index, which rotates
  every later temp and therefore most of the function. **Ascending field
  offset is the order to try first**: `fcl_103420` in
  `kernel/dds3AdminiProcess` writes one argument and zeroes six fields, and
  went from 23 of 37 differing words written in the obvious order (`kind`
  then the argument store) to 7 on ascending offset; its two siblings went
  from 29 to 11 the same way. This is the same lever as the Stores section's
  rotation rule but on the address temps rather than the store order itself,
  so it is worth sweeping even when the stores are not all constants.
- An `&&` of two tests on the same field compiles to the same two branches as
  nested `if`s, so the branch shape is no evidence either way, but it can
  decide the whole function's register map. `fclEncycFreeEntries` writes
  `kind > 0 && kind < 3`; both spellings give retail's `blezl` then
  `slti`/`beqz`, yet with the `&&` the register holding `%lo(fclEncycTable)`
  and the one holding `slot * 0x44` come out exchanged and the swap propagates
  through every address temp and every store, 54 of 122 words against 2 for
  the nested form. Reading the field into a local first changes nothing. So
  when a function is far out but every instruction is present in retail's
  order, try nesting each `&&` before anything else.

## Duplicated tails and post-reload cross-jumping

gcc 2.96 runs cross-jumping after reload, in `jump2`, so it compares
instructions by their **allocated hard registers**. Two consequences, both
measured on `dds3ParticleColorAnim`:

- Where retail computes a value in both arms of an `if` but converts or
  stores it once at the join, the source most likely duplicates the whole
  tail in both arms and gcc merged the common suffix. Writing one shared tail
  after the `if` instead loses the duplicated constant materialization that
  the merge leaves behind: a `lui`/`ori` pair scheduled earlier in each arm
  than the merged suffix stays in both arms, which is exactly what retail
  shows. The draft went from 45 of 83 words to 64 on that change alone.
- The merge stops as soon as one register differs, which is why retail can
  keep two of three float-to-int conversions inside the arms and share only
  the third. Do not read that asymmetry as asymmetric source.

Beware the inverse trap: making the two arms' last instructions *identical*
invites a merge retail does not have. Two arms that each end
`alpha = product; alpha &= 0xFF000000;` get the `and` cross-jumped into the
join and come out two words short, where the same two statements with the
mask folded into the first expression in one arm only do not.

Also: `objdump` collapses a run of identical instructions to `...`, so a pair
of `nop`s that ee-gcc emitted before a `div.s` can look absent when you
compare a disassembly by hand. Pass `-z`.

## Callee visibility: the cause of most one-word branch-likely misses

This is the single most productive mechanism found so far, and it is one cheap
test rather than a search. Whether ee-gcc emits `beq` or `beql`, `bne` or
`bnel`, `bgez` or `bgezl` for a given test is decided by whether the function
that test's arm **calls** is visible in the unit as a C definition. Nothing
about the branch's own source shape reaches it.

It runs in both directions, so when a function is one or two words out and the
difference is an annul bit:

1. Find the callee in the arm whose branch differs.
2. If it is still `INCLUDE_ASM`, the caller will probably close by itself once
   the callee is C. Do not permute the caller.
3. If it is already a C definition, test the reverse: in a scratch copy, rename
   that definition so only the file's `extern` remains, and recheck the caller.

In `newbattle/nbMakePacket` one callee, `nbFreeEffectSlot`, was costing four
functions at 1, 1, 1 and 4 differing words. Reducing it to the `extern` the
file already carried made all four exact. It is byte-exact on its own, so it is
parked behind a `NON_MATCHING` fallback: a matching function can cost more than
it earns, and nineteen more of its callers are still in assembly.

Ruled out as causes, measured: every spelling of the caller's own guard, the
callee's declaration style including K&R and no prototype at all, and cc1 heap
shift, since twenty unused `extern`s above the callee change nothing. It is the
definition's identity alone.

A side effect worth noticing: the four callers that discriminate all sit above
that callee's address, which is evidence for a retail translation-unit boundary
between them. A callee-visibility result is also a boundary hint.

Two neighbours that both look responsible are not interchangeable, and the
fallback is not a weaker form of renaming. In `effect/billManager`,
`dds3Billboard_P2ADraw` is one annul word out, and parking
`dds3Billboard_P2AAnimDraw` behind the fallback closes it exactly as renaming
the definition does. Parking `dds3Billboard_P2ASetNumber` instead leaves it at
one word, although an earlier reading reported either neighbour would do and
that the fallback did not reach the effect at all. Both halves of that were
measured wrong. Identify the neighbour by testing one at a time, and use the
fallback, which is reversible and keeps the draft.

Confirmed again in `event/evtPolygonMovie`, and this time on a callee that does
*not* match: `evtHokanFilter` is 71 of 72 words with a single `bnel` for
retail's `bne`, and compiling the parked `evtHokanColor` draft unguarded makes
it byte-exact while `evtHokanColor` itself stays 9 words out. So the mechanism
also tells you which pair to attack: a one-word annul miss next to a parked
draft is a reason to spend the budget on the draft rather than on the caller.

## Two global addresses in callee-saved registers can come out swapped

A function that calls through two different global state pointers often keeps
both addresses in callee-saved registers, rematerializing the `lui %hi` in
each block and spending an `addiu %lo` per call. Which symbol gets which
register is gcc's global-allocation priority tie-break
(`floor_log2(n_refs) * freq / live_length`, then allocno number, which is
pseudo creation order), and no source change found so far reaches it.

Both large functions in `effect/parManager` stop on exactly this: with
everything else identical, `dds3Particle_SparkBuffInit` is 490 of 512 words
and `dds3Particle_SparkOffsetBuffInit` 462 of 484, at their exact sizes, with
retail holding `dds3EffectBase` in `$21` and `ParRand` in `$20` and the
drafts the other way round. The two pseudos have the same reference counts and
the same first-reference order in both compiles, so the difference is below
the source. Measured and rejected: `&ParRand` for `ParRand`; reordering the
two tail statements that are each symbol's only tail use; all five non-retail
orderings of the `switch` cases that hold the other uses; three forms of
sharing or splitting the float local that sits in the same arms.

Treat a diff that is only this swap as finished work: write the draft, say so,
and spend the budget elsewhere. It is the same class of residual as the spill
slot rotation in `fldSkyDrawFilter`.

## A caller can need its callee's definition, not just a prototype

Measured in `mc/slMain`: `slDrawInfo` at 183 of 189 words and `slDrawError`
at 293 of 299 both become exact the moment `slDrawWindow` is a C definition
above them, and while it is still `INCLUDE_ASM` they each emit six `bnel`
where retail has `bne`, on exactly the tests whose arms call it. Cracking one
callee therefore banked 801 words across three functions.

So when a caller sits within a couple of branch-annul words of exact and
calls something still in assembly, decompile the callee first rather than
permuting the caller.

It is not universal. An agent on `field/fldDraw` tested it directly, reverting
a converted callee to `INCLUDE_ASM` while changing nothing else, and its
caller still compiled exact; no near miss in that unit emitted a `bnel` for a
`bne` that a C callee fixed. Try it, but measure rather than assume.

It also runs the other way, which is worth knowing before you convert
something. `nbMaeCheckDone` in `newbattle/nbMakePacket` is **byte-exact on its
own** and is parked behind a `NON_MATCHING` fallback anyway, because compiling
it turns a neighbouring function's `bnel` into a `bne`. A matching function
can cost more than it earns, so check the whole unit's result after adding
one, not just the function you wrote. This is the same mechanism as the CONTEXT row below,
seen from the other side: what the compiler can see of the rest of the unit
changes the code it emits for one function.

## Check the whole unit, not the function you edited

ee-gcc's output for one function can change with code earlier in the file, so
a function that matched can stop matching because of an unrelated edit.
`tools/check_unit.py` reports that as `CONTEXT <name>: compiles differently
inside the full unit`. Always read the whole unit's result, not the line for
the function you were working on.

## When to stop, and keeping the draft

A differing function must not stay in the build: a near miss earns no credit
and hides that the unit is incomplete. The draft itself is worth keeping, so
`python tools/revert_differing.py <unit.c>` wraps every function the unit
check will not credit as

    #ifdef NON_MATCHING
    <the draft>
    #else
    INCLUDE_ASM(const s32, "unit/path", name);
    #endif

The normal build never defines `NON_MATCHING`, so it takes the asm and stays
byte-identical, and the function still counts as unmatched everywhere. Run
`configure.py --force-split` afterwards, since the set of `INCLUDE_ASM` lines
has changed, and `python3 tools/check_nonmatching.py` to confirm the draft
still compiles.

Label the draft with one line saying how close it is and what differs, which
`revert_differing.py` writes from check_unit's measurement:

    /* NON_MATCHING: 682 of 694 words, exact size; three reload spill slots rotated. */

Then write the draft up in [Non-matching drafts](nonmatching.md): what the
remaining difference actually is, and which source shapes you ruled out. The
one-line label tells the next person whether to look; that page tells them
where to start and what not to retry. `python3 tools/check_nonmatching.py`
compiles every draft and refuses one with no entry, so this is not optional.
`--discard` deletes a draft not worth keeping.

A draft is never evidence of anything. It is not compiled by the build, so
`tools/check_nonmatching.py` is what stops it rotting when a struct or a
signature changes under it.

A draft and its `#else` fallback must stay in the same file when a container
is split, and the fallback may carry an `INCLUDE_RODATA` line before its
`INCLUDE_ASM`. `tools/split_unit.py` does not know about either: it has
copied a draft region into every part that inherited any of the container's
text, parked a part's extern declarations inside the draft branch so the
region could not simply be deleted, left an orphan `#else`, and emitted empty
`#ifdef NON_MATCHING` pairs. After a split, run the draft checker before
believing the result.

These have been ruled out repeatedly and are not worth a second attempt:

- Pure instruction scheduling, where the same instructions appear in a
  different order with no semantic difference to steer.
- Register allocation that needs one register fewer than the C uses, shifting
  every later allocation. Change the source shape or stop.
- A set of reload spill slots that appear in a rotated order. The slot offsets
  follow pseudo-register numbers, so the fix would have to change which pass
  creates a value rather than its source shape. `fldSkyDrawFilter` sits at 682
  of 694 words on a three-slot rotation that twenty-odd source shapes, nine
  declaration orders and every pointer form tried leave untouched.
- An epilogue duplicated into a branch-likely delay slot is **no longer in
  this list**: the cause is known and it is usually fixable. See the section
  on callee visibility below. Note the narrow scope: ee-gcc 2.96 emits `bnel` from
  ordinary C readily, 42 times in `field/fldDraw` and 126 in
  `newbattle/nbMakePacket`, so a `bnel` in the retail code is not a reason to
  skip a function. Only this one use of it has resisted.
- A frameless conditional sibling call. ee-gcc always saves `$ra` for a
  sibcall.
- Zero-extending a byte with a `sll 24`, `srl 24` pair. ee-gcc emits
  `andi 0xff` for every source shape tried.
- `xori reg, reg, 0` in front of a `movn`. `frGetXShiftLev` in
  `interface/frFont` has one, on `(fontIndex < 2)` feeding
  `movn $18, $6, $3`, so that function is capped by this row and not worth a
  long attack. No `!= 0` shape (plain `if`,
  ternary, `> 0`, `>= 1`, explicit `^ 0`, a `continue`, a local of either
  width) adds it; every one of them emits the `movn` alone.
  `sdfModelLinkDrawPacket` in `sdf/sdfModel` is a second instance, on
  `if (aMode != 0) { aOtIndex = packed >> 16; }`, and twenty-three spellings
  were compiled and dumped in isolation against it: both ternary polarities,
  an `if`/`else`, `== 0` with the arms swapped, `!!`, `-0`, `> 0` unsigned, a
  one-case `switch`, a 0/1 condition local, three shapes of a zero local that
  constant-folds, and `u8`/`u16`/`s8`/`s16`/`s64`/`u64`/pointer condition
  types. The only thing that produces an `xori` at all is comparing against a
  **non-zero** constant, so the zero case is not reachable. The missing
  reference also costs the allocation: retail gives `aMode` a saved register
  ahead of the next parameter, and without the `xori` the whole body's
  registers rotate.
- A `jal` plus a shared epilogue in a `switch` default arm, where `return`
  against `break`, a non-void or struct return type on the callee, and a dead
  assignment of the call's *result* all leave the arm a sibcall. A dead store
  to another variable does work, so try that first: see the tail-call row
  above.

A function whose body is mostly inline assembly is refused by the production
report anyway, at 8 instructions and more than half the body, so writing one
to force a match earns nothing. See [Reading progress](progress.md).

## Sweep permutations in the real file, not an isolated copy

`check_unit.py --source <file>` compiles an experiment as the unit, which is
the right tool for a single question but the wrong one for a sweep. One
agent's natural store order was **exact in the real file and ten words out in
a `--source` copy** that happened to lack two neighbouring C functions, and a
720-permutation sweep of the isolated form found nothing better than seven.
Whatever the sweep is over, run the winner in the real unit before believing
it, and prefer sweeping in place.

## Do not run the rodata and sdata placers standalone

`tools/include_rodata.py` and `tools/include_sdata.py` rewrite every source
file they can reach, not just the unit you are working on: one standalone run
touched 21 unrelated files. `configure.py` runs them as part of its normal
pass and leaves the tree clean, so `configure.py --force-split` is the only
thing to call.

A string that splat folded into a now-decompiled function's asm file needs no
manual `INCLUDE_RODATA` either: `configure.py` places the line and emits the
split file, but `build/eeasm/` only catches up after a `ninja`, so
`check_unit.py` reports `NOASM` until then.

## A second label in the split assembly: which kind matters

A `.s` file holding a second **code** symbol (`glabel` or `alabel`) is a
boundary problem, not a matching one: writing C for the named function drops
the other's bytes, so leave it and record it. A `.s` holding a second **data**
symbol (`dlabel`) is ordinary. The data does disappear when the C replaces the
function, and `check_unit` only says `NOASM`, so add an
`INCLUDE_RODATA(const s32, "<unit>", D_XXXXXXXX);` in address order and run
`configure.py --force-split` again. A `jtbl_` label needs nothing: the C
`switch` emits its own table.

`check_unit` will not tell you which kind you have. Grep the `.s` for a second
label before concluding anything.

## `lw $4` against `lw $2` plus `move $4, $2`

`if (p->h != NULL) { f(p->h); p->h = NULL; }` compiles two ways in this tree.
Retail's shape loads the handle straight into the argument register and tests
it there, so the copy does not exist and the delay slot goes to the next
block's load as a `beql`. ee-gcc only does that when `$2` is unavailable
across the guarded block: `pipeFreeEffects` in `pipe/pipeEffect` gets
`lw $4, 12($16)` because `$2` there holds the `u8` counter's `i + 255`, and
the identical shape in `code_00116AD0` gets `lw $2` plus `move $4, $2`
because nothing else wants `$2`.

So this is not a spelling of the guard. Ruled out on it: a reused local and a
fresh local per arm (cse propagates both), `while` for the `if`, `s32` and
unprototyped callee declarations, an `s32` parameter in place of `void *`, a
cast at the call site, and every loop spelling of the enclosing walk. Two
functions in `code_00116AD0` sit on it; see
[Non-matching drafts](nonmatching.md).

So when a caller sits within a couple of branch-annul words of exact and
calls something still in assembly, decompile the callee first rather than
permuting the caller. This is the same mechanism as the CONTEXT row below,
seen from the other side: what the compiler can see of the rest of the unit
changes the code it emits for one function.

## Functions that cannot be written in C

`fldSetTerminalState` branches into `fldSetTerminalIndex` across the symbol
boundary and computes a value in the delay slot that the second function uses
without setting. The second has a register live on entry, so no per-function
compile can produce either. They are one cross-jumping routine the IDB has
split into two symbols; see [TU recovery](tu-recovery.md).

Four more in `newbattle/nbMakePacket`, each blocked by something outside the
function body:

- `nbInitPlayer` needs three separate symbols at `test_skilllist`, `+0xF0`
  and `+0x100`. gcc 2.96 folds anything derived from one symbol into a base
  plus adds, so this is a data split, not a source change.
- `nbLoadMotionByIndex` has two byte-identical arms that gcc cross-jumps. The
  retail source must have differed in a way that still folds to the same
  instructions.
- A call with no argument to a function the unit defines with a prototype,
  as `Hatudo` and `nbE915_Test` do to `nbGetEventForm`. **There is a way out
  when the definition precedes the bare call**: define the callee old-style,
  `s32 f(id) s32 id; { ... }`, which accepts any arity and matched both the
  caller and the callee in `code_001FAE80`'s `shpItemIsUnlocked`. An
  unprototyped `extern` works instead when the callers sit above the
  definition. Try both before treating this as impossible.
- `nbMakaDispOn` needs `GetMakaProcessData` to take an argument, as the HD
  reconstruction shows, but it is defined `(void)` in this unit. This is
  *not* in general a blocker: adding the parameter to the definition costs
  nothing, since an unused parameter emits no instructions.
  `interface/frFont`'s `frGetYShiftLev` is defined `s32 frGetYShiftLev(void)`
  and matches, and giving it the two parameters `frSetFont` passes and it
  ignores left it byte-exact while letting `frSetFont` compile at all. Check
  that the callee still matches with the parameters added before concluding
  anything.

Two in `code_001FAE80` are blocked by the machine and the split rather than
by C:

- `func_00213DF0` uses `mult1`, the EE's second multiply pipe, in a way no C
  expression reaches. `mult1` on its own is not a reason to skip a function:
  ee-gcc 2.96's scheduler puts independent multiplies on the second pipe from
  ordinary C, and the `fldSkyDrawFilter` draft reproduces sixteen interleaved
  `mult`/`mult1` pairs from plain `v * gFilterAlpha / 128` expressions.
- `fclDrawSpriteEx`'s split assembly holds two symbols, so writing C for one
  would drop the other's bytes. That is a boundary problem, not a matching
  one.
