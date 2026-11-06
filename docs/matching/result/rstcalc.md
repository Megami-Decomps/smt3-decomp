# Matching notes: rstcalc

## cmpFindKugatatiMsgEntry (rstcalc, 0x60)

24 of 24 words match at 96 bytes in ordinary C. The scan returns the signed
level byte of the first kind-five entry among the unit's 24 level-up entries,
or -1. It does not compare the unit level, read the value halfword or write
memory. The existing signed-return consumer and ABI are unchanged.

Two genuine source properties close the old 10/24-word draft. Indexing the
actual row's entry member gives 12/24, retaining the native bare-table anchor
and +7/+6 field displacements. Factoring the real 102-byte stride into unsigned
ID times three and then sizeof(row)/three gives 21/24 with an extracted entry
pointer. Combining both properties gives the exact native allocation and
addressing. No constraint, binding, helper, declaration or compiler flag changes.

The row has six header bytes and 24 four-byte entries; the actual table has
224 rows. The complete-array byte view converts the selected address back to
the true row type. Both unsigned products fit for every u16 ID. Valid row IDs
remain inherited input requirements. Signed level conversion, scan order and
failure return are preserved, including negative signed-byte values.

Qualified full-owner stock expansion records the factored product in a source
scalar and puts the shifted term before it in the next sum; the single product
puts the three-times-ID term first. This establishes reconstruction evidence,
not original source spelling or an instrumented allocation claim. Fifteen
full-assembly/literal-object plain-dump pairs cover five controls in three
contexts using fixed same-length scratch paths; an unmodified wrapper also
agrees after normalizing only its random file metadata. Complete owner checks
94 match, 0 differ. All 104/93/104 peers and owned data are unchanged after
resolving local code relocations. Strict production and SKIP_ASM retain the
pre-existing rstSetSkillActFlag declaration gap; retained-draft strict passes.

Independent semantic and exact-byte review reproduced all three contexts and
six complete output pairs, then audited the full fifteen-pair qualification
and its 687-file relevant compiler/runtime input closure. Full retail checksum,
generated-source checks, 568 local and 504 public tooling cases, retained EE
and IRX compilations, and reconciled reports pass. Linked production evidence
proves one unique 96-byte C-owned span, adding one function and 96 game bytes.
The optional external dvp-as comparison is skipped because that executable is
unavailable; native VU reconstruction and the full retail checksum pass.

Historical standalone ID-type, declaration-order, byte-walk and index-binding
controls are superseded by this paired ordinary source evidence, not reopened
as independent register permutations.

## cmbChkDevilEvo (rstcalc, 0x74)

29 of 29 words match at 116 bytes in ordinary C. The complete owner is
92 match, 0 differ. The scan returns the signed level byte of the devil
named by the first kind-5 entry, or -1 when none occurs. It does not read or
compare the unit's level or the entry's level, and it does not return the
entry index as the type-five/type-seven eligibility scans do.

Two independent source properties close the current draft's seven words:

- Index the actual row's `entry[24]` member. The original advancing entry
  pointer gives 22/29; a factored row offset with the same pointer walk gives
  26/29. Row indexing preserves the native table-plus-eight anchor, with
  `lbu -1` for kind and `lhu 0` for value. Indexing an extracted entry pointer
  alone still gives 22/29; the containing record matters.
- Factor the genuine 102-byte stride into `idTripled = (u32)unit->id * 3`
  and `offset = idTripled * (sizeof(datLevelUpRecord_t) / 3)`, as in the
  matched evolution scans. A single multiply or direct typed row address
  gives 25/29. The factorization supplies the first intermediate in `$3`
  and the shifted/final offset in `$2`, and also restores the first `sll`
  before the hoisted devil-table `addiu`.

