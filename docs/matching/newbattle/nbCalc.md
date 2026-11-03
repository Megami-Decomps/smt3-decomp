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

## nbGetHitType (newbattle/nbCalc, 0x3AC)

235 of 235 words match at the full 940-byte native body, excluding the
alignment nop after endlabel. The function begins at 0x002654B8. The complete
owner and owned data pass with 60 match, 0 differ on clean 46b4591c, preserving
the independently matched 1908-byte nbGetKoukaType and 376-byte nbCheckBackAttack.
No header, provider contract, compiler option, other body or unit boundary is
changed. This reconstruction adds no volatile, barrier, register binding,
compiler constraint, dummy consumer or assembly.

### Real state, observations and numeric domain

The initial attribute switch excludes -1 and 12..15. Skill-zero auto-critical
checks 0x12C and 0x12D occur before the negative-affinity and party exits, with
the original autoskill write only on their successful paths. Status factors,
formation stat/event observation, optional 0x12B factor, effect dispatch,
diagnostic, signed random comparison and final conditional write preserve
native ordering. The source and target pointers may alias; both parameter
calls remain, each followed by that work object's level observation.

The table is ordinary mutable initialized storage, not volatile or globally
const. A real pointer to special[11] names the activated skill record and
produces the native halfword relocation at datSkillTable+0x4860. The native
normal record is 0x3C bytes with signed-halfword criticalpoint/failpoint.
Across all 288 initialized records, criticalpoint is 0..50 and failpoint is
0..45; special[11].n is 300. Native/source writer inspection identifies
nbSetNormalSkill0 changing only targettype, target count and visual/attribute
fields, not these numeric fields. These are verified retail-data/provider
invariants, not a fabricated signed-halfword bound.

The actual parameter provider clamps to 1..40, and levels retain the full u16
domain. Integer denominators are therefore 3..13110, quotients 0..13 and
scaled source/target rates 0..1300. The base 100-failpoint is 55..100, and the
lower-clamped hit rate is 50..181.25. The status/event factor is 1, 8 or 100;
only normal skill zero can multiply by three. Even the loose independent
bound (181.25/55)*50*300 is below 49,433, far inside s32. This proves safe
conversions for valid initialized records with the audited providers/writers,
without assuming ordinary gameplay levels or counters. Arbitrary modified
s16 tables are not covered: failpoint=100 permits a zero denominator and
other full-range replacements can overflow the conversion. Invalid indices,
invalid pointers or memory corruption are not given new handling.

All float operations remain in native order: source minus target, times
0.0625f, base addition, lower clamp, division by base, multiplication by the
critical field and then by the factor. The final f32 criticalChance has a
real consumer in the shared s32 point conversion. The two ordinary casts
emit the native CVT.W.S operations under the pinned EE compiler; no claim is
made about different host FPU behavior.

Native criticalpoint is loaded at 0x2657AC from entry+0x30+4 for the formula,
then independently at 0x2657E8 from entry+0x34 for printf. Keeping the physical
floating calculation in its side arm and the conversion/diagnostic in the
shared phase preserves both observations with ordinary indexed fields.
The owned diagnostic is an actual string literal and passes the complete
owner's code/data/relocation checks.

### Qualified copied-case reconstruction

At +0x200/+0x204, retail reads source->flag and masks 0x20 into v0; no consumer
uses the result before datGetParam overwrites v0. The provider consumes only
its real a0/a1 arguments. The copied side-case shape is corroborated by the
adjacent matched nbGetKoukaType physical case and its live magic side split.
This uses the canonical sibling/copied-case exception, not the alternative
rule for a predicate already consumed by live computation in this function.
Exactly one of the equal real physical calculation sequences executes.

Qualified whole-owner ee-gcc 2.96 snapshots retain flag load UID366, mask
UID368, branch UID369 and four datGetParam call sites through 25.sched2. In
27.jump2, the branch and duplicate calculation arm are removed; two provider
call sites remain and the load/mask survive with the native operands and
ordering. 28.mach and 29.dbr retain them. This brackets the observed deletion
between those passes; no internal breakpoint or unobserved hook decision is
claimed. The source comment identifies the native residual, corroborating
sibling and mechanistic rather than historical nature of the reconstruction.

The no-side-split control emits 896 bytes, omits the native residual and has
16 matching words in its 224-word emitted prefix. It preserves the other
59 C functions but is not a clean target/owned-data result. Moving the entire
point calculation outside the copied branches emits 932 bytes and loses the
required observation shape; it is not retained.

### Shared conversion and diagnostic scheduling

A qualified control converting criticalChance to s32 inside each arm is
940 bytes and 229/235 words: only the six diagnostic preparation words at
+0x31C..+0x330 differ. Diagnostic pointer, value and format locals do not fix
that schedule. Separating the real floating result from the common point
conversion closes the six words without extra operations or constraints.
In the exact 25.sched2, common block 34 contains point conversion UID719,
index subtraction UID765, diagnostic conversion UID784, table-low UID759,
index shift UID767, format-high UID756, index add UID771, format-low UID780
and criticalpoint load UID782 in native order. The prior control instead
keeps the point conversions in the arms. This is measured block/dependency
and scheduling evidence from actual pass snapshots, not a claim to know the
original variable names or source spelling.

