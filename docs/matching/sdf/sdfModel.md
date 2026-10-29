# Matching notes: sdf/sdfModel

## sdfModelSetOneSkinDrawPacket (sdf/sdfModel, 0x180)

82 of 96 words match at the exact 384-byte size, first difference at +0x38.
Four source properties got it from 3 of 90 to here, and all four are worth
carrying to the sibling packet writers:

1. **`qwc` is the `packed` word narrowed in place**, not a separate `u16`.
   Retail loads the `packetSize`/`matIndex` word into `$22`, takes `matIndex`
   with `srl $5, $22, 16`, then `andi $22, $22, 0xFFFF` and keeps `$22` to the
   final `sh $22, 0($17)`. That is one variable with five references,
   `packed &= 0xFFFF;`, which is what wins `$22` over the mask in
   `allocno_compare`. With a separate `u16 qwc` the mask takes the saved
   register and the function comes out six words short.
2. **`ofs = aId << 7;` sits after the `sdfModelSetMatRefPacket` call.** That
   makes `aId`'s pseudo live across the call, so it gets `$16` and retail's
   `daddu $16, $7, $0` plus `sll $16, $16, 7` appear; the shift's result is
   spilled to `4($29)` after the call, which frees `$16` for the bone cursor
   `addiu $16, $30, 0x10`. Written before the call the shift is one word
   shorter, the spill lands in the call's delay slot and the bone cursor takes
   `$17`, rotating every later saved register.
3. **The mask is one local for the loop and the literal `0x0FFFFFFF` in the
   tail.** The local is spilled, so its constant is built through the slot
   (`lui`, `sw`, `lw`, `ori`, `sw`), and the literal gives the tail its own
   rematerialised `lui`/`ori` pair. One local for both sites reloads the tail
   copy with `lw 0($29)` and is a word short; two locals or a literal in the
   loop run over 0x180.
4. **The bone packet's address is a local assigned before the `dat0`/`dat1`
   stores.** A call flushes the scheduler's memory lists, so a store written
   before `sdfModelAllocTmPacket` cannot move after it. Retail stores `qwc`
   and `id`, calls, then stores `dat0`, `dat1` and `addr`, which only comes
   out of `addr = (u32)sdfModelAllocTmPacket(node);` as its own statement.
   That one change fixed the whole loop body, 13 words.

The residual is one allocation tie plus the tail ordering that follows it.
`aDataPtr` and the hoisted `0x6C07C000` constant are both `refs=5 live=64`:

    tools/cc1probe.sh <candidate> --as-unit src/smt3/sdf/sdfModel.c --version smt3 \
        --func sdfModelSetOneSkinDrawPacket --alloc --no-dumps --out <dir>   # verify: OK

    a1 {reg85  refs=5 live=64 calls=3}  pri=1562  => $23   (aDataPtr)
    a17{reg108 refs=5 live=64 calls=3}  pri=1562  => $fp   (0x6C07C000)

Equal priority, so `allocno_compare` decides on the allocno number, and the
parameter's pseudo is always lower than a constant cse creates. Retail has the
constant in `$23` and `aDataPtr` in `$30`, so retail's compile broke the tie
the other way: either `aDataPtr` has four references there, or the constant has
six. `refs` is frequency weighted (a reference at loop depth 1 counts twice),
which is how a two-insn constant with one use in the loop reaches five.

Measured and ruled out for the tie: the constant as a declared local, assigned
at the top, after the call and just before the loop; `0x6C07C000 | vuAddr`;
`u32 count`; a second pointer local for the tail read of `dataBody` (combine
propagates it back); and reading `boneCount` or `boneId` before the call.
Nothing moves either number. The last six differing words are the tail's
restore ordering, which follows the swap: with the pair the right way round
retail schedules `ld $16, 0x10($29)` into the slot after `lw $3, 8($30)`.
Moving `tag->dat0 = 0;` behind the `addr` store wins that one word by accident
(83 of 96) and is kept out of the draft as the wrong reason.

## sdfModelLinkDrawPacket (sdf/sdfModel, 0x208)

