#include "common.h"
#include "sdf_resource.h"
#include "nb.h"
#include "effBattle.h"
#include "newdata.h"
#include "event.h"
#include "motion.h"

nbMainProcessData_t *nbGetMainProcessData(void);

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer,
                                         dds3ProcessFunc_t processFunc,
                                         dds3ProcessFunc_t shutdownFunc, void *workData);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern dds3ProcessFunc_t nbAtoProcess1(dds3ProcessID_t *id);
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

extern void nbSingleFormExceptedDispOff(nbFormation_t *form);
extern void evtSetUnitColorRGB(evtUnit_t *unit, s32 frames, u32 color);
extern void evtSetUnitColorA(evtUnit_t *unit, s32 frames, u32 color);
extern void evtSetUnitMotion(evtUnit_t *unit, s32 motion, s32 before, s32 smooth, s32 mode);
extern void nbCameraSetBougai(nbFormation_t *form);

s32 nbAtoGameoverProcess(dds3ProcessID_t *id)
{
    s32 runtime = dds3GetProcessRunTime(id);
    nbMainProcessData_t *data = nbGetMainProcessData();
    nbFormation_t *form = data->form;

    switch (runtime) {
    case 1:
        nbSingleFormExceptedDispOff(form);
        evtSetUnitColorRGB(form->hunit, 0, 0x80808080);
        evtSetUnitColorA(form->hunit, 0, 0x80808080);
        form->hunit->owner->first->speed = 1.0f;
        evtSetUnitMotion(form->hunit, 0x17, 0, 0, 2);
        nbCameraSetBougai(form);
        break;
    case 50:
        dds3FadeOut(255, 255, 255, 30);
        break;
    case 80:
        return -1;
    }
    return 0;
}

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

extern void datBossRashUpdRec(s32 encounter, s32 presses);
extern void nbPanelDraw(s32 flag);
extern void nbBgmFadeOut(void);
extern void fldPlayerAFree(void);
extern s32 mdlGetMinorNum(mdlObj_t *model);

dds3ProcessFunc_t nbAtoProcess1(dds3ProcessID_t *id)
{
    nbMainProcessData_t *data = nbGetMainProcessData();

    if (data->endtype == 1) {
        datBossRashUpdRec(data->encno, data->unk140);
    }
    switch (data->endtype) {
    case 2:
    case 6:
        nbPanelDraw(0);
        nbBgmFadeOut();
        break;
    case 3:
        break;
    }
    switch (data->endtype) {
    case 6:
        fldPlayerAFree();
        return nbAtoStoneProcess;
    case 2:
        fldPlayerAFree();
        if (mdlGetMinorNum(data->form->hunit->owner) == 0) {
            return nbAtoGameoverProcess;
        }
        return nbAtoGameover2Process;
    case 1:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
        break;
    }
    return (dds3ProcessFunc_t)(dds3GetProcessRunTime(id) != 0 ? -1 : 0);
}

void nbAtoShutdown(void)
{
    nbGetMainProcessData()->AtoProcessID = 0;
}

void nbInitAtoProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *id;

    id = dds3StartProcess(D_005510C8, 0x3F9, 1, 1, (dds3ProcessFunc_t)nbAtoProcess1,
                          (dds3ProcessFunc_t)nbAtoShutdown, NULL);
    dds3LinkProcess(data->MainProcessID, id);
    data->AtoProcessID = id;
}

extern sdfMemHandle_t *D_00551FC4, *D_00551FC8, *D_00551FCC;
extern void *D_00551FD0, *D_00551FD4, *D_00551FD8;
extern char D_00543598[], D_005435B8[], D_005435D8[];

void dds3LoadBootFile(void)
{
    D_00551FC4 = sdfDevLoadFileEx(D_00543598, (u32 *)&D_00551FD0, NULL);
    D_00551FC8 = sdfDevLoadFileEx(D_005435B8, (u32 *)&D_00551FD4, NULL);
    D_00551FCC = sdfDevLoadFileEx(D_005435D8, (u32 *)&D_00551FD8, NULL);
}

INCLUDE_RODATA(const s32, "newbattle/nbAtoProcess", D_00543598);

INCLUDE_RODATA(const s32, "newbattle/nbAtoProcess", D_005435B8);

INCLUDE_RODATA(const s32, "newbattle/nbAtoProcess", D_005435D8);

