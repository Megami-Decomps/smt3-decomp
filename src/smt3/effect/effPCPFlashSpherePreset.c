#include "effPCPFlashFilterData.h"

typedef char flashSpherePresetSize[(sizeof(effPCPFlashPolySphereParam_t) == 0x40) ? 1 : -1];

effPCPFlashPolySphereParam_t D_0031DC20 __attribute__((aligned(16))) = {
    .number = 100, .lifetime = 35, .scaleRampTime = 50,
    .abgr_in = 0x005A7878, .abgr_out = 0x00000A1E,
    .width_in = 30.0f, .width_out = 11.0f,
    .radius = 150.0f, .rotate = 0.14999999478459358f, .blend = 2,
};
