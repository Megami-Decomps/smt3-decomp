# Matching notes: fclCombineInit

## func_00206308, func_002065C8 (fclCombineInit, 0x2C0 each)

Both directions reproduce all 176 native words. The routines copy the same
fields between two layouts, with exchanged source and destination offsets.
The early word and halfword remain separate typed operations. In the
middle group the actual footer word is loaded and stored, then the actual
footer halfword is loaded and stored. The two same-register lifetimes are
disjoint; no temporary lives across another write to that register and no
empty operand is required. The real destination stays in its native
incoming register.

The 36-, 32- and 576-byte blocks retain their existing halfword-aligned
representations. The compiler emits the native block-copy alignment check
and loop paths; no alignment assumption, restrict qualifier, replacement
copy procedure, or artificial read is added. Each individual block copy
requires valid readable and writable storage at its native offsets and
cannot be assumed to support partial input/output overlap. No claim is made
about arbitrary overlap or about the original source spelling.

Earlier drafts swapped the initial transfer and alignment-test register
homes. Folding their data into one aggregate changed the native halfword
load, and forcing a tied destination operand disturbed scheduling. The
current source preserves the transfer widths and all existing owner bytes.

## cmbDrawFullScreenPanel (fclCombineInit, 0x68)

26 of 26 words match at 104 bytes. The former 20/26 scheduling residual is
superseded. The five calls retain their original order: disable RGB, draw
sprite 0x34000, draw the box, allocate the reference GIF tag, and tail-call
the same sprite draw. Providers retain truthful void interfaces and every
argument, packet/callback effect and conversion is unchanged.

Four localized reconstruction aids apply only to actual initialized values.
Each sprite call has a separate scoped `s32` ID of 0x34000 and an empty
`FAKEMATCH_ASM_LIVE_VOLATILE` operand immediately before its real consumer.
Neither ID is pinned, carried across an earlier call or exposed by address.
For the box call, real `s32 boxX = 0` and `s32 boxOt = flags` values are bound
to their actual EE argument registers `$4` and `$10` immediately before the
seven-argument call. All other arguments are constants, with no intervening
call or side-effecting argument evaluation. These bound values are consumed
by that call and never relied on after its clobbers; `flags` remains an
ordinary value independently live for later calls.

No artificial consumer, memory barrier, instruction replay, false ABI or
compiler flag is introduced. The constraints emit no instructions. Exact
output initializes box X before OT, then supplies the remaining real box
arguments and the native call with zero color in its delay slot. This is a
measured target-specific source/output relationship, not an internal compiler
pass explanation, hardware requirement or original-source spelling claim.

The ordinary-X control gives 19/26. Five deletion-only controls from the
exact source also fail: removing OT gives 20/26; first ID gives 1/26 at
116 bytes; tail ID gives 22/26; both box bindings give 20/26; both ID operands
give 20/26. All other controls remain 104 bytes, with peers/data preserved.
These results support the retained four aids and establish no tested simpler
exact form, not absolute irreducibility over all possible reconstructions.
The old nonvoid-prototype experiment is historical negative evidence; its
unsupported contract is not retained.

Independent canonical checks of both frozen and comment-cleaned sources give
193 match, 0 differ, with owned data exact. The ablation runs were independently
inspected rather than independently recompiled. No diagnostic compiler-pass
or normal/diagnostic parity claim is made.

Final landing gates pass the retail checksum, all 27 test files (436 cases),
the inline-assembly gate, all 145 guarded owners / 505 drafts with stable
inputs, seven IRX checks and reconciled reports. Source commit
`29f5a04f4e09bd446ef79dba7caa69fbf9971ad8` passed exact-source hosted
[CI run 41377958870](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41377958870),
attempt 1, including build and Pages deployment. Raw logs verify the exact
checkout and deployed commit; the run completed successfully on 2026-11-02
at 02:32:35 UTC.

### Matched result footer

`cmbDrawResultFooter` is ordinary exact C (116 bytes). Its four corner colors
remain `[color, color & ~0xFF, color, color & ~0xFF]`, between panel sprites
`0x48` and `0x44`. No source arithmetic or store-order change was needed.
The complete default owner checks182 match,0 differ with only this former
guard promoted; other guarded bodies need not be visible.

The old four-word register residual is resolved by the current truthful
`void fclDrawPanelSprite2(s32, s32)` contract, adopted in777f9340. In the
current owner, restoring only that declaration to `s32` reproduces25/29
matching words; restoring only `cmpDrawBoxGDefault` to `s32` retains29/29.
The historical report that the void sprite declaration was inert does not
reproduce in this context and must not be used to park the current body.

Qualified actual-owner allocation traces pass plain/hooked/unhooked output
parity for both declarations. The combined mask/result quantity85 has the
same five references and lifetime10..36 in both; local allocation selects
`$2` with the actual void sprite contract and `$3` in the non-void control.
Its priority remains3846. This isolates the declaration-dependent register
choice without inventing a scalar use, constraint or compiler flag.

## cmbDrawResultPanel (fclCombineInit, 0x228)

138 of 138 words match: 552 emitted and native bytes. The complete owner
checks 183 match, 0 differ. Source commit:
`6b234980219e4cf7cc9a003d0e7daa790456e374`. Exact-source
[CI run 40956924261](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40956924261)
passed build and deployment. The local target/retail, report, tooling and docs gates pass;
the final rebase rechecked three changed guarded owners and carried the
remaining unchanged input closure from the full 149-owner / seven-IRX pass.

Retain the native three-way footer dispatch: mode 1 and default use
`0x5FA5E240`, while mode 2 uses `0x698AEC40`. The identical colors in case 1
and default do not justify collapsing their separate call sites. Initialize
`alpha` to 256 after the footer calls, as native does. These two ordinary C
changes together reproduce the owner; either change alone remains non-matching.

Keep the signed `(0x200 - phase) / 2` calculation, the later recomputation
from `sacrificePanelAnim`, and the existing global-pointer reloads across
calls. No dummy work, opacity, constraints or ABI change is required.

## cmbStartResultScript (fclCombineInit, 0x84)

