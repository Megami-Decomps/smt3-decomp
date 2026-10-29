# Billboard matching

## Single-billboard tag linking

`dds3BillboardTagLink` is 1,180 retail bytes at `0x0015B320`. The C adapts
Megami-Decomps/dds-decomp (redthing1), `effect/billManager.c` at
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. SMT3 keeps its actual four pending
lists and resource offsets; the DDS five-list layout is not copied.

### Actual flat corner rows

The canonical batch's `offset` field is fifteen rows of eight floats, not
fifteen rows of four two-float structs. Both layouts have the same extent,
but the flat coordinates have independent provider and peer evidence:

- `sdfBillBoardMake1` reads eight consecutive floats. `sdfBillBoardMakeN`
  and its multi counterpart copy 32 bytes per quad from the offset stream.
- The existing `effPCPMisc.c` batch view already has `f32 offset[15][8]`.
- Native TagLink shifts the row index by five and stores eight X/Y values
  at batch offset `0x21C` onward. `count` remains at `0x3FC`; the entire
  batch is still `0x400` bytes and the following resource is unchanged.

Direct flat indexed stores recover the native address-copy chains. A private
control retaining the old VTX rows and casting each row to float emits 1,112
bytes; the truthful flat field reaches 1,176 bytes with explicit soft-double
helper calls. This is a real coordinate representation, not a parallel alias,
fake stride, synthetic padding or register constraint. Other header consumers
only pass the stream to packet providers, and `Sprite3D_t::vt[4]` is untouched.

### Ordinary double arithmetic and the real runtime

The facing factor is ordinary `1.0 - fabs(dot)`, with the dot widened from
float and the result narrowed back to float. The pinned compiler emits its
comparison libcall and double subtractions, preserving the native parameter
base across those calls. Explicitly spelling the same helpers previously
emitted 1,176 bytes; ordinary C closes the remaining register/lifetime issue.

The prerequisite, separately qualified zero-credit correction names the real
comparison provider `dpcmp`; see [IDB corrections](idb-corrections.md#double-comparison-runtime-name).
No `dpcmp` compatibility alias is added. The conversions now use the native `fptodp` and `dptofp` symbol names directly.

The aligned 16-float matrix is genuinely stored by VU0 and passed to
`sdfBillBoardMakeTMPacket`. The four-float direction carrier is genuinely
written by the native quadword store at `0x0015B4F4` (stack `+0x40`); it has
no later scalar read, and no dummy consumer is introduced. The parameter
vector is read through the existing barrier macro after its C Y-component
write. All VU register operations use existing concrete macros and their
established shared-register convention; no new asm, clobber or operand is
introduced.

### TagLink-only qualification

At the TagLink-only snapshot `e27ec762`, the complete billboard owner had
62 exact functions and no differences,
including the untouched P2A caller/visibility arrangement and all owned data.
Header layout assertions, guarded drafts, and full retail image verification
pass; SHA-1 is `5facfbf7916836613449c922e26a4ec412207b32`. Only TagLink earns
new credit: one function, 1,180 bytes.

## Animation drawing

`dds3Billboard_P2AAnimDraw` is 464 retail bytes at `0x0015C400`. Commit
`79519a4d` activates its existing retained C body without changing its
statements, declarations, types or compiler flags. The complete billboard
owner checks **63 match / 0 differ**, including the 696-byte
`dds3Billboard_P2ADraw` caller and all owned data. Ordinary and stock `-da`
compiles agree; the full retail image retains SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`.

The earlier annul-bit trade below is historical, not a current fallback.
Reactivating the unchanged guard at `c1223151` and `e275e63c` reproduces the
caller's one differing word at `+0x144`; the same activation in `e27ec762`
checks 63 / 0. The relevant source context changed when the native-sized
TagLink implementation, real flat corner-row type, and ordinary double
arithmetic landed. These observations establish the context in which the
trade disappears, not which individual change altered compiler liveness.
No new animation source form or internal compiler cause is claimed.

### Historical caller-visibility trade

The following controls and trace observations describe the earlier source
context. They are retained here rather than listed as an unresolved draft.

The body was byte-exact as C but was kept behind the fallback because its
visibility as a definition cost `dds3Billboard_P2ADraw` one annul bit: with this function
compiled, `P2ADraw`'s `modeFlags & 0x40` test emits `beq $2, $0` where retail
has `beql`. Parked, `P2ADraw` is byte-exact. The trade is 464 bytes here for
696 there at the same matched-function count, so it is worth +232 bytes.

Two earlier readings of this pair were wrong and are corrected here, both by
direct measurement:

- The fallback **does** reach callee visibility. An earlier entry said the
  effect needs a compiled body at that point in the unit rather than merely
  the symbol gone, and that only renaming the definition would close
  `P2ADraw`. Parking this function with `#ifdef NON_MATCHING` closes it, same
  as renaming: 60 match, 0 differ, checksum passing.
- Only this function is the effective neighbour. Parking
  `dds3Billboard_P2ASetNumber` instead, which an earlier reading said would
  work equally and would be the cheaper trade at 432 bytes, leaves `P2ADraw`
  at 1 of 174 words differing. The two neighbours are not interchangeable.

Ruled out, each measured: hiding `dds3Billboard_P2AColorMul` the same way (no
effect); and `dds3BillboardTagLink` present as a C definition or not, which
changed nothing in that older source context. Those controls did not use
the later matched TagLink implementation.

`P2ADraw`'s own four levers are recorded here because the draft they came from
is now in the build.

At `c1223151`, paired current-provider reorg probes confirmed the same residual. Both the
parked source and the same unit with only the animation body unwrapped passed
`verify: OK` with `tools/cc1probe.sh --func dds3Billboard_P2ADraw --reorg
--no-dumps` (the candidate also uses `--as-unit src/smt3/effect/billManager.c`).
Hooked and unhooked assembly had output parity.

The branch at instruction UID227 has `REG_BR_PROB=3999` and
`mostly_true_jump=0` in both compiles. The later `0x80` mask, UID389, changes
ordinary delay-slot eligibility when the animation definition is visible;
the visible-body run selects it with flags `0x11`. The observed difference is
eligibility, rather than the branch-probability classification. Which liveness
input causes that difference remains unresolved.

Destructively narrowing the existing local flags for the final `0x80` dispatch
worsens the caller to three differing words. Early returns after selected modes
and a one-pass dispatch block with breaks both leave the original single-word
residual. None of these forms qualified the full unit, so they were discarded.

The rest of the function needed four separate levers and they are all in the
draft: the plural loop's `out` pointer has to be the index-cast form
`(BillOut *)((u8 *)(i * sizeof(BillOut)) + (u32)obj->resolvedEntries)` for
retail's `addu rd, index, base`; the `frameIndex < 0` arm has to be written
*last* (`if (frameIndex >= 0) { draw } else { frameIndex++ }`), or gcc places
the small arm first and emits `bgez` where retail has `bltz`; the plural
branch's two trailing restores have to be written rotation-then-colour, which
emits colour-then-rotation and stops gcc cross-jumping the colour store into
the shared exit the other three branches use; and the packet-list selection has
to assign `obj->pair.kind` directly in all three arms and let gcc cross-jump
the three `sh` into one. A `u16` or `u32` local for that value, with or without
`goto`s, is if-converted into a `movz` pair (23 words out), and a `switch` over
`flags & 6` gets the size right but reshapes the compare chain (18 words out).
