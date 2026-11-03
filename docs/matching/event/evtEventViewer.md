# Matching notes: event/evtEventViewer

## evtEventViewerProcess3 (event/evtEventViewer, 0x16C)

91 of 91 words match at the exact 364-byte size. The complete owner has
55 matching functions and no differences, including its owned data; the five
remaining guarded drafts compile.

The last three differing words were the pause test after `FrameSelect`.
Retail reloads `viewerinfo->flags` into `$2`, masks it there and branches on
it; the branch delay slot replaces the dead pause value with the zero return.
The ordinary expression instead reserved `$2` for the return and used `$3`
for the load, mask and branch.

A local signed `paused` scalar, initialized from the real post-call flags
reload, is bound to native `$2` in a block after `FrameSelect`. It has a real
branch consumer and is dead before any later call. This is a target-specific
compiler reconstruction aid, not evidence of original source spelling. No
empty asm operand is necessary: the declaration alone gives the exact owner.
With the generic empty `+r` operand alone, 74 of 91 words match; with the
ordinary expression, 88 of 91 words match.

Direct actual-compiler plain and `-da` compiles produce identical whole-owner
assembly after normalizing file names and compiler diagnostic comments, and
identical complete `.text` and relocation sections.
Internal debugger hooks were unavailable, so no unobserved allocator decision
is claimed. Behavior, calls, field widths, callback-sensitive reloads and return
values are unchanged. PiM supplied the local live-value binding and ablation.

## RunFrameUnit (event/evtEventViewer, 0x94)

37 of 37 words match at 148 bytes in ordinary C. The complete owner has
51 matching functions and no differences, including its owned data.
Source commit: `7e989fba5a29f13ca18a64420e83727423f2fca5`.
Exact-source [CI run 40970257246](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40970257246)
passed build and deployment.

The traversal uses an initial jump to the test and a shared next-frame
label. Dispatch is a switch on the actual signed-byte values: 0 and 1 update
`mode`, 2 through 4 update `otherMode`, and all other values leave both
unchanged. In the measured controls, both source properties matter: the
same switch in an ordinary `for` traversal emits 140 bytes, while the
labeled traversal with `if`
comparisons emits 148 bytes but differs in eight words.

Qualified compiler dumps explain the last difference. The switch's
`jump2` output keeps the first range branch targeted at the real `mode`
assignment, with the second range test on the fallthrough path. Delay-slot
reorganization can absorb that assignment and redirect the branch to the
next-frame label, producing retail's `bnel` and its assignment slot. The
`if` form instead targets the second comparison, which fills the branch
slot while a separate backward jump carries the mode assignment. Moving
the assignment out of line explicitly is compiler-inert. Whole-owner
plain/dump assembly agrees apart from the scratch `.file` pathname, and
its entire `.text` agrees. This identifies the observed compiler property,
not the original source spelling. No compiler constraints are retained.

The null and frame-time tests precede the payload read. The unsigned
16-bit frame is promoted before comparison with signed `nowFrame`; the
payload remains signed 8-bit. Each eligible node advances once, and the
first future node stops traversal. The defaults are 0 and 2, with the last
qualifying update winning independently for each mode. The two
`evtRunUnitMode` calls retain their order and separate `track->basic`
reads; the second remains the native tail call.

## evtRunFrame (event/evtEventViewer, 0x2BC)

175 of 175 words match at 700 bytes in ordinary C. Separate real flag clears,
initialization of `nearestFrame` only at the second traversal, and ordering of
independent pre-callback blur resets produce the native sequence. There are no
constraints, aliases, new types or declaration changes. All 51 previously
matching peers, including RunFrameUnit, and owned data remain exact.

