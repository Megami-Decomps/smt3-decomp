#include "effThunderSphereData.h"

SIZE_CHECK(thunderSpherePresetSize, effPCPThunderSpherePolyParam_t, 0x4C);

effPCPThunderSpherePolyParam_t effBTLSystem_Paralyze_ThunderSpherePolyLowParam = {
    .systemParam = 2,
    .cellCount = 6, .pad18 = { 1 },
    .width = 100.0f, .height = 100.0f, .rotationScale = 0x1.1eb85p-3f,
    .startDelayRange = 1, .activeFrameRange = 7,
    .perCell = 15,
    .width_center = 0x1.0a3d7p-4f, .color_center = 0xFFFFFFFF,
    .width_border = 0x1.333332p-1f, .color_border = 0xFF1C3BCD,
    .width_edge = 0x1.333332p-1f, .color_edge = 0xFF1C3BCD,
};
