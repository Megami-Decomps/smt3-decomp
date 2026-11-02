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

## nbFindBadCurseTarget (newbattle/nbEncProcess, 0x18C)

The guarded draft now emits the native 396 bytes, with 58 of 99 instruction
words matching. It remains assembly in production and earns no exact credit.
Target-only whole-owner checks preserve all 21 existing compiled peers.

The enemy branch captures the actual devil ID once, then takes a typed
entry cursor within each of the three five-action tables of the full AI row.
Each cursor advances only within its five-element array, including its legal
one-past value after the final iteration. The shared row is 164 bytes, with
three condition records followed by three action tables; the older
four-action-table view at an interior symbol was merely a stride overlay.
The independently observed script ID and death-script/event fields complete
the same row, rather than separate overlapping arrays.

The player branch preserves the work pointer and re-reads its skill count
after each predicate call. The enemy branch reads each skill before its
predicate and does not refresh the captured ID across callbacks. Both append
the original sixteen-bit skill to the signed scratch array, then use the
existing RNG call and signed-halfword result. No extra filtering, capacity
limit or initialization is introduced.

The target-only baseline emits 408 bytes. Correcting the row type alone is
inert. A full-row-pointer plus scoped entry cursor emits 392 bytes; capturing
the actual ID instead of the full-row pointer gives the preferred 396-byte
form. A common result variable at the return tail also emits 392 bytes and
was rejected. Earlier all-NON_MATCHING scores used a different compiler
context and are not promotion evidence.

The remaining 41 word differences include saved-register choices, the skill
field displacement, the outer loop's two native address inductions and the
return-tail shape. No register constraints, artificial arithmetic expansion
or cursor outside the actual typed subobjects is justified by this residual.
