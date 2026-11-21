#include "effect.h"

SIZE_CHECK(eventFilterDefaultsSize, dds3Effect_Filter_t, 0x18);

/* The writable event filter starts with a full-screen rectangle. */
dds3Effect_Filter_t Eve_Filter = {
    .abgr = 0x40808080,
    .blend = 0x48,
    .xy = { {0, 0}, {512, 448} },
};
