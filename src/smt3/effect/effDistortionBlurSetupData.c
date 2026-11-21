#include "effDebug.h"
#include "effDistortionBlur.h"

SIZE_CHECK(effDistortionBlurMenuSize, effDebugMenu_t, 0x40);
SIZE_CHECK(effDistortionBlurBackupSize, dds3Effect_DistortionBlurParam_t, 0x2C);

extern const char D_005343A0[16];
extern effDebugMenuItem_t D_003205C8[15];
extern dds3Effect_DistortionBlurParam_t dblurParam;
extern dds3Effect_DistortionBlurParam_t D_0056B0E8;
extern void *D_00551DF8;

/* The editor saves the live parameters, then creates or updates the resource. */
effDebugMenu_t dblurDebugMenu = {
    .title = D_005343A0,
    .x = 18,
    .y = 18,
    .items = D_003205C8,
    .itemCount = ARRAY_COUNT(D_003205C8),
    .source = &dblurParam,
    .destination = &D_0056B0E8,
    .copyBytes = sizeof(dds3Effect_DistortionBlurParam_t),
    .mode = 1,
    .create = dds3EffectDistortionBlurCreate,
    .update = dds3EffectDistortionBlurDraw,
    .destroy = dds3EffectDistortionBlurDelete,
    .param = &D_00551DF8,
    .recreate = dds3EffectDistortionBlurCreate,
};
