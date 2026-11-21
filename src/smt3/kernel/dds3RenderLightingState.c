#include "draw3d.h"

SIZE_CHECK(dds3NativeRenderFogSize, sdf3DFogParam_t, 0x14);

sdfVector4_t dds3RenderAmbientColor __attribute__((aligned(16))) = {
    0.199999988079071044921875f, 0.199999988079071044921875f, 0.199999988079071044921875f, 1.0f,
};
sdf3DFogParam_t dds3RenderFogParam = {
    255.0f, 0.0f, 1000.0f, 20000.0f, 0x00605040,
};
