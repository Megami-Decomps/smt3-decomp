#include "effect.h"

/* The rectangle-blur editor copies this whole native 0x28-byte source. */
effScreenDrawParams_t evtUnityCalcArea = {
    .source = {
        .abgr = 0x60808080,
        .blend = 0x44,
        .rotate = 0.02f,
        .scale = 0.985999942f, /* One ULP below 0.986f in retail. */
        .center = { -6, 26 },
        .xy = { { 0, 0 }, { 512, 448 } },
    },
};
