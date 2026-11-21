#include "draw3d.h"

SIZE_CHECK(dds3NativeRenderLightSize, sdf3DLight_t, 0x40);

/* Native light settings for the off-screen renderer. The last two lights
 * are initially black but keep their own directions and writable storage. */
sdf3DLight_t D_002F5C50 __attribute__((aligned(16))) = {
    { 0.699999988079071044921875f, 0.699999988079071044921875f, 0.699999988079071044921875f, 0.0f },
    { 0.0f, 1.0f, 0.5f, 0.0f },
};
sdf3DLight_t D_002F5C90 __attribute__((aligned(16))) = {
    { 0 },
    { 1.0f, 0.0f, 0.0f, 0.0f },
};
sdf3DLight_t D_002F5CD0 __attribute__((aligned(16))) = {
    { 0 },
    { 0.0f, 0.0f, 1.0f, 0.0f },
};
