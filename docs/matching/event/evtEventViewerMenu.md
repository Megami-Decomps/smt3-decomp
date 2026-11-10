# Matching notes: event/evtEventViewerMenu

## evtPollEffectFrameControl (event/evtEventViewerMenu, 0xE4)

20 of 56 words differ in the current 224-byte emitted body, against retail's
228 bytes; four native bytes are uncovered. The fallback explicitly declares
size 0xE4. The zero at 0x1F7A8C is following alignment, not part of the function.
The old exact-size claim was incorrect. The eleven switch-table destinations
are exact retail addresses, but the busy/viewerinfo argument-register homes,
default comparison and success exit still differ.

The prerequisite change only corrects the local `dds3KillProcess` declaration
to the actual `s32(dds3ProcessID_t *, s32)` provider in `kernel/dds3KernelCore`,
and removes the sole unnecessary `void *` cast on the already-typed menuProcess
argument. The existing header type, creator's returned pointer, timer one,
ignored return and callback-sensitive viewerinfo reload before clearing are
unchanged. All guards remain intact. Complete direct assembly and ELF objects
are literally identical before/after this correction in production, SKIP_ASM
and retained contexts; direct/wrapper parity also passes after only the source
filename normalization. The complete guarded owner remains 60 match, 0 differ.

Three fresh target forms are closed. After independently resolving all twelve
target relocations against verified addresses, the ordinary baseline has 20
literal word differences in its 56-word overlap. Binding the initialized real
busy result to `$5` and giving it an input-only operand immediately before the
common test reduces that to 14. It restores the initial viewerinfo and
callback-result homes, but not the comparison or exit. A final forward shared-success
label control has 15 differences. Every form still emits 224 bytes against
228 native bytes; none is an exact match or a full-link approval.

No constrained value crosses a clobbering call: each calling arm replaces busy
with its result, and the common consumer ends that lifetime before the kill.
The early-null success path still bypasses kill and clear. The final control
was motivated by the qualified baseline delay-reorganization dump's separate
partial epilogue and native's shared return-one block, but did not recover it.

The comparison mismatch is already present in qualified initial RTL: `not`
followed by logical-right-shift31, before allocation, in both baseline and busy
controls. The old allocation-only explanation is therefore unsupported. This
observes emitted RTL, not an internal expansion predicate or historical source.
The 60 peer contracts are unchanged in production and retained contexts.
In SKIP_ASM they agree only after accounting for the audited 48-byte target
table/alignment insertion and shifted later rodata references. All three active
forms have identical allocated data and data-relocation contracts within each
mode. This is not raw SKIP_ASM identity to the guarded owner.
Complete source/output qualification covers all 15 source/mode contexts. All
inexact controls remain private; no new matching credit is claimed.

Previously ruled out: busy initialized in its declaration, after it, or only in
arms that need it; a local switch value; equivalent 0x10/0x11 null-test branch
spellings; and the documented default-comparison spellings (`>= 0`, `!(< 0)`,
`(< 0) == 0`, `(< 0) ^ 1`, `1 - (< 0)`, conditional and ternary forms). They all
kept the `nor`/`srl` pair. Retain the eleven explicit case labels: the reduced
case set instead fell below `case_values_threshold` and became a comparison
tree. Do not repeat these grids, the busy constraint or the forward-success
form without new source/consumer evidence for the native comparison or exit.

## evtFrameListHead (event/evtEventViewerMenu, 0x138)

78 of 78 words match at the native 312-byte size in ordinary C. The real
column-width read belongs after both `sdfDevPrintf` and `sdfDrawTagLink`.
The entry track type remains captured once. Each iteration reads its header
column before the calls, then reloads the same column and its signed width
after them. The count is also reloaded for the next iteration. The exact
`evtFrameListItemSub` sibling already has this post-call width observation.

The former draft initialized `w` before the draw calls. That snapshot does
not express native callback-sensitive read ordering. Moving only its actual
assignment after the calls restores native output without a binding, empty
operand, volatile qualifier, provider change, or new storage view. The
existing ten-byte row type and its eight-column member remain unchanged.
The old advice to keep the width computation before drawing was incorrect
for the native memory contract and is retired.

The current complete-owner baseline emits 308 bytes, with 31 matching and
46 differing words over its 77-word prefix and four native bytes uncovered.
The historical 39/78 score referred to an older source context and is
not the current baseline. A separate genuine column-member pointer control
preserves post-call reloads and native 312-byte size, but matches 48/78; the
simpler indexed form is preferred. No other coordinate, declaration-order,
base-pointer, register, or constraint sweep is needed.

Complete-owner candidate checks preserve all 59 exact peers and owned data.
Production and SKIP_ASM canonical/plain/dump builds of the baseline,
candidate and member-pointer control have matching full allocated-section,
non-file-symbol, relocation and normalized-assembly contracts. All nine
three-mode strict builds are silent. Six existing row-layout assertions are
object/assembly-neutral in both modes, and a wrong row-size assertion fails.
These compiler observations do not establish original source spelling or
prove that the tables are immutable. Full retail and regression qualification
accompanies promotion.

## evtSaveViewerHeader (event/evtEventViewerMenu, 0x400)

256 of 256 words match at 1024 bytes in ordinary C. A block-local signed
`headerCount = 12` is initialized before the header writes. The writes then
follow retail order: byteLength, tag[0..3], count, kind, and the remaining
reserved fields. The real count local dies at its store, before evtWriteRecord.
It changes no calls, types, field widths, values, or arithmetic.

The previous draft matched 254 of 256 words, with count and byteLength stores
exchanged. Merely reordering the direct constant stores changed their register
allocation. Separating the real count initialization from its store preserves
both the desired allocation and store sequence. A diagnostic native-register
binding first established the candidate, but binding-only ablation remained
exact, so the final source uses no register binding or empty asm. This is a
source/code-generation reconstruction, not a claim about historical spelling.

Keep payloadOffset = 0xE0 after the mode != 2 early return, and i declared before
payloadOffset: the existing body, all twelve byte-length products, 21-entry jump
table, fourteen directory arms and payload advance remain unchanged. The native
writer consumes the same initialized 32-byte header and directory records.

Qualification: complete owner 59 match, 0 differ including owned data;
normal and RTL-dump compilation agree in all allocated/relocation sections,
normalized instruction assembly and target bytes. Full retail and regression
qualification accompanies the promotion.
