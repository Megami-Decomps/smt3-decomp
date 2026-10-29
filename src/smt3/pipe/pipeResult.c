#include "fpu.h"
#include "common.h"
#include "sdf_resource.h"
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

extern u8 ResultIndex;
extern u8 pipeResultFlag;
extern void pipeMessInit(void);
extern void pipeFreeMessage(void);
extern s32 pipeResultDraw(void);
extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);

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

#include "sdk_libc.h"

extern void fclSetMessageVar_20743088(s8 Index, char *pStr);


s32 pipeResultSection(void)
{
    s32 finished = 0;

    switch (ResultIndex) {
    case 0:
        pipeMessInit();
        pipeResultFlag = 0;
        ResultIndex = 1;
        break;
    case 1:
        dds3FadeIn(0xFF, 0xFF, 0xFF, 0x1E);
        ResultIndex = 2;
        pipeResultFlag = 0;
        break;
    case 2:
        if (pipeResultDraw() != 0) {
            ResultIndex = 3;
        }
        break;
    case 3:
        dds3FadeOut(0xFF, 0xFF, 0xFF, 0x1E);
        ResultIndex = 0xFF;
        pipeResultFlag = 0;
        break;
    case 0xFF:
        if (pipeResultFlag < 0x1F) {
            pipeResultFlag++;
            break;
        }
        ResultIndex = 0;
        finished = 1;
        pipeFreeMessage();
        break;
    }
    return finished;
}

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

extern u8 messIdx;
extern s32 GetMaka(void);
extern s32 pipeStartMessage(void);
extern void pipeSetMessageNum(s32 num);
extern void pipeStartRandomMessage(s32 index, s8 value);
extern s32 pipeGetRandomMessage(void);
extern void datAddMaka(s32 addmaka);
extern void datAddItem(s32 id, s32 num);
extern void DEBUG_PRINT(const char *format, ...);

void pipeMessStartIdx(void)
{
    switch (messIdx) {
    case 0:
        pipeStartMessage();
        messIdx = 1;
        DEBUG_PRINT("complete\n");
        return;
    case 1: {
        s32 maka = GetMaka();
        pipeSetMessageNum(maka);
        datAddMaka(maka);
        messIdx = 2;
        DEBUG_PRINT("MAKA add\n");
        return;
    }
    case 2: {
        s32 item = pipeGetRandomMessage();
        DEBUG_PRINT("B rBox %x \n", item);
        pipeStartRandomMessage(item, 1);
        datAddItem(item, 1);
        DEBUG_PRINT("rBox open\n");
    }
    }
}

extern void *pipeMessData;
extern s8 pipeMessCount, messCount, pipeMessMax;
extern u8 messIdx;
extern s32 mapIndex, pipeObjList, pipeObjTail, myMaka;
extern u32 D_00551810;
extern s32 D_00551818;
extern s8 fclInitMessage(void *data);
extern void DEBUG_PRINT(const char *format, ...);

const char D_00548F18[24] = "/pipe/pipemes.bmd";
const char D_00548F30[24] = "MAKA ALL Complete!!\n";

void pipeMessInit(void) {
    pipeMessData = sdfDevLoadFileEx(D_00548F18, &D_00551810, NULL);
    fclInitMessage((void *)D_00551810);
    if (pipeObjTail < 8) {
        D_00551818 = pipeObjList * 5 + (100 << pipeObjTail) - 100;
    } else {
        s32 value = 9999;
        D_00551818 = value * pipeObjTail + pipeObjList * 5 - 57293;
    }
    if (D_00551818 == myMaka) {
        DEBUG_PRINT(D_00548F30);
        pipeMessCount = 1;
        messIdx = 0;
    } else {
        pipeMessCount = 0;
        messIdx = 1;
    }
    messCount = 0;
    if (mapIndex == 1) {
        pipeMessMax = 1;
    } else if (mapIndex < 10) {
        pipeMessMax = mapIndex / 2 + 1;
    } else if (mapIndex == 10) {
        pipeMessMax = 6;
    } else if (mapIndex == 11) {
        pipeMessMax = 7;
    }
}


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
INCLUDE_SDATA(const s32, "pipe/pipeResult", ResultIndex);

INCLUDE_SDATA(const s32, "pipe/pipeResult", D_00551801);

INCLUDE_SDATA(const s32, "pipe/pipeResult", D_00551808);

INCLUDE_SDATA(const s32, "pipe/pipeResult", pipeMessData);

INCLUDE_SDATA(const s32, "pipe/pipeResult", D_00551810);

INCLUDE_SDATA(const s32, "pipe/pipeResult", pipeMessCount);

INCLUDE_SDATA(const s32, "pipe/pipeResult", D_00551815);

INCLUDE_SDATA(const s32, "pipe/pipeResult", D_00551818);

