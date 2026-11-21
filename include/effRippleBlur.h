#ifndef EFF_RIPPLE_BLUR_H
#define EFF_RIPPLE_BLUR_H

#include "common.h"

/* Phase-driven blur parameters and the scale-slot allocation they own. */
typedef struct dds3Effect_RippleBlurParam_t {
    s32 number;
    f32 speed;
    f32 interval;
    u32 abgr;
    s32 blend;
    f32 rotate;
    f32 power;
    f32 frequency;
    s32 center[2];
    s32 area;
} dds3Effect_RippleBlurParam_t;

/* Native editor storage contains one handle and a second unclassified word. */
typedef struct effRippleBlurEditorResource_t {
    void *handle;
    u32 unknownWord;
} effRippleBlurEditorResource_t;
SIZE_CHECK(effRippleBlurEditorResourceSize, effRippleBlurEditorResource_t, 8);

/* The ripple provider implements the shared editor callback ABI. */
void *dds3EffectRippleBlurCreate(void *params);
void dds3EffectRippleBlurDelete(void *resource);
void dds3EffectRippleBlurDraw(void *resource);

#endif
