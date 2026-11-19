# Matching notes: facility/fclShopCalc

## shop_Process (facility/fclShopCalc, 0x100)

64 of 64 words match at 256 bytes. The dispatch observes signed-byte state
once: zero and eight enter their respective bodies, negative states join the
common step reset, and other nonnegative states reset the step before joining
the flag check. Expressing the last case as a separate real assignment and
retaining explicit body destinations lets the compiler emit both native
branch-likely instructions with the step load in their delay slots.

The callbacks, callback-sensitive global reloads, and final flag test are
preserved. The earlier two-case switch lacked the native negative guard;
adding empty cases or using a single shared reset with explicit body labels
did not produce matching output. No assembly or compiler constraints are
needed, and the matching shape does not imply the original source spelling.

## shopDrawBuyRow (facility/fclShopCalc, 0x20C)

131 of 131 words match at 524 bytes. The native bottom-tested item loop retains
row-count checks before item lookup, item-zero exit and the common footer. A
real initialized one survives the price/item callbacks and feeds the disabled
flag after a nonzero heart result. Both sell-price calls remain distinct; a
second-price local sequences that callback before the global money read.
Signed-byte mode retains all zero/nonzero cases. Fresh signed selectedTop and
unsigned rowCount observations remain at the required callback boundaries.
Row height is reloaded after row text; unsigned shift/add expresses native
low-32-bit y wrapping before the pinned target's signed conversion, without
assuming positive heights or bounded coordinates.

One initialized real list pointer is locally bound to callee-saved `$21`.
Both actual window calls consume that same pointer; there is no duplicate work
pointer, empty asm, dummy use or invented operation. The ordinary identical-
consumer control matches 119/131 words at the same size: its twelve differences
are saved-register allocation and dependent prologue scheduling. Final and
ordinary complete owners were independently compiled plain and with `-da`;
normalized assembly, text and relocations agree within each pair. This is
output-parity/allocation evidence, not an unobserved internal allocator trace
or a claim of historical spelling.

Native and final save incoming `$21`, initialize the real list address once,
and never overwrite it until epilogue restoration. Its actual consumers span
calls; the evolving y occupies `$22`, with no overlapping live value. All
thirteen direct callees and their tails were audited. The three direct bodies
that write `$21` save/restore it. Initialized list-panel type-1 draw/update and
price-panel type-2 callbacks were also checked: type-1 draw and its border tail
preserve `$21`; update and type-2 draw/tails do not modify it. This relies on
the existing valid-callback ABI, not callback purity, arbitrary corrupt
function pointers or a lexical lifetime claim. Native carries the same pointer
across these calls. Footer global loads, callback order and real provider types
are unchanged.

The final target-only source excludes an unrelated sell-row y-wrap edit caught
during draft isolation. All bytes outside the target replacement are preserved.
Independent complete-owner/data review gives 16 match, 0 differ. Full retail
checksum, 149 guarded owners / 578 drafts, seven IRX owners and 26 tooling files /
427 tests pass. Actual published-checkout verification proves all 780 tracked
files, 24 owner dependencies, report/provenance equality and no-op builds.
Source commit `7aca43983e4f34d6d3f452cff3794c69cbf18fc7` passed exact-source hosted CI
[40999998682](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40999998682),
including build and deploy.
