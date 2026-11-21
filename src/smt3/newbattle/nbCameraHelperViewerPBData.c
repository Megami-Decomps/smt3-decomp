#include "effDebug.h"

SIZE_CHECK(nbCameraHelperViewerPBDataSize, effDebugMenu_t, 0x40);
extern const char nbCameraHelperBedSelectText[16];
extern void *nbCameraHelperViewer_PB_Init(void *context);
extern void nbCameraHelperViewer_PB_Draw(void *context);
extern void nbCameraHelperViewer_PB_End(void *context);

/* This file selector has no item rows; its update callback draws the browser. */
effDebugMenu_t nbCameraHelperViewerPBParam = {
    .title = nbCameraHelperBedSelectText,
    .x = 16,
    .y = 16,
    .items = (effDebugMenuItem_t *)&nbCameraHelperViewerPBParam,
    .mode = 1,
    .create = nbCameraHelperViewer_PB_Init,
    .update = nbCameraHelperViewer_PB_Draw,
    .destroy = nbCameraHelperViewer_PB_End,
};
