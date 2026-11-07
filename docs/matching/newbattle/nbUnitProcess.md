# Matching notes: newbattle/nbUnitProcess

## UpdateFormAnime (newbattle/nbUnitProcess, 0x260)

152 of 152 words match at 608 bytes. Independent complete-owner verification
gives 39 match, 0 differ, including owned data and full compiler context.

The typed reconstruction preserves the dying-motion gate, acceleration and
progress clamps, VU0 position interpolation, pose callback, movement-bit
clearing and HP scale update. Status and blend are reloaded after the pose
callback; the no-callback path retains its snapshots, as retail does.

Two localized reconstruction aids retain real values with distinct consumers.
Inside the active-animation branch, `movement = anime` feeds the actual
`0x20` vector-selection test. `FAKEMATCH_ASM_LIVE_VOLATILE(movement)` preserves
that copy without a fixed integer register or a new memory access. The real
`interpolationBlend = blend` input to the second vector scale is locally bound
to FPU `$f2`; the original blend still feeds the complementary weight and
completion comparison. No empty float constraint is needed. No call occurs
while the bound float is live, and the intervening macros use COP2 vector
registers rather than clobbering FPU `$f2`. Both added snapshots are dead before
the pose callback. The empty constraint is not a memory barrier.

The ordinary separate float local is inert at 138/152 words. An unbound float
live constraint reaches 143/152; the float binding alone reaches 149/152.
Combining that binding with the branch-local volatile movement constraint is
exact. Deleting the float binding from the exact source gives 140/152;
deleting the movement constraint gives 149/152. All these controls preserve
38 exact peers and owned data. These are measured source/output dependencies,
not an internal-pass attribution, proof of original source spelling, or a
claim that no simpler ordinary matching form exists.

Integrated retail SHA1 and byte comparison, no-op rebuild, 436 tooling tests,
inline-asm gate, all 144 guarded owners / 501 drafts with stable inputs,
7 IRX checks and reconciled reports pass. Source commit
`a16a594a9c235c3065897e325641a8484821bb92`; exact-source
[CI 41406322067](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41406322067)
attempt 1 passed build and Pages deployment.

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

## RunSake (newbattle/nbUnitProcess, 0x104)

All 65 of 65 words match at 260 bytes. The four local f32 bindings carry
actual interpolation values: endpoint 1 in f2, positive range-update product
in f5, negative power in f4, and negative range-update product in f5. Products
are assigned before their sole uses in mutually exclusive branches. Every
bound value dies before either pose call. No empty operand, fabricated use,
provider/type/layout/flag change or new instruction is introduced. Redundant
phase bindings and next-power locals were removed. These are allocation aids,
not historical-source claims or proof of an internal compiler-pass cause.

Null basic clears time and type without reading shake fields or position.
The old range supplies displacement in both branches. Positive interpolation
uses (1 - old range) * old power, publishes power + 0.02 and the updated range,
and at range >= 0.99 stores (-addmovet) * 0.6. Negative decay uses unary -power
and its real product, publishes power - 0.01 and the range subtraction. At
range <= 0.01 it clears only type and publishes the original position. The
other pose path adds displacement only to the copied position.x. Separate
single-precision operations, unary negation, loads/stores, annulled delay
slots and callback order remain native; no reassociation or FMA is substituted.

Actual EE header assertions check formation size and all consumed field
positions/widths. The original 16-byte position at +0x20 is copied by lq/sq
into the real aligned stack slot. The unchanged canonical pose provider
consumes const sdfVector4_t synchronously; the argument does not escape.
The complete provider owner is checked in the integrated generated context,
in addition to the complete candidate owner and full linked retail image.

Independent final ablations all remain 260 bytes and preserve the 39 peers:
endpoint removed 44/65; negative-power binding removed 49/65; positive product
removed 59/65; negative product removed 58/65; all four removed 45/65.
Guarded owner is 39 match, 0 differ; final owner is 40 match, 0 differ.
Earlier diagnostic source forms are not alternate landing recommendations.

