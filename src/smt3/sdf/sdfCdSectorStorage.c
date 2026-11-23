#include "common.h"

/* One sector for unaligned reads and the cached remainder of a short read. */
u8 sdfCdSectorBuf[0x800] __attribute__((section(".bss"), aligned(16)));