33 of 33 words match at 132 bytes in ordinary C. The complete owner has
184 matching functions and no differences, including its owned data.
Source commit: `6f214449f849679f552b9d80cf03e49b14f7c88e`.
Exact-source [CI run 40972478153](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40972478153)
passed build and deployment.

Use the actual two-case switch, with case 2 written before case 1. Case 2
selects script entry 6, case 1 selects entry 5, and all other signed-byte
values retain the initialized entry 0. This source order gives retail's
`beql` with the entry-6 assignment in its slot and the correct shared
publication block. The inherited `if`/`else if` form emits 120 bytes;
the switch with case 1 written first emits 140 bytes. No empty cases,
compiler constraints, ABI changes or dummy operations are needed.

Qualified dumps show the case-order effect. With case 2 first, `jump2`
leaves its inequality branch targeted at the shared join and the entry-6
assignment on the fallthrough path. Delay-slot reorganization reverses and
annuls that branch into the native equality test with entry 6 in its slot;
the entry-5 block then falls into common state publication. The opposite
case order leaves both assignments out of line, adding a jump/alignment
and duplicating the state-byte store. For both switch orders, whole-owner
plain/dump assembly agrees apart from the scratch `.file` pathname, and
text, data and relocations agree. This describes observed compiler output,
not the original source spelling.

The signed enable byte still gates all state changes and the script call.
When enabled, the state byte at offset 5 is set to 1 before script setup.
The current global work pointer is reloaded for the script and argument
address, and again after `fclScriptStart` before its actual return value is
stored at offset 8. The declaration and return-value consumer remain
unchanged.

## cmbGetBirthPhase (fclCombineInit, 0x88)

34 of 34 words match at 136 bytes in ordinary C. The complete owner checks
185 match, 0 differ, including owned data. Source commit: `85baa43edce88819d8bde6f068752a63814a2a1c`.
Exact-source [CI run 40974879934](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40974879934)
passed build and deployment.

Use the real switch cases 5, 6 and default, in that order. Each arm performs
its own selected stock-byte lookup from the group's signed 16-bit row plus
unsigned 8-bit cursor column. Mode 5 selects group 0, mode 6 selects group 1,
and every other signed mode selects group 2. Interpret the selected stock
byte as signed 8-bit, select that party entry's unsigned 16-bit unit ID and
tail-call `cmpModelSetup(id, 0)`. The valid cursor and party-index contracts
remain unchanged; no fabricated bounds or new pointer assumptions are used.
There are no intervening callbacks, new stores, constraints or ABI changes.
The same per-arm expression shape appears in matched `cmbDrawStatus`.

The inherited selected-cursor draft emits 120 bytes. Real per-arm row/column
snapshots reach the native 136 bytes but only 12 of 34 words; the actual
switch order reaches 22. Reversing the two cases reaches 14. Narrower truthful
snapshot types and a separate unit-ID local do not improve 22; factoring only
the sum into the arms reaches 15. Putting the complete real signed stock-byte
lookup in each arm produces all 34 words without artificial cases or work.

Qualified compiler dumps retain three stock-byte accesses through `sched2`,
then merge their common suffix in `jump2`. The resulting shared sum uses $2,
the signed stock byte uses $4, and the DDS3 work base loads into $3 at the
native point. Early source-level factoring instead allocates the sum to $3
and the DDS3 base to $6, hoisting the latter. Whole-owner plain/dump assembly
agrees apart from the scratch `.file` pathname; text and relocations agree,
and text agrees with the canonical checker's full object, for both the
snapshot-switch control and exact per-arm form. This is observed compiler
behavior, not proof of the original source spelling.

## cmbUpdateStockCursor2 (facility/fclCombineInit, 0x22C)

139 of 139 words match at 556 bytes. The sole body correction removes the local
cursor-subobject pointer and expresses the same two observations directly
through `CMB_GBWK->stockCursorGroup[2].cursor`. The original unsigned-character
low-byte view of signed16 `unk_22` and unsigned8 conversion of signed8 `unk_24`
are retained. No alias, constraint, new type or fabricated address is added.

The real cursor starts at global+0x30D0; its fields are at +0x30F2 and +0x30F4.
Retail reads the byte at +0x30F4 before the byte at +0x30F2. The draft's local
subobject pointer materializes an extra +0x30D0 address and shifts the prefix
until alignment absorbs the instruction, leaving 117 of 139 words matching.
Direct whole-object fields produce the exact native addresses, observation
order and registers. Target-compiler layout assertions verify these offsets and
the corresponding stock-slot fields. Complete-owner plain and stock-dump
assembly, text and relocations agree for both diagnostic forms. This is paired
output evidence, not a claim about an unmeasured compiler comparator or original
source spelling.

Both observed bytes promote to int, so their sum 0..510 cannot overflow. The
existing target signed-byte narrowing is preserved. The selected byte is
published and reloaded for the stock map, the mapped stock index is published
and reloaded for `cmbStockIsLocked`, and a fresh global is observed after that
callback before the result lookup. Later movement, input and sound calls and
all native callback-dependent reloads remain unchanged. The disabled-result
early return and lock-failure button 0x8000 retain their behavior. Existing
valid cursor, map, stock and result-table index contracts remain required;
arbitrary negative or oversized indices are not claimed safe. Provider types
and call targets are unchanged.

Independent final-source owner checking gives 186 match, 0 differ with normal
owned-data checks, including the earlier result-panel, result-script and
birth-phase matches. Final-source gates pass the retail checksum, all 149
guarded owners / 588 drafts, seven IRX owners, 26 tooling test files / 427 cases,
shell mocks and documentation checks. Source commit `dd0e8255e7b22c6bbb7cffbe2cc6564281a4b28d` passed exact-source
hosted CI [40991191105](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40991191105),
including build and deploy.

## cmbCalcBirthDevil (facility/fclCombineInit, 0x434)

269 of 269 words match at 1,076 bytes. The production owner has 187 matching
functions and no differences, including the previously published StockCursor2
and normal owned-data checks. The reconstruction uses a real initialized phase
flag, reuses it for the later boolean, shares the two actual zero-return exits,
and dispatches results 10, 11 and default with an ordinary switch. The unit
snapshots, independent third-slot observations, complete unit copies, panel and
availability callbacks, fresh result pointer, birth-kind publication and skill
callback retain their native order. No register constraints or opaque aliases
are retained.

