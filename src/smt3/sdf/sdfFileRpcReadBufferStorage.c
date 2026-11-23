#include "common.h"

/* Native one-chunk staging buffer for file RPC reads. */
u8 sdfFileRpcReadBuf[0x4000] __attribute__((section(".bss"), aligned(16)));
