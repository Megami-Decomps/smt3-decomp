# Matching notes: newbattle/nbNegoProcess

## nbSetAllHojoSkillWork (newbattle/nbNegoProcess, 0x184)

97 of 97 words match at 388 bytes in ordinary C. Direct indexed
`n->data->party[i].hojoskill[j]` stores and indexed flag accesses replace
redundant party/slot temporaries. The baseline reached 94 of 97 words;
a direct slot address reached 95, indexed zeroing reached 96, and indexing
all affected fields resolved the last address-word difference. No constraints,
register bindings, ABI views, type changes or new bounds are used.

Qualified plain versus `-da` full-owner builds preserve normalized assembly,
complete text and relocations for the indexed controls and final source.
Initial RTL exposes scaled-index-plus-base rather than base-plus-scaled-index
operand order for the final address. This is measured ordinary expression
lowering, not proof of historical source spelling or an inferred later-pass
mechanism.

The native allocation is 0x700 bytes: 14 party records of 0x80 bytes, each
with eight u16 support-skill slots at +0x70. Each iteration keeps the eight
halfword zero stores, the fresh party-base observation, and flag-bit 0x80
clear. Eligibility remains the first four parties with flag bit 0 set,
excluding the current party index, in the original short-circuit order.
`nbGetUnitWorkPointer` still precedes `datCheckTalk`; work and skill observations
follow those calls, and destination/global bases are reloaded for insertions.
The skill-count test is not cached: native reloads it after each qualifying
store, preserving the alias-sensitive behavior.

Skill type 2 and request mode 7 retain their short-circuit checks and native
index arithmetic. The existing valid provider, skill-table and count domain
is inherited, not strengthened: this change does not promise null-provider
safety, valid access for corrupt skill IDs, more than 24 source skills, or
more than eight selected support skills. No clamp or callback assumption
absent from retail is introduced.

Independent complete-owner verification gives 42 match, 0 differ, preserving
all 41 peers and normal owned-data checks. Full qualification passes the
retail checksum, 149 guarded owners / 571 drafts, seven IRX owners,
26 tooling files / 427 tests, shell mocks and docs checks. Actual-published
verification repeats the owner, retail, report, all 34 owner dependencies,
all 780 tracked files and final Ninja no-op checks.
Source commit `7af97f5ab372e5640fb0c10b642610395ec96cf0` has exact-source hosted CI
[41006111982](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41006111982):
build and deploy both completed successfully.
