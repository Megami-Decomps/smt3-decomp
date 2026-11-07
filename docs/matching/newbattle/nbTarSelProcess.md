# Matching notes: newbattle/nbTarSelProcess

## UpdateSelect (newbattle/nbTarSelProcess, 0x174)

93 of 93 words match at 372 bytes. Ordinary nested switches, direct indexed
clearing and genuine shared comparison/eligibility continuations reduce the
residual to 16 words. Two scoped register bindings resolve the demonstrated
allocation differences: the actual initialized selection flag in `$18` and
the case-zero signed candidate in `$3`. Final-source ablations without those
bindings differ by 14 and three words respectively. There is no empty asm,
fabricated consumer, artificial extra use or instruction-emitting assembly.
These are target-specific reconstruction aids, not historical-source evidence.

Independent plain versus `-da` complete-owner comparisons qualify both the
ordinary control and final source: normalized assembly, complete text and
relocations, initialized data and BSS/SBSS sizes agree. Traces establish the
observed allocation difference, not an unsupported allocator-cause claim.
The real flag may span CheckFriend/CheckEnemy calls, but both exact 68-byte
leaf providers neither touch callee-saved `$18` nor call anything else or
invoke callbacks. The target preserves the caller's saved registers. Candidate
`$3` is initialized on each path reaching its genuine comparison and dies
before any call; unsupported areas do not read it. Neither binding spans a
clobber or retains a value solely for pressure.

Exactly 16 formations are visited. Bit 8 is cleared unconditionally, and
eligibility tests bit 4 from the original stat. Signed ctype 0/1/2 and its
entire default domain remain; unsigned area 1/2 and every unsupported area
retain separate behavior. Case zero preserves signed nowno indexing and
signed friend/enemy candidate loads, writing nowform only on equality.
Case one calls exactly the corresponding truthful s32 provider and normalizes
its genuine result at the common join. Native providers return only 0/1,
so the old raw-result flag assignment was already semantically equivalent,
not a bug. Their pre-existing C `formindex - 4` expression is not claimed safe
for arbitrary extreme s32 inputs: UpdateSelect supplies only 0..15, and the
providers are unchanged. Case two selects every bit-4-eligible formation.

The final bit-8 update freshly reloads process data and formation storage
after possible provider calls and reads current stat; no stale absolute
formation pointer is retained. Layouts, access widths, call order and inherited
valid-pointer/signed-index contracts are unchanged. Invalid nowno or corrupt
pointers are not newly made safe, and no bounds or fabricated fields are added.

Independent normal whole-owner verification gives 18 match, 0 differ,
including all 17 peers and owned data.
Full qualification passes the retail checksum, 148 guarded owners / 569
drafts, seven IRX owners, 26 tooling files / 427 tests, shell mocks and docs
checks. The guarded owner remains present with four drafts instead of five;
all peer counts are unchanged.
Actual-published verification repeats the owner, retail/report/provenance,
all 24 owner dependencies, all 781 tracked blobs and final Ninja no-op checks.
Source commit `4a723899b0e4392df56dbb089ee06745496c3e60` has exact-source hosted CI
[41008129760](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41008129760):
build and deploy both passed.

## CursorSelect (newbattle/nbTarSelProcess, 0x294)

40 of 165 words match in the native extent; 115 differ and 40 native bytes remain uncovered.
The draft emits 620 bytes against the native 660-byte extent.
The C remains `NON_MATCHING`. Unsupported volatile process and pad RAM views
are replaced by ordinary typed accesses. The initial unsigned state snapshot
has a real later area-count consumer and stays live across the optional sound
call; ctype and selection fields are still read after that call. The raw
friend-count byte snapshot is interpreted as signed for its two positivity
tests. Both movement directions and the final area-switch stores retain their
native order. No bounds, pointer-distinctness promise, helper, constraint or
compiler option was added.

The reproduced earlier draft emitted 728 bytes against 660 native bytes:
7 positional matching and 158 differing native-prefix words, with 68 excess
bytes. The new draft emits 620 bytes: 40 matching and 115 differing words,
with 40 native bytes uncovered. The native extent is 165 words, excluding
alignment. This is a source-quality cleanup, not an exact match or new
matching credit; shorter output alone is not the justification.

