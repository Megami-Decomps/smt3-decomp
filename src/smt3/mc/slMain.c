#include "common.h"
#include "basic.h"
#include "model.h"
#include "newdata.h"
#include "scr.h"
#include "field.h"
#include "ps2asm.h"

typedef struct fldFileRequest_t fldFileRequest_t;

enum sl_mc_result
{
    SL_MC_RESULT_PENDING = 0,
    SL_MC_RESULT_SUCCESS = 1,
    SL_MC_RESULT_ERROR = -1,
    SL_MC_RESULT_SPECIAL_ERROR = -2
};

typedef enum sl_mc_result slMcResult_t;

enum sl_slot_flag
{
    SL_SLOT_FLAG_CAN_READ_SAVE = 1,
    SL_SLOT_FLAG_DIRECTORY_READY = 2,
    SL_SLOT_FLAG_DELETE_BEFORE_WRITE = 8
};

void sdfMemFreeDirect(sdfMemHandle_t *allocation);

extern void rsetOtOfs(u32);

extern void rsetAlpha(s32);

dds3ProcessID_t *dds3SearchProcessName(const char *name);

extern s32 slProcShutDownFlg;

extern s32 gSlWindowCnt;

extern s32 gslInfoCode;

extern s32 slChooseSaveIndex;

extern u32 D_005516BC;

void sdfMcSetSlotIndex(s32 request, s32 selectedSlot);

s8 sdfMcGetSlotIndex(s32 request);

u32 sdfMcGetSlotFlags(s32 request, s32 slot);

extern s32 slChooseData_getCardInfoNext(void);

extern void mcMakeFileName(char *dst, s32 number);

extern u32 strlen(const char *s);

extern void *memcpy(void *, const void *, unsigned int);

void sdfMcFuncOpen(u32 context, const char *path, s32 openFlags);

s32 slHeadLoadProc_openFile(void);

extern s32 D_00551D50;

slMcResult_t sdfMcFuncOpen1(s32 *resultOut);

sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes);

void *sdfMemLock(sdfMemHandle_t *allocation);

extern sdfMemHandle_t *slLoadFileFd;

extern void sdfMcFuncRead(s32, u32, s32);

extern void *slLoadFileHandle;

s32 slHeadLoadProc_readFile(void);

extern s32 slNoCard(void);

slMcResult_t sdfMcFuncRead1(void);

extern void sdfMcFuncClose(s32);

extern s32 slHeadLoadProc_closeFile(void);

void dds3FadeOut(s32 red, s32 green, s32 blue, s32 duration);

extern void *slExitWaitNextFunc;

s32 slExitWaitProc(void);

extern s32 slExitWaitCnt;

extern s32 gslMode;

extern s32 slTop(void);

s32 slEndingExit(void);

extern u32 gslErrorCode;

extern u32 slQueryYesNoMsg;

extern u32 gslDrawFlagFile;

s32 slQueryYesNo(void *start, void *finish, s32 mode);

s32 slSaveData(void);

extern s32 slFileSel(void);

s32 slExitToTitle(void);

s32 slBootProc_CheckCard(void);

void sdfMcRegetInfo(s32 request);

extern s32 gslCheckCardWait;

extern u32 gSlNowCardCheck;

s32 slBootProc_CheckCardR(void);

extern u32 gslDrawFlagBg;

s32 slExitToTitleSub(void);

extern char D_005328C0[];

s32 slExitToTitle2(void);

extern char D_005328D0[];

fldFileRequest_t *fileLoad(const char *requestName);

extern sdfMemHandle_t *D_00551D44;

extern u32 baseIconFileHandle;

s32 sdfMcGetFreeBytes(s32 request);

s32 sdfMcFuncGetInfo1(void);

u8 sdfMcGetFileInfoReady(s32 request);

extern s32 slChooseData_B(void);

extern u32 gslErrorKeyFlag;

extern s8 dds3PadButtonState[];

void p2sdPlaySE(s32 trackId, s32 volume, s32 pan);

extern u32 slHitAnyKeyNextFunc;

extern s32 slHitAnyKeyNoChkProc(void);

extern s32 slHitAnyKeyProc(void);

void sdfMcFuncChdir(u32 context, const char *path);

s32 slChooseData_getCardInfo2(void);

slMcResult_t sdfMcFuncChdir1(void);

void sdfMcSetSlotFlags(s32 request, s32 slot, s32 mask);

extern char D_005328E0[];

extern u8 D_005696C0[];

void sdfMcFuncGetDir(u32 context, const char *path, void *directoryEntries, s32 entryLimit);

s32 slChooseData_getCardInfo3(void);

slMcResult_t sdfMcFuncGetDir1(s32 *resultOut);

s32 slChooseData_getCardInfo2_B(void);

s32 slChooseData_getCardInfo3_B(void);

s32 slChooseData_getCardInfoNext_B(void);

void sdfMcSetCardStat(s32 request);

s32 slSaveData_chdir(void);

void sdfMcFuncMkdir(u32 context, const char *path);

