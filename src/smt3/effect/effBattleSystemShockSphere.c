#include "effThunderSphereData.h"

SIZE_CHECK(thunderSpherePresetSize, effPCPThunderSpherePolyParam_t, 0x4C);

effPCPThunderSpherePolyParam_t effBTLSystem_Shock_ThunderSpherePolyLowParam = {
    .systemParam = 2,
    .cellCount = 6, .pad18 = { 1 },
    .width = 100.0f, .height = 100.0f, .rotationScale = 0x1.eb851ep-4f,
    .startDelayRange = 1, .activeFrameRange = 7,
    .perCell = 9,
    .width_center = 0x1.ccccccp-1f, .color_center = 0xFFFFFFFF,
    .width_border = 0x1.666666p-1f, .color_border = 0xFFF85C28,
    .width_edge = 0x1.666666p-1f, .color_edge = 0xFFF85C28,
};
