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

## nbSearchHojoSkillWork (newbattle/nbNegoProcess, 0x3CC)

243 of 243 words match at 972 bytes. Signed skill-type switches recover the
native dispatch and nine-entry owned jump table. The four-party collection,
empty return, count-at-least-two sorting guard, signed parameter comparisons
and stable ties retain their native order. Unit lookup precedes the used-flag
check. Each party contributes at most eight eligible slots; random selection
uses only the initialized nonempty prefix. All equal unmarked selected slots
are marked, and the result combines the party index and selected skill.
Type 3 skips 0x1A2 and accepts 0x19C only after the actual race comparison.
Inherited valid provider, party and table domains are not widened or clamped.

Retail reads an eligible support skill twice with `lhu`: the eligibility read
at 0x26B5C0 and the dispatch read at 0x26B5DC. A local empty volatile `+m`
operand names the actual initialized `u16 hojoskill[j]` lvalue and prevents
reuse of the first read. It emits neither an instruction nor a store; the
following read is the real consumer. Volatile qualifies the asm, not the
ordinary party object. This is the SMT3 localized compiler-constraint aid,
not a memory barrier or evidence of original source spelling.
Removing only the constraint folds the reads and produces 956 bytes;
volatile-pointee alternatives produce 980 bytes with unwanted conversions.
The selected skill local also serves the later real selection phase without
a dummy consumer or additional constraint.

Independent complete-owner checking gives 43 match, 0 differ, including all
42 peers, the nine resolved switch targets and owned data. Integrated retail
checksum, 427 tooling tests, guarded EE/IRX and report checks passed.
Source commit `b4a94d9d52ae7f6944a05ba55a72f8257ef1e2a4` has green exact-source CI
[41034646323](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41034646323).

## nbNegoProcess1 (newbattle/nbNegoProcess, 0x290)

164 of 164 words match at 656 bytes in ordinary C. The original ordinary
candidate loop already compiles to the native peeled first iteration: a
nonpositive count exits, and matching candidate zero avoids an extra
selectcur-zero store. Explicitly splitting that iteration increased the
664-byte baseline to 680 bytes and is not retained. This match does not claim
a newly reconstructed loop optimization.

The actual required change shares a neutral index between callback-table
selection and the later candidate loop. The first value is the upper sixteen
bits of u32 nownegoid, exactly representable in s32; it dies after callback
target calculation. Every later selection entry resets index to zero before
its real comparisons, addressing and selectcur store. No old index or incoming
state is cached across the callback. Signed count behavior and the original
eight-entry array remain unchanged; no oversized-count safety is invented.

Qualified final-owner RTL defines shared pseudo87 at UID159, marks its last
callback-index use dead at UID166 before the indirect call, and freshly
defines it at UID240 for selection. Global allocation maps it to hard5.
Separating only callbackIndex produces hard3 for callback pseudo89 and hard4
for selection pseudo87, yielding 664 rather than 656 bytes while all 43 peers
remain exact. Both final and split-index sources separately pass complete
plain/dump normalized assembly, allocated-section contracts, relocations and
function-inventory parity, independently rerun. These observations establish
real phase grouping, not an unobserved allocator decision or original spelling.
The initially failed broad rename and its exploratory dumps are not qualifying
evidence; all final checks use the corrected immutable source.

Index reuse alone leaves three waitframe/epilogue words different. Two ordinary
short-circuit-equivalent if statements and a separate waitFrames local finish
the match: waitFrames loads only after the packet-count callback returns
nonpositive. Result remains the signed callback return used by its nonzero
test and endtype store. Callback-result/waitFrames reuse was unnecessary and
is absent. Fresh phase, next-ID/count, flags and data observations, message
continuation, decrement and signed modulo-14 history update remain native.
No new constraints, volatile accesses, provider changes or artificial consumers
are introduced; the owner's pre-existing Hojo constraint is unrelated.

Independent complete-owner verification gives 44 match, 0 differ, preserving
all 43 peers and owned data. Retail checksum, 427 tests, 148 guarded owners /
552 drafts, seven IRX owners and report gates passed for source
`f6985edbf85ccfed618748bc1a327ac6094fdde0`. Exact-source CI
[41070334139](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41070334139)
completed successfully, with every actual step passing and none skipped.
