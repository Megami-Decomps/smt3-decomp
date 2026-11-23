#include "common.h"

/* Two packed face indices for each of the eight quads. */
u64 fldQuadFaceIndex[8] = {
    0x0003020000020100ULL,
    0x0007060400060504ULL,
    0x000B0A08000A0908ULL,
    0x000F0E0C000E0D0CULL,
    0x0013121000121110ULL,
    0x0017161400161514ULL,
    0x001B1A18001A1918ULL,
    0x001F1E1C001E1D1CULL,
};

/* The separate retained pair reverses the winding and uses the other
 * diagonal. The live eight-row table above is not extended. */
u64 D_002FC790 __attribute__((section(".data"))) = 0x0000010300010203ULL;
