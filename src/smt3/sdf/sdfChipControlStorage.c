#include "common.h"
#include "libsdf.h"

/* One allocator control record per native chip size class. */
sdfChipCB_tag sdfChipCB[7] __attribute__((section(".bss"), aligned(16)));

typedef char sdfChipControlStorageSize[(sizeof(sdfChipCB) == 0x54) ? 1 : -1];
