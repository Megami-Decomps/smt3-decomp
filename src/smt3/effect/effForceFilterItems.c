#include "effect.h"
#include "effDebug.h"

extern char D_0054F2A0[8];
extern char D_0054F298[8], D_0054F290[8], D_0054F288[8], D_0054F280[8];
extern const char D_00534370[16];

/* The raw setup block has separate linked labels at its color and blend words. */
extern dds3Effect_Filter_t dblurParam2;
extern struct {
    s32 blend;
    s32 xy[2][2];
} D_00320774;

SIZE_CHECK(effForceFilterEditorItemSize, effDebugMenuItem_t, 0x18);

effDebugMenuItem_t D_00320788[7] = {
    { .label = D_0054F2A0, .type = 14, .value = &D_00320774.blend, .step = 1 },
    { .label = D_0054F298, .type = 9, .value = &dblurParam2.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F290, .type = 10, .value = &dblurParam2.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F288, .type = 11, .value = &dblurParam2.abgr, .maxValue = 255, .step = 1 },
    { .label = D_0054F280, .type = 12, .value = &dblurParam2.abgr, .maxValue = 255, .step = 1 },
    { .type = 18 },
    { .label = D_00534370, .type = 21 },
};