Eighteen of 122 words match, 488 bytes against retail's 520. The control
flow, the eight-entry jump table, the five arms, the packet-walk loop and the
`bnezl` backedge are all reproduced; the residual is four shapes, one of them
already on the ruled-out list.

1. Retail has `xori $3, $23, 0x0` in front of `movn $21, $2, $3` for
   `if (aMode != 0) { aOtIndex = packed >> 16; }`. That is the
   `xori reg, reg, 0` before a `movn` row in
   [matching levers](matching-levers.md), and this is a second instance of
   it. Twenty-three spellings were compiled and dumped in isolation: `!= 0`,
   plain truthiness, both ternary polarities, an `if`/`else`, `== 0` with the
   arms swapped, `(u32)` casts, `!!`, `^ 0`, `-0`, `> 0` unsigned, a
   one-case `switch`, a 0/1 condition local, a zero local that constant-folds
   (three ways), and `u8`/`u16`/`s8`/`s16`/`s64`/`u64`/pointer condition
   types. Every one emits the bare `movn`; the only thing that produces an
   `xori` is comparing against a **non-zero** constant (`mode != 1`), which
   is the wrong constant. The missing reference to `aMode` is also what
   rotates the saved registers: retail allocates `aMode` to `$23`/`$7` before
   `aIdx`, and the draft the other way round, so this one word drags the
   whole body.
2. Retail copies `sdfMemLock`'s result into `$6` and addresses the header
   stores off the copy; the draft uses `$2` directly and is one word short.
3. Retail gives cases 4, 6 and 8 their own three-instruction block that
   branches forward into the 5/7 arm's `sdfModelNewPacketHeader` call, where
   the draft lets gcc cross-jump the two argument setups into one block, four
   words short.
4. The remaining words are the argument-setup order inside each arm.

Also worth knowing before picking it up: compiling this function unguarded
makes its neighbour `sdfModelGetDrawPacketSize`, which matches on its own,
stop matching. Parking the draft restores it, so an attack on this function
has to recheck the whole unit, not just this line.

## sdfModelLinkShapedDrawPacket (sdf/sdfModel, 0x258)

Fifty of 150 words match at the exact 600-byte size, and the mnemonic
sequence is identical apart from two alignment `nop`s and one saved-register
save/restore pair: `diff` over the opcodes alone leaves the prologue's
`li`/`sd` ordering, two `lui`/`addiu` pairs that swap places inside the
morph scan, and one extra `ld`.

The whole residual is that retail fits the function's long-lived values into
**seven** saved registers and the draft needs eight, which moves the frame
from 0x50 to 0x60 and renames every register after it. The seven values are
`used`, `aPacket`, `aNode`, `listCount`, `matIndex`, `aIdx` and `aTag`;
retail gets away with seven because the packet buffer shares `aNode`'s
register. It does that by splitting the buffer's live range: the
`sdfGrAllocPacketBuff` result goes into `$6`, the third argument register,
`lw $4, 0x10($18)` is `aNode`'s last use, and only then does
`daddu $18, $6, $0` in the `jal`'s delay slot put the buffer into the
register `aNode` has just vacated. The draft instead copies the result
straight into a saved register before `aNode` dies, so the two conflict.

Ruled out, all measured here: a second local copied from the buffer
(assigned right after the allocation, and again immediately before the
call), the two locals with their roles reversed, a `u8 *` second local, the
allocation written inline as the call's third argument (with and without a
second local), a `(sdfDmaTag_t *)` cast on the argument, the buffer declared
`sdfDmaTag_t *` and `u8 *`, the buffer declared first and last among the
locals, `tag = buf;` before the call, a `(void *)` cast at
`sdfDrawTagLink2`, `(u8 *)buf + 0` at the same place, reassigning the
`aNode` parameter to the buffer after the call and reassigning `aPacket`
instead, a `root` local for the second `aNode->root` read, and
`packed >> 16` written at the call instead of a `matIndex` local. Every one
of them leaves eight saved registers; gcc coalesces the two locals in every
spelling.

What the draft does get right, and is worth keeping even if the register
question is never solved:

