# Battle filter state and helper menu

The mutable field-color record contains the original blend-mode word, the
512 by 448 rectangle, and initially idle source and target colors and frame
counters. The next word is a separate raw alignment seam. Typed consumers
still write the same fields and submit the same rectangle to the filter.

The debug helper uses the common 64-byte editor layout. Its five-item list,
title, and distance-update callback are the original three pointer slots.
The callback formally accepts the editor's parameter but does not use it:
both real call branches provide one argument, and the compiled callback and
all existing battle-effect owner code remain byte-identical.

The effects submenu is a separate 64-byte editor record. Its unchanged
raw item bank contains thirteen 24-byte rows. Generic submenu dispatch
reads its native title and item pointers; it has no update callback.

ShockWave02 uses one writable 84-byte thunder-poly parameter record, with
the original sixteen-byte vector alignment. The spawner writes the
endpoints and the constructor copies the complete record. Its last word
is a packed edge color, not a data-bank pointer. The neighboring raw
words are not part of the record.

## EFFECT editor items

The existing EFFECT submenu uses 13 native `effDebugMenuItem_t` rows at
`0x0031CC80`: four submenu links, six byte toggles, and three separators.
The first row's label points to `BED SELECT`. Its pointer bytes resemble
text to the reference splitter, so that one word uses a named relocation.
The submenu descriptor, its title, and the preceding raw descriptor stay
with their existing owners.
