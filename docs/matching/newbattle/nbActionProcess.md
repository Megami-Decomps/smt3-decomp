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
