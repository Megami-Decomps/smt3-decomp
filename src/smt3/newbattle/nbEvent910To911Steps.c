#include "newbattle.h"

extern s32 nbE910_Test(nbEventProcessData_t *event);
extern s32 nbE910_End(nbEventProcessData_t *event);
extern s32 Effect1(nbEventProcessData_t *event);
extern s32 Wait1(nbEventProcessData_t *event);
extern s32 Effect2(nbEventProcessData_t *event);
extern s32 nbE911_End(nbEventProcessData_t *event);

s32 (*D_00527518[2])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE910_Test,
    nbE910_End,
};

s32 (*nbE911Table[4])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    Effect1,
    Wait1,
    Effect2,
    nbE911_End,
};

/* Formation slots affected by the shared 911/912 sequence. */
s8 nbE911FormIndex[4] __attribute__((section(".data"))) = { 4, 5, 7, 8 };
