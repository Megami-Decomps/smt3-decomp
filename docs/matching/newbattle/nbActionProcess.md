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
