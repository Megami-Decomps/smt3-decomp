#include "effPCPFlashFilterData.h"

typedef char multiFilterPresetSize[(sizeof(effPCPMultiFilterExpParam_t) == 0x3C) ? 1 : -1];

effPCPMultiFilterExpParam_t D_0031DC60 __attribute__((aligned(16))) = {
    .life = 30, .fade_in = 8, .fade_out = 8,
    .area_s = 250, .area_e = 250,
    .res = { .area = 250, .Filter = { { { 0xF8, 0x58, 0x12, 0x28 } }, 72 } },
};
