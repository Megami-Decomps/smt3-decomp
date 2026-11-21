#include "effThunderSphereData.h"

SIZE_CHECK(thunderSpherePresetSize, effPCPThunderSpherePolyParam_t, 0x4C);

effPCPThunderSpherePolyParam_t effPCP_ShockWave01_ThunderSpherePoly_Param = {
    .origin = { 0x0p+0f, 0x0p+0f, 0x0p+0f, 0x0p+0f },
    .systemParam = 2, .pad12 = { 0x00, 0x00 },
    .cellCount = 9, .pad18 = { 0x01, 0x00, 0x00, 0x00 },
    .width = 0x1.9p+9f, .height = 0x1.9p+8f, .rotationScale = 0x1.333332p-4f,
    .startDelayRange = 1, .activeFrameRange = 20,
    .perCell = 39, .pad32 = { 0x00, 0x00 },
    .width_center = 0x1.470a3cp+1f, .color_center = 0xFFEBFFC3,
    .width_border = 0x1.8p-1f, .color_border = 0xE16C5200,
    .width_edge = 0x1.cp+1f, .color_edge = 0x003A0200,
};
