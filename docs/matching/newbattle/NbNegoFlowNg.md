# Matching notes: newbattle/NbNegoFlowNg

## Flow10_10 (newbattle/NbNegoFlowNg, 0x90)

36 of 36 words match at 144 bytes in ordinary C. Each affinity arm makes
its own message call and returns zero; the compiler emits the native shared
call and epilogue. The old shared-argument draft emitted 140 bytes. Separate
calls with a common final return emitted 148 bytes. No constraint, false
type, dummy consumer, added memory qualifier, changed flag or declaration
is needed. The result establishes behavior and emitted shape, not historical
source spelling.

The candidate-registration call remains first. The following writes retain
workid[6] = 0x60009 and both zeroed negotiation counters. The signed affinity
field is tested against one. That arm uses the signed personality index into
the affinity message table and the existing affinity message number; the
other uses the flow message table and message number 0xD9. Both call the
unchanged void message provider with selection zero, exactly once, and then
return signed zero. Native 32-bit accesses, callback-sensitive observations,
write order and branch-gated table reads remain exact. No new index bound or
invalid-input guarantee is introduced.

Independent whole-owner review confirms 31 match, 0 differ and unchanged
allocated code/data and resolved relocation contracts. Complete assembly and
literal entire-ELF plain/dump pairs preserve every byte except the exact
observed equal-length scratch source pathname. Full retail checksum,
generated-source, tooling regression, guarded EE/IRX, inline-asm and
production-report checks pass before publication.


## Flow10_21 (newbattle/NbNegoFlowNg, 0x98)

38 of 38 words match at the native 152-byte size. The two candidate-registration
calls remain first. IDs 0x5C, 0x3D and 0x61 choose the second candidate without
an RNG call when event bit eight is clear. Every other path chooses the first
candidate for unsigned RNG values at least five, and the second otherwise.
The chosen u32 ID is stored once, and the function returns signed zero.

The final RNG branch places the slot-zero selection in its first arm. The real
zero return has a localized binding to the ABI return register, initialized
after the final callback on every path and consumed by the return. No call
clobbers a live bound value, and no empty operand is needed. Removing only the
binding emits 156 bytes with ten of the 38 covered words different. Keeping
the binding but reversing the final arms gives 152 bytes with four words
different: two branch displacements/polarities and the two candidate loads.
These paired output measurements establish a target-specific compiler aid,
not an internal pass mechanism or historical source spelling.

The native unsigned halfword ID read and word candidate/destination accesses
retain their existing types and offsets. Candidate reads remain after the last
callback, including callback-visible candidate updates. Provider signatures,
call arguments, short-circuiting and callback order are unchanged. No new input
bound, object layout, qualifier or arithmetic operation is introduced.

Complete-owner checks give 32 match, 0 differ. Guarded, ordinary-ablation and
candidate sources each pass full ordinary/-da assembly and entire-object
parity in production, SKIP_ASM and retained-C modes, after normalizing only the
canonical temporary source pathname. All 32 production, 31 SKIP_ASM and 32
retained-C peers and resolved relocations, plus allocated nontext sections,
remain unchanged. The source-level host fixture compares complete records,
return values and callback logs across all 65,536 IDs, four event results,
eight unsigned RNG boundary values and two callback-mutation modes. It does
not emulate EE register binding or establish bounds for invalid caller objects.

Retained-C strict implicit-declaration builds pass. Production and SKIP_ASM
retain the same two existing Flow10_33 implicit declarations, nbCameraSetTaizi
and datGetAisyoFlag, with identical guarded/ordinary/candidate diagnostics.

## Flow10_1 (newbattle/NbNegoFlowNg, 0x100)

64 of 64 words match at 256 bytes. The registration calls remain first and in
order. The work ID is reloaded after them as an unsigned halfword. The special
ID/event path returns its actual tested-zero event result without a constraint.
Signed request/counter tests retain full-word zero/nonzero behavior. Candidate
loads follow the corresponding RNG callback, and the initial selected-ID store
remains before the race provider observes it. The final alternate candidate is
reloaded after that provider. No provider ABI, type, bound or layout is changed.

Two localized compiler aids remain. In the middle path an initialized zero is
consumed by an empty operand, the selected ID is stored, and the same zero is
returned. There is no intervening call or fabricated memory barrier. In the
final path the selected u32 ID is bound to native v0 only from its post-race load
through its real store; the previous race result is dead, and the bound value
dies before zero is returned. Neither constrained value crosses a call.

Independent ablations in this source shape give 248 bytes with 37/62 overlap
words matching and 8 native bytes uncovered without the return operand; 252
bytes with 52/63 matching and 4 uncovered without the selected-ID binding; and
252 bytes with 48/63 matching and 4 uncovered without either. These observations
justify the two local aids in the reduced shape without claiming no other
ordinary spelling could match or asserting a compiler-internal mechanism.

Integration asserts every source byte outside this target span is unchanged,
including the existing Flow10_21 binding. Guarded, enabled original, ordinary,
final and three ablation owners receive 21 fresh complete assembly/entire-ELF
plain-versus-stock -da parity checks. Only one equal-length temporary source
filename is normalized. The final owner is 33 match, 0 differ; each nonmatching
control leaves only this target different. All 32 peers and allocated nontext
sections remain unchanged in every mode. Independently resolved final code for
all 33 functions matches raw retail, as do the 84 owned rodata bytes; its 12-byte
native tail is zero padding.

Strict production/SKIP builds retain identical inherited Flow10_33 errors for
nbCameraSetTaizi and datGetAisyoFlag. Retained strict builds pass. These are
baseline-equivalent failures, not clean production strict passes. The target's
callees are declared and their actual providers were independently reviewed.

A fresh bounded interpreter fixture compares the exact extracted original,
final and three ablation bodies with the 64 raw retail instruction words.
Optimized and ASan/UBSan runs each pass 562,144 scenarios and 2,810,720 complete
state/callback comparisons. Coverage includes every u16 ID, signed state
boundaries, candidate slots, valid registration capacities, real bounded RNG
modulo behavior and callback mutation. Caller-saved registers are poisoned,
and the native stack/saved-register contract is checked. Target constraints
are erased only for host execution; EE output and lifetime evidence establish
their native behavior. This is not a full game emulator, provider end-to-end
test, or a real-table bound proof. Out-of-contract negative capacities are
excluded, and LeakSanitizer alone is disabled for executor compatibility.

The full linked retail image is byte-identical. Generated-source controls,
505 tooling tests without optional skips, 818 layout assertions, frozen EE/IRX
draft sweeps, inline-assembly policy and primary/audit report-scope checks pass.
The complete music 27/0, recovery 391/0 and birth-mask 196/0 owners are preserved.
