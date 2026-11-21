#include "kernel_draw.h"

SIZE_CHECK(dds3SubLightStateSize, dds3LightState_t, 0x34);

extern sdf3DLight_t D_002F5E20;
extern sdf3DLight_t D_002F5E60;
extern sdf3DLight_t D_002F5EA0;

/* The alternate render environment starts with its own parallel lights and
 * fog state. The automap updates the same fog record before drawing. */
dds3LightState_t dds3LightParam2 __attribute__((aligned(16))) = {
    { &D_002F5E20, &D_002F5E60, &D_002F5EA0, NULL },
    { 0.199999988079071044921875f, 0.199999988079071044921875f, 0.199999988079071044921875f, 1.0f },
    { 255.0f, 0.0f, 1000.0f, 20000.0f, 0x00605040 },
};