s32 slSaveProc_Write_mkdir(void);

s32 slSaveProc_Write_chdir(void);

slMcResult_t sdfMcFuncMkdir1(void);

extern s32 slFileSelError;

s32 slSaveData_deleteIconSys(void);

slMcResult_t sdfMcFuncDelete1(void);

extern s32 slSaveData_writeFile(void);

void slKillLoadProcess(void);

extern char D_005328F0[];

extern s32 slSaveWriteEnd_baseIcon(void);

extern u32 D_00551D48;

extern u32 D_00551D4C;

s32 slSaveWriteFile(const char *fileName, u32 *dataAddressSlot, u32 *dataSizeSlot, void *continuation, u32 *pendingRequestStatus);

void sdfMcFuncDelete(u32 context, const char *path);

extern u32 *slSaveWriteFile_buff;

extern u32 *slSaveWriteFile_size;

extern void *slSaveWriteFile_endFunc;

extern u32 *slSaveWriteFile_dataWaitPtr;

s32 slSaveProc_openFile(void);

s32 slSaveProc_waitDataReady(void);

extern void sdfMcFuncWrite(s32 request, u32 first, u32 second);

s32 slSaveProc_writeFile(void);

slMcResult_t sdfMcFuncWrite1(void);

s32 slSaveProc_closeFile(void);

slMcResult_t sdfMcFuncClose1(void);

slMcResult_t sdfMcFuncFormat1(void);

extern void dds3SoftResetOn(void);

s32 slCardChk(void);

extern void dds3SoftResetOff(void);

void sdfMcFuncFormat(u32 port);

extern u32 gSlEndingMode;

void slVersionConvert_Step1(void);

s32 slLoadProc_openFile(void);

s32 slLoadProc_readFile(void);

s32 slLoadProc_closeFile(void);

void slVersionConvert_Step3(void);

extern s8 D_005516C1;

extern s32 dds3ConfigChanged(void);

extern void dds3ConfigSaveBackup(void);

extern void dds3ConfigRestoreBackup(void);

extern s32 gslAnimCnt;

extern s32 slDraw(s32);

s32 pacFileCheckEnd(const fldFileRequest_t *request);

sdfMemHandle_t *fileGetMemHandle(const fldFileRequest_t *request);

u8 *fileGetLoadPtr(const fldFileRequest_t *request);

u32 fileGetLoadSize(const fldFileRequest_t *request);

s32 fileHandleFree(fldFileRequest_t *request);

extern char D_00532948[];

extern u32 gslExitType;

extern void *slQueryYesFunc;

extern void *slQueryNoFunc;

extern s32 slQueryYesNoCursor;

extern s32 slQueryYesNoKeyFlag;

extern s32 slQueryYesNoProc(void);

extern u32 gSlExitMode;

extern s32 (*proc_func_ans)(dds3ProcessID_t *process);

extern void *frwGetSmallFontFRQ(s32 x, s32 y, s32 mode, u32 rgba, const char *text, s32 flag);

extern void *frwGetSmallNumFontFRQ(s32 x, s32 y, s32 mode, u32 rgba, const char *text, s32 flag);

extern void frDrawFont(void *request);

extern void frDrawFontEx(void *request, s32 mode);

extern void frDestroyFontRequestAfter(void *request);

extern void *slKanjiDraw_handle;

extern s32 slDDS3To31Flg;

extern s32 sprintf(char *dst, const char *format, ...);

extern char D_00532860[];

extern s8 D_00569743[];

extern s32 version_convert_ver;

extern u8 D_00316B00[];

extern u32 D_0054F06C;

extern u32 D_0054F070;

extern void dds3AdminiCallSequence(s32 kind, s32 *argument, s32 mode, s32 flag);

extern void frSetFontDrawOT(s32 ot);

extern void sdfMcClearSlotFlags(s32 request, s32 slot);

extern s32 slChooseData_getCardInfo_B(void);

extern s32 slTopMenuSel[];

extern f32 slNextPageDraw_fc;

extern f32 sdfSin(f32 radians);

extern void DrwSprite2DF(f32 x, f32 y, f32 w, f32 h, s32 u, s32 v, s32 uw, s32 vh,
                         s32 color, sdfTexHandle_t *texture);

extern sdfTexHandle_t *slProcPtr;

extern s32 slErrorWaitMc(void);

extern s32 gSlSelCnt;

extern s32 slSaveProc_CheckCard(void);

extern s32 slChooseData(void);

extern s32 slFormatStart(void);

extern s32 slFormatCancel(void);

extern void frEnableMode(s32 mode);

extern void frDisableMode(s32 mode);