The match also corrects a consistent availability argument boundary: both
`cmbGetBirthDevil` declarations now take `s32`, and its actual guarded definition
receives `s32 availableInput` and immediately initializes `s8 available` from it.
All later provider uses consume that narrowed local. All three native callers
forward the availability result directly into argument register 8. The native
provider itself performs a left shift by 24 followed by an arithmetic right
shift by 24 before observing that value. Thus every incoming 32-bit pattern is
converted to the sign-extended low byte, as in the explicit conversion under the
pinned target compiler. This is an evidence-backed representation of the native
machine boundary, not uniquely recovered historical C signature spelling. It
is not a caller-only false declaration, and does not assume the producer can
return only zero or one. Signed narrowing is target/compiler-defined.

The final-source counterfactual restoring the narrow declarations and definition
emits 1,084 bytes with redundant caller-side sign extension; its owner is 186/1.
The shared real zero-return label separately resolves the remaining initial
allocation differences. Complete-owner plain/stock-dump assembly, text and
relocation parity is qualified for the diagnostic source; it is not a hooked
allocator trace or a claim about an unmeasured compiler decision.

The provider is still guarded and is **not** credited as matching. Its old and
new C diagnostics emit 1,256 and 1,264 bytes respectively, versus retail's 1,252,
with the same owned-rodata context warning at 0x2A9. Its C output changes; it is
neither invariant nor exact. The other guarded caller also remains nonmatching.
Production continues to use those assembly implementations. Existing pointer,
stock/table index and callback contracts are inherited without extra bounds or
termination claims.

Independent final-source checking gives 187 match, 0 differ. Full qualification
passes the retail checksum, all 149 guarded owners / 587 drafts, seven IRX
owners, 26 tooling files / 427 tests, shell mocks and documentation checks.
Source commit `f0b8209e21c8000163a002297e0f1a3045fbeb23` passed exact-source hosted CI
[40992706526](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40992706526),
including build and deploy.

## cmbUpdateBirthDevil (facility/fclCombineInit, 0x3B8)

238 of 238 words match at 952 bytes in ordinary C. Direct snapshots of the
real signed-byte stock indices replace broad draft volatile aggregate views;
two actual unit pointers recover the native address calculations. The idle
success arm comes first, and explicit phase alternatives retain the exact
`index == -1` distinction. There are no constraints, register bindings, new
types, changed prototypes or ABI changes. The previously documented BirthCalc
availability boundary and all 187 prior matching peers remain unchanged.

Both unit pointers are formed after the model-reload callback and consumed
before another call. The second address is formed unconditionally, but its ID
load remains short-circuited after the first comparison. This retains the
engine's valid selected-party-index contract, not safety for arbitrary negative
or out-of-range indices. Model, script, ribbon and cursor callbacks and their
required global reloads remain in native order. The signed unlock result and
its nonnegative test are unchanged.

Writing the unsigned-16-bit level comparison as birth greater than player is
value-equivalent over its complete domain and produces the native load order
after the event-bit callback. This is measured output, not a claim that C
operand spelling mandates left-to-right evaluation. Zero-first load-status
branching is equivalent for every return value; it does not assume only the
provider's currently observed zero/one/two results.

Independent guard-removed full-owner checking gives 188 match, 0 differ with
normal owned-data checks. Complete-owner plain/stock-dump assembly, text and
relocation parity is qualified, and the plain text equals the checker object.
Full qualification passes the retail checksum, 149 guarded owners / 584 drafts,
seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs checks.
Source commit `8923543b5266dd2dbc7a61ad10f3fdf035bf2fe1` passed exact-source hosted CI
[40995071024](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40995071024),
including build and deploy.

## cmbDrawBirthDevil (facility/fclCombineInit, 0x274)

157 of 157 words match at 628 bytes in ordinary C. The genuine `hasBase`
flag is initialized before the first panel callback, a signed switch selects
cases 2 / 8 / 9 / default, and an ascending loop clears the actual six-byte
local delta array. The compiler emits the native descending clear. No
constraints, register bindings, artificial objects, type or prototype changes,
or ABI aids are introduced. This measures the SMT3 output; it does not prove
historical source spelling or a particular internal optimizer pass.

The early flag initialization is invisible to callbacks: its address never
escapes and its later adjustment guard is a real consumer. Dispatch preserves
every signed birthKind value, including negatives, and the type-12 conversion
still depends on the signed third stock index. Delta stores are unobservable
until clearing completes, with no intervening callback or escaped array
pointer. Both stat loops use the real six-element bounds. Display and base
remain complete, distinct unit objects with lifetimes covering the callbacks.

Panel, mode, party and ribbon observations retain their callback-sensitive
reloads and native call order. Cases 8/9 still observe selected->id after the first copy callback.
Parameter differences are within -39..39;
the signed-byte parameter update retains the native low-byte narrowing.
The base panel pointer is cleared before the mask callback on every mode-3
path; the default path never exposes an uninitialized base. Existing valid
party/table index contracts remain unchanged, without invented negative-index
safety. The full signed hideRibbon argument still uses only its zero test.

Independent guard-removed full-owner checking gives 189 match, 0 differ,
including all 188 prior peers and normal owned-data checks. Earlier BirthCalc,
BirthUpdate and stock-cursor matches are unchanged. Full qualification passes
the retail checksum, 149 guarded owners / 575 drafts, seven IRX owners,
26 tooling files / 427 tests, shell mocks and docs checks. Actual-published
verification repeats the owner, retail, report, all 91 owner dependencies,
all 780 tracked files and final Ninja no-op checks. Source commit
`fb259583d38dc35d3e818448e3f1356171a52554` passed exact-source hosted CI
[41001938999](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41001938999):
including build and deploy.

## cmbUpdateRoot (facility/fclCombineInit, 0x1F4)

125 of 125 words match at 500 bytes in ordinary C. The actual cursor-sum
expression directly combines the original two byte observations, and the
selection-1 action tests the signed-byte stock count below two first. A separate
sum with that branch order reached 117 of 125 words; the direct expression
resolved the remaining allocation/branch differences. No constraints, false
types, fabricated bounds or artificial values are used. This measures the
reconstructed compiler output, not historical source spelling.

