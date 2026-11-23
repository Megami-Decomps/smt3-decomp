#include "common.h"

/* Runtime workspace for the sound sif rpc server thread. */
u8 sdfSifRpcThreadStack[0x1000] __attribute__((section(".bss"), aligned(16)));
