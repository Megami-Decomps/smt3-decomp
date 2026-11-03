# Matching notes: newbattle/nbCalc

## nbGetVirtualAisyo (newbattle/nbCalc, 0xAC)

43 of 43 words match, at the exact 172-byte native size. Matched in
`f55fd495` with ordinary C and no compiler constraints.

A `switch` groups attributes `-1`, `12`, `13` and `14` into a `break` to
the final zero return. The default calls
`nbGetAisyo(nskill, dformindex, skillattr)` and returns the first present
flag in priority order `0x20000`, `0x40000`, `0x10000`; otherwise it also
returns zero. The source formation argument is unused in retail.

The earlier note was filed under `nbMakePacket`, but the owner is `nbCalc`.
Its 39-of-43 switch residual is historical, not the current compiler result.
In the current owner/provider context, the switch with default-arm early
returns reproduces the native dispatch, its delay slots and the prioritized
flag tests exactly. The prior range-chain draft emitted 140 bytes; assigning
a shared result in the switch emitted 172 bytes but differed in 22 words.
No new scheduling or allocation constraint is needed.

The complete owner and owned-data check passes with 57 match, 0 differ;
all eight remaining owner drafts compile. Regression tests and the EE/IRX
draft sweeps pass. The source-owned production span is independently verified,
and the full image retains retail SHA-1
`5facfbf7916836613449c922e26a4ec412207b32` with a no-op repeat build.

## nbCheckBackAttack (newbattle/nbCalc, 0x178)

94 of 94 words match at the native 376-byte size. The complete owner has
58 exact C functions with no code or owned-data difference.

The encounter table's signed byte is observed once and promoted to an `s32`
comparison value. A single localized binding keeps that value in `$4` for
the `-1` and `1` early-exit tests. It is dead before `nbGetLvAvg2`; neither
a fixed-register value nor an artificial dependency crosses a call. This
is a target-specific compiler reconstruction aid, not original source evidence.
The ordinary signed `s32` local produces 372 bytes and 30 of 93 matching words:
it leaves the incoming data argument available in `$4`, removing the native
argument restoration and changing the subsequent allocation and schedule.
Ordinary `register s32` and `register s8` controls give the same 372-byte,
30-of-93 result, so the fixed binding is not replaced by storage-class spelling.

The initial rate of 12 is assigned after acquiring the player work pointer,
before its level observation and parameter calls. Initializing it at entry
with the choice binding retained gives 376 bytes but only 55 of 94 matching
words. Both changes therefore have independent ablation evidence. Ordinary
level-to-score reuse and explicit score accumulation phases were weaker and
are not retained.

The two level/parameter scores remain diagnostic arguments, not inputs to
the encounter rate. Provider call order, signed division by two, the player
level snapshot before parameter callbacks, the random roll, the special-skill
query and the optional table divisor are preserved. The final result remains
`roll < rate`. No provider type, data layout or extra memory observation changes.

Plain and `-da` full-owner builds have identical allocated and relocation
sections (ten sections) and normalized assembly. The production compiler flags
are unchanged.

## nbGetKoukaType (newbattle/nbCalc, 0x774)

477 of 477 words match at the full 1908-byte native size with ordinary C.
The retail function begins at 0x00265948.
The complete owner and owned-data check is 59 match, 0 differ on the
`11349504` owner, preserving its independently contributed
`nbCheckBackAttack` match. This function adds no compiler constraint, register
binding, barrier, volatile qualifier, fake field or helper wrapper.

### Native state and calculations

The skill table is ordinary initialized storage. `nbSetNormalSkill0` writes
its actual target-type/count fields; nothing supports the earlier draft's
volatile table view. Repeated indexed field expressions preserve the native
post-call loads without taking a stale skill snapshot. The normal records
remain 0x3C bytes, with byte hit/effect selectors, unsigned-halfword HP type,
signed-halfword HP amount/failpoint and a word program selector. No shared
type or provider declaration was changed.

The existing work flag is a word at +0. `nbAddUnitStock` sets its enemy-side
bit 0x20 with the native OR 0x22/store at `0x24D640`/`0x24D648`.
`datCheckSyojiSkill` consumes that bit to select the native devil-format
skills, and the adjacent magic accuracy calculation uses it to choose
95 versus 100. The physical and magic provider call order, post-call level
loads, float multiplication order, clamp, status quarter factor, diagnostic
and random roll all reproduce native. The one target write remains the
word `d31_kantuu` at +0x14C under the original affinity conditions.