Qualified full-owner stock RTL establishes the expansion difference:
`00.rtl` retains the first product as a source scalar and puts the shifted
term first in the following sum. The single multiply puts the three-times-ID
term first. This is compiler reconstruction evidence, not a claim about
historical spelling or an instrumented scheduler comparator. Both traces
match ordinary full assembly and the entire object; fifteen production,
SKIP_ASM and retained-draft plain/dump pairs also pass. All 104/91/104 peers
and owned data are unchanged after resolving expected local-call relocations.

The actual table has 224 rows of 102 bytes, each with six header bytes and
24 four-byte entries. The selected entry's unsigned halfword indexes the
384-entry, 60-byte devil table; its byte at +5 is returned as signed eight-bit.
Both unsigned products fit for every u16 ID; valid row and devil indexes
remain inherited input requirements. No guard, call, write, constraint,
helper, type or flag is added. The full-owner byte view is converted back to
the real row type before its members are accessed.

## datFindKugatatiEntry (rstcalc, 0x74)

The guarded typed lookup emits 108 bytes against 116 native bytes, with all
27 emitted words differing. It scans 24 level-event records after a six-byte
header. Each four-byte record contains a level byte, kind byte and u16 value;
the lookup compares the level as a low byte, returns the first matching
record and optionally writes the eight-bit match count.

The consumer `rstCalcEventInfo` now uses that record view directly. Its signed
count/index, halfword value store and signed event-kind return are recovered;
the lookup remains assembly-backed.

Native keeps `id * 0x66 + 4 * i` as a byte offset, the bare table base in `$10`
and a separate `$12 = $10 + 6`. Its level load is `lbu 6($6 + $10)` and its
returned entry address is `$6 + $12`. The typed pointer walk folds these into
one induction pointer. The source-level representation preserving both native
address forms is still unresolved; the size gap also remains.

Typed nested-row indexing emitted 128 bytes. A direct byte-offset form emitted
116 bytes but matched only two of 29 words, so correct size alone does not close
this function. Earlier integer-address and explicit two-base forms either
rematerialised the table inside the loop or added another induction variable.

## cmbGetHeartsParamByIndex (rstcalc, 0x58)

22 of 22 words match at 88 bytes. The signed-byte race and index select one
of two flat signed-byte tables, preserving each native symbol and byte load.
Their sum is formed as index + race * 66: the signed-byte inputs keep this
calculation within s32 range. The existing valid table-index contract is
unchanged; no new bounds or pointer assumptions are introduced.

Each arm binds its real initialized table offset to $2, used immediately by
the byte load and never live across a call. This localized target-specific
allocation aid makes the assembler use $at for the load address, as retail
does; it is not evidence of the original source spelling. No empty asm,
fabricated consumer, instruction replay or provider change is involved.

The original operand order without bindings gives 14/22 words. Binding the
offset while retaining that order gives 16/22; index-first without binding
gives 10/22; both changes give 22/22, all at 88 bytes. Final diagnostic owner
checking gives 87 match / 0 differ including owned data. Plain and stock-dump
builds have identical allocated/relocation section bytes and normalized
assembly. Independent current-tree and retail qualification remain the
publication gate.

## cmbCountEvoReady (rstcalc, 0x84)

33 of 33 words match at 132 bytes using ordinary C. The initial u8 count
guards a nonempty traversal. Inside that path, a separate unit cursor and
u32 countdown retain the native initial copies and full-width decrement.
The countdown starts in 1..255 and stops at zero, so it never underflows;
this preserves the byte-count traversal without an extra per-step mask.

Each unit's flags are observed before its conditional evolution lookup:
alive bit 1 must be set and evolved bit 4 clear. The lookup receives the
unit ID as a halfword. The u8 result count wraps as before and is returned
through the existing signed-byte interface. Unit stride remains 0xEC.
No new bounds, provider contracts, constraints or compiler options are used.

A full-width while countdown gives 24/33 words at 132 bytes. An explicit
nonempty do-loop with shared return gives 26/33 while mutating the incoming
unit parameter; using the separate traversal cursor in that same shape gives
33/33. The early-return form gives 140 bytes and is not retained. These
controls isolate actual countdown width, guard shape and cursor lifetime,
rather than the previously exhausted standalone byte-local copies.

