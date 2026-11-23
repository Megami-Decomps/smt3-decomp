#include "common.h"

/* Runtime workspace for the ipu decode thread. */
u8 sdfIpuDecodeThStack[0x800] __attribute__((section(".bss"), aligned(16)));
