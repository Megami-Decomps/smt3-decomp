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

extern s32 trmGetJumpTerminalNo(void);

extern void cmpInitCursorRange(void *cursor, u8 count, u8 visible, s32 layout);

extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 mode);

extern s32 fclBlendColor(s32 color, s32 mask, s32 alpha);

extern s32 cmpDrawBoxF(s32 x, s32 y, s32 color, s32 w, s32 h, s32 arg6, s32 ot);

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
    /* 0x21F1 */ s8 TerminalType;
    /* 0x21F2 */ u16 TerminalNo;
    /* 0x21F4 */ s8 TerminalList[0x20];
    /* 0x2214 */ s8 previousTerminalType;
    /* 0x2215 */ s8 unk_2215;
    /* 0x2216 */ s8 cursor;
    /* 0x2217 */ u8 pad2217;
    /* 0x2218 */ void *scriptAllocationHandle;
    /* 0x221C */ u32 scriptData;
    /* 0x2220 */ s32 ScriptPID;
    /* 0x2224 */ u8 unk_2224[0xC];
    /* 0x2230 */ s32 saveProcess;
    /* 0x2234 */ u8 unk_2234[4];
    /* 0x2238 */ s32 texHandle[1];
} trmUpdateWork_t;

extern trmUpdateWork_t *trmUpdateWork;

extern void fclTerminalSetMode(s32 mode, s32 id, trmUpdateWork_t *work);

typedef struct
{
    u8 StepY[4];
    s8 StopFlag;
    u8 unk_05[3];
    s32 CursorSize;
    s32 CursorPos;
    u16 unk_10;
    u16 unk_12;
    u16 unk_14;
    u16 unk_16;
    s32 unk_18;
    s32 unk_1C;
    u8 unk_20;
    u8 unk_21;
    s16 unk_22;
    s8 unk_24;
    s8 unk_25;
    u8 unk_26;
    u8 unk_27;
    s32 unk_28;
    s32 unk_2C;
    s32 unk_30;
    s32 unk_34;
} cmpCursorInfo_t;

typedef struct
{
    cmpCursorInfo_t cursor;
    s8 unk_38;
} cmpPadStopWork_t;

extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);

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
    if (trmUpdateWork->TerminalType != 0) {
        return fldAllCnt[trmUpdateWork->TerminalNo][1];
    }
    return trmUpdateWork->TerminalList[trmUpdateWork->unk_11D8 + trmUpdateWork->unk_11D6];
}
#else
INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmGetJumpTerminalNo);
#endif

extern s32 cmpGetPressedButton(s32 mask);
extern s32 fclSetWorkValue(s8 index, s32 value);
extern s32 fclScriptStart(s32 script, s32 entry, void *process);
extern void cmpResetCursorInfo(cmpCursorInfo_t *cursor);
extern s32 cmpChkPadPressUpDown(void);
extern s32 cmpDisablePadBitStop(cmpPadStopWork_t *work, s32 bits, s32 stop);
extern void cmpPlayCursorSE(u32 button);

void trmUpdateSeqTrans(void)
{
    u16 button = cmpGetPressedButton(0x33);

    switch (button) {
    case 1:
        fclSetWorkValue(0, trmGetJumpTerminalNo());
        fclScriptStart(trmUpdateWork->scriptData, 3, &trmUpdateWork->ScriptPID);
        trmUpdateWork->unk_0001 = 5;
        trmUpdateWork->unk_0004 = button;
        break;
    case 2:
        cmpResetCursorInfo((cmpCursorInfo_t *)((u8 *)trmUpdateWork + 0x11B4));
        if (trmUpdateWork->TerminalType == 0) {
            fclScriptStart(trmUpdateWork->scriptData, 0, &trmUpdateWork->ScriptPID);
        } else {
            fclScriptStart(trmUpdateWork->scriptData, 1, &trmUpdateWork->ScriptPID);
        }
        trmUpdateWork->unk_0001 = 0;
        trmUpdateWork->unk_0004 = 2;
        break;
    case 0x10:
        cmpMoveCursor((cmpCursorInfo_t *)((u8 *)trmUpdateWork + 0x11B4), -1);
        break;
    case 0x20:
        cmpMoveCursor((cmpCursorInfo_t *)((u8 *)trmUpdateWork + 0x11B4), 1);
        break;
    }
    if (cmpChkPadPressUpDown() == 0) {
        cmpMoveCursor((cmpCursorInfo_t *)((u8 *)trmUpdateWork + 0x11B4), 0);
    }
    cmpPlayCursorSE((u16)cmpDisablePadBitStop(
        (cmpPadStopWork_t *)((u8 *)trmUpdateWork + 0x11B4), button, 0x30));
}

void trmActInitMisc(void)
{
    p2sdStopAllSe();
}

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateSeqConfirm);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateSeqRoot);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateActLearge);

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateActSmall);

void trmUpdateAct(void)
{
    if (trmUpdateWork->cursor < 0 && dds3FadeCheck() == 0) {
        trmEndEvent(1);
        trmUpdateWork->TerminalNo = trmGetJumpTerminalNo();
        trmUpdateWork->previousTerminalType = trmUpdateWork->TerminalType;
        trmUpdateWork->TerminalType = 0;
        fclTerminalSetMode(trmUpdateWork->TerminalType, trmUpdateWork->TerminalNo, trmUpdateWork);
        trmUpdateWork->cursor = 0;
        return;
    }
    if (trmUpdateWork->previousTerminalType == 0) {
        trmUpdateActLearge();
    } else {
        trmUpdateActSmall();
    }
}

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
        fclScriptForcedEnd((u8 *)trmUpdateWork->ScriptPID);
        trmUpdateWork->ScriptPID = 0;
        trmEndEvent(0);
        slSetTmnlID(trmUpdateWork->TerminalType, trmUpdateWork->TerminalNo);
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

s32 fcl_2120B0(s16 x, s16 y, s16 w, s16 h, s16 mode, void *cursor)
{
    cmpInitCursorRange(cursor, 2, 2, 0);
    cmpInitCursorLayout(cursor, x, y, w, h, mode);
    return cmpMoveCursor(cursor, 1);
}

s32 fclDrawListSprite(s32 x, s32 y, s32 color, s32 w, s32 h, s32 arg6, s32 blend, s32 ot)
{
    if (blend != 0) {
        arg6 = fclBlendColor(arg6, arg6 & 0xFF, 0x80);
    }
    return cmpDrawBoxF(x, y, color, w, h, arg6, ot);
}

INCLUDE_RODATA(const s32, "facility/fclTerminalUpdate", D_0053D6D8);

