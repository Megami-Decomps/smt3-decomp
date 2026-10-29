# Matching notes: sdf/sdfCoreDraw

## sdfCorePacketGen (sdf/sdfCoreDraw, 0x944)

543 of 575 words differ in the emitted prefix: 32 words match, 2,300 bytes
are emitted against 2,372 native bytes, and 72 bytes are uncovered. Production
remains assembly-backed. This is a broad register-allocation and code-layout residual,
not a 72-byte-only frontier. The six existing C functions remain exact.

The driver now uses the shared model, material and generated-packet headers.
The third parameter is the attribute mask passed to `corePktInitCb`; retain it
between the packet pointer and color. Frame and reverse are the final integer
parameters, after the two floating-point subdivision parameters. The result is
the packet pointer returned by `corePktClosePacket`.

- Kinds 7 and 8 use the VU0 generated-geometry path. Kind 7 reads a halfword
  node-index list, uploads seven quadwords from each selected node's buffered
  transform, and uploads the screen, light, material and subdivision data.
  Its input header is `12 * nodeCount + 0x14` bytes and its point stride is
  `28 * nodeCount + 8`. Kind 8 composes the node/screen transform, saves the
  matrix and uses header/stride 0x1C/0x20. The shared dispatch returns the input
  cursor advanced by `corePktVu0Run`, then places optional secondary UVs and
  vertex colors before opening/emitting/closing the draw packet.
- Other kinds prefer BLENDVERT over BLENDGRP, then the plain vertex path.
  Skinning consumes one aligned weighted-position stream per indexed node,
  optional aligned normals, First/Accum helpers and final normal normalization.
  It uses the screen/light registers directly, without composing a node matrix.
- Morphing reads two targets from the selected 16-byte morph entry. Both the
  entry/count and target/bound comparisons are signed. Weights must exceed
  `0.000001f`; the target's delta begins one aligned base-stream stride beyond
  its indexed position. Null or out-of-range morph entries return NULL before
  transforming, emitting or closing anything. The serialized cursor advances
  by the low-halfword blend count times the aligned stride, doubled for normals.
- Plain geometry transforms directly from its serialized vertex/normal streams.
  All ordinary paths preserve the optional face, UV and secondary-UV spans and
  select the textured helper from the saved masked attribute bits.

The UV upload primitive deliberately stores a 64-bit GPR payload with `sq`.
Native loads each packed float pair with `ld` and stores a full quadword; a C
128-bit conversion inserts extra moves and promises upper lanes that the
native pair does not set.
The uploaded upper lanes are not a promised sign or zero extension. In
`src/smt3/sdf/vu/vu0_micro.vsm`, entry 001C loads only XY and computes the W
lighting terms separately. Entry 008C loads XYZW, but its UV operations at
00A4/00A7/00A8 use only XY. Both consumers therefore use the two packed floats.
The three destination quadwords are VU0 data addresses 0xE6 through 0xE8.

A qualified real-unit compiler trace preserves hooked/unhooked and dumped/plain
output parity. The switch's shared microprogram dispatch and forward ordinary
geometry path retain the native control distinction without changing state.
No register pins, artificial dependencies or compiler-option changes are used.
The control-block member offsets remain those in `draw3d.h`; this reconstruction
does not establish a new sizeof or array stride for that stack-only block.
