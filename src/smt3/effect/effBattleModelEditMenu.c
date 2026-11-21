#include "effDebug.h"

extern const char D_00533A90[16];
extern effDebugMenuItem_t D_0031C810[24];
extern void *func_00174DC0(void *unused);
extern void eff_174E88(void *unused);

/* Both live callbacks have the exact editor dispatcher prototypes. */
effDebugMenu_t D_0031CA50 = {
    .title = D_00533A90,
    .x = 16,
    .y = 16,
    .items = D_0031C810,
    .itemCount = 24,
    .mode = 1,
    .create = func_00174DC0,
    .update = eff_174E88,
};