The distinct reset fields have no intervening callback observation. Separate
`flags &= ~2` and `flags &= ~4` yield the native two AND operations and one final
store, not extra volatile observations. `nearestFrame` is initialized before
every consumer; its address is first taken in the second traversal. Both
traversals retain the real frame/track links observed after callbacks, and the
second reloads the global first-object pointer after the first traversal.
Camera publication, RunFrame3, quake handling and later flag/blur observations
preserve their native order and reloads.

Differences involving promoted signed-16-bit end-frame and unsigned-16-bit
frame/index fields are within signed-32-bit range. Total-frame subtraction and
frame-plus-length addition instead use explicit unsigned-32-bit arithmetic,
then convert to signed with the pinned target's bit-preserving behavior. This
preserves native wrap without invented overflow bounds or a universal ISO-C
portability claim. Existing valid pointer/list domains remain inherited; cyclic
or adversarially mutated lists are not newly guaranteed to terminate.

Independent final-source owner checking gives 52 match, 0 differ. Diagnostic
complete-owner plain/stock-dump assembly, text and relocation parity is verified;
no historical spelling or internal scheduler-decision claim follows from that.
Final qualification passes the retail checksum, all 149 guarded owners / 586
drafts, seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs checks.
Source commit `5fac154cda0e857968ae35f52c62aa13163e78de` passed exact-source hosted CI
[40993213967](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40993213967),
including build and deploy.

## evtStopFrame (event/evtEventViewer, 0x2A4)

169 of 169 words match at 676 bytes in ordinary C. Two signed track-type
switches retain cases 3, 18, 20 and 21 and skip every other signed value.
The real basic pointer is reused only after its earlier traversals finish;
the same correctly typed control pointer feeds the genuine model and motion
consumers in disjoint phases. There are no dummy uses, artificial lifetimes,
constraints, new types, prototype changes or ABI changes. All 52 prior matches,
including evtRunFrame, and normal owned data remain exact.

Both active-scene calls are retained. Unit, matter and mover traversal order,
post-callback next loads, and fresh effect-viewer observations are preserved.
Unit model-first is reloaded after Seek before Pause, while matter motion is
intentionally snapshotted. Zero `sw` seeks then pauses; every nonzero value
unpauses. Conversion of unsigned nowFrame to float preserves the high-bit
path, and the existing signed mover parameter receives the same low 32 bits
under the target ABI. Inner switch default advances the child, not the track.

Effect case 18 uses its work pointer. Cases 3/20/21 skip negative signed-byte
slots, without inventing an upper bound. The 127-entry array's slot 127 remains
outside the inherited valid index contract; this is not a claim that arbitrary
inputs are safe. Null selected basics and null mover payloads skip as native;
the native control-provider dereference still relies on a valid eligible
payload. No defensive check, list termination guarantee or callback cache is
introduced.

Qualified complete-owner plain/stock-dump assembly, text and relocation parity
covers the two-switch, basic-reuse and control-reuse controls. Initial RTL
assigns the same correctly typed control pseudo in the unit and matter phases;
its measured allocated home changes from v1 to v0. This supports truthful scalar
reuse, not an invented internal scheduling explanation or historical-source
claim. Independent guard-removed checking gives 53 match, 0 differ.
Final qualification passes the retail checksum, 149 guarded owners / 583 drafts,
seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs checks.
Source commit `08f344607276167c36247afc66440c54fce5cdcd` passed exact-source hosted CI
[40995572614](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40995572614),
including build and deploy.

## evtEventViewerProcess4 (event/evtEventViewer, 0x280)

160 of 160 words match at 640 bytes in ordinary C. Signed action-result and
menu-state switches preserve their complete domains. A forward label inside
the loop places the real PopMenu call after the normal continuation and then
exits once. Direct accesses use the existing menuStack entries, with initialized
x, y and mode argument locals. Reordering independent state stores and real
argument initialization recovers the native output without constraints,
volatility, artificial dependencies, shifted structs or before-array pointers.
No layout, prototype or ABI change is introduced.

