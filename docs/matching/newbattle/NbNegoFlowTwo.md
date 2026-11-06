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

## Flow6_20 (newbattle/NbNegoFlowTwo, 0x1A0)

104 of 104 words match at 416 bytes. An ordinary forward branch replaces
the currency path's separate zero return with a jump to the existing final
zero return. Retail likewise shares that completion point between currency
and item paths. The original singleton had three differing words at the
same size; this single control-flow change gives complete-owner 71 match,
0 differ, with owned data exact.

Both old exits already returned zero. The currency path still skips all item
messages, and the item path falls through to the shared return. No intervening
cleanup, initialization, memory operation or callback is skipped or added.
Repeated request-type observations across callbacks, message arguments,
request arithmetic and conversions remain unchanged. No new scalar,
constraint, type, provider change or artificial data dependency is introduced.

Independent review and complete-owner checking confirm 71 match, 0 differ.
Paired plain and diagnostic builds preserve full assembly and 71-function /
8-contract output parity. Expansion exposes the shared zero-return label;
final delay-slot output restores the currency-path register before that
common zero assignment, matching native. This is compiler-shape evidence
for the forward completion branch, not historical-source recovery.

Combined local gates pass the retail checksum, 26 tooling test files / 427
cases, all 147 guarded owners / 533 drafts with stable source/compiler inputs,
seven IRX owners, and reconciled production reports.

## Flow6_22 declaration visibility

The existing `s32 datCheckMaka(s32)` declaration now applies in every build,
not only under `NON_MATCHING`. Its provider in `newdata/datCalc` loads the
signed 32-bit currency field, performs a signed comparison with its argument,
and returns 0 or 1. Both source calls in `Flow6_22` pass the existing signed
`requestdata` field and consume that Boolean result. Retail shares those calls
at 0x2701B8; the post-call request-data reload and currency deduction remain
unchanged. No body, fallback guard, conversion, header or flag changes.

Production and SKIP_ASM previously failed
`-Werror-implicit-function-declaration` for this callee;
NON_MATCHING/SKIP_ASM already passed. All three complete-owner modes now pass.
The paired baseline/corrected objects retain all four allocated sections,
normalized assembly, ordered non-file symbols and ordered relocations:
production 451 symbols / 358 relocations, SKIP_ASM 403 / 325, and
NON_MATCHING/SKIP_ASM 453 / 358. Plain and strict-diagnostic candidate output
also agrees in every mode. Complete retail owner checking remains 72 match,
0 differ, with owned data unchanged; all four guarded drafts still compile.
This adds no matching credit or compiler constraint.

## Flow6_65 (newbattle/NbNegoFlowTwo, 0x190)

100 of 100 words match at 400 bytes. Separate race-specific affinity switches
retain the native branch structure, and one localized binding keeps the
initialized signed rate in its native callee-saved register. The ordinary
switch-only control already allocates the rate to the same physical register,
but has ten differing words at the same size; it omits the native XOR-zero
comparison normalization. The binding exposes the hard register earlier,
before allocation. Binding the old if ladder alone also fails. These paired controls establish the narrow source/output
dependency without claiming a deeper allocator mechanism or historical
source spelling.

Qualified whole-owner snapshots locate the distinction. In the ordinary
control, an early XOR in 02.jump disappears by 03.cse; conditional conversion
later consumes the comparison directly. The candidate remains branch-based
through 14.combine, then 15.ce introduces XOR UID 453 feeding the real rate
update. That XOR survives in retail's position. These are observed pass
boundaries, not a claim about an unobserved backend decision.

The default rate is 50. Affinity 1 selects 30 or 40 by race, affinity 2 selects
70, and other affinities retain the default. The independent second race
query and player-then-enemy parameter calls remain. Strict signed parameter
comparisons adjust the live rate by five; provider ranges keep the arithmetic
defined. The unsigned RNG(10) test and signed RNG(100) comparison remain
distinct. Negotiation counters, candidate loads after callbacks, message
arguments and all zero returns preserve native behavior.

The rate is initialized before its first use, and its real adjustment
and comparison consumers remain. Every intervening provider preserves the
callee-saved register; the function saves and restores it in the native
32-byte frame. No caller-clobbered value crosses a call, and no artificial
consumer, type, bound, memory effect or compiler flag is added. Provider and
dispatch signatures, field offsets and the shared negotiation layout were
independently reviewed.

Both complete-owner checkers give 73 match, 0 differ, preserving all 72 C
peers. Independent production objects have identical complete allocated
text (14,200 bytes), rodata (228 bytes), empty data/BSS, and all 358 ordered
relocations. Eighteen complete plain/dump assembly and literal ELF comparisons
pass with 672 stable compiler/runtime inputs. Normalization changes only one
same-length scratch-source filename. Strict declaration and uninitialized
checks also pass in production, SKIP_ASM and retained-C modes without changing
output.

Full retail checksum, generated-source, tooling, retained EE/IRX, compiler
wrapper and reconciled report checks pass. The linked production proof
identifies a unique 400-byte C-owned span matching retail. Game coverage
gains one function and 400 bytes.
