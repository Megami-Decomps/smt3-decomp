# Battle help message pointer pair

The two native pointers at 0x5263F8 refer to the existing encoded
messages at 0x545868 and 0x545850. The pair ends at 0x526400, where
battle formation vectors start. The help dispatcher indexes the pair
and passes the selected string to the read-only renderer through its
legacy mutable-pointer API. Keep the pointed-to data const.

In both stock compiler modes, removing the table's explicit placement
puts its eight bytes in .sdata rather than the native writable .data.
The retained preceding data has no relocations. The new pointer pair
emits the same two named 32-bit relocations as the original.
