# Matching notes: field/fldSnow

## fldSnowInit (field/fldSnow, 0x1B4)

109 of 109 instruction words match at 436 bytes. Independent complete-owner
verification gives 9 match, 0 differ, including owned data. The former draft
emitted 444 bytes with 28 matching words.
Native instruction bytes exclude the four-byte alignment after the return.

The two area cases have independent floor locals. The floor-zero path and
completed area-0x18 path branch forward to common seed allocation; only the
remaining area-0x1F floors pass through the final height-step assignment.
This ordinary forward-branch shape reproduces every native branch and
allocation instruction. It avoids duplicating the allocation/lock sequence.

Coordinate chains preserve equal X/Z initialization. Resetting the mode before
those coordinates, and defining the area-0x18 speed before its height step,
recover the native constant registers and first-area store order. The life and
width defaults precede their floor-specific overrides. Earlier qualified compiler
probes confirmed normal/dumped/hooked assembly and RTL parity for the nearby
control and then-retained near-match, not the new binding variant. Independent
native/provider review confirms
every early return, value and shared allocation path.

The prior retained draft exchanged the mode and speed stores at +0xE0 and
+0xF0. A scoped real `f32 speed = -3.0f` bound to `$f0`, followed by mode
publication and the existing speed store, gives the native order. The actual
negative speed bits are `0xC0400000`; the sole value consumer is the real
global speed store. No call or FPU-register clobber occurs during its lifetime.
No empty asm, extra memory access or runtime barrier is added.

Deleting the binding gives 104/109 words; restoring the old speed-before-mode
source order with the binding gives 107/109. All eight peers and owned data
remain exact. This is a local reconstruction aid with paired output evidence,
not a hardware register requirement or proof of original source spelling.
All branches, floor defaults and early returns, allocation/lock/seed calls,
other state stores and literal ownership remain unchanged. The unresolved
SnowTask reconstruction is separate and is not promoted by this match.

Integrated retail SHA1/byte comparison and no-op rebuild, 436 tooling tests,
inline-asm gate, all 142 guarded owners / 492 drafts with stable inputs,
7 IRX checks and reconciled reports pass. Source commit
`996fef5f597220ca8af9a8e0cd99a87f0118a3a8`; exact-source
[CI 41433004720](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41433004720)
attempt 1 passed build and Pages deployment.

### Guarded snow update and rendering reconstruction

`fldSnowTask` remains guarded with its assembly fallback. The reconstruction
now emits the native 1280-byte extent with 240 of 320 instruction words matching,
versus 1316 bytes and 117 matching words before this revision. This is not an
exact match and earns no matching credit.

The two ordinary seed pointers no longer impose unsupported volatile accesses.
Each draw-loop iteration still reloads the global seed-buffer base after any
render callback; the per-iteration item pointer remains valid only within that
iteration. The actual 12.0f corner radius supplies both signed corner coordinates,
and that scalar is reassigned to the snow speed in the later update phase.
A distinct drift scalar supplies the actual 0.125f multiplier. The two temporary
stack objects follow native direction-vector then sprite order. Coordinate
addition retains its original floating-point grouping, and unsigned color
composition preserves every bit, including signed alpha converted to unsigned.
No new compiler constraints, type views, or flags are added.

Full-owner verification retains all nine exact peers. Normal, SKIP_ASM, and
NON_MATCHING/SKIP_ASM plain/dumped compiler outputs agree byte-for-byte. Host
comparison covers 2048 complete-call scenarios with 700-element seed pools,
mode changes, null buffers, signed velocities/lifetimes/alpha, and callbacks
mutating the spawn inputs, global seed-buffer base, mode and rendering threshold.
Optimized and address/undefined-sanitized runs agree with the previous guarded
body. Remaining differences include the random-coordinate float allocation,
camera setup schedule, update speed/drift registers and packed-color schedule.
