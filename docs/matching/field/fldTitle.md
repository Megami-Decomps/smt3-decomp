# Matching notes: field/fldTitle

## fldTitle_Process (field/fldTitle, 0xBD4)

757 of 757 words match at 3028 bytes in ordinary C. Independent complete-owner
verification gives 8 match, 0 differ, including owned data and the existing
2088-byte Mini process. No compiler constraint or additional asm is used.

A real `s32 nextPhase` receives one immediately after the final entry render
setup call. The existing case-zero transition stores that value into the
phase global when its countdown finishes. The switch still reads its phase
after the setup calls, and all animation/callback-visible state accesses are
unchanged. This explicit transition value produces the native constant-before-
phase-load dispatch order. Its sole source consumer is the actual state store;
it is not a dummy value, an invented dependency, or a recovered spelling claim.

The prior literal-store draft is 755/757, differing only at +0x4C and +0x50.
The ordinary transition-local form is exact before any packing corrections.
Independent deletion of that local while retaining all packing corrections
returns exactly to 755/757, with all seven peers and data unchanged. These
are source/output comparisons, not a new internal scheduler trace.

The body keeps native entry shrink, overlay fades, interleaved moving strips,
white/colored title layers, exit displacement order, callback behavior
and state transitions. Thirteen alpha-packing shifts now convert to `u32`
before shifting by 24: eight in this target and five in the already-matched
Mini process. Signed alpha 128 occurs in normal animation and previously
made that shift undefined. Signed pre-pack calculations and clamps remain;
signed packet locals/parameters retain the verified EE bit-pattern conversion,
which is implementation-defined for high-bit results rather than a portable
signed-wrap guarantee. The changes are byte-inert for both functions.

Normal initialized animation progression is bounded, but this does not assert
new bounds against arbitrary global mutation or invalid raw state. Countdown
decrement and earlier signed arithmetic are unchanged. Native reloads after
callbacks and the Mini lower-strip condition's existing title-state read
remain intact. This fixes packing, not every possible arithmetic input.

### Earlier scheduling diagnosis

On the earlier guarded literal-store source, a qualified plain/hooked/unhooked
trace selected phase load UID1748 (priority 3, cost 2) before constant-one
UID1750 (priority 2, cost 1) in both scheduling passes. That was a critical-path
choice, not an original-order tie. A phase snapshot and a phase-increment
transition were inert. This earlier graph does not prove that every truthful
source form must retain its priorities, nor establish the internal mechanism
of the now-exact transition-local form.

Integrated retail SHA1/byte comparison and no-op rebuild, 436 tooling tests,
inline-asm gate, all 142 guarded owners / 496 drafts with stable inputs,
7 IRX checks and reconciled reports pass. Source commit
`cac817b4d1b0eafccce5c8670b18ffc8fb8d1949`; exact-source
[CI 41416864184](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41416864184)
attempt 1 passed build and Pages deployment.
