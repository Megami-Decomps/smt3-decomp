#include "newbattle.h"

extern s32 nbE906_Test(nbEventProcessData_t *event);
extern s32 nbE906_End(nbEventProcessData_t *event);
extern s32 nbE907_Model1Start(nbEventProcessData_t *event);
extern s32 nbE907_Model1Wait(nbEventProcessData_t *event);
extern s32 nbE907_Model1Wait_2(nbEventProcessData_t *event);
extern s32 nbE907_Model2Start(nbEventProcessData_t *event);
extern s32 Model2Wait_1(nbEventProcessData_t *event);
extern s32 Model2Wait_15(nbEventProcessData_t *event);
extern s32 nbE907_End(nbEventProcessData_t *event);

s32 (*nbE906Table[2])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE906_Test,
    nbE906_End,
};

s32 (*nbE907Table[7])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE907_Model1Start,
    nbE907_Model1Wait,
    nbE907_Model1Wait_2,
    nbE907_Model2Start,
    Model2Wait_1,
    Model2Wait_15,
    nbE907_End,
};
