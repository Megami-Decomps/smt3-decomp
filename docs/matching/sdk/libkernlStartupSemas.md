# Matching notes: SDK startup semaphores

The signed IDs at 0x52C378 and 0x52C37C start at zero.
`supplement_crt0` creates both semaphores and stores the results here.
The libc heap lock uses `mallocSema`; the libgcc frame registry brackets
its list accesses with `frameObjectSema`. Keep their existing declarations.

The explicit section keeps the two zero-initialized words in native
writable .data. Removing it places both symbols in .sdata in both
stock modes. The raw neighbors keep all their original bytes and named
relocations. The scoped stock link reproduces the full retail binary.
