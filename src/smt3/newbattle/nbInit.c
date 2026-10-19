#include "common.h"
#include "nb.h"
#include "effBattle.h"

extern void nbInitPlayer(void);

extern void nbKillMainProcess(void);

void nb_246898(void)
{
    nbInitPlayer();
}

INCLUDE_ASM(const s32, "newbattle/nbInit", nb_2468B0);

void nbExit(void)
{
    nbKillMainProcess();
}

INCLUDE_ASM(const s32, "newbattle/nbInit", nbCallNewBattle);
