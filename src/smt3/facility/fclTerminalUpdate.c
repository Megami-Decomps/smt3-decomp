#include "common.h"
#include "camp.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

void fclFreeStage(void);

extern u32 dds3GetProcessStat(dds3ProcessID_t *id);

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
    /* 0x2217 */ s8 movieState;
    /* 0x2218 */ void *scriptAllocationHandle;
    /* 0x221C */ u32 scriptData;
    /* 0x2220 */ s32 ScriptPID;
    /* 0x2224 */ u8 unk_2224[0xC];
    /* 0x2230 */ s32 saveProcess;
    /* 0x2234 */ dds3ProcessID_t *secondaryMovieProcess;
    /* 0x2238 */ s32 texHandle[1];
} trmUpdateWork_t;

extern trmUpdateWork_t *trmUpdateWork;

extern void fclTerminalSetMode(s32 mode, s32 id, trmUpdateWork_t *work);

extern s32 cmpMoveCursor(cmpCursorInfo_t *cursor, s8 direction);

s32 trmEndEvent(s32 BgmMode);

extern s16 fldAllCnt[][2];

extern u32 slGetExitMode(void);
extern s32 evtLoadEventBgm(s32 eventId);
extern s32 evtCheckEventBgmLoaded(s32 eventId);
extern dds3ProcessID_t *evtStartPolygonMovie2(s32 priority, s32 eventno, s32 cutno);

#ifdef NON_MATCHING
void trmStartSaveAct(s32 saved)
{
    s32 event;
    s32 cut = 0;
    u32 exitMode;
    u16 terminal = trmUpdateWork->TerminalNo;

    if (trmUpdateWork->TerminalType == 0) {
        if ((terminal >= 18 && terminal < 20) || (terminal >= 23 && terminal < 25)) {
            event = 0x326;
        } else {
            event = 0x324;
        }
    } else {
        event = 0x325;
    }
    if (saved == 0) {
        trmEndEvent(1);
        trmUpdateWork->saveProcess = 0;
        return;
    }
    exitMode = slGetExitMode();
    if (exitMode == 0) {
        trmUpdateWork->saveProcess = 0;
        return;
    }
    if (exitMode == 1) {
        cut = event == 0x324 ? 60 : 20;
    }
    evtLoadEventBgm(event);
    while (evtCheckEventBgmLoaded(event) == 0) {
    }
    trmUpdateWork->saveProcess = (s32)evtStartPolygonMovie2(0x3E8, event, cut);
}
#else
INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmStartSaveAct);
#endif

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

s32 trmGetJumpTerminalNo(void)
{
    s32 terminal;
    if (trmUpdateWork->TerminalType != 0) {
        terminal = fldAllCnt[trmUpdateWork->TerminalNo][1];
    } else {
        terminal = trmUpdateWork->TerminalList[trmUpdateWork->unk_11D6 + trmUpdateWork->unk_11D8];
    }
    return terminal;
}

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

extern s32 fclGetFlag(s8 index);
extern void dds3FadeOut(s32 r, s32 g, s32 b, s32 frames);

s32 trmUpdateSeqConfirm(void)
{
    if (dds3GetProcessStat((void *)trmUpdateWork->ScriptPID) == 2) {
        return 0;
    }
    switch (fclGetFlag(2)) {
    case 0:
        trmActInitMisc();
        dds3FadeOut(0, 0, 0, 20);
        trmUpdateWork->unk_0001 = 6;
        trmUpdateWork->unk_0004 = 1;
        fclScriptForcedEnd((u8 *)trmUpdateWork->ScriptPID);
        trmUpdateWork->ScriptPID = 0;
        break;
    case 1:
        if (trmUpdateWork->TerminalType == 0) {
            fclScriptStart(trmUpdateWork->scriptData, 2, &trmUpdateWork->ScriptPID);
            trmUpdateWork->unk_0001 = 1;
            trmUpdateWork->unk_0004 = 2;
        } else {
            fclScriptStart(trmUpdateWork->scriptData, 1, &trmUpdateWork->ScriptPID);
            trmUpdateWork->unk_0001 = 0;
            trmUpdateWork->unk_0004 = 2;
        }
        break;
    case 2:
        break;
    }
    return 1;
}

