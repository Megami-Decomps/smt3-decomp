#include "effect.h"
#include "effDebug.h"

extern dds3Effect_PartialMultiBlurParam_t mblurParam;
extern char effLabelArea[5];
extern char effLabelRotate[8], effLabelScale[8], D_0054F2A0[8];
extern char D_0054F298[8], D_0054F290[8], D_0054F288[8], D_0054F280[8];
extern const char D_00534380[16], D_00534370[16];

SIZE_CHECK(effMultiBlurEditorItemSize, effDebugMenuItem_t, 0x18);

/* The partial multi-blur editor updates the live blur parameter prefix. */
effDebugMenuItem_t D_00320450[11] = {
    { .label = effLabelRotate, .type = 7, .value = &mblurParam.Blur.rotate, .step = 1 },
    { .label = effLabelScale, .type = 7, .value = &mblurParam.Blur.scale, .step = 1 },
    { .label = D_0054F2A0, .type = 13, .value = &mblurParam.Blur.blend, .step = 1 },
    { .label = D_0054F298, .type = 9, .value = &mblurParam.Blur.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F290, .type = 10, .value = &mblurParam.Blur.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F288, .type = 11, .value = &mblurParam.Blur.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F280, .type = 12, .value = &mblurParam.Blur.abgr, .maxValue = 255, .step = 1 },
    { effLabelArea, 20, { 0 }, &mblurParam.area, { mblurParam.Blur.center }, 0, 1 },
    { D_00534380, 16, { 0 }, mblurParam.Blur.center, { &mblurParam.area }, 0, 1 },
    { .type = 18 },
    { .label = D_00534370, .type = 21 },
};
