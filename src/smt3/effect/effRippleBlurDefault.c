#include "effect.h"

SIZE_CHECK(rippleBlurDefaultSize, dds3Effect_RippleBlurParam_t, 0x2C);

dds3Effect_RippleBlurParam_t rblurDefaultParam = {
    .number = 40,
    .speed = 0x1.47ae14p-5f,
    .interval = 0x1.999998p-3f,
    .abgr = 0x80808080,
    .blend = 0x44,
    .rotate = 0x1.999998p-6f,
    .power = 0x1.1eb850p-3f,
    .frequency = 0x1.c28f5cp-3f,
    .center = { 0, 0 },
    .area = 256,
};