The index observation is the unsigned byte at cursor+0x22, not the enclosing
signed16 field; the offset at +0x24 is also observed as an unsigned byte.
The real promoted sum spans 1..511, with low-byte storage and a separate
signed-byte selected value. Wrapping is preserved for the full byte domain,
not merely menu-valid indices. The signed-byte conversion is verified for
this compiler/native output, not claimed portable across C implementations.
Independent review checked all 65,536 input pairs. There is no intervening callback or write
between those two observations, and emitted access order matches native.

The original u16 button snapshot remains live across callbacks. A zero flag
still exits before common pad handling. The insufficient-stock path preserves
script argument publication, script result, and independent global work
reloads; script startup can alter flags and script state. Selection 3 keeps
the signed selected value across its callback, selection 4/button 2 retain
cmbExit, and default selection/button behavior is unchanged. Cursor movement,
up/down handling, pad disabling and sound retain their order and original
button conversions. No global pointer is cached across callback-sensitive
observations.

Independent complete-owner verification gives 190 match, 0 differ,
preserving all 189 peers and normal owned-data checks. Full qualification
passes the retail checksum, 149 guarded owners / 572 drafts, seven IRX owners,
26 tooling files / 427 tests, shell mocks and docs checks. Actual-published
verification repeats the owner, retail, report, all 90 owner dependencies,
all 780 tracked files and final Ninja no-op checks.
Source commit `40e9d025e6951ae2a2951eec3f8a2146f8de58f9` has exact-source hosted CI
[41005099595](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41005099595):
build and deploy both completed successfully.

## func_00201FB0 (facility/fclCombineInit, 0x94)

37 of 37 words match at 148 bytes after recovering the local drawing chain's
void return contract. The target, its declaration, and the already matched
`cmbDrawPhase3` and `cmbDrawPhasePanel` are consistently void. Early exits
use bare returns; panel arms call their helper and return without propagating
incidental register contents. Only the target is new matching credit; both
existing peers retain exact emitted bytes.

This correction uses native bodies and actual consumers, not merely an
ignored result at one call site. The target and phase-three helper finish
after the void list-selection update without constructing a result. The
panel dispatch tail-calls those helpers. All recovered sequence callers
reach a common path that overwrites the return register before reading it,
then constructs its own result. Independent direct-call scanning agrees with
source references; an aligned-word scan finds no absolute function pointers
for these three functions. These scans corroborate the recovered contract,
not the absence of every hypothetical computed indirect target.

Arguments, conditions, callback order and memory effects are unchanged. No
value is fabricated from the void provider. The original singleton emits
152 bytes with an extra zero-result instruction; the corrected chain passes
independent complete-owner checking at 191 match, 0 differ, with owned data
exact. This recovers a usable contract without claiming historical spelling.

## cmbUpdateCameraPos (facility/fclCombineInit, 0x14C)

83 of 83 words match at 332 bytes. An ordinary real index starts at four
before the buffer clear and changes to three only for a zero birth variant
observed afterward. Both valid indices have actual model/address consumers;
no field observation moves across the clear. Three ordinary `s32` casts
replace scalar-assembly conversion calls in this target. Existing supported
VU vector storage into the aligned four-float mark buffer is unchanged.

All three assembled conversion words are `0x46000064`, matching retail at
0x20534C, 0x205360 and 0x205374. This is raw instruction and complete-output
evidence, not an inference from a mnemonic or RTL pattern name. No rounding
mode is changed or assumed, and no portable C guarantee is claimed for NaN
or out-of-range inputs. The initialized zero coordinates and model-derived
coordinates both have real work-value consumers. Callback-sensitive global
and model reloads and subsequent model/action calls retain native order.

The combined owner, including the void drawing-chain correction above,
passes independent canonical checking at 192 match, 0 differ, with normal owned-data
checks. No new constraint, binding, false type, provider change or compiler
flag is introduced.

Combined local gates pass the retail checksum, 26 tooling test files / 427
cases, all 147 guarded owners / 533 drafts with stable source/compiler inputs,
seven IRX owners, and reconciled production reports.

## cmbCalcRoot (facility/fclCombineInit, 0x170)

92 of 92 words match at 368 emitted/native bytes in ordinary C. Two source
properties are needed together: use a switch for the general-status-to-script
entry mapping (0 to 1, 1 to 2, everything else to 0), and direct the initial
zero exit to the existing trailing zero return. The two return-one paths
remain separate and every provider call and callback-sensitive work read is
unchanged.

The forced original body emits 356 bytes with 17 of 89 covered words matching
and 12 native bytes uncovered. Changing only the switch gives 84 of 92 words matching
at 368 bytes; its eight residual words are in the initial work-pointer,
zero-exit and process-handle setup. Changing only the shared zero exit gives
18 of 89 covered words matching at 356 bytes. Neither deletion control is exact. Actual-status
operand and local-binding controls did not improve on the ordinary switch
and are not retained.

The forward branch uses the existing matched shared-return idiom, including
`RetryMapLoad` in `field/fldProcess`; see
[control-flow levers](../../levers/control-flow.md). This is an observed source
and output dependency, not evidence of original spelling or a newly traced
internal compiler decision. There is no new constraint, volatile access,
register binding, declaration, layout, arithmetic, compiler flag or helper.

Complete owner and owned data check at 194 match / 0 differ. Plain versus
`-da` complete assembly and ELF-object comparisons pass for the guarded
original, forced original, each single-change control and combined source in
production, SKIP_ASM and retained-draft modes, with only the equal-length
random canonical source filename normalized. Resolved peer code and allocated
nontext data are preserved. Retained-mode strict declaration checks pass;
raw production and SKIP_ASM strict checks retain the same pre-existing
implicit-declaration failures outside this function. The existing
`dds3GetProcessStat` declaration also differs from its canonical provider;
this change does not modify it, and native's returned 0..3 values retain the
same equality-to-two behavior.

## shop_206A38 (facility/fclCombineInit, 0x118)

