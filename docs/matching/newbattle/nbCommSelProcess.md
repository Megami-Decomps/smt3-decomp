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

## DispCommandPanel (0x33C)

203 of 207 instruction words match at the native 828-byte size when the
existing `DispComm1PanelComm` C draft is visible. The caller remains
assembly-backed; the row callee is now matched as documented above.

The five inputs are process, command type, X, Y and opacity. Preserve the
original coordinates for the background, cursor setup, help origin and final
label. Normalize the incoming X and Y for row rendering. Types 0, 1, 2, 4
and 5 use 21-pixel rows; type 3 uses 43-pixel rows and the converted slide
fraction. The no-op talk renderer receives the same six arguments as the
other row renderers.

Capture each entry before callbacks. Reload `drawcursor` after the selected
row's cursor callback before deciding whether to draw help. Read the command
count again for each row and `drawlisty` again for the loop bound; retain the
visible-row count returned before layout selection. Rows 4 and 5 advance Y
without an entry-renderer call. The help callback uses seven arguments,
including fresh depth and help opacity.

With a separate normalized-coordinate local, the reconstruction has broad
entry/register differences. Updating the incoming coordinate variables while
retaining original-coordinate snapshots recovers the native allocation.
Declare the help-coordinate locals before the original Y snapshot: its reload
home is then the native frame offset 0x10 rather than 8. Moving the help
coordinate calculations earlier instead extends their lifetimes and changes
the allocation.

The remaining differences are two scheduling exchanges. Offsets +0x110 and
+0x118 exchange the row-step store and normal-layout Y adjustment. Offsets
+0x12C and +0x130 exchange the type-3 X and final Y adjustments. Computing the
slide addition separately changes the float-result register and does not
close the match; moving the normal row-step assignment leaves its pair
unchanged.

A truthful command-row forward declaration alone leaves this caller at
836 bytes with 132 matching instruction words and 75 differing words in the
native overlap. Making the existing genuine command-row C body visible
recovers the native size and leaves the four words above. The row callee's former
three-word lower-bound residual is now resolved independently; this does not
resolve the caller's scheduling differences. Reproduce the caller measurement with
`tools/check_unit.py src/smt3/newbattle/nbCommSelProcess.c
--cflags=-DNON_MATCHING --func DispCommandPanel -v`.
