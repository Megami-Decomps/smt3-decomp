#include "effDebug.h"
#include "effRippleBlur.h"

SIZE_CHECK(effRippleBlurMenuSize, effDebugMenu_t, 0x40);
SIZE_CHECK(effRippleBlurBackupSize, dds3Effect_RippleBlurParam_t, 0x2C);

extern const char D_005343C0[16];
extern effDebugMenuItem_t D_003209E0[15];
extern dds3Effect_RippleBlurParam_t rblurParam;
extern dds3Effect_RippleBlurParam_t D_0056B158;
extern effRippleBlurEditorResource_t D_0054F320;

/* The editor saves the live parameters and updates its owned ripple resource. */
effDebugMenu_t rblurDebugMenu = {
    .title = D_005343C0,
    .x = 18,
    .y = 18,
    .items = D_003209E0,
    .itemCount = ARRAY_COUNT(D_003209E0),
    .source = &rblurParam,
    .destination = &D_0056B158,
    .copyBytes = sizeof(dds3Effect_RippleBlurParam_t),
    .mode = 1,
    .create = dds3EffectRippleBlurCreate,
    .update = dds3EffectRippleBlurDraw,
    .destroy = dds3EffectRippleBlurDelete,
    .param = &D_0054F320.handle,
    .recreate = dds3EffectRippleBlurCreate,
};