Qualified full-owner ordinary/-da dumps identify two remaining dependencies.
The initial jump pass contracts the enemy-positive branch into a conditional
assignment; later combine yields a single comparison. GCSE explicitly marks
the second friend-byte left shift redundant and reuses the first shifted
value across movement logic. Native retains the branch and two shifts.
Localized snapshot-binding and live-operand controls did not resolve these
residuals and are not retained. Area-branch layout also remains different.
These are actual RTL-stage observations, not hooked-compiler or original-source
claims.

Independent semantic review covers all signed-byte count/cursor domains,
local arithmetic bounds, callback and store order, and snapshot lifetimes.
On area 1 the ordinary compiler reads the signed count before the raw byte,
where native reverses those reads; they commute with no intervening call or
write. No asynchronous-memory or identical-access-trace claim is made.
Local exhaustive byte-domain checks supplement the review without proving
whole-program valid indexes for UpdateSelect.

The guarded production owner remains 18 match, 0 differ. Target-active
checks preserve all 18 peers and owned data. Production object contracts
remain identical to the earlier guarded owner. Strict production, SKIP_ASM,
and NON_MATCHING/SKIP_ASM compilations pass. Full-owner plain/dump parity
covers all four allocated sections, including the 152-byte owned rodata,
148 non-file symbols and 66 ordered relocations in production.

## OnOffTargetCursor (newbattle/nbTarSelProcess, 132 bytes)

All 33 of 33 words match at the native 132-byte size. Ordinary local mask
selection supplies the real boolean value, and one nonvolatile empty `+r`
operand keeps the actual data pointer available at the decision/write boundary.
This target-specific reconstruction aid retains native formation-pointer
re-observation; it is not a memory barrier or an original-source claim.
No fixed register, volatile storage, extra consumer, type, layout or compiler flag
change is needed. Removing the operand from this same source emits 92 bytes,
with the formation base hoisted and the separate offset inductions collapsed.
An earlier mask-register binding was unnecessary and was removed.

The data pointer is captured once from the process. Every one of 16 iterations
reloads the byte state and, only for state one, reads the current formation's
32-bit status. The selected boolean is normalized from status mask 0x8. The formation
pointer is then reloaded before the 32-bit anime read/modify/write, which sets
flag 0x8 for a qualifying formation and clears it otherwise. Other bits,
240-byte stride, ascending formation order and all accesses are preserved.
The pointer operand lies within its real condition/write/loop lifetime and
never crosses a call; this function has none.

The old guarded volatile spelling is not retained. Valid process, main-data
and formation storage is still required; no new disjointness, alignment or
state-stability promise is introduced. The earlier four emulator vectors
produced no memory changes and are not used as a general semantic proof.
Independent native access, alias and caller review covers the retained source.

Both complete-owner checkers report 19 match, 0 differ. Final and same-source
ordinary controls pass strict production, SKIP_ASM and retained-C plain/dump
complete assembly and ELF parity. All 21/18/21 other functions and owned data
are preserved in those contexts after resolving code-relative relocations.
Full retail, generated-source, regression, guarded-source and report gates
accompany this source-only landing; private tooling and traces are excluded.

## CheckFormCursor (newbattle/nbTarSelProcess, 388 bytes)

The native extent is `[0x25DA78, 0x25DBFC)`: 97 words, excluding the
following four-byte alignment zero. The unchanged target-active draft emits
364 bytes: 26 of its 91 prefix words agree and 24 native bytes are uncovered.
The canonical C remains guarded and the retail assembly fallback is unchanged.

Explicit zero/one switches and an initialized conditional side value recover
the native 388-byte extent, with 66/97 positional words agreeing. A separate masked local
for the final rule-eight test gives 60/97 at the same size. Replacing only
the initial side normalization with the matched sibling's ordinary masked
`u32` local and nonzero comparison instead emits 392 bytes: 30/97 native-prefix
words agree, 67 differ, and four bytes are excess. None is a matching result.
These CheckFormCursor controls add no constraint, volatile RAM, false type, invented bound,
helper or compiler-option change.

