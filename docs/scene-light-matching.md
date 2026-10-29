# Scene-light transition matching

`kernel/dds3KernelDraw::dds3SceneLightUpdateFirst` is 1,532 bytes at
`0x001074F0`. The reconstruction follows the matched DDS implementation in
`src/dds1/game/code_00107FD8.c`, retaining SMT3's recovered symbols and existing
light/fog records. Source credit: **Megami-Decomps/dds-decomp / redthing1**.
The donor snapshot's complete-file SHA-256 is
`4f0aee2f4783b5f695affe6a7dbc1bedb980f08e97729e287be91d115f8db96e`.

## Native behavior and real storage

- Three parallel lights use successive color/direction flag pairs. Null light
  pointers clear both flags. The native unsigned-halfword counters increment
  before computing each interpolation ratio and clear their flag at the end.
- The remaining paths interpolate ambient color, packed fog color, four fog
  parameters, and the face-divide soften/bias pair. The original masks,
  endpoints, counter widths and arithmetic order are retained.
- Direction interpolation uses the existing `Lerp(f32)` and `ToMatrix(void)`
  providers in `sdf/sdfQuat.c`. Their shared VU quaternion/matrix interface
  matches the native two-call sequence at `0x001076C4` and `0x001076CC`.
- Six calls to one shared float-input unpack helper consume the two light,
  two ambient and two fog color words. Each owns a real 16-byte color carrier;
  the native stores/read addresses occupy frame offsets `0x00` through `0x50`.
  The final packed fog result uses the seventh carrier at `0x60`. Its C output
  is used for the actual fog-color store, not a dummy read or unused padding.
  Together with the saved registers these reproduce the native `0xC0` frame.

`EE_MMI_RGBA_UNPACK_READONLY` already declares the actual first-word memory
input and the integer scratch clobber. Passing its scale through the shared
`f32` helper preserves the donor's normal SF-mode reload behavior; there is
no added fake FP operand, pinned C register, memory clobber or forced bit-move.
The existing scalar VU macros are direct specializations of the donor macros;
their float operands transfer binary32 bits rather than numerically converting
them to integers.

The existing address-only VU stores write the parallel light's color/direction
and ambient vector. No following C scalar read in this function consumes those
stored vector elements. This is the established SDK/VU register-and-address
interface, not a claim that those macros declare general C memory outputs.
The unpack helper, separately, has a real read-only memory operand.

## Canonical fog owner and compiler evidence

The final fog-color store uses the existing `dds3FogState.color` view, just as
the four scalar fog stores below use that record. The pre-existing
`PS2_SYMBOL_AT(sdf3DFogParam_t, dds3FogState, dds3LightParam, 0x20)` allocates
nothing: `dds3SetFog` copies the actual 20-byte fog record into that same
subobject. The primary light-state type and all aliases/layouts are unchanged.

The first donor adaptation used `dds3LightParam.fog.color` and emitted one
extra address instruction (1,536 bytes, 239/383 native words matching).
Selecting the already-established fog owner removes that extra address and
matches all 383 words. The final RTL starts the packed result as a real C
assignment to carrier `+0x60` and then to fog-record `+0x10`; it does not insert
an artificial dependency. No source-order or register-assignment grid was used.

Final plain compilation and ordinary `-da` compilation produce identical
assembly after normalizing only the temporary `.file` line. This is unhooked
RTL evidence, not a new `cc1probe`/GDB trace or a claim to have repeated the
DDS donor's historical hooked trace. The official compiler is unchanged.

## Qualification

Against public source baseline `dbbed7f69a8001cc55fedcb59944e707bf846610`:

- Complete `kernel/dds3KernelDraw` owner: **87 match, 0 differ**.
- The unrelated `dds3DrawFade` stays behind `NON_MATCHING`; its one draft
  compiles. Its draft documentation is retained.
- Full retail build: SHA-1 **`5facfbf7916836613449c922e26a4ec412207b32`**.
- Generated nonmatching indexes were regenerated from that public baseline.

Only this function is newly matched. No shared header, provider, compiler
option, symbol definition or unrelated production fallback is changed.
