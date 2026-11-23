#include "common.h"

/* Runtime workspace for the vertical-blank thread. */
u8 sdfVblankThreadStack[0x800] __attribute__((section(".bss"), aligned(16)));
