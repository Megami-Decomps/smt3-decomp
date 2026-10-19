#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"

extern void (*D_00551828)(void);

extern s32 fclChkMessage(void);

extern void fclUpdateMessage(void);

extern void fclEndMessage(void);

extern u32 dds3GetRandA(void *seed);

extern char *pStr[];

extern s32 D_00529240[][10];

extern s8 pipeMessMax;

extern char D_00551808[];

extern void *pipeMessData;

extern s8 fclStartMessage(s32 msgno);

extern void fclDestroyMessage(void);

extern void sdfMemFreeDelayedDirect(void *handle);

extern s32 sprintf(char *buf, const char *fmt, ...);

extern void fclSetMessageVar_20743088(s8 index, char *str);


INCLUDE_ASM(const s32, "pipe/pipeResult", pipeResultSection);

INCLUDE_ASM(const s32, "pipe/pipeResult", pipeResultDraw);

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
