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

extern char D_00548E18[], D_00548E30[], D_00548E48[], D_00548E60[];

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


/* NON_MATCHING: 15 of 24 words; retail keeps the result in $2, this needs a move. */
#ifdef NON_MATCHING
s32 getPartsType(s32 id)
{
    tagPipePartsInfo *pInfo = getPartsInfo(id);
    s32 ret = (s32)pInfo;

    if (pInfo != NULL) {
        s32 base = (s8)((id >> 6) << 1);

        if (pInfo->inWay + pInfo->goWay == 7) {
            ret = base + 1;
        } else {
            ret = base + 2;
        }
    }
    return ret;
}
#else
INCLUDE_ASM(const s32, "pipe/pipePosTable", getPartsType);
#endif


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
