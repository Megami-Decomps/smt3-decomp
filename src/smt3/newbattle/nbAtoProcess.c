#include "common.h"
#include "nb.h"
#include "effBattle.h"
#include "newdata.h"

nbMainProcessData_t *nbGetMainProcessData(void);

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer,
                                         dds3ProcessFunc_t processFunc,
                                         dds3ProcessFunc_t shutdownFunc, void *workData);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern s32 nbAtoProcess1(dds3ProcessID_t *id);
extern char D_005510C8[];
extern u32 dds3GetProcessRunTime(dds3ProcessID_t *id);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frame);
extern void nbSetDevilNameMes(s32 hmsg, s32 no, datUnitWork_t *work);
extern void nbDispMsgHelp(s32 msgid, s32 no, s32 msgtype);
extern void nbCameraSetState(u16 state);

s32 nbAtoStoneProcess(dds3ProcessID_t *id)
{
    s32 runtime = dds3GetProcessRunTime(id);
    nbMainProcessData_t *data = nbGetMainProcessData();

    switch (runtime) {
    case 10:
        nbCameraSetState(0x14);
        return 0;
    case 30:
        nbSetDevilNameMes(data->mesmngid, 0, (datUnitWork_t *)((u8 *)DDS3_GBWK + 0xA54));
        nbDispMsgHelp(data->mesmngid, 0x3A, 1);
        return 0;
    case 70:
        dds3FadeOut(0xFF, 0xFF, 0xFF, 0x1E);
        return 0;
    case 100:
        return -1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoGameoverProcess);

s32 nbAtoGameover2Process(dds3ProcessID_t *id)
{
    u32 runtime = dds3GetProcessRunTime(id);

    switch (runtime) {
    case 10:
        dds3FadeOut(0xFF, 0xFF, 0xFF, 30);
        return 0;
    case 40:
        return -1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoProcess1);

void nbAtoShutdown(void)
{
    nbGetMainProcessData()->AtoProcessID = 0;
}

void nbInitAtoProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_005510C8, 0x3F9, 1, 1, nbAtoProcess1,
                          (dds3ProcessFunc_t)nbAtoShutdown, NULL);
    dds3LinkProcess(data->MainProcessID, id);
    data->AtoProcessID = id;
}

extern sdfMemHandle_t *D_00551FC4, *D_00551FC8, *D_00551FCC;
extern void *D_00551FD0, *D_00551FD4, *D_00551FD8;
extern char D_00543598[], D_005435B8[], D_005435D8[];
extern void *sdfDevLoadFileEx(char *name, void **data, s32 *size);

void dds3LoadBootFile(void)
{
    D_00551FC4 = sdfDevLoadFileEx(D_00543598, &D_00551FD0, NULL);
    D_00551FC8 = sdfDevLoadFileEx(D_005435B8, &D_00551FD4, NULL);
    D_00551FCC = sdfDevLoadFileEx(D_005435D8, &D_00551FD8, NULL);
}

INCLUDE_RODATA(const s32, "newbattle/nbAtoProcess", D_00543598);

INCLUDE_RODATA(const s32, "newbattle/nbAtoProcess", D_005435B8);

INCLUDE_RODATA(const s32, "newbattle/nbAtoProcess", D_005435D8);