Plain/-da -fsched-verbose=5 builds of the final full owner agree in six
allocated sections and their metadata, 585 non-file symbols, 698 ordered
relocations and normalized assembly. The same categories pass for the
no-side-split and prior conversion-placement controls. Production, SKIP_ASM
and NON_MATCHING/SKIP_ASM strict implicit-declaration compiles pass for the
final source. Independent review, full retail checksum/build, regressions
and repository-wide guard sweeps remain integration gates, not replaced by
these focused checks.

## nbGetKoukaBadKaifuku (newbattle/nbCalc, 0x1EC)

98 of 123 words match at the full 492-byte native size. The function remains
`NON_MATCHING` with its assembly fallback; this work adds no matching credit.
Native code runs from `0x267180` through `0x267368`, excluding the alignment
word at `0x26736C`. This is distinct from `nbCheckBadKaifuku`.

### Ordinary reconstruction and native behavior

The draft uses indexed, ordinary normal-skill fields instead of the old
unsupported volatile table view. The record stride remains 0x3C; badtype and
badlevel are bytes at +0x24/+0x25, and basstatus is a halfword at +0x26.
Both work-pointer calls precede the status and level reads. No type, provider
contract, table ownership or translation-unit boundary changes.

The real parameter sums use source parameter 2 plus parameter 5 divided by 4, then the
same target calculation. `datGetParam` and `datGetBaseParam` establish 1..40
per parameter, so each sum is 1..50 and the target divisor is nonzero for
valid work pointers. Signed quarter arithmetic and the native float
multiply/divide dependencies remain intact. The cast emits native CVT.W.S;
no new clamp, guessed input bound or portable-host conversion guarantee is
introduced. Retail's initialized table has all 288 normal-skill minus fields
set to 100, and all 13 badtype-2 records have badlevel 255, bypassing this
arithmetic. The identified writer, nbSetNormalSkill0, changes target fields
rather than minus, badtype or badlevel. This is evidence for the initialized
data and identified writer, not formal closure over every possible alias or
writer.

For modified inputs, float-to-s32 conversion safety remains unresolved.
SetAction_SKILL starts power at 1 and repeatedly multiplies it by
nbGetGensyoRitu; that provider reads an unsigned-halfword minus field and
divides by 100.0f. The declared field domain alone permits factors up to
655.35, so repeated multiplication does not prove a bounded, finite chance.
The affinity provider alone also cannot establish the complete input range.
Nonfinite or out-of-range chance would retain the old draft's language-level
conversion risk; emitting native CVT.W.S does not prove that cast safe for
arbitrary inputs. No reachable failure is established for the observed retail
data. The draft stays guarded and unpromoted, without a clamp or a claim of
complete arbitrary-input semantic qualification.

Badlevel 0xFF or nonzero nvirtual bypasses the parameter calls, diagnostic,
RNG and conversion. Otherwise the native source 2/source 5/target 2/target 5,
float promotion, printf and RNG ordering is preserved. The target status
halfword is reread after those calls, including when source and target alias.
The native virtual/actual tails both return the complete skill status mask
on overlap, not the intersection. These two live tails are visible at
`0x267300..0x267334`; no dead calculation or artificial barrier was added.

### Qualified compiler blocker

With this ordinary draft, the second badlevel load survives through the real
compiler's `19.lreg` snapshot as insn 144, but becomes a copy from a0 to v0
in `20.greg`. Retail instead reloads the byte at `0x26721C`. There is no
intervening call or write requiring the reload; the ordinary table has no
supported volatile contract. This local-register to global/reload-stage
interval is the measured blocker, not an early-CSE explanation or evidence
for restoring false volatility. No internal compiler hook was observed.

The real 100.0f constant is still positioned before the first source provider
call in `19.lreg` and `20.greg`; `25.sched2` places it after the second source
call, unlike retail. Remaining differences include target-rate allocation
and related scheduling. These are actual `-da -fsched-verbose=5` pass
snapshots, not a hooked decision trace or a recovered original spelling.

Plain versus dump builds agree on normalized assembly, all six allocated
sections and their metadata, 587 non-file symbols and 698 ordered relocation
contracts. The activated draft's complete owner check is 59 match, 1 differ,
with no owned-data difference. Its diagnostic literal is placed after the
preceding D_00544BA8 data; leaving it as an external reference while removing
the target assembly would omit the native string. The guarded production
view remains 59 match, 0 differ and strict NON_MATCHING/SKIP_ASM compilation
with implicit-declaration errors enabled passes.

### Controls already measured

In the same owner, activating the old guarded draft emitted 460 bytes and
matched 9 of its 115 emitted words against the native prefix; it did not cover
all 123 native words. That historical baseline is not an alternative native
size or matching denominator.

