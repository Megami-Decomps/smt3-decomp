# Matching notes: newbattle/nbSound

## nbLoadMotionByIndex (newbattle/nbSound, 0x78)

All 30 words match at 120 bytes. The complete owner checks at 25 match,
0 differ, including its owned data and unchanged direct callers.

The true event arm computes the actual unsigned pre-shift result, retains it
through one `FAKEMATCH_ASM_LIVE` operand, then shifts and returns it. Both arms
spell the native addition order. This localized empty operand emits no
instructions and has an immediate real consumer. It introduces no call,
memory effect, extra arithmetic, artificial lifetime or fixed register.
It is a target-compiler reconstruction aid, not recovered source spelling.

A paired ablation removes only that operand. Both complete arithmetic arms
remain in the ablation's `25.sched2` dump, but `27.jump2` removes the event
branch and the duplicate arithmetic; the function becomes 100 bytes. With the
operand, `27.jump2` retains the event branch and both pre-shift calculations.
It still shares the final shift. `29.dbr` places a copy of that shift in the
true exit's branch delay slot, yielding the native two tails. These are
qualified stock-pass observations, not a claim about an unobserved internal
comparator. The earlier commuted-arm ordinary draft emitted 112 bytes; its
remaining eight-byte gap was not the same ablation as this ordered-arm form.

The mapped id is loaded as `u16`. `nbGetMotionTable` returns a sign-extended
byte; negative motion returns zero before the event query. Otherwise the
pre-shift sum ranges from 4096 through 1052783, safely within `s32`.
The final unsigned shift preserves native truncation without signed-shift
undefined behavior; conversion back to `s32` follows the configured target's
representation. Independent exhaustive checks cover all 8,388,608
nonnegative id/motion pairs in both event arms, including zero and negative
result representations. Calls, argument contracts, accesses and early-return
ordering are unchanged.

Fresh candidate and ablation builds qualify all three complete-owner modes:
production, `SKIP_ASM`, and retained drafts. Direct plain/dump assembly and
whole ELF objects are identical; wrapper outputs agree after replacing only
the asserted equal-length source filename metadata. The explicit frozen
closure covers 21,362 named paths and the compiler/runtime dependencies.
This is a conservative closure, not a syscall-complete read-set claim.
Independent semantic and output-evidence review, complete affected-owner
checks, generated-source checks, the full retail build and tooling gates are
required before publication.

Earlier mapped-id, motion and post-shift-result operands did not match. In
particular, an operand after the final shift kept the shift out of the needed
exit delay slot. The new operand is before the shift, on its actual input.
Arithmetic-spelling sweeps and an exact-signature opaque provider declaration
were closed negative controls and are not needed by the accepted form.
