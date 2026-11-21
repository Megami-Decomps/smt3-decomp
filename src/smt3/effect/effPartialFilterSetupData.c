#include "effect.h"
#include "effDebug.h"

SIZE_CHECK(effPartialFilterSetupSize, effDebugMenu_t, 0x40);
SIZE_CHECK(effPartialFilterBackupSize, dds3Effect_PartialMultiFilterParam_t, 0x24);
extern const char D_005343B0[];
extern effDebugMenuItem_t D_00320898[9];
extern dds3Effect_PartialMultiFilterParam_t mfilterParam;
extern u8 D_0056B130[0x28];
extern void *D_0054F318;
extern void *dds3EffectPartialMultiFilterCreate(void *params);
extern void dds3EffectPartialMultiFilterDraw(void *work);
extern void dds3EffectPartialMultiFilterDelete(void *work);

/* The live slot is filled by create and reused by draw, destroy, and recreate. */
effDebugMenu_t mfilterDebugMenu = {
    .title = D_005343B0,
    .x = 18,
    .y = 18,
    .items = D_00320898,
    .itemCount = ARRAY_COUNT(D_00320898),
    .source = &mfilterParam,
    .destination = D_0056B130,
    .copyBytes = sizeof(dds3Effect_PartialMultiFilterParam_t),
    .mode = 1,
    .create = dds3EffectPartialMultiFilterCreate,
    .update = dds3EffectPartialMultiFilterDraw,
    .destroy = dds3EffectPartialMultiFilterDelete,
    .param = &D_0054F318,
    .recreate = dds3EffectPartialMultiFilterCreate,
};