## cmbGetLevelUpCount (rstcalc, 0x84)

33 of 33 words match at 132 bytes. The routine copies the input devil into
the real scratch unit, adds experience with unsigned arithmetic, and calls
the existing level-threshold provider until experience is insufficient or
the observed level reaches 255. The level increment is stored as a halfword
before checking the old level, preserving native behavior even at the cap.
The count increments only after a successful below-cap iteration.

The old-level snapshot and the value incremented/stored have separate real
consumers. One nonvolatile empty asm with a generic +r operand on the latter
retains the native copy before incrementing. Both values are initialized by
the same halfword load and dead before the next callback. No fixed register,
volatile access, invented use, provider change or instruction replay is used.
This is a localized target-specific reconstruction aid, not evidence of the
original source spelling.

Ordinary separate snapshots coalesce to 128 bytes, 22/32 matching words.
Constraining only the old comparison snapshot gives 132 bytes but 27/33
matching words. Constraining only the increment/store operand gives the
exact result; a paired-operand constraint and an old-snapshot $3 binding were
therefore removed as unnecessary. Final-source owner, compiler parity and
independent retail checks qualify publication.

## rstFindEvoEntryType7 (result/rstcalc, 0x6C)

27 of 27 words match at 108 bytes in ordinary C. The scan returns the first of
24 level-up entries whose kind is 7 and whose level is at or below the unit's
snapshotted level, narrowing the index to `s8`, or returns -1 if none qualifies.
No new constraint, register binding, helper, type or compiler flag is needed.

The current guarded pointer walk starts at 21/27. Direct indexing through the
actual `datLevelUpRecord_t` restores the native record-relative `+6` and `+7`
loads, giving 24/27. The remaining three words exchange the two temporaries
in the 102-byte row multiply. Naming `idTripled = (u32)unit->id * 3`, then
multiplying it by `sizeof(datLevelUpRecord_t) / 3`, gives the native allocation:
three times the ID in `$3`, with its shifted term and final byte offset in `$2`.
Removing that factorization alone restores the three differences at `+0x10`,
`+0x14` and `+0x18`. Binding either the final offset or the factored intermediate
is unnecessary; no binding is retained.

The source forms the byte address from `&datLevelUpTable`, the complete array
object, and converts the selected row address back to its actual record type.
The existing record is 102 bytes: six header bytes and 24 four-byte entries.
The recorded table extent is `0x5940`, or 224 rows. Valid table IDs remain an
inherited input requirement; no new behavior is claimed for out-of-range IDs.
For every `u16` ID the two unsigned products fit in `u32` and equal ID times
102. The indexed loads stay within the actual row's entries. The function
preserves the unsigned ID, level and kind reads, the scan order, signed result
conversion and failure return, and performs no writes or calls.

Qualified stock compiler dumps show a source-expansion difference: the single
multiply adds the three-times-ID term before its shifted term, while the two
source multiplications put the shifted term first in that addition. The
bounded unsigned sum is unchanged. This is a measured reconstruction aid,
not evidence of the original source spelling or an instrumented claim about
every allocator decision.

The complete target-enabled owner checks 90 match, 0 differ. Nine complete
ordinary/stock-dump assembly and ELF parity pairs cover the original owner,
final source and minimal ablation in production, `SKIP_ASM` and retained-draft
contexts. Production allocated code/data and resolved relocations equal the
assembly-backed owner. All 104/89/104 peers and non-code owned data are
preserved; thirteen changed raw call words in the original-to-final
`SKIP_ASM` comparison resolve to the same function targets after insertion.
Independent native, layout and focused host-model review approved the source.
The existing strict production/SKIP implicit declaration in `rstInitSkillAct`
is unchanged; the strict retained-draft compile passes.

## rstFindEvoEntryType5 (rstcalc, 0x6C)

