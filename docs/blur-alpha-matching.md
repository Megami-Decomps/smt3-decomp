# Blur alpha packet matching

`dds3EffectBlur_AlphaMultiParmSet` is 180 retail bytes at `0x001A18F8`.
The existing project draft already recovered the four vertices, corner scaling,
texture coordinates and ADC fields. Its only semantic-source change is reading
alpha from the existing packed color member:

```c
pSpriteParm->a = pBlur->abgr >> 24;
```

`dds3Effect_Blur_t::abgr` is already a `u32` at offset zero. The native unsigned
byte load at offset three is the upper eight bits of that little-endian word.
No type, layout, signature or store-order change is needed.

## Compiler evidence

The real project-patched ee-gcc 2.96 compiler expands the old `u8 *` access as
`mem:QI` in alias set 0. The packed access begins as `mem/s:SI` in integer alias
set 4, then narrows to the same unsigned byte load while retaining alias set 4.
The latter conflicts with the packet's integer stores, but not its float or
halfword ADC stores. This permits the native interleaving; the old character
access creates extra scheduling dependencies. Both forms emit 45 words, but
only the packed-member form reproduces all 45.

Output-parity qualification used both source forms in the complete
`effect/effBlur_Filter.c` context:

```sh
SMT3_AS_UNIT=src/smt3/effect/effBlur_Filter.c SMT3_KEEP_S=plain.s \
  tools/cc.sh -DSKIP_ASM candidate.c -o plain.o
SMT3_AS_UNIT=src/smt3/effect/effBlur_Filter.c SMT3_KEEP_S=dump.s \
  tools/cc.sh -DSKIP_ASM -da -dumpbase alpha candidate.c -o dump.o
```

For each form, all assembly after the randomized first `.file` line is identical
between the plain and dumped compile. In the packed form's initial RTL, UID 29
loads the word and UID 31 extracts alpha. In its final scheduling dump, UID 31
is the narrowed `mem/s:QI ... 4` load. The old form's UID 29 remains
`mem:QI ... 0`. This is ordinary unhooked RTL evidence, not a hooked scheduler
trace or a new compiler modification. The compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.

The complete owner, including the 144-byte caller and data, matches. The separate
`dds3EffectFilterDrawTexQuad` fallback is not changed: extracting its existing
union's `rgba` member left its old 22-word residual intact.

The complete retail image checksum also passes:
`5facfbf7916836613449c922e26a4ec412207b32` (SHA-1).
