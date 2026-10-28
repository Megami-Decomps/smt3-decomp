# Non-matching drafts: kernel/dds3CreateData

## stfLoadUITexture (kernel/dds3CreateData, 0x8C)

12 of 35 words differ, first at +0x4; the natural signed-remainder draft
has the exact 140-byte size. Retail preserves the full field index and a
four-slot remainder while selecting texture input arrays at +0x24, +0x44,
and +0x90 and storing the returned handles at +4, +8, and +0xC.

The font cleanup loop proves four allocations at +0x14 and +0x34 and
fifteen at +0x54; their paired texture input arrays are exposed in the
work layout. The cleanup function matches with those ordinary arrays.
Spelling out signed division's bias/shift lowering reproduced this loader's
instructions, but does not establish a source-level reason to avoid `% 4`,
so that form was discarded. The unresolved question is which truthful source
layout or value lifetime gives the natural remainder its retail allocation.
