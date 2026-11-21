#include "effThunderPolyPresetData.h"

typedef char effPCP_ThunderRadiatePolyParam_tSize[(sizeof(effPCP_ThunderRadiatePolyParam_t) == 0x48) ? 1 : -1];

effPCP_ThunderRadiatePolyParam_t D_0031DB50 __attribute__((aligned(16))) = {
    .origin = { 0.0f, -500.0f, 0.0f, 0.0f }, .blend = 2,
    .pad12 = { 0x00, 0x00 }, .length = 500.0f,
    .number = 8, .opaque1C = { 0x01, 0x00, 0x00, 0x00, 0x34, 0xFA, 0x0E, 0x3D },
    .startDelayRange = 10, .activeFrameRange = 7,
    .split = 14, .pad2E = { 0x00, 0x00 },
    .coreWidth = 1.0f, .color_center = 0xFFFFFFFF,
    .bandWidth = 0.5f, .color_border = 0xFF802000,
    .edgeWidth = 10.0f, .color_edge = 0x00802000,
};
