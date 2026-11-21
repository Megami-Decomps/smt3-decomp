#include "effDebug.h"

extern const char D_00533A40[16];
extern void *effMakeBedPath(void *path);
extern void eff_174D38(void *unused);
extern void eff_174D98(void *unused);

/* This file picker has no menu items; the native self pointer is not indexed. */
effDebugMenu_t D_0031C7D0 = {
    .title = D_00533A40,
    .x = 16,
    .y = 16,
    .items = (effDebugMenuItem_t *)&D_0031C7D0,
    .mode = 1,
    .create = effMakeBedPath,
    .update = eff_174D38,
    .destroy = eff_174D98,
};
