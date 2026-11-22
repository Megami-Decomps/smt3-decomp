#include "common.h"

/* GS ALPHA_2 halfwords read by sdfMatUpdatePacketMultiTex.
 * Retain the complete native slot, including its two trailing zero halfwords. */
u16 sdfMatMultiTexAlphaTable[8] = {
    0x0044, 0x0048, 0x0042, 0x0044, 0x0048, 0x0048, 0x0000, 0x0000,
};
