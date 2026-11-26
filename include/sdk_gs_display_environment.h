#ifndef SMT3_SDK_GS_DISPLAY_ENVIRONMENT_H
#define SMT3_SDK_GS_DISPLAY_ENVIRONMENT_H

#include "common.h"

typedef struct {
    u64 pmode;
    u64 smode2;
    /* The EE updates the low register word without changing the high word.
     * The GS SDK transfers the same little-endian register as one u64. */
    union {
        u64 packed;
        struct {
            u32 low;
            u32 high;
        } words;
    } dispfb;
    u64 display;
    u64 bgcolor;
} sceGsDispEnv;

SIZE_CHECK(sceGsDispEnvNativeSize, sceGsDispEnv, 0x28);

void sceGsSetDefDispEnv(sceGsDispEnv *disp, s16 psm, s16 w, s16 h, s16 dx, s16 dy);
void sceGsPutDispEnv(sceGsDispEnv *disp);

#endif
