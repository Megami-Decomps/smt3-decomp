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
| `slt` then `sltiu` | write `if (a < b) return 0; return 1;` rather than `return a >= b` |
| explicit branches where you emit `andi` or `sltiu` | gcc folds `(x & 1) ? 1 : 0` and `f() == 0`. Usually unreachable from C |
| the same field stores duplicated per `switch` case | write the full store list in each case and let gcc merge the tail. Hoisting the values into locals does not match |

## Addressing and types

- Externs of 8 bytes or less are addressed through `$gp`. Where retail uses
  `lui`/`%lo`, declare the symbol as an incomplete array (`extern s32 x[];`)
  to force absolute addressing.
- `div` against `divu`, and `lb` against `lbu`, come from a cast at the use
  site, not from narrowing the field's declared type.
- An address that looks like a global may be a `.lit4` pool entry holding a
  float constant, and must be written as a literal.
  `tools/check_unit.py` says so when you get it wrong.
- Holding a struct address in a local (`nbCamera_t *p = &pNbCamera;`)
  reproduces retail keeping it in a saved register. Assigning it after a
  leading call keeps it out of `$16`.

## Float literals

ee-gcc truncates a decimal literal toward zero, so `-946.42f` lands one ULP
away from retail and `-946.41993f` is exact. Derive the shortest decimal that
round-trips to the retail word rather than transcribing the rounded value.

`ee-as` owns the `.lit4` pool, not `cc1`. `configure.py` rewrites every
`lwc1 %gp_rel(D_...)` in the split asm into `li.s`, so literal values are
readable in `build/eeasm/` and the pool is rebuilt per object in emission
order. The order in which the C uses a constant has to match retail.

## Stores

Adjacent constant stores come out in reverse source order, so swapping them
is the first thing to try on a two-word diff. A short-circuit `||` with one
store beats an if/else chain that stores in each arm.

## Check the whole unit, not the function you edited

ee-gcc's output for one function can change with code earlier in the file, so
a function that matched can stop matching because of an unrelated edit.
`tools/check_unit.py` reports that as `CONTEXT <name>: compiles differently
inside the full unit`. Always read the whole unit's result, not the line for
the function you were working on.

## When to stop

Revert to `INCLUDE_ASM` rather than leave a differing function: a near miss
earns no credit and hides the fact that the unit is incomplete.
`python tools/revert_differing.py <unit.c>` puts every function the unit
check will not credit back to `INCLUDE_ASM`, which is faster and safer than
doing it by hand. Run `configure.py --force-split` afterwards, since the set
of `INCLUDE_ASM` lines has changed.

These have been ruled out repeatedly and are not worth a second attempt:

- Pure instruction scheduling, where the same instructions appear in a
  different order with no semantic difference to steer.
- Register allocation that needs one register fewer than the C uses, shifting
  every later allocation. Change the source shape or stop.
- `bnel` and the other branch-likely forms, which duplicate the epilogue into
  the delay slot. ee-gcc 2.96 does not emit them from C here.
- A frameless conditional sibling call. ee-gcc always saves `$ra` for a
  sibcall.
- Zero-extending a byte with a `sll 24`, `srl 24` pair. ee-gcc emits
  `andi 0xff` for every source shape tried.

A function whose body is mostly inline assembly is refused by the production
report anyway, at 8 instructions and more than half the body, so writing one
to force a match earns nothing. See [Reading progress](progress.md).

## Two functions that cannot be written in C

`fldSetTerminalState` branches into `fldSetTerminalIndex` across the symbol
boundary and computes a value in the delay slot that the second function uses
without setting. The second has a register live on entry, so no per-function
compile can produce either. They are one cross-jumping routine the IDB has
split into two symbols; see [TU recovery](tu-recovery.md).
