# Non-matching drafts: event/evtUnitManager

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## evtSetUnitLightAndAmbient (event/evtUnitManager, 0x94)

34 of 37 words match, at the exact 148-byte size.

The only difference is where the `lui $17, 0x4300` that materialises the
colour-pack scale lands. Retail emits it last in the entry block, after
`sd $31, 0x30($29)` and both parameter stores and immediately before
`jal GetBaseLightRGB`; ee-gcc schedules it into the prologue instead, between
`sd $17, 0x28($29)` and `sd $31, 0x30($29)`, so the three instructions after
it shift down one word each. Every instruction is otherwise identical and the
two pack blocks, the dead `sw $2, 0x0($29)`, and the flag tail all match.

Confirmed on the way: the scale is a GPR constant fed to
`MMI_RGBA_PACK_UNIT`, not an `f32` through `SDF_VU0_dds3ColorFtoI`, since
retail has no `mfc1`; the frame's 0x20 bytes of locals are two 16-byte colour
slots, and `work[0].word = light;` written between `unit->firstCurrent = light;`
and `GetBaseAmbientRGB` is what puts retail's dead `sw $2, 0x0($29)` in that
call's delay slot (the store is deleted outright if it is written after the
second pack, and `volatile` on the member keeps it but moves it to the tail and
costs `$18`); the parameter store pair is `unit->color08` before
`unit->colorDuration`, which is the store right-rotation.

Ruled out, all 4 of 37 or worse and none moving the `lui`: the `scale`
assignment at all four positions among the three parameter stores and as an
initializer at the declaration; all six orders of those three stores crossed
with four declaration orders and `work[1]`, `work[2]`, `work[3]`; `s32`
against `u32` for `scale` and for the two pack results; two separate `scale`
locals, one per pack; and two `sdfVector4_t` locals with
`*(s32 *)&v.x = light;` in place of the colour-slot array. The `work[1]` form
is 10 of 37, so the frame size is load-bearing.

The draft depends on `evtColorWork_t`, defined in the unit above
`MakeLightPacket` and used by it, so the typedef survives a harvest of this
draft alone.
