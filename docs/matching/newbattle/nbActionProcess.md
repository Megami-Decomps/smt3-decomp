# Matching notes: newbattle/nbActionProcess

## CheckAction_READY (newbattle/nbActionProcess, 0x3EC)

251 of 251 words match at 1004 bytes in ordinary C. A genuine completion
result starts at zero for states 0/1/2/3/default and incomplete timer state 5.
Failed MIDI readiness exits state 0 before setup. State 2 explicitly selects
sequence 3 when a bed file exists, otherwise 5. State 5 sets completion to
one before signed-halfword command dispatch, including out-of-range commands;
the native case-body order is 0/1/5/6, 3/8/9/10, 2, 4, 7. An unknown command
therefore completes without inventing a sequence transition.

The timer test retains the unsigned16 conversion of signed16 seqtime and the
actual timer type. Timer is reloaded after SetNextActionSeq, preserving its
callback observation. Calls, strings, memory widths and ordering remain
native; p2sdSetPcmBuffer keeps its real void(s32,s32) contract. No constraints,
asm, artificial dependencies, layout or provider changes are needed. This
establishes reconstructed behavior and exact code, not original spelling.

Independent complete-owner verification gives 81 match, 0 differ, including
all 80 peers and owned data. Retail checksum, 427 tooling tests, guarded EE/IRX
and report gates passed. Source `5b974641e1ba11de26e10789bf84e2311d0fe9dd`
has green exact-source CI
[41050483841](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41050483841).

## CheckAction_COMM (newbattle/nbActionProcess, 0x354)

213 of 213 words match at 852 bytes. Native signed16 command loading followed
by an unsigned <9 guard excludes negatives and commands above 8: those paths
do not select a sequence or set result/close, though the final sound may play.
Cases 2/3/7/8 select sequence 7 and set close/result. Case 1's special skill
IDs DE/DF/E2/E3/E4/E5/E6/FC perform their updates, then genuinely fall through
to case 5. After nbSetActionCommand, command and index are freshly loaded
through the preserved original work pointer. After the item helper, debug
printing observes a fresh index; the helper's full signed32 skill result is
not narrowed. Current status and party are observed after the error helper.

Three real sequence-5 arms remain distinct: party flags 6, missing targetarea
bit 1, and targetarea mask 0xC; otherwise sequence 6. The actual skill stride
60 and byte +9 target area access are unchanged. Debug/error handling,
post-sequence status reads and sound-then-close-then-return ordering remain
native, without volatile traffic or invented defaults.

Two minimal live-value bindings remain, with no empty or instruction-emitting
asm. Close in `$20` is initialized zero and consumed by the final guard;
this callee-saved register is saved/restored across its actual calls. The
case-5 `$5` index is u16 widened to s32, feeds the ordinary skill assignment
and item-helper argument, and is dead before the call can clobber it. Later
index reads are new observations, not uses of that bound value. Independently
removing close binding changes nine allocation words at unchanged size;
removing index binding yields 860 rather than 852 bytes. These measured local
aids establish neither original spelling nor an untraced compiler mechanism.

Independent complete-owner verification gives 82 match, 0 differ, preserving
all 81 peers and owned data. Retail checksum, 427 tooling tests, guarded EE/IRX
and report gates passed. Source `b2ad5f7a1ab45adc298969a021f4fe0d75693306`
has green exact-source CI
[41052309663](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41052309663).

## CheckAction_CHECK1 (newbattle/nbActionProcess, 0x28C)

163 of 163 words match at 652 bytes in ordinary C. The actual signed-halfword
command switch copies for commands 0/3; command 1 clears indices 0x31/0x32,
retains previous memory for 0xA4..0xA6 and 0xF7, and otherwise copies. All
other commands, including negatives, clear. Unsigned16 subtraction casts
preserve native modular range tests over the full index domain. Both real
clear-call sites remain, with no widened fields or invented bounds.

Printf precedes fresh command/index and action status/data observations.
Form/time/death checks retain callback-sensitive reloads, and packettime
remains unsigned for its comparison/decrement. Providers keep truthful types;
the final return/next sequence remain 1/20. Source/destination memory fields
are actual disjoint members, not fabricated views. The now-owned printf string
is its actual literal; final whole-owner checking resolves ownership without
the discarded private promotion wrapper's inactive-fallback sharing artifact.

