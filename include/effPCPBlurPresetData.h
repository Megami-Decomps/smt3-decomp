#ifndef EFF_PCP_BLUR_PRESET_DATA_H
#define EFF_PCP_BLUR_PRESET_DATA_H

#include "effect.h"

/* Shared input to the expansion and blink multi-blur constructors. */
typedef struct effPCPBlinkMultiBlurParam_t {
    u32 count;
    u32 colors[2];
    f32 scale;
    s32 minSize;
    dds3Effect_PartialMultiBlurParam_t res;
} effPCPBlinkMultiBlurParam_t;

extern effPCPBlinkMultiBlurParam_t D_0031DBA0;
extern effPCPBlinkMultiBlurParam_t D_0031DBE0;
extern effPCPBlinkMultiBlurParam_t D_0031DCE0;

#endif
