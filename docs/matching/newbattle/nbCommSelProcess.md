# Matching notes: newbattle/nbCommSelProcess

## DispComm1PanelComm (newbattle/nbCommSelProcess, 0xE0)

56 of 56 words match, at the native 224-byte size (2026-10-30).

The guard remains `if (no <= 0x8003) { if (no >= threshold) return; }`
over an `s32` local holding the `u16` parameter. Native zero-extends that
parameter and uses two signed comparisons, rejecting exactly 0x8000..0x8003
before the skill-table access. Other inputs retain the original lookup and
sprite-call behavior.

The ordinary literal lower bound was 53/56 words: `li 0x7FFF`, reversed
`slt`, and `bnez`, rather than retail's `li 0x8000`, `slt no, threshold`,
and `beqz`. Paired full-owner compiler output shows this complementary
endpoint already in initial RTL; this does not identify an instrumented
internal folding pass.

An initialized `s32 threshold = 0x8000` inside the existing outer branch,
with one nonvolatile empty `+r` constraint immediately before its genuine
comparison, preserves the native endpoint. This is a localized target-specific
compiler reconstruction aid, not evidence of original source spelling. There
is no dummy consumer, fixed register, call crossed, memory clobber, or ABI,
type, layout, access-width or call-order change. Plain and `-da` builds agree
on normalized assembly and allocated/relocation sections. The promoted owner
checks at 31 match / 0 differ.

Earlier ordinary forms ruled out: both nesting orders; complement `||` and
range `&&`; a forward `goto`; `else return`; `!(no < 0x8000)`;
`no <= 0x7FFF`; `no > 0x7FFF`; an explicit comparison-result local;
operand casts; and separate locals for the two tests.

## DispCommCursor (newbattle/nbCommSelProcess, 0x184)

29 of 97 words differ, at the exact 388-byte size and with the same instruction
sequence.

Two register copies retail keeps are what the structure hinges on, and both
are reproduced: the alpha `if` has to come first in the source, which makes
`ox` a global pseudo and emits `daddu $20, $4, $0` in the entry block, and
`ox` has to be reused for the left edge of both quads (`ox = xb + 0x7000;`
then `ox = x1;` between the calls), which is where the second
`daddu $20, $21, $0` comes from.

What is left is a three-way register rotation: retail has `ox` in `$20`, `x1`
in `$21` and `oy` in `$22` with `y0` sharing `oy`'s register, while the draft
has `oy` in `$17` with `y1` sharing it, `ox` in `$21` and `x1` in `$22`. Both
spend nine saved registers.

Ruled out: every spelling that makes `y0` reuse `oy` (`yb = oy << 3;
oy = yb + 0x7900; yb += 0x7998;` and three variants), all of which make gcse
hoist the whole y chain into the entry block ahead of
`dds3FontReqAllocDrawTag` and cost 27 more words; both source orders of the
two y values; a separate local for the depth `-1`; a named local for the
second quad's right edge; an `x` local copied from `ox`; computing `ox` after
the `if`; and all declaration orders of the locals, which change nothing.

## AnimeCommandList (newbattle/nbCommSelProcess, 0x18C)

99 of 99 words match, at the native 396-byte size (2026-10-30).

The integer animation arms (`0x20` help opacity, `1`/`2` alpha and `8`
X position) retain their original comparisons, clamps and float-to-int step.
The flag-4 arm uses a real initialized `f32 endpoint = 0.01f`, with one
volatile empty `+f` constraint immediately after initialization. Its only
consumers are the absolute-smritu comparison, minimum-step comparison and
minimum-step assignment. The multiply by 0.6f, subtraction, zero clamp and
state writes are unchanged; the endpoint crosses no call.

The ordinary baseline and nonvolatile `+f` control both retained the former
two-load residual at +0x12C/+0x130. Actual RTL showed that sched1 moved the
independent smritu load (UID265) before endpoint initialization (UID260) and
the constraint (UID262). The qualifier-only volatile control adds the actual
`insn_list 262` scheduling dependency on UID265 and keeps the native endpoint
load before smritu. Final output preserves this order. This is a localized
target-specific compiler reconstruction aid, not evidence of historical
source spelling or a claim that empty asm is a general memory barrier.