INCLUDE_ASM(const s32, "facility/fclTerminalUpdate", trmUpdateSeqRoot);

extern void dds3FadeIn(s32 r, s32 g, s32 b, s32 frames);
extern s32 dds3PlayMovieByIndex(s32 index);
extern s32 dds3WaitMovieEnd(void);
extern void movieStop(void);
extern dds3ProcessID_t *evtStartPolygonMovie2(s32 priority, s32 eventno, s32 cutno);
extern void evtSetPolygonMovieFlag(dds3ProcessID_t *id, s32 flag);
extern void evtResetPolygonMovieFlag(dds3ProcessID_t *id, s32 flag);
extern u8 dds3PadButtonState[2][2][16];

void trmUpdateActLearge(void)
{
    s32 done = 0;

    switch (trmUpdateWork->cursor) {
    case 0: {
        dds3ProcessID_t *movieProcess;

        movieProcess = evtStartPolygonMovie2(0x3E8, 0x324, 0x1E);
        trmUpdateWork->saveProcess = (s32)movieProcess;
        evtSetPolygonMovieFlag(movieProcess, 1);
        dds3PlayMovieByIndex(3);
        dds3FadeIn(0, 0, 0, 0);
        trmUpdateWork->cursor = 1;
        break;
    }
    case 2:
        movieStop();
        evtResetPolygonMovieFlag((dds3ProcessID_t *)trmUpdateWork->saveProcess, 1);
        trmUpdateWork->cursor = 3;
        break;
    default: {
        s8 phase = trmUpdateWork->cursor;

        switch (phase) {
        case 1:
            done = dds3WaitMovieEnd() != 0;
            if ((s8)dds3PadButtonState[1][0][1] < 0) {
                done = 1;
            }
            if ((s8)dds3PadButtonState[1][0][3] < 0) {
                done = 1;
            }
            break;
        case 3:
            done = dds3GetProcessStat((void *)trmUpdateWork->saveProcess) == phase;
            if (dds3GetProcessStat((void *)trmUpdateWork->saveProcess) == 0) {
                done = 1;
            }
            break;
        }
        break;
    }
    }
    if (done != 0) {
        switch (trmUpdateWork->cursor) {
        case 1:
            trmUpdateWork->cursor = 2;
            break;
        case 3:
            fclFreeStage();
            trmUpdateWork->unk_0001 = 0;
            trmUpdateWork->unk_0004 = 1;
            break;
        }
    }
}

extern s32 dds3KillProcess(dds3ProcessID_t *id, s32 shutdownTimer);
extern s32 evtCheckPolygonMovieRun(dds3ProcessID_t *id);