Fresh qualification covers all 23 complete sources in production, SKIP_ASM
and retained contexts: 69 entire-assembly/entire-ELF plain/-da pairs, with
only one equal-length random source filename substituted in each whole output.
This is plain/dump parity, not hooked/unhooked trace qualification or a
syscall-complete compiler read trace. The named source/header/config/generated
assembly/tool/runtime input closure is hashed before and after. Strict implicit
declaration builds pass for guarded, ordinary, final and all five final
ablations. The inherited RegistUnit incompatible-pointer warning remains
identical after path/line-location normalization; no all-warnings-clean claim.

All 39 peer functions per mode and allocated nontext data are preserved in
66 resolved comparisons against enabled baseline. The guarded production and
retained pools also agree. Activating guarded SKIP inserts exactly 16 target
literal bytes at .lit4+8, shifting RunDamage's later literal reference by 16;
explicitly mapping that insertion preserves every guarded peer contract and
all other data. Guarded-SKIP literal bytes are not claimed identical. Target
relocation offsets can move among ablations; whole raw relocation identity
across different sources is not claimed.

Actual-source finite-domain fixtures, including thresholds, signed zeros,
subnormals and callback mutation, pass 6,000,000 bitwise comparisons each in
optimized and ASan/UBSan builds. Raw-bit stress separately passes 6,000,000
comparisons per build with explicit NaN payload/sign equivalence: optimized
has zero NaN-only differences, sanitized has 32,844. The original failing
sanitizer example, range 0x7FC00000 versus 0xFFC00000, is retained as a host
limitation. No universal host bit-equivalence or EE exceptional-arithmetic
simulation is claimed; only leak checking is disabled for the traced runtime.

Native literal bits are 0.02=0x3CA3D70A, 0.99=0x3F7D70A3,
0.6=0x3F199999 and 0.01=0x3C23D70A. Modern host 0.99/0.6 conversion differs.
Exact EE instruction/operand/literal bytes and independent native review,
not host NaN propagation, establish the target contract. No source finite
restriction, sanitization or changed exceptional behavior is introduced.

Full current-tree retail/generated-source, 505 no-skip tests plus 818 layout
claims, frozen EE/IRX drafts, inline policy and report-scope checks pass.
Production provenance establishes one compiler-owned linked 260-byte span
with no target fallback input. Prior peer changes and preceding matches are
preserved. This adds one game function / 260 bytes; SDK coverage is unchanged.

### Historical residual and rejected controls

The following records the earlier ordinary draft and its limits, before the
current real-value constraints. The actual owner is nbUnitProcess.

45 of 65 words match, at the exact 260-byte size and with the same instruction
multiset.

Three facts are settled. The quadword position copy is
`SDF_VU0_COPYVECTOR(&pos, &form->pos)`: the macro's two `"r"` operands are what
give retail's `addiu $3, $5, 0x20` address temp per copy site, where a `u128`
cast folds the offset into the `lq` and a struct assignment of a 16-byte
aligned type drops to `ld`/`sd` pairs. `form->basic` is a local read once at
the top, which keeps `form` in `$5` with no copy. And `form->range` is read
inline inside each arm rather than through a shared local, which is what leaves
the `bc1f`'s delay slot empty for the else arm's `neg.s` and gives retail's
`bc1fl`.

What is left is the float register assignment: retail has `phase` in `$f1`,
`1.0f` in `$f2` and `-power` sharing `$f4` with the `0.99f` constant, while the
draft pairs `1.0f` and `-power` in `$f1` and puts `phase` in `$f2`. The
schedule shifts with it, and in both arms retail's literal load comes one word
before the `form->damageRange` load where the draft has them the other way
round.

Ruled out: all six statement orders in the two arms, in each of three spellings
of the `form->range` update (`+=`, the written-out form, and the multiply
commuted) and both operand orders of `form->damageRange * form->range`, which
all produce the identical object; an `f32 q = -power` local for the negation;
and `f32 phase`/`f32 power` locals shared by both arms, which reload nothing
and cost 2 words.
