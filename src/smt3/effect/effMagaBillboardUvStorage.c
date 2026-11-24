#include "common.h"

/* The initializer writes fifteen 16-byte UV rows; each billboard copies one. */
s16 effMagaBillboardTexCoords[15][8] __attribute__((section(".bss")));