Nonvolatile pointer and indexed-field controls, native split tails, later
success initialization, staged percentage division, direct format literal,
separate/compound target-rate expressions, source and target inline parameter
expressions, mixed pointer/indexed access, and fully inline chance/percentage
forms were checked. A shared scale scalar reached 99/123 but did not resolve
the reload/scheduling dependency and was not retained. The old volatile
shape was measured only as a diagnostic, never proposed for publication.
No register/initializer grid, fabricated bound, inline assembly, fake barrier,
new dead computation or compiler-flag change was used. The next useful work
is an evidence-backed ordinary live source shape for the identified
reload/scheduler dependency, not repeating these controls blindly.

Integrated qualification after nbGetHitType promotion: the production owner
now has 60 matching C functions and no differences. Activating only the
BadKaifuku draft still gives 98/123 words at 492 bytes, with all 60 peers
and owned data preserved. The standalone 59-peer receipts above retain
their original compilation context. Final combined production/plain/dump
parity covers six allocated sections, 585 non-file symbols and 698 ordered
relocations; plain/dump normalized assembly also agrees. Full retail
checksum/byte comparison, regressions and fresh guard checks pass.

## nbGetKoukaBadDamage (newbattle/nbCalc, 0x41C)

263 of 263 words match at the full 1052-byte native size. Complete owner and
owned data: 61 matches, 0 differences. The preceding HitType and guarded
BadKaifuku changes are preserved and excluded from this target's credit.

### Native observations and ordinary reconstruction

Ordinary indexed fields replace the unsupported volatile skill view. The
normal record remains 60 bytes, with u8 badtype/badlevel and u16 status.
Main-data lookup still precedes the wrong-type return; source/target work
lookups precede status and the second badtype observation. Status remains a
snapshot across affinity, event, diagnostic and RNG calls. Target flags are
loaded after the event query; source/target aliasing is not excluded.

The status-selection RNG and final success RNG use separate ordinary locals.
The final signed comparison follows the diagnostic in source, reproducing
native saved-value lifetimes and its scheduled call delay slot. Diagnostic
strings and 0.7/1.3 float literals are actual source-owned data. No provider
contract, field width, compiler flag or translation-unit boundary changes.

The type 3 candidate list has 14 signed-word cells, one per tested bit 0..13.
The old guard's count-zero early return was not native: retail calls the RNG
which updates its state and then executes DIVU/BREAK 7 for a zero limit.
The reconstruction restores that call, not a new safe result or fabricated
positive-count bound. All 12 initialized type 3 records have 1, 3 or 5 candidates;
skill 272 follows its separate six-way random-selection path. This observed
initialization is not a proof that no other code can modify the table.

### Copied branch and localized live level

Native 0x267008/0x26700C loads source->flag and masks 0x20, with no live consumer.
The matched nbGetKoukaType/nbGetHitType copied-case shapes and the adjacent
live source-side accuracy split corroborate retaining the same real
percentage calculation in two exclusive source-side branches. This uses the
copied-case exception, not an otherwise-live-predicate claim for this target.
Exactly one conversion/divide/multiply sequence executes. Qualified compiler
snapshots show the branch and both arms through 25.sched2; 27.jump2 deletes
the branch/duplicate arm while retaining the native load/mask. This is a
mechanistic reconstruction, not original spelling or a recovered macro.

The ordinary level snapshot reaches 261/263 words. In 19.lreg it still reloads
the unsigned byte; 20.greg substitutes a copy of the earlier loaded value
from v1 to a0. 29.dbr places that redundant copy in the virtual-check delay slot
and converts a0, whereas native leaves the slot empty and converts v1.
A single localized binding keeps the real initialized s32 level in v1 only
until its float conversion. It is initialized inside the nonvirtual,
non-255 gate, consumed on each exclusive arm, and dead before evtBitCheck.
No call clobbers a live bound value. There is no empty asm, memory barrier,
artificial lifetime, fake volatile qualifier or register search grid.
Deleting only the binding reproduces the two-word ordinary residual.

Final ordinary/probed full-owner builds agree on all 6 allocated sections
and metadata, 586 non-file symbols, 698 ordered relocation contracts and
normalized assembly. These are real unhooked compiler-pass snapshots; no
unobserved internal routine is asserted. The single-conversion control
reaches 260/263, with only native-mask scheduling different. Pointer/indexed
and scalar-lifetime controls remain retained as negative evidence.

### Numeric scope

The native arithmetic is level/100, times affinity, times100, followed by
the event/status-dependent 0.5,0.7,1.3 or1.5 product. Non-0x800 status uses the
masked low 16 affinity divided by 100, so chance and printed casts are safely
bounded over those typed values. Status 0x800 uses caller ritu. The live
caller multiplies affinity by a running power, which starts at 1 and is
updated after each target by the table's u16 minus/100. All 288 initialized
minus fields equal 100; the known normal-skill writer leaves these fields
unchanged. This supports initialized retail bounds but does not establish
global alias/writer closure. Arbitrary nonfinite/out-of-range incoming ritu
or modified minus values retain the inherited conversion-domain limitation;
exact pinned-target code is not a promise of portable total float-cast
semantics. No clamp or fabricated gameplay bound was introduced.
