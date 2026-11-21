#include "common.h"
#include "kernel.h"
#include "libsdf.h"

#include "sdf_math_state.h"

extern sdfRandCB_t dds3RandCB;

extern void sdfRandomize(sdfRandCB_t *aCB, u32 aSeed);

void dds3InitRandCB(void) {
    sdfRandomize(&dds3RandCB, 0x12345678);
}

