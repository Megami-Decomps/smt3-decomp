# Non-matching drafts: newbattle/nbCommSelProcess

Conventions: [nonmatching drafts](../../nonmatching.md).

## DispComm1PanelComm (newbattle/nbCommSelProcess, 0xE0)

53 of 56 words match, at the exact 224-byte size.

The guard is `if (no <= 0x8003) { if (no >= 0x8000) return; }` over an `s32`
local holding the `u16` parameter, which is what gives retail's two separate
signed `slt`s rather than the subtract plus one `sltiu` that an `&&` or `||`
range chain folds to, and what stops `shorten_compare` turning the compares
unsigned.

The three remaining words are the lower bound's encoding. Retail materialises
0x8000 and tests `slt no, reg` with a `beqz`, the inverted-branch form of
`LT`. ee-gcc 2.96's `fold` rewrites every spelling of `< 0x8000` and
`>= 0x8000` into `> 0x7FFF` before the MIPS backend sees it, so the compare
comes out as `li 0x7FFF`, `slt reg, no`, `bnez`: same test, three words off.

Ruled out: both nesting orders; `||` of the complement and `&&` of the range,
which fold; a `goto` into the `if` body so the draw block is the fallthrough;
an `else return`; `!(no < 0x8000)`; `no <= 0x7FFF`; `no > 0x7FFF` with an
early return; an explicit `s32` local holding the comparison result, which
adds an `xori`; `(s32)` casts on either operand; and two distinct `s32` locals
for the two tests, which does defeat the range fold but leaves the same
encoding.

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

97 of 99 words match, at the exact 396-byte size and with the same instruction
multiset.

The four flag arms (`0x20` on `helpa`, `1` and `2` on `alpha`, `8` on `ox`) are
exact, including the `movn`/`movz` clamps and the `(s32)(ox * 0.5f)` step with
its `d == 0 ? -1 : d` fixup.

The two remaining words are the order of the two loads that open the `0x4` arm.
Retail loads the `0.01f` literal first and `smritu` second; every source form
tried loads `smritu` first, because gcc expands the compare's left operand
first and then rematerialises the constant at the compare.

Ruled out: `0.01f > fabsf(...)` and `!(fabsf(...) >= lim)`, both of which
`fold` rewrites back; a block-local and a function-local `f32 lim = 0.01f`,
which gcc rematerialises (the function-local form also hoists the load into the
entry block and costs 95 words); `f32 a = fabsf(s->smritu)` as a separate
statement; an `f32 ritu` local for the field; both source orders of the
then-arm's two stores; and `extern f32 D_0054E500`/`D_0054E504` globals in
place of the literals, which are MEMs rather than constants and get hoisted
into the `beqz`'s delay slot, where retail has a `nop` (19 words).

## DispCommandPanel (0x33C)

203 of 207 instruction words match at the native 828-byte size when the
existing `DispComm1PanelComm` C draft is visible. Both functions remain
assembly-backed in the normal build; there is no exact-match credit.

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
recovers the native size and leaves the four words above. That callee still
has its documented three-word lower-bound residual, so it cannot yet replace
its fallback. Reproduce the caller measurement with
`tools/check_unit.py src/smt3/newbattle/nbCommSelProcess.c
--cflags=-DNON_MATCHING --func DispCommandPanel -v`.

## SetCommList (newbattle/nbCommSelProcess, 0x668)

382 of 410 instruction words match at the native 1,640-byte size, in both
the isolated and complete guarded compilation. Production remains
assembly-backed. The 28 differing words are confined to +0x40C through
+0x47C: the escape command's disable test, its count update, the following
command append and item-loop setup. The skill and final stock loops match.

The signed skill kinds are unused (-1), normal (0), passive (1) and secondary
(2). Naming the unused case preserves the native dispatch tree. The secondary
path has a separate word access at `attr + 0xDA0 + entryId * 104`; its byte
view shares the original assembler symbol and does not assert an unsupported
record layout. Normal skills use `flag`, `use` and `koukatype` at offsets
zero, one and two. Disable bytes are written at each native decision, before
later callbacks can change command counts. Stock entries use the existing
unit view at +0xA54, unsigned index bytes at +0x1914, the word count at +0x1924
and item quantities at +0x1928.

The shared `entryId` is consumed as the real key in both skill and stock
enumeration. A separate stock-key control matches 359/410 words. Qualified
`--alloc --no-dumps` controls identify a skill quantity (reg90, 32 references,
live96, one crossed call) assigned `$16` and a stock quantity (reg460, ten
references, live20, no crossed calls) assigned `$5`. Reusing the real key
combines those references into reg90 (42 references, live116, one crossed
call), retaining `$16` and closing all 23 stock-register words. Plain,
hooked and unhooked assembly agree in both controls; no extra operation,
fixed register or artificial dependency is used.

Trace the guarded source with
`tools/cc1probe.sh src/smt3/newbattle/nbCommSelProcess.c --func SetCommList --alloc --no-dumps --cflag -DNON_MATCHING --out /tmp/command-keys`.
