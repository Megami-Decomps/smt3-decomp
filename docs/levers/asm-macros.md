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

the bit length of a word, used on `dimension - 1` to get `log2`. The shared
SDK operation has one real input/output value and the literal `$2` scratch
clobber used by the repeated native idiom:

    #define PS2_WORD_BIT_LENGTH(x) ({ s32 ps2Bits = (x); __asm__ ( \
        ".set noreorder\n\tplzcw $2, %0\n\tandi $2, $2, 0xFF\n\t" \
        "addi %0, $0, 0x1F\n\tsubu %0, %0, $2\n\t.set reorder" \
        : "+r" (ps2Bits) : : "$2"); ps2Bits; })

This is the established `EE_MMI_BIT_LENGTH` contract in the canonical DDS
[inline-assembly policy](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/idioms.md#inline-asm-cop2-and-mmi),
not a target-local register workaround. Independently of the movie body,
SMT3 `sdfTexSetPacket` at `0x002AB918` and `0x002AB934` uses the same literal
scratch, as does `release_movieobj2` at `0x002C5238` and `0x002C5258`.
The macro is a pure value calculation: no volatile qualifier, memory clobber,
dummy dependency or constrained surrounding C local is added. Its expression
wrapper evaluates its argument once and returns the real calculated value.

The former allocator-selected early-clobber scratch emitted the arithmetic
but did not describe that shared SDK register contract. Aligning the single
existing macro fixes both uses consistently, rather than introducing a second
fixed-register variant for one target. Actual consumer owners are
`sdf/sdfTex.c` and `sdf/sdfMovie.c`; complete owners, guarded drafts and the
retail image qualify the shared change, separately from any new match credit.

cc1 still models the whole block as one asm instruction, so source scheduling
around it can differ. Historical measurements below used the old scratch
contract and must not be treated as a proof that the corrected primitive's
surrounding C is impossible. Splitting the operation into arbitrary one-word
macros remains inappropriate. The additive movie packet reconstruction is
qualified separately.

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

## SDK scalar scratch in a VU0 scale

`nbMakeNegoBackPacket` uses the existing `SDF_VU0_SCALEVECTOR_V0` primitive
for the SDK's `mfc1 $2; qmtc2.ni $2, vf2` transfer. The allocator-selected
primitive uses `$3` here and leaves two words different in the 260-byte body.
The fixed SDK scratch convention follows the canonical
[inline-asm rules](https://github.com/Megami-Decomps/dds-decomp/blob/main/docs/idioms.md#inline-asm-cop2-and-mmi).

This use has a finite emission audit: the preceding call's return is unused;
the radius and scale product is in `$f0`; no unrelated `$2` value crosses the
transfer; and the next `$2` use is a fresh position-address definition. The
three stack vectors are initialized or produced before their VU consumers,
and the resulting position is stored before its packet call. The complete
owner and retail checksum qualify this emitted sequence. Reaudit changes to
the source, compiler or macros; the legacy scratch convention does not supply
a general C memory or register guarantee.
