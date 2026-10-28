# Control flow


| Retail shows | Change |
|---|---|
| two early `return <same constant>` reached by separate `if`s, with the constant's block laid out **after** the body | nest the two tests and give them one shared trailing `return K`. Written flat, cross-jumping merges the two blocks at the *first* one's position, which puts it between the second test and the body and inverts that test; nested, the merged block lands after the body and gcc's delay-slot pass steals the constant out of it, which is what fills the branch's slot. `cmpLoadStatusModel` in `code_0022C578` went from 18 of 38 words to exact; `cmpAddQuePanelEff` from a one-instruction-short cross-jump to 40 of 46 |
| `jal` and a fallthrough for the **last** `switch` arm while the other arms sibcall with `j`, where the switch value is a call result | give the call result a local and dead-store it in that arm, `flag = 0;`. With the call written straight into `switch (f())` there is no variable to store to, and a `void` return type does not reach it either. `cmpUpdateStatusModelLoad` needed both the `void` return type (for the other two arms' `j`) and the dead store (against the last arm's `jal`) |
| the opposite arm falls through (`bnez` where you emit `beqz`) | invert the `if` condition |
| a guard's `beq`/`bne` where your C emits `beql`/`bnel`, with the following pointer reload in the other register | flatten the nested `if`s into a chain of early returns. `camp_calc_status_Process` in `code_0022C578` tests a flag bit, calls a loader and acts on its result; nested, both guards come out branch-likely and the reloaded global lands in `$3` instead of `$2`, 5 of 33 words. The same body as `if (...) return 0; if (...) return 0; <tail>` flips both annul bits and the register pair at once. Reading the global into a local, an `&&` of the two tests, a `ret` variable and a local for the call result are all inert |
| a `b` over a single `daddu rd, rs, $0` placed at the very end of the function | the cached-pointer arm written as the `else`, not as an early return: `if (g == NULL) { ... allocate ... } else { work = g; }` with one trailing `return work;`. gcc puts the `else` body at the branch target, which is what lays that one copy after the allocation. `return g;` from inside the guard merges the two exits and `cmpAllocPanelEffWork` comes out 8 bytes short; `work = g;` read before the guard hoists the copy above the branch |
| the **same** call appearing once, with its pointer argument formed in two arms that share only the scaled index | write the whole call out in each arm and let `jump2` cross-jump it. Selecting the address into a local if-converts to `movz` however the condition and the arms are spelled, which costs the branch and 8 bytes. `cmpSetStockRemoveEff` in `code_0022C578` went from 13 of 33 words differing and 8 bytes short to 9 of 35 at the exact size on that alone; the taken arm has to be the one whose address needs a single `addiu`, so put it first |
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
| two consecutive guards that each return the same constant, where retail branches to one shared return block with the branch annulled onto the *epilogue*'s first restore (`beql` plus `ld $16` in the slot) rather than inverting to a single `bnel` | fuse the pair into one `if (a || b) { return 0; }`. Separate `if`s let gcc invert each branch straight onto the shared epilogue and come out two words short; the `||` chain gives the two tests one common true-label, which is the block retail jumps to. `CheckMukanNego`'s two `datCheckTalk` guards gained the two missing words on that alone, 476 of 492 bytes to 484 |
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
| a long flat chain of early `return 0` guards where one guard's `bnel` comes out as a `beq` over a hoisted `lui %hi(sym)`, one word long | convert **any one** of the guards to `goto ret0;` with a trailing `ret0: return 0;`. The shared zero's extra reference is what stops gcse hoisting the `%hi` of a symbol first used below the chain into the guard's own block. `fldInputPanelCheckKey` went from 44 of 68 words and one word long to 7 of 67 at the exact size, and `fldInputPanelProcess2` from 51 to 6, with the mask of which guards convert swept over all 128 and 256 combinations: every mask with at least one `goto` reaches the floor and the choice of guard does not matter. This is the row above on `ProcCampCheck` read the other way: there the point was how many to convert, here one is enough |
| a last guard whose `beql` tests the field in `$2` while your `beq` tests it in `$3`, with the return constant in the delay slot either way | write that guard positively, `if (x != 0) { body } return 1;`, not inverted with an early `return 1`. The inverted form fills the slot unconditionally so the load cannot have `$2`, and the whole tail's registers shift. That was the last 7 words of `fldInputPanelCheckKey` |
| a guarded block whose exit is a forward `goto` where your `break`-style early `return` rotates the registers | `goto ret0;` out of the guard with the body falling into `ret0: return 0;`. `fldInputPanelProcess2`'s `work->draw` test needed it; seven shapes of the tail (nesting the body, an early `return` per arm, a trailing `return 0` above the label) were swept against all 128 guard masks and only this one is exact |
| a two-sided clamp whose high arm has its own `jr $31` and a `mov.s` from the bound's register | `if (hi < x) { x = hi; } else if (x < lo) { x = lo; } return x;`. `return 2.0f;` in the high arm puts the constant straight in `$f0` and rotates every float temp, 20 of 34 words in `evtCalcEffBedScale`; an early `return x;` after the assignment does the same, and two sibling `if`s instead of the `else if` are 8 words out |
| a guard whose failure returns the **same variable** the function's tail returns, with two `daddu $2, rs, $0` copies and an empty `jr` delay slot | an early `return total;` inside the guard plus a second `return total;` at the end, not an `if` wrapping the body around one trailing `return`. The wrapped form coalesces the copy into the `jr`'s delay slot and is one word short; the two returns keep one copy in the guard's delay slot and one at the join, which `jump2` merges only down to the shared `jr`. `func_001C61B8` in `interface/fmGslCont` went from 5 of 44 words to exact. This is the opposite reading of the `rstUpdateEvoBgFade` row above, where the failure arm returns a *constant*, so check which one the arm returns before choosing |
| a `nop` in a call's delay slot, where your C fills it with the last argument's load | declare the local those arguments are read from `volatile`. A volatile read cannot be moved into a delay slot, which is the only thing that reaches an empty one here: `sdfMotCLBK_modelLinerRotXYZSeek` and its `Blend` sibling in `sdf/sdfMotion` store the lerped vector to a stack local and pass its three components to `FromEuler`, and retail leaves the `jal`'s slot empty. Both were 17 and 23 of 47 and 55 words and a word short with a plain local, and exact with `volatile sdfVector4_t euler;`. Measured and rejected on the same two: the `"memory"` clobber on the store macro, `f32 euler[4]` in place of the struct, three `f32` locals read before the call, and the read-write-address store form. The matching `sdfModelNodeBaseSet` in `sdf/sdfModel` calls `FromEuler` with three struct fields and does get the load in the slot, so this is about the local, not about the callee |
| a guard arm's first store duplicated into a `bnel` delay slot, with the value it stores computed *above* the branch, where retail has a plain `bne`, a `nop` and both inside the arm | read the tested field **in the guard** and declare the local **inside** the arm: `if (data->SwitchFrame == 0) { ... } else { u16 frames = data->SwitchFrame; ... }`. With the local read before the test, reorg hoists the arm's `frames - 1` into the test block and annuls the `sh` into the slot; read in the guard, the arm still owns both, and the arm then opens with a two-word `li.s` that `ee-as` cannot put in a delay slot, which is where retail's `nop` comes from. `lightmainprocedure` went from 98 of 130 words to 17 on that one move, and the arm's own statement order, `frames--`, `--frames`, a field read-modify-write, a separate `f32` local for the cast and a `goto` rewrite of the pair are all inert |
| three `case` labels sharing one body, where retail has a three-node compare tree and your C emits one `slti` range test | write the body out in **each** arm, `case 6: p[0] = v; break; case 7: p[0] = v; break; case 8: p[0] = v; break;`. Consecutive cases with one shared label are merged into a range before the tree is built; three separate bodies keep three nodes, gcc splits them at the middle one and `jump2` cross-jumps the identical stores back into one block. `itfPnlRegist2` went from 29 of 42 words and nine instructions short to exact |
| two early exits each returning a different call's result, where your C reuses one variable and spends a saved register on it | two locals and `return res;` then `return object;`. The single variable spans both calls, so its copy lands in a callee-saved register; retail copies the first result into `$3` for the test and leaves `$2` holding it as the return value. `evt_1FA398` went from 26 of 58 differing words to 21 |
| `slt index, count` then `xori rd, rd, 0` and a `movn`, where your C emits `slt count - 1, index` and a `movz` | write the guard relationally the way retail reads it, `if (index >= count) { x = a; } else { x = b; }`, not `if (count - 1 < index)`. Both are one `slt` and both if-convert, but the spelling decides the `slt` operand order and with it whether the select comes out `movn` or `movz`. The initialiser form, `x = a; if (index < count) { x = b; }`, does not if-convert here at all: it costs a branch, a second block and 16 bytes of frame. `fldDoor_ElvExit` went from 151 of 156 words and four words short to exact, with `count <= index` and `!(index < count)` equally good |
| a loop whose back edge assigns the one parameter and jumps to a label just past the prologue | a self tail call, `return f(value);`, not a `for (;;)` with `continue`. ee-gcc 2.96 eliminates self tail recursion into exactly that jump, while the `for` form gets jump-threaded: gcc proves the loop test true on the back edge and keeps its result in a callee-saved register, which costs the flag's register, its save and restore, and a duplicated loop header. `fldInfChkFastPanel` dropped from 56 to 32 excess bytes on the rewrite alone and is still open on the rest |
| `jal` and a full epilogue for **one** call where every other call in the function must keep its `j` | wrap that one call in a loop: `do { f(); } while (0);`. `expand_call` drops the sibling call whenever `stmt_loop_nest_empty()` is false, so any loop nesting around the call blocks it, only there, at no instruction and no register cost. Cheaper and more local than the whole-function `s32 dummy; s32 *p = &dummy;` and than a dead store, and it is the one form that works when the other arms of the same `switch` have to sibcall. `fldRunDanteAi` has nine `switch` arms that `j` and a default that `jal`s `fldRunDanteMove`, and went from 3 of 51 differing words to exact; `for (;;) { f(); break; }` and `while (1) { f(); break; }` are identical, while `(void)f();`, `if (1) { f(); }`, a prototype for the callee, a `goto` to a label just above the call, and the arm written with `break` or `return` all still sibcall |

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

