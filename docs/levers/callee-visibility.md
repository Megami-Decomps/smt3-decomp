# Callee visibility: the cause of most one-word branch-likely misses


This is the single most productive mechanism found so far, and it is one cheap
test rather than a search. Whether ee-gcc emits `beq` or `beql`, `bne` or
`bnel`, `bgez` or `bgezl` for a given test is decided by whether the function
that test's arm **calls** is visible in the unit as a C definition. Nothing
about the branch's own source shape reaches it.

It runs in both directions, so when a function is one or two words out and the
difference is an annul bit:

1. Find the callee in the arm whose branch differs.
2. If it is still `INCLUDE_ASM`, the caller will probably close by itself once
   the callee is C. Do not permute the caller.
3. If it is already a C definition, test the reverse: in a scratch copy, rename
   that definition so only the file's `extern` remains, and recheck the caller.

In `newbattle/nbMakePacket` one callee, `nbFreeEffectSlot`, was costing four
functions at 1, 1, 1 and 4 differing words. Reducing it to the `extern` the
file already carried made all four exact. It is byte-exact on its own, so it is
parked behind a `NON_MATCHING` fallback: a matching function can cost more than
it earns, and nineteen more of its callers are still in assembly.

Ruled out as causes, measured: every spelling of the caller's own guard, the
callee's declaration style including K&R and no prototype at all, and cc1 heap
shift, since twenty unused `extern`s above the callee change nothing. It is the
definition's identity alone.

A side effect worth noticing: the four callers that discriminate all sit above
that callee's address, which is evidence for a retail translation-unit boundary
between them. A callee-visibility result is also a boundary hint.

Two neighbours that both look responsible are not interchangeable, and the
fallback is not a weaker form of renaming. In `effect/billManager`,
`dds3Billboard_P2ADraw` is one annul word out, and parking
`dds3Billboard_P2AAnimDraw` behind the fallback closes it exactly as renaming
the definition does. Parking `dds3Billboard_P2ASetNumber` instead leaves it at
one word, although an earlier reading reported either neighbour would do and
that the fallback did not reach the effect at all. Both halves of that were
measured wrong. Identify the neighbour by testing one at a time, and use the
fallback, which is reversible and keeps the draft.

Confirmed again in `event/evtPolygonMovie`, and this time on a callee that does
*not* match: `evtHokanFilter` is 71 of 72 words with a single `bnel` for
retail's `bne`, and compiling the parked `evtHokanColor` draft unguarded makes
it byte-exact while `evtHokanColor` itself stays 9 words out. So the mechanism
also tells you which pair to attack: a one-word annul miss next to a parked
draft is a reason to spend the budget on the draft rather than on the caller.