27 of 27 words match at 108 bytes in ordinary C. This sibling uses the
same real 102-byte row-stride factorization as the matched type-seven scan,
but selects kind 5 with a level strictly below the unit's snapshotted low-byte level.
Equal levels must not qualify. It returns the first qualifying index narrowed
to `s8`, or -1, and preserves the unsigned ID, kind and level reads.

The current pointer-walk draft gives 21/27. Indexing the actual row's entries
with an ordinary single multiply gives 24/27. Factoring the real row offset
as `idTripled = (u32)unit->id * 3`, then multiplying by
`sizeof(datLevelUpRecord_t) / 3`, gives 27/27. The single-multiply ablation
restores only the three register-allocation differences at `+0x10`, `+0x14`
and `+0x18`. No constraint, binding, helper, declaration or compiler flag
changes are needed.

As in type seven, the byte view starts from the complete `&datLevelUpTable`
array and the selected row is accessed through its actual record type.
Each 102-byte row contains a six-byte header and 24 four-byte entries; the
recorded `0x5940`-byte table has 224 rows. Valid row IDs remain an inherited
input requirement. Both unsigned products fit for every `u16` ID, and the
loop's field reads remain within the selected actual row. There are no
writes, calls, invented bounds or extra consumers.

The complete target-enabled owner checks 91 match, 0 differ. Nine complete
plain/stock-dump assembly and ELF pairs pass for the final source, original
owner and single-multiply ablation in production, `SKIP_ASM` and retained-draft
contexts. Production allocated code/data and resolved relocations equal the
assembly-backed owner. All 104/90/104 peers and non-code data are preserved;
SKIP-only call displacements shifted by insertion resolve to the same targets.
These measurements establish the source reconstruction, not original spelling.

Earlier negative address, ID-type, declaration-order, pointer-walk and nested
condition controls predated the successful factorization. They did not establish
a source-level barrier, and are superseded by the minimal ordinary ablation.

## cmbInitStockUnit

The earlier 172-byte guarded entry-pointer form has been reduced to 160
emitted and native bytes; 37 of 40 words match. Three differing words
at `+0x20`, `+0x24` and `+0x2C` exchange the temporaries in the multiply-by-102
address expansion. All loads, conditions, stores, loop ordering and calls
match. Normal builds retain assembly.

The helper masks the devil ID to 16 bits, clears the unit's skills, then
scans all 24 level-up entries in order. A kind-one, level-zero entry appends
its 16-bit value and increments the unit's 16-bit skill count. The established
102-byte row and four-byte entry types express the complete access path.

A parity-qualified `cc1probe` run on this guarded body used `--alloc --sched
--no-dumps --cflag -DNON_MATCHING --func cmbInitStockUnit`; hooked and unhooked
assembly matched (`verify: OK`). In local allocation, UID33 combines pseudo91
into92; UID37 combines92 into94; UID39 combines94 into95. The resulting
quantity has nine references over lifetime10..26 and receives `$2`, while
the separate pseudo93 receives `$3`. Thus the three-times-ID intermediate
shares the final offset quantity. Native instead holds the three-times-ID
value in `$3` and the shifted term/final offset in `$2`. This is coalescing,
not instruction scheduling or an incorrect row size.

An ordinary signed byte-offset form and a shared table/entry index control
are inert. The direct typed-row source remains preferred. A further change
needs a genuine source lifetime or access-path premise that explains the
different temporary combination; manually expanding the constant multiply
is not justified by this observation.

## rstSetCurrentDevil (176 bytes, matched)

The signed stock scan is now exact at 44/44 words. Existing signed and unsigned
union views of the index byte express retail's `lb`/`lbu` observations without
volatile casts. A real `DDS3_GBWK` snapshot before the index-byte store supplies
the stock access; the later global observation supplies the unit pointer.
The labeled retry and shared exhaustion exit preserve native control flow,
call order, and the conditional publication of `EventStart->unit`.