70 of 70 words match at 280 bytes. Initialize the real row index directly,
retain the entry StepY scalar and apply its eightfold spacing at each row,
and bind the genuine font priority to native argument register 6 immediately
before its sole font-call consumer. This per-iteration initialized `s32`
priority is dead after argument delivery. No earlier call crosses its live
interval, and the subsequent draw/destructor calls may clobber that register.
No empty operand, dummy consumer, extra memory effect, fabricated type or
compiler flag is added. The ordinary priority local leaves 68 of 70 words
matching: its final materialization and the x argument copy exchange places.
Adding a tied empty operand gives 66 matching words; input-only gives 46.
These are measured output properties, not proof that no ordinary spelling
could match, a hidden scheduler explanation or historical source spelling.

Independent native/provider review confirms signed cursor offset plus unsigned
byte selection, unsigned row count, and entry snapshots of traversal bounds
and StepY. Empty lists make no font calls. Each row loads the current work
pointer, eligible-stock id and text pointer, then requests, draws and destroys
the same font request with unchanged arguments and order. Work/text changes
between callbacks remain visible; cursor/StepY changes do not alter this
traversal. The final native loop-delay-slot work-pointer read is retained.
Valid eligible-stock/text indices and representable coordinate arithmetic
remain inherited contracts; the normal 16-entry stock array is not proof
that malformed cursor values are safe. No bounds or masking were invented.

Complete owner: 195 match, 0 differ. Twenty-one development whole-assembly and
entire-ELF normal/stock-dump pairs cover seven sources in three modes. Six
fresh integrated guarded/final pairs pass with 176 stable collected inputs.
All 214/194/214 production/SKIP_ASM/retained peers and owned data are unchanged.
Independent GP-relative load resolution gives all 280 native target bytes.
Production/SKIP_ASM strict checks still fail with identical baseline implicit
declaration diagnostics and an inherited pointer warning; they are not clean
passes. Retained strict checks pass. No hooked compiler trace is claimed.

Optimized and ASan/UBSan host fixtures each pass 1,080,000 actual-source/reference
comparisons across 180,000 valid-domain cases with callback mutation. They
replace the register binding with an ordinary host declaration and do not
establish EE runtime behavior or arbitrary-index safety. Leak detection is
disabled in the sanitizer fixture. Integrated retail checksum/byte comparison,
generated-source validation, 505 unittest cases without skips plus 818 layout
assertions, frozen guarded EE/IRX checks and reconciled reports pass. The
previous map/recovery matches remain exact; configure changes no source
imports. Only this 280-byte function gains credit: game totals become 8,655
functions and 1,503,504 bytes. The subsequent build has no work.

## cmbDrawBirthDevilMaskBG (facility/fclCombineInit, 0x160)

88 of 88 words match at 352 bytes. After the event callback, capture the real
fusion-work root, load the birth level, and read the two background dimensions
through that full root. The sprite argument remains its actual member address.
Separate dimension scalars and color stores recover the native argument
schedule. Both roots are observed only after the event call; every work read
is complete before rendering begins, so later callback mutation needs no reload.
The original event and unsigned level gates, one mask draw and four resource
draws remain in order with unchanged arguments and providers.

The birth field is u16. A u32 snapshot preserves its entire value domain and
the mixed-width comparison's ordering. One localized binding places this real
value in native register 3: `lhu` at target +0x2C defines it and `sltu` at +0x40
is its sole consumer, with no intervening call. It is dead before drawing.
The ordinary u32 declaration gives 84 of 88 words, differing only in the two
root/level register assignments, party-load base and comparison operand.
This is a target-specific allocation aid, not original source spelling or a
claim that no other ordinary form can match. There is no false type, dummy
consumer, artificial lifetime, added memory access or changed compiler flag.

Independent semantic and compiler reviews confirm the native accesses,
providers and lifetime. Twenty-four development whole-assembly and entire-ELF
plain/stock-dump pairs cover eight sources in three modes. Nine fresh integrated
pairs cover guarded, activated-original and final source in production,
SKIP_ASM and retained-draft modes. Exactly one equal-length canonical source
filename is normalized; no object sections or symbols are omitted. All
214/195/214 peers and allocated nontext data are preserved. Resolving the two
ordinary GP relocations recovers every native target byte. Complete owner:
196 match, 0 differ. Production/SKIP_ASM strict checks still fail on inherited
declarations and retain the existing pointer warning; activation removes the
former target implicit-declaration diagnostic and adds none. Retained strict
checks pass. No hooked decision trace is claimed.

Optimized and ASan/UBSan development fixtures each pass 3,310,720 comparisons
of 20 actual bodies. A fresh integrated final-body fixture passes 165,536
cases in each mode, including each individual u16 width and height, event and
level exits, callback root mutation and supplied-color mutation. This is not
the full width/height Cartesian product or a native renderer test. Constraints
are ordinary declarations in the host fixture; target output qualifies their
real behavior. Valid work/resource objects remain inherited preconditions,
and leak detection is disabled in these allocation-free fixtures.

Integrated on main through 5d487c81 with peer changes preserved. Complete
retail checksum and byte comparison, generated-source validation, all 505
unittest cases without skips plus 818 layout assertions, frozen guarded EE
and seven IRX owners, and reconciled production reports pass. The seven latest
affected peer owners also remain exact. Only this function gains credit:
game totals become 8,656 functions and 1,503,856 bytes. A subsequent build
has no work. No private fixture, runtime or compiler artifact is published.

## cmbDrawUnitLevel (facility/fclCombineInit, 0x1C0)

112 of 112 words match at 448 bytes in ordinary C. One real X coordinate
advances from the race column (original X minus 32), through the name column
(original X plus 1120), to the level column (original X plus 2880). The name
pointer snapshot and nameX are immediately consumed by the name font call
while X advances to the level column. There is no unrelated scratch reuse,
dummy consumer, binding, optimizer assertion or added qualifier.

The initial active/color flags are captured at their native epoch. The ID is
reloaded after the race draw/destruction sequence for the name; ID and u16
level are reloaded after the name sequence for the species-level comparison.
The species level is u8. The real level provider clamps to 99, and the %2d
string fits the 32-byte buffer. All eleven provider events preserve their
order, coordinates, color, text, draw mode, OT and request identities. Null
font handles still reach draw and deferred destruction as native does.
Existing opaque handle declarations retain the actual providers' pointer ABI.

