#include "draw3d.h"

SIZE_CHECK(dds3RenderScreenStateSize, sdf3DScreenParam_t, 0xA0);
typedef char dds3RenderScreenFovyOffset[
    (size_t)&((sdf3DScreenParam_t *)0)->fovy == 0x0C ? 1 : -1];

/* The render-texture camera starts with a square viewport. Its projection
 * math fills in the derived portion and shares fovy with the boss effect. */
sdf3DScreenParam_t dds3RenderScreenParam __attribute__((aligned(16))) = {
    .flags = SDF_3D_SCREEN_FLAG_FOVY,
    .aspect = 0.7999999523162841796875f,
    .fovy = 0.6283185f,
    .ox = 2048.0f,
    .oy = 2048.0f,
    .width = 128.0f,
    .height = 128.0f,
    .zMin = 256.0f,
    .zMax = 16711680.0f,
    .nearClip = 16.0f,
    .farClip = 65536.0f,
};
