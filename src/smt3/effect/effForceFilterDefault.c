#include "effect.h"

/* The force-filter editor also writes the existing blend view at offset four. */
dds3Effect_Filter_t dblurParam2 __attribute__((section(".data"))) = {
    .abgr = 0x40808080,
    .blend = 0x48,
    .xy = { {0, 0}, {512, 448} },
};
