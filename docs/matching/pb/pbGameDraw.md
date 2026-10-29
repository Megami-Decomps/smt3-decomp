# pb/pbGameDraw

## pbRebuildOutline

96 of 98 instruction words match at the native 392-byte size. The two
remaining differences, at +0x4C and +0xDC, commute the operands of a block
address addition. Frame, loops, calls, vertex accesses and all other words
match. The default build retains assembly.

Each 0x90-byte block contains an outline count at +6 and eight four-float
vertices at +0x10. Four edge bits append two vertices apiece from the existing
four-row template at `D_00327780`. X and Z receive the signed block coordinates
scaled by 40; Y is copied and W is untouched. The block base is reloaded after
`pbGetBlockAt`, and the outer loop rereads the group count.

The earlier 380-byte reconstruction and its integer-address casts are
superseded by the guarded typed array reconstruction. Writing the actual
block lookup as index-first typed pointer addition instead of array indexing
is compiler-inert: the same two additions still commute. No extra address
lifetimes, qualifiers or allocation constraints are introduced. Recover the
reason for the native induction-address ordering before retrying this pair.
