#include "draw3d.h"

SIZE_CHECK(dds3PrimaryScreenStateSize, sdf3DScreenParam_t, 0xA0);

/* TM, scale and translation have native aligned VU loads. The draw
 * setup computes the derived portion of this record at runtime. */
sdf3DScreenParam_t dds3ScreenParam __attribute__((aligned(16))) = {
    .flags = SDF_3D_SCREEN_FLAG_HALFHEIGHT | SDF_3D_SCREEN_FLAG_FOVY,
    .aspect = 1.165f,
    .fovy = 0.6283185f,
    .ox = 2048.0f,
    .oy = 2048.0f,
    .width = 512.0f,
    .height = 448.0f,
    .zMin = 256.0f,
    .zMax = 16711680.0f,
    .nearClip = 16.0f,
    .farClip = 65536.0f,
};

/* Frame setup loads the target and eye as VU quadwords, and the field
 * camera writes their full quadwords. The preceding aligned screen and
 * 16-byte rows keep all four records on native quadword boundaries. */
sdfVector4_t aViewTarget = { 0.0f, -100.0f, 0.0f, 0.0f };
sdfVector4_t aViewEye = { 0.0f, -100.0f, -400.0f, 0.0f };
sdfVector4_t aViewDown = { 0.0f, 1.0f, 0.0f, 0.0f };
