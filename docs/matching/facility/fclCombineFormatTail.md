# Fusion name and level format strings

The residual `.sdata` owner contains two formats read by the native fusion
panel renderers: `fclCombineNum2dFmt` (four bytes including NUL at
`0x0054FB10`) displays a demon level with a minimum width of two;
`fclCombineStrFmt` (three bytes including NUL at `0x0054FB18`) copies a
race or demon-name string through the existing `sprintf` call. The four
intervening zero bytes remain uncredited compiler alignment padding. The
cursor pointer and action mode/step keep their native offsets and types.

Stock production builds preserve all allocated sections and relocation
entries of `fclCombineInit.o` exactly. The linked owner matches retail
`.text`, `.rodata`, `.lit4` and all 24 bytes of `.sdata`. In the
`SKIP_ASM` object, four existing HI16/LO16 pairs now refer to the local
section and their real offsets rather than the previously undefined
format symbols. Thirteen existing GP-relative references similarly
follow the two action bytes from packed draft offsets 4/5 to their
native offsets 19/20. No instruction opcode, register, or referenced
object changes.
