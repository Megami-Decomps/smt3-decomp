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
