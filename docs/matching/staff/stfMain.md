# Matching notes: staff/stfMain

## stfMain2 (staff/stfMain, 0x2D4)

155 of 181 words match at the native 724-byte size in the guarded draft.
The target remains assembly-backed; this is not exact-match credit.

The earlier 145-word draft duplicated the centered row's `sprintf` and
`stfDrawText` calls in the two text arms, used `continue` for the centered
row, and placed the fade block between the state guard and scroll update.
Those choices remain. Moving the common drawing calls before the text test
was 24 words short; nesting all updates under a single trailing return gave
99 matching words. The centered coordinate and local declaration spellings
were also previously tested without improvement.

The previous compound state guard used the same observed scalar for
`state >= 0 && state < 3`. Qualified initial RTL already contains one unsigned
comparison against three. The guard is already folded by the initial RTL
snapshot; the exact frontend or expansion routine is not identified. Repeated
RAM-observation variants were previously worse and are not used here.

Separate lower and upper tests with immediate returns retain both signed
bounds but emit 740 bytes, 16 too many. Sending rejected states to one real
common return instead gives 153/181 at 724 bytes. Naming the real scroll
continuation as well gives the current 155/181. State zero still performs the
fade and reset, states one and two update scrolling, and all negative states
or states at least three return without those actions. Exactly one state
observation is used, with no barrier, volatile qualifier, extra RAM read,
constraint or fabricated comparison.

### Qualified remaining delay-slot boundary

The explicit destinations preserve separate signed bounds through the
compiler passes. The lower branch's delay slot receives the upper comparison,
as in native code. The remaining upper-branch mismatch is later: the second
scheduler places the first saved-register restore before the return-zero
assignment in the common exit. Its dump reports priority four/cost two for
the restore and priority three/cost one for zero, issuing the memory operation
first and then the ALU operation in the same modeled cycle. The delay-branch
pass copies that restore into the upper branch's slot and retargets the branch
past its original copy. Native instead uses an annulled zero-return assignment
there. This is a qualified scheduler/delay-fill boundary, not the old
compound-range-fold blocker.

Changing only the fade path back to a direct return restores the native
upper branch-likely/zero-slot shape, but emits 732 bytes, eight too many, and
108 words match in the compared prefix. That control is not retained. No dummy
return-value use or unrelated memory dependency is justified to force a slot.
Other residuals include the centered coordinate's register choice, fade-reset
store scheduling and the later division schedules. They are not established
as consequences of the state guard alone.

Potentially overflowing row/scroll coordinates, the noncentered width
subtraction, scroll increment and end-row arithmetic use explicit unsigned
wrapping before signed interpretation. Signed division remains truncating as
in native code. The centered width calculation and remaining division offsets
are within full-range signed bounds. Conversions back to `s32` use the pinned
EE compiler's low-word interpretation, not a portable-C out-of-range guarantee.
These arithmetic changes preserve the 155-word output, including all
callback-sensitive reloads.

Plain and diagnostic builds of the final source agree on six allocated
sections, 108 non-file symbols, 80 ordered relocations and normalized assembly.
The five existing C functions and owned data remain exact. Uninitialized-value
and implicit-declaration diagnostics are clean and produce identical output;
these options are not production flags. The adjacent guarded `ObjDispOff`
improvement is unchanged.

## ObjDispOff (staff/stfMain, 0x504)

311 of 321 words match at the native 1284-byte size in the guarded draft.
The target remains assembly-backed; this is not exact-match credit.

The earlier 303-word draft established the following ordinary source layout.

The two things that got it there, from 87 matching words, are both layout. The
`x < -0xFF` fallback must be the `else` of `if (x >= -0xFF)`, so retail's
forward `bnez` puts it out of line. And `width = 0x100` has to precede
`fade = 0`, because retail's `bnez` delay slot holds `daddu $5, $0, $0`, which
serves as both `fade` and the `y` argument of the fallback's `DrwSprite2D`;
with `fade = 0` first the draft emits that zero twice and is 14 words worse.

The earlier 303-word draft emits `movn $4, $2, $3` for the cover clamp,
where retail keeps the result with its input in `movz $2, $5, $3`. The old
explanation attributed the later register and constant-order differences to
this same clamp. Qualified controls now separate those effects. Ruled out: `>= 0x201`
for the clamp, both ternary spellings, loading `unk_0100` into the same local
first, every permutation of the local declarations, and splitting the two uses
of `alpha` (which costs 16 words).


### Qualified clamp dependency and bounded controls

The plain and diagnostic compiler builds agree on six allocated sections,
109 non-file symbols, 86 ordered relocations and normalized assembly. In the
old draft, if-conversion defines the clamp result in a fresh pseudo from the
product and the constant. The subsequent **regmove** dump explicitly records
that it fixes the constant operand to match the destination, rewrites the
result to the constant's pseudo and updates the following comparison. Thus
the `movn` direction is already determined before register allocation; it is
not merely an allocator tie inferred from final disassembly.

A localized binding of the actual cover input/result to native register `$2`
keeps that value together through regmove and gives 310 of 321 matching words.
It is initialized by the real product and dies at the first clamp-result test,
before subsequent calls. No empty operand or fabricated consumer is needed.
The binding is a reconstruction aid, not evidence of original source spelling.
Adding a read/write empty operand before the clamp is worse at 301/321 and
is not retained. Removing the binding restores 303/321.

Separately, the actual fade numerator is assigned to `fade` and then divided
by two, retaining that real scalar across the two phases. This gives the
current 311/321. It restores the native numerator register but does not repair
all instruction scheduling. Explicit unsigned wrapping for products, affected
coordinate sums/subtractions and counters, and unsigned alpha packing,
preserve that output without relying on signed overflow or signed left shifts.
Conversions back to `s32` use the pinned EE compiler's low-word interpretation,
not a portable-C out-of-range conversion guarantee. Other subtractions and
divisions remain within their branch-proven ranges.

The ten remaining words belong to four distinct phases: the two entry step
constants; two words in the opposite-direction slide product; the width-range
predicate and branch; and four instructions scheduling the fade division and
remaining-width calculation. They are not all knock-on effects of the first
clamp. Native-register controls for the actual step-one value, slide product
and fade numerator give 304/321, 309/321 and 310/321 respectively when based on
the isolated 310-word clamp control; none is retained. Reversing the ordinary
step initializer/condition on the old baseline gives 302/321.

All measured controls preserve the five existing C functions and owned data.
The hardened draft also compiles without uninitialized-value or implicit-
declaration diagnostics and with identical output. Diagnostic flags are not
production options. The remaining work requires evidence for those separate
value lifetimes and scheduling dependencies, not more clamp spelling variants.
