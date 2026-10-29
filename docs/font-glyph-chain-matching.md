# Glyph-chain drawing and fade provider

Two native SMT3 bodies are exact ordinary C in `interface/frFont`:

| Function | Address | Bytes |
| --- | --- | ---: |
| `frDrawEffect` | `0x001C7CB8` | 328 |
| `func_001C7E38` (glyph-chain drawing) | `0x001C7E38` | 640 |

The shared algorithm comes from DDS1 `frFontAdvanceChildFadeAndGetJitter`
and `frFontDrawGlyphChain`, `src/dds1/interface/frFont.c:687,797`, at
Megami-Decomps/dds-decomp commit
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. That donor unit differs locally
only by a final blank line. Existing donor and SMT3 peer work is not counted
again: this promotion adds two functions and 968 instruction bytes.

## Native title-specific contracts

The fade provider uses the existing parent/child types in `include/font.h`.
A child's `previous` at +0x24 supplies the predecessor fade. If absent, it
checks the preceding parent's `lastChild` at +0x20, then one further parent
at +0x24 when that parent has no last child. These are exactly two parent
levels, not an unbounded traversal. Only those parent fallback paths load the
predecessor's option byte +0x16; the direct-child path retains option zero.
Absent predecessors retain fade 0x80. Native `0x001C7CC0..0x001C7D10`
establishes the branches and shared fade-byte read.

The provider advances the real render word at child +0x10, saturating its low
fade byte at 0x80. A signed-nonnegative step is scaled by 22 over child advance
plus parent spacing; a zero advance uses 24. The high-bit step bypasses that
scaling. No DDS wait-frame, sound or lips-stop state is imported into SMT3's
smaller parent record.

Jitter is the five-phase sequence from the real u16 draw count: center its
remainder around two, convert to 4-bit fixed point, and negate. The centered
value is only -2 through 2. An unsigned shift/negation computes the 32-bit
fixed-point representation without shifting a negative signed value; the EE
compiler's two's-complement conversion to the signed return yields only -32
through 32. This relies on the target's signed conversion convention, not on
signed-negative-shift undefined behavior. A multiplication spelling distributes
the -2 before shifting and changes two native words; no compiler option is used.
Glyph advance likewise uses unsigned 32-bit addition and shift before the EE
signed-coordinate conversion, matching native ADDU/SLL even with negative
spacing. No full-s32 safety claim is made for signed multiplication: the setter
accepts a generic s32 width, although its two actual call sites pass 16 and 12.

The caller follows `chainHead` (+0x2C), then each parent's real `firstChild`
(+0x1C) and child's `next` (+0x28). It resets `frDrawFontWork` per parent and
increments it after each actual draw; it reads the child's fade and next link
after the packet call. It returns the total child count only when all fades
are complete. The initialized-one filtering state still has its ASCII
`enabled = 1` reassertion: native `0x001C7FA4` retains the conditional MOVN,
and the DDS twin has the same initialized-one/reasserted-one source pattern.
This is a native retained trace, not an invented mutable state or dummy use.
Mode is genuinely s8, and the render selector is the original
third argument, not a DDS-only context or timer.

Only font slots 0 and 1 receive ASCII bearing adjustments. Native guards use
`(u16)(code-'A') < 26` or `(u16)(code-'a') < 26`; the exact range and four-byte
indexed loads establish four 26-entry s32 coordinate tables independently of
adjacent symbol boundaries:

| Alphabet | X table | Y table |
| --- | --- | --- |
| uppercase | `0x00326278` | `0x00326210` |
| lowercase | `0x003262E0` | `0x00326348` |

Each is 0x68 bytes. The uppercase X table has a correction for Y; lowercase X
has corrections for i and k, and lowercase Y for i, l and r. The remaining
entries, including all uppercase Y entries, are zero. Their central data owner
and bytes are unchanged; these are declarations of the actual indexed arrays,
not oversized views or additional data definitions.

The packet provider is existing exact `frCreateDrawFontPacket_20550848` in the
same owner. Both paths pass an actual `FontMem_t::uv` rectangle from the cached
or user-supplied source, and `&gFT.atlas`, the existing +0x160 atlas subobject
(native label `frFontTable`). Direct atlas arguments remove an unnecessary
local; no new alias or record is introduced. The provider consumes all eleven
real arguments and registers the allocated packet through the selected OT.

## Source reductions and compiler evidence

The direct donor traversal plus native bearings first emitted the exact
640-byte size with three differing words. A genuine C fade provider closes
one mode-call branch-annul word. Removing the redundant cached atlas pointer
closes the other two scratch-register words. Neither step uses a stub or a
false provider declaration.

For the provider, a generic final predecessor-null test left 336 rather than
328 bytes. Keeping the fade read in each actual successful fallback branch
lets the compiler merge those reads and preserves the native two-level
control flow. Directly reading `previousParent->previous->lastChild` avoids
needlessly overwriting the first-parent local. The two live zero-initialized
outputs are declared jitter first, then fade eligibility, matching native's
initialization order; neither is a dummy value. No declaration grid or fake
state was added.

Final-source stock `-da` dumps reproduce the complete plain compiler assembly,
normalizing only randomized scratch `.file` prefixes. This is stock pass
output, not a hooked allocator/decision trace. The pinned compiler and normal
per-owner build flags are unchanged.

## Qualification

Complete `interface/frFont`: **77 match, 0 differ**, including native data and
all 75 previously matched peers. The full image reproduces retail SHA-1
`5facfbf7916836613449c922e26a4ec412207b32`. No ownership boundary, header layout,
SDK provider, alias, macro, compiler or runtime flag changes are required.
