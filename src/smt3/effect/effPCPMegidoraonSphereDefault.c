#include "effThunderSphereData.h"

SIZE_CHECK(thunderSpherePresetSize, effPCPThunderSpherePolyParam_t, 0x4C);

effPCPThunderSpherePolyParam_t effPCP_Megidoraon01_ThunderSpherePoly_Param = {
    .origin = { 0x0p+0f, 0x0p+0f, 0x0p+0f, 0x0p+0f },
    .systemParam = 2, .pad12 = { 0x00, 0x00 },
    .cellCount = 8, .pad18 = { 0x01, 0x00, 0x00, 0x00 },
    .width = 0x1.1f8p+10f, .height = 0x1.1f8p+10f, .rotationScale = 0x1.333332p-3f,
    .startDelayRange = 8, .activeFrameRange = 8,
    .perCell = 13, .pad32 = { 0x00, 0x00 },
    .width_center = 0x1.470a3cp+1f, .color_center = 0xFFFFFFFF,
    .width_border = 0x1.066666p+0f, .color_border = 0xE1FF5640,
    .width_edge = 0x1.3p+2f, .color_edge = 0x00802000,
};
