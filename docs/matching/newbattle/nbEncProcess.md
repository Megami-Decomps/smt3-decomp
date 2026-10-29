# Matching notes: newbattle/nbEncProcess

## nbApplyBadCurse (newbattle/nbEncProcess, 0x174)

80 of 93 words match at the native 372-byte size. All ten jump-table entries
match. The remaining thirteen differences begin at +0xE4 in the timer-table
scan: its index, count, table base and entry-address registers differ, including
the order of the base-address and scaled-index instructions. The assembly
fallback remains enabled; all nineteen existing C functions remain exact.

The initializer consumes `nbActionProcessData_t *` through its formation and
unit fields. The dispatcher also explicitly passes the action context to
`nbGetBadCurseStep`, although the selector uses only the weighted table. Its
existing body remains exact with the recovered unused parameter. The retry
limit calls Step0 and independently forces the loop result to zero, and the
dispatcher returns zero. The draft preserves those contracts rather than
relying on unspecified argument registers or a helper's incidental result.

Cases 0 and 9 invoke Step0; cases 1 through 7 invoke the corresponding helpers;
case 8 preserves the current result. Out-of-range steps set the result to one.
After a nonnegative step with a nonzero result, clear the first matching table
entry's weight. Stop after a successful step or eleven iterations.

A direct indexed weight store adds a redundant address calculation and four
bytes. A per-iteration entry pointer reaches native size. Moving that pointer
to function scope, spelling its address as base-plus-index, or capturing the
timer locally does not resolve allocation. A pointer-increment traversal
changes more of the loop. No register pins, dummy uses or alternate types are
used; another attempt needs a concrete lifetime or compiler-allocation premise.
