#include "effect.h"

SIZE_CHECK(distortionBlurDefaultSize, dds3Effect_DistortionBlurParam_t, 0x2C);

dds3Effect_DistortionBlurParam_t dblurDefaultParam = {
    .number = 100,
    .interval = 15,
    .speed = 0x1.c6a7eep-3f,
    .abgr = 0x65848484,
    .blend = 0x44,
    .rotate = 0x1.47ae14p-8f,
    .distortion = 0x1.0a3d70p-3f,
    .center = { 0, 0 },
    .area = 256,
    .width = 136,
};
