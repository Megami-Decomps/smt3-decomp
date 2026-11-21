#include "newbattle.h"

extern s32 RunUgou(nbEventProcessData_t *event);
extern s32 Wait1_nbE901(nbEventProcessData_t *event);
extern s32 SetUp1(nbEventProcessData_t *event);
extern s32 SyuGo(nbEventProcessData_t *event);
extern s32 SyuGo2(nbEventProcessData_t *event);
extern s32 Cls1(nbEventProcessData_t *event);
extern s32 Daisupe(nbEventProcessData_t *event);
extern s32 ReadWait2(nbEventProcessData_t *event);
extern s32 Wait2_nbE901(nbEventProcessData_t *event);
extern s32 Syutugen2(nbEventProcessData_t *event);
extern s32 End(nbEventProcessData_t *event);

s32 (*D_00527420[11])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    RunUgou,
    Wait1_nbE901,
    SetUp1,
    SyuGo,
    SyuGo2,
    Cls1,
    Daisupe,
    ReadWait2,
    Wait2_nbE901,
    Syutugen2,
    End,
};
