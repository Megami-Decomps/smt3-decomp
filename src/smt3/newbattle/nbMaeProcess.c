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

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbMaeProcess02);

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbMaeProcess01);

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

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbInitMaeProcess);