`datGetParam` supplies 1..40 and levels remain full-range u16. Physical
quotient denominators are 3..13110 and scaled quotients are at most 1300.
The actual magic-hit provider is at least 20 and at most 65652, while the
evasion provider is below 65654, so its times100 numerator stays safely in
s32. Signed16 failpoint subtraction is safe too. The status-counter provider returns s16; the moon provider folds the full
u8 moon phase to 0..8. Adjusted counts are therefore -32770..32769 and the
source/target factors have magnitude below 8194. These bounds keep the float
calculation finite before its 50..100 clamp and optional quarter factor; no
usual gameplay +/-4 counter limit is assumed. No narrower game-level assumption or new invalid
input handling is used. Ordinary casts emit the native CVT.W.S word under
the pinned EE compiler; this does not promise different host FPU behavior.

The two exclusive diagnostic formats are ordinary string literals, as is
the eight-byte `MISS!!\n` string in this owner's `.sdata`. Float literals
`0.01f` and `0.7f` reproduce the two native `.lit4` words. Their contents,
placements and relocation contracts are included in the full-owner check.

### Qualified physical dead-branch reconstruction

At native `0x265DF8`/`0x265DFC`, offsets +0x4B0/+0x4B4, retail loads
`source->flag` into v0 and masks 0x20. Nothing consumes that result before
`datGetParam(source,4)` overwrites v0. The provider reads only its actual
a0/a1 parameters, so this is not an omitted calculation or hidden argument.
The prior discarded volatile read could not explain the mask and is gone.

The physical case keeps the same source-side branch shape as the live magic
case beside it, with the same real physical calculation in each arm. Exactly
one sequence executes. This is the canonical dead-code/copied-case-shape
exception, corroborated by the adjacent live source-side split. The matched
`datGetNormalAtkPow`/`datGetMagicAtkPow` family also contains ordinary equal
side branches and native residual masks. It is a mechanistic reconstruction,
not a claim about original branch spelling or a recovered copied macro.

Qualified whole-owner snapshots of the real ee-gcc2.96 show both physical
calculation arms and their conditional branch through `25.sched2`. In
`27.jump2` the branch and duplicate arm are gone, while the source load/mask
survive. `28.mach` and `29.dbr` retain the exact native residual. The complete
native size, position, operands and ordering match, rather than merely a
similar dead instruction or better percentage. The no-physical-split control
is independently checked against the same owner.

These are actual pass snapshots captured with `-da -fsched-verbose=5`, not a
hooked internal-decision trace. Plain and dump builds are qualified by
allocated-section bytes/metadata, non-file symbols, ordered relocation
contracts and normalized assembly. The source decision and deletion pass
interval are observed; no unobserved compiler hook or historical source
claim is implied.

### Ordinary control-flow and source controls

The original retained draft emitted 1580 of 1908 bytes in the historical
`80c41cbf` owner. The reconstruction uses switches matching the native
program/attribute/status cases; direct indexed table fields; real signed
and float calculations; and complete per-case print, RNG and return tails.
The compiler merges those terminal tails to the native shared code. A shared
format-pointer tail was six words off; directly passing each format to a
separate print followed by a shared RNG tail was not exact either. No
register/initializer grid, instruction replay or compiler-flag sweep was
used to bridge those differences.

Independent review of the final source and native/provider semantics passed.
The complete owner/data check is 59/0; plain/dump parity covers six allocated
sections, 587 non-file symbols and 698 ordered relocations, plus normalized
assembly. At this function's landing, strict NON_MATCHING/SKIP_ASM
compilation passed, while normal production and SKIP_ASM retained eight
inherited implicit declarations.

A subsequent declaration-only cleanup removes five NON_MATCHING guard pairs
around those eight existing, provider-compatible prototypes. Their s32/int
returns and default-promoted int/pointer arguments were already compatible
with the GNU89 implicit contracts; no runtime or ABI defect is demonstrated.
Strict implicit-declaration compilation now passes in production, SKIP_ASM,
and NON_MATCHING/SKIP_ASM. Ordinary baseline/candidate and candidate
ordinary/strict comparisons preserve allocated-section bytes and metadata,
ordered non-file symbols, ordered relocations and normalized assembly in
all three modes. The owner remains 59/0, including nbGetKoukaType and nbCheckBackAttack, with unchanged
matching coverage. This is not a repository-wide warning-clean claim. Full
retail rebuild/checksum, repository regressions and guard sweeps remain the
integration gates, not replaced by these focused results.
