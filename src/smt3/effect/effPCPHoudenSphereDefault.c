#include "effThunderSphereData.h"

SIZE_CHECK(thunderSpherePresetSize, effPCPThunderSpherePolyParam_t, 0x4C);

effPCPThunderSpherePolyParam_t effPCP_Houden01_ThunderSpherePoly_Param = {
    .origin = { 0x0p+0f, 0x0p+0f, 0x0p+0f, 0x0p+0f },
    .systemParam = 2, .pad12 = { 0x00, 0x00 },
    .cellCount = 9, .pad18 = { 0x01, 0x00, 0x00, 0x00 },
    .width = 0x1.9p+7f, .height = 0x1.9p+6f, .rotationScale = 0x1.999998p-3f,
    .startDelayRange = 1, .activeFrameRange = 24,
    .perCell = 11, .pad32 = { 0x00, 0x00 },
    .width_center = 0x1.733332p+0f, .color_center = 0x87E1B973,
    .width_border = 0x1.733332p+0f, .color_border = 0xE1802000,
    .width_edge = 0x1.2p+2f, .color_edge = 0x00760C00,
};
