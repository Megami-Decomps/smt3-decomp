#include "effect.h"
#include "effDebug.h"
#include "effRectangleBlur.h"

SIZE_CHECK(effRectangleBlurSetupSize, effDebugMenu_t, 0x40);
SIZE_CHECK(effRectangleBlurBackupSize, dds3Effect_Blur_t, 0x28);

extern char effLabelBlur[5];
extern effDebugMenuItem_t D_003202F0[10];
extern effScreenDrawParams_t evtUnityCalcArea;
extern dds3Effect_Blur_t D_0056B090;
extern void *effRectangleBlurSetupCreateArgs[2];

/* The editor saves the initial rectangle before its mode-one callback runs. */
effDebugMenu_t effRectangleBlurSetupRecord = {
    .title = effLabelBlur,
    .x = 18,
    .y = 18,
    .items = D_003202F0,
    .itemCount = ARRAY_COUNT(D_003202F0),
    .source = &evtUnityCalcArea.source,
    .destination = &D_0056B090,
    .copyBytes = sizeof(dds3Effect_Blur_t),
    .mode = 1,
    .update = dds3EffectBlur,
    .param = effRectangleBlurSetupCreateArgs,
};
