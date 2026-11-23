#include "kernel_draw_packets.h"

/* DMA chains reference each packet's 16-byte-aligned header. */
dds3ZClearPacket_t dds3ZClearDrawTag[3][2] __attribute__((section(".bss"), aligned(16)));
dds3FadePacket_t dds3FadePacket[2] __attribute__((section(".bss"), aligned(16)));
