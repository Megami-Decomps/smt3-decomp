# Event-unit visual transition matching

`UnitAnime` is 3,920 retail bytes at `0x001E2248`. It adapts
`evtAdvanceUnitVisualTransitions` from Megami-Decomps/dds-decomp (redthing1),
`src/dds1/event/evtUnitManager.c`, working snapshot based on
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. The donor is 3,968 bytes.
SMT3 lacks DDS's final `evtFindUnitSlotAuxCoordinates` branch; it applies face
division parameters only for the native target-controlled case. That absent
call and branch are not reconstructed here.

The driver advances model RGB/alpha tracks, validates current and previous
light targets, blends directions and colors, and updates face subdivision
parameters. The existing `evtUnit_t`, `evtTargetInfo_t`, scene-basic, pose,
model and light-parameter records cover every native access. Its legacy integer
transition-reference fields are converted to `dds3Basic_t *` in the same way as
the already-matched sibling light accessors; no new overlay or alias type is
introduced. Actual scene membership, model-position, mark-position and
face-division provider definitions establish the declarations and arities.

## Existing VU/MMI contracts

No shared header or macro changes are needed. In particular:

- The six four-float vector buffers are explicitly 16-byte aligned and occupy
  native offsets `0x00` through `0x50`. Identity is fully initialized by
  `memset` and a W-component store. The remaining vector slots are produced
  and consumed through the existing VU register pipeline, not C scalar reads.
- Existing `SDF_VU0_STOREVECTOR_NOMEM` hand-offs feed later VU loads only. The
  target direction vector occupies its own `0x10..0x20` record range, separate
  from the scalar flags, distances and auxiliary parameters. No stale cached
  C vector elements are read after these stores. This audit does not turn the
  NOMEM primitive into a general memory barrier.
- The twenty four-word packed-color carriers occupy native `0x60..0x190`
  slots. `EE_MMI_RGBA_UNPACK_READONLY` declares the actual first-word read with
  an input memory operand; the source color stores therefore remain live.
  Packed results are read by the following C color assignments, even when the
  compiler forwards those reads. No unread output store or padding object is
  added. This is the unchanged donor's SDK-style color workspace.
- `MMI_RGBA_PACK_UNIT` has the same early-clobber output and GPR scale input as
  the donor. Existing `SDF_VU0_SCALEVECTOR_BITS` and its B-register counterpart
  specialize the donor's register-selected scale to vf10/vf11 without changing
  operands. Actual float values pass through their GPR constraints as binary32
  bits; there is no integer numeric conversion or extra transfer temporary.
- Full four-lane direction transforms, explicit color W-lane zero/one operations,
  target/current/previous weights and all native scalar callback boundaries are
  retained. No basic/extended asm boundary is replaced to steer scheduling.

The resulting native frame is `0x210`, with `0x1A0` bytes of real workspace,
followed by saved registers and alignment. No artificial aggregate explains it.

## One bounded initialization-order correction

The initial native-adapted donor reproduced 978 of 980 words. Only the initial
`previousWeight` zero and `sdfFaceDivSoften` load exchanged places at `+0x60`
and `+0x68`. Initializing the previous-target weight before reading the two
face-division defaults closes that residual; no other expression or order was
changed. These are the same native initial values and ordinary independent
initialization statements, not a register pin or dependency between them.

Both forms were compiled plain and with `-da` in the complete owner. Their plain
and dumped assemblies are identical after the randomized first `.file` line.
The selected initial RTL records the zero as UID 47 followed by default loads
UIDs 50/53, rather than the donor order of loads 47/50 followed by zero 53.
In `25.sched2`, the matching form places zero UID 47 at cycle 17 and the soften
load UID 50 at cycle 19; the donor form schedules its soften load at 17 and zero
at 20. The final emitted words reproduce the native placement. This is ordinary
output-parity-qualified RTL evidence, not a hooked compiler trace.

## Qualification

The complete event-unit owner now has 46 matching functions and no assembly
fallbacks. In particular, the newly published 1,560-byte motion driver remains
exact when this local callee becomes C. Production-context/data checks, targeted
draft compilation, and the full `ninja` checksum build pass. Retail image SHA-1:
`5facfbf7916836613449c922e26a4ec412207b32`.

```sh
python tools/check_unit.py src/smt3/event/evtUnitManager.c -v
python tools/check_nonmatching.py src/smt3/event/evtUnitManager.c
ninja -j4
```

The compiler is unchanged, SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
