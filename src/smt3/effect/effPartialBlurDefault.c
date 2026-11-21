#include "effect.h"

SIZE_CHECK(partialBlurDefaultSize, dds3Effect_PartialMultiBlurParam_t, 0x2C);

dds3Effect_PartialMultiBlurParam_t mblurDefaultParam = {
    .area = 150,
    .Blur = {
        .abgr = 0x70808080,
        .blend = 0x44,
        .rotate = 0x1.1eb850p-4f,
        .scale = 0x1.da1cacp-1f,
    },
};
