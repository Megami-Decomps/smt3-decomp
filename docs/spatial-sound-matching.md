# Field spatial sound — 816 exact bytes

`field/fldSe::fldPlaySe3D`, `0x001448E8..0x00144C18`, is ordinary C matching all
**816 instruction bytes**. It depends on the separately qualified
[sound stop payload/API correction](sound-stop-contract.md), which earns no
additional matching credit.

## Source provenance and title differences

Adapted from the qualified DDS `func_001415F8` in
[code_001411F0.c](https://github.com/Megami-Decomps/dds-decomp/blob/3419648e1c2ef3c9acbc55f149dd20c03a163d6c/src/dds1/game/code_001411F0.c),
commit `3419648e1c2ef3c9acbc55f149dd20c03a163d6c`. Credit: Megami-Decomps DDS
reconstruction / redthing1. The native SMT3 routine, not the donor's extra
behavior, governs this adaptation.

SMT3 always derives its listener from the camera, clamps horizontal distance
to 200, and retains the near-point Y coordinate. It omits DDS's alternative
camera-mode branch, vertical-distance rejection and stage-11 event remap.
Both source loops use a fixed 500-unit radius margin. Native loads that
constant independently at `0x001449E4` and `0x00144A98`; the donor's
camera-dependent `extraRadius` local is therefore unnecessary and removed.
This is a title-specific constant, not an artificial lifetime constraint.

The first pass determines whether any source is nearby. On odd ticks it sends
the listener and current near-point coordinates to the real six-float
`p2sdSetStandPos` provider. The second pass preserves the start, still-active
and stop paths, source flags and coordinate stores. Each play/stop resolves
the current area; callback-sensitive area and source-count reloads are not
replaced with the donor's cached stage.

## Storage, arithmetic and provider audit

- Existing source records retain their native 24-byte layout: flags, sound ID,
  radius and XYZ. Existing 20-byte BGM rows supply the current area's `smgId`.
- `p2sdPlaySe3DEx(s32,s32,f32,f32,f32)` receives the track, radius divided by
  100, and three coordinates. Its actual provider queues those fields and
  scales coordinates; no angle/pan behavior is invented.
- Camera declarations remain compatible with `fldCamera`'s real float arrays.
  No new array extent, overlapping alias or callback type is introduced.
- Native copies 16 bytes from `D_00532110`, the four floats `{0,-100,0,1}`
  ending at `0x00532120`, into a local position. The ordinary four-float
  aggregate preserves the full copy and native XYZ stores, including the
  still-active path. No padding, special alignment or forced consumer is used.
- `D_0054EE34` and `D_0054EE38` are zero-initialized word counters referenced
  only by these increments. There is no signed consumer or reset bound.
  Both counters and the tick local are `u32`, giving defined wrap matching
  native nontrapping increment/store behavior.
- The radius is supplied unchanged by the FL2 sound record through
  `fldAddSe3D`. No explicit finite/range validation was found. The source
  preserves the native radius conversion and valid-resource assumptions;
  it does not invent a bound, clamp or validation absent from retail.

## Qualification

After the zero-credit prerequisite, the complete field owner is **22 match,
0 differ**, including initialized data and all existing C peers. The other
five prerequisite owners remain exact and affected guarded drafts compile.
A second `configure.py --force-split` and full `ninja` independently pass the
same 4,551,696-byte retail image, SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. Compiler bytes and options are
unchanged. No unrelated function or prerequisite is included in the 816-byte
credit.
