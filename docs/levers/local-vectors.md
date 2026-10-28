# Two local vectors: declaration order fixes the stack offsets


Local aggregates are laid out in declaration order from offset 0, each rounded
to 16 bytes, so which of two `f32 v[4]` locals lands at `0($29)` decides
whether a VU0 macro taking the address as a register operand gets a bare
`$29` or retail's `addiu $2, $29, 0x10`. One wrong order shifts the whole body
by a word: `dds3Particle_SparkBuffInit` sat at 55 matching words and 2008
bytes until `position[4]` was declared before `direction[4]`, and went to 335
matching words at the exact size on that line alone. Read the offsets in the
retail `lqc2`/`sqc2` address setups before writing the declarations.

## The store macro's memory clobber decides whether a frame address is reused

Three uses of `&v[0]` in a row, each as a register operand of a VU0 macro,
either collapse into one callee-saved register or stay as three separate
`addiu $2, $29, off`. `SDF_VU0_STOREVECTOR` carries a `"memory"` clobber and
`SDF_VU0_STOREVECTOR_NOMEM` does not, and that is the whole difference: with
the clobber cc1 keeps the address in one register, without it each use
recomputes it. In `nbCameraState_NegoHojo_Init` the clobbering form left one
extra callee-saved value live, which renamed every register in the function;
swapping the two sphere-centre stores to the `_NOMEM` form took it from 33 to
250 of 250 words with no other change.

Two smaller levers from the same function. Local arrays, not scalars, are what
put a value in memory and give it 16-byte alignment: `nbFormation_t *p[2]` at
`0($29)` and `f32 len[2]` at `0x10($29)` leave the 8-byte gaps retail has,
which separate scalars never produce. And `u32 i` is what keeps a `for (i = 0;
i < 4; i++)` counting up: with `s32 i` gcc reverses it into a `bgez`
countdown, while the unsigned compare retail emits as `sltiu $2, $17, 0x4`
blocks the reversal.
