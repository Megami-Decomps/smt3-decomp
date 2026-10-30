# Matching notes: field/fldMain

## fldReadyScene (field/fldMain, 0x594)

357 of 357 words match at 1428 bytes. Mode-2 title transitions use the actual
short-circuit OR of previous/current area pairs: 40->{41..45}, 41->{40,42},
42->{40,41,43}, 43->{40,42,44}, 44->{40,43,45}, 45->{44,40}. Right-nesting
the equality checks for 40, 41 and 43 preserves the logic without the
adjacent-range sequence emitted for the left-nested form. Both range subtractions cast to unsigned first, preserving native
modulo-32 behavior without signed overflow. Tests cover 41,209 previous/current
pairs drawn from [-100,100] plus INT_MIN/INT_MAX, not every possible input pair.

Previous-area lifetime is separate from later city predicates, which observe
globals freshly. All four restore-mode calls remain distinct, especially the
one after the mode-2 title callback; it cannot become a cached result or else
arm. Resource-reuse state remains captured before evtLoadArea, while scene
mode after dds3SetActiveScene, encounter fields after evtBitCheck, and camera
state after unit/pad/follow callbacks retain native fresh observations.
Ordinary camera branch stores avoid ternary movn lowering. Null-resource
fallback and final damage-counter clear remain unchanged.

The point pointer always names a real fldResourcePoint_t: resource/default,
the genuine safeUnit union member during rotation, or aligned adjustedPoint
for unit setup. Qualified paired compiler evidence establishes the effect of
this live phase reuse independently of the constraints below. Separating only
the safeUnit phase, while retaining its actual operand constraint, leaves the
same 1428-byte size but 83 differing words. Final RTL pseudo84 spans all phases,
survives local allocation and maps to hard17 globally. The control allocates
safePoint pseudo85 locally to hard16 and remaining point84 globally to hard19.
Each source separately passes normalized plain/dump full-owner assembly parity
and section-header/byte identity except debug path contents, including allocated
sections, relocations, symbols and function inventory. These are observed
allocation facts, not historical variable spelling or a guessed allocator rule.
Every pointer definition has a truthful type and real consumers.

Two local constraints address different residuals. Empty `+r(point)` after
the genuine VU rotation store retains the containing-point base for following
scalar reads; removal changes only +0x180 from `lwc1` via point+0x10 to the
same subobject via rot+0. It emits nothing and is not a memory barrier; the
actual VU store has its own memory clobber. The initialized firstArea value 32
is bound to `$6` for the first two city comparisons, with an empty `+r`
keeping it separate from the later independent literal 32. Unbinding it changes
seven words. Both constrained values have actual consumers and are dead before
any clobbering call. Earlier point lifetimes across rotation helpers are
compiler-allocated callee-saved values, not pinned caller-clobbered storage.
No ABI alias, provider change, fabricated layout or scalar opcode replay remains.

Independent complete-owner verification gives 51 match, 0 differ, preserving
all 50 peers and owned data. Existing rodata is unchanged; two native rotation
sites account for eight emitted literal bytes, with linked placement verified
by the full image gate. Full local gates passed for source
`01513b8f9b69342cf6c44e35e864848ce3ae5316`. Exact-source CI
[41060045539](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41060045539)
completed successfully, including every actual step and retail checksum.
