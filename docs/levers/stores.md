# Stores


Adjacent constant stores come out in reverse source order, so swapping them
is the first thing to try on a two-word diff. For a longer run the rule is a
right rotation by one, and it holds for a run of twelve: `frInitFontHandle`
writes `glyph->chainHead = glyph;` last and retail emits that store second,
so take retail's emitted list and left-rotate it by one to get the source
order. Mixed `sb` and `sw` stores rotate together. A short-circuit `||` with one
store beats an if/else chain that stores in each arm.

- Two unrelated member stores in front of an expression that loads a `gp`
  global decide the whole block's schedule, and the winning order is not the
  one the stores come out in. `Flow1_12` writes `n->workid[3] = 0x1000F;`
  first, `n->nextnegoid = n->nextkouho[1];` second and the loaded expression
  third, yet retail emits the `sw 0x1C` before the `sw 0x68`; the other five
  orders are 10 to 45 words out. Three statements is six compiles, so sweep
  them rather than reading the emitted order back into the source.

- Where retail's last store before a `jal` is a constant store rather than the
  argument setup, hoist the argument into a local above the stores. The store
  then falls into the call's delay slot. `nbSetBadstatMotion2` went from six
  differing words to exact on `party = &data->party[form->partyindex];` moved
  above three `sw $0` stores, and no order of the stores alone reached it.

- Two adjacent struct stores only reverse when neither value is a reload. A
  value an output parameter left in an **address-taken** stack local is killed
  by the first store through the struct pointer, so gcc reloads it for the
  second store and the pair then comes out in source order instead. Copy it
  into a plain local first and test *that* local: in
  `dds3SceneBasicLoadScriptFile`, `resolved = address;` ahead of
  `if (resolved == 0)` lets `work->scriptHandle = handle;
  work->script = (void *)resolved;` emit as retail's `sw 0x1C` then `sw 0x18`,
  one word shorter. Four spellings were measured: without the local, either
  store order is 2 or 11 words out (the 11 being the reload and a branch
  displacement), a `void *` local for the same value does not reach it, and a
  `(u32 *)` cast on either store's lvalue does not either.

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
- Field stores placed **after** the pointer arithmetic that reads the same count
  keep the count in its own register. `dds3NeedleModelVNCListCreate` computes
  `normal = position + vertexCount; color = (u32 *)(normal + vertexCount);` and
  then stores the four header fields in ascending offset order: that keeps `n`
  live across the `sll`, so the scaled offset takes a caller-saved register of
  its own where storing `work->vertexCount` first lets the shift reuse `n`'s
  register in place. The scheduler still hoists the count store back to retail's
  position, so the source order is not visible in the output. 4 of 142 words.
- A vertex writer's three component stores are written in **field order**, `x`,
  `y`, `z`, even where retail emits the `z` store between the other two.
  `dds3NeedleModelVNCListCreate` writes nine such triples; with the `0.0f` `z`
  store placed between the `sdfCos` and `sdfSin` calls, as the asm's emitted
  order suggests, the schedule moves it ahead of the `jal` and the function is
  40 of 142 words out, and 17 with only the position triples still reordered.