On the active path, defined coordinate arithmetic requires original X in
[INT32_MIN + 32, INT32_MAX - 2880] and original Y in
[INT32_MIN, INT32_MAX - 24]. The continuous calculation adds no overflow
domain beyond the original direct expressions. Flag-clear returns perform
no coordinate arithmetic. IDs/races and strings retain existing game/provider
validity requirements; synthetic fixture tables are not a game-bound proof.

The enabled original draft emits 436 bytes: 12 of 109 overlap words match,
with 12 native bytes uncovered. Moving only the name-pointer snapshot after nameX leaves 448 bytes but changes
10 of 112 words, starting at +0xCC. Qualified compiler dumps independently show
the live X advances and the name-load/copy/advance order. This is observed
source-order-dependent output, not an assertion of a scheduler tie-break
mechanism or historical source spelling.

Twelve fresh guarded/original/control/final whole-owner plain/stock -da pairs
preserve entire assembly and ELF objects after exactly one equal-length source
filename normalization. The final owner is 197 match, 0 differ; all
214/196/214 peers and allocated nontext sections remain intact. Production's
complete allocated bytes and all 1,778 ordered relocation records are unchanged.
The 19 target relocation contracts agree across modes, including the owned
fclCombineNum2dFmt at .sdata+0x3C versus its external reference. Its %2d bytes
also match retail. Strict production/SKIP retain identical inherited failures;
retained strict builds pass with the same inherited pointer warnings. No inherited failure is counted as a clean pass.

The fresh independent exact-body/actual-level-provider fixture passes 262,144
optimized and 262,144 ASan/UBSan cases, warning-free. Complete eleven-event
traces and final state cover every initial u16 ID/level, color/flag choices,
null-handle combinations, defined coordinate boundaries and callback mutations.
Stale ID, stale level and skipped-null-draw negative controls all fail as
expected. This is not a full renderer, allocator or game integration test.

Full linked retail bytes, generated-source negative controls, 505 tooling tests
without optional skips, 818 layout assertions, frozen EE/IRX drafts, inline-asm
policy and primary/audit report scope pass. Flow10 33/0, music 27/0 and recovery
391/0 complete-owner checks preserve the preceding landings.

## cmbUpdateCameraZoom (facility/fclCombineInit, 0xE8)

58 of 58 words match at 232 bytes in ordinary C. Each native mode arm loads
the signed target index separately and dispatches grouped cases. Mode one
accepts targets zero through four plus terminal target five; every other mode
accepts zero and one plus terminal target two. Signed switches preserve the
negative and out-of-range no-op domain and reproduce the native call layout.
The former range-test draft emits 200 bytes, per-arm target acquisition 216,
and separate negative exits 224. No constraint, type, declaration, data table
or compiler option is added. This establishes an exact source shape, not
historical source spelling or an internal compiler decision.

The early zoom load, signed action-step/frame tests, 80/220 frame boundaries,
positive-zero override and original model-drawing arguments are preserved.
The same-owner provider remains `cmbLoadTargetModel(s32, s32, f32)`. Every
accepted path calls it once; all other paths return without a call. Callback
mutation of globals is not followed by additional state reads.

Complete owner: 198 match, 0 differ after preserving the later mask and unit-level matches. Twelve full assembly/ELF route-parity
pairs cover baseline and final production, SKIP_ASM and retained contexts.
All 214/197/214 peers and owned data, layouts and relocations are preserved,
including the subsequently matched mask and unit-level renderers.
Independent review matches all target bytes in each context. Optimized and
ASan/UBSan actual-source fixtures each pass 2,494,464 comparisons covering
signed domains, frame boundaries, floating representations and callback
mutation. Leak detection is disabled in the sanitizer fixture; it uses no
heap allocations. Strict production/SKIP declaration failures and the pointer
warning are identical inherited debt; retained strict checks pass.


**Birth-step dispatch reconstruction.** The guarded `cmbRunBirthDevilStep`
draft now uses the actual cases 5, 6 and 8 rather than an equality chain.
Only when signed byte +4 equals 1, it reads signed byte +3 and calls
`cmbSetupBirthPanel` with 0, 1 or 2 respectively; every path then returns
`cmbGetBirthPhase`. No callback, access width, return contract or order changes.
The ordinary switch recovers the native selector tree and all three call blocks:
140 emitted bytes and 33/35 words, versus the old 116-byte draft's 8 matching
words in its 29-word prefix. It remains guarded and earns no matching credit.

The residual is precisely the return-address restore occupying the case-5
conditional branch delay slot at +0x3C rather than the following unconditional
branch delay slot at +0x44. Normal and diagnostic complete-owner assembly and
raw object output are identical. The real compiler's delay-slot dump clones
the common epilogue restore into that conditional branch; this is not an
argument allocation, selector lowering or extra runtime operation problem.
Six real-case source orders, per-arm returns, explicit default break, early
return, nested comparisons, one-pass control and a real selector read operand
were bounded diagnostics: none matched. No constraints are retained. All
199 other functions and owned data check exact; exhaustive 65,536 signed-byte
state pairs preserve the original and native selection and argument behavior.

## cmbDrawStockRow (facility/fclCombineInit, 0x144)

81 of 81 instruction words match at the native 324-byte extent. The sprite,
blink, two gradient, second sprite, and solid-box calls retain their original
order and arguments. The two 4-entry color buffers reuse the same real
stack array and preserve each native store, including the first gradient's
fourth color in its call delay slot.

A single local binding gives the actual unsigned byte returned by narrowing
the signed blink result its native callee-saved register. This preserves
the low-byte mask before the first gradient call and uses that byte only
for the final box's coefficient. No empty constraint, volatile color
access, artificial dependency, or separate saved full-width blink value is
needed. The widened product uses unsigned arithmetic; the result is 0..75.
Second X and final Z retain the low-word native arithmetic, avoiding signed
overflow. This is a target-specific reconstruction, not a claim about
the original source spelling.

The real compiler's normal and diagnostic complete-owner assembly and
object outputs agree. Independent replay checks 8,192 paired native and
compiled cases, including 4,096 executions of the retail blink provider
covering all 256 low bytes. It verifies physical color write order and
ABI arguments at the scoped drawing-provider boundaries, not GPU output.

