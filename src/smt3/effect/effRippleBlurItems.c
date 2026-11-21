#include "effect.h"
#include "effDebug.h"

extern char effLabelArea[5];
extern char D_0054F298[8], D_0054F290[8], D_0054F288[8], D_0054F280[8];
extern const char D_00534380[16], D_00534370[16];

extern char effLabelNumber[7], effLabelIntval[8], effLabelSpeed[8];
extern char effLabelRotate[8], effLabelDistor[8], effLabelDistS[7], effLabelBlend[8];
extern dds3Effect_RippleBlurParam_t rblurParam;

SIZE_CHECK(effRippleBlurEditorItemSize, effDebugMenuItem_t, 0x18);

/* Ripple-blur controls edit the live parameters referenced by rblurDebugMenu. */
effDebugMenuItem_t D_003209E0[15] = {
    { .label = effLabelNumber, .type = 5, .value = &rblurParam.number, .maxValue = 40, .step = 1 },
    { .label = effLabelSpeed, .type = 7, .value = &rblurParam.speed, .step = 1 },
    { .label = effLabelIntval, .type = 7, .value = &rblurParam.interval, .step = 1 },
    { .label = effLabelRotate, .type = 7, .value = &rblurParam.rotate, .step = 1 },
    { .label = effLabelDistor, .type = 7, .value = &rblurParam.power, .step = 1 },
    { .label = effLabelDistS, .type = 7, .value = &rblurParam.frequency, .step = 1 },
    { .label = effLabelBlend, .type = 13, .value = &rblurParam.blend, .step = 1 },
    { .label = D_0054F298, .type = 9, .value = &rblurParam.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F290, .type = 10, .value = &rblurParam.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F288, .type = 11, .value = &rblurParam.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F280, .type = 12, .value = &rblurParam.abgr, .maxValue = 255, .step = 1 },
    { effLabelArea, 20, { 0 }, &rblurParam.area, { rblurParam.center }, 0, 1 },
    { D_00534380, 16, { 0 }, rblurParam.center, { &rblurParam.area }, 0, 1 },
    { .type = 18 },
    { .label = D_00534370, .type = 21 },
};
