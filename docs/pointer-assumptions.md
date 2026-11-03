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

`include/eeregs.h` gains two macros, guarded and identity on target:

- `EE_ADDR(ptr)`, a pointer in the EE's 32-bit address representation.
- `EE_ADDR_PTR(addr)`, the same address back as a `void *`. It keeps the
  `(u32)` the sites spell, because dropping it would change the target
  conversion, not just the host one.

Converted, one commit each, each verified byte-identical:

1. `EE_ADDR_PTR` at the 16 `(T *)(u32)` sites.
2. `EE_ADDR` composed under `EE_PHYSICAL` at the 22 `EE_PHYSICAL((u32)p)`
   sites.
3. `EE_ADDR` through the `sdf/` core units.
4. `sizeof(sdfModelTmImage_t)` for the three `<< 7` strides.

## What was left, and why

- **The ~350 remaining `(u32)` packet-address casts in game code**, led by
  `event/evtEventViewerMenu.c` (74), `newbattle/nbMainProcess.c` (55),
  `model/mdlEffect.c` (33), `interface/itfTestMesManager.c` (19),
  `newbattle/nbDebugProcess.c` (17). One mechanical pattern repeated, mostly
  in debug and viewer units. Deferred rather than rejected: the diff is large
  enough to need its own review pass, and the macro is the same one, so
  nothing is learned by landing it here.
- **Field offsets, quadword shifts, GS packet byte offsets and class 3**, for
  the reasons in each row: they are already correct on a host.
