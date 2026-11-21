#ifndef EFF_DISTORTION_BLUR_H
#define EFF_DISTORTION_BLUR_H

#include "common.h"

/* Random-position blur parameters and their allocated slot owner. */
typedef struct dds3Effect_DistortionBlurParam_t {
    s32 number;
    s32 interval;
    f32 speed;
    u32 abgr;
    s32 blend;
    f32 rotate;
    f32 distortion;
    s32 center[2];
    s32 area;
    s32 width;
} dds3Effect_DistortionBlurParam_t;

/* The blur provider implements the shared editor callback ABI. */
void *dds3EffectDistortionBlurCreate(void *params);
void dds3EffectDistortionBlurDelete(void *resource);
void dds3EffectDistortionBlurDraw(void *resource);

#endif
