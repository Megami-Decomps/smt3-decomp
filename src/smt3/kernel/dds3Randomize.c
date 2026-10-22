#include "common.h"
#include "kernel.h"
#include "libsdf.h"

extern u8 D_002F5948[];

extern void sdfRandomize(void *aCB, u32 aSeed);

void dds3InitRandCB(void) {
    sdfRandomize(D_002F5948, 0x12345678);
}