- The weight epsilon at `0x0054E744` is a `.lit4` pool entry, not a global:
  it is `1e-6f` (`0x358637BD`) and has to be written as the literal.
- The header quadwords are a unit-local struct,
  `sdfModelShapedHeader_t { sdfDmaTag_t dmatag; f32 weight[2]; u32 code0;
  u32 code1; }`, reached by casting the running `sdfDmaTag_t *`. Indexing
  `weight[k]` through the struct is what leaves retail's `+0x10` in the store
  displacement after the `addu` of the index; an `f32 *weight = (f32 *)(tag +
  1)` local folds the `0x10` into the base instead and costs the second
  zero-fill loop its shape. Advancing the one `tag` variable by `tag += 2`
  rather than keeping a separate header pointer is what gives
  `addiu a2, a2, 32` in place.
- The shape entry needs the operand-order cast,
  `(sdfModelShapeEntry_t *)((u8 *)(out[m].targetId * 8) + (u32)list)` read as
  `entry[1]`, so that retail's `addu rd, index, base` and the `8`/`0xC` load
  displacements both fall out. `list->entry[id]` hoists `list + 8` out of the
  inner loop, and writing the subscript at both field reads builds the
  address twice.
- The tag word is `(qwc | 0x30000000) | ((u64)(addr & 0x0FFFFFFF) << 32)`
  with the `qwc` group written **first**; the other grouping swaps the two
  registers at the final `or`.

## sdfModelTempPacketLinkOt (sdf/sdfModel, 0x468)

Two hundred twenty-seven of 282 words differ positionally, at 281 emitted
words against 279 native; aligned on the instruction stream the distance is
one hundred twenty-four of 279, of which fifty-nine are the same instruction
with one register renamed. The draft's body is believed right: the node walk,
the format-1 shape list, the screen transform, the bounds reject, the four
draw-tag slots, the 0xC0 temp packet and the ordering-table flush all appear
in retail's order with retail's operands.

The function shares almost its whole skeleton with `sdfModelLinkOt` below,
which is at 61 words: the prologue (frame id, `sdfMatListUpdatePacket`, the
light and fog defaults, the face-division terms), the per-node transform and
`sdfModelCheckBounds`, the local-colour select, the `drawAttr` mask, the
four-slot clear loop, `sdfCoreDrawSetZBias`, the `movz` on a zero
ordering-table index, the draw-parameter packet and the flush loop are one
source template in both. Everything learned on one carried to the other, so
fix them together.

What is not established, and is the whole residual here:

- Retail holds nine values in saved registers across the node loop: `aRoot`
  in `$23`, the node, the matrix pointer `node + 0xC0` in `$30`, the shape
  list cursor, the header cursor, the draw tag, `attr`, the packet and the
  `0x0FFFFFFF` mask. The draft needs ten, so `aRoot` takes `$30`, the matrix
  pointer spills to the frame instead, and every saved register after it is
  renamed. Which tenth value the draft is carrying is not identified.
- `otIndex` comes from `lw` of the header's first word plus `srl 16`, where
  the draft emits `lhu` at +2. gcc narrows a word load whose only use is
  `>> 16`. Eight spellings ruled out here: `*(u32 *)header >> 16`, through a
  `u32` local, `((u32 *)header)[0] >> 16`, a `u32 info` struct member,
  `*cursor++ >> 16`, `/ 0x10000`, `(u32)*(s32 *)header >> 16` and
  `(x << 0) >> 16`. In a small test function `(w & 0xFFFF0000) >> 16` does
  keep the word load, but in this function it is narrowed anyway; a second
  live use of the low half is the only form that held.
- The sign test on the colour is `slti`, not `srl 31`; see the shared
  dead end in the `sdfModelLinkOt` entry.

Also corrected while drafting: `sdfDrawTagLink2` takes three arguments
(tag, packet, range tail), not two. The unit's old two-argument `extern` is
replaced and `sdfModelLinkShapedDrawPacket`'s draft call above now passes
the final tag as the third argument; that draft's own word count is
unchanged.
