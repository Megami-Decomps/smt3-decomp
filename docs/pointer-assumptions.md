# Pointer assumptions in the engine's C

The decompilation is 32-bit MIPS C: a pointer is four bytes, and the engine
stores one in a `u32` wherever the retail code did. A consumer running this
code natively on a 64-bit host cannot reinterpret `(u32)somePointer`, so the
only help the decompilation can give is a name the consumer redefines.
`include/eeregs.h` already carries that pattern: `EE_PHYSICAL`,
`EE_UNCACHED`, `SDF_DMA_TAG`, `SDF_FLAG_WAIT`, each `#ifndef`-guarded and
identity on target.

This is an inventory by pattern, not by site. A 9500-function decompilation
has thousands of `p + 1`s and naming them all would make the code worse.
Rows are ranked by what a native consumer actually needs: pointer size is a
correctness problem, implied sizes are a readability problem, cast arithmetic
mostly is not a problem at all.

Counts are measured over `src/smt3/**/*.c` (309 units) with the scripts
described per row; where a heuristic can produce a false positive the row
says so and gives an upper bound.

## Class 1: pointer size

A pointer narrowed to `u32`, or a `u32` widened back to a pointer. The only
class whose absence is a bug on a host rather than an unnamed constant.

| Pattern | Count | Files | Representative sites | Macro pays? |
| --- | --- | --- | --- | --- |
| `(u32)<pointer-returning call>` | 403 | 47 | `sdfDrawTagLink(list, (u32)sdfDevPrintf(...))` (`basic/dds3DbgBasic.c:51`), `sdfDrawTagLink(tag, (u32)sdf3DPrimMakePoly(&request))` (`pb/pbModel.c:667`), `(u32)effPkt3DPrimMakePoly(...)` | Yes |
| `(u32)<pointer variable or member>` | <=276 | <=60 | `sdfDrawTagLink(tag, (u32)packet)` (`code_0022C578.c:7411`), `*cursor++ = (u32)aParm->zBias` (`sdf/sdf3DPrim.c:285`), `node->data = (u32)sourceAddress` (`model/mdlEffect.c:618`) | Yes |
| `EE_PHYSICAL((u32)<pointer>)` | 22 | 12 | `sdf/sdfMat.c:455`, `sdf/sdfIpu.c:442`, `sdf/sdfDevCons.c:346` | Yes, composes with the existing `EE_PHYSICAL` |
| `(T *)(u32)<expr>` and `(void *)(u32)<expr>` | 16 | 8 | `sdf/sdf3D.c:491`, `mc/slMain.c:2605-2608`, `facility/fclCombineInit.c:1042` | Yes |
| `(u32)&<object>` | 18 | 10 | `sdf/sdfGsMemIO.c`, `sdf/sdfModel.c`, `facility/fclRibbon.c` | Yes, same macro as the variable row |
| `(u32)&((T *)0)->field` (offsetof) | 5 | 3 | `sdf/sdfModel.c:603`, `model/mdlManager.c:1497`, `newbattle/NbNegoFlowOne.c` | No. A field offset is correct on a host as written; a host does not redefine it |
| `(u32)(unsigned long)<expr>` | 1 | 1 | `sdk/libc.c` | No. One site, and it is C runtime, not engine |

The first two rows are the same idiom seen from two sides, and 433 of them are
the argument of one API family: `sdfDrawTagLink`, `sdfDrawTagLink2`,
`sdfDrawTagLinkRef`, `sdfDrawTagLinkRefGif`, `sdfDrawTagLinkCall`, each
declared `(sdfDrawTag_t *aTag, u32 aPacket)`. The pointer-size assumption is
in the signature, so the call-site cast is forced.

**Caveat a consumer must know.** Naming the call site does not by itself make
the API 64-bit clean: `aPacket` is still `u32`, so a host needs a wider
address type in those signatures as well. The macro marks every place that
narrowing happens, which is the prerequisite, not the whole fix.

Scripts: pointer-returning calls were found by collecting every identifier
appearing as `(u32)<ident>(`, resolving its declared return type across
`include/**` and `src/smt3/**`, and dropping the twelve integer-returning ones
(`dds3GetRandIntA`, `effBTLModelGetID`, `scrGetIntPara`, `sceGsGetIMR` and
nine more). The pointer-variable row matches `(u32)<ident>` against the
identifiers declared with a `*` in the same file, which over-counts where a
pointer and a scalar share a name in different functions (`alpha`, `timer`,
`color` all appear that way), hence the upper bound.

## Class 2: implied sizes

A size or stride written as a shift or a literal where a `sizeof` is missing.
Smaller than it looks, because most shifts in this engine are not sizes.

