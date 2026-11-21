#include "draw3d.h"

SIZE_CHECK(dds3SubLightSize, sdf3DLight_t, 0x40);

/* The packet builder reads the color and direction with aligned LQC2. */
typedef sdf3DLight_t dds3SubLight_t __attribute__((aligned(16)));

/* Parallel lights of the alternate render environment. */
dds3SubLight_t D_002F5E20 = {
    { 0.7f, 0.7f, 0.7f, 0.0f },
    { 0.0f, 1.0f, 0.5f, 0.0f },
};
dds3SubLight_t D_002F5E60 = {
    { 0 },
    { 1.0f, 0.0f, 0.0f, 0.0f },
};
dds3SubLight_t D_002F5EA0 = {
    { 0 },
    { 0.0f, 0.0f, 1.0f, 0.0f },
};
