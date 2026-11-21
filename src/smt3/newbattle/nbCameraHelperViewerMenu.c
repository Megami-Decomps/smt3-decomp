#include "effDebug.h"

SIZE_CHECK(nbCameraHelperViewerMenuSize, effDebugMenu_t, 0x40);
extern const char nbCameraHelperCameraHelperText[16];
extern effDebugMenuItem_t D_00325A70[4];
extern void *nbCameraHelperViewer_Init(void *context);
extern void nbCameraHelperViewer_Draw(void *context);
extern void nbCameraHelperViewer_End(void *context);

/* Use the same viewer runner for entering the submenu and reloading its clip. */
effDebugMenu_t D_00325AD0 = {
    .title = nbCameraHelperCameraHelperText,
    .x = 16,
    .y = 16,
    .items = D_00325A70,
    .itemCount = ARRAY_COUNT(D_00325A70),
    .mode = 1,
    .create = nbCameraHelperViewer_Init,
    .update = nbCameraHelperViewer_Draw,
    .destroy = nbCameraHelperViewer_End,
    .recreate = nbCameraHelperViewer_Init,
};
