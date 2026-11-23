#include "common.h"

/* Runtime workspace for the main engine thread. */
u8 sdfSystemThreadStack[0x2000] __attribute__((section(".bss"), aligned(16)));
