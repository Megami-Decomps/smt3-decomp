# ModeSelect matching notes

## mselDevilChoice (ModeSelect, 0x33C)

207 of 207 words match at 828 bytes using ordinary C. The real target limit
starts at five before the first selection loop, then clamps against a fresh
ordinary saved-count read after the RNG/helper calls. The inherited volatile
cast is unnecessary and removed. The RNG declaration agrees with its actual
provider: `u32 dds3GetRandIntA(void *state, u32 limit)`, with NULL selecting the
shared PRNG state. No register binding, asm, false ABI or compiler constraint
is used.

The top-ten index/score arrays really contain ten entries and chosen contains
five. Initial ranking retains unsigned experience comparisons and native equal
score insertion. The first phase tests remaining>0 before i<3 and decrements
its positive draw bound after each choice. The membership search preserves
`j >= chosenCount && j != 0` and `i == chosen[j]`, including the native unusual
zero-chosen skip rather than silently correcting it.

The second phase uses a decrementing poolCount for random draws and an immutable
remaining snapshot of allocated pool capacity for the shift helper. Trailing
stale entries are not drawn because the selection bound shrinks. These are real
count/capacity values with algorithmic consumers, not artificial lifetimes.
The true five-element limit, ordinary fresh load, and native comparison spelling
are sufficient. Earlier volatile-bearing and false-RNG-declaration candidates
are superseded, not the published source.

For valid stable saved counts n in 0..184, the first phase selects min(n,3).
At n=0 no draw/allocation executes; n=1..3 uses only the first phase, without
an empty-pool draw. For n>=4 the pool is positive, exclusion fills precisely
n-chosenCount entries, and the second phase cannot exhaust its divisor on an
executed iteration. Chosen writes stay within five entries. Native allocation
and freeing still occur even if the target was already met; no allocation-
failure check or fabricated runtime count guard is added. The record capacity
and actual count producers establish valid-input bounds, not safety for corrupt
counts or arbitrary asynchronous count mutation. Actual RNG and shift helpers
have no user callbacks. Global writes and subsequent selected/pool reloads,
final color and byte stores, and call ordering remain unchanged.

Complete-owner plain and stock-`-da` builds have identical normalized assembly,
text and relocations. Stock dumps and final assembly corroborate the real limit
at stack offset 80, decrementing count in s17 and capacity snapshot in s16;
this is output evidence, not proof of original variable names. Independent
final guard-removed review checks 13 match, 0 differ, retaining all 12 peers
and owned data.

Full qualification passes the retail checksum, 149 guarded owners / 579 drafts,
seven IRX owners, and 26 tooling files / 427 tests. Actual published-checkout
verification confirms all 779 tracked files, 240 owner dependencies, source
identity and no-op builds.
Source commit `86df3ba0a84b78250fe6af2a8c7feda4f1565801` passed exact-source hosted CI
[40999068438](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40999068438),
including build and deploy.
