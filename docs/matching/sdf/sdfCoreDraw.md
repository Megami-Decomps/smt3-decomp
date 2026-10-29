# Matching notes: sdf/sdfCoreDraw

## sdfCorePacketGen (sdf/sdfCoreDraw, 0x944)

70 of 583 words match, first difference at +0x18: 2,332 bytes are emitted
against 2,372 native bytes, 40 uncovered. Production remains assembly-backed.
The frame (0x140), the save set ($16-$23, $30, $31, $f20, $f21), the control-block
and spill slots, the block layout and the per-region instruction counts for the
generated-geometry, kind-7, VU0-dispatch and plain-geometry regions are retail's.
The six existing C functions remain exact.

`corePktCb_t` ends at `backfaceSign`, 0xB0 bytes. `savedAttrBits` and the second
material-packet pointer are locals of this function, not members: retail's first
spill slot is 0xB0, which only a 16-byte-rounded 0xB0 control block allows.
The kind dispatch is an `if`/`else if` chain on the packet type, not a `switch`:
retail inverts both tests and falls through into each arm. The VU0 dispatch call
is written in both arms, so only the `jal` cross-jumps. The plain-geometry block
is physically last, after the point-buffer transform tail, which both other
paths branch over. The blend count and its entry index are one reused `u32`,
which is why retail stores the raw `extData[0]` word and then the masked low
half to the same slot. The skinned path's vertex pointer is the same variable
the plain path uses, and its per-node normal test is written inside the loop,
so the `andi` is hoisted into the preheader and duplicated on the empty-loop arm.

What blocks the match is one callee-saved assignment. The nine mutually
conflicting long-lived values are colored in `allocno_compare` priority order,
`floor_log2(n_refs) * n_refs * 10000 / live_length`, and take $16 upward. Seven
land on retail's register. The remaining two are swapped: `node` needs $18 and
the packet pointer $20, so `node` has to be colored first.

    tools/cc1probe.sh src/smt3/sdf/sdfCoreDraw.c --func sdfCorePacketGen \
        --alloc --no-dumps --cflag -DNON_MATCHING    # verify: OK

measures `node` at refs=6, live_length=75, priority 1600 and the packet pointer
at refs=5, live_length=56, priority 1785. `node` therefore needs a seventh
reference (priority 1866) or live_length 67, or the packet pointer needs
live_length 63. Its six references are the five `node->` reads retail performs
plus the parameter copy, and its live range is the CFG span from that copy to
the last `node->root` read in the three geometry arms. Reordering the
declarations in each arm leaves both allocnos bit-identical (same probe on a
scratchpad candidate through `--as-unit`), so neither number moves without
changing which fields the function reads.

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
