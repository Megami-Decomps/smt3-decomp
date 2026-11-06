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
