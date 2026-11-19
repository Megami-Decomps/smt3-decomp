# Matching notes: newbattle/nbEncProcess

## nbApplyBadCurse (newbattle/nbEncProcess, 0x174)

The ordinary C implementation matches all 93 instruction words (372 bytes),
and the complete owner matches 25 functions with no differences. Directly
indexing the two table fields produces the retail scan; the earlier temporary
row-pointer form differed in 13 words. No compiler constraints are needed.

Cases 0 and 9 invoke Step0; cases 1 through 7 invoke the corresponding helpers;
case 8 preserves the current result. Out-of-range steps set the result to one.
After a nonnegative step with a nonzero result, the scan clears the first
matching entry's weight. The dispatcher stops after success or eleven
iterations and returns zero. The initializer and step selector receive the
action context, including the selector's unused recovered parameter.

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

## nbSetBadCurseTimer (newbattle/nbEncProcess, 0x2C)

The native 80-byte table is represented consistently as ten rows of two
signed 32-bit values. A row is now an actual `s32[2]`, with step at `[0]`
and weight at `[1]`; selector, logging and retained dispatcher accesses use
the same representation. No cast, constraint, changed bound or new access
is introduced. The count still exposes only fully initialized prefix rows.

In the complete current owner, this ordinary representation changes the two
remaining setter instructions and reproduces all eleven native words. The
44-byte function excludes the following four-byte alignment nop. Complete
owner checks report 24 match, 0 differ. All production and retained peers,
including `nbFindBadCurseTarget` and `nbApplyBadCurse`, retain
their instruction and ordered relocation contracts. Plain and stock-dump
assembly and raw ELF output agree in all four checked macro contexts.
This is a measured reconstruction, not evidence of the original source
spelling or a claim about an unobserved compiler pass.
