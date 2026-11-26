#include "fldModelResourceStorage.h"

/* sdfDevLoadFileEx writes the EE address and byte size to separate word banks. */
u32 fldModelResPtr[64] __attribute__((section(".bss")));
u32 fldModelResSize[64] __attribute__((section(".bss")));
