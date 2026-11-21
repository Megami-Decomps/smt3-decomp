# Name-entry sprite resources

## The first two texture-rectangle banks

**Unit:** `nme/nmeSpriteResources0`, `nme/nmeSpriteResources1`. **Size:** 868 bytes.

The first two entries of `GBWK_2` pair their resource arrays with 74 and
27 position entries. The native position entries address resource IDs
0 through 44 and 0 through 16, respectively. Each resource uses the existing
14-byte `fclSpriteRes_t` layout: a signed texture selector, one alignment
byte, and six unsigned halfwords of rectangle data.

The 630-byte and 238-byte objects stop before a separate two-byte zero gap on
each page. The stock disassembler rounds an exact halfword-ending span down
to words. Curated symbol extents and the existing native-data-tail recovery
keep those halfwords in the checked references without crediting either gap.
The source definitions add no relocations and do not change the existing
page descriptors or their pointers.