- A message or result local that retail holds in a **callee-saved** register,
  where the C gets a caller-saved one or coalesces it into the argument
  register, is initialised **before** the function's leading call, not after
  it. `Flow2_9` in `newbattle/nbMakePacket` sets `mes = 0;` above
  `nbAddNegoIdKouho` and retail still emits the `daddu $17, $0, $0` after the
  `jal`, because the scheduler sinks it; with the assignment written after the
  call the local coalesces into `$6` and the frame loses `$17` entirely,
  32 of 36 words. The same placement is what gives `Flow6_40`, `Flow7_23` and
  `Flow7_24` their `$16`/`$17`.
- A two-constant `movn` from a ternary needs the **`== 0 ? low : high`**
  spelling. `cond != 0 ? high : low` reads as the direct translation of
  `movn rd, high, cond` and is what gives `movz` with the constants swapped,
  which is the `Flow2_13` row below read from the matching side:
  `n->nowselect == 0 ? 0xD : 0xF` is exact where `!= 0 ? 0xF : 0xD` is not.
  Four functions in `newbattle/nbMakePacket` turned on it.
- A **four-leaf constant tree whose two leaves on one side are merged by a
  `movn` on the *outer* condition** is a `goto` funnel, not a nested `if`.
  `Flow12_3` and `Flow12_4` in `newbattle/NbNegoFlowMukan` pick a rate from
  `plevel >= elevel` and `pparam < eparam`: retail branches on the level test
  first, keeps each `pparam >= eparam` leaf as its own `li`, and reaches one
  shared block for both `pparam < eparam` leaves that ends
  `li rate, 0x28` / `movn rate, 0x14, levelLess`. Write the two `pparam <
  eparam` arms as `goto low;` and put
  `low: rate = (plevel < elevel) == 0 ? 0x28 : 0x14;` after a `goto got;`.
  The nested `if`/`else` with the four constants written out is 43 of 99 words
  and eight bytes short; a ternary on either condition inside an `if`/`else if`
  chain reaches 33 but keeps the branch order wrong; the `goto` funnel is exact.
  The half that no source shape reaches on its own is one `movn` where the
  sibling leaves stay branches, so look for the funnel whenever a cmov's
  condition is the *outer* test rather than the one next to it.