| Pattern | Count | Verdict |
| --- | --- | --- |
| `<< 4` | 290 | Almost all GS 12.4 fixed-point coordinate scaling, not a size. Not a pointer assumption |
| `>> 4` bytes to quadword count | ~20 of 71 | The quadword is 16 bytes of DMA payload on any host, so a consumer does not redefine it. `SDF_QWORD_BYTES` and `SDF_DMA_QWORD_SHIFT` already name it at 18 sites; the rest are local byte counts (`sdf/sdfBillBoard.c:229`, `sdf/sdfModel.c:649`, `sdf/sdf3DPrim.c:278`) |
| `<< 7` as a structure stride | 3 | `(u8 *)aNode->tmPacket + (aId << 7)` and the pair at `sdf/sdfModel.c:1349-1350` are `sizeof(sdfModelTmImage_t)`, which is 0x80. Worth a `sizeof`, since the struct is C and a host could change it |
| `<< 7` otherwise | 18 | Fixed-point and bitfield packing. Not sizes |
| `+ 0x10`, `+ 0x180`, `+ 0x1a0` on a packet base | many | Byte offsets into a GS or VIF packet whose layout is hardware, fixed on any host. `sdf/sdfDraw.c:884-895` is the dense case. A name would document the packet, not a size |

## Class 3: pointer arithmetic on a cast

`(u8 *)p + n`, `dmaPacket + 2`, `header + 1`. A pattern match for a cast
followed by `+` finds 531 sites and `(u8 *)` alone appears 857 times. Almost
all of them step through a GS or VIF packet by a hardware byte offset, which
is right on a host as written. Converting them would add 500 macro calls and
name nothing a consumer needs.

Left alone deliberately, with one exception already covered above: where the
offset is a C structure's size rather than a packet layout, it belongs in
class 2.

## What was converted

`include/common.h` gains two macros, guarded and identity on target. They
live there rather than in `eeregs.h` beside `EE_PHYSICAL` because 60 units
need them and 305 of the 309 units already include `common.h`, where
`QWORD_ALIGNED` and `QUAD_ZERO` already serve as the same kind of
layout hook; `eeregs.h` itself includes `common.h`, so nothing is lost.

- `EE_ADDR(ptr)` is `((u32)(ptr))`, a pointer in the EE's 32-bit address
  representation.
- `EE_ADDR_PTR(addr)` is `((void *)(u32)(addr))`, the same address back as a
  pointer. It keeps the `(u32)` the sites spell, because dropping it would
  change the target conversion, not only the host one.

Converted, one commit each, each built to the retail SHA-1:

| Commit | Change | Sites |
| --- | --- | --- |
| `b0d54a0d` | `EE_ADDR_PTR` at every `(T *)(u32)` site, and the macros themselves | 16 in 9 files |
| `57f47f6f` | `EE_ADDR` composed under `EE_PHYSICAL` | 22 in 8 files |
| `6ad4aa90` | `EE_ADDR` in the sdf draw and packet units | 18 in 5 files |
| `7da4aeeb` | `EE_ADDR` for the light, fog and sound buffer pointers | 12 in 3 files |
| `d2a6f2a5` | `sizeof(sdfModelTmImage_t)` for the VU1 transform packet stride | 2 of 4 |

Two conversions needed their operand scope corrected before they would have
been right, and both were caught by reading the diff rather than by the
checksum: `EE_PHYSICAL((u32)ex + SDF_QWORD_BYTES)` in `sdf/sdfDraw.c:569` and
`EE_PHYSICAL((u32)cb->scratchBuffer + (cb->firstSlot << 13))` in
`sdf/sdfIpu.c:681` cast only the pointer, so wrapping the whole argument in
`EE_ADDR` would have turned a byte offset into typed pointer arithmetic. The
image still matched either way, because `EE_ADDR` is identity on target and
`ex + 16` is `ex + 256` only once a host widens it. A conversion of this
class can be wrong without the checksum saying so, which is the reason to
read each one.

## Sites that could not be converted

- **`sdfModelCopyStat`, `sdf/sdfModel.c:1349-1350`.** The two strides in the
  transform packet copy stay `(frame << 7)`. Replacing them with
  `sizeof(sdfModelTmImage_t)` changes two of that function's 154 words from
  `+0x198`, with or without an `(s32)` on the `sizeof`, so signedness is not
  the cause: ee-gcc 2.96 shares the one shift between the two addresses and
  will not share the multiply. The other two strides in the same unit, at
  `:381` and `:879`, take the `sizeof` and match. A source comment at the
  type records this.
- **`(u32)aParm->zBias` and its two neighbours, `sdf/sdf3DPrim.c:285-287`.**
  Not pointer casts at all: `zBias`, `sdfFaceDivSoften` and
  `sdfFaceDivBias` are `f32`, so these are float-to-integer conversions that
  happen to sit among packet-address stores. Left alone, and named here so a
  later sweep does not mistake them.
- **`(u32)fogColor`, `sdf/sdf3D.c:452`**, and **`(u32)function`,
  `sdf/sdfSound.c:1043`**, for the same reason: `s32` values, not pointers.

No site was left out because it broke the match except the one stride pair
above.

## What was deferred

- **The ~350 remaining `(u32)` packet-address casts in game code**, led by
  `event/evtEventViewerMenu.c` (74), `newbattle/nbMainProcess.c` (55),
  `model/mdlEffect.c` (33), `interface/itfTestMesManager.c` (19),
  `newbattle/nbDebugProcess.c` (17). One mechanical pattern repeated, mostly
  in debug and viewer units, and the macro they need is the one that now
  exists. Deferred rather than rejected: the diff is large enough to want its
  own review pass, and landing it here would teach nothing new.
- **Field offsets, quadword shifts, GS packet byte offsets and class 3**, for
  the reasons in each row above: they are already correct on a host.
