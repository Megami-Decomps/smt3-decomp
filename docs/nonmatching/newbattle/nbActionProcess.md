# Non-matching drafts: newbattle/nbActionProcess

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## SetAction_NEGORESULT (newbattle/nbActionProcess, 0x494)

272 of 293 instruction words match at the exact 1172-byte size. Both jump
tables and the original data layout match. All 76 other C functions remain exact.

The reconstruction preserves the result 1..9 dispatch, recruitment and return
packet order, separate same-ID and all-party return scans, and fresh party reads
after return-packet callbacks. Results 6 and 9 leave the press update disabled;
only result 9 recruits before its scan. Motion reset retains the unit lookup
before taking the fresh formation pointer. Camera setup and the final ten-party
scan preserve their native gates and reads.

The separate nested flag tests are significant: a combined condition folds the
bit-2 and bit-0x20 checks together. Snapshotting the actual u16 flags and keeping
three nested checks reproduces the native test sequence. Explicit no-op cases
0, 5 and 6 in the negotiation-type switch retain its seven-entry dispatch table.

The remaining 21 words are register choices: seven in the first scan's fresh
party lookup and time-list argument setup, seven in the result filters, and seven
in the final party scan. No control-flow, frame, size or data difference remains.
A fresh party-pointer local did not improve these choices; separate result
switches and a separate final-scan cursor also left 21 words differing. The
remaining question is how the native pointer and count lifetimes give the
allocator its argument-register choices without artificial dependencies.
