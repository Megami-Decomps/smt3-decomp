#include "newbattle.h"

extern s32 nbE912_Hatudo(nbEventProcessData_t *event);
extern s32 nbE912_Wait1(nbEventProcessData_t *event);
extern s32 Move1(nbEventProcessData_t *event);
extern s32 nbE912_Effect1(nbEventProcessData_t *event);
extern s32 nbE912_End(nbEventProcessData_t *event);

s32 (*nbE912Table[5])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE912_Hatudo,
    nbE912_Wait1,
    Move1,
    nbE912_Effect1,
    nbE912_End,
};
