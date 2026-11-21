#include "effect.h"
#include "effDebug.h"

SIZE_CHECK(effFilterSetupSize, effDebugMenu_t, 0x40);
SIZE_CHECK(effFilterBackupSize, dds3Effect_Filter_t, 0x18);
extern char effLabelFilter[7];
extern effDebugMenuItem_t D_00320788[7];
extern dds3Effect_Filter_t dblurParam2;
extern dds3Effect_Filter_t D_0056B118;
extern void *filterDebugCreateArgs[2];
extern void dds3EffectFilter(void *filter);

/* The filter editor saves its live rectangle and rebuilds it with the drawing callback. */
effDebugMenu_t filterDebugMenu = {
    .title = effLabelFilter,
    .x = 18,
    .y = 18,
    .items = D_00320788,
    .itemCount = ARRAY_COUNT(D_00320788),
    .source = &dblurParam2,
    .destination = &D_0056B118,
    .copyBytes = sizeof(dds3Effect_Filter_t),
    .mode = 1,
    .update = dds3EffectFilter,
    .param = filterDebugCreateArgs,
};
