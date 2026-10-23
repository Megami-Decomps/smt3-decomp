#include "common.h"
#include "fpu.h"
#include "libsdf.h"
#include "newdata.h"
#include "ps2asm.h"
#include "event.h"

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
    /* 0x0000 */ u8 unk_0000[0x2216];
    /* 0x2216 */ s8 cursor;
    /* 0x2217 */ s8 unk_2217;
} trmCalcWork_t;

extern trmCalcWork_t *trmCalcWork;

extern u8 *GBWK_3;

extern s32 printf(const char *fmt, ...);

extern void sdfTexFree(s32 aHandle);

extern u8 fclTerminalCommandCount[1];

extern char D_0053D640[];

extern char D_0053D650[];

extern char D_0053D660[];

extern void fclScriptForcedEnd(u8 *PID);

extern s32 sprintf(char *out, const char *fmt, ...);

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

INCLUDE_ASM(const s32, "facility/fclTerminalInit", trmGetEventNo);

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

INCLUDE_ASM(const s32, "facility/fclTerminalInit", trmCreateTerminalList);

void trmEndBGM(void)
{
    p2sdStopBGM(0x20001);
}

INCLUDE_ASM(const s32, "facility/fclTerminalInit", trmStartEvent);

s32 trmEndEvent(s32 BgmMode)
{
    char name[0x20];

    if (BgmMode != 0) {
        trmEndBGM();
    }
    fclScriptForcedEnd(*(u8 **)(GBWK_3 + 0x2220));
    *(s32 *)(GBWK_3 + 0x2220) = 0;
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
        fclScriptForcedEnd(*(u8 **)(GBWK_3 + 0x2220));
        fclSetTerminalMode(5);
        gProcessStat_3 = 2;
        gCallMode = 0;
    }
}

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
    /* 0x21F4 */ s8 TerminalList[0x21];
    /* 0x2215 */ s8 unk_2215;
    /* 0x2216 */ u8 unk_2216[0xA];
    /* 0x2220 */ s32 ScriptPID;
    /* 0x2224 */ u8 unk_2224[0xC];
    /* 0x2230 */ s32 saveProcess;
    /* 0x2234 */ u8 unk_2234[4];
    /* 0x2238 */ s32 texHandle[1];
} trmUpdateWork_t;

extern trmUpdateWork_t *trmUpdateWork;

INCLUDE_RODATA(const s32, "facility/fclTerminalInit", D_0053D5B0);

INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclTerminalScanCommands);

INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclTerminalInitList);

extern u32 trmMenuTextTable[];

/* NON_MATCHING: 4 of 31 words differ (first at +0x54). */
#ifdef NON_MATCHING
void fclTerminalInitMenu(s32 a, void *work, s32 slot)
{
    u32 *table;

    if (trmGetEventNo(1, slot) == 0x296) {
        table = &trmMenuTextTable[4];
    } else {
        table = trmMenuTextTable;
    }
    fclInitCursorWindow(work, table, 4);
    fclSetCursorList(work, 0x19);
    fclTerminalCommandCount[0] |= 2;
    *((s8 *)work + 0x25) = 1;
}
#else
INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclTerminalInitMenu);
#endif

INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclTerminalDumpJump);

INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclTerminalResetCommands);

extern char *D_00330808[];

/* NON_MATCHING: 16 of 27 words match, 108 bytes against retail's 112; the `u8`
 * counter normalizes with `andi 0xff` where retail has sll 24 / sra 24. */
#ifdef NON_MATCHING
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

extern void fclTerminalScanCommands(void *work);

extern void fclTerminalResetCommands(s8 mode, u16 id, void *commands, void *work);

extern void fclTerminalInitList(void *list, s8 count, s32 handle);

void fclTerminalSetMode(s32 mode, s32 id, trmUpdateWork_t *work)
{
    work->TerminalType = mode;
    work->TerminalNo = id;
    fclTerminalScanCommands(work);
    fclTerminalResetCommands(mode, id, (u8 *)work + 0x18, work);
    fclTerminalInitList((u8 *)work + 0x1058, work->unk_2215, work->texHandle[0]);
}

INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclAllocTerminalWork);

INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclDestroyTerminal);

INCLUDE_ASM(const s32, "facility/fclTerminalInit", fclTerminalProcessStart);

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
        *GBWK_3 |= 0x10;
    }
}

INCLUDE_ASM(const s32, "facility/fclTerminalInit", trm_210A00);

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
    count = GBWK_3[0x3D];
    GBWK_3[0x3D] = count - 1;
    return 1;
}

s32 func_00210B90(void)
{
    s32 count;

    fclTerminalCommandCount[0] |= 2;
    count = GBWK_3[0x3D];
    GBWK_3[0x3D] = count + 1;
    return 1;
}

void trmResetCursor(void)
{
    trmCalcWork->cursor = -1;
    trmCalcWork->unk_2217 = 0;
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