The switch-only control has the native size but ten differing load/store
words. Qualified paired regmove/sched1 observations guide two natural
increasing-address source-order changes. In null-work initialization, the
last actual data-pointer use changes from the 0x5A store to the 0x5C store;
sched1 moves that last-use store first, yielding native 0x5C/0x56/0x58/0x5A
order. In command-memory copy, the last actual pointer use becomes previous
form at 0x78 rather than previous area at 0x76. Final sched1 loads current
area/form at 0x6E/0x70, then stores 0x72/0x78/0x74/0x76, exactly native.
These are observed pass outputs and genuine last uses, not a guessed internal
scheduler priority or original-source claim. No extra access, consumer,
constraint, binding, volatility, ABI change or artificial dependency is added.
The owner's pre-existing unrelated bindings remain unchanged.

Control and final each independently pass full-owner plain/dump normalized
assembly, allocated-section contracts, relocations and function-inventory
parity. Bounded classification models pass 851,968 implementation cases and
1,310,720 independent-review cases spanning all u16 indices or all s16 commands
against representative values of the other variable. They test clear/copy/
retain decisions, not the exhaustive Cartesian product, callback execution
or arbitrary-memory behavior; exact accesses and call ordering remain required.

Independent complete-owner verification gives 83 match, 0 differ, preserving
all 82 peers and owned data. Retail checksum, 427 tests across 26 tooling files,
148 guarded owners / 551 drafts, seven IRX owners and report gates passed for
source `43b917262373f48e4cc269a792e472cd19ad0595`. Exact-source CI
[41072560237](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41072560237)
completed successfully, with every actual step passing and none skipped.

## CheckCounter (newbattle/nbActionProcess, 0x238)

142 of 142 words match at 568 bytes. A signed switch on the masked status
preserves all 4096 values and native balanced rejection of 1,2,4,0x10,0x80,
0x100,0x400; zero and other combinations proceed. Counter initializes only
after rejection, before every use. An early zero-counter return precedes the
real action/printf success tail. The existing sentinel loop already supplies
native induction, not a newly invented loop. Arrays remain skills[4] with
-1 sentinel and counters[3]; the sentinel stops before a fourth counter access.
Skill callbacks precede denominator reads; party indices retain post-callback
observations. Signed division, including the native zero-denominator trap,
is not clamped or repaired. Actual constant/string ownership checks pass.

The typed-address control is 568 bytes with two differing words: it combines
the base to datSkillTable+0x3F08 and loads offset zero. Retail instead starts
from the actual special subarray at +0x4808, adds skill*8 and loads at -0x900.
Final source expresses this as unsigned32 integer arithmetic from the real
subarray base, adding the skill offset then subtracting 0x120 records. Only
the final valid address becomes a datSpecialSkill_t pointer and reads its
signed16 n member. There is no intermediate invalid C pointer, fabricated
offset pointer, false extent or altered field type.

This is explicitly pinned EE32, not portable ISO-C provenance reasoning.
Compiler assertions establish four-byte pointer/u32 representations, eight-byte
records and n at offset zero. The real array has 95 records; actual skills
0x133/0x132/0x131 yield indices 19/18/17. At the retail table location the
integer operations do not wrap, and every final pointer is aligned inside
that real array. Intermediate integers need not themselves designate objects.
The pointer/integer conversions remain target-specific implementation behavior,
not a historical-source claim or general portability guarantee.

Qualified paired expansion/CSE/combine observations explain the difference:
typed control subtracts 2304 before the variable offset and CSE records the
combined symbol+0x3F08; final adds the real offset first, and combine folds
its later subtraction into the actual signed-halfword memory operand. Each
complete-owner plain/dump pair passes normalized assembly, allocated-section
contracts, relocations and function-inventory parity, independently rerun.
No constraint, binding, extra consumer, volatile, alias or provider change
is introduced. Bounded models cover all status halfwords and the three actual
addresses; they are not whole-target execution or arbitrary-input proofs.

Independent complete-owner verification gives 84 match, 0 differ, preserving
all 83 peers and owned data. Retail checksum, 427 tests across 26 tooling files,
148 guarded owners / 550 drafts, seven IRX owners and report gates passed for
source `0e182dd0654b7576aead7a4f983de7b6b5c25f38`. Exact-source CI
[41074871555](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41074871555)
completed successfully, with every actual step passing and none skipped.

## CheckBinjoNego (newbattle/nbActionProcess, 0x258)

150 of 150 words match at 600 bytes. Independent ordered form/party flag tests
preserve every bit pattern; the signed party-index test rejects >=4, not
negative indices. Moon precedes the fresh work observation for Talk, whose
actual result is returned only on its zero edge. Iteration remains 4..13.
The per-iteration form pointer is a real snapshot across callbacks; success
reloads current action data/party while taking partyindex from that saved
form. Push and printf each observe the current form index. The real limit
100 feeds both boosted rate and RNG bound, without an invented consumer.

