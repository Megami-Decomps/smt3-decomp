#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern void fldSkyBattle(void);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultLoadTextures);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultFreeTextures);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultCountEntries);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultInitLabels);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultDrawPC1);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultInitTimer);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultFindBestScore);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbFindTobiwarpLabel);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", ShutdownProcess);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultUpdateUnits);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", GetAddExp);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultAddExp);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbResultProcess2);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nb_result_Process);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nb_result_Shutdown);

INCLUDE_ASM(const s32, "newbattle/nbResultProcess", nbInitResultProcess);

void nbLoadStageSky(void)
{
    fldSkyBattle();
}

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543618);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543628);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543648);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543668);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543678);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543688);

INCLUDE_RODATA(const s32, "newbattle/nbResultProcess", D_00543698);

