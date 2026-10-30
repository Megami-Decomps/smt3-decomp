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