- The **source order of a `switch`'s arms** is a lever in its own right and is
  cheap to sweep: `Flow6_40`'s three arms over `requesttype` 0, 1 and 3 are
  exact written 1, 3, 0 and 3, 1, 0, two words over written 0, 1, 3 or 1, 0, 3,
  and 36 of 55 words out written 0, 3, 1 or 3, 0, 1. gcc lays the arm bodies
  out in source order while the compare tree stays balanced over the case
  values, so the arm order decides which body the dispatch falls into. Read the
  arm bodies' address order in retail and write them in that order first, then
  sweep all permutations: six compiles take a few seconds.
- A `jal` with a bare `nop` in its delay slot and no argument setup is **not**
  evidence of an unprototyped call. It also happens when the argument register
  already holds the value from the surrounding code: `Flow1_18` calls
  `datGetRace(n->ework)` right after a twelve-way `||` chain on
  `n->ework->id`, so `$4` is already `n->ework` and gcc emits nothing.
  Reading that as `datGetRace()` and weakening the unit's prototype to
  `extern s32 datGetRace();` leaves the function 13 words out; the prototyped
  call with the argument is exact. Check whether the live value in the argument
  register is the argument before changing a declaration.
- An explicit **`return;` after the last call in a `switch` arm turns that call
  into a sibling call** (`j callee`, callee-saved registers restored first),
  where `break;` leaves `jal` plus the shared epilogue. `SetAction_WAIT` in
  `newbattle/nbStageProcess` has three arms whose last statement is a call; it
  is exact only with `return;` on the `case 10` and `case 8` arms and `break;`
  on the `case 3` arm, and the branch-likely form of the dispatch's second
  compare comes with it. All eight combinations compile in seconds, and the
  wrong ones are 14 to 51 words out at the same arm order, so sweep
  `return`-against-`break` on every arm that ends in a call before anything
  else. The same rule names which arm order to use: write the arms in the
  address order of their bodies in retail (`SetAction_WAIT` is 10, 8, 3, 9,
  with the empty `case 9:` last), then sweep.
