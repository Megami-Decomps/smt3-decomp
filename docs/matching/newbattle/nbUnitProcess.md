# Matching notes: newbattle/nbUnitProcess

## UpdateFormAnime (newbattle/nbUnitProcess, 0x260)

138 of 152 words match, at the exact 608-byte size. Production remains
assembly-backed.

The typed draft preserves the dying-motion gate, acceleration and progress
clamps, VU0 interpolation of the position, pose callback, movement-bit clearing
and HP scale update. It keeps pre-update progress separate from the published
blend value, and clears movement bit 0x20 from the captured animation status.
After the pose callback it reloads both status and progress; the no-callback
path retains the snapshots, as retail does.

The fourteen remaining words are concentrated in the interpolation block.
Retail copies animation status from $6 into $3 in the first branch delay slot,
and blend from $f1 into $f2 before subtracting the complementary weight. The
draft coalesces these values, changes the first branch to its annulled form,
and shifts the scale/vector-selection sequence. All other instructions match.
A separate movement local and a one-pass complementary-weight block did not
restore either copy. No dummy copies or register constraints are retained.

## nbAddUnitStock (newbattle/nbUnitProcess, 0x370)

220 of 220 words match at 880 bytes. Separate side switches retain the
player 0..3 and enemy 4..13 scans, u16 flag masking and native exhausted-slot
behavior. Entry stores precede descending count clears; counters remain
halfword and unit flags word. The work pointer survives ID/visual callbacks,
while current global/enemy bases are reloaded for later flag updates.
Hearts lookup is short-circuited by work flag 4. Its affinity-copy path loads
the party after the callback; the datGetAisyo path reloads it after each call.
Formation flag stores, autowait cases, the existing aligned SDF vector copy,
float/access ordering and final nbGetMotionNo tail call remain native.
No volatile accesses or new invalid-input guarantees are introduced.

Three local empty `+r` constraints place freshly loaded, actual party bases
in `$3`: one per scan arm and one for common entry setup. Each feeds real
indexing or stores and dies before any call. Removing all three yields
872 bytes rather than 880. An earlier fourth entry constraint is unnecessary
and absent; there are no dummy values, consumers or extended lifetimes.

Three address expressions add an unsigned 32-bit byte offset to the real
base converted to u32, then convert the resulting address to its truthful
pointer type. The earlier fabricated offset-pointer formulation was rejected
and is not retained. This is explicitly nonportable EE flat-32 addressing:
canonical compiler probes establish four-byte pointers/u32, 0x80 party,
0xF0 formation and 0xEC unitwork layouts; roundtrip and party-address code
preserve representation and perform shift-by-7 plus integer addition.
Real array-pointer and byte-pointer controls change `addu` operand order.
The pinned ABI evidence does not establish portable ISO C semantics.

Ordinary party-pointer reuse spans two disjoint real phases: initial entry
stores and later hearts-affinity copy. Both definitions have actual consumers;
the second reload follows the callback. Paired complete-owner traces show
shared pseudo92 survives local allocation and receives hard2 globally,
whereas a separate initial-entry local leaves the later party pointer local
to block18 and allocated hard3. The separate control remains 880 bytes but
has nine differing words, with peers/data unchanged. Each source separately
passes plain/dump normalized assembly, allocated-section headers/contents,
relocation/symbol contracts and function-inventory parity. These measured
allocation facts qualify live reuse, not historical spelling or an inferred
internal compiler decision algorithm.

Independent complete-owner verification gives 38 match, 0 differ, preserving
all 37 peers and owned data. Integrated retail checksum and full regression,
guarded EE/IRX and report gates passed for source commit
`ed06b35d5318943f87268e43fa00e2c7150d03bc`. Exact-source CI
[41038394824](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41038394824)
completed successfully, including every build/deploy step and retail verification.
