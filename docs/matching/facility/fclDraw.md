# Matching notes: facility/fclDraw

## trm_212CB8 (facility/fclDraw, 0x170)

92 of 92 words match at the native 368-byte size.

The function draws all row boxes, reloads the panel coordinates, then draws
all centered row labels. Its `s8` count is signed: zero and negative counts
execute neither loop. Widths and heights are unsigned halfwords; spacing is
a signed byte. Each box or text call precedes fresh loads of that row's
spacing and width for the horizontal advance. The next row therefore observes
changes made during the preceding call. Panel depth is read for each draw;
x and y are initialized again between the box and text phases. The text phase
subtracts another 0x20 from y. The provider calls and their contracts remain
unchanged, including the unused height and color arguments to the centered
text provider.

### Local allocation dependency

The ordinary retained body produced 85 of 92 matching words. All seven
residuals were a v0/v1 allocation swap in the text loop's post-call advance,
at function offsets 0x118, 0x11c, 0x124, 0x12c, 0x130, 0x134 and 0x13c.
The box loop, text call setup, frame and all remaining words already matched.
Reversing the addition operands changed load order; splitting the spacing
value into an ordinary local and using an exact same-symbol ABI declaration
did not resolve the swap.

Two localized `FAKEMATCH_ASM_REG` declarations express the measured allocation
requirement: the actual spacing-element address occupies v0 and the signed
byte loaded through it occupies v1. The address is defined after the text
call and consumed once by the byte load. The byte value feeds the real
horizontal advance and dies before the next call. Neither binding crosses
a call, carries an uninitialized value, changes an access width or introduces
a dummy consumer. No empty assembly template or instruction-emitting assembly
is needed. Binding the byte value alone leaves three differing address-chain
words; binding both real values reproduces all 368 bytes. This is a
target-specific reconstruction aid, not a claim about original source spelling.

Pinned ee-gcc 2.96 RTL dumps corroborate the allocation scope. In the ordinary
body, scheduler RTL has post-call address UID 197, signed-byte load UID 203,
width reload UID 211, addition UID 213 and shift UID 217. The final body has
an explicit v0 address followed by the v1 signed-byte load (UID 210), then
width reload UID 219. Global-allocation RTL and final assembly show the
correct address/value registers and the unchanged post-call ordering.
These are ordinary compiler dumps, not a hooked debugger decision trace.

### Provider declarations and qualification

`fclSetSpriteEx` already had its exact provider declaration guarded by
`NON_MATCHING`, leaving an implicit declaration in the production compile.
Exposing that existing declaration in all modes preserves the provider in
`facility/fclShare.c`: `void`, sprite and rectangle pointers, and six `s32`
arguments. `cmpDrawBoxF` remains `void` with seven `s32` arguments;
`fclDrawStrCenterTarget` remains `s32` with three `s32` coordinates/priority,
two `u16` dimensions, `u32` color, `const char *` text and `s32` ordering-table
argument. No ABI opacity or provider body change is retained.

Whole-owner checks pass with 29 matches and zero differences both normally
and with `NON_MATCHING`. Production, `SKIP_ASM`, and
`SKIP_ASM` plus `NON_MATCHING` compile with
`-Werror-implicit-function-declaration`. All non-target assembly remains
identical in those three modes. For the ordinary control and final candidate,
whole-unit plain and `-da` compilations have identical assembly after only
normalizing the random scratch-directory `.file` directive, identical
allocated sections, and identical relocation contracts. Probe runs use the
canonical `src/smt3/facility/fclDraw.c` owner through `--as-unit`; no compiler
flags or translation-unit boundaries change. Full-image integration and
regression checks remain the final publication gates.

Independent native and ABI review validates the short register lifetimes and
callback observations. An extracted-actual-body oracle passes 2,048 cases at
O2 and under ASan/UBSan, including callback mutations and signed-count edges.
This is a host control/data-flow test, not native hardware execution.
