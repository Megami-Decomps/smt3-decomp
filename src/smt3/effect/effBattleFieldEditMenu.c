#include "effDebug.h"

extern const char D_00533AA0[16];
extern effDebugMenuItem_t D_0031CAA8[17];
extern void *func_00175030(void *unused);
extern void eff_175070(void *unused);

/* Entering saves the battle light state; exiting restores it. */
effDebugMenu_t D_0031CC40 = {
    .title = D_00533AA0,
    .x = 16,
    .y = 16,
    .items = D_0031CAA8,
    .itemCount = 17,
    .mode = 1,
    .create = func_00175030,
    .destroy = eff_175070,
};
