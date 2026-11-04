# Matching notes: nme/Nme_Init

## nmeStartProcesses (nme/Nme_Init, 0x11C)

69 of 71 words match, at the exact 284-byte size. Allocates name-entry work
unless it is already active, starts the calculation, draw and update processes,
saves and clears the kernel debug sign, and begins a 30-frame fade. Event bit
`0x12` selects black when set and white otherwise.

Only the first two white-color byte stores differ: retail stores offsets 2, 0,
then 1, while this draft stores 0, 2, then 1. The black stores and every other
instruction match. The chained white assignment expresses equal RGB values;
the separate zero stores retain the native black-path ordering.

Bounded checks ruled out reversed and permuted three-field chains, separate
stores in native order, paired assignments before or after the third store,
a named color pointer, and a saved event result. These either retain two or
more differing stores or change register allocation and grow to 292 bytes.
The unresolved question is the source shape that gives the native white-store
schedule without changing the surrounding allocation and control flow.

A fresh unchanged full-owner diagnostic localizes the ordering to prereload
scheduling. Before that pass, the three white stores remain in offsets 2, 1, 0
order and the real white-value byte pseudo dies at the offset-0 store. All
three have equal priority, but the offset-0 store has register weight -1 while
the others have 0. Qualified live comparator decisions choose offset 0 first
on register weight, then offset 2 before offset 1 on LUID. Postreload scheduling
preserves 0, 2, 1 through LUID comparisons; register weight is not the deciding
tier there. Final output retains offset 1 in the branch delay slot. This is a
case-specific measured cause, not a general source-order or reorg rule.

The unchanged case has byte-identical complete assembly and objects from the
ordinary wrapper, direct plain and direct dump paths. Fresh observer baseline
and observed outputs agree across all 28 artifacts; 5,565 frozen inputs remain
unchanged. Target and four pass gates, normal completion, verified loopback
listener ownership and cleanup pass. The owner comparison preserves 20 exact
C peers and reports the three existing guarded nonmatches honestly; the target
remains 69/71 at 284 bytes. This qualifies the baseline observation only, not an
untested source variant or blanket observer neutrality.

No new truthful source/lifetime control follows from this measurement alone.
The ordinary permutations above remain closed. Do not add a dummy consumer or
artificial lifetime extension merely to change the last-use scheduling weight;
a further control needs a real consumer and preserved native memory accesses.

## nmeInitNameRow (nme/Nme_Init, 0x144)

52 of 81 words match at the native 324-byte size, improved from
6 matching words in a 316-byte draft. The mode byte is multiplied in place,
preserving signed-byte truncation before indexing the name table. The decoded
character count begins after the row has been cleared. A guarded do loop
retains the native upward clear counter; an ordinary for loop is reversed
into a countdown by the compiler.

Both encoded two-byte characters and decoded single-byte characters retain
their original stores, and the final character count still wraps as a signed
byte. Normal, instrumented and dump-enabled compilation produce identical
assembly and intermediate representations. Remaining differences include
row/count register allocation, row-pointer scheduling, and the destination
pointer addressing the first byte rather than the native second byte. The
assembly fallback remains active; this is not an exact-match promotion.

### Second-byte addressing control (closed negative)

The canonical guarded baseline was reproduced at 52 matching words out of
81, 324 emitted/native bytes, with zero uncovered or excess bytes. A bounded
whole-row character-view investigation did not improve it:

- The initial indexed form emitted 296 bytes: 19 matching and 55 differing
  words among 74 emitted words, 28 native bytes uncovered, zero excess. Its
  shared row-base lifetime and explicit unsigned snapshot changed address
  and source-load scheduling, so it was rejected.
- The corrected form used a second-byte clear cursor and a separate integer
  second-byte decode index. It emitted 316 bytes: 22 matching and 57 differing
  words among 79 emitted words, 8 native bytes uncovered, zero excess.

The corrected form obtains the native -1/0 stores in both loops, including
encoded first/second-byte and decoded second/first-byte ordering. However,
row/count remain in registers 19/18 rather than native 18/19, and row-address
setup, scheduling and instruction extent remain different. Thus second-byte
addressing alone does not solve the allocation/scheduling residual. This is
an observed output blocker, not an identified allocator-pass cause. Do not
repeat this family without a new compiler premise. The entire family remains
negative; canonical source and its assembly fallback are unchanged.

Each input row is the actual 32-byte `u8[16][2]` object. A naïve cursor starting
at its second byte and advancing after every glyph forms row+33 after a valid
16-glyph input, beyond one-past. The safe indexed alternative casts from the
whole row object, stores at integer offsets secondByte-1 and secondByte, and
increments only that integer at loop end. It forms no row+33 pointer and
neither narrows capacity nor introduces truncation or checks. The real clear
provider returns 8, so its separate second-byte cursor ends at row+17 safely.

