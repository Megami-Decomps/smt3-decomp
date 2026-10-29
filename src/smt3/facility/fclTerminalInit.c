#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"
#include "kernel.h"

extern void dds3KillProcessName(char *name, s32 shutdowntimer);

void fclFreeStage(void);

extern void *dds3SearchProcessName(const char *namestr);

extern s8 gCallMode;

extern s8 trmMode;

extern void p2sdStopBGM(s32 id);

extern s8 gProcessStat_3;

extern u8 gCommandFlag[4];

extern s32 scrGetIntPara(s32 n);

typedef struct
{
    u8 pad_0000[0x2216];
    s8 cursor;
    u8 pad_2217;
} trmCalcWork_t;

extern trmCalcWork_t *trmCalcWork;

typedef struct
{
    u8 terminalFlags;              /* 0x0000 */
    u8 unk_01;
    u8 pad_0002;
    u8 unk_03;
    u8 unk_04;
    u8 pad_0005[0xF];
    void *allocation;               /* 0x0014 */
    u8 pad_0018[0x25];
    u8 terminalReferenceCount;      /* 0x003D */
    u8 pad_003E[0x21B3];
    s8 terminalType;                /* 0x21F1 */
    u16 terminalNo;                 /* 0x21F2 */
    s8 terminalList[0x20];          /* 0x21F4 */
    s8 previousTerminalType;        /* 0x2214 */
    s8 terminalCommandCount;        /* 0x2215 */
    s8 cursor;                      /* 0x2216 */
    u8 pad_2217[1];
    void *scriptAllocationHandle;     /* 0x2218 */
    u32 scriptData;
    s32 ScriptPID;                  /* 0x2220 */
    u8 pad_2224[0xC];
    s32 saveProcess;                /* 0x2230 */
    u8 pad_2234[4];
    s32 texHandle[1];               /* 0x2238 */
} trmTerminalWork_t;

extern trmTerminalWork_t *GBWK_3;

#include "sdk_libc.h"

extern void sdfTexFree(s32 aHandle);

extern u8 fclTerminalCommandCount[1];

extern char D_0053D640[];

extern char D_0053D650[];

extern char D_0053D660[];

extern void fclScriptForcedEnd(u8 *PID);

s8 trmGetCallMode(void)
{
    return gCallMode;
}

s8 trmGetMode(void)
{
    return trmMode;
}

void fclSetTerminalMode(s8 mode)
{
    trmMode = mode;
}

extern void cmpSetCursorFlag(void *cursor, u8 *flags);

#ifdef NON_MATCHING
extern s32 evtBitCheck(s32 no);
#endif

s32 trmGetEventNo(s32 update, trmTerminalWork_t *work)
{
    s32 event = -1;

    if (work->terminalType == 0) {
        switch (work->terminalNo) {
        case 1:
            if (!evtBitCheck(0xD)) {
                if (evtBitCheck(9)) {
                    gCommandFlag[0] |= 2;
                    if (update != 0) {
                        work->terminalReferenceCount++;
                    }
                } else {
                    gCommandFlag[0] |= 2;
                    if (update != 0) {
                        work->terminalReferenceCount++;
                    }
                    event = 0x26B;
                }
            }
            break;
        case 2:
            if (!evtBitCheck(0xD)) {
                if (evtBitCheck(9) && evtBitCheck(0xA)) {
                    event = 0x26F;
                    gCommandFlag[0] |= 2;
                    if (update != 0) {
                        work->terminalReferenceCount++;
                    }
                } else {
                    gCommandFlag[0] |= 2;
                    if (update != 0) {
                        work->terminalReferenceCount++;
                    }
                }
            }
            break;
        case 3:
            if (!evtBitCheck(0x34) && evtBitCheck(0xE)) {
                event = 0x280;
            }
            break;
        case 9:
            if (!evtBitCheck(0x53)) {
                if (evtBitCheck(0x4F)) {
                    event = 0x296;
                } else if (evtBitCheck(0x4B)) {
                    event = 0x294;
                } else if (evtBitCheck(0x44)) {
                    event = 0x290;
                } else if (evtBitCheck(0x3E)) {
                    event = 0x28C;
                } else {
                    event = 0x286;
                }
            }
            break;
        case 18:
        case 19:
        case 23:
        case 24:
            event = 0x326;
            gCommandFlag[0] |= 2;
            if (update != 0) {
                work->terminalReferenceCount++;
            }
            break;
        }
        if (event < 0) {
            event = 0x324;
        }
    } else {
        event = 0x325;
        if (!evtBitCheck(0xD)) {
            gCommandFlag[0] |= 2;
            if (update != 0) {
                work->terminalReferenceCount++;
            }
        }
    }
    cmpSetCursorFlag((u8 *)work + 0x1C, gCommandFlag);
    return event;
}

