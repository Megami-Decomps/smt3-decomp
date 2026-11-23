#include "common.h"

/* Runtime workspace for the display thread. */
u8 sdfDispThreadStack[0x1000] __attribute__((section(".bss"), aligned(16)));
