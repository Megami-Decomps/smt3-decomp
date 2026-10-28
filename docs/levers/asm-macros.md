# Reaching an instruction cc1 never selects, and what it costs

`addi rd, $0, imm` in a retail body means the source was assembly: ee-gcc 2.96
materialises a small constant with `addiu` only. The instruction is still
reachable from C through `include/ps2asm.h`, the same way every VU0 and MMI
match in the tree works, and `sdf/sdfTex`'s `sdfTexSetPacket` is the first
place that was tried.

`PS2_WORD_BIT_LENGTH` is the result. The retail idiom is four instructions,

    plzcw   $2, $5
    andi    $2, $2, 0xFF
    addi    $5, $0, 0x1F
    subu    $5, $5, $2

the bit length of a word, used on `dimension - 1` to get `log2`. The `addi`
writes the register the input arrived in, so the value and the scratch have to
be one in/out operand and one early-clobber output:

    #define PS2_WORD_BIT_LENGTH(x) ({ s32 ps2Bits = (x); s32 ps2Lzc; __asm__ ( \
        ".set noreorder\n\tplzcw %1, %0\n\tandi %1, %1, 0xFF\n\t" \
        "addi %0, $0, 0x1F\n\tsubu %0, %0, %1\n\t.set reorder" \
        : "+r" (ps2Bits), "=&r" (ps2Lzc)); ps2Bits; })

**The register binding works.** cc1 emits the four instructions contiguously,
in retail's order, with the same two-register roles, at the point in the body
where the C puts them, twice in one function from one macro. Nothing about
`addi` or `plzcw` makes a function unmatchable.

**What it costs is the schedule.** cc1 treats the whole block as one insn with
unit latency, so around it the ready-list priorities are those of a
four-instruction dependency chain collapsed to one. In `sdfTexSetPacket` that
leaves the body the right length with the right dataflow and the tail exact,
and the first fifty instructions in a different order from the hand-written
routine. Leave the block whole: splitting it into one-instruction macros is
worse, because two `addi %0, $0, 0x1F` blocks with no inputs are identical
patterns and cc1 CSEs them into one.

So a body with one such idiom in an otherwise ordinary function is a normal
match; a body where the idiom sits in the middle of a long dependency graph
may come out the right size and still not close.

## A `|` chain's tree shape comes from the term order, not from parentheses

Not specific to asm macros, but it is what the rest of `sdfTexSetPacket` turned
on. cc1 reassociates a chain of `|` into a bottom-up pairwise tree: pair
adjacent terms, pair the pairs, and append whatever is left over. The shape is
a function of the order the terms are written in and of nothing else.
Parentheses in the source do not survive, and the declaration order of the
locals holding the terms has no effect at all (300 permutations, byte-identical
output).

That makes the order recoverable: compile every permutation of the terms and
compare the tree. For the eight fields of a GS `TEX0`, 96 of the 40320 orders
give retail's tree and 4 of those also give the tree of the second expression
in the same function. All of them are the GS fields written most significant
first, which is what the retail source did.
