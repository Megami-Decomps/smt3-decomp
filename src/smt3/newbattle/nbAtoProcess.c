#include "common.h"
#include "nb.h"
#include "effBattle.h"

nbMainProcessData_t *nbGetMainProcessData(void);

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoStoneProcess);

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoGameoverProcess);

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoGameover2Process);

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbAtoProcess1);

void nbAtoShutdown(void)
{
    nbGetMainProcessData()->atoProcess = 0;
}

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", nbInitAtoProcess);

INCLUDE_ASM(const s32, "newbattle/nbAtoProcess", dds3LoadBootFile);

