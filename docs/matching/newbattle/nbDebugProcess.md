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
provider remained **NON_MATCHING** at that landing, with its then-current
552-versus-536-byte mismatch. Only nbDebugProcess1 received matching credit.

Independent complete-owner verification gives 15 match, 0 differ, preserving
all 14 compiled peers and both owned tables. Integrated retail checksum and
full regression, guarded EE/IRX and report gates passed for source commit
`a831913c94e1ade4034aa302f27a59cb8df6378f`. Exact-source hosted CI
[41038826142](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41038826142)
completed successfully, including all build/deploy steps and actual retail verification.


## SelectDevil (newbattle/nbDebugProcess, 0x218)

79 of 134 words match at the native 536-byte size; 55 still differ. This is
an improved guarded draft, not an exact match. The context-parameter ABI
above is unchanged, and the assembly fallback remains active.

The former volatile casts accessed ordinary RAM: the cursor and page-base
small-data objects and the 64-byte pad-state buffer maintained by the kernel.
Plain accesses still retain the cursor reload after the drawing callbacks
and the page-base reload between the two text calls. No asynchronous writer
or MMIO behavior is invented to force additional loads.

Two initialized coordinates, 230 and 2720, feed the actual text arguments
`xText << 4` and `x + 0x7000`. One empty read/write operand pair inside their
existing loop lifetime keeps those values unscaled until their consumers.
It emits no instruction, has no fixed-register binding or memory clobber,
and is not a RAM-ordering barrier. Both values are dead after the drawing
loop. This is a localized SMT3 compiler constraint, not an original-source
claim.

Paired complete-owner checks preserve all 15 compiled peers and owned data.
The inherited cast-bearing draft emits 552 bytes and differs in 96 of the
134 native-prefix words. Removing just the casts emits 512 bytes; ordinary
coordinate locals emit 524 bytes. The final operand pair restores the
96-byte frame, ten saved registers, native-sized body and per-call coordinate
arithmetic, but leaves different saved-register assignments and scheduling.
The no-call paging tail also forwards the current page base rather than
performing retail's second load, and its conditional-store control flow
still differs. Explicit branch arms alone did not resolve that tail. These
are source/output observations; no compiler-pass trace is claimed.
