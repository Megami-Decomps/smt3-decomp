#include "effThunderPolyPresetData.h"

SIZE_CHECK(shockWavePolyPresetSize, effPCP_ThunderPolyParam_t, 0x54);

/* The spawner adjusts both endpoints before the poly constructor copies the
   same complete writable record into its work state. */
effPCP_ThunderPolyParam_t effPCPShockWave02Param[1] __attribute__((aligned(16))) = {
    {
        .start = { 0.0f, -500.0f, 0.0f, 0.0f },
        .blend = 2,
        .number = 1,
        .restartFrameLimit = 1,
        .waveAmplitude = 10.0f,
        .startDelayRange = 4,
        .activeFrameRange = 10,
        .split = 10,
        .color_center = 0xFFB9FF87,
        .width_border = 1.5f,
        .color_border = 0xE16C5C00,
        .width_edge = 6.0f,
        .color_edge = 0x003A0200,
    },
};