An initialized success value of one has a real return-choice consumer after
`rstSetLevelUpCount` and `rstChkParamLimitAll`. A localized empty `+r` constraint
keeps that value live across the scan and calls without fixing a register.
The allocator retains it in saved `$18`; the unconstrained ordinary form sinks
one after the final call and produces 168 bytes rather than retail's 176.
This is a target-specific compiler aid, not original-source evidence or a claim
about an unmeasured compiler pass. The constraint is not a memory barrier;
the complete target output establishes the required accesses and ordering.

The inherited contract still requires valid stock and unit indices whenever
those accesses occur. The signed scan is not a new lower-bound check, and no
bounds or guaranteed retry termination under arbitrary callbacks are invented.
The byte increment is promoted before narrowing back to the stored byte.
Return zero leaves the published unit unchanged; a qualifying unit selects two
unless the parameter-limit check returns zero, selecting the retained one.

Independent complete-owner checking gives 83 match, 0 differ, including the
caller and both helpers, with owned-data checks enabled. Final-source checks
also pass the retail checksum, all 149 guarded owners / 601 drafts, seven IRX
owners, 26 tooling test files / 427 cases, shell mocks and documentation checks.
Source commit `ee3811254c1dfa0a09931f37f9ec61c5478ec292` passed exact-source
hosted CI [40977985806](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40977985806),
including build and deploy.

## rstChkSkillPowerUp2 (result/rstcalc, 0xA8)

42 of 42 words match at 168 bytes in the unsigned random interval check. The
unit ID selects one call to the existing `u32 dds3GetRandA(void *)` provider with `NULL`, which
uses the mutable global RNG state. The result is reduced modulo 254. The first
arm tests the low interval below 112 and then unsigned `roll - 112 < 142`; the
other tests below 128 and the remaining interval below 254. The shared fallback
preserves retail's return-two path, although that path is unreachable for the
actual modulo-reduced roll. No extra bounds or optimizer assumptions are added.

The ordinary shared-fallback form reaches 39/42 words at the correct size;
three words differ only in the first arm's interval temporary register. A single
localized empty operand constraint retains both real initialized values at the
boundary after RNG/modulo: the unsigned delta uses native `$5`, while the roll
uses a generic `+r` operand. The subsequent low and interval tests consume these
values, and neither constrained lifetime crosses a call. Constraining the delta
alone coalesces the roll into `$5` and changes the test ordering, so it is not the
accepted reconstruction. The paired form emits the native roll in `$4`, delta
in `$5`, and complete branch/delay-slot/epilogue sequence.

This is a target-specific compiler aid, not evidence of original source spelling
or a claim about an unmeasured internal compiler pass. The diagnostic plain
and stock-dump complete-owner assembly pair has qualified parity; this
establishes output parity for that diagnostic source, not a debugger-hook trace
of the final source. No false provider declaration,
fabricated consumer, instruction-emitting asm or new pointer bounds are used.
The existing valid `EventStart` and unit-pointer contracts remain unchanged.

Independent final-source owner checking gives 84 match, 0 differ with normal
owned-data checks, including the previously published `rstSetCurrentDevil`.
Final-source gates pass the retail checksum, all 149 guarded owners / 599 drafts,
seven IRX owners, 26 tooling test files / 427 cases, shell mocks and documentation
checks. Source commit `cf18b20a8a57745e600ca3c1467549745b9cd8f8` passed exact-source
hosted CI [40982354001](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40982354001),
including build and deploy.

## cmbCopyEvoDevil (result/rstcalc, 0x100)

64 of 64 words match at 256 bytes with ordinary C. Declaring the real signed
skill-attribute load before the skill-form halfword load gives the native
address formation and register allocation. These are independent ordinary
observations before all calls; no volatile access, write or call lies between
them. Their source declaration order is not a claim about machine load order:
the exact output schedules the form before the attribute as retail does.
Grouping the real unit-level minus species-level difference before adding the
narrowed quotient then reproduces the native arithmetic sequence. No compiler
constraints, aliases, provider declarations or data layouts change.

