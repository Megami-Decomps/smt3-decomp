#include "sdf_types.h"

/* Writable render presets passed to the lighting and fog setters. */
sdfVector4_t fclEncycLightColor __attribute__((aligned(16))) = {
    1.0f, 1.0f, 1.0f, 0.0f
};
sdfVector4_t fclEncycLightDir __attribute__((aligned(16))) = {
    0x1.eb851ep-1f, 0x1.70a3d6p-2f, 0x1.c28f5cp-2f, 1.0f
};
sdfVector4_t fclEncycAmbientColor __attribute__((aligned(16))) = {
    0x1.eb851ep-2f, 0x1.ccccccp-2f, 0x1.147aep-1f, 0.0f
};
sdfVector4_t fclEncycFogColor __attribute__((aligned(16))) = {
    0x1.999998p-4f, 0x1.999998p-4f, 0x1.999998p-4f, 1.0f
};

/* ControlModel loads these axis steps with lqc2. */
f32 fclEncycPanStepX[4] __attribute__((aligned(16))) = {3.0f, 0.0f, 0.0f, 0.0f};
f32 fclEncycPanStepY[4] __attribute__((aligned(16))) = {0.0f, 3.0f, 0.0f, 0.0f};