Initial, form and party statuses remain separate locals. Shared-status spelling
also matched but was unnecessary, so no phase-reuse dependency is claimed.
Diagnostic address and mask constraints were removed independently without
losing exactness; no new constraint, binding, volatile or alias remains.

Address arithmetic starts with the signed16 index scaled by the real unsigned
sizeof(nbParty_t), adds the actual array base in unsigned32 arithmetic, and
forms only the final typed pointer. Pinned probes establish four-byte pointer,
u32 and size_t, 128-byte party records and the actual halfword flag at zero.
This preserves EE32 LH/SLL7/ADDU behavior without an intermediate invalid C
pointer, fabricated offset pointer or false extent. Valid final accesses retain
the original precondition: arbitrary signed indices are not promised safe,
and this is not portable ISO-C provenance reasoning.

The typed-address control differs only in using ordinary array indexing and
leaves seven target words different. Qualified expansion observes final
offset-first versus control base-first addition and actual index/base load
order. Local allocation assigns final pointer/status to hard2/hard3 versus
control hard3/hard2. Both full-owner plain/dump pairs independently pass
normalized assembly, allocated-section contracts, relocations and function
inventory. These are observed lowering/allocation facts, not an unseen
comparator rule or original-source claim. Earlier data diagnostics were
secondary to incomplete target relocation recognition; exact target passes
unchanged owned-data checks without new imports or manufactured literals.

Bounded models pass 196,608 single-field flag/stage predicate cases and 262,144
low32 address cases over all index patterns and four representative bases.
They do not prove all state combinations, callbacks, pointer validity or
whole-program execution. Independent complete-owner checking gives 85 match,
0 differ, preserving all 84 peers and owned data. Combined retail checksum,
427 tests across 26 tooling files, 147 guarded owners / 547 drafts, seven IRX
owners and report gates passed for source
`b362bd871f355c758be545f97a43237e7f324ead`. Exact-source CI
[41078870717](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41078870717)
completed successfully, with every actual step passing and none skipped.

## SetAction_CHECK1 (newbattle/nbActionProcess, 0x34C)

211 of 211 words match at 844 bytes. The living-actor timestamp retains the
GetAllEndTime snapshot, then performs GetAllUnlockTime but discards its return.
End plus actual motion delay supplies the three packet consumers; it is not
unlock time and is not recomputed after SetTimeList. Earlier unlock still
supplies the dead-formation loop timestamp. Callback-sensitive form/motendtype
reloads and final recheck remain native. Independent loop guards consume the
real status snapshot without combining mask 0x41. The loop stays 0..15; form
addresses use the actual EE32 base/index/240-byte stride and only form the final
valid typed pointer, with no invented bounds or portable-provenance claim.

Three localized empty identity regions act on initialized actual values:
frameScratch `+r` retains the real previous delay on the conditional-move path;
block-local motionMode=2 `+r` feeds the actual first comparison without keeping
that constant through the join; actual signed16 motendtype `+m` at the join
inhibits reuse of its earlier load, preserving the native second halfword
read. The last operand is an identity, **not an invented field write**. Empty
templates emit no instruction/store and assert no global memory barrier.
There is no fixed register binding, false volatile or altered provider ABI.
These are explicitly compiler reconstruction aids, not historical source facts.

Final scratch phases are genuine and disjoint: initialized 0/3 supplies motion
delay, then a fresh packettime-10 definition supplies the later stock-panel
fade clamp. Qualified local-allocation RTL marks pseudo88 dead at its end+delay
consumer UID631, redefines it at UID825, and marks it dead after clamp use
UID848. No value or artificial consumer extends across that phase boundary.
Unsigned packettime arithmetic and explicit unsigned end+delay preserve native
modulo32 operations before target s32 conversion and signed clamp; no timestamp
range assumption is invented. The skipped native delay-slot assignment on a
no-consumer path has no observable effect.

Qualified ordinary GCSE propagates scratch to zero and CSE removes the second
motendtype load. Final retains the real scratch arm, second halfword load and
fresh later mode constant. Independent final/ordinary complete-owner plain/dump
comparisons preserve normalized assembly, allocated-section headers/contents,
relocations/symbol contracts and function inventory. These are actual pass
observations, not an unobserved comparator mechanism. Reduction removed an
unnecessary fixed mode binding; combining the separate initialization operands
regressed allocation. No broader barrier or extra field store replaces them.

Independent complete-owner verification gives 86 match, 0 differ, preserving
all 85 peers and owned data. Retail checksum, 427 tests, 147 guarded owners /
543 drafts, seven IRX owners and report gates passed for source
`df2cf13c91edaa9dfdc6dcae1a91b1a96290d359`. Exact-source CI
[41088837974](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41088837974)
attempt 1 completed successfully, with every actual step passing and none skipped.