void trmUpdateActSmall(void)
{
    s8 phase = trmUpdateWork->cursor;
    s32 done = 0;

    switch (phase) {
    case 0:
        trmUpdateWork->secondaryMovieProcess = evtStartPolygonMovie2(0x3E8, 0x324, 0x1E);
        trmUpdateWork->saveProcess = (s32)evtStartPolygonMovie2(0x3E8, 0x325, 0x32);
        evtSetPolygonMovieFlag((dds3ProcessID_t *)trmUpdateWork->saveProcess, 1);
        evtSetPolygonMovieFlag(trmUpdateWork->secondaryMovieProcess, 1);
        trmUpdateWork->cursor = 1;
        break;
    case 2:
        if (dds3GetProcessStat((void *)trmUpdateWork->saveProcess) == 1 ||
            dds3GetProcessStat((void *)trmUpdateWork->saveProcess) == 2) {
            dds3KillProcess((dds3ProcessID_t *)trmUpdateWork->saveProcess, 0);
            fclFreeStage();
            p2sdStopAllSe();
        }
        evtResetPolygonMovieFlag(trmUpdateWork->secondaryMovieProcess, 1);
        trmUpdateWork->cursor = 3;
        break;
    default:
        switch (phase) {
        case 1:
            done = dds3GetProcessStat((void *)trmUpdateWork->saveProcess) == 3;
            if (dds3GetProcessStat((void *)trmUpdateWork->saveProcess) == 0) {
                done = 1;
            }
            if ((s8)dds3PadButtonState[1][0][1] < 0) {
                done = 1;
            }
            if ((s8)dds3PadButtonState[1][0][3] < 0) {
                done = 1;
            }
            if (trmUpdateWork->movieState == 2) {
                done = 1;
            }
            if (done != 0) {
                trmUpdateWork->movieState = 2;
            }
            if (evtCheckPolygonMovieRun((dds3ProcessID_t *)trmUpdateWork->saveProcess) == 0) {
                return;
            }
            if (trmUpdateWork->movieState == 0) {
                evtResetPolygonMovieFlag((dds3ProcessID_t *)trmUpdateWork->saveProcess, 1);
                dds3FadeIn(0, 0, 0, 0);
                trmUpdateWork->movieState = 1;
            }
            break;
        case 3:
            done = dds3GetProcessStat(trmUpdateWork->secondaryMovieProcess) == phase;
            if (dds3GetProcessStat(trmUpdateWork->secondaryMovieProcess) == 0) {
                done = 1;
            }
            break;
        }
        break;
    }
    if (done != 0) {
        switch (trmUpdateWork->cursor) {
        case 1:
            trmUpdateWork->cursor = 2;
            break;
        case 3:
            fclFreeStage();
            trmUpdateWork->unk_0001 = 0;
            trmUpdateWork->unk_0004 = 1;
            break;
        }
    }
}

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

#ifdef NON_MATCHING
extern s8 trmStartEvent(void);
#endif

void trmUpdateSaveActOut(void)
{
    if (trmCheckSaveAct() == 0) {
        trmUpdateWork->unk_003D = 1;
        trmStartEvent();
        trmUpdateWork->unk_0001 = 0;
        trmUpdateWork->unk_0004 = 1;
    }
}

extern void *dds3GetProcessData(dds3ProcessID_t *id);
extern s32 fclChkDbgProcess(void);
extern s32 trmChkEventWait(void);
extern s8 trmGetCallMode(void);
extern void trmSetProcessTerm(void);
extern void trmUpdateSeqRoot(void);
extern s32 cmpUpdateSeqInfo(void *work, s8 previous);

s32 trmUpdate(dds3ProcessID_t *id)
{
    s8 previous;
    u8 flags;

    trmUpdateWork = dds3GetProcessData(id);
    previous = trmUpdateWork->unk_0001;
    if (fclChkDbgProcess() != 0) {
        return 0;
    }
    if (trmChkEventWait() != 0) {
        return 0;
    }

    flags = trmUpdateWork->unk_0000;
    if (flags & 0x40) {
        return -1;
    }
    if ((flags & 2) && (flags & 1)) {
        switch (trmUpdateWork->unk_0001) {
        case 0:
            trmUpdateSeqRoot();
            break;
        case 1:
            trmUpdateSeqTrans();
            break;
        case 2:
            trmUpdateSave();
            break;
        case 3:
            break;
        case 4:
            if (trmGetCallMode() == 0 && dds3FadeCheck() != 0) {
                break;
            }
            trmSetProcessTerm();
            trmUpdateWork->unk_0000 |= 0x10;
            break;
        case 5:
            if (trmUpdateSeqConfirm() == 0) {
                return 0;
            }
            break;
        case 6:
            trmUpdateAct();
            break;
        case 7:
            trmUpdateSaveActIn();
            break;
        case 8:
            trmUpdateSaveActOut();
            break;
        }
    }

    cmpUpdateSeqInfo(trmUpdateWork, previous);
    return 0;
}

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