extern void *frSetFontString(const char *text, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

extern void frSetFontSpace(void *request, s32 space);

extern void frSetDrawPosition(void *request, s32 x, s32 y);

extern void frSetFontColor(void *request, u32 rgba);

extern void func_001C7E38(void *request, s32 a1, s32 a2);

extern void *slKanjiDrawH_handle;


void mcMakeFileName(char *dst, s32 number) {
    if (slDDS3To31Flg != 0) {
        sprintf(dst, D_00532860, 0xFEDA, number);
    } else {
        sprintf(dst, D_00532860, 0x51AF, number);
    }
}

void slVersionConvert_Step1(void) {
    version_convert_ver = *(D_00569743 + sdfMcGetSlotIndex(D_005516BC) * 52);
}

/* Save revisions have different persistent block extents. Keeping this
 * earlier provider in C also exposes the native same-TU call visibility to
 * slLoadProc_openFile (DDS idioms: earlier definitions and branch-likely). */
u32 slVersionConvert_GetSize(void) {
    if (version_convert_ver == 1) {
        return 0xF990;
    }
    if (version_convert_ver == 2) {
        return 0xFA30;
    }
    return (u32)(version_convert_ver - 3) < 2 ? 0x16DC0 : 0x1A510;
}

INCLUDE_RODATA(const s32, "mc/slMain", D_00532860);

INCLUDE_ASM(const s32, "mc/slMain", slVersionConvert_Step3);

u8 slGetPadTrig(s32 condition) {
    return condition != 0 && slProcShutDownFlg == 1;
}

u8 slGetPadRep(s32 loaded) {
    return loaded != 0 && slProcShutDownFlg == 1;
}


/* NON_MATCHING: 14 of 16 words, exact size; the trailing store and src++ are swapped. */
#ifdef NON_MATCHING
void slCopyNonZero(u8 *dst, u8 *src, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (*(s8 *)src == 0) {
            src++;
        } else {
            *dst = *src;
            dst++;
            src++;
        }
        *dst = *src;
        dst++;
        src++;
    }
    *dst = 0;
}
#else
INCLUDE_ASM(const s32, "mc/slMain", slCopyNonZero);
#endif


void slKanjiDraw(s32 x, s32 y, u32 rgba, const char *text) {
    slKanjiDraw_handle = frwGetSmallFontFRQ(x << 4, y << 4, 0, rgba, text, 0);
    frDrawFont(slKanjiDraw_handle);
    frDestroyFontRequestAfter(slKanjiDraw_handle);
}

void slKanjiDrawH(s32 x, s32 y, u32 rgba, const char *text) {
    frEnableMode(1);
    slKanjiDrawH_handle = frSetFontString(text, 0, 0, 0, 0, 0);
    frDisableMode(1);
    frSetFontSpace(slKanjiDrawH_handle, 3);
    frSetDrawPosition(slKanjiDrawH_handle, x << 4, y << 4);
    frSetFontColor(slKanjiDrawH_handle, rgba);
    func_001C7E38(slKanjiDrawH_handle, 0, 0x4A);
    frDestroyFontRequestAfter(slKanjiDrawH_handle);
    frSetFontDrawOT(0x48);
}

void slKanjiDrawN(s32 x, s32 y, u32 rgba, const char *text) {
    void *request = frwGetSmallNumFontFRQ(x << 4, y << 4, 0, rgba, text, 0);

    frDrawFontEx(request, 1);
    frDestroyFontRequestAfter(request);
}

void slNextPageDraw(void) {
    f32 phase;

    rsetOtOfs(0x4A);
    rsetAlpha(0);
    phase = slNextPageDraw_fc + 0.4f;
    slNextPageDraw_fc = phase;
    DrwSprite2DF(448.0f, sdfSin(phase) + 308.0f, 25.0f, 18.0f, 0xDB, 0x2D, 0x19, 0x12,
                 0x80808080, slProcPtr);
}

INCLUDE_ASM(const s32, "mc/slMain", slMcAccessDraw);

s32 slChkWindowOpenDone(void) {
    if (gSlWindowCnt < 7) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "mc/slMain", slDrawWindow);

INCLUDE_ASM(const s32, "mc/slMain", slDrawError);

void slSetInfo(u32 state) {
    s32 previous;

    previous = gslInfoCode;
    gslInfoCode = state;
    if (previous == 0) {
        gSlWindowCnt = 0;
    }
}

INCLUDE_ASM(const s32, "mc/slMain", slDrawInfo);

s32 slHeadLoad(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    sdfMcSetSlotIndex(D_005516BC, slChooseSaveIndex);
    ctx = D_005516BC;
    slot = sdfMcGetSlotIndex(ctx);
    if ((sdfMcGetSlotFlags(ctx, slot) & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slChooseData_getCardInfoNext();
    }
    path[0] = 0x2F;
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return (s32)slHeadLoadProc_openFile;
}

s32 slHeadLoadProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        slLoadFileFd = sdfMemAllocDirect(0x34);
        slLoadFileHandle = sdfMemLock(slLoadFileFd);
        sdfMcFuncRead(D_00551D50, (u32)slLoadFileHandle, 0x34);
        return (s32)slHeadLoadProc_readFile;
    }
    return slNoCard();
}

s32 slHeadLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slHeadLoadProc_closeFile;
    }
    sdfMemFreeDirect(slLoadFileFd);
    return slNoCard();
}

