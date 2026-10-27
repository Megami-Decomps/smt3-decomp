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

`sdf/sdfDev9`'s `sdf_2BFC58` is a hand-written quadword clear. Its body carries
`neg $2, $3`, `add $2, $2, $3` and `addi $3, $3, -0x8`, and cc1 only ever emits
`subu`, `addu` and `addiu`. It also jumps into the middle of an eight-`sq`
unrolled block through `jr $2` on a computed label address, which no C
construct reaches.

`newbattle/nbCameraPress`'s `nbCameraPressProcessInit` is frameless and ends
`beqz $2, nbCameraPressStart`, a conditional branch into the next symbol's
entry. ee-gcc 2.96 does sibling-call a trailing `void` call, but it keeps the
prologue while doing so: `kernel/dds3StartMenu`'s `InitTitleProcess` has
`sd $31`, `ld $31` and then `j dds3AdminiCallSequence`. Every spelling here
(if/else, an early `return` either way round, an explicit `return;` after the
call, and a `goto`) emits the same 60-byte framed `jal`. The two symbols are
one routine with a second entry point, the same shape as `fldSetTerminalState`
above; `nbCameraPressStart`'s matching two-store body is what `beqz` reaches.

Two more are epilogue-only symbols, which makes the function above each of them
unwritable too until the split is corrected:

- `sdf/sdfPrint`'s `sdf_2BD3E8` is three instructions, `ld $31, 0x70($29)`,
  `jr $31`, `addiu $29, $29, 0x80`, with no prologue. It is the epilogue of
  `sdfPrintRpcThread` directly above it, whose own 0x50 bytes end on
  `jal sceSifRpcLoop` and its delay slot and whose frame is exactly 0x80 with
  `$31` at 0x70. One C function covers both symbols' bytes, so neither can be
  written while the boundary stands.
- `sdf/sdfTmx`'s `sdf_2C7BC4` is the same thing for
  `SdfFileRpcServerThreadProc`: `ld $16/$17/$18/$31` at 0xB0, 0xB8, 0xC0, 0xC8
  and `addiu $29, $29, 0xD0`, matching that function's prologue exactly.

Both are `docs/tu-recovery.md` work, not matching work: drop the epilogue symbol
from the split and the function above it compiles as one piece.

## Handwritten assembly, found by an instruction cc1 never emits

ee-gcc 2.96 materialises a small constant with `addiu`, never `addi`, so an
`addi rd, $0, imm` in a body means the body was written by hand. The split
asm already marks each one `/* handwritten instruction */`; this is the list,
so nobody spends a pass on one. Re-derive it with:

    grep -rlE '\*/\s+(addi|subi) ' asm/smt3/nonmatchings/

21 functions, 3864 bytes. Nineteen are the `corePkt*`
VU1 packet builders in `sdf/sdfVu1`, which hand-assemble the DMA tags they emit.

`sdf/sdfDev9`:
- `sdf_2BFC58` (148 B)
`sdf/sdfMovie`:
- `release_movieobj2` (604 B)
`sdf/sdfTex`:
- `sdfTexSetPacket` (364 B)
`sdf/sdfVu1`:
- `corePktVu0Run` (308 B)
- `corePktEmitRefMapPass` (292 B)
- `corePktXfmLightClipUV` (252 B)
- `corePktXfmLightClipUVPB` (212 B)
- `corePktSkinPointsAccum` (204 B)
- `corePktXfmLightClip` (204 B)
- `corePktXfmLightClipPB` (164 B)
- `corePktApplyFaceDivColor` (156 B)
- `corePktRelightBackFaces` (144 B)
- `corePktSkinPointsFirst` (140 B)
- `corePktMorphVertsNorms` (116 B)
- `corePktSkinVertAccum` (100 B)
- `corePktXfmUV` (100 B)
- `corePktSkinVertFirst` (92 B)
- `corePktLoadVertsNorms` (76 B)
- `corePktMorphVerts` (76 B)
- `corePktNormalizeNorms` (60 B)
- `corePktLoadVerts` (52 B)