The twelve initialized default strings decode to
0,0,0,5,4,0,5,5,0,8,6,0 glyphs. This establishes initialized data, not immutable
table contents or global writer/alias closure. Named source/assembly references
show no writer, but the pointer table is writable and indirect/numeric writes
have not been excluded. The ordinary row wrapper passes rows 0..2; bootstrap
passes modes 0..3, while load/draw/script paths propagate target values without
a global input-validity proof. Signed row normalization, byte-truncated mode*3,
signed termination, unsigned source snapshots, truthful `u32(s8)` decode
provider, no-call encoded arm and signed-byte count conversion were preserved.

All controls keep the 20 exact peers. The guarded canonical owner checks
20 match, 0 differ, including owned data. Plain and dump-enabled complete
assembly, allocated sections, non-file symbols and ordered relocations agree
for all three controls. No debugger-hook or causal allocator trace is claimed.
A host differential harness passes 612 whole-work/call-sequence comparisons
for lengths 0..16 across ASCII, encoded and mixed inputs under ASan/UBSan
(leak checking disabled because the runner prevents LeakSanitizer). Its decode
stub and host execution check addressing equivalence, not native execution
or the real decode table. Oversized rows/count wrap were not invented as a
valid test domain.

Strict `-Werror-implicit-function-declaration` production and SKIP_ASM checks
both fail with the same inherited diagnostics in canonical and corrected
owners; NON_MATCHING+SKIP_ASM passes. Canonical source locations are:

- `memset`: implicit call at 53; its existing declaration at 44 is under
  NON_MATCHING (43..45).
- `nmeInitNameCursor`: implicit call at 170; its existing declaration at 161
  is under NON_MATCHING (158..162); definition begins at 189, with the
  compiler reporting its conflicting implicit type at 190.
- `cmpInitCursorRange`: implicit call at 174; existing conditional declaration
  at 160, and later unconditional declaration at 186.
- `cmpInitCursorLayout`: implicit call at 175; existing conditional declaration
  at 159, and later unconditional declaration at 187.

The same logs warn about returning a value from the void function at 175,
and the subsequent range/layout/cursor declarations disagreeing with their
previous implicit int returns (186/187/190). No declaration or guard was
changed for this negative investigation. No matching credit is earned.

A later read-only visibility audit found ordinary surviving calls to the
real clear-bound and decode providers, consumed return values and no
identified omitted shared-memory reload attributable to provider visibility.
The evidence supplies no concrete premise for a same-symbol opaque-provider
control. No alias trial or source change was made; this is not a proof that
an alias could never affect output.

### Declaration visibility follow-up

Only two guard pairs around four existing declarations were removed:
memset, nmeInitNameCursor, cmpInitCursorRange and cmpInitCursorLayout.
Provider signatures, argument values, native call targets and unused returns
were independently reviewed; later duplicate declarations remain unchanged.
All function bodies and assembly fallbacks are unchanged. The existing
void-expression return in nmeInitNameBuffer is accepted by the supported GNU
EE compiler and preserves its native tail jump; this is not strict ISO-C
portability or a body rewrite.

Paired baseline/candidate full object contracts and normalized assembly
agree in all three modes. Strict production/SKIP_ASM/retained status changes
from 33/33/0 to 0/0/0; candidate logs are empty. Historical failed checks above
belong to the original negative-control investigation, not this final source.
No exact matching credit or runtime bug repair is claimed.


Final integrated owners 77/0 (center), 20/0 (name) and unchanged Hansya
owner 290/0 pass. Fresh stable 140-owner/480-draft qualification, 450 tests,
817 layout assertions, canonical strict/plain/dump parity, inline/shell checks,
seven IRX modules, retail SHA1/byte comparison/no-op and reconciled reports
pass. Coverage remains 8594 functions / 1485744 bytes; zero matching credit.

## nmeBuildNameFont (nme/Nme_Init, 0x348)

1 of 206 words match; native is 840 bytes against the draft's 824. Checked
against `asm/smt3/nonmatchings/nme/Nme_Init/nmeBuildNameFont.s` word by word
rather than trusted at this match rate: the `fontNo` arithmetic (`addu $5,$6,$5`
against the `0xFFFF7F80` constant, i.e. `(decoded - 0x8080)` masked and
shifted) matches the draft's formula exactly, and the per-row reload of
`work->inputRowEnabled[row]` inside the loop condition (retail reloads it with
`lb` every pass rather than caching it) matches the draft's unconditional
re-read. The size and instruction-count gap is explained by one copy: retail
copies the 0x120-byte `nmeNameGlyphImage_t` with an explicit alignment-tested
manual loop (`ldl`/`ldr`/`sdl`/`sdr` selected by `andi $3,$3,0x7`, falling back
to the IRX's unaligned-copy helper when needed), while the draft's plain
`*destination = *(nmeNameGlyphImage_t *)source;` lets the compiler choose its
own copy strategy for the same bytes. Confirmed as a codegen-shape difference,
not a wrong value, lane or layout; the assembly fallback remains production.