Qualified ordinary compiler dumps distinguish several problems. The first
dispatch control's initial jump pass creates a constant-one pseudo for the
conditional normalization; first CSE reuses it for both switch comparisons.
The masked-side control removes that seed and sharing lineage, but does not
recover the native dispatcher or final-arm continuation. Separately, the
last signed-range arm is converted to a result in the initial jump pass.
The original rule-eight expression expands to xor/and; its separate masked
local initially gives the desired and/unsigned-compare, but GCSE reuses an
earlier bit-one mask and later conditional execution changes its guard.
These are retained-stage observations, not an executed internal decision
trace, proof of one cause for every difference, or original-source evidence.
Do not repeat these controls as allocation-only experiments.

Independent native and source review preserves the actual normalized side
domain, signed formation comparison, unsigned wraparound range comparison,
rule order and conditional formation/party reads. The sole call remains
`GetTarSelProcessData`, at target offset `0x2C`; no callback follows it.
Existing valid-storage and index assumptions remain. The old emulator-vector
comment applies to the old draft, not these new controls.

Across the unchanged source and four target-active sources, fifteen complete
ordinary/dump assembly and ELF comparisons pass, as do fifteen comparisons
against the actual-build-order direct ordinary route. Only the single generated
source filename is normalized. All 21 production, 19 assembly-skipping and
21 retained-draft peers preserve their complete code and ordered relocation
contracts; owned data remains unchanged. The original direct production object
also equals the current build object. These checks qualify the
observations and peer preservation, not a full-link acceptance of inexact C.


## AnimeTargetCursor (newbattle/nbTarSelProcess, 540 bytes)

All 135 of 135 words match at the native 540-byte extent. The final function
is ordinary C: four initializer-sized arrays, a separate pre-copy `nextTime`
snapshot and post-store `time` reload, and real scale-result locals calculated
A then B and stored B then A. There is no added constraint, fixed register,
volatile object, fabricated bound, extra consumer or compiler option.

Each array has exactly seven real native values. Declaration/literal order is
alpha A, scale A, alpha B, scale B; the native runtime copies execute alpha A,
alpha B, scale A, scale B. Time increments through the existing unsigned-word
addition, resets on signed `nextTime >= 30`, then reloads the stored value
before signed division/remainder by five. Initialization and reachable updates
keep time in 0..29, so both interpolation endpoints stay in the seven-element
arrays. No new guarantee is made for corrupt state producing negative indexes.
The two separately rounded products and addition per channel, integer alpha
conversions, and alpha A/B then scale B/A stores retain native behavior.

The exposed baseline has 49/135 matching words at 540 bytes. Inferred bounds
and native literal order reach 81/135; a diagnostic time binding and ordinary
scale phases reach exactness. Separating the two genuine time lifetimes then
removes the final binding. Final-source controls that merge those lifetimes,
restore direct scale stores, or explicitly fix all bounds at seven retain
83/135, 129/135 and 83/135 respectively, each at 540 bytes. The selected bounds
are truthful initializer-derived extents, not a false alias or storage promise.
DDS array-copy observations supplied a lead; measured SMT3 output parity does
not establish a historical source spelling or an unobserved scheduler cause.

Source-owned tables require regenerated imports. The old assembly-backed
object owned 152 rodata bytes beginning at 0x5442B8. The final production
object emits the first 140 native bytes; the next owner's verified 16-byte
alignment supplies the remaining 12 native zero bytes before 0x544350.
SKIP_ASM and retained C expose 124 native table bytes without the assembly-only
name and padding. Raw object-section equality is therefore not claimed.
All other allocated data and all peers preserve their resolved contracts;
full-image equality verifies the literal and alignment span together.

Independent semantic/compiler/native-data review approves the final source.
The complete owner is 20 match, 0 differ. Fresh integrated production,
SKIP_ASM and retained-C ordinary/dump pairs preserve complete assembly and
ELF output, including three final-source controls. Strict declaration checks
pass. Optimized and ASan/UBSan exact-body fixtures cover all 30 reachable times,
sentinel bytes, transitions and valid reset states; earlier broader numeric
fixtures supplement them without claiming arbitrary-input PS2 FPU emulation.
The combined battle landing passes full retail, generated-source, regression,
retained EE/IRX and report/provenance gates. It preserves OnOffTargetCursor,
UpdateSelect and all remaining guarded drafts.