INCLUDE_ASM(const s32, "mc/slMain", slHeadLoadProc_closeFile);

s32 slExitWait(void *callback) {
    dds3FadeOut(0, 0, 0, 15);
    slExitWaitNextFunc = callback;
    slExitWaitCnt = 20;
    return (s32)slExitWaitProc;
}

s32 slEndingTop(void) {
    gslMode = 0;
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 4;
    gslDrawFlagFile = 0;
    return slQueryYesNo(&slTop, &slEndingExit, 0);
}

s32 slEndingUpd2(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 9;
    return slQueryYesNo(&slSaveData, &slFileSel, 1);
}

s32 slEndingExit(void) {
    slSetInfo(0);
    gslErrorCode = 0;
    slQueryYesNoMsg = 8;
    gslDrawFlagFile = 0;
    return slQueryYesNo(&slExitToTitle, &slEndingTop, 1);
}

s32 slBootTop(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return (s32)slBootProc_CheckCard;
}

s32 slBootRetry(void) {
    sdfMcRegetInfo(0);
    gslCheckCardWait = 30;
    gSlNowCardCheck = 1;
    slSetInfo(1);
    gslDrawFlagFile = 1;
    return (s32)slBootProc_CheckCardR;
}

s32 slBootError(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 5;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

s32 slBootError2(void) {
    gslDrawFlagBg = 1;
    slSetInfo(0);
    slQueryYesNoMsg = 6;
    return slQueryYesNo(&slExitToTitle, &slBootRetry, 1);
}

s32 slNoCard(void) {
    sdfMcSetCardStat(D_005516BC);
    sdfMcClearSlotFlags(D_005516BC, 0);
    sdfMcClearSlotFlags(D_005516BC, 1);
    sdfMcClearSlotFlags(D_005516BC, 2);
    sdfMcClearSlotFlags(D_005516BC, 3);
    sdfMcClearSlotFlags(D_005516BC, 4);
    slSetInfo(0);
    gslErrorCode = 1;
    gslErrorKeyFlag = 0;
    sdfMcRegetInfo(D_005516BC);
    return (s32)slErrorWaitMc;
}

s32 slTop(void) {
    gSlSelCnt = 0;
    sdfMcSetCardStat(D_005516BC);
    sdfMcClearSlotFlags(D_005516BC, 0);
    sdfMcClearSlotFlags(D_005516BC, 1);
    sdfMcClearSlotFlags(D_005516BC, 2);
    sdfMcClearSlotFlags(D_005516BC, 3);
    sdfMcClearSlotFlags(D_005516BC, 4);
    slTopMenuSel[0] = 0;
    slTopMenuSel[1] = 0;
    slTopMenuSel[2] = 0;
    slTopMenuSel[3] = 0;
    slTopMenuSel[4] = 0;
    sdfMcRegetInfo(0);
    gslCheckCardWait = 15;
    gSlNowCardCheck = 1;
    slSetInfo(1);
    gslDrawFlagFile = 1;
    return (s32)slSaveProc_CheckCard;
}

s32 slExit(void) {
    slSetInfo(0);
    gslDrawFlagFile = 0;
    gslDrawFlagBg = 0;
    return 0xffffffff;
}

s32 slExitToTitleSub(void) {
    s32 argument = 0;

    if (dds3SearchProcessName(D_005328C0) != NULL) {
        return -1;
    }
    if (slDDS3To31Flg != 0) {
        return -1;
    }
    dds3AdminiCallSequence(2, &argument, 4, 0);
    return 0;
}

s32 slExitToTitle(void) {
    return slExitToTitleSub();
}

s32 slExitToTitle2(void) {
    s32 argument = 2;

    gslExitType = 1;
    if (dds3SearchProcessName(D_005328C0) != NULL) {
        return -1;
    }
    if (slDDS3To31Flg != 0) {
        return -1;
    }
    dds3AdminiCallSequence(2, &argument, 4, 0);
    return 0;
}

s32 slCardChk(void) {
    sdfMcSetCardStat(D_005516BC);
    sdfMcClearSlotFlags(D_005516BC, 0);
    sdfMcClearSlotFlags(D_005516BC, 1);
    sdfMcClearSlotFlags(D_005516BC, 2);
    sdfMcClearSlotFlags(D_005516BC, 3);
    sdfMcClearSlotFlags(D_005516BC, 4);
    slSetInfo(1);
    gslErrorCode = 0;
    gslDrawFlagFile = 0;
    return slTop();
}

INCLUDE_ASM(const s32, "mc/slMain", slFileSel);

s32 slCancel(void) {
    if (gslMode == 1 && dds3SearchProcessName(D_005328C0) == NULL) {
        return slExitToTitle2();
    }
    slSetInfo(0);
    return slExitWait(&slExit);
}

s32 slSaveProc1st(void) {
    D_00551D44 = 0;
    baseIconFileHandle = (u32)fileLoad(D_005328D0);
    return slTop();
}

s32 slLoadProc1st(void) {
    return slTop();
}

s32 slEndingProc1st(void) {
    D_00551D44 = 0;
    baseIconFileHandle = (u32)fileLoad(D_005328D0);
    return slEndingTop();
}

s32 slBootProc1st(void) {
    sdfMcRegetInfo(0);
    gSlNowCardCheck = 1;
    gslCheckCardWait = 0;
    slSetInfo(0);
    gslDrawFlagFile = 0;
    return slBootTop();
}

s32 slSaveProc_CheckCard(void) {
    if (sdfMcFuncGetInfo1() == 0) {
        return 0;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(D_005516BC);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(D_005516BC)) {
    case 0:
        return slNoCard();
    case 2:
        if (gslMode == 0) {
            sdfMcSetCardStat(D_005516BC);
            sdfMcClearSlotFlags(D_005516BC, 0);
            sdfMcClearSlotFlags(D_005516BC, 1);
            sdfMcClearSlotFlags(D_005516BC, 2);
            sdfMcClearSlotFlags(D_005516BC, 3);
            sdfMcClearSlotFlags(D_005516BC, 4);
            slSetInfo(0);
            gslErrorCode = 0;
            slQueryYesNoMsg = 2;
            return slQueryYesNo(&slFormatStart, &slFormatCancel, 1);
        }
        slSetInfo(0);
        gslErrorCode = 7;
        gslErrorKeyFlag = 0;
        sdfMcRegetInfo(D_005516BC);
        return (s32)slErrorWaitMc;
    case 3:
        sdfMcSetCardStat(D_005516BC);
        sdfMcClearSlotFlags(D_005516BC, 0);
        sdfMcClearSlotFlags(D_005516BC, 1);
        sdfMcClearSlotFlags(D_005516BC, 2);
        sdfMcClearSlotFlags(D_005516BC, 3);
        sdfMcClearSlotFlags(D_005516BC, 4);
        slSetInfo(0);
        gslErrorCode = 2;
        gslErrorKeyFlag = 0;
        sdfMcRegetInfo(D_005516BC);
        return (s32)slErrorWaitMc;
    case 1:
        return slChooseData();
    }
    return 0;
}

s32 sl1Ken(void) {
    s32 i = 0;
    s32 count = 0;

    do {
        u32 flags = sdfMcGetSlotFlags(D_005516BC, i);

        i++;
        if (flags & SL_SLOT_FLAG_CAN_READ_SAVE) {
            if (flags & SL_SLOT_FLAG_DIRECTORY_READY) {
                if (flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE) {
                    count++;
                }
            }
        }
    } while (i < 5);
    return count;
}

s32 slSizeChk(void) {
    return sdfMcGetFreeBytes(D_005516BC) > 0x293FF;
}

s32 slBootProc_CheckCard(void) {
    s32 result = sdfMcFuncGetInfo1();
    if (result == 0) {
        return result;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(D_005516BC);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(D_005516BC)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return slExitToTitle();
    case 1:
        if (slSizeChk() != 0) {
            return slExitToTitle();
        }
        return (s32)slBootRetry();
    default:
        return 0;
    }
}

s32 slBootProc_CheckCardR(void) {
    if (sdfMcFuncGetInfo1() == 0) {
        return 0;
    }
    if (gslCheckCardWait > 0) {
        gslCheckCardWait--;
        sdfMcRegetInfo(D_005516BC);
        return 0;
    }
    switch (sdfMcGetFileInfoReady(D_005516BC)) {
    case 0:
    case 3:
        return slBootError();
    case 2:
        return slExitToTitle();
    case 1:
        if (slSizeChk() != 0) {
            return slExitToTitle();
        }
        return slChooseData_B();
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "mc/slMain", slErrorWaitMc);

s32 slErrorWaitToFileMc(void) {
    if (gslErrorKeyFlag == 0) {
        if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
            slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
            p2sdPlaySE(8, 0x7F, 0x3F);
            gslErrorKeyFlag = 1;
        }
    }
    if (sdfMcFuncGetInfo1() != 0) {
        if (gslErrorKeyFlag == 1) {
            gslErrorCode = 0;
            return slFileSel();
        }
        if (sdfMcGetFileInfoReady(D_005516BC) != 1) {
            gslErrorCode = 0;
            gslDrawFlagFile = 0;
            return slTop();
        }
        sdfMcRegetInfo(D_005516BC);
    }
    return 0;
}

s32 slErrorWait(void) {
    if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
        slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        gslErrorCode = 0;
        gslDrawFlagFile = 0;
        return slTop();
    }
    return 0;
}

