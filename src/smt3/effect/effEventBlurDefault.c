#include "effect.h"

SIZE_CHECK(eventBlurDefaultSize, dds3Effect_Blur_t, 0x28);

/* The writable event blur contains the complete native framebuffer quad. */
dds3Effect_Blur_t Eve_Blur = {
    .abgr = 0x60808080,
    .blend = 0x44,
    .rotate = 0x1.47ae14p-6f,
    .scale = 0x1.f8d4fcp-1f,
    .center = { -6, 26 },
    .xy = { { 0, 0 }, { 512, 448 } },
};
