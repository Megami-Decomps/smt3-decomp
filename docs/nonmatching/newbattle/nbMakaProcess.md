# Non-matching drafts: newbattle/nbMakaProcess

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## nbDrawMakaNum (newbattle/nbMakaProcess, 0x2B4)

136 of 173 words match, at the exact 692-byte size.

The 40-byte `ldl`/`ldr` copy of `nbDigitStringTable` onto the stack comes from
`memcpy(nums, nbDigitStringTable, sizeof(nums))` over a `char *nums[10]`, and
the whole digit loop, including the `break 0, 7` guard on the variable divisor
`base = 10`, is exact. The one real difference is where the scheduler puts
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
