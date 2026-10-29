# Single-billboard tag linking

`dds3BillboardTagLink` is 1,180 retail bytes at `0x0015B320`. The C adapts
Megami-Decomps/dds-decomp (redthing1), `effect/billManager.c` at
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. SMT3 keeps its actual four pending
lists and resource offsets; the DDS five-list layout is not copied.

## Actual flat corner rows

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

## Ordinary double arithmetic and the real runtime

The facing factor is ordinary `1.0 - fabs(dot)`, with the dot widened from
float and the result narrowed back to float. The pinned compiler emits its
comparison libcall and double subtractions, preserving the native parameter
base across those calls. Explicitly spelling the same helpers previously
emitted 1,176 bytes; ordinary C closes the remaining register/lifetime issue.

The prerequisite, separately qualified zero-credit correction names the real
comparison provider `dpcmp`; see [IDB corrections](idb-corrections.md#double-comparison-runtime-name).
No `dpcmp` compatibility alias is added. The existing `EQUATE_FPTODP` and
`EQUATE_DPTOFP` macros retain the repository's established conversion bindings.

The aligned 16-float matrix is genuinely stored by VU0 and passed to
`sdfBillBoardMakeTMPacket`. The four-float direction carrier is genuinely
written by the native quadword store at `0x0015B4F4` (stack `+0x40`); it has
no later scalar read, and no dummy consumer is introduced. The parameter
vector is read through the existing barrier macro after its C Y-component
write. All VU register operations use existing concrete macros and their
established shared-register convention; no new asm, clobber or operand is
introduced.

## Qualification

The complete billboard owner has 62 exact functions and no differences,
including the untouched P2A caller/visibility arrangement and all owned data.
Header layout assertions, guarded drafts, and full retail image verification
pass; SHA-1 is `5facfbf7916836613449c922e26a4ec412207b32`. Only TagLink earns
new credit: one function, 1,180 bytes.