- **Three cases over 0, 1 and 2 come out tested 1, 0, 2, and the arms are laid
  out in source order.** `balance_case_nodes` splits three singleton cases at
  the middle one, so a `switch` written `case 0:`, `case 1:`, `case 2:` emits
  `beq k, 1` then `beq k, 0` then `beq k, 2` with the bodies in the order the
  source lists them. `fldEveHit_ChkDir` in `field/fldDraw` went 102 differing
  words to 27 on the arm order alone, with the dispatch already right: read the
  first tested value for the middle case and the first *body* for the first
  case, and do not take the test order for the source order.
- **A guard whose early return is the `else` of the real body gets retail's
  branch-likely.** `if (x != K) { return 0; } body;` emits `bne` with the
  return value computed before the branch, while
  `if (x == K) { body; } else { return 0; }` emits `bnel` with the
  `daddu $2, $0, $0` stolen from the target, which is what retail has.
  `fldPlayerCalc` in `field/fldDraw` went 21 differing words to 17 on that one
  guard, and a `goto ret0` on the same test instead costs 16 bytes. Read the
  `l` on the branch before choosing which way round to write the guard.
- **Where retail branches *to* an `if`'s body, the test is inverted and the
  body is last.** `fldPlayerCalc`'s world-map guard is `beq fldWmWarpMode, 1`
  into a one-call arm placed after everything else: written
  `if (fldWmWarpMode == 1) { call(); return 0; } rest;` ee-gcc puts the call
  inline behind a `bne`, and `if (fldWmWarpMode != 1) { rest; return 0; }
  call(); return 0;` is retail's, worth 49 words here. The same inversion did
  not work on the three-way `mode` dispatch in the same function, where a
  `switch` is what places both arms out of line.
