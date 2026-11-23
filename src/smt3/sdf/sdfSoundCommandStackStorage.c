#include "common.h"

/* Runtime workspace for the sound command-interface thread. */
u8 sdfSndCmdThreadStack[0x1000] __attribute__((section(".bss"), aligned(16)));
