#include "common.h"

/* Runtime workspace passed to the draw thread's native constructor. */
u8 sdfDrawThreadStack[0x8000] __attribute__((section(".bss"), aligned(16)));
