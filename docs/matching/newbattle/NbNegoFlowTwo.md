# Matching notes: newbattle/NbNegoFlowTwo

## Flow6_24 (newbattle/NbNegoFlowTwo, 0x4FC)

319 of 319 words match at 1276 bytes. Race-first affinity switches recover
native rate adjustments and shared tails. Six candidate registrations,
low-word wrapping counter increment, four unsigned-byte initial rates,
the independent second race query and parameter-call order are preserved.
Strict signed comparisons, negotiation/request overrides and early candidate
returns remain unchanged. Negative adjusted rates are intentionally not
clamped. The four-entry signed cumulative scan keeps its strict comparison,
bound and exhausted-choice remap to 2; final choice-3 redirection preserves
RNG(10), message 0x63 and candidate 4. All paths return zero.

Callback-sensitive count and level observations are native requirements.
Pre-event levels remain live across evtBitCheck; count reloads follow the
actual returning paths. Special-ID event paths reload the enemy pointer,
while the no-call path can retain it. Ordinary typed member reads preserve
the final player-pointer `lw` and enemy/player `lhu` sequence at
0x2706BC/0x2706C0/0x2706C4, without volatile casts. Levels and IDs remain
truthful u16 values; subtraction by 10 or 17 is safe after s32 promotion.
RNG returns are unsigned and bounded by the real nonzero limit; the native
unsigned <4/<6 tests and signed cumulative roll comparison remain distinct.

The reduced source uses four narrowly scoped real-value bindings: player
pointer `$3`, reloaded enemy/player levels `$18`/`$16`, and threshold Boolean
`$2`. The pointer is dead after the level load; the predicate is consumed
before RNG can clobber `$2`. Both initialized levels feed their actual
comparison and one input-only empty asm. It emits no instructions and is not
a general memory barrier; exact output establishes the required load order.
No caller-clobbered binding carries a live value across a call.

Reduction removed the pointer and predicate empty asms, changed the remaining
level constraint to input-only, removed volatile casts and separated parameter
and level locals without losing exactness. Removing the predicate binding
alone leaves three differing words at +0x3C8/+0x3CC/+0x3D0; additionally
removing the pointer binding leaves six. The final has no dummy consumer,
artificial lifetime, disjoint-phase reuse requirement or altered provider.
These are measured target-specific compiler constraints, not an inferred
compiler-pass mechanism or evidence of original source spelling.

Independent complete-owner verification gives 70 match, 0 differ, including
all 69 peers and owned data. Integrated retail checksum, 427 tooling tests,
guarded EE/IRX and report checks passed for source commit
`f245ab2f90610e7f64f90a829978234b47eb2746`. Exact-source CI
[41036132155](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41036132155)
completed successfully, with all steps green.