### Historical stock-row scheduler frontier

An output-qualified trace of the earlier 67/81 guarded version placed
its low-byte mask across the first box call in post-reload scheduling, not in
initial expression lowering or allocation. The mask is already ready at cycle
21. The real comparator chooses the final stack color store instead: its
critical-path priority is 17 versus the mask's 12, and the comparator returns
-5 at the priority tier. No register-weight, dependence-class, successor-count
or final-order tie-breaker decides this comparison.

That store discharges the call's remaining dependency. The scheduler selects
the first box call within the same cycle while the mask remains in the ready
list; the mask is not selected until cycle 26. There is no direct mask-to-call
dependency requiring this order, and the trace does not show a separate sorted
comparison of the mask against the newly released call. Consequently, changing
only declaration order or a final tie breaker does not address this observed
frontier. A future source change needs a real computation/use or control-flow
reason to change the critical path; an artificial color dependency or general
memory barrier is not justified.

The pinned compiler observer captured 4,824 events with all six baseline and
observed artifacts byte-identical and 853 frozen inputs verified before and
after. Same-path direct replay also agrees with the ordinary wrapper, and
fresh plain/stock-dump complete-owner assembly and objects agree. This is
compiler-property evidence for the guarded 67/81 candidate, not the scheduler
mechanism of the matching replacement above.

## cmbDrawSequence (facility/fclCombineInit, 0x27C)

159 of 159 instruction words match at the native 636-byte extent (2026-11-16).
Both 17-entry switch tables retain their case-to-destination mapping. The
independent, break-terminated cases are listed in native block order, not
replaced by computed gotos or a fabricated dispatch table.

Case 14 retains an equal-arm decision on the existing signed `actionFrame`
counter, initialized with the facility work and reset before phase 14 entry.
It executes the same real camera update once on either path. The counter
has live introduction, timetable and camera consumers; no dummy state,
volatile access, numerical range assumption or extra callback is added.
This is a mechanistic compiler-deletion reconstruction, not evidence of
original branch spelling or a historical meaning for the threshold.

Fresh complete-owner ordinary and `-da` output agrees literally in assembly
and ELF. In the final source's actual dumps, the field load (UID432), signed
comparison (434), decision (435) and both camera calls (443/456) remain through
`25.sched2`. Adjacent `27.jump2` removes the decision and duplicate call while
retaining the load and comparison. `29.dbr` puts the comparison in the remaining
call's delay slot, reproducing native LW/JAL/SLTI120 at
0x0020231C–0x00202324. This establishes the observed pass boundary, not a
uniquely identified internal optimizer routine.

Cases 5, 6 and 8 reload the real work pointer after `cmbDrawTopText`, then read
its signed display flag. Only the root uses a localized native `$2` binding;
it dies before the next call. Removing the three proposed flag bindings
preserves all production allocated bytes and relocations, so the final source
uses ordinary `s32` flag locals. A fresh deletion-only control from this final
ordinary-flag context removes the three root bindings and leaves 15 differing
words (144/159) at the same size, with all 214 peers, raw relocations and
allocated nontext unchanged. This is distinct from the earlier six-word
root-removal result with fixed flag locals. The conditional result
panel and subsequent stock-panel calls retain their order. These constraints
express target-specific allocation dependencies, not hardware requirements.

The existing `fclCombineSeqChangeMsg[32]` object is explicitly 16-byte aligned.
Retail's second table ends at 0x0053CC54, followed by 12 zero alignment bytes,
the unchanged string at 0x0053CC60, and the next table at 0x0053CC80. This
preserves the actual object and extent without inventing padding storage or
claiming the original declaration. A fresh unaligned-source control fails
six owner checks through the eight-byte string displacement; the corrected
and subsequently simplified owners pass 201 match / 0 differ.

Independent review covers source semantics, callback epochs, the causal
trace, both switch tables and complete peer/data contracts. Production's
214 peers and allocated nontext bytes are unchanged; SKIP's 200 and retained
C's 214 peers preserve their resolved contracts, with address shifts and
target-owned tables explicitly attributed. All three contexts pass literal
ordinary/dump output parity. Strict production and SKIP compilation still
fail on the same 13 pre-existing implicit declarations as baseline; retained
strict compilation passes. No strict failure is waived or described as a pass.
Current guarded-draft coverage, IRX syntax checks, 794 regression tests,
generated-source controls, full retail byte equality, no-op rebuild and
production report scope checks pass. Earlier lost diagnostic packets are
historical attestations; the ablations and final qualification above are fresh.

## cmbRotateModelPanel (facility/fclCombineInit, 0x27C)

159 of 159 words match at 636 bytes. The mode switch keeps the original
bottom and height rules, and the incoming radius is reused as its doubled
diameter. Each of the 64 iterations emits six vertices and their colors in the original order; it leaves all W
fields untouched.
The second angle wraps from segment 63 to segment 0.

The degree increment, pi multiply, and divide are separate binary32
operations in the retail order. The first conversion and its genuine degree
product use their native FP temporaries before the first trig call; no
fixed temporary survives a call. The second degree product is prepared
among the independent first-endpoint stores and consumed before the next
trig call. These local compiler aids are not original source spelling.
The native cosine and sine call order and each post-call center read are
preserved. All existing C functions and owned data in the unit still match.

## cmbUpdateTimeTable (facility/fclCombineInit, 0x188)

98 of 98 words match at the native 392-byte extent. The signed action mode
selects one of the two tables (4 and 7 twelve-byte rows). The row index
starts at zero and advances only if the next row exists and its frame is
due under the native unsigned comparison. The increment uses the fresh
work record after the transition diagnostic.

The dispatch uses the resulting mode and index and calls only a non-null
event of mode 1 or a due event of mode 0. The early current-row pointer
and late event pointer use the native register at separate last-use
boundaries. The mode byte and row index determine the same dispatch
address; the localized operands retain their native scheduling. No
arithmetic, bounds check, provider or handler is invented to force the
match.

The signed periodic diagnostic uses the live frame value for both the
remainder and the printed argument. The increment uses the fresh work
pointer after either callback and retains the native low 32 bits.

