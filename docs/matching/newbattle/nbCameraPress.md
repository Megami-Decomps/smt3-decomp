# Matching notes: newbattle/nbCameraPress

## nbCameraPressProcessRun (newbattle/nbCameraPress, 0xF4)

2 of 61 words differ, at the exact 244-byte size, and every instruction is
retail's.

The two are a load and a store swapped in the block after `effColorBlend`:
retail reloads `nbCameraPressFrame` into `$4` and then stores the blend result
through `$16`, while the draft stores first. `-fno-strict-aliasing` leaves it
exactly where it is and `-fno-schedule-insns2` costs 29 words, so this is the
second scheduler's ordering of two independent memory references, not anything
the source states. Measured and rejected: `s32 *`, `f32 *` and `void *` for the
store's pointer with matching declarations for `D_00325970` and `D_00325998`;
`s32` for `nbCameraPressFrame` itself, which costs a word by turning the
`sltiu` bound test into `slti`; a `u32 color` local for the blend result; and
re-reading `nbCameraPressFrame` into the frame local inside the `if`, before and
after the store, which gcc folds back to the same two instructions.

Confirmed on the way: the whole body has to nest inside the two guards rather
than use early returns, which is what gives retail's one shared epilogue and
took the draft from 43 of 61 words to 59; `frame` has to be read from the global
*inside* the `nbCameraPressFrame < 0xA` test, not above it, since the unsigned
bound test wants the global directly and the signed `< 3` and `0xA - frame`
tests want the `s32` copy, which is retail's `lw $4` plus `daddu $3, $4, $0`;
the rate is `frame / 3.0f` on the way in, `(0xA - frame) / 3.0f` on the way out
and `1.0f` in between; the colours are `0x808080` and `0x60808080`; and reusing
`frame` for the trailing increment is what gets retail's registers, where a
separate local is the same two words but reached from a worse starting point.
