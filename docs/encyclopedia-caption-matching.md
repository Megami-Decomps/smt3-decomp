# Encyclopedia read-list caption renderer

`facility/fclEncyc::DrawReadListwinCaption` at `0x0021C928` reproduces all
1,576 retail instruction bytes. The reconstruction uses the existing matched
`drawRegistSprites` vertex/UV records and primitive contracts. No new type,
helper, compiler lever or shared-header change is required.

## Native algorithm and providers

The renderer allocates and initializes one 32-byte draw tag. Four panel bounds
come from the four real eight-byte `{left, right}` records at `0x00332388`:
`(1552,2512)`, `(2624,4544)`, `(4656,5136)`, `(5696,6992)`. Their common vertical
bounds are 944 and 1016. The first three panels use the four-word color at
`0x00332368`, `(0,0,0,64)`; the fourth uses `0x00332378`, `(47,31,31,128)`.
These remain references to their original data, not reconstructed duplicate
constants or a flattened cursor over a typed object.

Two actual local vertices and one four-component UV record are reused for
sixteen textured sprites. Unchanged coordinates are deliberately retained
between calls, exactly as the native stores do. Each call sees initialized
values in every component that it consumes. Five calls bind a texture through
`itfMiscDrawSpritePrim`; the eleven `itfMiscDrawSpritePrim2` calls retain that
texture. The five bindings are `pinst->texHandle[0]`, `[1]`, `D_00345D88[1]`,
`pinst->texHandle[2]`, `D_00345D88[3]`, in native order. The complete sequence
is sixteen textured sprites plus the four untextured panels.

The real providers in `interface/itfMisc.c` read two eight-byte XY vertices and
four 32-bit UV/color components, emit their GS packet and link it to the tag.
The bound-texture provider has seven arguments; the retaining provider has six;
the plane provider has five. The existing owner declarations match these
contracts. The `fclEncycSetTexHandle` setter fills all seven texture handles in
the instance; `cmpTexLoad` in `code_0022C578.c` populates the separate camp handle
array, including the two referenced indices. These handles and the initialized
OT callback are existing live-renderer preconditions, not newly checked inputs.
The final call is the real `D_002F67F8.regFunc(&D_002F67F8, tag)` dispatch at
`0x0021CF18`; `DrawPhaseReadTop` is the typed caller.

## Provenance and title-specific behavior

The retail PS2 instructions and data are authoritative. The independent HD
reference `smt-ret/smt3-reconstructed-sources`, commit
`cdcd3e5874d72e63afae6e771e3b2d8f85cf66b1`, `src/fclEncyc.c`'s
`DrawReadListwinCaption`, corroborates the panel loop, sprite sequence, partial
coordinate updates and texture ownership. Its transpiler output and altered
HD coordinates were not copied as an ABI or numeric contract. For example,
the first sprite begins at native X `0x690`, and the following sprite spans
`0xCA0..0xFA0`; the HD reference instead uses 1696 and 3024..3792. Existing
same-owner renderer types and provider declarations are credited to the
project's prior contributors; this change replaces only the caption fallback.

## Qualification

Baseline: public `e9dfd7c52f2e7afd46ecfbb1d005e2eb914432eb`.
The first ordinary reconstruction was already exact, without source searches,
register constraints, dummy storage, new aliases or compiler-option changes.
The unchanged project compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.

- Complete normal owner: **108 match, 0 differ**.
- All **three** retained non-matching drafts compile.
- `configure.py --force-split`, then `ninja`: full retail image check passes.
- Original and built image SHA-1:
  `5facfbf7916836613449c922e26a4ec412207b32`.

Commands run in the isolated tree with the established QEMU i386 compiler
launch transport:

```sh
python3 configure.py --force-split
ninja -j12
python3 tools/check_unit.py src/smt3/facility/fclEncyc.c -v
python3 /tmp/smt3-check-nonmatching-transport.py src/smt3/facility/fclEncyc.c
sha1sum orig/smt3/SLUS_209.11 build/smt3/SLUS_209.11
```

The draft launch wrapper only substitutes the already-qualified i386 loader
transport; it leaves compiler bytes and arguments unchanged. The draft gate
checks compilation, not exactness of deliberately retained non-matching bodies.
