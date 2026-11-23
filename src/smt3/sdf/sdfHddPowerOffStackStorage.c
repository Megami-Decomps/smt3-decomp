#include "common.h"

/* Native workspace for the HDD power-off worker. */
u8 sdfHddPowerOffStack[0x800] __attribute__((section(".bss"), aligned(16)));
