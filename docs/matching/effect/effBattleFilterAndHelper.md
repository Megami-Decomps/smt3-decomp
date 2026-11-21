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