s32 slExitWaitProc(void) {
    s32 remaining = slExitWaitCnt - 1;
    slExitWaitCnt = remaining;
    if (remaining <= 0) {
        if (slHitAnyKeyNextFunc == -1) {
            return -1;
        }
        return ((s32 (*)())slExitWaitNextFunc)();
    }
    return 0;
}

s32 slHitAnyKeyNoChk(u32 callback) {
    slHitAnyKeyNextFunc = callback;
    gslErrorKeyFlag = 0;
    return (s32)slHitAnyKeyNoChkProc;
}

s32 slHitAnyKeyNoChkProc(void) {
    if (slGetPadTrig(dds3PadButtonState[0x21] < 0) ||
        slGetPadTrig(dds3PadButtonState[0x23] < 0)) {
        slSetInfo(0);
        gslErrorCode = 0;
        gslDrawFlagFile = 0;
        if (slHitAnyKeyNextFunc == -1) {
            return slExitWait((void *)-1);
        }
        return ((s32 (*)())slHitAnyKeyNextFunc)();
    }
    return 0;
}

s32 slHitAnyKey(u32 callback) {
    slHitAnyKeyNextFunc = callback;
    gslErrorKeyFlag = 0;
    sdfMcRegetInfo(D_005516BC);
    return (s32)slHitAnyKeyProc;
}

