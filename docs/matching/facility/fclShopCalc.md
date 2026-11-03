# Matching notes: facility/fclShopCalc

## shop_Process (facility/fclShopCalc, 0x100)

37 of 64 words match at the exact 256-byte size. A fresh complete-owner
check retains all sixteen existing C peers and reports no owned-data differences.
The draft remains assembly-backed.

Native dispatch observes signed-byte state once: zero enters its body,
negative goes to shared step reset, eight enters its body, and other values
also reach reset. Both native body-entry branches are `beql` with the step
load in their delay slots. The two-case draft lacks the negative guard.
An older explanation attributed this to `emit_case_nodes` and inferred a
third empty case; the current measurements do not establish that internal
mechanism or the original source's switch cardinality.

Historical rejected controls: empty `case -1:` or `case -2:` (42/64, one
word over), empty `case 9:` (32/64, one word over), and the inline nested
zero/nonnegative/eight chain (43/63).

A new ordinary explicit-destination control names only the actual zero,
eight and shared-reset blocks, with one signed state observation and the
native negative guard. It regresses to 22/64 at 264 bytes, eight excess.
All sixteen existing C peers remain exact. The control restores `bltz`,
but emits a non-likely zero branch with a pointer move in its delay slot,
an inverted eight/default branch, then a separate branch and step load.
The late flags register and branch-likely differences also remain.

Each baseline/control plain versus `-da -fsched-verbose=5` pair agrees in
all five allocated sections, non-file symbols, ordered relocations and
normalized assembly. Current pass snapshots show the control's separate
comparisons and actual destinations; they do not prove an internal switch
lowering decision or historical spelling. All callback-sensitive global
reloads are preserved, with no artificial case, bound or consumer.

Next evidence must explain the native two likely body-entry branches and
step-load delay slots while retaining the real callback observations.
Extra empty-case guesses and repeated inline-if controls are not a new
premise. The explicit-destination control is rejected, not a promoted draft.

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