DrawEffect precedes traversal. Index starts at one and compares live stack
depth; callbacks are followed by fresh menu/viewer observations. The first
dispatch result survives the optional supplementary mode-four dispatch, whose
result is intentionally ignored. Action zero records the current mode and
continues; minus one pops once and exits; every other signed action calls the
existing callback. Callback result one returns Process5_0 immediately, minus
one sets done, and every other signed result leaves done unchanged. Every
non-early callback path exits the loop.

The u16 x coordinate plus s16 window offset fits signed32; y and mode retain
zero extension. Window movement keeps its else-if priority and representable
signed16 stores. Process killing is followed by a fresh viewer reload before
clearing its current process slot. Menu state zero resets the real fields,
sets state one and pushes the menu; state one clears it and sets done; all
other signed states remain unchanged. The final nowmenu test and Process3
transition retain the existing EE32 function-pointer return convention.

The eight-entry stack's active indices one through seven and live object
contract remain inherited. The callback table still requires an eligible mode
with a real nonnull callback; no invented bound, defensive fallback or arbitrary
malformed-input termination guarantee is added.

Complete-owner plain/stock-dump assembly, text and relocation parity is
qualified for the ordered, argument-load and cleaned final controls. This is
measured source/output evidence, not historical-source spelling or an unobserved
optimizer explanation. Independent target-only rebasing onto published StopFrame
preserves its surrounding bytes and gives 54 match, 0 differ, including RunFrame
and StopFrame. Full qualification passes the retail checksum, 149 guarded owners /
582 drafts, seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs
checks. Source commit `127dbb80bec3b72661d7586294dfad93c54398e4` passed exact-source hosted CI
[40996592892](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40996592892),
including build and deploy.

## EnterData (event/evtEventViewer, 0x378)

189 of 222 words match at the native 888-byte size; the ordinary draft remains guarded.

EnterData's ordinary reconstruction was investigated without promoting it. The complete owner retains its 55 existing exact functions. The strongest ordinary body emits the correct 888-byte extent and matches 189 of 222 instruction positions, while preserving typed, unaligned parameter-record copies, actual lighting hardware copies, source-subobject origins and callback ordering. One separately retained selected-frame allocation diagnostic matches 203 positions but is also nonmatching.

The remaining differences concern pointer allocation, lighting address materialization and a post-light global observation. No fake volatile, padding, register search or memory barrier was introduced. Real callback arguments and the target-only declaration visibility adjustment were qualified separately.

Both diagnostic bodies pass strict production, SKIP_ASM and NON_MATCHING compilation. Full-owner plain/dump assembly, allocated sections, symbols and ordered relocations agree. The proposed guarded ordinary draft preserves the production fallback and checks 55 match, 0 differ; it earns no new matching credit. Independent review approved only the ordinary guarded proposal. The private investigation did not run full repository gates; final integration results are recorded below.

The canonical patch changes only the guarded ordinary definition and its
truthful `s32 EnterData(s32 action, s32 previousMenu)` callback contract.
Native ignores the incoming arguments and returns zero after evtRunFrame and
PopMenu. The separate 203/222 diagnostic binds a real selected frame, but
that binding and private diagnostic declaration-visibility changes are not
retained. Its higher score does not authorize promotion.

Provider copies use the qualified packed/unaligned record views and native
lighting helpers, with valid live allocations and the EE GNU object domain.
This is not a portable ISO C, overlapping/corrupt-storage or asynchronous
mutation claim. No source ownership or literal changes are made.

Final owner 55/0 and ordinary target-active 55/1 checks pass with data intact. Three strict modes, production/retained plain-dump parity, fresh stable 141-owner/481-draft qualification, 450 tests, 817 layout assertions, seven IRX modules, retail checksum/byte comparison/no-op and reconciled unchanged reports pass.

## evtAddFrameInfo (event/evtEventViewer, 0x540)

