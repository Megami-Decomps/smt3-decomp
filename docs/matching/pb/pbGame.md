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

12 of 34 words differ, 136 bytes against retail's 140.

The body and every register match. What differs is block placement at the
end: retail gives each clamp arm its own `swc1 $f0, 0xB8(block)` followed by
`b` to the shared `sh step` block, and places the `addiu $2, $0, -1` guard
exit between the clamp arms and that block. ee-gcc cross-jumps the two
identical stores into one block and puts the guard exit last, which is one
word shorter.

Ruled out: the clamp pair as `if`/`else if`, as two separate `if`s with their
own `work->step` store and `return 0`, as a nested `if` inside the `else`, and
through a `goto` to a shared tail; `block->rot` through a `pbBlockObj_t *`
local, through `work->block->rot` at every use, and with the sum held in an
`f32` local and stored explicitly before the tests; the guard as
`if (step >= 4) return -1;` ahead of the body (17 of 32) against the body
nested in `if (step < 4)` with a trailing `return -1` (12 of 34); and
`step` as `u16` rather than `u32`, which adds an `andi 0xffff`.

Confirmed on the way: `pbBlockObj_t` carries the rotation as an `f32` at
`0xB8`, the `.lit4` entries the function reads are `0.39269909f`,
`6.28318501f` and `-6.28318501f` in that order, and `work->step` has to be
read into a `u32` local for retail's `sltiu`.

## pbInitGame (pb/pbGame, 0x290)

152 of 164 words match, 12 differ, at the exact 656-byte size. The draft
restores the saved 20-by-20 map, validates it, initializes the exit and
character panels, refreshes motion, calls the selected character callback,
clears the record, and optionally starts the stage script.

The work prefix contains four registration bytes at +4, four pairs of signed
coordinates at +8, exit coordinates at +0x18/+0x1A, and the map at +0x1C.
Character retirement uses the original slot index. Creation uses a separate
compact output cursor, advanced only for registered characters. The constructor
receives parent, color index and motion zero; its USA provider ignores the last
two arguments. Record clearing follows both callback paths. Script startup
passes zero as its third argument. Work and character-slot loads after callbacks
remain observable reloads.

A qualified `cc1probe --alloc --no-dumps` run has identical hooked/unhooked
output. The loop index has 13 references over a live range of 92, priority 4239;
the output cursor has 10 references over 68, priority 4411. The cursor therefore
gets saved register 17 and the index 18, opposite retail. The map-copy dispatch,
other registers and branch layout match. See [compiler traces](../../cc1probe.md).

An indexed compact output adds four bytes. Early cursor initialization hoists
its address and grows the function. A shared loop-index/script-label scalar
fixes the loop registers but changes the final label register; it remains five
words short of exact. Explicit postincrement in the final draw call, a do-loop,
and carrying map-validation status in the index do not close the gap. The
preferred draft keeps the separate index, cursor and script-label values.

A current parity-qualified allocation observation also identifies the fixed
character-array base as pseudo 129 (three references, 64 live instructions,
three crossed calls), assigned GPR19. It is distinct from the advancing cursor
pseudo 87/GPR17 and ordinal pseudo 86/GPR18; changing the source to name that
fixed array base explicitly preserves the same 152 matching words. Reusing a
signed index for the ordinal and script label gives 158/164 matching words:
the loop bound becomes signed and the final label still occupies the wrong
register. These controls do not close the residual. The facility flag-clear
declaration now agrees with its actual signed-byte provider; both callers
here pass literal zero, so their emitted instructions stay unchanged.

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
