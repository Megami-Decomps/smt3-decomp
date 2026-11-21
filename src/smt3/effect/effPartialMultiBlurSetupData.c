#include "effPartialMultiBlur.h"
#include "effDebug.h"

extern const char D_00534390[16];
extern effDebugMenuItem_t D_00320450[11];
extern dds3Effect_PartialMultiBlurParam_t mblurParam, D_0056B0B8;
extern effPartialMultiBlurEditorSlot_t D_0054F2C8;

SIZE_CHECK(effPartialMultiBlurSetupSize, effDebugMenu_t, 0x40);
SIZE_CHECK(effPartialMultiBlurBackupSize, dds3Effect_PartialMultiBlurParam_t, 0x2C);
SIZE_CHECK(effPartialMultiBlurEditorSlotSize, effPartialMultiBlurEditorSlot_t, 8);

/* Keep the live template distinct from the editor's saved reset copy. */
effDebugMenu_t mblurDebugMenu = {
    .title = D_00534390,
    .x = 18,
    .y = 18,
    .items = D_00320450,
    .itemCount = ARRAY_COUNT(D_00320450),
    .source = &mblurParam,
    .destination = &D_0056B0B8,
    .copyBytes = sizeof(mblurParam),
    .mode = 1,
    .create = dds3EffectPartialMultiBlurCreate,
    .update = dds3EffectPartialMultiBlurDraw,
    .destroy = dds3EffectPartialMultiBlurDelete,
    .param = &D_0054F2C8.handle,
    .recreate = dds3EffectPartialMultiBlurCreate,
};
