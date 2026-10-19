#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern void nbMaeWaitStep(void);

nbMainProcessData_t *nbGetMainProcessData(void);

s32 nbMaeProcess02Wait(void)
{
    nbMaeWaitStep();
    return -1;
}

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbMaeProcess02Loop);

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbMaeProcess02Step);

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbMaeProcess02);

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbMaeProcess01);

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbMaeProcess0);

void nbMaeShutdown(void)
{
    nbGetMainProcessData()->maeProcess = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbMaeProcess", nbInitMaeProcess);
