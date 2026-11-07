# Matching notes: pb/pbGame

## deleteMoveBlock (pb/pbGame, 0x144)

46 of 77 words differ, 308 bytes against retail's 324. The first 31 words, both
record arms and the `pbStartMoveBlock` call are exact.

What is left is the `index == 4` arm. Retail reads `work->block` once, keeps a
second copy of it in another register, zeroes the counter before loading
`num`, and reloads `block->blk` on every iteration while holding
`D_0054F5C4 + 0xC` in a register across the loop. ee-gcc hoists `block->blk`
out of the loop instead, which is where the four missing words go, and sinks
the counter's zero into the guard's delay slot.

Ruled out: `block` as a local assigned before and after the two leading
calls, and `work->block->` at every use; the counter zeroed before the
`block` load, after it, and as a `for` init; `block->blk[i]` against
`(block->blk + i)`; and the map word through a cached `pbMapArea_t *`
(`D_0054F5C4 + 0xC`, retail's form) against the plain
`D_0054F5C4->area.map.blk[y][x]` expression (`+ 0x1C`). None of them makes
gcc reload the entry array.

Confirmed on the way: the `index != 4` arm has to be written first, since
retail branches to the `== 4` arm; `moveB` holds a `pbPanel_t *`, not an
`s32`; and `pbDeleteGroupEntry` takes the group id from `block->grpID` at
`0x4`.

## pbGetBlockAt (pb/pbGame, 0x120)

60 of 68 words differ, 272 bytes against retail's 288. Only the three `sll`/`slti`
words at the top match.

The four neighbour tests are each `if (out of range || (cell & 0x7FF) != grp)
ret |= bit;` over `D_0054F5C4->area.map.blk[y][x]` with the neighbour offset
in the load displacement, which is the right shape: retail recomputes the
cell address and reloads `D_0054F5C4` four times, as the draft does. Two
things differ. Retail if-converts the first test to `xor` plus
`movn $8, $5, $3` with the 0 in the guard's delay slot, where ee-gcc emits a
second branch; and retail allocates `x` to `$9` and `y` to `$7` where the
draft gets `$7` and `$5`, so every later word is shifted by the register
numbering.

Ruled out for the first test: the `||` chain with `ret` zeroed at the top;
`if (x + 1 < 20) ret = (... != grp) ? 4 : 0; else ret = 4;` and the same with
the arms swapped; and `ret` as `s32` rather than `u8`. The ternary does not
reach the `movn`, and none of the four changes the register numbering.

## roundBlock (pb/pbGame, 0x8C)

35 of 35 words match at 140 bytes in ordinary C, excluding trailing alignment.
The success step update and zero return follow the rotation arm; the guard's
`else` returns -1. This places the two real clamp stores before the guard exit
and the shared success tail, reproducing native layout without a goto,
constraint, new declaration, artificial dependency or changed evaluation.
Earlier inside-arm and goto-tail controls did not match. Whole-output parity
establishes this source shape's output, not an internal pass attribution or
historical spelling.

`step` is the zero-extended halfword snapshot, compared unsigned with four.
The native signed halfword rotation index converts to float, multiplies by
0.39269909f, and adds to the current float rotation. The first rotation store
precedes the ordered positive and negative 6.28318501f comparisons. Each clamp
performs its actual float addition/subtraction and store. Exactly one halfword
step increment follows successful rotation; the guard path leaves both fields
unchanged. Equality is not clamped, and unordered comparisons follow native
behavior. The process callback still returns -1 or zero with its original ABI.
No invalid-pointer behavior is repaired.

Independent semantic review and both current and HEAD-public whole-owner
checkers pass 65 match, 0 differ. All 70 production, 64 SKIP_ASM and 70 retained
peers survive, including the constructor. Production data is byte-identical;
SKIP_ASM gains only the target's native 12-byte literal triple at offset 24,
and all affected consumers resolve to their unchanged native values.

Three final complete assembly and entire-ELF plain/dump pairs pass with 1,137
stable inputs including actual compiler, QEMU, loaders and generated inputs.
Independent fresh qualification repeats all three modes with 1,457 stable
inputs including the Python environment. Only the exactly-once equal-length
random generated source path is normalized. Strict diagnostics reproduce
inherited `pbInitGameCamera` declaration and `sdfDrawRotatedModel` argument-type
debt; no target or uninitialized diagnostic is introduced.

Integrated verification passes the generated-source stale-line control, full
retail checksum and literal image comparison, all retained EE/FIX_BUGS and
seven IRX owners, inline-assembly/compiler-wrapper checks, both production
reports and a no-op rebuild. The local tooling suite reports 569 cases and
the clean public-tooling snapshot 505; each skips only the optional external
dvp-as comparison because that executable is unavailable. Native VU assembly
and the retail image remain exact. The linked target is a unique C-owned span
at 0x001D7950, 140 bytes, with no assembly fallback. Game progress gains one
function and 140 bytes with unchanged data totals.

## pbInitGame (pb/pbGame, 0x290)

164 of 164 words match at the native 656-byte size. The source keeps the
unsigned original-slot ordinal, separate compact output cursor, signed script
label and fixed character-array base. The compact cursor uses a local saved-
register binding through FAKEMATCH_ASM_REG; the other values remain ordinary C.
There is no empty assembly, invented consumer, artificial bound, widened type,
new memory access or changed callback.

The fixed base is initialized from the real mychar array and used to retire
original slots. The advancing cursor only increments for registered characters
and writes created handles compactly. Both pointers address the same underlying
global array, without any noalias promise. Reloads from character slots and work
after callbacks remain present. The cursor's assigned register is callee-
preserved, so its genuine cross-call lifetime is ABI-safe. The source comment
describes a reconstruction aid, not original spelling.

Ordinary fixed-base control still differs in 12 of 164 words. Binding the cursor
while leaving mychar[i] spelled directly gives 11 differences and hoists cursor
address formation. The fixed-base source plus cursor binding matches all 164
words. These paired comparisons establish the allocation/scheduling dependency
and output, not an unobserved internal-pass mechanism. Older qualified
allocation observations remain background, not a fresh trace of this source.

Complete owner: 66 match, 0 differ. Production keeps all 70 peers, SKIP_ASM all
65 peers, retained all 70 peers. Resolved HI16/LO16, call, absolute, GP-relative
and literal relocation contracts preserve every peer and owned data. The
initializer's existing pbInitGameCamera declaration is naturally exposed when
promoting its guard, removing the old implicit declaration in production and
SKIP_ASM. Strict implicit-declaration and uninitialized diagnostics pass all
final modes; the preexisting sdfDrawRotatedModel pointer-type warning is
unchanged.

Nine complete baseline/ordinary/final plain/dump assembly-and-allocated-section
pairs pass. A separate frozen final-source run verifies full assembly and entire
ELF bytes with only the exactly-once, equal-length generated source path
normalized, and hashes the input closure before and after. The 1,035-input
closure remains unchanged. Independent semantic and ABI review passes. An
extracted-actual-body host oracle passes 8,192 callback/state cases at O2 and
under ASan/UBSan, without claiming EE hardware execution. Full integrated retail
and regression gates remain required for publication.

## roundDirCheck (pbGame, 0x2B4)

173 of 173 words match at 692 bytes using ordinary C. Explicit initialized-false
axis classification retains the native less/equal/greater distinction: less
and equal leave false, greater selects true. Coordinates and centers are signed
16-bit values, safely promoted for the comparisons. A real second direction
switch separates horizontal cases 0/2 from vertical cases 1/3. The first switch
uses native body order 0, 2, 1, 3; writing groupID on the left of equality
reproduces the final three native comparison operand orders. No register aids,
asm, declaration changes, extra consumers or compiler flags are needed.

Group search occurs before character lookup; signed center snapshots precede
the position-provider call. Every direction writes stepCount=1. Directions
outside 0..3 retain rotation -1 and perform no neighbor tests. For valid cases,
the two neighbor accesses preserve native order and asymmetric bounds: positive
neighbor <20 and negative neighbor >=0, without invented complementary checks.
Each matching low-11-bit map cell sets stepCount=2 without skipping the second
access. The comparison uses the full unsigned groupID; repeated map-global
loads after potentially aliasing output stores remain intact.

Null group/character, invalid character indices and out-of-map coordinates
retain the native validity requirements, not new corruption guarantees. No
caching, noalias promise, early-return fusion or new validation is introduced.
The observed source shapes explain matching output, not historical spelling
or an unobserved optimizer mechanism. The complete owner checks 62 match,
0 differ, preserving all 61 prior C peers and owned data.

Independent guard-removed review and full qualification pass the retail
checksum, 149 guarded owners / 580 drafts, seven IRX owners, and 26 tooling
files / 427 tests. Actual published-checkout verification confirms all 779
tracked files, 40 owner dependencies, source identity and no-op builds.
Source commit `f0b01710d19a260271ee032185ecaa60a9b3f8c2` passed exact-source hosted CI
[40998601934](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/40998601934),
including build and deploy.

## pb_draw_map_cursor_Process (pb/pbGame, 0x268)

154 of 154 words match at 616 bytes using ordinary C. Real positions[4],
colors[4][4] and offsets[4][4] declaration order reproduces the native frame;
the 64-byte offset copy and every array extent are genuine. The ordinary state
switch handles 1, 5 and default; a real mask `1 << side` uses side 0..3 and is
consumed by the work-bit test. A temporary empty-asm diagnostic constraint was
removed successfully: no asm, fixed register or artificial operand remains.
All 62 prior C peers, including roundDirCheck, and owned data remain exact.

Each selected side initializes all four positions and sixteen color components
before drawing. Passing colors[side] and the corresponding UV row is intentional:
the actual sprite provider consumes four color/UV words and two 2D vertices.
Work bits and texture are reobserved across callback-bearing draws, while the
real saved work pointer is retained. No fabricated mask range or array extent
is imposed. Native constructor storage is a 16-byte panel and four-byte value;
unsigned-16-bit state semantics and the pre-null-test dereference are preserved.

The amplitude retains sdfSin's float result converted to double, multiplication
by 50.0 and then signed-integer conversion. Native integer offsets and products
remain in order. This preserves the existing valid game-state domain; it does
not promise defined C behavior for NaN-to-integer conversion, arbitrary
nonfinite phase, extreme injected sine results or corrupt pointers.

The tag callback precedes phase reload. Addition precedes the ordered
threshold comparison, equality resets to positive zero, and the native final
store placement is reproduced. Tail comparison behavior alone is not a claim
that the whole sine-to-integer path accepts nonfinite inputs. The pinned old
compiler emits literal words 0x3e4ccccc and 0x40490fda from 0.2f and 3.1415927f;
independent assembled-pool and mapped-retail-byte checks confirm both. Modern
host rounding must not replace these measured target literals. No invented
volatile pool globals remain.

Independent full-owner checking gives 63 match, 0 differ. Plain and `-da`
complete-owner output agree in normalized assembly, text, data, rodata, lit4
and emitted relocation sections. This is output parity, not a claim of
historical spelling or internal pass causality. Outside-target source bytes
are identical to the committed baseline.
Full retail checksum, 149 guarded owners / 576 drafts, seven IRX owners and
26 tooling files / 427 tests pass. Actual published-checkout verification proves
all 780 tracked files, 39 owner dependencies, report/provenance equality and
no-op builds.
Source commit `35a11cd4cd99a5a5073c1493efc915b132f1c757` passed exact-source hosted CI
[41001048339](https://github.com/Megami-Decomps/smt3-decomp/actions/runs/41001048339),
including build and deploy.

## viewModeCamera (pb/pbGame, 0x114)

69 of 69 words match at 276 bytes, excluding the four-byte trailing alignment.
The actual 0x30-byte camera work contains a scale, twelve padding bytes and
named `f32 eye[4], target[4]` arrays at 0x10 and 0x20. This replaces the old
scalar-member pointer walking with bounded indexing of genuine arrays. The
constructor and final VU0 copies use the actual element-zero addresses;
all existing peers, including `pbStartViewModeCamera`, remain exact.
Target assertions establish size 0x30 and alignment four, not sixteen;
physical quadword placement remains the native allocation/copy contract.
No union, flat record alias, new qualifier, constraint, helper or flag remains.

An ordinary state switch retains state-one flag clearing, state-five early
return and other states continuing. The four-element index is unsigned.
The scale is promoted to double, compared against 1.025, incremented by 0.005,
narrowed to float and stored. Each component retains the native float
subtraction, two real scratch stores, scale reload, multiplication, target
reload and addition before the eye store. Both native vector copies and the
callback ABI remain unchanged. Exact EE operations, not modern-host floating
point assumptions, establish the numerical behavior. Invalid input pointers
remain unsupported; the old pre-null-check dereference is not repaired.

The C now emits the two native double constants previously supplied by
`INCLUDE_RODATA` at D_00535F40 and D_00535F48. Their ownership migrates to the
compiler; values, native placement and the other data are unchanged.

Independent semantic/ABI/layout review and both public and local complete-owner
checks pass 64 match, 0 differ, versus 63 before promotion. Three final-source
complete assembly and whole-ELF plain/dump pairs pass with 692 stable explicit
source/compiler/runtime inputs, including actual QEMU and the second loader.
Only the exactly-once equal-length random source path is normalized. Preliminary
union and eight-float alternatives also matched but are unnecessary and rejected
in favor of the two natural arrays. No hidden optimizer decision or original
source spelling is claimed. Full integrated gates remain required separately.

Complete preservation auditing keeps all 70 production, 63 SKIP_ASM and
70 retained peers, including the constructor, unchanged after resolving local
code and literal relocations. Production rodata (352 bytes) and lit4 (100 bytes)
are byte-identical. SKIP_ASM adds exactly the target's 16 bytes of native double
constants before the existing string; its relocated consumers resolve to the
same data. Production constants remain at native 0x00535F40 and 0x00535F48.

Strict diagnostics reproduce baseline debt: production and SKIP_ASM see the
existing `pbInitGameCamera` declaration gap, and all modes see the existing
`sdfDrawRotatedModel` first-argument type warning. No target or uninitialized
warning is introduced; this is not a strict-clean owner claim.

Integrated verification passes the generated-source stale-line control, full
retail build and literal image comparison, all retained EE/FIX_BUGS and seven
IRX owners, inline-assembly/compiler-wrapper checks, reconciled game/all-binary
reports and a no-op rebuild. The local tooling suite reports 569 cases and the
clean public-tooling snapshot 505; each skips only the optional external dvp-as
comparison because that executable is unavailable. Native VU assembly and the
retail checksum pass. Linked production proofs identify both promoted targets
as unique C-owned spans with exact retail bytes; combined game progress rises
by two functions and 436 bytes with unchanged data totals.

## moveBlock (pb/pbGame, 0x8C)

### Scalar-vector table diagnostic (2026-11-06)

A complete-owner diagnostic declares the existing displacement table as
`extern sdfVector3_t D_00327370[]` and reads its actual x/y/z components directly.
It emits the native 140-byte extent: 31 of 35 words match and four differ.
However, the matching `moveChar` consumer regresses by two of its 49 words at its
unchanged 196-byte size. Do not land this declaration or diagnostic body.

The view is supported by the five native three-float displacements at
`0x00327370`: (0,0,10), (10,0,0), (0,0,-10), (-10,0,0), (0,10,0).
The four trailing zero bytes are not treated as another component or row.
Existing `sdfVector3_t` has size 12 and offsets 0/4/8; no bound, padding,
alignment, volatile access or compiler constraint is invented.

The stock dumps establish this boundary in production, complete-owner
`SKIP_ASM`, and retained-draft modes:

- `00.rtl` expands three separate component addresses, with explicit base+4 and
  base+8 expressions for y/z. `03.cse` reuses common table and scaled-index
  values; the three address expressions survive through `13.life`.
- `14.combine` moves 4/8 into memory displacements. The x address retains
  offset+base order; y/z have base+offset order. The table high/low pair remains,
  unlike the cached-row control's folded address.
- All three sums remain in `19.lreg`. In `20.greg`, z becomes a copy from `$6`
  to `$3`, while x remains `$2+$3 -> $5` and y remains `$3+$2 -> $6`.

The four final residuals are at +0x40, +0x48, +0x4C and +0x64: a second sum
remains, the object-z load is later, and the z pointer uses `$3` rather than
native `$2`. This is not merely a different assignment of identical copies.
These pass boundaries do not establish an internal allocator or scheduler cause.

For `moveChar`, the original flat table has equally ordered sums after combine,
one of which is a copy by `20.greg`. The vector view leaves oppositely ordered
sums and both remain. Its only changed words are +0x48 (commuted addition) and
+0x4C (addition instead of a copy). Calls, memory widths and later code are
unchanged. This is a cross-consumer regression, not matching progress.

Snapshotting the real object components gives the same complete function/data
contract as direct vector rows. Caching the vector-row pointer returns
`moveBlock` to 132 bytes. Separately replacing only the object's three position
floats with a position[3] member also remains 132 bytes and leaves even the
initial target RTL identical to the scalar-position control. The useful observed
distinction is the indexed displacement-table access, not that object-member
array. Existing initialization and `pbSetPrimMatrix`'s pos[0..2] accesses support
the position layout used by that rejected diagnostic.

All 15 complete plain/dump assembly and ELF pairs passed with stable inputs,
normalizing only one equal-length generated source path. Position-array preserves
70/65/70 peers; vector rows preserve 69/64/69 peers, with only the stated
`moveChar` regression. Owned data is unchanged. Normal and strict diagnostics
match the sibling control, including existing implicit-declaration and pointer
type warnings; this is not a strict-clean claim.

Keep the canonical table declaration and fallback. Further investigation should
explain the observed operand-order/copy boundary without regressing `moveChar`,
rather than repeat generic pointer, snapshot or register sweeps.
