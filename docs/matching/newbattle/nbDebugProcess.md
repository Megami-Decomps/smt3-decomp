# Matching notes: newbattle/nbDebugProcess

## nbDebugProcess1 (newbattle/nbDebugProcess, 0x2A8)

170 of 170 words match at 680 bytes. Native state dispatch retains distinct
menu/HP/MP/status outcomes and the ordered inactive/active motion callbacks.
A real shared menu continuation sets nextseq and advance, without an artificial
consumer. Invalid menu selections do not advance; all non-early paths call
SetNextSeq only when advance is set, then return NULL. State 3 checks the
required side from status bit 8 before any order callback. After nbSetOrder
and every nbSetNextOrder, it reloads d->data, active party and both form-index
observations, retaining callback mutation behavior until the native match.

Two empty operands act on real initialized values: a pointer-valued -1
return and the side-comparison value one. Only the latter has a fixed `$2`
binding, consumed before the next call; no caller-clobbered live value spans
a call. The former preserves a distinct native early-return epilogue;
removing it leaves two differing words at +0x220/+0x224. Removing the side
binding while retaining generic operands changes allocation/size. Reduction
removed two other bindings and an unnecessary side operand without losing
exactness. Neither constraint emits an instruction or asserts a memory
barrier. They are SMT3 reconstruction aids, not original-source evidence.

SelectDevil's context formal is a recovered ABI fact, not a fake dependency:
the recorded prototype takes nbDebugProcessData_t *d, and the native caller
at 0x2851E0 explicitly supplies d in the call delay slot. The provider discards
incoming a0 before use at 0x284B3C. Reference and retail-word audits find one
direct call and no address-taken site. The exact prototype, guarded definition
and call agree; no opaque alias, wrapper or extra-argument cast is used.
Paired canonical NON_MATCHING builds preserve the provider's 552-byte draft
text and all 29 relocations; normal provider assembly is unchanged. The
provider remains **NON_MATCHING**, with its existing 552-versus-536-byte
mismatch. Only nbDebugProcess1 receives matching credit.

Independent complete-owner verification gives 15 match, 0 differ, preserving
all 14 compiled peers and both owned tables. Integrated retail checksum and
full regression, guarded EE/IRX and report gates passed for source commit
`a831913c94e1ade4034aa302f27a59cb8df6378f`. Exact-source hosted CI
[41038826142](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41038826142)
completed successfully, including all build/deploy steps and actual retail verification.