Plain and `-da` output agree on normalized assembly and allocated/relocation
sections. The endpoint and multiplier literal bits remain `0x3C23D70A` and
`0x3F199999`, matching retail. No dummy consumer, fixed register, memory
clobber, added arithmetic/conversion, or ABI/layout/access-width change is
introduced. The combined owner checks at 32 match / 0 differ.

Earlier ordinary forms ruled out: inverted comparisons, ordinary endpoint
locals, a separate absolute-smritu snapshot, swapped store order and external
globals substituted for literal-pool constants. The nonvolatile control's
negative result remains useful: maintaining the endpoint alone did not
constrain the independent load's schedule.

## DispCommandPanel (newbattle/nbCommSelProcess, 0x33C)

207 of 207 words match at the native 828-byte extent.

The five inputs are process, command type, X, Y and opacity. Preserve the
original coordinates for the background, cursor setup, help origin and final
label. Normalize the incoming X and Y for row rendering. Types 0, 1, 2, 4
and 5 use 21-pixel rows; type 3 uses 43-pixel rows and the converted slide
fraction. The no-op talk renderer receives the same six arguments as the
other row renderers.

Capture each entry before callbacks. Reload `drawcursor` after the selected
row's cursor callback before deciding whether to draw help. Read the command
count again for each row and `drawlisty` again for the loop bound; retain the
visible-row count returned before layout selection. Command types 4 and 5 advance Y
without an entry-renderer call. The help callback uses seven arguments,
including fresh depth and help opacity.

Two complementary source changes close the former four-word residual:

- In the normal-layout arm, `rowStep` temporarily holds the actual Y sum,
  supplies `oy`, then becomes the real 21-pixel stride. The phases are
  disjoint and have genuine consumers. This ordinary scalar reuse restores
  the native Y adjustment before the branch and stride store in its delay
  slot, without adding arithmetic, calls or memory observations.
- In the type-3 arm, a nonvolatile empty `+r` operand on initialized,
  adjusted `ox` preserves the native X-before-final-Y order. This is a
  localized compiler reconstruction aid on the real row coordinate, not
  evidence of original source spelling. It introduces no fixed register,
  memory clobber, dummy consumer or artificial lifetime extension. Removing
  only this operand leaves the two type-3 words different.

The previous normalized-coordinate and initializer controls remain closed.
A separate normalized-coordinate local caused broad allocation differences;
updating the incoming variables while retaining original-coordinate snapshots
recovered the native allocation. Declaring help-coordinate locals before the
original Y snapshot gives its native frame offset 0x10 rather than 8. Moving
help calculations earlier extends their lifetimes and changes allocation.
Separating the slide addition changes the float-result register; merely
moving the normal stride assignment did not resolve its scheduling pair.

Qualified unchanged-source compiler observations explain the old ordering,
not original source intent. Before reload, the type-3 X/final-Y comparison
has equal priorities and register weights 1/0, selecting Y first. After
reload, normal-layout Y and the stride spill are co-ready: the reached tiers
tie until per-pass linear instruction keys 65/63 select the spill first.
That key is not C-source order, and candidate selection is not a proof of
issue or delay-slot eligibility. The ordinary scalar-reuse result above is
verified from emitted code; no unobserved lowering cause is asserted.

The whole-function semantic review covers the initialized command-selection
process and its valid resources, not arbitrary fabricated state. Producers
bound command types, populated rows, scrolling and slide fractions; the
retail debug-skill table also fits the existing 288-entry row. Original
coordinates, callback-sensitive reloads, byte/halfword accesses and provider
ABIs remain unchanged. The outer layout guard does not make an invalid type
safe: earlier code already indexes the type-dependent state.

Production and retained target code resolve to all 828 retail bytes. Complete
owner/data checks, ordinary/dump output parity, finite attribution of the
SKIP-context table insertion, draft/IRX checks, regression tests and literal
retail equality qualify the promotion. Historical failed observer attempts
and rejected controls remain separate from the successful observations and
source qualification.

## SetCommCursor2 (newbattle/nbCommSelProcess, 0x21C)

135 of 135 words match at the native 540-byte extent.

The native command scan has separate successful call/return blocks for the
special-command cases and ordinary row types 0, 1 and 2. Writing these actual
arms directly, with a signed switch for the ordinary row type, recovers the
native block layout and five jump-table destinations. The shared matches
accumulator was 464 bytes with 52 matching words; direct calls with an if-chain
were 508 bytes with 49 matching words. The ordinary direct-switch form is
133/135 at 540 bytes, differing only in the initial halfword-load order.

