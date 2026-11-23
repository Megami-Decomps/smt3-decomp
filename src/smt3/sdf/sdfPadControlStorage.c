#include "sdf_pad_control.h"

/* Two records consumed by the pad manager at a 0x28-byte stride. */
sdfPadCb_t sdfPadCb[2] __attribute__((section(".bss"), aligned(16)));
