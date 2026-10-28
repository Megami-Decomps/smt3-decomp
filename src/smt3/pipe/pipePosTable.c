#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

typedef struct tagPipePosTable tagPipePosTable;

typedef struct tagPipePartsInfo {
    u16 partsID;
    u8 inWay;
    u8 goWay;
    s32 type;
} tagPipePartsInfo;


extern tagPipePartsInfo D_00529030[];

s32 pipeFileSave(s8 filename, tagPipePosTable *posPtr, s8 index)
{
    return 1;
}

INCLUDE_ASM(const s32, "pipe/pipePosTable", pipeFileLoad);

tagPipePartsInfo *getPartsInfo(s16 partsID)
{
    tagPipePartsInfo *pInfo = D_00529030;
    s32 i;

    for (i = 0; i < 60; i++) {
        if (pInfo->partsID == partsID) {
            return pInfo;
        }
        pInfo++;
    }
    return NULL;
}


s32 getPartsType(s32 id)
{
    s32 shiftedId = id >> 6;
    tagPipePartsInfo *pInfo = getPartsInfo((s16)id);
    s32 base = (s8)(shiftedId * 2);

    if (pInfo == NULL) {
        return (s32)pInfo;
    }
    return base + ((pInfo->goWay + pInfo->inWay == 7) ? 1 : 2);
}


s32 pipe_2A2F38(void)
{
    return 0;
}

s32 pipe_2A2F40(void)
{
    return 1;
}

void func_002A2F48(void) {
}

void func_002A2F50(void) {
}
INCLUDE_SDATA(const s32, "pipe/pipePosTable", D_005517E0);

INCLUDE_SDATA(const s32, "pipe/pipePosTable", D_005517E8);

