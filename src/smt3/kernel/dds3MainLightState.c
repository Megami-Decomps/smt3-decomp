#include "kernel_draw.h"

SIZE_CHECK(dds3MainLightStateSize, dds3LightState_t, 0x34);

extern sdf3DLight_t fldFixedLight;
extern sdf3DLight_t D_002F5AC0;
extern sdf3DLight_t D_002F5B00;

/* The primary light setup reads three parallel lights, the aligned
 * ambient vector and then the fog parameters. */
dds3LightState_t dds3LightParam __attribute__((aligned(16))) = {
    { &fldFixedLight, &D_002F5AC0, &D_002F5B00, NULL },
    { 0.199999988079071044921875f, 0.199999988079071044921875f, 0.199999988079071044921875f, 1.0f },
    { 255.0f, 255.0f, 3000.0f, 20000.0f, 0x00605040 },
};
