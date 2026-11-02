# Matching notes: newbattle/nbMakaProcess

## nbDrawMakaNum (newbattle/nbMakaProcess, 0x2B4)

136 of 173 words match, at the exact 692-byte size.

The 40-byte `ldl`/`ldr` copy of `nbDigitStringTable` onto the stack comes from
`memcpy(nums, nbDigitStringTable, sizeof(nums))` over a `char *nums[10]`, and
the whole digit loop, including the `break 0, 7` guard on the variable divisor
`base = 10`, is exact. A major difference is where the scheduler puts
`addiu $17, $0, 6`, the loop counter's initialiser: retail issues it at +0x158,
immediately after the third `nbPnlSprite` call reads `y` out of `$17` for the
last time, while ee-gcc sinks it to +0x1EC, just before the loop. Every other
differing word is that one-slot shift rippling through the fourth and fifth
call's argument blocks, plus the `$4`/`$5` order in the third call and the
`or $20` / `addiu $18, 0xA` pair swapping.

Ruled out: the three initialisers (`i`, `started`, `base`) in all six source
orders, as plain statements, as a `while`, as a `do`/`while`, and with `i` and
`started` sharing the `for`-init comma; `i = 6` written literally between the
third and fourth `nbPnlSprite` calls (the scheduler sinks it anyway); `i` as an
initialised declaration (60 words differ, the parameter registers move);
reusing the `y` parameter as the counter (62 words differ, `x` and `y` swap
registers); and `i != -1` and `i > 0` loop tests, both of which overrun the
function. `v` must be `s32`, not `u32`, or `v % base` becomes `divu`.


### Defined shifts and truthful font interfaces

The retained draft now converts alpha to `u32` before shifting by 24. The
normal fade reaches alpha 128, for which the old signed shift was undefined.
Coordinate shifts similarly use unsigned low-word arithmetic before explicit
conversion to the provider's `s32` coordinates. That last conversion relies on
the verified EE representation for high-bit results; this is not a portable
signed-wrap guarantee and does not change the earlier signed additions.

The three local font declarations now match their providers: the numeric
request returns and accepts canonical font handles, its color is `u32` and
text is const; the drawing helper takes `s8` mode and `u32` flags and returns
`s32`; the destructor returns `s32`. The real ignored returns, null previous
handle, call order and provider bodies are unchanged. These corrections are
byte-inert: the privately promoted draft remains 136/173 words at 692 bytes,
with all 13 existing peers exact. Production retains the assembly fallback
and receives no additional matching credit.

A new counter-lifetime study reached 151/173 using an input-only constraint
and a descending do/while loop. A real third-call Y argument capture also
placed the counter initialization before the third call, as retail does,
but still left 23 scheduling differences at the native 692-byte size. Thus
achieving the early counter definition alone is insufficient. These failed
constraints are not retained in the source or shared headers.
