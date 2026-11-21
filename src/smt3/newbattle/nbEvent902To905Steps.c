#include "newbattle.h"

extern s32 Pos1(nbEventProcessData_t *event);
extern s32 SetForm(nbEventProcessData_t *event);
extern s32 nbE902_Wait1(nbEventProcessData_t *event);
extern s32 SelectCopy(nbEventProcessData_t *event);
extern s32 Bunshin1(nbEventProcessData_t *event);
extern s32 Wait2(nbEventProcessData_t *event);
extern s32 Hontai(nbEventProcessData_t *event);
extern s32 nbE902_End(nbEventProcessData_t *event);
extern s32 Hatudo(nbEventProcessData_t *event);
extern s32 nbE903_Wait1(nbEventProcessData_t *event);
extern s32 Parts1(nbEventProcessData_t *event);
extern s32 PartsEffect1(nbEventProcessData_t *event);
extern s32 PartsWait(nbEventProcessData_t *event);
extern s32 PartsEffect2(nbEventProcessData_t *event);
extern s32 EffDel(nbEventProcessData_t *event);
extern s32 nbE903_End(nbEventProcessData_t *event);
extern s32 Test(nbEventProcessData_t *event);
extern s32 nbE904_End(nbEventProcessData_t *event);
extern s32 Model1Start(nbEventProcessData_t *event);
extern s32 Model1Wait(nbEventProcessData_t *event);
extern s32 Model1Wait_2(nbEventProcessData_t *event);
extern s32 nbE905_Model2Start(nbEventProcessData_t *event);
extern s32 func_002930E8(nbEventProcessData_t *event);
extern s32 nbE905_End(nbEventProcessData_t *event);

s32 (*nbE902Table[8])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    Pos1,
    SetForm,
    nbE902_Wait1,
    SelectCopy,
    Bunshin1,
    Wait2,
    Hontai,
    nbE902_End,
};

s32 (*nbE903Table[8])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    Hatudo,
    nbE903_Wait1,
    Parts1,
    PartsEffect1,
    PartsWait,
    PartsEffect2,
    EffDel,
    nbE903_End,
};

/* The opening position for formation slot five in event 904. */
sdfVector4_t D_00527490 = { -111.431727f, 0.0f, 394.814911f, 0.0f };

s32 (*nbE904Table[2])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    Test,
    nbE904_End,
};

s32 (*nbE905Table[7])(nbEventProcessData_t *event) __attribute__((section(".data"))) = {
    Model1Start,
    Model1Wait,
    Model1Wait_2,
    nbE905_Model2Start,
    func_002930E8,
    func_002930E8,
    nbE905_End,
};
