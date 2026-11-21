#include "effect.h"
#include "effDebug.h"

extern char effLabelArea[5];
extern char D_0054F298[8], D_0054F290[8], D_0054F288[8], D_0054F280[8];
extern const char D_00534380[16], D_00534370[16];

extern char D_0054F2A0[8];
extern dds3Effect_PartialMultiFilterParam_t mfilterParam;

SIZE_CHECK(effPartialMultiFilterEditorItemSize, effDebugMenuItem_t, 0x18);

/* Multi-filter controls share the live packed color, blend, and area geometry. */
effDebugMenuItem_t D_00320898[9] = {
    { .label = D_0054F2A0, .type = 14, .value = &mfilterParam.Filter.blendControl, .step = 1 },
    { .label = D_0054F298, .type = 9, .value = &mfilterParam.Filter.rgba, .maxValue = 255, .step = 1 },
    { .label = D_0054F290, .type = 10, .value = &mfilterParam.Filter.rgba, .maxValue = 255, .step = 1 },
    { .label = D_0054F288, .type = 11, .value = &mfilterParam.Filter.rgba, .maxValue = 255, .step = 1 },
    { .label = D_0054F280, .type = 12, .value = &mfilterParam.Filter.rgba, .maxValue = 255, .step = 1 },
    { effLabelArea, 20, { 0 }, &mfilterParam.area, { mfilterParam.center }, 0, 1 },
    { D_00534380, 16, { 0 }, mfilterParam.center, { &mfilterParam.area }, 0, 1 },
    { .type = 18 },
    { .label = D_00534370, .type = 21 },
};
