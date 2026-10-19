# Two local vectors: declaration order fixes the stack offsets


Local aggregates are laid out in declaration order from offset 0, each rounded
to 16 bytes, so which of two `f32 v[4]` locals lands at `0($29)` decides
whether a VU0 macro taking the address as a register operand gets a bare
`$29` or retail's `addiu $2, $29, 0x10`. One wrong order shifts the whole body
by a word: `dds3Particle_SparkBuffInit` sat at 55 matching words and 2008
bytes until `position[4]` was declared before `direction[4]`, and went to 335
matching words at the exact size on that line alone. Read the offsets in the
retail `lqc2`/`sqc2` address setups before writing the declarations.
