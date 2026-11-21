#include "effThunderPolyPresetData.h"

SIZE_CHECK(ziodinThunderParamSize, effPCP_ThunderPolyParam_t, 0x54);

/* Initialization adjusts this writable preset before creating the strip. */
effPCP_ThunderPolyParam_t effPCPZiodin03Param[1] __attribute__((aligned(16))) = { {
    .start = { 0x0p+0f, -0x1.f4p+8f, 0x0p+0f, 0x0p+0f },
    .end = { 0x0p+0f, 0x0p+0f, 0x0p+0f, 0x0p+0f },
    .blend = 2, .pad22 = { 0x00, 0x00 },
    .number = 1, .restartFrameLimit = 1,
    .waveAmplitude = 0x1.4p+3f, .startDelayRange = 4, .activeFrameRange = 10,
    .split = 10, .pad3A = { 0x00, 0x00 }, .coreWidth = 0x0p+0f,
    .color_center = 0xFFFFFFFF, .width_border = 0x1.8p+0f,
    .color_border = 0xFF802020, .width_edge = 0x1.ep+3f,
    .color_edge = 0x00802000,
} };
