#include "newbattle.h"

extern s32 nbE908_Test(nbEventProcessData_t *event);
extern s32 nbE908_End(nbEventProcessData_t *event);

s32 (*nbE908Table[2])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE908_Test,
    nbE908_End,
};
