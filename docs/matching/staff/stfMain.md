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
these options are not production flags. The later `ObjDispOff` promotion below preserves this retained result.

## ObjDispOff (staff/stfMain, 0x504)

321 of 321 words match at the native 1284-byte size. The complete owner is
6 match, 0 differ. The still-guarded stfMain2 retains exactly its original
26 differences at 155 of 181 words and 724 bytes.

The prior 311-word draft had four independent residuals, resolved separately before combining them:

- A real initialized `normalStep = 1` local before `step = 2` preserves the native entry-constant order in ordinary C. The conditional still chooses the same 1/2 step.
- The opposite-direction slide binding starts at the actual frame input, before the wrapped multiply, and ends at the x calculation. Binding only the product had not preserved this destructive-input choice.
- In the other direction, the real frame input and product/test scalar retain native homes through the clamp and width test. Both die before drawing calls. Binding only the predicate changes the preceding product/clamp allocation and is insufficient.
- A single nonvolatile live operand on the actual fade numerator, immediately before its signed divide, preserves the native division/remaining-width schedule. It emits no instruction and is not a memory barrier.

The bindings and empty operand are localized reconstruction aids, not evidence of original source spelling. All values are initialized and have real consumers; no fixed value crosses a clobbering call. Explicit unsigned wrapping, signed divisions, color packing and callback-sensitive global reloads are unchanged. Two local declarations now agree exactly with their existing providers: signed rsetOtOfs input and signed DrwSprite2D packed-color input. This declaration correction is byte-inert.

Qualified stock regmove, local-allocation and scheduler dumps agree with plain full assembly and ELF output in production, SKIP_ASM and retained contexts. Ordinary/direct outputs and uninitialized/implicit-declaration diagnostic outputs also agree. The complete owner is 6/0, peers and owned data are preserved, and the still-guarded stfMain2 remains 155/181 at 724 bytes.

Rejected new controls include a live operand on normalStep, predicate-only bindings/live operands, using x for the width predicate, a live operand on the clamped width, and additive slide rewrites. The useful properties are the ordinary initial step value, the slide input's actual lifetime, the paired width input/test lifetimes, and the real fade numerator operand; none of the previous cover-clamp spelling controls is needed.

Independent native, range, provider and fixed-value lifetime review approves
the unchanged source. Production, SKIP_ASM and retained ordinary/direct/strict/
selective-stock outputs agree over complete assembly and ELF. Strict
uninitialized-value and implicit-declaration builds are warning-free. All six
non-target functions and allocated non-text sections are unchanged in the
production and retained comparisons. No header, layout or production compiler
option changes. Aggregate retail and repository checks remain release gates.

### Earlier guarded draft, superseded

311 of 321 words match at the native 1284-byte size in the guarded draft.
The target then remained assembly-backed; that draft earned no exact-match credit.

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
