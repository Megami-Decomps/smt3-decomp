# Explanation-text pointer table

`datExplainTable` contains 528 32-bit EE pointers. The named span ends at
`pStr` and starts after `text2`. The split replaces only the pointer entries;
the 234 distinct text providers remain in the existing camp owner, and no
text bytes or neighboring data move. The initializer uses the existing
same read-only view for both const and writable providers while leaving the
pointer slots mutable. Only the existing battle-help and font interfaces
still take legacy unqualified pointers; both read the input text without
modifying it. The result message reader converts the pointer to its
existing signed EE address ABI.

The script command obtains both the message value and the selector from
the script. The selector-8 path performs an unchecked EE address
calculation. Its native `sll`, `addu`, and `lw` are modeled as unsigned
32-bit address arithmetic followed by an EE pointer-sized load, so this
does not impose a C array bound or claim that every address is readable.
The camp status renderer uses the same address calculation for the signed
affinity value. In particular, the saved equipped-heart value is an
unsigned byte and the native provider's signed-halfword load does not
check that the table contains that heart. No bound, clamp, or fallback
is introduced. The normal renderer paths using unsigned race IDs and
the bounded LND calculation keep ordinary in-array lookups.

For the script reader, the explicit shift and the element-size spelling
produce identical native output; the latter preserves the relationship
to the real pointer element. All affected source owners preserve
the complete stock text/data and relocations in production, base, and
both guarded modes. The table's 528 ordered relocations match the native
pointer sequence, and the left raw neighbor still has all 385 relocations
and all 1544 bytes. A scoped overlay reproduced the complete retail image.
