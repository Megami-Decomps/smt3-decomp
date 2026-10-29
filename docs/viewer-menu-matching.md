# Viewer menu grid coordinates

`evtMenuDisp` at `0x001F4B28` is exact ordinary C: 448 bytes, 112 words.
It retains the existing SMT3 reconstruction and adapts the grid-coordinate
formulas and actual header-result staging from DDS1 `evtDrawMenuFrame`,
`src/dds1/game/code_00235270.c:235`, at Megami-Decomps/dds-decomp commit
`f79f86dc00f7fbd266fd5144e71faa86bef12aa4`. The donor worktree differs in
that unit only by a trailing blank line. Existing reconstruction and donor
work are not credited again.

## Native source contracts

SMT3 has nine arguments, not the donor's ten: it has no callback-context
argument. Native keeps the header callback from register11 and loads the
ninth argument, the item callback, at entry `sp+0x60` after allocating its
0x60-byte frame. Header and item calls retain their real three/four arguments.
The item callbacks now share the canonical `void (void *,s32,s32,s32)`
contract; the separately qualified zero-credit correction is documented in
[Viewer callback signatures](viewer-menu-signatures.md). Callback operations
are unchanged, and no DDS-only callback context is introduced.

The panel and text work in grid/pixel coordinates before conversion into GS
fixed-point coordinates. Text starts at `(x*16+0x7000, y*8+0x7900)`; each
header/item row advances the encoded Y by 0x60. Blink arrows are based on the
actual grid offsets: upper Y `y-10`, arrow X `x+5*width+6`, and lower Y
`y+12*rows-2`, followed by shifts of 3 or 4 and the same GS origin. Native
folds these to 0x78B0, 0x7060 and 0x78F0. They are not invented arithmetic
identities: the matched donor uses these same grid formulas, and the matched
panel provider uses the corresponding row/column dimensions.

The shifts explicitly take `u32` values, so negative grid offsets do not
invoke signed-negative left shift. These are the actual packed fixed-point
bits consumed by `sdfDevPrintf(u32,u32,u32,s32,const char *,...)`. Its local
prototype now also agrees with its real `void *` packet return; panel and
tag-link declarations use their canonical `sdfDrawTag_t *` parameters. All
other owner functions stay exact after these truthful local declarations.
No provider implementation or shared header changes.

The header's returned row count is first used to advance Y, then becomes
the row counter. Native `0x001F4BA8..0x001F4BB8` shifts/adds the actual return
value while preserving it as `used`; the donor's meaningful `headerRows`
local models this. Removing this staging restores five differing words.
The existing loop initializer was retained: splitting it into a separate
assignment was unnecessary and removed.

## Qualified compiler evidence

The unmodified pinned 2.96 compiler was run normally and with stock `-da`
dumps through `tools/cc.sh`, with canonical unit paths. Complete assembly
parity passes for both the final source and the multiplication comparison,
normalizing only randomized scratch `.file` prefixes. No hooked GDB trace
is claimed.

The distinction appears before optimization: final `00.rtl` UIDs154/156/158
keep `y-10`, its shift by3, then `+0x7900`. Multiplication-form UIDs158/160/162
already represent `y`, its shift by3, then `+0x78B0`. Thus the latter exposes
a repeated scaled-Y expression early. Final `20.greg` keeps raw Y pseudo86
in register23 as retail does; the multiplication form instead gives register23
to scaled-Y pseudo100 and spills raw Y. Final combine/scheduling still emits
the native folded coordinate constants. No optimization flags, pinned
registers, dummy uses or artificial dependencies are introduced.

## Qualification

Whole `event/evtEventViewerMenu`: **57 match, 0 differ**, including the shared
strings/data and all existing callback/menu peers. `ninja` reproduces retail
SHA-1 `5facfbf7916836613449c922e26a4ec412207b32`. This is one new 448-byte
match; the separate retained viewer-header, frame-list and motion-name
fallbacks are untouched.
