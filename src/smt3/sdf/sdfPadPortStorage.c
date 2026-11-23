#include "common.h"

/* Two pad-port buffers, with a 0x100-byte stride. */
u8 sdfPadPortBuffers[2 * 0x100] __attribute__((section(".bss"), aligned(16)));
