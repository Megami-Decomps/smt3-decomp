# fldSnow.c

## fldSnowTask (1280 bytes)

The snow task reproduces all 320 native instructions. A single ordinary
local holds the actual spawn X coordinate across the Z RNG call in either
mode, as in the sibling seed reset. The wide-area X origin is an initialized
sum of the field base and the snow offset, sampled after the RNG call.
The shared local does not merge callbacks or move either root read across one.

The camera target and eye remain volatile. The separate local direction
vector is never passed to a callback or used as the rendered sprite. Stock
ee-gcc 2.96 retains its four ordinary stack stores in this frame even without
a volatile declaration, including the zero Y store in the mode branch delay
slot. The separate sprite receives the actual halfword UV endpoint and the
two signed corner offsets. The localized constraints hold only these
initialized inputs while the compiler emits their real stores and draws.
The two 700-record draw loops keep the native record stride, signed velocity
sample and unsigned color shifts, including the upper bits of the packed color.