The actual output preserves the pre-call skill/devil observations, the calls to
`cmbCopyEvoStats` and `cmbCheckEvoPair`, the conditional `cmbFindEvoEntry` call,
and the later unit-ID, unit-level and species-level reloads. The truthful
providers remain `u8 cmbCopyEvoStats(s16, s16)`,
`s32 cmbCheckEvoPair(u16, u16)` and `s32 cmbFindEvoEntry(u16)`.

The arithmetic regrouping is defined for every value of the declared fields:
unit level is `u16`, species level is `u8`, and the quotient is narrowed to `u8`
before addition. All promote to signed `int`; the difference ranges from -255
to 65535 and the final sum from -254 to 65791. There is no signed overflow.
Only results at least 100 are capped to 99. Other results, including negative
intermediates, narrow to `u8`; no lower clamp is added. Existing valid pointer,
index and table-key contracts remain required. A nonzero divisor is not assumed:
the inherited undefined C division / native break path for zero is unchanged.

For the original draft, 51 of 64 words match. After grouping the level difference,
55 of 64 words match; the attribute-first declaration completes the match. Diagnostic
plain and stock-dump complete-owner assembly, text and relocations have qualified
parity. This is output evidence for the diagnostic source, not a debugger-hook
trace or proof of the original spelling or an unmeasured internal comparator.

Independent final-source checking gives 85 match, 0 differ with normal
owned-data checks, preserving both `rstSetCurrentDevil` and `rstChkSkillPowerUp2`.
Final-source gates pass the retail checksum, all 149 guarded owners / 598 drafts,
seven IRX owners, 26 tooling test files / 427 cases, shell mocks and documentation
checks. Source commit `052a211d71c3b81c4f876ef3fe1a50fd195300d9` passed exact-source
hosted CI [40983613150](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40983613150),
including build and deploy.

## cmbCalcEvoEvent (result/rstcalc, 0x140)

80 of 80 words match at 320 bytes in ordinary C. The truthful signed-byte
candidate count is promoted into a real `s32` local, divided by two into a
real `s32 half`, then explicitly converted to `s8` at the comparison with
the ready-count call. Both providers retain their signed-byte return types
and original arguments. The first count call still precedes the ready-count
call; later entry observations, callbacks, result construction and writes
are unchanged.

For every possible signed-byte count, division truncating toward zero gives
-64..63, so the explicit narrowing preserves its value. Independent review
checks all 65,536 pairs of signed-byte returns for equal scalar selection;
this is not a whole-function execution proof. No nonnegative-count assumption,
false provider declaration, constraint or artificial consumer is introduced.

An earlier diagnostic used a quotient identity constraint, but independent
ablation removed it and still passed. The final ordinary source is exact;
the constraint is neither retained nor required. Diagnostic traces from other
variants do not establish the final ordinary source's internal compiler cause.
Independent final-source complete-owner checking gives 86 match, 0 differ,
with normal owned-data checks.

Combined local gates pass the retail checksum, 26 tooling test files / 427
cases, all 147 guarded owners / 533 drafts with stable source/compiler inputs,
seven IRX owners, and reconciled production reports.

### Production list-provider declaration

The exact `s32 datGetPlayerLNDList(u8 *list)` declaration is now visible in
production as well as guarded builds. Its same-owner provider conditionally
writes one byte through the actual list pointer and returns an `s32`
classification; `datGetPlayerLNDLevel` passes its real byte local and consumes
that classification. Only the declaration guard is removed. Provider and caller
bodies, call targets, memory accesses and ABI are unchanged.

Independent complete-owner verification gives 89 match, 0 differ. Across
production, NON_MATCHING and NON_MATCHING plus SKIP_ASM contexts, complete
allocated sections, non-file symbols, ordered relocations and normalized
assembly are identical before and after the change; plain/dump parity also
passes. No matching credit is added. The production implicit-declaration
diagnostic now has two inherited failures, `rstSetSkillActFlag` and
`cmpGetEncycWork`, rather than three. Both guarded contexts pass. This is not
a complete production strict-declaration repair. A paired setter declaration
control changes an exact caller and was rejected; the pointer-return chain
was unchanged in that earlier landing; the following repair addresses it.