extern char D_0053D5B0[]; /* "e%d_main" */

extern char D_0054FC20[];

s32 trmCreateEventProcessName(s32 Type, char *pBuf)
{
    s32 no = trmGetEventNo(1, GBWK_3);

    if (no < 0) {
        return 0;
    }
    if (Type == 0) {
        sprintf(pBuf, D_0053D5B0, no);
    } else {
        sprintf(pBuf, D_0054FC20, no);
    }
    return 1;
}

extern void p2sdPlayBGM(s32 id);

void trmCreateTerminalList(s32 type, s32 no)
{
    s32 event = trmGetEventNo(0, GBWK_3);

    if (type == 0) {
        switch (no) {
        case 1:
        case 3:
        case 9:
            if (event != 0x324) {
                return;
            }
            break;
        }
    }
    p2sdPlayBGM(0x20001);
}

void trmEndBGM(void)
{
    p2sdStopBGM(0x20001);
}

#ifdef NON_MATCHING
#endif

s8 trmStartEvent(void)
{
    s32 event;
    s8 result;
    switch (trmGetMode()) {
    case 0:
        event = trmGetEventNo(1, GBWK_3);
        if (event >= 0) {
            evtStartEvent(event);
            fclSetTerminalMode(1);
            result = 1;
        } else {
            result = 0;
        }
        break;
    case 1:
    case 5:
        result = 1;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

s32 trmEndEvent(s32 BgmMode)
{
    char name[0x20];

    if (BgmMode != 0) {
        trmEndBGM();
    }
    fclScriptForcedEnd((u8 *)GBWK_3->ScriptPID);
    GBWK_3->ScriptPID = 0;
    fclSetTerminalMode(0);
    if (trmCreateEventProcessName(0, name) != 0) {
        dds3KillProcessName(name, 0);
        trmCreateEventProcessName(1, name);
        dds3KillProcessName(name, 0);
        fclFreeStage();
    }
    return 1;
}

extern s32 trmCreateEventProcessName(s32 Type, char *pBuf);

s32 trmChkEventWait(void)
{
    char name[0x20];

    if (trmGetMode() == 1)
    {
        return 1;
    }
    if (trmGetMode() == 5)
    {
        trmCreateEventProcessName(0, name);
        if (dds3SearchProcessName(name) != 0)
        {
            return 1;
        }
    }
    return 0;
}

void trmSetProcessTerm(void)
{
    if (gCallMode != 0) {
        fclScriptForcedEnd((u8 *)GBWK_3->ScriptPID);
        fclSetTerminalMode(5);
        gProcessStat_3 = 2;
        gCallMode = 0;
    }
}

typedef trmTerminalWork_t trmUpdateWork_t;

extern trmUpdateWork_t *trmUpdateWork;

extern s32 D_002FC120[32];
extern s16 D_002FC1A2[][2];
extern s32 evtBitCheck(s32 no);

INCLUDE_RODATA(const s32, "facility/fclTerminalInit", D_0053D5B0);

void fclTerminalScanCommands(trmTerminalWork_t *work)
{
    s32 terminalNo;
    s32 i;
    s32 *terminal = D_002FC120;

    work->terminalCommandCount = 0;
    for (i = 31; i >= 0; i--) {
        s16 eventNo;

        terminalNo = *terminal++;

        if (work->terminalType == 0 && work->terminalNo == terminalNo) {
            continue;
        }

        eventNo = D_002FC1A2[terminalNo][0];
        if (eventNo == 0 || evtBitCheck(eventNo) == 0) {
            continue;
        }

        work->terminalList[work->terminalCommandCount++] = terminalNo;
    }

    printf("Transport count = %d\n", work->terminalCommandCount);
}

extern void lstInitListWindow(s32 X, s32 Y, s32 Z, s32 W, s32 H, s32 Col, s8 Style, void *work);
extern void fclSetPanelTex(void *panel, s32 a2, s32 a3, s32 a4, s32 a5, s32 tex);
extern void cmpInitCursorRange(void *cursor, u8 count, u8 visible, s32 layout);
extern void cmpInitCursorLayout(void *cursor, s32 a, s32 b, s32 c, s32 d, s32 mode);

void fclTerminalInitList(void *list, s8 count, s32 handle)
{
    void *cursor;

    lstInitListWindow(0, 0x298, 0xFFFFF, 0x129, 0xD3, 0x19191E59, 1, list);
    fclSetPanelTex(list, 0, 0, 0, 0, handle);
    cursor = (u8 *)list + 0x15C;
    cmpInitCursorRange(cursor, (u8)count, 7, 0);
    cmpInitCursorLayout(cursor, 0x2E, 7, 0x98, 0x19, 0x1D);
}


extern u32 trmMenuTextTable[];
extern void fclInitCursorWindow(void *cursor, void *data, u8 count);
extern void fclSetCursorList(void *cursor, s32 count);

void fclTerminalInitMenu(s32 id, void *commands, void *work)
{
    u32 *table;

    if (trmGetEventNo(1, work) == 0x296) {
        table = &trmMenuTextTable[4];
    } else {
        table = trmMenuTextTable;
    }
    fclInitCursorWindow(commands, table, 4);
    fclSetCursorList(commands, 0x19);
    fclTerminalCommandCount[0] |= 2;
    *((s8 *)commands + 0x25) = 1;
}

typedef struct
{
    s16 field;
    s16 jumpTerminalNo;
} trmFieldEntry_t;

extern trmFieldEntry_t fldAllCnt[];
extern char D_0054FC28[]; /* "%x\n" */

s32 fclTerminalDumpJump(trmTerminalWork_t *work)
{
    s32 i;

    printf("jump:%x %x\n", fldAllCnt[work->terminalNo].jumpTerminalNo, work->terminalNo);
    for (i = 0; i < work->terminalCommandCount; i++) {
        printf(D_0054FC28, work->terminalList[i]);
        if (work->terminalList[i] == fldAllCnt[work->terminalNo].jumpTerminalNo) {
            return 1;
        }
    }
    return 0;
}

extern void *memset(void *dst, s32 value, u32 size);
extern void cmpSetCursorFlag(void *cursor, u8 *flags);
extern void fclTerminalInitMenu(s32 id, void *commands, void *work);

void fclTerminalResetCommands(s8 mode, u16 id, void *commands, void *work)
{
    memset(gCommandFlag, 0, 4);
    if (mode == 0) {
        fclTerminalInitMenu(id, commands, work);
    } else {
        fclInitCursorWindow(commands, &trmMenuTextTable[8], 4);
        fclSetCursorList(commands, 0x19);
        if (fclTerminalDumpJump(work) == 0) {
            gCommandFlag[0] |= 1;
        }
        gCommandFlag[2] |= 2;
        *((u8 *)commands + 0x25) = 1;
    }
    cmpSetCursorFlag((u8 *)commands + 4, gCommandFlag);
}

extern char *D_00330808[];

/* NON_MATCHING: 16 of 27 words match, 108 bytes against retail's 112; the `u8`
 * counter normalizes with `andi 0xff` where retail has sll 24 / sra 24. */
#ifdef NON_MATCHING
extern s32 fclLoadTexture(char *pFileName);

void fclTerminalLoadTextures(void *work)
{
    u8 i;
    s32 *tex = (s32 *)((u8 *)work + 0x2238);

    for (i = 0; i < 1U; i++) {
        *tex = fclLoadTexture(D_00330808[i]);
        tex++;
    }
    printf("TERMINAL : Tex load.\n");
}
#else
INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclTerminalLoadTextures);
#endif

void fclTerminalFreeTextures(void *work)
{
    s32 *tex = (s32 *)((u8 *)work + 0x2238);
    s8 i;

    for (i = 0; i < 1U; i++) {
        sdfTexFree(tex[i]);
    }
    printf("TERMINAL : Tex free.\n");
}

extern void fclTerminalScanCommands(trmTerminalWork_t *work);

extern void fclTerminalResetCommands(s8 mode, u16 id, void *commands, void *work);

extern void fclTerminalInitList(void *list, s8 count, s32 handle);

void fclTerminalSetMode(s32 mode, s32 id, trmUpdateWork_t *work)
{
    work->terminalType = mode;
    work->terminalNo = id;
    fclTerminalScanCommands(work);
    fclTerminalResetCommands(mode, id, (u8 *)work + 0x18, work);
    fclTerminalInitList((u8 *)work + 0x1058, work->terminalCommandCount, work->texHandle[0]);
}

extern sdfMemHandle_t *sdfMemAllocDirect(s32 size);
extern void *sdfMemLock(sdfMemHandle_t *handle);
extern void *memset(void *dst, s32 value, u32 size);
extern void cmpInitSeqInfo(void *work, s32 seq);
extern void fclTerminalLoadTextures(void *work);
extern void fclClearTerminalState(void);

trmTerminalWork_t *fclAllocTerminalWork(s32 type, s32 no, s32 mode)
{
    trmTerminalWork_t *work;
    sdfMemHandle_t *allocation;

    allocation = sdfMemAllocDirect(0x223C);
    work = sdfMemLock(allocation);
    memset(work, 0, 0x223C);
    work->allocation = allocation;
    cmpInitSeqInfo(work, mode);
    work->unk_01 = 0;
    work->unk_03 = 0;
    work->unk_04 = 1;
    work->ScriptPID = 0;
    work->scriptAllocationHandle = sdfDevLoadFileEx("/facility/scr/terminal/message.bf",
                                                      &work->scriptData, 0);
    fclSetTerminalMode(0);
    fclTerminalLoadTextures(work);
    fclTerminalSetMode(type, no, work);
    fclClearTerminalState();
    return work;
}

extern void sdfMemFreeDirect(void *handle);
extern void fldProcSetTerminal(s8 type, u16 id);
extern void fldProc_CallSisetuReturn(void);
extern s32 evtBitCheck(s32 eventNo);

void fclDestroyTerminal(dds3ProcessID_t *id)
{
    if (GBWK_3 != NULL) {
        gProcessStat_3 = 0;
        fclTerminalFreeTextures(GBWK_3);
        fldProcSetTerminal(GBWK_3->terminalType, GBWK_3->terminalNo);
        fclScriptForcedEnd((u8 *)GBWK_3->ScriptPID);
        sdfMemFreeDirect(GBWK_3->scriptAllocationHandle);
        sdfMemFreeDirect(GBWK_3->allocation);
        GBWK_3 = NULL;
        if (evtBitCheck(58) == 0) {
            fldProc_CallSisetuReturn();
        }
        trmEndBGM();
    }
}

extern dds3ProcessID_t *dds3StartProcess(char *name, u32 priority, s32 startTimer,
                                         s32 shutdownTimer,
                                         dds3ProcessFunc_t process,
                                         dds3ProcessFunc_t shutdown, void *data);
extern s32 trmCalc(dds3ProcessID_t *id);
extern s32 trmDraw(dds3ProcessID_t *id);
extern s32 trmUpdate(dds3ProcessID_t *id);
extern void FclDebug(s32 mode, void *work);
extern trmTerminalWork_t *fclAllocTerminalWork(s32 type, s32 no, s32 mode);

void fclTerminalProcessStart(s32 terminalType, s32 terminalNo)
{
    if (GBWK_3 == NULL) {
        trmTerminalWork_t *work =
            fclAllocTerminalWork(terminalType, terminalNo, 0);
        GBWK_3 = work;
        FclDebug(1, work);
        dds3StartProcess(D_0053D640, 0x404, 1, 1, trmCalc, NULL, GBWK_3);
        dds3StartProcess(D_0053D650, 0x2B14, 1, 1, trmDraw, NULL, GBWK_3);
        dds3StartProcess(D_0053D660, 0x5210, 1, 1, trmUpdate,
                         (dds3ProcessFunc_t)fclDestroyTerminal, GBWK_3);
        gCallMode = 0;
        gProcessStat_3 = 1;
    }
}

void fclTerminalProcessEnd(void)
{
    dds3KillProcessName(D_0053D640, 1);
    dds3KillProcessName(D_0053D650, 1);
    dds3KillProcessName(D_0053D660, 1);
}

s32 trmCheckProcessStat(void)
{
    switch (gProcessStat_3) {
    case 0:
        break;
    case 1:
    case 2:
        return 1;
    }
    return 0;
}

void func_002109E0(void)
{
    if (GBWK_3 != NULL) {
        GBWK_3->terminalFlags |= 0x10;
    }
}

extern void scrSetIntReturnValue(s32 value);

s32 trm_210A00(void)
{
    s32 result = 0;

    switch (gProcessStat_3) {
    case 0:
    case 1:
        gCallMode = 1;
        switch (trmGetMode()) {
        case 1:
            trmCreateTerminalList(GBWK_3->terminalType, GBWK_3->terminalNo);
            fclSetTerminalMode(2);
            break;
        case 3:
            fclSetTerminalMode(4);
            scrSetIntReturnValue(2);
            return 1;
        case 4:
            fclSetTerminalMode(2);
            GBWK_3->unk_01 = 0;
            GBWK_3->unk_04 = 1;
            break;
        }
        break;
    case 2:
        result = 1;
        break;
    }
    scrSetIntReturnValue(result ^ 1);
    return result;
}

s32 trm_210AF8(void)
{
    s32 index = scrGetIntPara(0);

    if ((u32)index < 4) {
        gCommandFlag[index] |= 1;
    }
    return 1;
}

s32 func_00210B38(void)
{
    u32 i;

    for (i = 0; i < 4; i++) {
        gCommandFlag[i] = 0;
    }
    return 1;
}

s32 func_00210B68(void)
{
    s32 count;

    fclTerminalCommandCount[0] &= 0xFD;
    count = GBWK_3->terminalReferenceCount;
    GBWK_3->terminalReferenceCount = count - 1;
    return 1;
}

s32 func_00210B90(void)
{
    s32 count;

    fclTerminalCommandCount[0] |= 2;
    count = GBWK_3->terminalReferenceCount;
    GBWK_3->terminalReferenceCount = count + 1;
    return 1;
}

void trmResetCursor(void)
{
    trmCalcWork->cursor = -1;
    trmCalcWork->pad_2217 = 0;
}

INCLUDE_RODATA(const s32, "facility/fclTerminalInit", D_0053D640);

INCLUDE_RODATA(const s32, "facility/fclTerminalInit", D_0053D650);

INCLUDE_RODATA(const s32, "facility/fclTerminalInit", D_0053D660);

INCLUDE_RODATA(const s32, "facility/fclTerminalInit", D_0053D670);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", GBWK_3);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", gProcessStat_3);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", trmMode);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", D_0054FC08);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", D_0054FC10);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", D_0054FC18);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", D_0054FC20);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", D_0054FC28);

INCLUDE_SDATA(const s32, "facility/fclTerminalInit", D_0054FC30);

