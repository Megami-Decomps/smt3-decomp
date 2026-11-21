#include "common.h"

/* Native order of the 16 button-state slots populated by the pad manager. */
u16 sdfPadBitMask[16] = {
    0x0080, 0x0040, 0x0010, 0x0020, 0x8000, 0x2000, 0x1000, 0x4000,
    0x0004, 0x0001, 0x0008, 0x0002, 0x0800, 0x0100, 0x0200, 0x0400,
};

/* Actuator alignment selects the first two motors and leaves four unused. */
u8 sdfPadCb_0[8] __attribute__((section(".data"), aligned(8))) = { 0, 1, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 };

/* Both state buffers are initially zero in the file-backed native data. */
s8 dds3PadBitTable[32] __attribute__((section(".data"), aligned(8))) = { 0 };
u8 sdfPadPressure[24] __attribute__((section(".data"), aligned(8))) = { 0 };
