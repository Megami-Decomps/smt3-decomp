# Functions that cannot be written in C


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
  unprototyped `extern T f();` works instead when the callers sit above the
  definition, and also when the definition is parked behind
  `#ifdef NON_MATCHING`: `code_0022C578` declares
  `extern cmpPanelEff_t *cmpAllocPanelEffWork();` so the bare call in
  `cmpStartProcessPanelEff` keeps matching whichever side of the guard is
  compiled, with the draft written old-style. An ANSI `(s32 value)` prototype
  there makes the caller set up `$4` and breaks it. Try both before treating
  this as impossible.
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
