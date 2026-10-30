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

## mselMain (ModeSelect, 0xD7C)

863 of 863 words match at 3452 bytes, including native readonly data and
the 23-entry dispatch table. Explicit continuations preserve physical case
order 15/16/20/17: 15/20 share the message path, while 16/19 share attention
loading. This reconstructs control flow, not a claimed callback bug in the
old pre-draw phase test. Case 6 keeps the real fade in both choice arms;
case 7 preserves unsigned exit-mode handling and snapshot restoration.
Case 12 corrects completion-count access to the native signed halfword
`(s16)fileInfo.endcnt`, not the old word alias. Case 13 reloads gGlobal after
the conditional fade-out before its next counter access. Both texture-free
paths and fclLoadTexture likewise retain native post-call global reloads.
The loaded save pointer genuinely survives record-registration callbacks,
while each registration observes the current global base. Snapshot/header
copies retain actual aggregate layouts; no smaller fabricated views are used.

Three localized bindings have actual initialized consumers and end before
calls: cursorWork in `$6` after cursor update supplies its field test;
textureWork in `$2` supplies the handle condition/free argument, then the
code reloads gGlobal; maximum in `$4` supplies the real 128 subtraction/clamp
endpoint inside the bounded fade interval. Removing these bindings separately
changes 39, two and five words respectively. Ordinary pointer/endpoint and
if/else controls do not match. No empty asm or instruction-emitting asm is
introduced. These are measured target reconstruction aids, not original-source
claims. Separate save/exit/entry/counter locals avoid unnecessary reuse.

The native mselProc name is genuinely readonly owned data. Explicit casts to
legacy char* APIs are supported by their actual read-only consumers:
dds3StartProcess reads and copies the bytes; dds3KillProcessName delegates
to a read/hash/compare search. No writable replacement or invented const ABI
is used. Local declarations now match providers: slGetExitMode returns u32,
dds3AdminiCallSequence takes u32 size, and dds3KillProcessName returns s32
with char*/s32 parameters. The process callback remains
`s32 (dds3ProcessID_t *)`, and fclGetSelMessagePos remains s8.

The timer has two distinct evidence requirements. **Hours and seconds are
dead**, retained only under the narrow native-residue exception. A matching
DDS save-preview family has the same 0x066FEBF8 cap, 30-frame clock and
108000/1800/30 decomposition with real formatted/drawn hours, minutes and
seconds; SMT3 slFileSelDraw corroborates live hours/minutes. Final-owner
compiler dumps show redundant/dead arithmetic removed at CSE, while divisor
guards retain the three native unused definitions through allocation. Final
code omits those guards but leaves the definitions. The no-consumer audit
separately distinguishes those dead constants from the actual live divisors.
This supports a family-shaped reconstruction, not historical copying.

Completion-count/ticks/minute-digit **live scalar reuse is separate**. Every
phase assigns before use. In the exact final owner, PlayTime loads into
count pseudo86 and allocates to s0; a control separating only timerTicks
allocates its load to v0 instead. Both final and control separately pass
plain/dump normalized complete assembly, allocated-section headers/bytes,
relocation/symbol-contract and function-inventory parity. The earlier
exploratory owner with a 3428-byte target is not the acceptance basis. The final
owner containing the 3452-byte target and an independent fresh parity run qualify
the observations.
Positive divisors keep the added dead expressions defined across signed s32
PlayTime without new calls, reads or altered event conditions.

Independent complete-owner verification gives 14 match, 0 differ, preserving
all 13 peers and owned data. Integrated retail checksum, full tests, IRX,
report and fresh full guarded-source checks passed for source commit
`b95a2a093bfc656c5c55eb50af46a0d0987f1412`. Exact-source CI
[41042604248](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41042604248)
completed successfully, including every build/deploy step and retail checksum.
