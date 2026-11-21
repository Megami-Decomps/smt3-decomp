#ifndef EFF_BATTLE_SYSTEM_PART_DATA_H
#define EFF_BATTLE_SYSTEM_PART_DATA_H

#include "common.h"
#include "sdf_resource.h"
#include "effBattle.h"

typedef struct
{
    u16 type;
    void *pHandle;
} effPartHandle_t;

/* One entry of an effBTLSystem_*_PartList table. */
typedef struct
{
    u16 type;
    const char *fileName;
    void *pData;
    u16 refCount;
    effPartHandle_t *hPart;
    effBTL_t *hBTLEffect;
    sdfMemHandle_t *hMem;
} effBattleSystem_effBTLSystem_Part_tag;

#endif
