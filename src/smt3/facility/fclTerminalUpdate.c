#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

void fclFreeStage(void);

extern s32 dds3GetProcessStat(void *id);

extern void p2sdStopBGM(s32 id);

extern void p2sdStopAllSe(void);

extern s32 dds3FadeCheck(void);

extern void fclScriptForcedEnd(u8 *PID);

typedef struct
{
    /* 0x0000 */ u8 unk_0000;
    /* 0x0001 */ s8 unk_0001;
    /* 0x0002 */ u8 unk_0002[2];
    /* 0x0004 */ s8 unk_0004;
    /* 0x0005 */ u8 unk_0005[0x38];
    /* 0x003D */ s8 unk_003D;
    /* 0x003E */ u8 unk_003E[0x1198];
    /* 0x11D6 */ s16 unk_11D6;
    /* 0x11D8 */ u8 unk_11D8;
    /* 0x11D9 */ u8 unk_11D9[0x1018];
    /* 0x21F1 */ s8 unk_21F1;
    /* 0x21F2 */ u16 unk_21F2;
    /* 0x21F4 */ s8 unk_21F4[0x21];
    /* 0x2215 */ s8 unk_2215;
    /* 0x2216 */ u8 unk_2216[0xA];
    /* 0x2220 */ s32 unk_2220;
    /* 0x2224 */ u8 unk_2224[0xC];
    /* 0x2230 */ s32 saveProcess;
    /* 0x2234 */ u8 unk_2234[4];
    /* 0x2238 */ s32 texHandle[1];
} trmUpdateWork_t;

extern trmUpdateWork_t *trmUpdateWork;

s32 trmEndEvent(s32 BgmMode);

extern s16 fldAllCnt[][2];

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmStartSaveAct);

s32 trmCheckSaveAct(void)
{
    s32 stat = dds3GetProcessStat((void *)trmUpdateWork->saveProcess);

    if ((u32)stat < 4U) {
        if (stat != 0) {
            return 1;
        }
    }
    fclFreeStage();
    return 0;
}

/* NON_MATCHING: 8 of 17 words differ (first at +0x10). */
#ifdef NON_MATCHING
s32 trmGetJumpTerminalNo(void)
{
    if (trmUpdateWork->unk_21F1 != 0) {
        return fldAllCnt[trmUpdateWork->unk_21F2][1];
    }
    return trmUpdateWork->unk_21F4[trmUpdateWork->unk_11D8 + trmUpdateWork->unk_11D6];
}
#else
INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmGetJumpTerminalNo);
#endif

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateSeqTrans);

void trmActInitMisc(void)
{
    p2sdStopAllSe();
}

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateSeqConfirm);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateSeqRoot);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateActLearge);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateActSmall);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateAct);

extern s32 fmProcessChk(void);

extern s32 trmGetSaveResult(void);

extern void trmStartSaveAct(s32 Mode);

void trmUpdateSave(void)
{
    if (fmProcessChk() == 0)
    {
        p2sdStopBGM(trmGetSaveResult());
        trmStartSaveAct(1);
        trmUpdateWork->unk_0001 = 8;
        trmUpdateWork->unk_0004 = 1;
    }
}

extern void slSetTmnlID(s8 type, u16 id);

extern void fmProcTrigSequence(s32 mode);

void trmUpdateSaveActIn(void)
{
    if (dds3FadeCheck() == 0) {
        fclScriptForcedEnd((u8 *)trmUpdateWork->unk_2220);
        trmUpdateWork->unk_2220 = 0;
        trmEndEvent(0);
        slSetTmnlID(trmUpdateWork->unk_21F1, trmUpdateWork->unk_21F2);
        fmProcTrigSequence(0);
        trmUpdateWork->unk_0001 = 2;
        trmUpdateWork->unk_0004 = 1;
    }
}

void trmUpdateSaveActOut(void)
{
    if (trmCheckSaveAct() == 0) {
        trmUpdateWork->unk_003D = 1;
        trmStartEvent();
        trmUpdateWork->unk_0001 = 0;
        trmUpdateWork->unk_0004 = 1;
    }
}

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdate);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", fcl_2120B0);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", fclDrawListSprite);

INCLUDE_RODATA(const s32, "facility/fclTerminalUpdate", D_0053D6D8);

