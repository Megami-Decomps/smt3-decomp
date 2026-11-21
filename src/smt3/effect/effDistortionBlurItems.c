#include "effect.h"
#include "effDebug.h"

extern char effLabelArea[5];
extern char D_0054F298[8], D_0054F290[8], D_0054F288[8], D_0054F280[8];
extern const char D_00534380[16], D_00534370[16];

extern char effLabelNumber[7], effLabelWidth[8], effLabelIntval[8], effLabelSpeed[8];
extern char effLabelRotate[8], effLabelDistor[8], effLabelBlend[8];
extern dds3Effect_DistortionBlurParam_t dblurParam;

SIZE_CHECK(effDistortionBlurEditorItemSize, effDebugMenuItem_t, 0x18);

/* Distortion-blur controls edit the live parameters referenced by dblurDebugMenu. */
effDebugMenuItem_t D_003205C8[15] = {
    { .label = effLabelNumber, .type = 5, .value = &dblurParam.number, .maxValue = 100, .step = 1 },
    { .label = effLabelWidth, .type = 4, .value = &dblurParam.width, .step = 1 },
    { .label = effLabelIntval, .type = 5, .value = &dblurParam.interval, .step = 1 },
    { .label = effLabelSpeed, .type = 7, .value = &dblurParam.speed, .step = 1 },
    { .label = effLabelRotate, .type = 7, .value = &dblurParam.rotate, .step = 1 },
    { .label = effLabelDistor, .type = 7, .value = &dblurParam.distortion, .step = 1 },
    { .label = effLabelBlend, .type = 13, .value = &dblurParam.blend, .step = 1 },
    { .label = D_0054F298, .type = 9, .value = &dblurParam.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F290, .type = 10, .value = &dblurParam.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F288, .type = 11, .value = &dblurParam.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F280, .type = 12, .value = &dblurParam.abgr, .maxValue = 255, .step = 1 },
    { effLabelArea, 20, { 0 }, &dblurParam.area, { dblurParam.center }, 0, 1 },
    { D_00534380, 16, { 0 }, dblurParam.center, { &dblurParam.area }, 0, 1 },
    { .type = 18 },
    { .label = D_00534370, .type = 21 },
};
