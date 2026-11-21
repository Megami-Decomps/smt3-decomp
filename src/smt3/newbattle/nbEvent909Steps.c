#include "newbattle.h"

extern s32 nbE909_Model1Start(nbEventProcessData_t *event);
extern s32 nbE909_Model1Wait(nbEventProcessData_t *event);
extern s32 nbE909_Model1Wait_2(nbEventProcessData_t *event);
extern s32 Model2Start(nbEventProcessData_t *event);
extern s32 nbE909_Model2Wait_1(nbEventProcessData_t *event);
extern s32 nbE909_Model2Wait_15(nbEventProcessData_t *event);
extern s32 nbE909_End(nbEventProcessData_t *event);

s32 (*nbE909Table[7])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    nbE909_Model1Start,
    nbE909_Model1Wait,
    nbE909_Model1Wait_2,
    Model2Start,
    nbE909_Model2Wait_1,
    nbE909_Model2Wait_15,
    nbE909_End,
};
