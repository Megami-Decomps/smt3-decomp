#include "newbattle.h"

extern s32 nbE913_Test(nbEventProcessData_t *event);
extern s32 nbE913_End(nbEventProcessData_t *event);
extern s32 nbE914_Test(nbEventProcessData_t *event);
extern s32 nbE914_End(nbEventProcessData_t *event);
extern s32 nbE915_Test(nbEventProcessData_t *event);
extern s32 nbE915_End(nbEventProcessData_t *event);

s32 (*D_00527688[2])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE913_Test,
    nbE913_End,
};

s32 (*nbE914Table[2])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE914_Test,
    nbE914_End,
};

s32 (*D_00527698[2])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE915_Test,
    nbE915_End,
};