| `beq` against zero, then `bltz`, then `slti` on one value, where your `if` chain folds to a single `sltiu` | a `switch` with an **empty `case 0:`**, the real range as contiguous `case` labels, and the out-of-range work in `default:`. `SetGroupTarget2` guards its marking loop on `1 <= code <= 10` and zeroes `code` otherwise: as `if (code == 0) {} else if (code < 0) { code = 0; } else if (code >= 11) { code = 0; } else { loop }` it is 29 of 56 words out, and as `case 0: break;` plus `case 1:`..`case 10:` plus `default: code = 0;` it is exact. The empty zero case is what keeps the `beq` separate; the value `0` needing no work and the default needing some is exactly what the switch expresses and the chain does not |

An ascending `for` over a byte array is what produces retail's **descending**
walking pointer. `for (i = 0; i < 0x18; i++) { arr[i] = 0; }` gives
`addiu p, base, 0x17`, a `bgez` downcounter and `sb $0, 0($p)` with the pointer
decremented in the branch's delay slot. All four descending spellings,
`for (i = 0x17; i >= 0; i--)`, the same as a `while`, a `do`/`while (--i >= 0)`
and `i = 0x18; while (--i >= 0)`, leave the address as `addu rd, base, i`
recomputed inside the loop and come out two words short, and `memset` of the
array is wrong from the first word. `NewGame2Push`'s 24-byte clear went from
154 of 328 differing words to exact. This is the store side of the "global
pointer read inside a loop" row: there a destination local makes gcc reverse an
ascending loop, here the ascending loop is already what retail wrote.

Where a `switch` arm's stores come out in an order you did not write, sweep
**one** arm's permutations and transfer the winner. `fldRunDanteInit`'s seven
arms each store two gp-relative `sh`s and two struct `sh`s, gcc permuted each
arm differently, and retail's source order is the same in all seven: the two
globals first, then the two struct fields. Sweeping the first arm's 24 orders
found it in 24 compiles and the other six arms, including the six-statement one
with two more array stores appended, then matched with no further search.

Whether a `switch` becomes a jump table or a comparison tree is decided by the
number of case *nodes*, after `group_case_nodes` merges adjacent cases that
share a label; below four nodes gcc emits comparisons. So a seven-value switch
whose arms are `0,1,2,5,6` to one body and `3,4` to nothing collapses to two
nodes and comes out as a tree. Writing the two empty cases first, with the five
sharing one label after them, keeps the nodes at four and gives retail's table
with the same label repeated in five slots:

```c
switch (eff->kind) {
case 3:
    break;
case 4:
    break;
case 0:
case 1:
case 2:
case 5:
case 6:
    f(...);
    break;
}
```

Writing the five arms out separately also gives a table, but the duplicated
bodies are not cross-jumped and the function is eight instructions long.
`cmpUpdatePanelEffSlots` and `cmpFindPanelEffSlot` both need this.

