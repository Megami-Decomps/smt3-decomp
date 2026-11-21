#include "effect.h"
#include "effDebug.h"

extern effScreenDrawParams_t evtUnityCalcArea;

/* Original raw labels within the live rectangle-blur parameter prefix. */
extern s32 D_003202CC;
extern f32 D_003202D0, D_003202D4;
extern struct {
    s32 center[2];
    s32 xy[2][2];
} D_003202D8;
extern char effLabelRotate[8], effLabelScale[8], D_0054F2A0[8];
extern char D_0054F298[8], D_0054F290[8], D_0054F288[8], D_0054F280[8];
extern const char D_00534380[16], D_00534370[16];

SIZE_CHECK(effRectangleBlurEditorItemSize, effDebugMenuItem_t, 0x18);

/* The rectangle blur editor updates the live blur parameter prefix. */
effDebugMenuItem_t D_003202F0[10] = {
    { .label = effLabelRotate, .type = 7, .value = &D_003202D0, .step = 1 },
    { .label = effLabelScale, .type = 7, .value = &D_003202D4, .step = 1 },
    { .label = D_0054F2A0, .type = 13, .value = &D_003202CC, .step = 1 },
    { .label = D_0054F298, .type = 9, .value = &evtUnityCalcArea.source.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F290, .type = 10, .value = &evtUnityCalcArea.source.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F288, .type = 11, .value = &evtUnityCalcArea.source.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F280, .type = 12, .value = &evtUnityCalcArea.source.abgr, .maxValue = 255, .step = 1 },
    { .label = D_00534380, .type = 16, .value = D_003202D8.center, .step = 1 },
    { .type = 18 },
    { .label = D_00534370, .type = 21 },
};
