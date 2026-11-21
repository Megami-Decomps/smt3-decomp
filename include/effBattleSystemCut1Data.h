#ifndef EFF_BATTLE_SYSTEM_CUT1_DATA_H
#define EFF_BATTLE_SYSTEM_CUT1_DATA_H

#include "effBattle.h"

/* One shared cut effect of the effBTLSystemCut1 table. */
typedef struct
{
    const char *fileName;
    effBTL_t *hBTLEffect;
    s32 use;
    void *pData;
    sdfMemHandle_t *hMem;
} effBattleSystem_effBTLSystem_Cut1_tag;

extern effBattleSystem_effBTLSystem_Cut1_tag effBTLSystemCut1Data[25];

#endif
