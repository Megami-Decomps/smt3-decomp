#include "common.h"
#include "kernel.h"
#include "libsdf.h"
#include "scr.h"
#include "model.h"
#include "draw3d.h"
#include "ps2asm.h"

extern u8 keyActMode;

extern u8 keyAct[];

extern u8 pbRotLKeyAct(void);

extern u8 pbRotRKeyAct(void);

u8 pbGetKeyAction(u8 action) {
    return keyAct[action + (s8)keyActMode * 5];
}

u8 pbGetKeyMode(void) {
    return keyActMode;
}

void func_001DB0D8(u8 mode) {
    keyActMode = mode;
}

u8 pbRotRKeyAct(void) {
    return keyActMode = ((s8)keyActMode + 5) % 4;
}

u8 pbRotLKeyAct(void) {
    return keyActMode = ((s8)keyActMode + 3) % 4;
}