## cmbGetModelMarkPos (facility/fclCombineInit, 0x238)

142 of 142 words match at the native 568-byte extent. The three action
slots advance independently through the action records, camera marks and
output vectors. The function clears each found flag before the lookup;
it only fills the size and positions after the slot has a model and
the camera-point lookup succeeds. The provider-supplied work record is
observed again where the native code reloads it across callbacks.

The smoke branch copies the full aligned camera vector, then offsets its
three spatial components by the current model center. The other branch
uses the real VU model-mark provider and stores only XYZ, leaving W
untouched. The existing VU macros implement those native 16-byte
handoffs. The narrow empty operands preserve actual live action offsets,
source pointers and model-table setup. They do not impose a general
memory barrier or synthesize a new callback dependency.

All 205 other functions and owned data in the complete translation unit
remain exact.

## Sacrifice fusion timeline

The sacrifice route selects seven 12-byte entries. The initial row has no
callback; the remaining six retain their original frame values, dispatch
modes and callback addresses. The four bytes before the separate timeline
group descriptor stay raw. The ordinary timeline and both group pointers
are unchanged.

## Fusion labels and model marks

The three header captions keep their original pointers and the separate
null slot. The model-mark loop reads three existing name pointers. The
next camera-point pointer is not part of that array; its native four-byte
boundary stays raw because the stock standalone source object aligns it
to eight bytes.

## Fusion sprite resources

The three sprite pages use 35 main-menu, seven icon, and 25 stock-header
resource records. Each record has the native 14-byte stride. Ordinary C
alignment before the existing placement arrays reproduces the 6, 6,
and 2 intervening bytes. The preceding raw word remains separate.

The original 24-byte page layout is a distinct object, with the same
six resource and placement pointers. Its separate source owner retains
all six named relocations rather than converting the two stock-page
references to local section addends.

## Triple-race comparison row

The two permutations each test precisely the one native four-byte row: three
race bytes and its result byte. The following zero word is separate alignment.

## cmbGetRaceCombineResult (fclCombineInit, 0x1F8)

The race result search matches all 126 native words. It sorts the existing
race-specific table, filters each matching row through the four original
predicates in order, then scans the resulting halfword IDs. Eligibility
and the requested level remain separate: equality is accepted, the first
level strictly below the request selects the previous accepted row (or the
first row if none was accepted), and exhausting the list selects its tail.
The zero-race and empty-result exits still return zero.

The row view starts from the entire allocated array. The row and pointer
member are accessed at their real stride and offsets, with the original
halfword index loads. The filter's actual cursor and limit retain the loop
entry and callback-exit scheduling. In the selection loop, the stored
threshold and accepted-row flag retain their native stack accesses. The
real accepted index advances only on the continuing path; its localized
empty operand retains the loop comparison and update order. Fixed register
bindings and empty operands preserve only the native addresses, loaded
values, and selector state used by these operations. They emit no substitute
instructions and do not establish the original source spelling.

## Model-panel vertex and color storage

The panel draw visits two banks, advancing vertices by `0x1800` bytes and
colors by `0x600` bytes. The generator writes 64 groups of six vertex and
color records in each selected bank, and the draw reads the same vertex
groups. The C definitions own `0x3000` zero-fill bytes at `0x56D190` for
vectors and `0xC00` zero-fill bytes at `0x573190` for color words. The
unused tails after both banks stay raw. The interior generated labels
superseded by the vertex array have no references. The consumer, retail
file, and initialized bytes are unchanged.

## cmbDrawResultHeader (0x201068)

64 of 64 words match at 256 bytes. The opening panel sprite, three
resource draws, and final tail-called panel sprite retain the native order
and exact arguments. Each resource draw uses a freshly observed work
object's actual texture-handle member and the same sprite table. The first
two resource calls have fixed depth and draw order; the third uses the
incoming depth shifted by four with 32-bit wrap, and the incoming draw
order. The original provider types are unchanged.

The compiler constraints only place real values. The two panel IDs have
short-lived local bindings in their actual argument register and are
converted through the existing s32 prototype. The second resource call
keeps the fresh work pointer and computed texture member in separate
compiler phases. A fixed binding is unnecessary: the ordinary texture
pointer still reaches the outgoing argument register with both live
operands retained. The last resource call observes the genuine incoming
depth and current work
pointer storage together before forming its arguments; the empty operand
is not a memory barrier. No fixed argument is relied upon after a call.
Without the second resource call's two live operands, its texture address
addition and zero X setup exchange order.

Whole-owner checks and an isolated production link verify all affected
code and data. The ten target operands resolve to the retail addresses.
The assembler reorders three peer HI16 associations for the same
`cmbSpriteTable` symbol, each with the same signed low addend of zero;
the effective relocation expressions, ordered non-HI16 peer records,
allocated peer bytes and typed records are unchanged. The linked image
matches all 4,551,696 retail bytes. An independent instruction-decoding
fixture checks native and candidate outputs across callback-driven work
replacement, call arguments and callee-saved restoration, with a stale
work control.

## cmbFindBirthResult (facility/fclCombineInit, 0x1DC)

119 of 119 words match at the native 476-byte size. The routine sizes its
temporary race list from the initial signed count, passes the count's low
halfword to the sorter, and guards the first row and subsequent scan before
reading them. The first-row match and the incrementing scan use the same
native zero-based index. After locating the input, the routine reads the
walk bound and tries the adjacent entry in the requested direction. It
checks the bounds and race before calling all four rejection predicates,
in order. It reloads the entry ID for each predicate and before returning
an accepted ID. The normal caller's direction and table-size preconditions
are unchanged.

The initialized neutral scalar seeds the real scan index and default walk
step, then becomes the rejection accumulator after that phase. The local
compiler operands preserve the native cursor and short-lived argument
homes without padding the array, inventing a predicate input, or adding a
call. The exact output takes the first row's halfword from the transient
stack pointer after the sorter, with the real scan initializer and step
store in their native positions.

The complete owner and its data preserve all peer code and relocations,
and the full stock link remains byte-identical to retail. Verification
includes the native sorter and predicate call sequence, and an independent
build of the C body with injected row replacements and scan-bound cases.
