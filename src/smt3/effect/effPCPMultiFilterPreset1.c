#include "effPCPFlashFilterData.h"

typedef char multiFilterPresetSize[(sizeof(effPCPMultiFilterExpParam_t) == 0x3C) ? 1 : -1];

effPCPMultiFilterExpParam_t D_0031DCA0 __attribute__((aligned(16))) = {
    .life = 268435455, .fade_in = 0, .fade_out = 0,
    .area_s = 270, .area_e = 270,
    .res = { .area = 270, .Filter = { { { 0x12, 0x4B, 0x9B, 0x19 } }, 72 } },
};