Canonical qualification passes owner 89/0 with unchanged allocated data, fresh
139-owner / 479-draft compilation, 459 tests and 817 layout assertions, seven
IRX modules, inline/shell checks, retail checksum/byte comparison and build/report
no-ops. The 21,252 runtime inputs remain stable during these gates. Game
coverage remains 8595 functions and 1485932 bytes.

### Encyclopedia model pointer contract

The real `cmpGetEncycWork` provider returns `mdlObj_t *`. Its declaration is
now visible in every build, and the local in `rstGetEncycPanel` and parameter
of `rstFindUnitScene` carry that pointer type rather than an `s32` address.
The three consumers use ordinary byte-pointer views for the actual model
subobject pointer at offset 0x18 and preserve the native access at offset 0x80.
Calls, ordering, widths and values are unchanged. This fixes the local pointer
contract; it does not establish general host portability or new model layouts.

Independent provider/caller review and three-context complete-output checks
verify unchanged allocated sections, non-file symbols, ordered relocations and
normalized assembly. Six plain/dump pairs agree. Production owner remains
89/0; guarded comparison retains its existing 88/17 result. The production
implicit-declaration diagnostic now retains only `rstSetSkillActFlag`; both
guarded contexts pass. The rejected setter-declaration control is not included.
No matching credit is added.

Fresh canonical gates pass 459 tests, 817 layout assertions, 139 guarded owners /
479 drafts, seven IRX modules, inline/shell checks and exact retail checksum/byte
comparison. Complete owner data and output parity remain unchanged; build and
report are true no-ops after qualification, with stable runtime inputs.

## func_00222610 (rstcalc, 0xA8)

42 of 42 words match at 168 bytes in ordinary C. The scan considers the
selected row's eight Hearts condition/skill pairs in order. Nonzero pairs
advance an unsigned-byte count; reaching the supplied limit returns zero.
Otherwise a pair whose condition is at most the unit's unsigned level
appends its skill and increments the unit's unsigned-halfword skill count.
After the complete scan the count is returned through the signed-byte cast.

Direct `D_00339E00[id].heartConditions[i]` accesses produce one advancing
address for the actual row members. The original row pointer initialized
outside the loop emits 184 bytes and matches 5/42 words. Moving that genuine
row pointer declaration inside the loop also matches, as does indexing a
local table pointer. An actual entry-pointer walk reaches 38/42 at native
size but chooses different base/displacement values.

Qualified stock full-owner dumps locate the difference in `09.loop`.
The direct form combines condition/skill/condition addresses (UID139/88/59)
with giv57 and reduces them to one register plus 10/12/10. The original
pre-loop row pointer leaves its skill address separate and reduces only
the offset induction variable. The native table-plus-16 bias is generated
by the compiler, not written into the accepted source. No biased byte
cursor, type change, constraint or extra consumer is retained.

The existing record is 66 bytes; eight four-byte entries begin at +0x1A,
with skill halfwords at +0x1C through +0x38. Source accesses stay within
that real member array. A valid selected row and sufficient writable unit
skill capacity remain inherited preconditions; no bounds check or claim
for arbitrary invalid pointers is introduced. All native byte/halfword
reads, the repeated condition read, and count-before-skill stores remain.

Complete owner: 93 match, 0 differ. Nine full assembly/entire-ELF plain-dump
pairs cover final, original-row-pointer ablation and guarded owner across
production, SKIP_ASM and retained-draft contexts. Two stock trace pairs
also match the frozen builds. All 104/92/104 peers and owned data are
preserved, resolving expected changed local call displacements. The target's
production relocation changes from D_00339E10 to D_00339E00+16, resolving
to the same native address. Strict production and SKIP_ASM still encounter
the pre-existing rstSetSkillActFlag declaration gap; retained-draft strict
compilation passes. These measurements establish compiler reconstruction,
not original spelling or gameplay execution.
