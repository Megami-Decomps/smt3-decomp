#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);
extern s32 GetRBoxNum(void);
extern s32 GetDamage(void);
extern s8 pipeMessCount;
extern s8 messCount;
extern void pipeMessStartIdx(void);
extern void datPartyDamagePipe(s32 big, s32 sml);
extern s32 pipeMessDraw(void);

extern s32 fclChkMessage(void);

extern void fclUpdateMessage(void);

extern void fclEndMessage(void);

extern u32 dds3GetRandA(void *name);

extern char *pStr[];

extern s32 D_00529240[][10];

extern s8 pipeMessMax;

extern char D_00551808[];

extern void *pipeMessData;

extern s8 fclStartMessage(s32 MsgNo);

extern void fclDestroyMessage(void);

extern void sdfMemFreeDelayedDirect(void *aHandle);

extern s32 sprintf(char *buf, const char *fmt, ...);

extern void fclSetMessageVar_20743088(s8 Index, char *pStr);


INCLUDE_ASM(const s32, "pipe/pipeResult", pipeResultSection);

s32 pipeResultDraw(void)
{
    s32 rbox = GetRBoxNum();
    s32 damage = GetDamage();

    if (pipeMessDraw() != 0) {
        if (messCount < pipeMessCount + rbox + 1) {
            pipeMessStartIdx();
            messCount++;
        } else {
            datPartyDamagePipe((u32)damage >> 16, damage & 0xFFFF);
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "pipe/pipeResult", pipeMessStartIdx);

INCLUDE_ASM(const s32, "pipe/pipeResult", pipeMessInit);

void pipeFreeMessage(void)
{
    fclDestroyMessage();
    sdfMemFreeDelayedDirect(pipeMessData);
}

void pipeSetMessageNum(s32 num)
{
    char buf[32];

    sprintf(buf, D_00551808, num);
    fclSetMessageVar_20743088(0, buf);
    fclStartMessage(0);
}

void pipeStartRandomMessage(s32 index, s8 value)
{
    char buf[32];

    fclSetMessageVar_20743088(0, pStr[index]);
    sprintf(buf, D_00551808, value);
    fclSetMessageVar_20743088(1, buf);
    fclStartMessage(1);
}

s32 pipeStartMessage(void)
{
    return fclStartMessage(2);
}

s32 pipeMessDraw(void) {
    s32 finished = 0;
    if (fclChkMessage()) {
        fclUpdateMessage();
    } else {
        finished = 1;
        fclEndMessage();
    }
    return finished;
}

s32 pipeGetRandomMessage(void)
{
    return D_00529240[pipeMessMax - 1][dds3GetRandA(NULL) % 10];
}
