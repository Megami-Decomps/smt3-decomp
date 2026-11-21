#include "effDebug.h"

extern const char D_00533A40[16];
extern const char D_00533A90[16];
extern const char D_00533B10[16];
extern const char D_00533B00[16];
extern const char D_00533AF0[16];
extern const char D_00533AE0[16];
extern const char D_00533AD0[16];
extern const char D_00533AC0[16];
extern char D_0054F1C8[8];
extern const char D_00533AB0[16];

extern effDebugMenu_t D_0031C7D0;
extern effDebugMenu_t D_0031CA50;
extern effDebugMenu_t D_0031CC40;
/* The primary Camera Helper submenu descriptor. */
extern effDebugMenu_t D_00325AD0;

extern u8 D_0054F1BE;
extern u8 D_0054F1BF;
extern u8 D_0054F1C0;
extern u8 effBTLDebugCameraAllEdit;
extern u8 effBTLDebugCameraAllTable;
extern u8 D_0054F1C3;

enum { EFF_DEBUG_BOOLEAN = 8, EFF_DEBUG_SEPARATOR = 18, EFF_DEBUG_SUBMENU = 22 };
SIZE_CHECK(effBattleEffectDebugItemSize, effDebugMenuItem_t, 0x18);

/* EFFECT debug menu items, in native order. */
effDebugMenuItem_t D_0031CC80[13] = {
    { .label = D_00533A40, .type = EFF_DEBUG_SUBMENU, .value = &D_0031C7D0 },
    { .type = EFF_DEBUG_SEPARATOR },
    { .label = D_00533A90, .type = EFF_DEBUG_SUBMENU, .value = &D_0031CA50 },
    { .label = D_00533B10, .type = EFF_DEBUG_SUBMENU, .value = &D_0031CC40 },
    { .type = EFF_DEBUG_SEPARATOR },
    { .label = D_00533B00, .type = EFF_DEBUG_BOOLEAN, .value = &D_0054F1BE },
    { .label = D_00533AF0, .type = EFF_DEBUG_BOOLEAN, .value = &D_0054F1BF },
    { .label = D_00533AE0, .type = EFF_DEBUG_BOOLEAN, .value = &D_0054F1C0 },
    { .label = D_00533AD0, .type = EFF_DEBUG_BOOLEAN, .value = &effBTLDebugCameraAllEdit },
    { .label = D_00533AC0, .type = EFF_DEBUG_BOOLEAN, .value = &effBTLDebugCameraAllTable },
    { .label = D_0054F1C8, .type = EFF_DEBUG_BOOLEAN, .value = &D_0054F1C3 },
    { .label = D_00533AB0, .type = EFF_DEBUG_SUBMENU, .value = &D_00325AD0 },
    { .type = EFF_DEBUG_SEPARATOR },
};
