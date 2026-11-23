# Polygon movie frame callbacks

### Native frame callback bank

The writable 96-byte bank at `0x0032C790` has 24 function-pointer slots.
`FrameRun` indexes it by `ObjectType`, not by a camera-specific index.
Types 0 and 4 have null slots; types 20 and 21 both select `MG12FrameFunc`.
The IDB name `dds3FrameFuncTable` marks the type-5 suffix. No native
reference needs a separate linker symbol or allocation there. The 12 bytes before the bank remain separate padding.

The dispatcher calls the slot with work, current and next frame records,
and frame number; it does not inspect the return register. The only native
references to the quake, fade, light, BGM, and Bokasi callbacks are their
bank entries. All five callbacks have void contracts and no C return value.
One-pass blocks retain their regular calls and epilogues: changing just the
return types triggers EE sibling-call conversion. The BGM block begins at
the command switch, after the frame check, argument snapshot, and log call.

### Interpolated frame storage

The movie callbacks write eight zero-initialized scene/effect parameter
objects in the native BSS interval `[0x56CFF0, 0x56D148)`. The shared
declarations use the callback's existing lighting array and fog, blur, and
filter types. The 0x158-byte interval includes 0x20 bytes of alignment
between the typed parameters. The first object is the twelve lighting
floats consumed as three groups of four; the 0x18-byte filter ends where
the separate event-pack script path starts. The neighboring event-viewer
ripple object and event-pack path stay in their original raw ranges.

The storage definition does not change the consumer instructions, data,
or relocations. The linked image remains byte-identical, and the interval
is NOBITS rather than initialized data.

The five interior objects that end before the next aligned address have
explicit sizes in the hand-curated symbol inventory. Their alignment
bytes are not part of the C types.
