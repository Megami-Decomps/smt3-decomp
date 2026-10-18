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

## Addressing and types

- Externs of 8 bytes or less are addressed through `$gp`. Where retail uses
  `lui`/`%lo`, declare the symbol as an incomplete array (`extern s32 x[];`)
  to force absolute addressing.
- `div` against `divu`, and `lb` against `lbu`, come from a cast at the use
  site, not from narrowing the field's declared type.
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
- Giving a void function an `s32` return type with no `return` statement
  blocks the sibling call where retail uses `jal` and a full epilogue. It also
  collapses a duplicated epilogue: one function went from ten differing words
  to exact on that change alone.
- An ignored return value still reserves `$2` across the call. When retail
  skips `$2` for the address temp after a `jal`, declare the callee `s32`
  rather than `void`.
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
- Pick the VU0 load by how retail forms the address:
  `SDF_VU0_LOADVECTOR(ptr)` where it computes the address into a register,
  `SDF_VU0_LOADVECTOR_MEMOPS` where it addresses the frame directly.

## Function-local constant arrays

A block-scope initializer such as `s32 list[3] = {1, 11, 14};` goes to
`.rodata` at the function's position in the C file, not where splat put the
`INCLUDE_RODATA` line for it. Move the `INCLUDE_RODATA` lines for every
rodata object that precedes it above the function, or the relink reorders the
pool. `check_unit.py` masks local rodata relocations and still says the
function matches; only the `ninja` checksum catches this.

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

Where a handful of constant stores differ only in order, permute them
mechanically rather than reasoning about it. A loop driving
`check_unit.py --func <name>` costs about a second per permutation; two
functions turned on the first of 24 permutations and on a rotation of three.

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

That one line is why the draft is worth keeping: it tells the next person
whether to continue, start over, or leave it. The source shapes you ruled out
go in the commit message, which is where the reasoning for any change belongs.
`--discard` deletes a draft not worth keeping.

A draft is never evidence of anything. It is not compiled by the build, so
`tools/check_nonmatching.py` is what stops it rotting when a struct or a
signature changes under it.

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
- `Hatudo` and `nbE915_Test` call `nbGetEventForm` with no argument, which
  only an unprototyped declaration produces, and that function is defined
  with a prototype in the same unit. An unprototyped `extern` does work for
  callers placed above the definition, which matters if the unit is split.
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
