#include "effThunderSphereData.h"

SIZE_CHECK(thunderSpherePresetSize, effPCPThunderSpherePolyParam_t, 0x4C);

effPCPThunderSpherePolyParam_t effBTLSystem_Bind_ThunderSpherePolyLowParam = {
    .systemParam = 2,
    .cellCount = 6, .pad18 = { 1 },
    .width = 150.0f, .height = 150.0f, .rotationScale = 0x1.1eb85p-3f,
    .startDelayRange = 10, .activeFrameRange = 7,
    .perCell = 13,
    .width_center = 1.0f, .color_center = 0xFFFFFFFF,
    .width_border = 1.0f, .color_border = 0xFF1CFCDB,
    .width_edge = 1.0f, .color_edge = 0xFF1CFCDB,
};
