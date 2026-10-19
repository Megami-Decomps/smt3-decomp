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
| `slt` then `sltiu` | write `if (a < b) return 0; return 1;` rather than `return a >= b` |
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
| one shared exit with the return constant set in each predecessor's delay slot, and one arm falling into it | a `goto` to a single trailing `return 0`. Nesting the bodies inside their guards, an `&&` chain and an `else if` chain all reach within two words but keep one exit too few; the explicit label is what lets gcc put a `$2 = 0` in a delay slot on one path and fall into another on the next. `nbCheckModelPackLoaded` went 13 of 52 differing to exact |

## Addressing and types

- Externs of 8 bytes or less are addressed through `$gp`. Where retail uses
  `lui`/`%lo`, declare the symbol as an incomplete array (`extern s32 x[];`)
  to force absolute addressing.
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

- `case_values_threshold` is 10 in this build: a `switch` needs at least ten
  `case` nodes before ee-gcc emits a jump table rather than a compare tree,
  independently of the range test. Where retail has a jump table and the
  reconstruction has fewer labels, the missing ones share the `default` body.
- `extern f32 fabsf(f32);` and an ordinary call compile to `abs.s` directly,
  verified against this toolchain. `EE_ABS_F32` is what fails to match where
  retail has a plain `abs.s`.
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
- Blocking the sibling call where retail uses `jal` and a full epilogue has
  two forms, and which one works varies by unit. Giving a void function an
  `s32` return type with no `return` statement collapses a duplicated epilogue
  too, and took one function from ten differing words to exact. It failed for
  all seven functions tried in `code_0029B780`, where the answer was a dead
  store instead: introduce a local for one of an earlier call's constant
  arguments and assign 0 to it after the last call. Try both.
- An ignored return value still reserves `$2` across the call. When retail
  skips `$2` for the address temp after a `jal`, declare the callee `s32`
  rather than `void`.
- An implicit declaration is itself a void against non-void trap. A callee
  defined further down the unit is implicitly `int` at the call site and
  reserves `$2`, so a forward prototype alone can fix a near miss: one added
  prototype took four functions from two words off to exact. When a `$gp`
  reload lands in the wrong register, check whether the callee is merely
  implicitly declared before concluding its return type is wrong.
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

## Function-local constant arrays

A block-scope initializer such as `s32 list[3] = {1, 11, 14};` goes to
`.rodata` at the function's position in the C file, not where splat put the
`INCLUDE_RODATA` line for it. Move the `INCLUDE_RODATA` lines for every
rodata object that precedes it above the function, or the relink reorders the
pool. `check_unit.py` masks local rodata relocations and still says the
function matches; only the `ninja` checksum catches this.

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

To tell whether a `gp`-addressed `D_00XXXXXX` is a pool entry, subtract the
flat-image delta `0xff000` from its vram and look the file offset up against
the `.lit4` rows in `config/smt3/SLUS_209.11.yaml`, whose addresses are file
offsets. `D_0054C2B4` and `D_0054C2B8`, read by `fldCamCalcSlopeOffset`, land
at `0x44D2B4` and `0x44D2B8` inside `field/fldDraw`'s pool and hold `0.001f`
and `-0.001f`. The pool is not deduplicated: the same constant appears at
several offsets, so only the order of use matters.

`ee-as` owns the `.lit4` pool, not `cc1`. `configure.py` rewrites every
`lwc1 %gp_rel(D_...)` in the split asm into `li.s`, so literal values are
readable in `build/eeasm/` and the pool is rebuilt per object in emission
order. The order in which the C uses a constant has to match retail.

## Stores

Adjacent constant stores come out in reverse source order, so swapping them
is the first thing to try on a two-word diff. A short-circuit `||` with one
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

## A caller can need its callee's definition, not just a prototype

Measured in `mc/slMain`: `slDrawInfo` at 183 of 189 words and `slDrawError`
at 293 of 299 both become exact the moment `slDrawWindow` is a C definition
above them, and while it is still `INCLUDE_ASM` they each emit six `bnel`
where retail has `bne`, on exactly the tests whose arms call it. Cracking one
callee therefore banked 801 words across three functions.

So when a caller sits within a couple of branch-annul words of exact and
calls something still in assembly, decompile the callee first rather than
permuting the caller. This is the same mechanism as the CONTEXT row below,
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
- An epilogue duplicated into a branch-likely delay slot, where ee-gcc emits
  the plain branch. Note the narrow scope: ee-gcc 2.96 emits `bnel` from
  ordinary C readily, 42 times in `field/fldDraw` and 126 in
  `newbattle/nbMakePacket`, so a `bnel` in the retail code is not a reason to
  skip a function. Only this one use of it has resisted.
- A frameless conditional sibling call. ee-gcc always saves `$ra` for a
  sibcall.
- Zero-extending a byte with a `sll 24`, `srl 24` pair. ee-gcc emits
  `andi 0xff` for every source shape tried.
- `xori reg, reg, 0` in front of a `movn`. No `!= 0` shape (plain `if`,
  ternary, `> 0`, `>= 1`, explicit `^ 0`, a `continue`, a local of either
  width) adds it; every one of them emits the `movn` alone.
- A `jal` plus a shared epilogue in a `switch` default arm, where `return`
  against `break`, a non-void or struct return type on the callee, and a dead
  assignment of the call's *result* all leave the arm a sibcall. A dead store
  to another variable does work, so try that first: see the tail-call row
  above.

A function whose body is mostly inline assembly is refused by the production
report anyway, at 8 instructions and more than half the body, so writing one
to force a match earns nothing. See [Reading progress](progress.md).

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
  reconstruction shows, but it is defined `(void)` in this unit.

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
