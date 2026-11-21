#include "effDebug.h"

extern const char nbCameraHelperPbSelectText[16];
extern char D_0054F3F8[6];
extern char D_0054F3F0[8];
extern u8 nbCameraHelperViewerShowFrame;
extern u8 nbCameraHelperViewerShowLines;

extern effDebugMenu_t nbCameraHelperViewerPBParam;

effDebugMenuItem_t D_00325A70[4] = {
    { .label = nbCameraHelperPbSelectText, .type = 22, .value = &nbCameraHelperViewerPBParam },
    { .type = 18 },
    { .label = D_0054F3F8, .type = 8, .value = &nbCameraHelperViewerShowFrame },
    { .label = D_0054F3F0, .type = 8, .value = &nbCameraHelperViewerShowLines },
};
