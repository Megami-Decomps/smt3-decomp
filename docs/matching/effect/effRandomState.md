# Effect random state

The 16 bytes at file offset `0x217FE0` are one mutable generator state,
`dds3EffectBase`. `dds3EffectManagerInit` supplies its base to
`sdfRandomizeByTime`, and the clock seeder writes four consecutive
32-bit words. The effect and battle callers pass the same base to the
integer and floating-point RNG providers; the integer provider updates
those same four words. This is not the fallback RNG selected by a null
argument and does not overlap the particle RNG.

The 16-byte zero initializer is file-backed `.data`. The one-record
C array keeps the existing callers' base-address argument spelling
while sharing the provider's `sdfRandCB_t` type. The new data object
has no relocations. Consumer compilation preserves the original
allocated sections, ordered relocations and native symbol slots.
