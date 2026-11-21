#include "effThunderPolyPresetData.h"

SIZE_CHECK(mziodinThunderParamSize, effPCP_ThunderPolyParam_t, 0x54);

/* Initialization mutates one of the two templates before creating a strip. */
effPCP_ThunderPolyParam_t effPCPMZiodin01ThunderParam[2] = {
    {
        .start = { 0.0f, -600.0f, 0.0f, 0.0f },
        .blend = 2,
        .number = 1,
        .waveAmplitude = 15.0f,
        .startDelayRange = 2,
        .activeFrameRange = 10,
        .split = 20,
        .color_center = 0xFFFFFFFF,
        .width_border = 1.5f,
        .color_border = 0xFF802000,
        .width_edge = 10.0f,
        .color_edge = 0x00802000,
    },
    {
        .start = { 0.0f, -600.0f, 0.0f, 0.0f },
        .blend = 2,
        .number = 1,
        .waveAmplitude = 15.0f,
        .startDelayRange = 2,
        .activeFrameRange = 10,
        .split = 20,
        .color_center = 0xFFFFFFFF,
        .width_border = 1.5f,
        .color_border = 0xFF802000,
        .width_edge = 10.0f,
        .color_edge = 0x00802000,
    },
};

effPCP_ThunderPolyParam_t effPCPMZiodin02ThunderParam[2] = {
    {
        .start = { 0.0f, -500.0f, 0.0f, 0.0f },
        .blend = 2,
        .number = 1,
        .restartFrameLimit = 30,
        .waveAmplitude = 20.0f,
        .startDelayRange = 7,
        .activeFrameRange = 7,
        .split = 20,
        .color_center = 0xFFFFD791,
        .width_border = 1.5f,
        .color_border = 0xFF802A00,
        .width_edge = 13.0f,
        .color_edge = 0x00801600,
    },
    {
        .start = { 0.0f, -500.0f, 0.0f, 0.0f },
        .blend = 2,
        .number = 1,
        .restartFrameLimit = 30,
        .waveAmplitude = 15.0f,
        .startDelayRange = 5,
        .activeFrameRange = 8,
        .split = 15,
        .color_center = 0xFFFFD791,
        .width_border = 3.0f,
        .color_border = 0xFF802A00,
        .width_edge = 13.0f,
        .color_edge = 0x00801600,
    },
};
