# fclRagInit.c

## ragBuildList2 and ragBuildList3 (372 bytes each)

Both list builders preserve the native four-entry initialization,
shuffle and selective random extension. Unsigned modulo produces zero
through three additions; the byte count sent to sort and returned is
four through seven. The reverse pick initialization moves an EE address
as an integer cursor, forming a valid byte pointer only for each of the
four actual writes. It does not form a pointer before the array.

The initialized work pointer is used to form the real output address
after the RNG and VLA declarations in source; removing the corresponding
empty operand allows the native save/setup sequence. The late loop uses
the original remainder as its bound. The real total and consumed count
byte keep separate short register phases at the sort call. The original
two-byte fixed-table stride and each lookup's order are unchanged.

The comparator's actual two read-only pointer arguments now match the SDK
sort callback type directly, and the declaration uses the SDK's unsigned
count and width. This removes the old incompatible function-pointer cast
without changing the symbol or any emitted pointer argument.