INCLUDE_ASM(const s32, "mc/slMain", slHitAnyKeyProc);

s32 slChooseData_getCardInfo(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slChooseSaveIndex);
    sdfMcFuncChdir(D_005516BC, (const char *)buf);
    return (s32)slChooseData_getCardInfo2;
}

s32 slChooseData_getCardInfo2(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(D_005516BC, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(D_005516BC, D_005328E0, D_005696C0, 1);
        return (s32)slChooseData_getCardInfo3;
    }
    if (t == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

s32 slChooseData_getCardInfo3(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(D_005516BC, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slHeadLoad();
    }
    if (status == SL_MC_RESULT_ERROR) {
        return slNoCard();
    }
    return slHeadLoad();
}

INCLUDE_ASM(const s32, "mc/slMain", slChooseData_getCardInfoNext);

INCLUDE_ASM(const s32, "mc/slMain", slChooseData);

s32 slChooseData_getCardInfo_B(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slChooseSaveIndex);
    sdfMcFuncChdir(D_005516BC, (const char *)buf);
    return (s32)slChooseData_getCardInfo2_B;
}

s32 slChooseData_getCardInfo2_B(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(D_005516BC, slChooseSaveIndex, SL_SLOT_FLAG_DIRECTORY_READY);
        sdfMcFuncGetDir(D_005516BC, D_005328E0, D_005696C0, 1);
        return (s32)slChooseData_getCardInfo3_B;
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfo3_B(void) {
    s32 value;
    slMcResult_t status = sdfMcFuncGetDir1(&value);
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (value == 1) {
            sdfMcSetSlotFlags(D_005516BC, slChooseSaveIndex,
                                SL_SLOT_FLAG_CAN_READ_SAVE | SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
        }
        return slChooseData_getCardInfoNext_B();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError();
    }
    return slChooseData_getCardInfoNext_B();
}

s32 slChooseData_getCardInfoNext_B(void) {
    s32 slot = slChooseSaveIndex;
    s32 buttons = sdfMcGetSlotFlags(D_005516BC, slot);

    slot++;
    if (buttons & SL_SLOT_FLAG_CAN_READ_SAVE) {
        if (buttons & SL_SLOT_FLAG_DIRECTORY_READY) {
            if (buttons & SL_SLOT_FLAG_DELETE_BEFORE_WRITE) {
                slSetInfo(0);
                gslDrawFlagFile = 0;
                gslErrorCode = 0;
                return slExitToTitle();
            }
        }
    }
    slChooseSaveIndex = slot;
    if (slot == 5) {
        slSetInfo(0);
        gslDrawFlagFile = 0;
        gslErrorCode = 0;
        return slBootError2();
    }
    return slChooseData_getCardInfo_B();
}

s32 slChooseData_B(void) {
    s32 i = 0;
    s32 *selection;

    slSetInfo(1);
    gslDrawFlagFile = 0;
    slChooseSaveIndex = 0;
    sdfMcSetCardStat(D_005516BC);
    selection = slTopMenuSel;
    do {
        *selection = 0;
        selection++;
        sdfMcClearSlotFlags(D_005516BC, i);
        i++;
    } while (i < 5);
    return slChooseData_getCardInfo_B();
}

s32 slSaveData(void) {
    u8 name[0x50];
    u32 entry = D_005516BC;
    s32 slot = sdfMcGetSlotIndex(entry);
    sdfMcGetSlotFlags(entry, slot);
    slSetInfo(2);
    sdfMcSetCardStat(entry);
    if (sdfMcGetSlotFlags(entry, slot) & SL_SLOT_FLAG_DIRECTORY_READY) {
        return slSaveData_chdir();
    }
    name[0] = '/';
    mcMakeFileName(name + 1, slot);
    sdfMcFuncMkdir(entry, (const char *)name);
    return (s32)slSaveProc_Write_mkdir;
}

s32 slSaveData_chdir(void) {
    u8 buf[0x50];
    u32 entry = D_005516BC;
    s32 slot = sdfMcGetSlotIndex(entry);

    buf[0] = 0x2F;
    mcMakeFileName(&buf[1], slot);
    sdfMcFuncChdir(entry, (const char *)buf);
    return (s32)slSaveProc_Write_chdir;
}

s32 slSaveProc_Write_mkdir(void) {
    slMcResult_t status = sdfMcFuncMkdir1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcSetSlotFlags(D_005516BC, slFileSelError, SL_SLOT_FLAG_DIRECTORY_READY);
        return slSaveData_chdir();
    }
    slSetInfo(0);
    gslErrorCode = 4;
    return (s32)slErrorWait;
}

s32 slSaveProc_Write_chdir(void) {
    slMcResult_t t = sdfMcFuncChdir1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        return slSaveData_deleteIconSys();
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_Write_deleteIconSys(void) {
    slMcResult_t t = sdfMcFuncDelete1();

    if (t == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (t == SL_MC_RESULT_SUCCESS) {
        return slSaveData_writeFile();
    }
    if (t == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_doNothing(void) {
    sdfMcSetSlotFlags(D_005516BC, slFileSelError, SL_SLOT_FLAG_CAN_READ_SAVE);
    sdfMcSetSlotFlags(D_005516BC, slFileSelError, SL_SLOT_FLAG_DELETE_BEFORE_WRITE);
    slSetInfo(4);
    return slHitAnyKey((u32)slFileSel);
}

s32 slSaveWriteEnd_iconSys(void) {
    slKillLoadProcess();
    return (s32)slSaveProc_doNothing;
}

s32 slSaveWriteEnd_baseIcon(void) {
    D_00316B00[0xE2] = -0x7E;
    D_00316B00[0xE3] = slFileSelError + 0x50;
    D_00316B00[0xE4] = 0;
    D_00316B00[0xE5] = 0;
    return slSaveWriteFile(D_005328E0, &D_0054F06C, &D_0054F070, slSaveWriteEnd_iconSys, 0);
}

s32 slSaveWriteEnd_saveData(void) {
    return slSaveWriteFile(D_005328F0, &D_00551D48, &D_00551D4C,
                          slSaveWriteEnd_baseIcon, &baseIconFileHandle);
}

INCLUDE_ASM(const s32, "mc/slMain", slSaveData_writeFile);

s32 slSaveData_deleteIconSys(void) {
    u32 entry = D_005516BC;
    s32 slot = sdfMcGetSlotIndex(entry);
    u32 flags = sdfMcGetSlotFlags(entry, slot);
    if (!(flags & SL_SLOT_FLAG_DELETE_BEFORE_WRITE)) {
        return slSaveData_writeFile();
    }
    sdfMcFuncDelete(entry, D_005328E0);
    return (s32)slSaveProc_Write_deleteIconSys;
}

s32 slSaveWriteFile(const char *fileName, u32 *dataAddressSlot, u32 *dataSizeSlot, void *continuation, u32 *pendingRequestStatus) {
    slSaveWriteFile_buff = dataAddressSlot;
    slSaveWriteFile_size = dataSizeSlot;
    slSaveWriteFile_endFunc = continuation;
    slSaveWriteFile_dataWaitPtr = pendingRequestStatus;
    sdfMcFuncOpen(D_005516BC, fileName, 0x203);
    return (s32)slSaveProc_openFile;
}

s32 slSaveProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        if (slSaveWriteFile_dataWaitPtr != NULL && *slSaveWriteFile_dataWaitPtr != 0) {
            return (s32)slSaveProc_waitDataReady;
        }
        sdfMcFuncWrite(D_00551D50, *slSaveWriteFile_buff, *slSaveWriteFile_size);
        return (s32)slSaveProc_writeFile;
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_writeFile(void) {
    slMcResult_t status = sdfMcFuncWrite1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slSaveProc_closeFile;
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        return ((s32 (*)())slSaveWriteFile_endFunc)();
    }
    if (status == SL_MC_RESULT_ERROR) {
        slSetInfo(0);
        gslErrorCode = 4;
        return (s32)slErrorWait;
    }
    return 0;
}

s32 slSaveProc_waitDataReady(void) {
    if (baseIconFileHandle != 0) {
        return 0;
    }
    sdfMcFuncWrite(D_00551D50, *slSaveWriteFile_buff, *slSaveWriteFile_size);
    return (s32)slSaveProc_writeFile;
}

s32 slProc_Format(void) {
    slMcResult_t status = sdfMcFuncFormat1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    dds3SoftResetOn();
    if (status < 0) {
        slSetInfo(0);
        gslErrorCode = 6;
        return (s32)slErrorWait;
    }
    slSetInfo(6);
    return slHitAnyKey((u32)slCardChk);
}

s32 slFormatStart(void) {
    slSetInfo(5);
    dds3SoftResetOff();
    sdfMcFuncFormat(D_005516BC);
    return (s32)slProc_Format;
}

s32 slFormatCancel(void) {
    if (gSlEndingMode != 0) {
        slQueryYesNoMsg = 7;
        return slQueryYesNo(slEndingExit, slTop, 1);
    }
    slQueryYesNoMsg = 7;
    return slQueryYesNo(slExit, slTop, 1);
}

s32 slLoadData(void) {
    char path[0x50];
    u32 ctx = D_005516BC;
    s32 slot = sdfMcGetSlotIndex(ctx);
    u32 attr = sdfMcGetSlotFlags(ctx, slot);
    u32 len;

    slSetInfo(3);
    if ((attr & SL_SLOT_FLAG_CAN_READ_SAVE) == 0) {
        return slFileSel();
    }
    slVersionConvert_Step1();
    path[0] = '/';
    mcMakeFileName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = '/';
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    sdfMcFuncOpen(ctx, path, 1);
    return (s32)slLoadProc_openFile;
}

s32 slLoadProc_openFile(void) {
    slMcResult_t status = sdfMcFuncOpen1(&D_00551D50);
    u32 size;

    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    size = slVersionConvert_GetSize();
    slLoadFileFd = sdfMemAllocDirect(size);
    slLoadFileHandle = sdfMemLock(slLoadFileFd);
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncRead(D_00551D50, (u32)slLoadFileHandle, size);
        return (s32)slLoadProc_readFile;
    }
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

s32 slLoadProc_readFile(void) {
    slMcResult_t status = sdfMcFuncRead1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        sdfMcFuncClose(D_00551D50);
        return (s32)slLoadProc_closeFile;
    }
    sdfMemFreeDirect(slLoadFileFd);
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

s32 slLoadProc_closeFile(void) {
    slMcResult_t status = sdfMcFuncClose1();
    if (status == SL_MC_RESULT_PENDING) {
        return 0;
    }
    if (status == SL_MC_RESULT_SUCCESS) {
        slVersionConvert_Step3();
        sdfMemFreeDirect(slLoadFileFd);
        D_005516C1 = 1;
        slKillLoadProcess();
        if (dds3ConfigChanged() == 0) {
            dds3ConfigSaveBackup();
        } else {
            dds3ConfigRestoreBackup();
        }
        slSetInfo(13);
        return slHitAnyKeyNoChk(-1);
    }
    sdfMemFreeDirect(slLoadFileFd);
    slSetInfo(0);
    gslErrorCode = 3;
    return (s32)slErrorWait;
}

INCLUDE_ASM(const s32, "mc/slMain", slFileSelCalc);

INCLUDE_RODATA(const s32, "mc/slMain", D_005328C0);

INCLUDE_RODATA(const s32, "mc/slMain", D_005328D0);

INCLUDE_RODATA(const s32, "mc/slMain", D_005328E0);

INCLUDE_RODATA(const s32, "mc/slMain", D_005328F0);

INCLUDE_ASM(const s32, "mc/slMain", slFileSelDraw);

s32 slCalc(dds3ProcessID_t *task) {
    s32 next;
    u32 job;
    s32 (*cur)();
    gslAnimCnt++;
    slDraw((s32)task);
    next = proc_func_ans(task);
    if (next == -1) {
        return next;
    }
    job = baseIconFileHandle;
    cur = proc_func_ans;
    if (next != 0) {
        cur = (s32 (*)())next;
    }
    proc_func_ans = cur;
    if (job != 0 && pacFileCheckEnd((fldFileRequest_t *)job) != 0) {
        baseIconFileHandle = 0;
        D_00551D44 = fileGetMemHandle((fldFileRequest_t *)(u32)job);
        D_00551D48 = (u32)fileGetLoadPtr((fldFileRequest_t *)(u32)job);
        D_00551D4C = fileGetLoadSize((fldFileRequest_t *)(u32)job);
        fileHandleFree((fldFileRequest_t *)(u32)job);
    }
    return 0;
}

INCLUDE_ASM(const s32, "mc/slMain", slDraw);

INCLUDE_ASM(const s32, "mc/slMain", fmProcTrigSequence);

INCLUDE_ASM(const s32, "mc/slMain", slShutDownProcess);

u32 fmShutDown2(void) {
    return 1;
}

s32 fmProcessChk(void) {
    return dds3SearchProcessName(D_00532948) != 0;
}

u32 slGetExitType(void) {
    return gslExitType;
}

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoProc);

s32 slQueryYesNo(void *start, void *finish, s32 mode) {
    slQueryYesFunc = start;
    slQueryNoFunc = finish;
    slQueryYesNoCursor = mode;
    slQueryYesNoKeyFlag = 0;
    if (slQueryYesNoMsg != 4 && slQueryYesNoMsg != 8) {
        sdfMcRegetInfo(D_005516BC);
    }
    return (s32)slQueryYesNoProc;
}

INCLUDE_RODATA(const s32, "mc/slMain", D_00532948);

INCLUDE_ASM(const s32, "mc/slMain", slQueryYesNoDraw);

u32 slGetExitMode(void) {
    return gSlExitMode;
}

void slSetTmnlID(s16 x, s16 y) {
    dds3GlobalWork_t *state = DDS3_GBWK;

    state->previewX = x;
    state->previewY = y;
}
