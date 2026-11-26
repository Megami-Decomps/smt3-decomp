#include "sdk_libpad_layout.h"

/* Each port has two native 0x80-byte DMA halves. scePadPortOpen requires a 64-byte aligned port row. */
padDma sdfPadPortBuffers[2][2] __attribute__((section(".bss"), aligned(64)));
