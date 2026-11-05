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
