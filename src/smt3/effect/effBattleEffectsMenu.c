#include "effDebug.h"

extern const char D_00533B20[16];
extern effDebugMenuItem_t D_0031CC80[13];

/* The effects submenu is independent of its 13 raw menu items. */
effDebugMenu_t D_0031CDB8 = {
    .title = D_00533B20,
    .x = 16,
    .y = 16,
    .items = D_0031CC80,
    .itemCount = 13,
    .mode = 1,
};
