# Matching notes: newbattle/NbNegoFlowInnen

## Flow5_3 (newbattle/NbNegoFlowInnen, 0x164)

89 of 89 words match at 356 bytes. Direct field expressions and two complete
60-byte interpretations of the existing phase/message record reproduce the
native accesses. The union's phase view retains the 13 four-byte item/count
entries at offset 8. Its message view retains the real unknown byte regions,
unsigned 16-bit main message/count at offsets 32/34, and three signed six-byte
motion records at offset 42. Both complete views preserve the signed 16-bit
ID and unsigned 16-bit header fields at offsets 0/2/4/6. Target-compiler
assertions check record and element sizes and every named field offset.

This changes the nesting, not the bytes, object origin, alignment, widths,
extents or field meanings. All consumers, including the two remaining guarded
phase drafts, use consistent member paths. There is no shifted alias symbol,
packed type, alignment attribute, fabricated object or cross-field arithmetic.
The prior nested representation was behaviorally valid; its different compiler
address decomposition was the matching obstacle. Neither representation is
claimed to recover the historical source spelling.

The target observes the actual message/count fields directly and reloads
`innen_id` and `nownegophase` inside each motion iteration, preserving
callback-sensitive behavior. The same three motion entries, side selection,
motion choice, calls and zero return remain. Existing valid table/index and
object-lifetime contracts are unchanged; no broader bounds guarantee is added.

### Qualified expansion evidence

The pinned compiler's record-layout code normalizes field offsets into byte
and residual-bit quantities using a 128-bit offset alignment. Its
`get_inner_reference` accumulates those quantities separately. A variable
array index makes the byte quantity nonconstant, so it cannot be folded back
into the bit quantity at that return. This is observed compiler behavior,
not a compiler-correctness complaint.

In the old nesting, the message's offset 24 becomes 16 bytes plus 64 bits,
while the outer data offset 8 contributes another 64 bits. The resulting
main-message reference is `id * 60 + 16` bytes plus 128 residual bits; the
count has 144 residual bits. Complete-record fields at 32/34 instead have
32 byte offsets plus 0/16 residual bits. Motion at 42 already decomposes as
32 bytes plus 80 bits in either nesting. This explains why the coherent
complete-record view changes the message-header expansion without disturbing
motion offsets.

A bounded observation of the unchanged compiler through the existing QEMU
runtime records four instruction PCs. All 200 sampled layout alignments are
128 bits; returned reference bit positions include 128 and 144. The observer
does not capture source expression names: their attribution uses the qualified
initial RTL/source correspondence. Observed and plain full-owner assembly
agree after excluding only the first `.file` directive's random scratch path;
raw assembly is not byte-identical. Allocated sections, relocation output and
symbol output also agree. This qualifies the observed direct-field baseline,
not an unobserved final-source trace or a general allocator theory.

Independent review and the frozen candidate pass canonical complete-owner checking
at 20 match, 0 differ, preserving all 19 peers and owned data. Both retained
guarded drafts also pass the standard nonmatching check, and a separate
canonical `-DNON_MATCHING` compilation succeeds. These are source/owner checks;
full-project integration and publication are separate.

The table remains assembler-owned in `asm/smt3/data/data.data.s`. The only
other C consumer, `nbNegoProcess.c`, retains its existing complete 60-byte
phase/header view and native header reads. There is no C definition or shared
header changed by this work. This is a verified platform-specific local view
of assembler-backed storage, not a claim of portable ISO-C compatibility
between the independently declared external types.

Final local gates pass the retail checksum, 26 tooling test files / 427 cases,
all 147 guarded owners / 527 drafts with stable source/compiler inputs,
seven IRX owners, and reconciled production reports. The unchanged
`nbNegoProcess.c` owner also passes 44 match, 0 differ.

Source commit `9360b924f3307fb01c208e73ead68968805467f4` passed exact-source
hosted CI [41172587625](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41172587625),
attempt 1, including build and deploy. The run completed successfully at
05:28:48 UTC on 2026-10-31; the deployment reported success at 05:28:46 UTC.