305 of 336 words match at the native 1344-byte size. The ordinary reconstruction
remains guarded, with 55 exact peer functions and valid owned data and jump table.
There is no matching credit or source promotion.

The explicit type-0 no-op is supported by the real stage-track constructor and
native 24-entry dispatch table. It restores the table's lower bound without
inventing a case or claiming historical spelling. Six memcpy calls copy actual
provider subobjects into distinct live allocations: 0x28 blur parameter bytes, 0x2c
multi-blur, distortion and ripple parameter bytes, 0x18 filter bytes and 0x24
multi-filter parameter bytes. Allocate, publish the work pointer, call the getter,
copy, and initialize the frame flag remain separate ordered operations. The
normal live-object contract excludes overlap, malformed pointers and asynchronous
mutation; these are representation-byte copies, not numerical reinterpretations.

The shorter decimal 0.52359874f emits native binary32 0x3f060a91. Both it and the
previous long spelling mathematically round to that value, but the pinned compiler
emits 0x3f060a90 for the long spelling. Raw retail bytes, compiler assembly and
object data establish the discrepancy; no internal parser cause is inferred.

Type 18 initializes the real end-frame halfword normally. The unexplained
redundant low-byte write is omitted rather than justified by false volatile or
native presence alone. Final initialized state agrees under the ordinary-object
contract, but the draft does not reproduce the complete native memory-write
sequence. A separate 305/336-word diagnostic retaining that store is not retained
or approved under the dead-code exception. Remaining differences include store
ordering, registers, lighting/fog addresses and scheduling.

The allocator declaration now agrees with the provider's signed s32 parameter.
All local requests are small positive constants; paired production, SKIP_ASM and
retained output contracts are unchanged by that declaration correction. This is
not a demonstrated runtime bug. Independent semantic review, strict compilation
and complete allocated-section, non-file-symbol, ordered-relocation and normalized
assembly comparisons support guarded retention, not an exact match. No invented
allocation checks, extra fog-tail initialization, layout changes or constraints
are introduced.

Integrated qualification: owner 55/0 and target-active 55/1 with owned data intact;
three strict modes and complete plain/dump parity pass. Fresh stable 140-owner /
480-draft sweep, 459 tests, 817 layout assertions, inline/shell checks, seven IRX
modules, retail SHA1/byte comparison/no-op and reconciled reports pass. Game
coverage remains 8594 functions and 1485744 bytes; this batch earns zero credit.

### Ordinary fog-subobject refinement

A real sdf3DFogParam_t pointer initialized to &dds3LightParam.fog after the
type-11 allocation improves the guarded target from 302/336 to 305/336 words,
still at the native 1344 bytes. Five field reads use that exact subobject;
allocation, minimum/color stores, work-pointer publication, maximum/near/far
stores and the final flag retain their ordering. There is no intervening call,
escaped pointer, new qualifier, constraint, layout change or tail initialization.
The native 0x20 parent offset and five field offsets are independently verified.
Seven fog register/scheduling differences remain, along with other residuals.

This ordinary 305/336 result is distinct from the rejected 305/336 diagnostic
that retained the redundant type-18 byte store. That store remains absent and
the earlier rejection remains in force. Neither result is an exact C match.

Whole active-control text also changes six EnterSelect jump-table words despite
unchanged EnterSelect source. They move from a known SKIP_ASM-only mismatch to
the native sequence; the production function was already exact. Complete owner
and paired output-contract checks qualify both contexts, without claiming an
isolated target-only text change or any additional matching credit.

Fresh integrated qualification preserves production owner 55/0 and confirms the
305/336 active diagnostic with data intact. Three strict modes and full canonical
and active plain/dump parity pass; production and SKIP object contracts equal the
prior published version. The fresh stable 140-owner/480-draft sweep, 459 tests,
817 layout assertions, inline/shell checks, seven IRX modules, retail byte
comparison/SHA1/no-op and unchanged coverage reports all pass. Zero exact credit.