The node count also decides where a comparison tree is rooted, and three nodes
is a special case: `balance_case_nodes` has a literal "if there are just three
nodes, split at the middle one", so a three-case switch always tests the middle
value first. Retail rooting such a tree at the *lowest* value, with the
`index <= low` test to the default after it, means its switch had a fourth
label. `Effect2` in `newbattle/nbE911` switches on 0x4B, 0x53 and 0x8C and
needs a fourth empty case ahead of them to get retail's `beq 0x4B` root; with
three it tests 0x53 first and the whole dispatch is 22 words out. The bisect
for four or more nodes is by count, never by the cost table, since
`estimate_case_costs` gives up as soon as one case value exceeds 127.

Where a value computed from a parameter lands in the callee-saved register the
parameter's own prologue copy occupies, and the copy is a separate `daddu`
before the shift that overwrites it, the parameter is live across the call: the
computation belongs **inside** the guarded block after it, not before.
`camp_23CDB8` and `camp_23CC70` compute `x + h * 16` inside
`if (g != 0) { ... }`, after the `cmpDrawBoxF` call that still reads `h`; with
it hoisted above the call the scheduler produces the same instruction count but
a different register for every local, 47 of 60 words out.


### Shared switch assignment and branch annulment

`rcvUpdatePanelAnim` in `facility/fclRecoverUpdate` needs phases 1–4 to
fall through to phase 5's single `active = 0` statement after their flag guard.
Separate identical assignments leave one differing word: plain `beq` instead
of retail's `beql` at +0x28, with the same zero assignment in the delay slot.

Both forms were traced with:

```
tools/cc1probe.sh <candidate.c> \
    --as-unit src/smt3/facility/fclRecoverUpdate.c \
    --func rcvUpdatePanelAnim --reorg --no-dumps --out /tmp/recovery-panel
```

Both returned `verify: OK`: hooked and unhooked assembly matched the plain
compilation. In the separate-assignment form, reorg's first delay pass fills
branch uid76 from zero assignment uid48. With switch fallthrough, branch uid70
has no such predecessor assignment; `fill_slots_from_thread` at reorg.c:3009
instead takes the shared zero assignment uid42 from its target. The opposite
path keeps `$16` live, so that target fill must annul. Both equality branches
have the same probability 3999 and `mostly_true_jump` result 0: prediction
annotations are not needed. The ordinary fallthrough reproduces all 168 bytes;
the complete unit and retail-image checksum remain the qualification.

In camp `cmpUpdateHeartsAttachConfirm`, the cancellation path restores the
previous parameters and resets its local choice with `cancelled = 0`. Retail
has `jal cmpReturnParamDH` plus the shared epilogue; omitting the reset instead
emits a sibling call and changes four words. The qualified compiler trace
retains the reset through `rtl.01.sibling` and `rtl.11.cfg`, then deletes it in
`rtl.13.life`. Hooked and unhooked output and dump/plain assembly agree. The
result matches all 272 bytes. Reproduce with
`tools/cc1probe.sh src/smt3/code_0022C578.c --func cmpUpdateHeartsAttachConfirm --reorg`.
The same-unit `cmpUpdateHeartsAttachStatus` resets its handled button after
`cmpPlayCursorSE` and preserves its native `jal` plus epilogue at 396 bytes.
Both resets update their own completed local state, under the contributing
rules' qualified post-call-statement exception.

### Reset a consumed cursor-row local after the default call

`nmeInitCursor` (`nme/Nme_Init`) resets its local `row` after the default
`cmpResetCursorPos(0, cursor)` call. The reset is dead local state, not an
observable work-record write. This uses the narrow post-call statement
exception in CONTRIBUTING: retail has a `jal` and epilogue on that path,
while rows 3 through 5 use the early tail return.

A qualified `tools/cc1probe.sh src/smt3/nme/Nme_Init.c --func nmeInitCursor
--jump` run reports `verify: OK`, including dumped/plain assembly parity.
Reset UID 149 follows ordinary call UID 145 in `.01.sibling` and remains
in `.11.cfg`; it disappears in `.13.life`. The in-range call UID 110 is
already a sibling call and remains one. Removing the reset makes the
otherwise exact initializer eight bytes short. The complete unit stays exact.
