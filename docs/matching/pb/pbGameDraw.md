# pb/pbGameDraw

## pbRebuildOutline (pb/pbGameDraw, 392 bytes)

All 98 of 98 words match at the native 392-byte size in ordinary C. Both
actual block lookups add `(u32)i * sizeof(pbBlockEntry_t)` to
`EE_ADDR(group->blk)` and convert the result with `EE_ADDR_AS`. The offset is
an integer throughout, not a fabricated pointer. No register constraint,
false qualifier, changed type, provider or compiler flag is needed.

This explicitly uses the configured EE flat-32 address representation.
Sixteen compile-time assertions qualify pointer, u32 and sizeof-expression
widths and the existing block/group/edge layouts; the assertions are
output-neutral. Within live aligned allocated storage, with no address wrap,
the result names the same block as typed array indexing. This is a
target-dependent pointer/integer conversion, not portable ISO-C provenance.

`pbAddGroupEntry` allocates `num * sizeof(pbBlockEntry_t)` bytes and stores the
locked allocation pointer in `blk`. Each 0x90-byte block has an outline count
at +6 and eight four-float vertices at +0x10. The 20-by-20 map supplies at most
400 entries per constructed group. More generally, a nonnegative index below
the unsigned-halfword count has a byte offset below 9.5 MiB, without u32
multiplication overflow. Valid current allocation/count relationships remain
caller preconditions; zero count never evaluates the block pointer.

Both lookups observe the current block base, including after `pbGetBlockAt`.
Coordinates remain snapshots taken before that call, the outline count is
reset before it, and destination/count are observed afterward. The outer loop
reloads the group count. Four low edge bits append at most eight vertices
from the existing four-row template at `D_00327780`, preserving order and
writing only XYZ; W is untouched. Signed-halfword coordinates times 40 fit
s32. The actual neighbor reader requires valid map coordinates 0..19 and
reads rather than mutates group storage; no new invalid-input protection is
introduced.

The ordinary typed-array baseline and a byte-pointer/sizeof control have
identical complete output: 96 of 98 words match, with only the two `addu`
operands commuted at +0x4C and +0xDC. Restoring just the first typed lookup
leaves only +0x4C different; restoring just the second leaves only +0xDC.
Each single-address control matches 97 of 98 words at 392 bytes. These
ablations isolate both real address dependencies, without claiming an
unobserved compiler decision or historical source spelling. The earlier
index-first typed-pointer control was already inert and was not re-swept.

Complete-owner verification gives 10 match, 0 differ, preserving all nine
peers and owned data. Seventeen complete plain/dump output pairs pass,
including original, final, typed and byte-view sources in production,
SKIP_ASM and retained-C modes. Twelve corresponding strict declaration builds
are silent. Production original/final allocated sections and raw relocations
are identical; native/compiler local labels change, so full symbol-contract
identity is not claimed. In retained-C mode, only the two target words change.

A bounded host fixture extracts the actual consumer-view types and bodies,
using offset-preserving uintptr_t address overrides. All 172,032 comparisons
pass separately at O0, O2 and with address/undefined-behavior sanitizers.
Controlled callbacks replace the block base, change the count and mutate
coordinates while preserving valid capacity; full arrays and observations
agree. A deliberately stale destination pointer produces 24,224 mismatches,
qualifying that negative control. These mutations test stronger equivalence
than the real read-only provider; this is not native gameplay execution.

The host fixture does not establish EE conversion behavior or whole-game
host portability. Default address macros truncate wider host pointers, and
inverse opaque-token mappings alone do not preserve byte offsets. There is
also inherited cross-owner layout debt: the producer's group puts `blk` at
+0xB8 on x64, while this consumer's opaque-prefix view keeps +0xB0; both use
+0xB0 on EE. The fixture allocates the consumer view correctly and does not
claim to repair that existing host mismatch.
