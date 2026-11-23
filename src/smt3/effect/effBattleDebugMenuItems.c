#include "effDebug.h"

extern char D_0054F1D8[7];
extern char D_0054F1D0[8];
extern u32 D_0054F168[2];
extern effDebugMenu_t D_0031CF48;
extern effDebugMenu_t D_0031CDB8;

enum {
    EFF_DEBUG_MENU_SEPARATOR = 18,
    EFF_DEBUG_MENU_HEADER = 19,
    EFF_DEBUG_MENU_SUBMENU = 22,
};

/* The distance update replaces the initial empty labels in slots 3 and 4. */
effDebugMenuItem_t effBTLDebugMenuItems[5] = {
    { .label = D_0054F1D8, .type = EFF_DEBUG_MENU_SUBMENU, .value = &D_0031CF48 },
    { .label = D_0054F1D0, .type = EFF_DEBUG_MENU_SUBMENU, .value = &D_0031CDB8 },
    { .type = EFF_DEBUG_MENU_SEPARATOR },
    { .label = (const char *)D_0054F168, .type = EFF_DEBUG_MENU_HEADER },
    { .label = (const char *)D_0054F168, .type = EFF_DEBUG_MENU_HEADER },
};
