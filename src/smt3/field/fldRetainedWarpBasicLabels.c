#include "common.h"

/* The early copy of the basic warp labels uses the same 16-byte slots
 * as the live selector labels. It is a separate retail storage range. */
u8 D_002FF3A0[4][16] = {
    "doorwarp_label",
    "runwarp_label",
    "anawarp_label",
    "tobiwarp_label",
};
