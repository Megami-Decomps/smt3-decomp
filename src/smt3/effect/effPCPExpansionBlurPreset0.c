#include "effPCPBlurPresetData.h"

typedef char pcpBlurPresetSize[(sizeof(effPCPBlinkMultiBlurParam_t) == 0x40) ? 1 : -1];

effPCPBlinkMultiBlurParam_t D_0031DBA0 __attribute__((aligned(16))) = {
    .count = 6, .colors = { 0x46766C6C, 0x5E766C6C },
    .scale = 1.5f, .minSize = 260,
    .res = {
        .area = 260,
        .Blur = {
            .abgr = 0x46766C6C, .blend = 72, .rotate = 0.010000000009313226f, .scale = 0.97999997437000275f,
            .center = { 0, 0 }, .xy = { { 0, 0 }, { 0, 0 } },
        },
    },
};
