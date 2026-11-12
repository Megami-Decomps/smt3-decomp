# Field test and chase matching

## fldRunDantePatr (field/fldTest, 0x2D0)

Matched in `b62c6009`: all 180 of 180 words match in the 720-byte production
body, and the assembly fallback is removed. The existing patrol body and
callback reloads are unchanged except for the short diagnostic's native symbol
reference.

`D_0054EAD0[]` retains the existing seven-byte `"Re1KO\n"` string and its
one-byte pad through `INCLUDE_SDATA`, preserving the complete 140-byte,
four-aligned owner. Emitting that literal instead raises alignment to eight
and fails retail despite a clean unit report. A truthful `[7]` extern keeps
data exact but emits a nonmatching 724-byte body; no enlarged bound is used.
The ten-byte `"ReCalcOK\n"` literal remains C at `0x00531308` in its 16-byte slot.

The complete owner checks 38 match, 0 differ; the entire retail image matches
SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`. Stock plain/`-da` output parity
also passes. `fldRunEffSet` and the remaining guarded drafts are preserved.

`18809150` later bound the initialized `f32` speed to `$f1` and used an empty
`+f` asm constraint before the real division. Rechecking `87be30f1` found that
form and the ordinary `f32 frames = distance / fldRun_Dante.speed;` form have
identical complete owner code and data, with the same 38/0, retail checksum and
stock plain/`-da` parity results. The ordinary form needs no constraint in that
context. [736066f5](https://github.com/Megami-Decomps/smt3-decomp/commit/736066f58556da27ad7a094adc892051d6a5f42c)
restores that simpler ordinary form after another complete 38/0 owner check and
retail checksum pass on `51d66ae0`, preserving the newer SDK declarations.
The hint emits no instructions and its live value crosses no call; it is a
target-specific reconstruction aid, not a general memory barrier or evidence
of the original source spelling. See the
[SMT3-local policy](../../CONTRIBUTING.md#localized-compiler-constraints-smt3-only).

### Historical residual and controls

The earlier `376abcae` draft matched 178 of 180 words at the native 720-byte
size, with assembly active in that context.
The two differences at +0x1C0 and +0x1C4 exchange the speed load and the
stack distance load used to compute the route timer.

The routine converts Dante and player positions to map coordinates, finds a
16-byte cell record, and uses its route offset/count at +8/+0xC. The found
cell pointer is stored at Dante +0x28, separately from the route data pointer
at +0x2C. A route with two or more points probes the next point against enemy
collision; a blocked probe returns to the first point. Successful shortcuts
advance the waypoint and set the shortcut flag. Patrol movement uses
turn-toward parameters 15 and 1 (divisor 15, minimum step 1), updates position,
and enables the patrol BGM state.

A qualified `cc1probe.sh --sched --no-dumps` trace of the isolated C body in
this unit has equal plain, hooked and unhooked outputs. In sched1, distance
load UID546 and speed load UID548 both have priority 14, register weight 1
and cost 2; their original order breaks the tie (LUID105 before106). In
sched2 the distance load has three dependents versus two for speed, retaining
that order. The native order is speed then distance. These observations
identify the residual without establishing the original source spelling.

The blocked-probe branch precedes the shortcut branch, retaining the native
branch and common completion rather than a shorter annulled branch. Advancing
the waypoint before setting the shortcut flag preserves the native store
ordering. A floating frame value followed by the integer timer assignment
keeps the native FPU registers; an explicit speed snapshot adds register
allocation differences. A one-pass timer block does not improve the two-word
residual and is not retained. Those ordinary-C trials used no artificial
dependency or compiler flag. The map-cell fields and target-coordinate/pointer
view are corroborated by `fldFindMapCell` and `fldRunAiCng`.

## fldRunEffTask (field/fldTest, 0x220)

A private native-sized control represents the actual two-table traversal
with an independent typed cursor and remaining-table countdown. It retains
21 of 136 words matching, the same as the target-active baseline. One
localized constraint on the initialized, genuinely consumed countdown reaches
22 words. Both remain 544 bytes and both reproduce the known two-load
fldRunDantePatr peer regression. Production output independently confirms
that regression: a target score alone cannot qualify this owner.

Seven declaration corrections to the actual effect/part providers were
isolated in a separate guarded control. Complete assembly and ELF objects
are unchanged in production, SKIP_ASM and retained contexts, after the sole
equal-length source-filename normalization; the guarded owner remains 38/0.
These private corrections are not a new allocation premise or a publication
of the nonmatching loop. Canonical source is unchanged.

The private compiler harness was checked at the canonical relative source
and output filename lengths. Changing those lengths can change this old
compiler's output, so an absolute-path diagnostic is not production proof.
Further work needs evidence beyond the closed cursor/countdown controls and
the unrelated already-studied TamaFree scheduling family.

### Tama sound quotient reconstruction

The guarded `fldRunTamaSe` draft now gives each sound block an ordinary signed
32-bit `volume` local for the product divided by 128, before passing
`volume / 2` to the unchanged sound call. Native code performs those two signed
round-toward-zero divisions separately. The previous single expression let
this compiler combine them into division by 256. The real quotient local
restores the native 968-byte size from 920 bytes without constraints,
volatility, extra memory accesses or a changed numeric operation.

The target-active reconstruction improves from 63 matching instruction words
(out of 230 emitted) to 162 matching instruction words out of 242 native words. It is still nonmatching:
aggregate-copy scheduling and ordering, six quotient-allocation/order words
per sound block, and target-active rodata ownership remain unresolved. All
38 existing matched text functions remain exact in the qualified quotient
context. This is not an exact owner or matching-credit promotion.

The actual frame-window scans, strict comparisons, pose handoff, distance
call, signed volume-table loads and first-match breaks are unchanged. Actual
body callback fixtures pass 300,000 cases each at O0, O2 and UBSan, using the
provider's bounded distance range and all three frame tables. Plain/dumped
raw compiler assembly and objects agree in both production and SKIP contexts;
the guarded-only source edit leaves both baseline production outputs byte
identical. Assembly fallback remains active.

The qualified first RTL dump already contains shift-by-eight lowering for the
single expression, versus shift-by-seven and then shift-by-one for the scoped
quotient. Thus the key distinction precedes instruction scheduling; the
remaining allocation/order differences are separate.
