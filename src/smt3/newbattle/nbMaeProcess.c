#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern void nbMaeWaitStep(void);
extern s32 nbMaeCheckDone(void);
extern void nbSetMaeFlag(s32 flag);

nbMainProcessData_t *nbGetMainProcessData(void);
extern void nbSetStageParam(s16 major, s16 minor);
extern void effBTLEncountStart(void);
extern s32 nbMaeProcess01(void);
extern s32 nbMaeProcess02(void);
extern s32 nbMaeProcess02Step(void);
extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer,
                                         dds3ProcessFunc_t processFunc,
                                         dds3ProcessFunc_t shutdownFunc,
                                         void *workData);
extern void dds3LinkProcess(dds3ProcessID_t *parent, dds3ProcessID_t *target);
extern void dds3SetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);
extern void dds3ResetProcessFlag(dds3ProcessID_t *target, u32 flag, s32 type);
extern s32 nbCheckEventEntries(nbMainProcessData_t *data);
extern s32 nbCheckEventPackLoaded(nbMainProcessData_t *data);
extern void nbStartEventPacks(nbMainProcessData_t *data);
extern void nbLoadDefaultModelPack(nbMainProcessData_t *data);
extern char D_005510C0[]; /* "nb_mae" */

s32 nbMaeProcess02Wait(void)
{
    nbMaeWaitStep();
    return -1;
}

s32 nbMaeProcess02Loop(void)
{
    if (nbMaeCheckDone() == 1) {
        return (s32)nbMaeProcess02Wait;
    }
    return 0;
}

s32 nbMaeProcess02Step(void)
{
    nbSetMaeFlag(0);
    return (s32)nbMaeProcess02Loop;
}

s32 nbMaeProcess02(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    if (nbCheckEventEntries(data) != 0) {
        if (nbCheckEventPackLoaded(data) != 0) {
            dds3ResetProcessFlag(data->StageProcessID, 0x20, 0);
            return (s32)nbMaeProcess02Step;
        }
    }
    return 0;
}

s32 nbMaeProcess01(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    nbStartEventPacks(data);
    nbLoadDefaultModelPack(data);
    dds3SetProcessFlag(data->StageProcessID, 0x20, 0);
    return (s32)nbMaeProcess02;
}

s32 nbMaeProcess0(void)
{
    nbMainProcessData_t *data;

    data = nbGetMainProcessData();
    nbSetStageParam(data->StageMajor, data->StageMinor);
    effBTLEncountStart();
    return (s32)nbMaeProcess01;
}

void nbMaeShutdown(void)
{
    nbGetMainProcessData()->maeProcess = 0;
}

void nbInitMaeProcess(nbMainProcessData_t *data)
{
    dds3ProcessID_t *process;

    process = dds3StartProcess(D_005510C0, 0x3F9, 1, 1,
                               (dds3ProcessFunc_t)nbMaeProcess0,
                               (dds3ProcessFunc_t)nbMaeShutdown, NULL);
    dds3LinkProcess(data->MainProcessID, process);
    data->maeProcess = process;
}

