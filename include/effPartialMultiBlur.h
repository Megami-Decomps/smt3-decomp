#ifndef EFF_PARTIAL_MULTI_BLUR_H
#define EFF_PARTIAL_MULTI_BLUR_H

#include "common.h"

/* Shared rectangle and sampling payload, copied as 0x28 bytes. */
typedef struct dds3Effect_Blur_t {
    u32 abgr;
    s32 blend;
    f32 rotate;
    f32 scale;
    s32 center[2];
    s32 xy[2][2];
} dds3Effect_Blur_t;

/* A copied 0x2C-byte body followed by its separately acquired texture. */
typedef struct dds3Effect_PartialMultiBlurParam_t {
    s32 area;
    dds3Effect_Blur_t Blur;
} dds3Effect_PartialMultiBlurParam_t;

typedef struct effBlurTemplate_t {
    dds3Effect_PartialMultiBlurParam_t body;
    struct sdfTexHandle_t *texture;
} effBlurTemplate_t;

/* The editor calls these three entry points through generic callbacks. */
void *dds3EffectPartialMultiBlurCreate(void *params);
void dds3EffectPartialMultiBlurDraw(void *owner);
void dds3EffectPartialMultiBlurDelete(void *owner);
void dds3EffectPartialMultiBlurSetParam(effBlurTemplate_t *owner,
    const dds3Effect_PartialMultiBlurParam_t *params);

typedef struct effPartialMultiBlurEditorSlot_t {
    void *handle;
    u32 reserved;
} effPartialMultiBlurEditorSlot_t;

#endif
