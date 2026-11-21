#include "effDebug.h"

extern char D_00533C00[];
extern u8 effDebugStatBit16;
extern char D_00533BF0[];
extern u8 effDebugStatBit17;
extern char D_00533BE0[];
extern u8 effDebugStatBit18;
extern char D_00533BD0[];
extern u8 effDebugStatBit19;
extern char D_00533BC0[];
extern u8 effDebugStatBit22;
extern char D_00533BB0[];
extern u8 effDebugStatBit25;
extern char D_00533BA0[];
extern u8 effDebugStatBit23;
extern char D_00533B90[];
extern u8 effDebugStatBit26;
extern char D_00533B80[];
extern u8 effDebugStatBit27;
extern char D_00533B70[];
extern u8 effDebugStatBit28;
extern char D_00533B60[];
extern u8 effDebugStatBit29;
extern char D_00533B50[];
extern u8 effDebugStatBit30;
extern char D_00533B40[];
extern s8 effBTLFieldColorOn;
extern char D_00533B30[];
extern s8 D_0054F0F1;

/* SYSTEM MENU menu entries. */
effDebugMenuItem_t D_0031CDF8[14] = {
    { .label = D_00533C00, .type = 8, .value = &effDebugStatBit16 },
    { .label = D_00533BF0, .type = 8, .value = &effDebugStatBit17 },
    { .label = D_00533BE0, .type = 8, .value = &effDebugStatBit18 },
    { .label = D_00533BD0, .type = 8, .value = &effDebugStatBit19 },
    { .label = D_00533BC0, .type = 8, .value = &effDebugStatBit22 },
    { .label = D_00533BB0, .type = 8, .value = &effDebugStatBit25 },
    { .label = D_00533BA0, .type = 8, .value = &effDebugStatBit23 },
    { .label = D_00533B90, .type = 8, .value = &effDebugStatBit26 },
    { .label = D_00533B80, .type = 8, .value = &effDebugStatBit27 },
    { .label = D_00533B70, .type = 8, .value = &effDebugStatBit28 },
    { .label = D_00533B60, .type = 8, .value = &effDebugStatBit29 },
    { .label = D_00533B50, .type = 8, .value = &effDebugStatBit30 },
    { .label = D_00533B40, .type = 8, .value = &effBTLFieldColorOn },
    { .label = D_00533B30, .type = 8, .value = &D_0054F0F1 },
};
