# Matching notes: effect/effPacket

## Default packet triangle indices

The initialized fallback is 32 native packed triplets over consecutive
vertices, not the old 68-byte inferred fragment. Audited default-mode
draws use at most the first 16 faces; all 32 initialized words belong to
the same data object.

Twelve packed triplets can resemble addresses to the reference splitter.
The builder reads their bytes as triangle indices, and the native raw
object had no relocations. The per-site annotations apply only to those
words. The following scale-frame data is unchanged.