One nonvolatile empty `+r` operand on the initialized `u16 previousIndex`
retains the native `lhu` observation before the signed `lh previousCommand`
observation. The index has real later equality comparisons and reset assignments;
no dummy consumer or fixed register is introduced. A deletion-only control
restores the two-word exchange; the volatile operand instead leaves six words
different. An ordinary promoted command local is inert.

Qualified pass snapshots show the ordinary load reversal first in sched1;
the candidate schedules the index load and its real operand before the command
load, which later fills the config-call delay slot. Complete plain/dump output
contracts agree. This is a localized target-specific compiler reconstruction
aid, not original-source evidence, a hooked comparator claim or a general memory
barrier. The exact emitted output verifies the particular native accesses.

Both previous-state values remain snapshots before config queries 2 then 1.
Each query's action-pointer/status observation stays after that callback.
Reset conditions, signed/unsigned widths, ascending first-match order, special
commands and literal successful row arguments are unchanged. Every successful
arm calls the existing SetPrevCursor exactly once and returns. The existing valid
work/action objects and row counts within 288 entries remain preconditions.

All 32 existing exact peers and owned data remain exact. Thirteen target layout
assertions preserve full output; all used fields fit within the requested
0x14D0-byte work region. The existing complete view is 0x14D4 and the allocator
rounds to 0x1500; neither fact establishes tail-field initialization. No layout or
allocation changed. Extracted-body host tests cover 15,728,640 single-entry
cases and 120,000 multirow/callback scenarios, not native execution. Strict
production/SKIP modes preserve four inherited missing declarations; retained
strict mode passes. Broader integration qualification is recorded separately.

## DispHelpPanel (newbattle/nbCommSelProcess, 0x1E8)

122 of 122 words match at the native 488-byte extent (2026-11-07).

Retail loads the live `drawtype` after the event-bit guard and retains an
unsigned `< 6` test at 0x0025AF30 whose result is overwritten by color setup.
An equal-arm conditional on this same live value performs the same real
coordinate, color and alpha calculation once on either path. This is the
canonical compiler-deletion-trace exception, not evidence of original branch
spelling or historical range assumptions. The later `type != 3` sprite test
continues to consume the snapshot.

Qualified complete-owner plain and `-da` output agrees in normalized assembly,
all allocated sections and explicit relocation contracts. In the actual
compiler dumps, UID51 loads drawtype, UID54 computes the unsigned comparison,
and UID55 branches to the alternate calculation. Both arms and UID55 remain
through `25.sched2`; `27.jump2` deletes the branch and duplicate arm while
retaining UID54 immediately after UID51. The final output reproduces the
native location, operands and ordering. Deleting only the equal-arm decision
returns to 480 bytes with 95 differing words in the native overlap. This
establishes the causal pass boundary, not an instrumented internal comparator
or source-history claim.

The second quad's `x2` calculation occurs in its actual phase after the first
quad call in C. Its non-escaping unsigned local inputs make this safe across
the call; the compiler schedules it earlier into the exact native position.
Keeping it before the first call in C leaves a three-word scheduling rotation.
No new constraint, dummy operation, call, type, layout or flag is introduced.
The event callback is before the drawtype snapshot; the texture load remains
at the sprite call; the tag is shared by both quad calls; the OT callback is
looked up after both. All original conversions and unsigned coordinate wrap
operations remain unchanged.

The complete owner checks 34 match / 0 differ. Baseline/candidate comparisons
preserve all 38 production peers, 33 SKIP peers and 38 retained peers, including
resolved code/data relocations. Production nontext bytes agree directly;
SKIP/retained jump-table addends agree after resolving the changed function
placements to owner-relative destinations. All seven target relocations keep
the same call targets and `sdfGrOtTable + 0x8E0` address. Accurate existing provider declarations are available in all modes: the
SelectCommandList and allocator/lock prototypes are outside their former
NON_MATCHING guards, and DispCommandPanel has its exact forward declaration.
All three candidate modes pass strict implicit-declaration checks. The memory
prototypes agree with the actual sdfMemory providers (handle-pointer allocation
result, void-pointer lock result), repairing the former implicit-int calls
without changing target output. An independent
extracted-actual-body callback oracle passes 200,000 cases at O2 and under
ASan/UBSan; this is a host semantic test, not native execution. Full integrated
retail and regression gates remain required for publication.
